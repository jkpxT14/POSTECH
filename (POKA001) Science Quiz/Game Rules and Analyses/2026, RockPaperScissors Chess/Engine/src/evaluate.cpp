#include "evaluate.h"

#include <array>
#include <utility>

namespace rpsc {
namespace {

constexpr Value ItemReserveValue = 6;

constexpr std::array<std::pair<int, int>, 4> Directions{{{0, 1}, {0, -1}, {1, 0}, {-1, 0}}};

int first_step_mobility(const Position& position, PieceId id) {
    const auto& piece = position.piece(id);
    if (!piece.alive()) return 0;

    int mobility = 0;
    const int file = file_of(piece.square);
    const int rank = rank_of(piece.square);
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
    const int file = file_of(piece.square);
    const int rank = rank_of(piece.square);
    for (const auto& [df, dr] : Directions) {
        const int first_file = file + df;
        const int first_rank = rank + dr;
        if (!valid_square(first_file, first_rank) ||
            position.occupied(make_square(first_file, first_rank), id)) {
            continue;
        }

        for (const auto& [next_df, next_dr] : Directions) {
            // A Roll may not immediately return to the previous square.
            if (next_df == -df && next_dr == -dr) continue;
            const int second_file = first_file + next_df;
            const int second_rank = first_rank + next_dr;
            if (!valid_square(second_file, second_rank)) continue;
            if (!position.occupied(make_square(second_file, second_rank), id)) ++continuations;
        }
    }
    return continuations;
}

}  // namespace

Value evaluate_white(const Position& position) {
    Value value = CaptureValue *
                  (position.captures(Color::White) - position.captures(Color::Black));

    // Small positional terms break otherwise equal leaves without overriding the official
    // score scale. They intentionally avoid unverified opening/center/gesture preferences.
    value += 12 * (position.alive_count(Color::White) - position.alive_count(Color::Black));

    // A small equal reserve term prevents finite-horizon searches from treating an unused
    // item as literally free to spend. It is deliberately far smaller than a scored capture
    // and does not claim that Push, Rotation, and Step have calibrated strategic values.
    int white_items = 0;
    int black_items = 0;
    for (int bucket = 0; bucket < 3; ++bucket) {
        white_items += position.item_count(Color::White, bucket);
        black_items += position.item_count(Color::Black, bucket);
    }
    value += ItemReserveValue * (white_items - black_items);

    for (int i = 0; i < PieceCount; ++i) {
        const auto id = static_cast<PieceId>(i);
        if (!position.piece(id).alive()) continue;
        // Reward local path flexibility rather than an unverified board-region preference.
        // The continuation term follows the no-immediate-reverse Roll rule and stays small
        // relative to the official capture score.
        const int mobility = 2 * first_step_mobility(position, id) +
                             second_step_continuations(position, id) / 2;
        value += piece_color(id) == Color::White ? mobility : -mobility;
    }

    return value;
}

Value evaluate(const Position& position) {
    const Value value = evaluate_white(position);
    return position.side_to_move() == Color::White ? value : -value;
}

}  // namespace rpsc
