#ifndef RPSC_SEARCH_H_INCLUDED
#define RPSC_SEARCH_H_INCLUDED

#include <chrono>
#include <cstdint>
#include <vector>
#include "evaluate.h"
#include "movegen.h"
#include "tt.h"

namespace rpsc {

struct SearchLimits {
    Depth depth = 6;
    std::uint64_t nodes = 0;
    std::chrono::milliseconds movetime{0};
};

struct SearchResult {
    Move best_move{};
    bool has_move = false;
    Value value = 0;
    Depth depth = 0;
    Depth seldepth = 0;
    std::uint64_t nodes = 0;
    std::chrono::milliseconds elapsed{0};
    std::vector<Move> pv;
};

class Search {
   public:
    explicit Search(TranspositionTable& tt);
    SearchResult run(Position position, const SearchLimits& limits);

   private:
    TranspositionTable& tt_;
    struct Context;
    Value negamax(Position& position, Depth depth, Value alpha, Value beta, int ply, Context& ctx, bool pv_node);
    Value quiescence(Position& position, Value alpha, Value beta, int ply, Context& ctx);
};

}  // namespace rpsc
#endif
