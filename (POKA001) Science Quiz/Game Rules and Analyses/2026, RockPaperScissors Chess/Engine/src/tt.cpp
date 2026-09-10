#include "tt.h"
#include <algorithm>
namespace rpsc {
TranspositionTable::TranspositionTable(std::size_t mb){resize(mb);} 
void TranspositionTable::resize(std::size_t mb){std::size_t bytes=std::max<std::size_t>(1,mb)*1024*1024;std::size_t requested=std::max<std::size_t>(1,bytes/sizeof(table_[0]));
std::size_t n=1;while((n<<1)<=requested)n<<=1;table_.assign(n,{});} 
void TranspositionTable::clear(){for(auto&c:table_)for(auto&e:c)e=TTEntry{};generation_=1;} 
const TTEntry* TranspositionTable::probe(Key key)const{if(table_.empty())return nullptr;const auto&c=table_[std::size_t(key)&(table_.size()-1)];for(const auto&e:c)if(e.bound!=Bound::None&&e.key==key)return &e;return nullptr;} 
void TranspositionTable::store(Key key,Depth depth,Value value,Bound bound,const Move*move){auto&c=table_[std::size_t(key)&(table_.size()-1)];TTEntry*slot=&c[0];for(auto&e:c){if(e.key==key){slot=&e;break;}int score=(e.bound==Bound::None?-100000:0)+e.depth*8+(e.generation==generation_?16:0);int best=(slot->bound==Bound::None?-100000:0)+slot->depth*8+(slot->generation==generation_?16:0);if(score<best)slot=&e;}if(slot->key==key&&slot->depth>depth&&bound!=Bound::Exact)return;slot->key=key;slot->depth=depth;slot->value=value;slot->bound=bound;slot->generation=generation_;if(move){slot->best_move=*move;slot->has_move=true;}else slot->has_move=false;}
}
