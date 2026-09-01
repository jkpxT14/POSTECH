#ifndef RPSC_ENGINE_H_INCLUDED
#define RPSC_ENGINE_H_INCLUDED

#include <cstddef>
#include <cstdint>
#include <vector>

#include "perft.h"
#include "search.h"

namespace rpsc {

struct ItemChoiceLine {
    int bucket = -1;  // 0 Push, 1 Rotation, 2 Step
    Value white_value = 0;
    SearchResult probe;
};

struct ItemChoiceResult {
    int best_bucket = -1;
    Value white_value = 0;
    std::vector<ItemChoiceLine> lines;
};

struct OrderChoiceResult {
    bool choose_first = true;
    Value white_value = 0;
    SearchResult probe;
};

struct InitialChoiceLine {
    bool choose_first = true;
    int bucket = -1;  // 0 Push, 1 Rotation, 2 Step
    Value chooser_value = 0;
    Value white_value = 0;
    SearchResult probe;
};

struct InitialChoiceResult {
    bool choose_first = true;
    int best_bucket = -1;
    Value chooser_value = 0;
    Value white_value = 0;
    std::vector<InitialChoiceLine> lines;
};

class Engine {
   public:
    explicit Engine(std::size_t hash_megabytes = 64);
    void new_game();
    Position& position() { return position_; }
    const Position& position() const { return position_; }
    SearchResult go(const SearchLimits& limits);
    ItemChoiceResult choose_item(Color chooser, const SearchLimits& limits);
    OrderChoiceResult choose_order(const SearchLimits& limits);
    InitialChoiceResult choose_initial(const SearchLimits& limits);
    std::uint64_t perft(int depth);
    void clear_search();

   private:
    Position position_;
    TranspositionTable tt_;
    Search search_;
};

}  // namespace rpsc

#endif
