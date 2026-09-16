#include "perft.h"
#include "movegen.h"
namespace rpsc {uint64_t perft(const Position&p,int d){if(d<=0)return 1;uint64_t n=0;for(auto&m:generate_moves(p,false)){Position q=p;q.apply(m.move);n+=perft(q,d-1);}return n;}}
