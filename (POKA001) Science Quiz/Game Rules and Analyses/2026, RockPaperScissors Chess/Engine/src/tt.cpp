#include "tt.h"

#include <algorithm>

namespace rpsc {

TranspositionTable::TranspositionTable(std::size_t megabytes) {
    const std::size_t bytes = std::max<std::size_t>(1, megabytes) * 1024 * 1024;
    std::size_t entries = 1;
    while ((entries << 1) * sizeof(TTEntry) <= bytes) entries <<= 1;
    table_.resize(entries);
}

void TranspositionTable::clear() {
    for (auto& entry : table_) entry = TTEntry{};
}

const TTEntry* TranspositionTable::probe(Key key) const {
    const auto& entry = table_[key & (table_.size() - 1)];
    return entry.key == key ? &entry : nullptr;
}

void TranspositionTable::store(Key key, Depth depth, Value value, Bound bound,
                               const Move* best_move) {
    auto& entry = table_[key & (table_.size() - 1)];
    if (entry.key != key || depth >= entry.depth || bound == Bound::Exact) {
        entry.key = key;
        entry.depth = depth;
        entry.value = value;
        entry.bound = bound;
        entry.has_move = best_move != nullptr;
        if (best_move) entry.best_move = *best_move;
    }
}

}  // namespace rpsc
