#include "engine.h"

#include <algorithm>
#include <array>
#include <chrono>

#include "evaluate.h"

namespace rpsc {
namespace {

SearchLimits divided_limits(const SearchLimits& limits, int divisor) {
    SearchLimits probe = limits;
    probe.multipv = 1;
    if (probe.nodes) probe.nodes = std::max<std::uint64_t>(1, probe.nodes / divisor);
    if (probe.movetime.count() > 0) {
        probe.movetime = std::chrono::milliseconds(
            std::max<std::int64_t>(1, probe.movetime.count() / divisor));
    }
    return probe;
}

void add_item(Position& position, Color color, int bucket) {
    auto items = position.items(color);
    ++items[static_cast<std::size_t>(bucket)];
    position.set_items(color, items[0], items[1], items[2]);
}

}  // namespace

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

    // Quiz outcomes are external input. Each acquisition candidate is evaluated from the
    // current exact RPSC state without fabricating a future Quiz result. All three candidates
    // share the TT, while the supplied total node/time budget is divided across the root
    // alternatives. Unlike 0.8, the probe is not capped at depth 2.
    const SearchLimits probe_limits = divided_limits(limits, 3);
    for (int bucket = 0; bucket < 3; ++bucket) {
        Position probe = position_;
        add_item(probe, chooser, bucket);
        probe.set_side_to_move(Color::White);

        Search search(tt_);
        SearchResult result = search.run(probe, probe_limits);
        const Value white_value = result.has_move ? result.value : evaluate_white(probe);
        choice.lines.push_back({bucket, white_value, std::move(result)});
    }

    auto better = [&](const ItemChoiceLine& lhs, const ItemChoiceLine& rhs) {
        if (lhs.white_value != rhs.white_value) {
            return chooser == Color::White ? lhs.white_value > rhs.white_value
                                           : lhs.white_value < rhs.white_value;
        }
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
    Position probe = position_;
    probe.set_side_to_move(Color::White);
    Search search(tt_);
    SearchResult result = search.run(probe, limits);
    const Value white_value = result.has_move ? result.value : evaluate_white(probe);
    return {white_value >= 0, white_value, std::move(result)};
}

InitialChoiceResult Engine::choose_initial(const SearchLimits& limits) {
    InitialChoiceResult choice;
    choice.lines.reserve(6);
    const SearchLimits probe_limits = divided_limits(limits, 6);

    // The first solo-correct team owns two coupled decisions: first/second order and the
    // acquired item. Evaluate all six combinations as one RPSC decision instead of greedily
    // choosing order and item in isolation. The chooser is White in choose_first branches and
    // Black in choose_second branches. Future Quiz outcomes are never generated.
    for (bool choose_first : {true, false}) {
        for (int bucket = 0; bucket < 3; ++bucket) {
            Position probe;
            const Color chooser = choose_first ? Color::White : Color::Black;
            add_item(probe, chooser, bucket);
            probe.set_side_to_move(Color::White);

            Search search(tt_);
            SearchResult result = search.run(probe, probe_limits);
            const Value white_value = result.has_move ? result.value : evaluate_white(probe);
            const Value chooser_value = choose_first ? white_value : -white_value;
            choice.lines.push_back(
                {choose_first, bucket, chooser_value, white_value, std::move(result)});
        }
    }

    std::stable_sort(choice.lines.begin(), choice.lines.end(), [](const InitialChoiceLine& lhs,
                                                                  const InitialChoiceLine& rhs) {
        if (lhs.chooser_value != rhs.chooser_value) return lhs.chooser_value > rhs.chooser_value;
        if (lhs.choose_first != rhs.choose_first) return lhs.choose_first;
        return lhs.bucket < rhs.bucket;
    });
    if (!choice.lines.empty()) {
        choice.choose_first = choice.lines.front().choose_first;
        choice.best_bucket = choice.lines.front().bucket;
        choice.chooser_value = choice.lines.front().chooser_value;
        choice.white_value = choice.lines.front().white_value;
    }
    return choice;
}

std::uint64_t Engine::perft(int depth) { return rpsc::perft(position_, depth); }

void Engine::clear_search() { tt_.clear(); }

}  // namespace rpsc
