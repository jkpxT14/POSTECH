#ifndef RPSC_SEARCH_H_INCLUDED
#define RPSC_SEARCH_H_INCLUDED

#include <chrono>
#include <cstdint>
#include <unordered_map>
#include <vector>
#include "evaluate.h"
#include "movegen.h"
#include "tt.h"

namespace rpsc {
struct SearchLimits { Depth depth = 8; std::uint64_t nodes = 0; std::chrono::milliseconds movetime{0}; int multipv = 1; };
struct RootLine { Move move{}; Value value = 0; std::vector<Move> pv; };
struct SearchResult {
    Move best_move{};
    bool has_move = false;
    Value value = 0;
    Depth depth = 0;
    Depth seldepth = 0;
    std::uint64_t nodes = 0;
    std::chrono::milliseconds elapsed{0};
    std::vector<Move> pv;
    std::vector<RootLine> lines;
};
class Search {
   public:
    explicit Search(TranspositionTable& tt);
    SearchResult run(Position position, const SearchLimits& limits);
    void clear_memory();
   private:
    TranspositionTable& tt_;
    std::vector<int> history_, capture_history_;
    std::unordered_map<std::size_t, Move> countermoves_;
    std::unordered_map<std::uint64_t, int> continuation_, followup_;
    std::unordered_map<Key, std::vector<RootLine>> root_cache_;
    std::unordered_map<Key, Depth> root_depth_cache_;
    struct Context;
    Value negamax(Position&, Depth, Value, Value, int, Context&, bool, const Move*, const Move*, int);
    Value quiescence(Position&, Value, Value, int, Context&);
};
}  // namespace rpsc
#endif
