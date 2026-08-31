#ifndef RPSC_TT_H_INCLUDED
#define RPSC_TT_H_INCLUDED
#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>
#include "move.h"
namespace rpsc {
struct TTEntry { Key key=0; Value value=0; Depth depth=-1; Bound bound=Bound::None; Move best_move{}; bool has_move=false; std::uint8_t generation=0; };
class TranspositionTable { public: explicit TranspositionTable(std::size_t megabytes=64); void clear(); void new_search(); const TTEntry* probe(Key key) const; void store(Key key,Depth depth,Value value,Bound bound,const Move* best_move); std::size_t size() const{return clusters_.size()*ClusterSize;} private: static constexpr std::size_t ClusterSize=4; struct Cluster{std::array<TTEntry,ClusterSize> entries{};}; std::vector<Cluster> clusters_; std::uint8_t generation_=1;};
}
#endif
