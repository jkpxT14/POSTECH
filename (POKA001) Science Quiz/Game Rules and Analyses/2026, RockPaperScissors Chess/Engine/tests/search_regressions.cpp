#ifdef NDEBUG
#undef NDEBUG
#endif
#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include "engine.h"
using namespace rpsc;
static Value exact(Position& p){ if(p.remaining_board_plies()==0)return evaluate(p); auto ms=generate_search_moves(p); if(ms.empty())return evaluate(p); Value b=-Infinity; for(auto&m:ms){UndoState u;p.do_move(m,u);b=std::max(b,-exact(p));p.undo_move(u);}return b; }
int main(){std::mt19937 rng(20260910);int checks=0;for(int sample=0;sample<8;++sample){Position p;p.set_items(Color::White,sample%2,1,1);p.set_items(Color::Black,1,sample%2,1);for(int j=0;j<2;++j){auto ms=generate_search_moves(p);UndoState u;p.do_move(ms[rng()%ms.size()],u);}p.set_match_context(sample%3,1,2);auto ms=generate_search_moves(p);std::vector<Value> v;for(auto&m:ms){UndoState u;p.do_move(m,u);v.push_back(-exact(p));p.undo_move(u);}std::sort(v.begin(),v.end(),std::greater<Value>());Engine e;e.position()=p;SearchLimits l;l.depth=1;l.multipv=3;auto r=e.go(l);assert(r.lines.size()==3);for(int k=0;k<3;++k){assert(r.lines[k].value==v[k]);++checks;}}std::cout<<"Endgame checks: "<<checks<<"\n";}
