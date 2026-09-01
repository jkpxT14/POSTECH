#include "evaluate.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <utility>

namespace rpsc {
namespace {

constexpr Value BaseItemReserveValue = 6;
constexpr int ReducedStateCount = 27;
constexpr std::array<std::pair<int, int>, 4> Directions{{{0, 1}, {0, -1}, {1, 0}, {-1, 0}}};
constexpr std::array<Direction, 4> DirectionIds{{Direction::North, Direction::South,
                                                 Direction::East, Direction::West}};

Direction opposite_direction(Direction direction) {
    switch (direction) {
        case Direction::North: return Direction::South;
        case Direction::South: return Direction::North;
        case Direction::East: return Direction::West;
        case Direction::West: return Direction::East;
    }
    return Direction::North;
}

struct ReachProfile {
    std::uint16_t normal = 0;
    std::uint16_t with_push = 0;
    std::uint16_t with_rotation = 0;
    std::uint16_t with_step = 0;
};

using ReachTable = std::array<std::array<ReachProfile, 24>, 64>;
using OutcomeMarks = std::array<bool, 64 * ReducedStateCount>;

void collect_roll_outcomes(int file, int rank, Orientation orientation, int remaining,
                           bool has_last, Direction last, OutcomeMarks& marks) {
    const auto& table = OrientationTable::instance();
    if (remaining == 0) {
        const int state = table.gesture_state_id(orientation);
        marks[static_cast<std::size_t>(make_square(file, rank)) * ReducedStateCount +
              static_cast<std::size_t>(state)] = true;
        return;
    }

    for (std::size_t i = 0; i < Directions.size(); ++i) {
        const Direction direction = DirectionIds[i];
        if (has_last && direction == opposite_direction(last)) continue;
        const int next_file = file + Directions[i].first;
        const int next_rank = rank + Directions[i].second;
        if (!valid_square(next_file, next_rank)) continue;
        collect_roll_outcomes(next_file, next_rank, table.roll(orientation, direction),
                              remaining - 1, true, direction, marks);
    }
}

void add_roll_family(Square start, Orientation orientation, int rolls, OutcomeMarks& marks) {
    collect_roll_outcomes(file_of(start), rank_of(start), orientation, rolls, false,
                          Direction::North, marks);
}

std::uint16_t mark_count(const OutcomeMarks& marks) {
    return static_cast<std::uint16_t>(std::count(marks.begin(), marks.end(), true));
}

ReachTable build_reach_table() {
    ReachTable result{};
    const auto& table = OrientationTable::instance();

    for (int square = 0; square < 64; ++square) {
        const Square start = static_cast<Square>(square);
        for (int oi = 0; oi < 24; ++oi) {
            const Orientation orientation = static_cast<Orientation>(oi);
            const int base = base_roll_length(table.top_gesture(orientation));

            OutcomeMarks normal{};
            add_roll_family(start, orientation, base, normal);

            OutcomeMarks push = normal;
            for (std::size_t i = 0; i < Directions.size(); ++i) {
                const int pushed_file = file_of(start) + Directions[i].first;
                const int pushed_rank = rank_of(start) + Directions[i].second;
                if (!valid_square(pushed_file, pushed_rank)) continue;
                add_roll_family(make_square(pushed_file, pushed_rank), orientation, base, push);
            }

            OutcomeMarks rotation = normal;
            add_roll_family(start, table.rotate_left(orientation), base, rotation);
            add_roll_family(start, table.rotate_right(orientation), base, rotation);

            OutcomeMarks step = normal;
            add_roll_family(start, orientation, base - 1, step);
            add_roll_family(start, orientation, base + 1, step);

            result[static_cast<std::size_t>(square)][static_cast<std::size_t>(oi)] =
                {mark_count(normal), mark_count(push), mark_count(rotation), mark_count(step)};
        }
    }
    return result;
}

const ReachTable& reach_table() {
    static const ReachTable table = build_reach_table();
    return table;
}

const ReachProfile& reach_profile(const PieceState& piece) {
    return reach_table()[static_cast<std::size_t>(piece.square)]
                        [static_cast<std::size_t>(piece.orientation)];
}

int first_step_mobility(const Position& position, PieceId id) {
    const auto& piece = position.piece(id);
    if (!piece.alive()) return 0;
    int mobility = 0;
    const int file = file_of(piece.square), rank = rank_of(piece.square);
    for (const auto& [df, dr] : Directions) {
        if (!valid_square(file + df, rank + dr)) continue;
        if (!position.occupied(make_square(file + df, rank + dr), id)) ++mobility;
    }
    return mobility;
}

int second_step_continuations(const Position& position, PieceId id) {
    const auto& piece = position.piece(id);
    if (!piece.alive()) return 0;
    int continuations = 0;
    const int file = file_of(piece.square), rank = rank_of(piece.square);
    for (const auto& [df, dr] : Directions) {
        const int ff = file + df, fr = rank + dr;
        if (!valid_square(ff, fr) || position.occupied(make_square(ff, fr), id)) continue;
        for (const auto& [ndf, ndr] : Directions) {
            if (ndf == -df && ndr == -dr) continue;
            const int sf = ff + ndf, sr = fr + ndr;
            if (!valid_square(sf, sr)) continue;
            if (!position.occupied(make_square(sf, sr), id)) ++continuations;
        }
    }
    return continuations;
}

Value terminal_match_value(const Position& position) {
    const int score_difference = position.score(Color::White) - position.score(Color::Black);
    if (score_difference) return ScoreUnit * score_difference;
    const int quiz_difference = position.quiz(Color::White) - position.quiz(Color::Black);
    return quiz_difference > 0 ? ScoreUnit / 4 : quiz_difference < 0 ? -ScoreUnit / 4 : 0;
}

int item_reserve_value(const Position& position) {
    const int remaining = position.remaining_board_plies();
    if (remaining < 0) return BaseItemReserveValue;
    return std::max(0, std::min<int>(BaseItemReserveValue, 2 + remaining));
}

int added_reach(const ReachProfile& profile, int bucket) {
    const int normal = profile.normal;
    if (bucket == 0) return static_cast<int>(profile.with_push) - normal;
    if (bucket == 1) return static_cast<int>(profile.with_rotation) - normal;
    return static_cast<int>(profile.with_step) - normal;
}

int inventory_value(const Position& position, Color side, int reserve_value) {
    int value = 0;
    for (int bucket = 0; bucket < 3; ++bucket) {
        const int count = position.item_count(side, bucket);
        if (count <= 0) continue;
        value += reserve_value * count;

        // An item is valuable because it expands the set of strategically distinct actions,
        // not because Push/Rotation/Step has a fixed global hierarchy. Use an obstacle-free,
        // reduced-state reach profile as a cheap horizon heuristic; the actual search still
        // decides whether, where, and when an item should be used.
        int best_added = 0;
        for (int i = 0; i < PieceCount; ++i) {
            const auto id = static_cast<PieceId>(i);
            if (piece_color(id) != side || !position.piece(id).alive()) continue;
            best_added = std::max(best_added, added_reach(reach_profile(position.piece(id)), bucket));
        }
        const int latent = std::min(9, best_added / 10);
        value += reserve_value == 0 ? 0 : latent * reserve_value / BaseItemReserveValue;
    }
    return value;
}

}  // namespace

Value evaluate_white(const Position& position) {
    if (position.remaining_board_plies() == 0) return terminal_match_value(position);

    Value value = ScoreUnit * (position.score(Color::White) - position.score(Color::Black));
    value += 12 * (position.alive_count(Color::White) - position.alive_count(Color::Black));

    const int reserve_value = item_reserve_value(position);
    value += inventory_value(position, Color::White, reserve_value) -
             inventory_value(position, Color::Black, reserve_value);

    for (int i = 0; i < PieceCount; ++i) {
        const auto id = static_cast<PieceId>(i);
        const auto& piece = position.piece(id);
        if (!piece.alive()) continue;

        // Local occupancy mobility catches immediate congestion. Obstacle-free full-Roll reach
        // adds a small RPSC-specific signal for square/orientation quality without invoking the
        // expensive compound move generator at every leaf.
        int mobility = 2 * first_step_mobility(position, id) +
                       second_step_continuations(position, id) / 2;
        mobility += static_cast<int>(reach_profile(piece).normal) / 20;
        value += piece_color(id) == Color::White ? mobility : -mobility;
    }
    return value;
}

Value evaluate(const Position& position) {
    const Value value = evaluate_white(position);
    return position.side_to_move() == Color::White ? value : -value;
}

}  // namespace rpsc
