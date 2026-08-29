#ifndef RPSC_TT_H_INCLUDED
#define RPSC_TT_H_INCLUDED

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
};

class TranspositionTable {
   public:
    explicit TranspositionTable(std::size_t megabytes = 64);

    void clear();
    const TTEntry* probe(Key key) const;
    void store(Key key, Depth depth, Value value, Bound bound, const Move* best_move);
    std::size_t size() const { return table_.size(); }

   private:
    std::vector<TTEntry> table_;
};

}  // namespace rpsc

#endif
