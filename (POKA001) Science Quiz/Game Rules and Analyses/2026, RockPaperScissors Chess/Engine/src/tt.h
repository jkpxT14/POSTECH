#ifndef RPSC_TT_H_INCLUDED
#define RPSC_TT_H_INCLUDED

#include <array>
#include <cstddef>
#include <vector>
#include "move.h"

namespace rpsc {
struct TTEntry {
    Key key = 0;
    Value value = 0;
    Depth depth = -1;
    Bound bound = Bound::None;
    Move best_move{};
    bool has_move = false;
    std::uint8_t generation = 0;
};
class TranspositionTable {
   public:
    explicit TranspositionTable(std::size_t megabytes = 64);
    void resize(std::size_t);
    void clear();
    void new_search() { ++generation_; }
    const TTEntry* probe(Key) const;
    void store(Key, Depth, Value, Bound, const Move*);
   private:
    static constexpr std::size_t Cluster = 4;
    std::vector<std::array<TTEntry, Cluster>> table_;
    std::uint8_t generation_ = 1;
};
}  // namespace rpsc
#endif
