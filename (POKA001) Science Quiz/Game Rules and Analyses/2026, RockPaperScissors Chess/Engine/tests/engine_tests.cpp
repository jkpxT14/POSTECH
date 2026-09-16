#include "engine.h"
#include "movegen.h"
#include "orientation.h"
#include "perft.h"
#include "position.h"
#include "types.h"
#include <cassert>
#include <iostream>

using namespace rpsc;
int main(){
  const auto&o=OrientationTable::instance();
  assert(o.size()==24);
  assert(fight(Gesture::Rock,Gesture::Scissors)==1);
  assert(fight(Gesture::Scissors,Gesture::Paper)==1);
  assert(fight(Gesture::Paper,Gesture::Rock)==1);
  Position p;
  assert(p.alive_count(Side::White)==4&&p.alive_count(Side::Black)==4);
  auto ms=generate_moves(p,false);assert(!ms.empty());
  // Push-return regression: create a Push item and require at least one legal path whose first Roll returns to the pre-Push square.
  p.items[0].push=1;auto pm=generate_moves(p,false);bool push_return=false;for(auto&x:pm){auto&m=x.move;if(m.item==Item::Push&&m.path.size()>1&&m.path[1].first==p.pieces[m.piece].x&&m.path[1].second==p.pieces[m.piece].y){push_return=true;break;}}assert(push_return);
  // W/B are board roles only. Quiz stream is not encoded as P/K anywhere in the native Position.
  p.quiz[0]=3;p.quiz[1]=1;assert(p.score(Side::White)==3&&p.score(Side::Black)==1);
  SearchLimits lim;lim.movetime_ms=40;lim.multipv=3;Engine e;auto r=e.analyze(p,lim);assert(r.has_best);assert(!r.candidates.empty()&&r.candidates.size()<=3);
  std::cout<<"RPSC Engine 0.24.0 self-test passed; legal roots="<<pm.size()<<", search depth="<<r.depth<<"\n";
  return 0;
}
