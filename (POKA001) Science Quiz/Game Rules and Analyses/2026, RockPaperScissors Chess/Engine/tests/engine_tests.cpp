#ifdef NDEBUG
#undef NDEBUG
#endif
#include <cassert>
#include <chrono>
#include <iostream>
#include <unordered_set>
#include "engine.h"
#include "evaluate.h"
#include "movegen.h"
#include "notation.h"
#include "orientation.h"
#include "perft.h"
using namespace rpsc;

static void verify_generator(Position& p){
    auto legal=generate_legal_moves(p);std::unordered_set<Key> expected,actual;
    for(const auto&m:legal){UndoState u;p.do_move(m,u);expected.insert(p.search_key());p.undo_move(u);}
    for(const auto&e:generate_search_moves_info(p)){assert(p.is_legal_path(e.move));UndoState u;p.do_move(e.move,u);actual.insert(p.search_key());p.undo_move(u);}assert(actual==expected);
}
static void verify_roundtrip(Position& p,const std::vector<Move>& moves){Key k=p.key(),sk=p.search_key();for(const auto&m:moves){std::string text=format_move(m);Move q;assert(parse_move(p,text,q));assert(q==m);UndoState u;p.do_move(m,u);p.undo_move(u);assert(p.key()==k&&p.search_key()==sk);}}
static bool distinct(const std::vector<RootLine>& lines){for(std::size_t i=0;i<lines.size();++i)for(std::size_t j=i+1;j<lines.size();++j)if(lines[i].move==lines[j].move)return false;return true;}
int main(){
    const auto&t=OrientationTable::instance();assert(t.size()==24);
    auto s=t.canonical(Gesture::Scissors,WristDirection::South);assert(t.top_gesture(t.roll(s,Direction::North))==Gesture::Rock);assert(t.top_gesture(t.roll(s,Direction::East))==Gesture::Paper);
    for(int i=0;i<24;++i){auto o=Orientation(i);assert(t.top_gesture(t.rotate_left(o))==t.top_gesture(o));assert(t.top_gesture(t.rotate_right(o))==t.top_gesture(o));assert(t.rotate_right(t.rotate_left(o))==o);assert(t.rotate_left(t.rotate_right(o))==o);assert(t.gesture_state_id(t.rotate_left(o))==t.gesture_state_id(t.rotate_right(o)));for(Direction d:{Direction::North,Direction::South,Direction::East,Direction::West}){Direction od=d==Direction::North?Direction::South:d==Direction::South?Direction::North:d==Direction::East?Direction::West:Direction::East;assert(t.roll(t.roll(o,d),od)==o);}}
    Position p;auto legal=generate_legal_moves(p);assert(legal.size()==161);assert(generate_unique_moves(p).size()==145);assert(generate_search_moves(p).size()==84);assert(perft(p,1)==161);assert(perft(p,2)==25575);assert(perft(p,3)==4215782);verify_generator(p);verify_roundtrip(p,legal);
    p.set_items(Color::White,1,1,1);p.set_items(Color::Black,1,1,1);legal=generate_legal_moves(p);assert(legal.size()==1472);assert(generate_unique_moves(p).size()==1019);assert(generate_search_moves(p).size()==427);verify_generator(p);
    bool push_return=false,l=false,r=false,ss=false,sl=false;for(const auto&m:legal){push_return|=m.item==Item::Push&&m.path_length>1&&m.path[1]==m.from();l|=m.item==Item::RotateLeft;r|=m.item==Item::RotateRight;ss|=m.item==Item::StepShort;sl|=m.item==Item::StepLong;}assert(push_return&&l&&r&&ss&&sl);verify_roundtrip(p,legal);
    Position symmetric;symmetric.set_match_context(0,0,20);assert(evaluate_white(symmetric)==0);Position wi=symmetric;wi.set_items(Color::White,1,0,0);assert(evaluate_white(wi)>0);
    Engine e;SearchLimits d4;d4.depth=4;d4.multipv=3;auto sr=e.go(d4);assert(sr.has_move&&sr.depth==4&&sr.lines.size()==3&&distinct(sr.lines));for(auto&ln:sr.lines)assert(e.position().is_legal_path(ln.move));
    Engine ie;ie.position().set_items(Color::White,1,1,1);ie.position().set_items(Color::Black,1,1,1);SearchLimits d2;d2.depth=2;d2.multipv=3;auto ir=ie.go(d2);assert(ir.has_move&&ir.depth==2&&ir.lines.size()==3&&distinct(ir.lines));
    SearchLimits choice;choice.depth=2;auto items=ie.choose_item(Color::White,choice);assert(items.lines.size()==3&&items.best_bucket>=0);Engine oe;auto initial=oe.choose_initial(choice);assert(initial.lines.size()==6&&initial.best_bucket>=0);
    // Same-key timed continuation must never discard the last completed result.
    Engine ce;ce.position().set_items(Color::White,1,1,1);ce.position().set_items(Color::Black,1,1,1);SearchLimits timed;timed.depth=84;timed.movetime=std::chrono::milliseconds(80);auto a=ce.go(timed);auto b=ce.go(timed);assert(a.has_move&&b.has_move&&b.depth>=a.depth);
    std::cout<<"RPSC Engine 0.14.0 regression suite passed\n";
}
