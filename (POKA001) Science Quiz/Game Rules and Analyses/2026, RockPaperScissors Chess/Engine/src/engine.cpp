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
    if (probe.nodes) probe.nodes = std::max<std::uint64_t>(1, probe.nodes * numerator / denominator);
    if (probe.movetime.count() > 0)
        probe.movetime = std::chrono::milliseconds(
            std::max<std::int64_t>(1, probe.movetime.count() * numerator / denominator));
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
    // One quiz has been answered and no board move occurred; at most 19 equal-result
    // questions remain, hence 38 future board plies in this deterministic board model.
    probe.set_match_context(choose_first ? 1 : 0, choose_first ? 0 : 1, 38);
    probe.set_side_to_move(Color::White);
    SearchResult result = search.run(probe, limits);
    const Value white_value = result.has_move ? result.value : evaluate_white(probe);
    return {choose_first, bucket, choose_first ? white_value : -white_value,
            white_value, std::move(result)};
}
}  // namespace

Engine::Engine(std::size_t hash_megabytes) : position_(), tt_(hash_megabytes), search_(tt_) {}
void Engine::new_game() {
    position_.reset();
    tt_.clear();
    search_.clear_memory();
}
SearchResult Engine::go(const SearchLimits& limits) { return search_.run(position_, limits); }

ItemChoiceResult Engine::choose_item(Color chooser, const SearchLimits& limits) {
    ItemChoiceResult choice;
    choice.lines.reserve(3);
    auto better = [&](const ItemChoiceLine& a, const ItemChoiceLine& b) {
        if (a.white_value != b.white_value)
            return chooser == Color::White ? a.white_value > b.white_value : a.white_value < b.white_value;
        return a.bucket < b.bucket;
    };
    if (!has_explicit_budget(limits)) {
        const auto probe = divided_limits(limits, 3);
        for (int bucket = 0; bucket < 3; ++bucket)
            choice.lines.push_back(probe_item(position_, chooser, bucket, search_, probe));
    } else {
        const auto screen = scaled_limits(limits, 15, 100);
        for (int bucket = 0; bucket < 3; ++bucket)
            choice.lines.push_back(probe_item(position_, chooser, bucket, search_, screen));
        std::stable_sort(choice.lines.begin(), choice.lines.end(), better);
        constexpr std::array<int, 3> pct{{25, 18, 12}};
        for (std::size_t rank = 0; rank < choice.lines.size(); ++rank) {
            int bucket = choice.lines[rank].bucket;
            choice.lines[rank] = probe_item(position_, chooser, bucket, search_,
                                             scaled_limits(limits, pct[rank], 100));
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
    auto result = search_.run(probe, limits);
    Value white = result.has_move ? result.value : evaluate_white(probe);
    return {white >= 0, white, std::move(result)};
}

InitialChoiceResult Engine::choose_initial(const SearchLimits& limits) {
    InitialChoiceResult choice;
    choice.lines.reserve(6);
    auto better = [](const InitialChoiceLine& a, const InitialChoiceLine& b) {
        if (a.chooser_value != b.chooser_value) return a.chooser_value > b.chooser_value;
        if (a.choose_first != b.choose_first) return a.choose_first;
        return a.bucket < b.bucket;
    };
    if (!has_explicit_budget(limits)) {
        auto probe = divided_limits(limits, 6);
        for (bool first : {true, false})
            for (int bucket = 0; bucket < 3; ++bucket)
                choice.lines.push_back(probe_initial(first, bucket, search_, probe));
    } else {
        auto screen = scaled_limits(limits, 8, 100);
        for (bool first : {true, false})
            for (int bucket = 0; bucket < 3; ++bucket)
                choice.lines.push_back(probe_initial(first, bucket, search_, screen));
        std::stable_sort(choice.lines.begin(), choice.lines.end(), better);
        constexpr std::array<int, 3> pct{{24, 16, 12}};
        for (std::size_t rank = 0; rank < pct.size(); ++rank) {
            bool first = choice.lines[rank].choose_first;
            int bucket = choice.lines[rank].bucket;
            choice.lines[rank] = probe_initial(first, bucket, search_,
                                                scaled_limits(limits, pct[rank], 100));
        }
    }
    std::stable_sort(choice.lines.begin(), choice.lines.end(), better);
    if (!choice.lines.empty()) {
        const auto& x = choice.lines.front();
        choice.choose_first = x.choose_first;
        choice.best_bucket = x.bucket;
        choice.chooser_value = x.chooser_value;
        choice.white_value = x.white_value;
    }
    return choice;
}

std::uint64_t Engine::perft(int depth) { return rpsc::perft(position_, depth); }
void Engine::clear_search() {
    tt_.clear();
    search_.clear_memory();
}
}  // namespace rpsc
