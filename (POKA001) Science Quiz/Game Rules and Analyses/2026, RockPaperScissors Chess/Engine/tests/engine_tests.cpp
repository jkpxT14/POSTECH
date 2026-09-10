#ifdef NDEBUG
#undef NDEBUG
#endif
#include <cassert>
#include <iostream>
#include <unordered_set>
#include "engine.h"
#include "evaluate.h"
#include "movegen.h"
#include "notation.h"
#include "orientation.h"
#include "perft.h"
using namespace rpsc;

static void verify_generator(Position& p) {
    std::unordered_set<Key> a,b;
    for (const auto& m: generate_legal_moves(p)) { UndoState u; p.do_move(m,u); a.insert(p.search_key()); p.undo_move(u); }
    for (const auto& e: generate_search_moves_info(p)) { assert(p.is_legal_path(e.move)); UndoState u; p.do_move(e.move,u); b.insert(p.search_key()); p.undo_move(u); }
    assert(a==b);
}
static bool distinct(const std::vector<RootLine>& lines) {
    for(size_t i=0;i<lines.size();++i) for(size_t j=i+1;j<lines.size();++j) if(lines[i].move==lines[j].move) return false;
    return true;
}
int main(){
    const auto& t=OrientationTable::instance(); assert(t.size()==24);
    for(int i=0;i<24;++i){auto o=Orientation(i); assert(t.apply_rotation(t.apply_rotation(o,Item::RotateNorth),Item::RotateSouth)==o); assert(t.apply_rotation(t.apply_rotation(o,Item::RotateEast),Item::RotateWest)==o); assert(t.apply_rotation(t.apply_rotation(o,Item::RotateLeft),Item::RotateRight)==o);}
    Position p; assert(generate_legal_moves(p).size()==161); assert(generate_unique_moves(p).size()==145); assert(generate_search_moves(p).size()==84); assert(perft(p,1)==161); assert(perft(p,2)==25575); assert(perft(p,3)==4215782); verify_generator(p);
    p.set_items(Color::White,1,1,1); p.set_items(Color::Black,1,1,1); assert(generate_legal_moves(p).size()==2146); assert(generate_unique_moves(p).size()==1459); assert(generate_search_moves(p).size()==568); verify_generator(p);
    Position pu; pu.set_items(Color::White,1,0,0); Move m; m.piece=PieceId::W1; m.item=Item::Push; m.push_to=make_square(1,0); m.path[0]=make_square(0,0); m.path[1]=make_square(0,0); m.path[2]=make_square(0,1); m.path[3]=make_square(1,1); m.path_length=4; assert(pu.is_legal_path(m)); m.path[2]=make_square(1,0); assert(!pu.is_legal_path(m));
    Position s; s.set_match_context(0,0,20); assert(evaluate_white(s)==0); Position wi=s; wi.set_items(Color::White,1,0,0); assert(evaluate_white(wi)>0);
    Engine e; SearchLimits lim; lim.depth=3; lim.multipv=3; auto r=e.go(lim); assert(r.has_move&&r.depth==3&&r.lines.size()==3&&distinct(r.lines));
    std::cout<<"RPSC Engine regression suite passed\n";
}
