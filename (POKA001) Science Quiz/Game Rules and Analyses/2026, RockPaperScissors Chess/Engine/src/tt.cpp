#include "tt.h"
#include <algorithm>
#include <limits>
namespace rpsc {
TranspositionTable::TranspositionTable(std::size_t megabytes){const std::size_t bytes=std::max<std::size_t>(1,megabytes)*1024*1024; std::size_t clusters=1; while((clusters<<1)*sizeof(Cluster)<=bytes) clusters<<=1; clusters_.resize(clusters);}
void TranspositionTable::clear(){for(auto& c:clusters_) for(auto& e:c.entries)e=TTEntry{}; generation_=1;}
void TranspositionTable::new_search(){++generation_; if(generation_==0)generation_=1;}
const TTEntry* TranspositionTable::probe(Key key) const{const auto& c=clusters_[key&(clusters_.size()-1)]; const TTEntry* best=nullptr; for(const auto& e:c.entries){if(e.key!=key)continue; if(!best||e.depth>best->depth)best=&e;} return best;}
void TranspositionTable::store(Key key,Depth depth,Value value,Bound bound,const Move* best_move){auto& c=clusters_[key&(clusters_.size()-1)]; TTEntry* target=nullptr; for(auto& e:c.entries){if(e.key==key){target=&e;break;} if(e.bound==Bound::None){target=&e;break;}} if(!target){int best_score=std::numeric_limits<int>::max(); for(auto& e:c.entries){int age=static_cast<std::uint8_t>(generation_-e.generation); int keep=e.depth-4*age; if(keep<best_score){best_score=keep;target=&e;}}} if(target->key==key&&target->bound!=Bound::None&&depth<target->depth&&bound!=Bound::Exact){target->generation=generation_; if(best_move&&!target->has_move){target->best_move=*best_move;target->has_move=true;} return;} target->key=key;target->depth=depth;target->value=value;target->bound=bound;target->generation=generation_;target->has_move=best_move!=nullptr;if(best_move)target->best_move=*best_move;}
}
