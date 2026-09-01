#include "engine.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>

#include "evaluate.h"

namespace rpsc {
namespace {

SearchLimits scaled_limits(const SearchLimits& limits, int numerator, int denominator) {
    SearchLimits probe = limits;
    probe.multipv = 1;
    if (probe.nodes) {
        probe.nodes = std::max<std::uint64_t>(
            1, probe.nodes * static_cast<std::uint64_t>(numerator) /
                   static_cast<std::uint64_t>(denominator));
    }
    if (probe.movetime.count() > 0) {
        probe.movetime = std::chrono::milliseconds(std::max<std::int64_t>(
            1, probe.movetime.count() * numerator / denominator));
    }
    return probe;
}

SearchLimits divided_limits(const SearchLimits& limits, int divisor) {
    return scaled_limits(limits, 1, divisor);
}

bool has_explicit_budget(const SearchLimits& limits) {
    return limits.nodes != 0 || limits.movetime.count() > 0;
}

void add_item(Position& position, Color color, int bucket) {
    auto items = position.items(color);
    ++items[static_cast<std::size_t>(bucket)];
    position.set_items(color, items[0], items[1], items[2]);
}

ItemChoiceLine probe_item(Position base, Color chooser, int bucket, Search& search,
                          const SearchLimits& limits) {
    add_item(base, chooser, bucket);
    // Quiz is external. The probe asks only what this resource is worth at the next known
    // board-decision interface, whose move order is White then Black.
    base.set_side_to_move(Color::White);
    SearchResult result = search.run(base, limits);
    const Value white_value = result.has_move ? result.value : evaluate_white(base);
    return {bucket, white_value, std::move(result)};
}

InitialChoiceLine probe_initial(bool choose_first, int bucket, Search& search,
                                const SearchLimits& limits) {
    Position probe;
    const Color chooser = choose_first ? Color::White : Color::Black;
    add_item(probe, chooser, bucket);
    probe.set_match_context(choose_first ? 1 : 0, choose_first ? 0 : 1, 38);
    probe.set_side_to_move(Color::White);
    SearchResult result = search.run(probe, limits);
    const Value white_value = result.has_move ? result.value : evaluate_white(probe);
    const Value chooser_value = choose_first ? white_value : -white_value;
    return {choose_first, bucket, chooser_value, white_value, std::move(result)};
}

}  // namespace

Engine::Engine(std::size_t hash_megabytes) : position_(), tt_(hash_megabytes), search_(tt_) {}

void Engine::new_game() {
    position_.reset();
    tt_.clear();
    search_.clear_memory();
}

SearchResult Engine::go(const SearchLimits& limits) {
    return search_.run(position_, limits);
}

ItemChoiceResult Engine::choose_item(Color chooser, const SearchLimits& limits) {
    ItemChoiceResult choice;
    choice.lines.reserve(3);

    auto better = [&](const ItemChoiceLine& lhs, const ItemChoiceLine& rhs) {
        if (lhs.white_value != rhs.white_value) {
            return chooser == Color::White ? lhs.white_value > rhs.white_value
                                           : lhs.white_value < rhs.white_value;
        }
        return lhs.bucket < rhs.bucket;
    };

    if (!has_explicit_budget(limits)) {
        // Fixed-depth verification mode: search every acquisition candidate to the same depth.
        const SearchLimits probe_limits = divided_limits(limits, 3);
        for (int bucket = 0; bucket < 3; ++bucket)
            choice.lines.push_back(probe_item(position_, chooser, bucket, search_, probe_limits));
    } else {
        // RPSC decision search uses one total budget. First screen all resources, then spend the
        // remaining budget on the currently strongest alternatives. The TT is shared across all
        // probes, so refined candidates reuse board states already discovered during screening.
        // Percentages sum to 100: 15+15+15 screening, then 25+18+12 refinement.
        const SearchLimits screen = scaled_limits(limits, 15, 100);
        for (int bucket = 0; bucket < 3; ++bucket)
            choice.lines.push_back(probe_item(position_, chooser, bucket, search_, screen));
        std::stable_sort(choice.lines.begin(), choice.lines.end(), better);

        constexpr std::array<int, 3> RefinePercent{{25, 18, 12}};
        for (std::size_t rank = 0; rank < choice.lines.size(); ++rank) {
            const int bucket = choice.lines[rank].bucket;
            ItemChoiceLine refined = probe_item(
                position_, chooser, bucket, search_, scaled_limits(limits, RefinePercent[rank], 100));
            choice.lines[rank] = std::move(refined);
        }
    }

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
    SearchResult result = search_.run(probe, limits);
    const Value white_value = result.has_move ? result.value : evaluate_white(probe);
    return {white_value >= 0, white_value, std::move(result)};
}

InitialChoiceResult Engine::choose_initial(const SearchLimits& limits) {
    InitialChoiceResult choice;
    choice.lines.reserve(6);

    auto better = [](const InitialChoiceLine& lhs, const InitialChoiceLine& rhs) {
        if (lhs.chooser_value != rhs.chooser_value) return lhs.chooser_value > rhs.chooser_value;
        if (lhs.choose_first != rhs.choose_first) return lhs.choose_first;
        return lhs.bucket < rhs.bucket;
    };

    if (!has_explicit_budget(limits)) {
        const SearchLimits probe_limits = divided_limits(limits, 6);
        for (bool choose_first : {true, false})
            for (int bucket = 0; bucket < 3; ++bucket)
                choice.lines.push_back(probe_initial(choose_first, bucket, search_, probe_limits));
    } else {
        // The first solo-correct decision has six coupled RPSC alternatives. Give every branch a
        // real screening search, then concentrate the rest of the same total budget on the top
        // three. 6*8 + 24+16+12 = 100 percent. This keeps all six visible while improving the
        // quality of the actions most likely to be chosen.
        const SearchLimits screen = scaled_limits(limits, 8, 100);
        for (bool choose_first : {true, false})
            for (int bucket = 0; bucket < 3; ++bucket)
                choice.lines.push_back(probe_initial(choose_first, bucket, search_, screen));
        std::stable_sort(choice.lines.begin(), choice.lines.end(), better);

        constexpr std::array<int, 3> RefinePercent{{24, 16, 12}};
        for (std::size_t rank = 0; rank < RefinePercent.size(); ++rank) {
            const bool choose_first = choice.lines[rank].choose_first;
            const int bucket = choice.lines[rank].bucket;
            choice.lines[rank] = probe_initial(
                choose_first, bucket, search_, scaled_limits(limits, RefinePercent[rank], 100));
        }
    }

    std::stable_sort(choice.lines.begin(), choice.lines.end(), better);
    if (!choice.lines.empty()) {
        choice.choose_first = choice.lines.front().choose_first;
        choice.best_bucket = choice.lines.front().bucket;
        choice.chooser_value = choice.lines.front().chooser_value;
        choice.white_value = choice.lines.front().white_value;
    }
    return choice;
}

std::uint64_t Engine::perft(int depth) { return rpsc::perft(position_, depth); }

void Engine::clear_search() { tt_.clear(); search_.clear_memory(); }

}  // namespace rpsc
