#include "search.h"
#include "move.h"
#include <algorithm>
#include <cmath>
#include <unordered_set>

namespace rpsc {
namespace {constexpr int INF=1000000000;bool skipped(const Move&m,const std::vector<Move>&xs){for(auto&x:xs)if(same_move(m,x))return true;return false;} }
bool Searcher::timed_out(){nodes_++;if((nodes_&255)==0&&Clock::now()>=deadline_)stop_=true;return stop_;}
int Searcher::qsearch(Position&p,int a,int b,int ply){sel_=std::max(sel_,ply);if(p.remaining_plies==0||ply>=64||timed_out())return evaluate(p);int st=evaluate(p);if(st>=b)return b;if(st>a)a=st;auto ms=generate_moves(p,true);std::sort(ms.begin(),ms.end(),[](auto&A,auto&B){return (A.tactical*100000+(A.reset?1000:0))>(B.tactical*100000+(B.reset?1000:0));});for(auto&x:ms){if(x.tactical<0&&!x.reset)continue;Position n=p;if(!n.apply(x.move))continue;int sc=-qsearch(n,-b,-a,ply+1);if(stop_)return a;if(sc>=b)return b;if(sc>a)a=sc;}return a;}
int Searcher::alphabeta(Position&p,int d,int a,int b,int ply,bool pv,int ext_used){sel_=std::max(sel_,ply);if(p.remaining_plies==0||ply>=64)return evaluate(p);if(d<=0)return qsearch(p,a,b,ply);if(timed_out())return evaluate(p);int st=evaluate(p);if(!pv&&d<=4){if(st-(60+30*d)>=b)return st;if(d==1&&st+70<=a)return qsearch(p,a,b,ply);}auto k=p.key();const TTEntry*e=tt_.probe(k);int oa=a,ob=b;if(e&&e->depth>=d&&!pv){if(e->bound==0)return e->value;if(e->bound==1&&e->value>=b)return e->value;if(e->bound==-1&&e->value<=a)return e->value;}
 auto ms=generate_moves(p,false);if(ms.empty())return evaluate(p);Move ttmove{};bool has_tt=e&&e->has_best; if(has_tt)ttmove=e->best;std::sort(ms.begin(),ms.end(),[&](auto&A,auto&B){auto score=[&](auto&x){int s=x.tactical>0?500000+x.tactical*10000:x.tactical<0?-250000:0;if(x.reset)s+=180000;if(has_tt&&same_move(x.move,ttmove))s+=1000000;if(x.move.item!=Item::None)s+=500;return s;};return score(A)>score(B);});Move best{};bool hb=false;int j=0;for(auto&x:ms){Position n=p;int alive0=p.alive_total();if(!n.apply(x.move))continue;int alive1=n.alive_total();bool transition=x.tactical!=0&&alive0>2&&alive1==2;int extra=(x.tactical!=0&&ext_used<1)?1:0;if((x.reset||transition)&&ext_used+extra<2)extra++;int ne=std::min(2,ext_used+extra),cd=d-1+extra,sc; if(j==0)sc=-alphabeta(n,cd,-b,-a,ply+1,pv,ne);else{bool critical=x.tactical!=0||x.reset||transition;bool reduce=!pv&&!critical&&d>=4&&j>=4;int r=(reduce&&d>=7&&j>=10&&x.move.item==Item::None)?2:1;if(reduce){sc=-alphabeta(n,std::max(0,cd-r),-a-1,-a,ply+1,false,ne);if(sc>a)sc=-alphabeta(n,cd,-a-1,-a,ply+1,false,ne);}else sc=-alphabeta(n,cd,-a-1,-a,ply+1,false,ne);if(sc>a&&sc<b)sc=-alphabeta(n,cd,-b,-a,ply+1,pv,ne);}j++;if(stop_)return a;if(sc>a){a=sc;best=x.move;hb=true;}if(a>=b)break;}
 TTEntry ne;ne.depth=d;ne.value=a;ne.bound=a<=oa?-1:a>=ob?1:0;ne.best=best;ne.has_best=hb;tt_.store(k,ne);return a;}
Searcher::RootPass Searcher::root_pass(const Position&p,const std::vector<ScoredMove>&roots,const Move*preferred,int d,int a,int b,const std::vector<Move>&skip,bool multipv){std::vector<ScoredMove>ord;for(auto&x:roots)if(!skipped(x.move,skip))ord.push_back(x);std::sort(ord.begin(),ord.end(),[&](auto&A,auto&B){auto s=[&](auto&x){int v=x.tactical>0?500000+x.tactical*10000:x.tactical<0?-250000:0;if(x.reset)v+=180000;if(preferred&&same_move(x.move,*preferred))v+=1200000;return v;};return s(A)>s(B);});RootPass r;if(ord.empty())return r;int alpha=a,j=0;for(auto&x:ord){Position n=p;int alive0=p.alive_total();n.apply(x.move);int alive1=n.alive_total();bool transition=x.tactical!=0&&alive0>2&&alive1==2;int extra=x.tactical!=0?1:0;if((x.reset||transition)&&extra<2)extra++;int cd=d-1+extra,v;if(j==0)v=-alphabeta(n,cd,-b,-alpha,1,true,extra);else{bool reduce=!multipv&&d>=3&&x.tactical==0&&j>=64;if(reduce){v=-alphabeta(n,std::max(0,cd-1),-alpha-1,-alpha,1,false,extra);if(v>alpha)v=-alphabeta(n,cd,-alpha-1,-alpha,1,false,extra);}else v=-alphabeta(n,cd,-alpha-1,-alpha,1,false,extra);if(v>alpha&&v<b)v=-alphabeta(n,cd,-b,-alpha,1,true,extra);}j++;if(stop_)break;r.scores.push_back({x.move,v});if(!r.has_best||v>r.value){r.has_best=true;r.best=x.move;r.value=v;}if(v>alpha)alpha=v;if(alpha>=b)break;}return r;}
std::vector<Move> Searcher::pvline(Position p,const Move&best,int d){std::vector<Move>out;Move m=best;bool has=true;for(int i=0;i<std::min(64,d+4)&&has;i++){out.push_back(m);p.apply(m);auto*e=tt_.probe(p.key());has=e&&e->has_best;if(has)m=e->best;}return out;}
SearchResult Searcher::search(const Position& root, const SearchLimits& lim) {
  auto start = Clock::now();
  nodes_ = 0; sel_ = 0; stop_ = false; tt_.clear();
  int total = std::max(20, lim.movetime_ms);
  int k = std::clamp(lim.multipv, 1, 3);
  auto hard_end = start + std::chrono::milliseconds(total);
  auto broad_end = start + std::chrono::milliseconds((int)(total * 0.20));
  auto primary_end = start + std::chrono::milliseconds((int)(total * 0.70));
  auto secondary_end = start + std::chrono::milliseconds((int)(total * 0.90));

  auto roots = generate_moves(root, false);
  SearchResult res;
  if (roots.empty()) return res;
  Move prev = roots[0].move;
  int prev_value = 0;
  std::vector<std::pair<Move,int>> global_ranking;

  auto merge_ranking = [&](const std::vector<std::pair<Move,int>>& fresh) {
    for (auto& rv : fresh) {
      auto it = std::find_if(global_ranking.begin(), global_ranking.end(), [&](auto& q){return same_move(q.first, rv.first);});
      if (it == global_ranking.end()) global_ranking.push_back(rv); else it->second = rv.second;
    }
    std::sort(global_ranking.begin(), global_ranking.end(), [](auto&A,auto&B){return A.second>B.second;});
  };
  auto publish = [&](const RootPass& pass, int depth, bool merge) {
    auto ranking = pass.scores;
    std::sort(ranking.begin(), ranking.end(), [](auto&A,auto&B){return A.second>B.second;});
    if (!merge) global_ranking = ranking; else merge_ranking(ranking);
    res.has_best = true; res.best = pass.best; res.value = pass.value; res.depth = depth;
    res.seldepth = sel_; res.nodes = nodes_; res.candidates.clear();
    res.candidates.push_back({pass.best, pass.value, depth, pvline(root, pass.best, depth)});
    for (auto& rv : global_ranking) {
      if ((int)res.candidates.size() >= k) break;
      if (same_move(rv.first, pass.best)) continue;
      res.candidates.push_back({rv.first, rv.second, std::max(1, depth-1), pvline(root, rv.first, std::max(1, depth-1))});
    }
    prev = pass.best; prev_value = pass.value;
  };
  auto search_pass = [&](const std::vector<ScoredMove>& pool, int depth, Clock::time_point end, bool merge) {
    deadline_ = end; stop_ = false;
    int w=50, a=depth>=4?prev_value-w:-INF, b=depth>=4?prev_value+w:INF;
    RootPass pass;
    while (true) {
      pass = root_pass(root, pool, &prev, depth, a, b, {}, false);
      if (stop_ || !pass.has_best) break;
      if (pass.value <= a) { w*=2; a=prev_value-w; b=prev_value+w; continue; }
      if (pass.value >= b) { w*=2; a=prev_value-w; b=prev_value+w; continue; }
      break;
    }
    if (stop_ || !pass.has_best) { stop_=false; return false; }
    publish(pass, depth, merge); return true;
  };
  auto add_root = [&](std::vector<ScoredMove>& pool, const ScoredMove* r) {
    if (!r) return;
    for (auto& q:pool) if (same_move(q.move,r->move)) return;
    pool.push_back(*r);
  };
  auto find_root = [&](const Move& m)->const ScoredMove* {
    for (auto& r:roots) if (same_move(r.move,m)) return &r;
    return nullptr;
  };
  auto champ_pool = [&]() {
    std::vector<ScoredMove> pool;
    for (auto& c:res.candidates) add_root(pool,find_root(c.move));
    for (auto& rv:global_ranking) { if (pool.size()>=4) break; add_root(pool,find_root(rv.first)); }
    for (auto& rv:global_ranking) { auto*r=find_root(rv.first); if (r&&r->tactical==0&&r->move.item==Item::None){add_root(pool,r);break;} }
    for (auto& rv:global_ranking) { auto*r=find_root(rv.first); if (r&&(r->tactical!=0||r->reset||r->move.item!=Item::None)){add_root(pool,r);break;} }
    for (auto& rv:global_ranking) { if (pool.size()>=std::min<size_t>(6,roots.size())) break; add_root(pool,find_root(rv.first)); }
    return pool;
  };

  // 0-20% broad search.
  for (int d=1; d<=lim.depth && Clock::now()<broad_end; ++d) {
    if (!search_pass(roots,d,broad_end,false)) break;
    if (root.remaining_plies>0 && d>=root.remaining_plies) break;
  }

  // 20-70% primary-biased championship over at most six candidates.
  auto champ = champ_pool(); if (champ.empty()) champ=roots;
  for (int d=std::max(1,res.depth+1); d<=lim.depth && Clock::now()<primary_end; ++d) {
    if (!search_pass(champ,d,primary_end,true)) break;
    champ=champ_pool();
    if (root.remaining_plies>0 && d>=root.remaining_plies) break;
  }

  // 70-90% Top-3 stabilization, allowing #2/#3 to trail primary by two depths.
  if (k>1 && res.candidates.size()>=2 && Clock::now()<secondary_end) {
    deadline_=secondary_end; stop_=false; int sd=std::max(1,res.depth-2);
    auto pool=champ_pool(); std::vector<Move> skip{res.best}; std::vector<Candidate> verified;
    for (int rank=1; rank<k && Clock::now()<secondary_end; ++rank) {
      const Move* pref=rank<(int)res.candidates.size()?&res.candidates[rank].move:nullptr;
      auto nx=root_pass(root,pool,pref,sd,-INF,INF,skip,true);
      if (stop_||!nx.has_best) break;
      verified.push_back({nx.best,nx.value,sd,pvline(root,nx.best,sd)}); skip.push_back(nx.best);
    }
    if (!stop_&&!verified.empty()) {
      std::sort(verified.begin(),verified.end(),[](auto&A,auto&B){return A.value>B.value;});
      std::vector<Candidate> out{res.candidates[0]};
      for(auto&x:verified)if((int)out.size()<k)out.push_back(x);
      for(size_t i=1;i<res.candidates.size()&&(int)out.size()<k;i++){bool dup=false;for(auto&x:out)dup|=same_move(x.move,res.candidates[i].move);if(!dup)out.push_back(res.candidates[i]);}
      res.candidates=std::move(out);
    }
    stop_=false;
  }

  // 90-100% final verification: #1 only for clear gaps, otherwise #1/#2 = 60/40 or 50/50.
  if (res.has_best && Clock::now()<hard_end && !res.candidates.empty()) {
    int gap=res.candidates.size()>1?res.candidates[0].value-res.candidates[1].value:INF;
    double share1=gap<15?0.50:gap<40?0.60:1.0;
    int count=(share1<1.0&&res.candidates.size()>1)?2:1;
    auto final_start=Clock::now(); auto remain=hard_end-final_start; std::vector<Candidate> verified;
    for(int i=0;i<count;i++){
      auto*r=find_root(res.candidates[i].move); if(!r)continue;
      auto end=i==0?final_start+std::chrono::duration_cast<Clock::duration>(remain*share1):hard_end;
      deadline_=end;stop_=false;int vd=std::min(lim.depth,res.depth+1);
      auto vr=root_pass(root,std::vector<ScoredMove>{*r},&res.candidates[i].move,vd,-INF,INF,{},false);
      if(!stop_&&vr.has_best)verified.push_back({vr.best,vr.value,vd,pvline(root,vr.best,vd)});
      stop_=false;
    }
    if(!verified.empty()){
      std::vector<Candidate> all=verified;
      for(auto&c:res.candidates){bool dup=false;for(auto&v:verified)dup|=same_move(v.move,c.move);if(!dup)all.push_back(c);}
      std::sort(all.begin(),all.end(),[](auto&A,auto&B){return A.value>B.value;});
      res.candidates.assign(all.begin(),all.begin()+std::min<size_t>(k,all.size()));res.best=res.candidates[0].move;res.value=res.candidates[0].value;res.depth=std::max(res.depth,res.candidates[0].depth);
    }
  }
  res.nodes=nodes_;res.seldepth=sel_;res.elapsed_ms=(int)std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now()-start).count();
  return res;
}

} // namespace rpsc
