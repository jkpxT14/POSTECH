#include "evaluate.h"
#include "movegen.h"
#include <algorithm>

namespace rpsc {
namespace {
int mob1(const Position&p,int i){if(!p.pieces[i].alive)return 0;int n=0;const int dx[4]={0,0,1,-1},dy[4]={1,-1,0,0};for(int d=0;d<4;d++){int x=p.pieces[i].x+dx[d],y=p.pieces[i].y+dy[d];if(x>=0&&x<8&&y>=0&&y<8&&!p.occupied(x,y,i))n++;}return n;}
int center_bonus(const Piece&p){if(!p.alive)return 0;int dx=std::min(p.x,7-p.x),dy=std::min(p.y,7-p.y);return dx+dy;}
int terminal(const Position&p){int d=p.score(Side::White)-p.score(Side::Black);if(d)return 100*d;int q=p.quiz[0]-p.quiz[1];return q>0?25:q<0?-25:0;}
}
int evaluate_white(const Position&p){if(p.remaining_plies==0)return terminal(p);int v=100*(p.score(Side::White)-p.score(Side::Black));int aw=p.alive_count(Side::White),ab=p.alive_count(Side::Black);v+=12*(aw-ab);for(int i=0;i<8;i++)if(p.pieces[i].alive){int s=i<4?1:-1;v+=s*(2*mob1(p,i)+center_bonus(p.pieces[i]));}int reserve=std::clamp(2+p.remaining_plies,0,6);auto iv=[&](int s){auto&x=p.items[s];return reserve*(x.push+x.rotation+x.step);};v+=iv(0)-iv(1);return v;}
int evaluate(const Position&p){int v=evaluate_white(p);return p.side==Side::White?v:-v;}
int pressure_for(Position p,Side side,int cap){p.side=side;int n=0;for(auto&m:generate_moves(p,true))if(m.tactical>0&&++n>=cap)break;return n;}
} // namespace rpsc
