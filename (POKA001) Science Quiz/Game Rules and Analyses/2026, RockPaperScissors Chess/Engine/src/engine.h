#pragma once
#include "perft.h"
#include "search.h"
#include <vector>

namespace rpsc {

struct ItemChoiceLine {
  int bucket = -1; // 0 Push, 1 Rotation, 2 Step
  int white_value = 0;
  SearchResult probe;
};
struct ItemChoiceResult {
  int best_bucket = -1;
  int white_value = 0;
  std::vector<ItemChoiceLine> lines;
};
struct OrderChoiceResult {
  bool choose_first = true;
  int white_value = 0;
  SearchResult probe;
};
struct InitialChoiceLine {
  bool choose_first = true;
  int bucket = -1;
  int chooser_value = 0;
  int white_value = 0;
  SearchResult probe;
};
struct InitialChoiceResult {
  bool choose_first = true;
  int best_bucket = -1;
  int chooser_value = 0;
  int white_value = 0;
  std::vector<InitialChoiceLine> lines;
};

class Engine {
public:
  static constexpr const char* VERSION = "0.24.1";

  Engine() = default;
  void new_game();
  Position& position() { return position_; }
  const Position& position() const { return position_; }

  SearchResult go(const SearchLimits& limits);
  SearchResult analyze(const SearchLimits& limits) { return go(limits); }
  SearchResult analyze(const Position& p, const SearchLimits& limits) { return searcher_.search(p, limits); }

  ItemChoiceResult choose_item(Side chooser, const SearchLimits& limits);
  OrderChoiceResult choose_order(const SearchLimits& limits);
  InitialChoiceResult choose_initial(const SearchLimits& limits);

  std::uint64_t perft(int depth);
  void clear_search();

private:
  Position position_{};
  Searcher searcher_{};
};

} // namespace rpsc
