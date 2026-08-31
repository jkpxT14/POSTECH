#include "engine.h"

#include <algorithm>
#include <chrono>

#include "evaluate.h"

namespace rpsc {

Engine::Engine(std::size_t hash_megabytes) : position_(), tt_(hash_megabytes) {}

void Engine::new_game() {
    position_.reset();
    tt_.clear();
}

SearchResult Engine::go(const SearchLimits& limits) {
    Search search(tt_);
    return search.run(position_, limits);
}

ItemChoiceResult Engine::choose_item(Color chooser, const SearchLimits& limits) {
    ItemChoiceResult choice;
    choice.lines.reserve(3);

    // The Quiz outcome is external input. Item choice therefore does not invent future Quiz
    // results. Each candidate is compared by a bounded probe of the next possible board event:
    // White moves, then Black moves. Tactical score-event extensions may still extend a forcing
    // combat continuation, but nominal probe depth never goes beyond those two board plies.
    SearchLimits probe_limits = limits;
    probe_limits.depth = std::clamp(probe_limits.depth, 1, 2);
    probe_limits.multipv = 1;
    if (probe_limits.nodes) probe_limits.nodes = std::max<std::uint64_t>(1, probe_limits.nodes / 3);
    if (probe_limits.movetime.count() > 0)
        probe_limits.movetime =
            std::chrono::milliseconds(std::max<std::int64_t>(1, probe_limits.movetime.count() / 3));

    for (int bucket = 0; bucket < 3; ++bucket) {
        Position probe = position_;
        const auto before = probe.items(chooser);
        auto next = before;
        ++next[bucket];
        probe.set_items(chooser, next[0], next[1], next[2]);
        probe.set_side_to_move(Color::White);

        Search search(tt_);
        SearchResult result = search.run(probe, probe_limits);
        const Value white_value = result.has_move ? result.value : evaluate_white(probe);
        choice.lines.push_back({bucket, white_value, std::move(result)});
    }

    auto better = [&](const ItemChoiceLine& lhs, const ItemChoiceLine& rhs) {
        if (lhs.white_value != rhs.white_value)
            return chooser == Color::White ? lhs.white_value > rhs.white_value
                                           : lhs.white_value < rhs.white_value;
        return lhs.bucket < rhs.bucket;
    };
    std::stable_sort(choice.lines.begin(), choice.lines.end(), better);
    if (!choice.lines.empty()) {
        choice.best_bucket = choice.lines.front().bucket;
        choice.white_value = choice.lines.front().white_value;
    }
    return choice;
}

OrderChoiceResult Engine::choose_order(const SearchLimits& limits) {
    // Order choice happens before the first board position is committed. The canonical
    // start position models the chooser as White (first mover). If the chooser instead
    // selects second, the same symmetric start-position value changes sign from that
    // team's point of view. No future Quiz outcome is assumed.
    Position probe = position_;
    probe.set_side_to_move(Color::White);
    Search search(tt_);
    SearchResult result = search.run(probe, limits);
    const Value white_value = result.has_move ? result.value : evaluate_white(probe);
    return {white_value >= 0, white_value, std::move(result)};
}

std::uint64_t Engine::perft(int depth) { return rpsc::perft(position_, depth); }

void Engine::clear_search() { tt_.clear(); }

}  // namespace rpsc
