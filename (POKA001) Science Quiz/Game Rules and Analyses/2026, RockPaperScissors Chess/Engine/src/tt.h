#pragma once
#include "types.h"
#include <string>
#include <unordered_map>
namespace rpsc {
struct TTEntry{int depth=0,value=0,bound=0;Move best;bool has_best=false;};
class TranspositionTable{public:explicit TranspositionTable(size_t max=150000):max_(max){};void clear(){map_.clear();}const TTEntry* probe(const std::string&k)const{auto i=map_.find(k);return i==map_.end()?nullptr:&i->second;}void store(std::string k,TTEntry e){if(map_.size()>max_)map_.clear();map_[std::move(k)]=std::move(e);}private:size_t max_;std::unordered_map<std::string,TTEntry>map_;};
} // namespace rpsc
