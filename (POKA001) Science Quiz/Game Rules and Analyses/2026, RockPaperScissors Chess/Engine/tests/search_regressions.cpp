#ifdef NDEBUG
#undef NDEBUG
#endif
#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include "engine.h"
using namespace rpsc;
// Independent finite-horizon minimax: no stand-pat, TT, reductions or heuristic leaves.
static Value exact(Position& p) {
    if(p.remaining_board_plies()==0) return evaluate(p);
    auto moves=generate_search_moves(p);
    if(moves.empty()) return evaluate(p);
    Value best=-Infinity;
    for(auto& m:moves){UndoState u;p.do_move(m,u);best=std::max(best,-exact(p));p.undo_move(u);}
    return best;
}
int main(int argc,char**) {
    std::mt19937 rng(20260909);Position p;int mismatches=0,checks=0;
    for(int sample=0;sample<12;++sample){
        p.set_items(Color::White,sample%2,1,1);p.set_items(Color::Black,1,sample%2,1);
        for(int j=0;j<3;++j){auto ms=generate_search_moves(p);if(ms.empty())break;UndoState u;p.do_move(ms[rng()%ms.size()],u);}
        // Use match-ending positions with both quiet and tactical outcomes.
        Position end=p;end.set_match_context(sample%3,1,2);
        auto moves=generate_search_moves(end);std::vector<Value> values;
        for(auto& m:moves){UndoState u;end.do_move(m,u);values.push_back(-exact(end));end.undo_move(u);}
        std::sort(values.begin(),values.end(),std::greater<Value>());
        Engine engine;engine.position()=end;SearchLimits lim;lim.depth=1;lim.multipv=3;
        auto r=engine.go(lim);assert(r.depth==1&&r.lines.size()==3);
        for(int k=0;k<3;++k){++checks;if(r.lines[k].value!=values[k])++mismatches;
            Position q=end;UndoState u;q.do_move(r.lines[k].move,u);if(-exact(q)!=r.lines[k].value)++mismatches;
            for(int j=0;j<k;++j)assert(r.lines[k].move!=r.lines[j].move);
        }
        assert(engine.position().key()==end.key());
    }
    // A finished board phase must not return another move.
    Engine done;done.position().set_match_context(10,9,0);SearchLimits lim;lim.depth=1;
    if(done.go(lim).has_move)++mismatches;
    // If a new iteration is interrupted, keep the last complete MultiPV set.
    Engine timed;SearchLimits t;t.depth=1;t.multipv=3;auto first=timed.go(t);
    t.depth=64;t.movetime=std::chrono::milliseconds(1);auto next=timed.go(t);
    assert(next.depth>=first.depth&&next.lines.size()==3);
    if(argc==1&&next.depth==first.depth)for(int k=0;k<3;++k)assert(next.lines[k].value==first.lines[k].value);
    std::cout<<"Endgame top-three checks: "<<checks<<"; mismatches: "<<mismatches<<"\n";
    return argc>1?0:(mismatches?1:0);
}
