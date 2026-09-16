#include "movegen.h"
#include "move.h"
#include <functional>
#include <unordered_set>

namespace rpsc {
namespace {
const int DX[4]={0,0,1,-1},DY[4]={1,-1,0,0};
bool inside(int x,int y){return x>=0&&x<8&&y>=0&&y<8;}
struct Variant{Item item=Item::None;int px=-1,py=-1;int ori=0;int sx=0,sy=0,len=0;};
}
std::vector<ScoredMove> generate_moves(const Position& pos,bool tactical_only){
  std::vector<ScoredMove> out;std::unordered_set<std::string> seen;const auto&o=OrientationTable::instance();Side mover=pos.side;int si=side_index(mover);
  for(int i=0;i<8;i++){
    const auto&p=pos.pieces[i];if(!p.alive||((i<4)!=(mover==Side::White)))continue;
    std::vector<Variant> vs;vs.push_back({Item::None,-1,-1,p.orientation,p.x,p.y,base_distance(o.top(p.orientation))});
    if(pos.items[si].push>0)for(int di=0;di<4;di++){int x=p.x+DX[di],y=p.y+DY[di];if(inside(x,y)&&!pos.occupied(x,y,i))vs.push_back({Item::Push,x,y,p.orientation,x,y,base_distance(o.top(p.orientation))});}
    if(pos.items[si].rotation>0){std::unordered_set<std::string> ro;for(Item it:{Item::RoN,Item::RoS,Item::RoE,Item::RoW,Item::RoL,Item::RoR}){int no=o.rotate(p.orientation,it);auto rk=o.reduced(no);if(ro.insert(rk).second)vs.push_back({it,-1,-1,no,p.x,p.y,base_distance(o.top(no))});}}
    if(pos.items[si].step>0){int L=base_distance(o.top(p.orientation));vs.push_back({Item::StepShort,-1,-1,p.orientation,p.x,p.y,L-1});vs.push_back({Item::StepLong,-1,-1,p.orientation,p.x,p.y,L+1});}
    for(auto&v:vs){
      std::vector<Dir> dirs;std::vector<std::pair<int,int>> path{{p.x,p.y}};
      std::function<void(int,int,int,std::optional<Dir>,int)> dfs=[&](int x,int y,int ori,std::optional<Dir> last,int rem){
        if(rem==0){Move m;m.piece=i;m.item=v.item;m.push_x=v.px;m.push_y=v.py;m.dirs=dirs;m.path=path;Position n=pos;bool reset=false;int t=0;if(!n.apply(m,&reset,&t))return;if(tactical_only&&t==0&&!reset)return;auto k=n.key();if(seen.insert(k).second)out.push_back({m,t,reset});return;}
        for(int di=0;di<4;di++){Dir d=(Dir)di;if(last&&d==opposite(*last))continue;int nx=x+DX[di],ny=y+DY[di];if(!inside(nx,ny)||pos.occupied(nx,ny,i))continue;if(rem==1&&pos.adjacent_enemies(nx,ny,mover,i).size()>=2)continue;int no=o.roll(ori,d);dirs.push_back(d);path.push_back({nx,ny});dfs(nx,ny,no,d,rem-1);path.pop_back();dirs.pop_back();}
      };
      dfs(v.sx,v.sy,v.ori,std::nullopt,v.len);
    }
  }
  return out;
}
} // namespace rpsc
