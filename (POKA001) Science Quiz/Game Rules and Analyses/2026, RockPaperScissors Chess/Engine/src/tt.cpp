#include "tt.h"
#include <algorithm>
namespace rpsc {
TranspositionTable::TranspositionTable(std::size_t mb) { resize(mb); }
void TranspositionTable::resize(std::size_t mb) {
    std::size_t bytes = std::max<std::size_t>(1, mb) * 1024 * 1024;
    std::size_t n = std::max<std::size_t>(1, bytes / sizeof(table_[0]));
    table_.assign(n, {});
}
void TranspositionTable::clear() {
    for (auto& cluster : table_) for (auto& entry : cluster) entry = TTEntry{};
    generation_ = 1;
}
const TTEntry* TranspositionTable::probe(Key key) const {
    if (table_.empty()) return nullptr;
    const auto& cluster = table_[std::size_t(key) % table_.size()];
    for (const auto& entry : cluster)
        if (entry.bound != Bound::None && entry.key == key) return &entry;
    return nullptr;
}
void TranspositionTable::store(Key key, Depth depth, Value value, Bound bound, const Move* move) {
    auto& cluster = table_[std::size_t(key) % table_.size()];
    TTEntry* slot = &cluster[0];
    for (auto& entry : cluster) {
        if (entry.key == key) { slot = &entry; break; }
        int score = (entry.bound == Bound::None ? -100000 : 0) + entry.depth * 8 + (entry.generation == generation_ ? 16 : 0);
        int best = (slot->bound == Bound::None ? -100000 : 0) + slot->depth * 8 + (slot->generation == generation_ ? 16 : 0);
        if (score < best) slot = &entry;
    }
    if (slot->key == key && slot->depth > depth && bound != Bound::Exact) return;
    slot->key = key;
    slot->depth = depth;
    slot->value = value;
    slot->bound = bound;
    slot->generation = generation_;
    if (move) { slot->best_move = *move; slot->has_move = true; }
    else slot->has_move = false;
}
}  // namespace rpsc
