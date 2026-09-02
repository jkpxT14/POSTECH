#include "perft.h"
namespace rpsc {std::uint64_t perft(Position&p,int d){if(d<=0)return 1;auto m=generate_legal_moves(p);if(d==1)return m.size();std::uint64_t n=0;for(auto&x:m){UndoState u;p.do_move(x,u);n+=perft(p,d-1);p.undo_move(u);}return n;}}
