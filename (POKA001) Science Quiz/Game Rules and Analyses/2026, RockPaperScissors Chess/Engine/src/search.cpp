#include "search.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <unordered_map>
#include <vector>

namespace rpsc {
namespace {
constexpr int MaxPly = 64;
constexpr int MaxSelectiveExtensions = 2;
constexpr Value AspirationWindow = 50;
constexpr int CountermoveBonus = 120000;
constexpr int ContinuationScale = 2;
constexpr int FollowupScale = 4;
constexpr int CaptureHistoryScale = 4;
constexpr std::size_t ItemActionCount = 6;
constexpr std::size_t HistoryMoveSlots = PieceCount * 64u * ItemActionCount;

int root_action_family(const Move& move) {
    if (move.item == Item::Push) return 1;
    if (move.item == Item::RotateLeft || move.item == Item::RotateRight) return 2;
    if (move.item == Item::StepShort || move.item == Item::StepLong) return 3;
    return 0;
}
struct OrderedMove { Move move{}; int score=0; int capture_swing=0; };
std::size_t move_slot(const Move& move) {
    const std::size_t base = static_cast<std::size_t>(piece_index(move.piece))*64u + static_cast<unsigned>(move.to());
    return base*ItemActionCount + static_cast<unsigned>(move.item);
}
std::uint64_t continuation_key(const Move& previous,const Move& current) {
    return static_cast<std::uint64_t>(move_slot(previous))*HistoryMoveSlots + move_slot(current);
}
bool is_killer(const Move& move,const std::array<Move,2>& killers,int index) {
    return index>=0 && index<2 && killers[index].path_length!=0 && move==killers[index];
}
bool is_excluded(const Move& move,const std::vector<Move>& excluded) {
    return std::any_of(excluded.begin(),excluded.end(),[&](const Move& other){return move==other;});
}
void bounded_add(int& value,int delta) {
    value += delta;
    if (value>100000 || value<-100000) value/=2;
}
}  // namespace

struct Search::Context {
    SearchLimits limits; std::chrono::steady_clock::time_point start; std::uint64_t nodes=0; Depth seldepth=0; bool stopped=false;
    std::vector<int>& history; std::vector<int>& capture_history;
    std::array<std::array<Move,2>,MaxPly> killers{};
    std::unordered_map<std::size_t,Move>& countermoves;
    std::unordered_map<std::uint64_t,int>& continuation;
    std::unordered_map<std::uint64_t,int>& followup;
    std::unordered_map<Key,std::uint8_t> pressure_cache;
    explicit Context(Search& s):history(s.history_),capture_history(s.capture_history_),countermoves(s.countermoves_),continuation(s.continuation_),followup(s.followup_) {}
    bool should_stop(){
        if(stopped)return true;
        if(limits.nodes && nodes>=limits.nodes)return stopped=true;
        if(limits.movetime.count()>0 && (nodes&63ULL)==0 && std::chrono::steady_clock::now()-start>=limits.movetime)return stopped=true;
        return false;
    }
    int sparse_score(const std::unordered_map<std::uint64_t,int>& table,std::uint64_t key)const{auto f=table.find(key);return f==table.end()?0:f->second;}
    void bounded_sparse_add(std::unordered_map<std::uint64_t,int>& table,std::uint64_t key,int delta){int&v=table[key];bounded_add(v,delta);if(table.size()>250000)table.clear();}
    int history_score(const Move&m)const{return history[move_slot(m)];}
    int capture_history_score(const Move&m)const{return capture_history[move_slot(m)]/CaptureHistoryScale;}
    int continuation_score(const Move*prev,const Move*prev2,const Move&m)const{
        int s=0;if(prev&&prev->path_length)s+=sparse_score(continuation,continuation_key(*prev,m))/ContinuationScale;
        if(prev2&&prev2->path_length) s+=sparse_score(followup,continuation_key(*prev2,m))/FollowupScale;
        return s;
    }
    bool is_countermove(const Move*prev,const Move&m)const{if(!prev||!prev->path_length)return false;auto f=countermoves.find(move_slot(*prev));return f!=countermoves.end()&&f->second==m;}
    void record_quiet_cutoff(const Move&m,Depth depth,int ply,const Move*prev,const Move*prev2,const std::vector<Move>& quiets){
        int bonus=std::max(1,depth*depth);bounded_add(history[move_slot(m)],bonus);
        if(prev&&prev->path_length){bounded_sparse_add(continuation,continuation_key(*prev,m),2*bonus);countermoves[move_slot(*prev)]=m;if(countermoves.size()>16384)countermoves.clear();}
        if(prev2&&prev2->path_length)bounded_sparse_add(followup,continuation_key(*prev2,m),bonus);
        int penalty=std::max(1,bonus/2);for(const auto&t:quiets){if(t==m)continue;bounded_add(history[move_slot(t)],-penalty);if(prev&&prev->path_length)bounded_sparse_add(continuation,continuation_key(*prev,t),-penalty);if(prev2&&prev2->path_length)bounded_sparse_add(followup,continuation_key(*prev2,t),-std::max(1,penalty/2));}
        if(ply>=0&&ply<MaxPly&&killers[ply][0]!=m){killers[ply][1]=killers[ply][0];killers[ply][0]=m;}
    }
    void record_tactical_cutoff(const Move&m,Depth depth){bounded_add(capture_history[move_slot(m)],std::max(1,depth*depth*2));}
    int capture_pressure(Position& p){Key k=p.search_key();auto f=pressure_cache.find(k);if(f!=pressure_cache.end())return f->second;int pressure=0;for(const auto&m:generate_tactical_moves_info(p))if(m.capture_swing>0&&++pressure>=4)break;if(pressure_cache.size()>50000)pressure_cache.clear();pressure_cache.emplace(k,static_cast<std::uint8_t>(pressure));return pressure;}
    int capture_pressure_for(const Position&p,Color side){Position q=p;q.set_side_to_move(side);return capture_pressure(q);}
};

Search::Search(TranspositionTable& tt):tt_(tt),history_(HistoryMoveSlots,0),capture_history_(HistoryMoveSlots,0){}
void Search::clear_memory(){std::fill(history_.begin(),history_.end(),0);std::fill(capture_history_.begin(),capture_history_.end(),0);countermoves_.clear();continuation_.clear();followup_.clear();root_cache_.clear();root_depth_cache_.clear();}

Value Search::quiescence(Position&p,Value alpha,Value beta,int ply,Context&c){
    if (p.remaining_board_plies()==0) return evaluate(p);
    ++c.nodes; c.seldepth=std::max(c.seldepth,ply);
    if (ply>=MaxPly||c.should_stop()) return evaluate(p);
    Value stand=evaluate(p);if(stand>=beta)return beta;if(stand>alpha)alpha=stand;
    auto info=generate_tactical_moves_info(p);std::vector<OrderedMove> ordered;ordered.reserve(info.size());
    for(const auto&e:info)ordered.push_back({e.move,e.capture_swing*100000+c.capture_history_score(e.move),e.capture_swing});
    std::stable_sort(ordered.begin(),ordered.end(),[](const auto&a,const auto&b){return a.score>b.score;});
    for(const auto&cur:ordered){UndoState u;p.do_move(cur.move,u);Value score=-quiescence(p,-beta,-alpha,ply+1,c);p.undo_move(u);if(c.stopped)return alpha;if(score>=beta){c.record_tactical_cutoff(cur.move,1);return beta;}if(score>alpha)alpha=score;}
    return alpha;
}

Value Search::negamax(Position&p,Depth depth,Value alpha,Value beta,int ply,Context&c,bool pv_node,const Move*prev,const Move*prev2,int extensions_used){
    if (p.remaining_board_plies()==0) return evaluate(p);
    if (ply>=MaxPly) return evaluate(p);
    if (depth<=0) return quiescence(p,alpha,beta,ply,c);
    ++c.nodes;c.seldepth=std::max(c.seldepth,ply);if(c.should_stop())return evaluate(p);
    const Value original_alpha=alpha,original_beta=beta,static_eval=evaluate(p);const Key key=p.search_key();
    const TTEntry* entry=tt_.probe(key);Move tt_move{};const bool has_tt=entry&&entry->has_move;if(has_tt)tt_move=entry->best_move;
    if(entry&&entry->depth>=depth&&!pv_node){if(entry->bound==Bound::Exact)return entry->value;if(entry->bound==Bound::Lower&&entry->value>=beta)return entry->value;if(entry->bound==Bound::Upper&&entry->value<=alpha)return entry->value;}
    const Color mover=p.side_to_move();Move best{};bool has_best=false;int move_index=0;std::vector<Move> quiets;quiets.reserve(16);bool searched_tt=false;
    if(has_tt&&p.is_legal_path(tt_move)){
        int own=p.captures(mover),opp=p.captures(opposite(mover));UndoState u;p.do_move(tt_move,u);int swing=(p.captures(mover)-own)-(p.captures(opposite(mover))-opp);
        bool extend=swing!=0&&extensions_used<MaxSelectiveExtensions;Value score=-negamax(p,depth-1+(extend?1:0),-beta,-alpha,ply+1,c,pv_node,&tt_move,prev,extensions_used+(extend?1:0));p.undo_move(u);
        searched_tt=true;move_index=1;if(c.stopped)return alpha;if(swing==0)quiets.push_back(tt_move);if(score>alpha){alpha=score;best=tt_move;has_best=true;}if(alpha>=beta){if(swing==0)c.record_quiet_cutoff(tt_move,depth,ply,prev,prev2,quiets);else c.record_tactical_cutoff(tt_move,depth);tt_.store(key,depth,alpha,Bound::Lower,&tt_move);return alpha;}
    }
    const auto info=generate_search_moves_info(p);if(info.empty()){if(has_best){tt_.store(key,depth,alpha,Bound::Exact,&best);return alpha;}return evaluate(p);}
    int own_pressure=0;for(const auto&e:info)if(e.capture_swing>0&&++own_pressure>=4)break;
    const bool inspect=depth>=4;const int opponent_pressure=inspect?c.capture_pressure_for(p,opposite(mover)):0;
    std::vector<OrderedMove> ordered;ordered.reserve(info.size());
    for(const auto&e:info){const Move&m=e.move;if(searched_tt&&m==tt_move)continue;int s=c.history_score(m)+c.continuation_score(prev,prev2,m);if(c.is_countermove(prev,m))s+=CountermoveBonus;if(e.capture_swing>0)s+=500000+e.capture_swing*10000+c.capture_history_score(m);else if(e.capture_swing<0)s+=-250000+c.capture_history_score(m);if(ply<MaxPly&&is_killer(m,c.killers[ply],0))s+=200000;else if(ply<MaxPly&&is_killer(m,c.killers[ply],1))s+=150000;ordered.push_back({m,s,e.capture_swing});}
    std::sort(ordered.begin(),ordered.end(),[](const auto&a,const auto&b){return a.score>b.score;});
    for(const auto&cur:ordered){UndoState u;p.do_move(cur.move,u);bool extend=cur.capture_swing!=0&&extensions_used<MaxSelectiveExtensions;int next_ext=extensions_used+(extend?1:0);Depth full=depth-1+(extend?1:0);Value score;
        if(move_index==0)score=-negamax(p,full,-beta,-alpha,ply+1,c,pv_node,&cur.move,prev,next_ext);
        else{
            int hist=c.history_score(cur.move)+c.continuation_score(prev,prev2,cur.move);bool known=c.is_countermove(prev,cur.move)||(ply<MaxPly&&(is_killer(cur.move,c.killers[ply],0)||is_killer(cur.move,c.killers[ply],1)))||hist>4*depth*depth;
            bool reducible=!pv_node&&cur.capture_swing==0&&depth>=4&&move_index>=4&&!known;int reduction=1;
            if(reducible){int opp_after=c.capture_pressure(p),own_after=c.capture_pressure_for(p,mover);bool defensive=opponent_pressure>0&&opp_after<opponent_pressure;bool threat=own_after>own_pressure;if(defensive||threat)reducible=false;if(reducible){Value child_static=-evaluate(p);bool improving=child_static>=static_eval+8;if(!improving&&cur.move.item==Item::None&&depth>=7&&move_index>=10&&hist<=0)reduction=2;}}
            if(reducible){Depth rd=std::max<Depth>(0,full-reduction);score=-negamax(p,rd,-alpha-1,-alpha,ply+1,c,false,&cur.move,prev,next_ext);if(score>alpha)score=-negamax(p,full,-alpha-1,-alpha,ply+1,c,false,&cur.move,prev,next_ext);}else score=-negamax(p,full,-alpha-1,-alpha,ply+1,c,false,&cur.move,prev,next_ext);
            if(score>alpha&&score<beta)score=-negamax(p,full,-beta,-alpha,ply+1,c,pv_node,&cur.move,prev,next_ext);
        }
        p.undo_move(u);++move_index;if(c.stopped)return alpha;if(cur.capture_swing==0)quiets.push_back(cur.move);if(score>alpha){alpha=score;best=cur.move;has_best=true;}if(alpha>=beta){if(cur.capture_swing==0)c.record_quiet_cutoff(cur.move,depth,ply,prev,prev2,quiets);else c.record_tactical_cutoff(cur.move,depth);break;}
    }
    Bound bound=Bound::Exact;if(alpha<=original_alpha)bound=Bound::Upper;else if(alpha>=original_beta)bound=Bound::Lower;tt_.store(key,depth,alpha,bound,has_best?&best:nullptr);return alpha;
}

SearchResult Search::run(Position root,const SearchLimits& limits){
    tt_.new_search();Context c(*this);c.limits=limits;c.limits.multipv=std::clamp(c.limits.multipv,1,8);c.start=std::chrono::steady_clock::now();c.pressure_cache.reserve(8192);
    SearchResult result;const auto root_info=generate_search_moves_info(root);if(root_info.empty())return result;
    std::vector<Move> root_moves;root_moves.reserve(root_info.size());std::unordered_map<Move,int,MoveHash> root_swing;root_swing.reserve(root_info.size()*2);
    for(const auto&e:root_info){root_moves.push_back(e.move);root_swing.emplace(e.move,e.capture_swing);}const Key root_key=root.search_key();
    std::vector<RootLine> prior;auto cached=root_cache_.find(root_key);if(cached!=root_cache_.end())prior=cached->second;Depth cached_depth=0;if(auto d=root_depth_cache_.find(root_key);d!=root_depth_cache_.end())cached_depth=d->second;
    std::unordered_map<Move,Value,MoveHash> prior_value;auto rebuild=[&]{prior_value.clear();prior_value.reserve(prior.size()*2+1);for(const auto&l:prior)prior_value[l.move]=l.value;};rebuild();
    Move previous_best=root_moves.front();Value previous_value=0;if(!prior.empty()){auto legal=std::find_if(prior.begin(),prior.end(),[&](const RootLine&l){return std::find(root_moves.begin(),root_moves.end(),l.move)!=root_moves.end();});if(legal!=prior.end()){previous_best=legal->move;previous_value=legal->value;}}
    // Reuse the previous completed root ordering on repeated timed analysis.
    if(limits.movetime.count()>0&&!prior.empty()&&cached_depth>0){result.has_move=true;result.best_move=previous_best;result.value=previous_value;result.depth=cached_depth;}
    auto build_pv=[&](const Move&first,Depth depth){std::vector<Move>pv;Position p=root;Move m=first;int max=std::min(MaxPly,depth+MaxSelectiveExtensions+2);for(int ply=0;ply<max&&m.path_length;++ply){if(!p.is_legal_path(m))break;pv.push_back(m);UndoState u;p.do_move(m,u);const TTEntry*e=tt_.probe(p.search_key());m=e&&e->has_move?e->best_move:Move{};}return pv;};
    struct RootPass{Move best{};Value value=-Infinity;std::vector<RootLine>scores;};
    auto search_root=[&](Depth depth,Value alpha,Value beta,const std::vector<Move>&excluded,const Move&preferred){
        std::vector<OrderedMove> ordered;ordered.reserve(root_moves.size());for(const auto&m:root_moves){if(is_excluded(m,excluded))continue;int swing=root_swing[m];int score=c.history_score(m);if(m==preferred)score+=1200000;if(auto f=prior_value.find(m);f!=prior_value.end())score+=1000*f->second;if(swing>0)score+=500000+swing*10000+c.capture_history_score(m);else if(swing<0)score+=-250000+c.capture_history_score(m);ordered.push_back({m,score,swing});}
        std::stable_sort(ordered.begin(),ordered.end(),[](const auto&a,const auto&b){return a.score>b.score;});RootPass pass;if(ordered.empty())return pass;pass.best=ordered.front().move;int move_index=0;Value current_alpha=alpha;std::array<int,4> family_seen{};
        for(const auto&cur:ordered){int fam=root_action_family(cur.move),fam_idx=family_seen[static_cast<std::size_t>(fam)]++;UndoState u;root.do_move(cur.move,u);bool extend=cur.capture_swing!=0;Depth child=depth-1+(extend?1:0);int ext=extend?1:0;Value score;
            if(move_index==0)score=-negamax(root,child,-beta,-current_alpha,1,c,true,&cur.move,nullptr,ext);
            else{Value old=-Infinity;if(auto f=prior_value.find(cur.move);f!=prior_value.end())old=f->second;int quota=fam==0?28:20;bool ranked_late=old>-Infinity&&old+12<previous_value;bool reduce=depth>=3&&cur.capture_swing==0&&fam_idx>=quota&&(ranked_late||move_index>=56);if(reduce){Depth rd=std::max<Depth>(0,child-1);score=-negamax(root,rd,-current_alpha-1,-current_alpha,1,c,false,&cur.move,nullptr,ext);if(score>current_alpha)score=-negamax(root,child,-current_alpha-1,-current_alpha,1,c,false,&cur.move,nullptr,ext);}else score=-negamax(root,child,-current_alpha-1,-current_alpha,1,c,false,&cur.move,nullptr,ext);if(score>current_alpha&&score<beta)score=-negamax(root,child,-beta,-current_alpha,1,c,true,&cur.move,nullptr,ext);}
            root.undo_move(u);++move_index;if(c.stopped)return pass;pass.scores.push_back({cur.move,score,{}});if(score>pass.value){pass.value=score;pass.best=cur.move;}if(score>current_alpha)current_alpha=score;if(current_alpha>=beta)break;
        }return pass;
    };
    Depth start_depth=(limits.movetime.count()>0&&cached_depth>0&&!prior.empty())?cached_depth:1;
    for(Depth depth=start_depth;depth<=std::max(1,limits.depth);++depth){Value alpha=-Infinity,beta=Infinity,window=AspirationWindow;if(depth>=4&&result.has_move){alpha=std::max(-Infinity,previous_value-window);beta=std::min(Infinity,previous_value+window);}RootPass pass;while(true){pass=search_root(depth,alpha,beta,{},previous_best);if(c.stopped)break;if(pass.value<=alpha&&alpha>-Infinity){window*=2;alpha=std::max(-Infinity,previous_value-window);beta=std::min(Infinity,previous_value+window);continue;}if(pass.value>=beta&&beta<Infinity){window*=2;alpha=std::max(-Infinity,previous_value-window);beta=std::min(Infinity,previous_value+window);continue;}break;}if(c.stopped)break;previous_best=pass.best;previous_value=pass.value;prior=std::move(pass.scores);std::stable_sort(prior.begin(),prior.end(),[](const auto&a,const auto&b){return a.value>b.value;});rebuild();result.has_move=true;result.best_move=previous_best;result.value=previous_value;result.depth=depth;result.pv=build_pv(previous_best,depth);root_cache_[root_key]=prior;root_depth_cache_[root_key]=depth;}
    if(result.has_move){int wanted=std::min<int>(c.limits.multipv,prior.size());for(int rank=0;rank<wanted;++rank){RootLine line=prior[static_cast<std::size_t>(rank)];line.pv=build_pv(line.move,result.depth);result.lines.push_back(std::move(line));}if(!result.lines.empty()){result.best_move=result.lines.front().move;result.value=result.lines.front().value;result.pv=result.lines.front().pv;}
        if(!c.stopped&&wanted>1){std::vector<Move>excluded{result.best_move};for(int rank=1;rank<wanted&&!c.stopped;++rank){Move pref=result.lines[static_cast<std::size_t>(rank)].move;RootPass pass=search_root(result.depth,-Infinity,Infinity,excluded,pref);if(c.stopped||pass.value<=-Infinity||pass.best.path_length==0)break;result.lines[static_cast<std::size_t>(rank)]={pass.best,pass.value,build_pv(pass.best,result.depth)};excluded.push_back(pass.best);}}
    }
    if(root_cache_.size()>256){root_cache_.clear();root_depth_cache_.clear();}
    result.nodes=c.nodes;result.seldepth=c.seldepth;result.elapsed=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-c.start);return result;
}
}  // namespace rpsc
