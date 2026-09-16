#include "engine.h"
#include "evaluate.h"
#include <algorithm>
#include <array>

namespace rpsc {
namespace {

SearchLimits scaled_limits(const SearchLimits& in, int num, int den) {
  SearchLimits out = in;
  out.multipv = 1;
  out.movetime_ms = std::max(20, in.movetime_ms * num / den);
  return out;
}

void add_item(Position& p, Side side, int bucket) {
  Inventory& inv = p.items[side_index(side)];
  if (bucket == 0) ++inv.push;
  else if (bucket == 1) ++inv.rotation;
  else if (bucket == 2) ++inv.step;
}

int white_value_from(const Position& root, const SearchResult& r) {
  if (!r.has_best) return evaluate_white(root);
  return root.side == Side::White ? r.value : -r.value;
}

ItemChoiceLine probe_item(Position base, Side chooser, int bucket,
                          Searcher& searcher, const SearchLimits& limits) {
  add_item(base, chooser, bucket);
  base.side = Side::White;
  auto result = searcher.search(base, limits);
  return {bucket, white_value_from(base, result), std::move(result)};
}

InitialChoiceLine probe_initial(bool choose_first, int bucket,
                                Searcher& searcher, const SearchLimits& limits) {
  Position probe;
  const Side chooser = choose_first ? Side::White : Side::Black;
  add_item(probe, chooser, bucket);
  // Initial solo-correct quiz: chooser receives the item. The first board move still belongs to White.
  probe.quiz = choose_first ? std::array<int,2>{1,0} : std::array<int,2>{0,1};
  probe.remaining_plies = 38;
  probe.side = Side::White;
  auto result = searcher.search(probe, limits);
  const int white = white_value_from(probe, result);
  return {choose_first, bucket, choose_first ? white : -white, white, std::move(result)};
}

} // namespace

void Engine::new_game() {
  position_ = Position{};
  searcher_.clear();
}

SearchResult Engine::go(const SearchLimits& limits) {
  return searcher_.search(position_, limits);
}

ItemChoiceResult Engine::choose_item(Side chooser, const SearchLimits& limits) {
  ItemChoiceResult out;
  out.lines.reserve(3);
  auto better = [&](const ItemChoiceLine& a, const ItemChoiceLine& b) {
    if (a.white_value != b.white_value)
      return chooser == Side::White ? a.white_value > b.white_value : a.white_value < b.white_value;
    return a.bucket < b.bucket;
  };

  // 45% broad screening (15% per item), then 55% rank-biased refinement.
  for (int bucket = 0; bucket < 3; ++bucket)
    out.lines.push_back(probe_item(position_, chooser, bucket, searcher_, scaled_limits(limits, 15, 100)));
  std::stable_sort(out.lines.begin(), out.lines.end(), better);
  constexpr std::array<int,3> refine{{25,18,12}};
  for (std::size_t rank = 0; rank < out.lines.size(); ++rank) {
    const int bucket = out.lines[rank].bucket;
    out.lines[rank] = probe_item(position_, chooser, bucket, searcher_, scaled_limits(limits, refine[rank], 100));
  }
  std::stable_sort(out.lines.begin(), out.lines.end(), better);
  if (!out.lines.empty()) {
    out.best_bucket = out.lines.front().bucket;
    out.white_value = out.lines.front().white_value;
  }
  return out;
}

OrderChoiceResult Engine::choose_order(const SearchLimits& limits) {
  Position probe = position_;
  probe.side = Side::White;
  auto result = searcher_.search(probe, limits);
  const int white = white_value_from(probe, result);
  return {white >= 0, white, std::move(result)};
}

InitialChoiceResult Engine::choose_initial(const SearchLimits& limits) {
  InitialChoiceResult out;
  out.lines.reserve(6);
  auto better = [](const InitialChoiceLine& a, const InitialChoiceLine& b) {
    if (a.chooser_value != b.chooser_value) return a.chooser_value > b.chooser_value;
    if (a.choose_first != b.choose_first) return a.choose_first;
    return a.bucket < b.bucket;
  };

  // Six broad probes use 8% each; the best three receive 24/16/12% refinement.
  for (bool first : {true, false})
    for (int bucket = 0; bucket < 3; ++bucket)
      out.lines.push_back(probe_initial(first, bucket, searcher_, scaled_limits(limits, 8, 100)));
  std::stable_sort(out.lines.begin(), out.lines.end(), better);
  constexpr std::array<int,3> refine{{24,16,12}};
  for (std::size_t rank = 0; rank < refine.size(); ++rank) {
    const bool first = out.lines[rank].choose_first;
    const int bucket = out.lines[rank].bucket;
    out.lines[rank] = probe_initial(first, bucket, searcher_, scaled_limits(limits, refine[rank], 100));
  }
  std::stable_sort(out.lines.begin(), out.lines.end(), better);
  if (!out.lines.empty()) {
    const auto& x = out.lines.front();
    out.choose_first = x.choose_first;
    out.best_bucket = x.bucket;
    out.chooser_value = x.chooser_value;
    out.white_value = x.white_value;
  }
  return out;
}

std::uint64_t Engine::perft(int depth) {
  return rpsc::perft(position_, depth);
}

void Engine::clear_search() {
  searcher_.clear();
}

} // namespace rpsc
