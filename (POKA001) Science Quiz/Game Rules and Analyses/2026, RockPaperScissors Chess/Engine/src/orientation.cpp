#include "orientation.h"
#include <algorithm>
#include <map>
#include <queue>
#include <sstream>
#include <stdexcept>

namespace rpsc {
namespace {
Vec3 transform(Vec3 v, const std::string& op){
  int x=v.x,y=v.y,z=v.z;
  if(op=="N") return {x,-z,y}; if(op=="S") return {x,z,-y};
  if(op=="E") return {z,y,-x}; if(op=="W") return {-z,y,x};
  if(op=="CW") return {y,-x,z}; if(op=="CCW") return {-y,x,z};
  throw std::logic_error("bad cube op");
}
Cube transformed(const Cube& c,const std::string& op){ Cube n=c; for(auto& f:n){f.normal=transform(f.normal,op);f.wrist=transform(f.wrist,op);} return n; }
std::string key(const Cube& c){
  auto a=c; std::sort(a.begin(),a.end(),[](auto&A,auto&B){return A.id<B.id;}); std::ostringstream o;
  for(auto&f:a)o<<f.id<<':'<<f.normal.x<<','<<f.normal.y<<','<<f.normal.z<<'/'<<f.wrist.x<<','<<f.wrist.y<<','<<f.wrist.z<<';'; return o.str();
}
Gesture gesture_on(const Cube& c,Vec3 n){ for(auto&f:c)if(f.normal==n)return f.gesture; throw std::logic_error("missing face"); }
const Face& top_face(const Cube& c){ for(auto&f:c)if(f.normal==Vec3{0,0,1})return f; throw std::logic_error("missing top"); }
std::string wrist_dir(Vec3 w){ if(w==Vec3{0,1,0})return"N";if(w==Vec3{0,-1,0})return"S";if(w==Vec3{1,0,0})return"E";if(w==Vec3{-1,0,0})return"W";if(w==Vec3{0,0,1})return"UP";return"DOWN"; }
int gidx(Gesture g){return g==Gesture::Scissors?0:g==Gesture::Rock?1:2;}
}

const OrientationTable& OrientationTable::instance(){ static OrientationTable t; return t; }
OrientationTable::OrientationTable(){
  Cube base{{
    {0,Gesture::Scissors,{0,0,1},{0,-1,0}}, {1,Gesture::Scissors,{0,0,-1},{0,-1,0}},
    {2,Gesture::Rock,{0,1,0},{0,0,1}}, {3,Gesture::Rock,{0,-1,0},{0,0,-1}},
    {4,Gesture::Paper,{-1,0,0},{0,-1,0}}, {5,Gesture::Paper,{1,0,0},{0,-1,0}}
  }};
  std::map<std::string,int> ids; std::queue<int> q; cubes_.push_back(base);ids[key(base)]=0;q.push(0);
  const std::array<std::string,4> ops{"N","S","E","W"};
  while(!q.empty()){
    int i=q.front();q.pop(); for(auto&op:ops){Cube n=transformed(cubes_[i],op);auto k=key(n);if(!ids.count(k)){int j=(int)cubes_.size();ids[k]=j;cubes_.push_back(n);q.push(j);}}
  }
  if(cubes_.size()!=24)throw std::runtime_error("cube orientation generation failed");
  for(int i=0;i<24;i++){
    for(int d=0;d<4;d++){Cube n=transformed(cubes_[i],ops[d]);roll_[i][d]=ids.at(key(n));}
    rot_l_[i]=ids.at(key(transformed(cubes_[i],"CCW"))); rot_r_[i]=ids.at(key(transformed(cubes_[i],"CW")));
    top_[i]=top_face(cubes_[i]).gesture;
    reduced_[i]=gesture_name(gesture_on(cubes_[i],{0,0,1}))+gesture_name(gesture_on(cubes_[i],{0,1,0}))+gesture_name(gesture_on(cubes_[i],{1,0,0}));
  }
  canonical_.fill(-1);
  for(Gesture g:{Gesture::Scissors,Gesture::Rock,Gesture::Paper})for(int w=0;w<2;w++){
    std::string wd=w==0?"S":"N"; int best=-1; for(int i=0;i<24;i++){auto&t=top_face(cubes_[i]);if(t.gesture==g&&wrist_dir(t.wrist)==wd){if(best<0||t.id==gidx(g)*2)best=i;}}
    canonical_[gidx(g)*2+w]=best;
  }
}
int OrientationTable::rotate(int o,Item item) const{
  switch(item){case Item::RoN:return roll_[o][0];case Item::RoS:return roll_[o][1];case Item::RoE:return roll_[o][2];case Item::RoW:return roll_[o][3];case Item::RoL:return rot_l_[o];case Item::RoR:return rot_r_[o];default:return o;}
}
int OrientationTable::canonical(Gesture g,bool white) const{return canonical_[gidx(g)*2+(white?0:1)];}
} // namespace rpsc
