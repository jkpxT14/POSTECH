#include "tt.h"

#include <algorithm>
#include <limits>

namespace rpsc {

TranspositionTable::TranspositionTable(std::size_t megabytes) {
    const std::size_t bytes = std::max<std::size_t>(1, megabytes) * 1024 * 1024;
    std::size_t clusters = 1;
    while ((clusters << 1) * sizeof(Cluster) <= bytes) clusters <<= 1;
    clusters_.resize(clusters);
}

void TranspositionTable::clear() {
    for (auto& cluster : clusters_)
        for (auto& entry : cluster.entries) entry = TTEntry{};
    generation_ = 1;
}

void TranspositionTable::new_search() {
    ++generation_;
    if (generation_ == 0) generation_ = 1;
}

const TTEntry* TranspositionTable::probe(Key key) const {
    const auto& cluster = clusters_[key & (clusters_.size() - 1)];
    const TTEntry* best = nullptr;
    for (const auto& entry : cluster.entries) {
        if (entry.key != key) continue;
        if (!best || entry.depth > best->depth) best = &entry;
    }
    return best;
}

void TranspositionTable::store(Key key, Depth depth, Value value, Bound bound,
                               const Move* best_move) {
    auto& cluster = clusters_[key & (clusters_.size() - 1)];

    TTEntry* target = nullptr;
    for (auto& entry : cluster.entries) {
        if (entry.key == key) {
            target = &entry;
            break;
        }
        if (entry.bound == Bound::None) {
            target = &entry;
            break;
        }
    }

    if (!target) {
        int best_replacement_score = std::numeric_limits<int>::max();
        for (auto& entry : cluster.entries) {
            const int age = static_cast<std::uint8_t>(generation_ - entry.generation);
            // Prefer retaining deep, current entries. Age dominates a few plies of depth so
            // old collisions do not pin a cluster indefinitely.
            const int keep_score = entry.depth - 4 * age;
            if (keep_score < best_replacement_score) {
                best_replacement_score = keep_score;
                target = &entry;
            }
        }
    }

    if (target->key == key && target->bound != Bound::None && depth < target->depth &&
        bound != Bound::Exact) {
        // Refresh the age and keep the stronger same-key result.
        target->generation = generation_;
        if (best_move && !target->has_move) {
            target->best_move = *best_move;
            target->has_move = true;
        }
        return;
    }

    target->key = key;
    target->depth = depth;
    target->value = value;
    target->bound = bound;
    target->generation = generation_;
    target->has_move = best_move != nullptr;
    if (best_move) target->best_move = *best_move;
}

}  // namespace rpsc
