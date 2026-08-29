#include "evaluate.h"

#include <array>
#include <utility>

namespace rpsc {
namespace {

int first_step_mobility(const Position& position, PieceId id) {
    const auto& piece = position.piece(id);
    if (!piece.alive()) return 0;

    int mobility = 0;
    const int file = file_of(piece.square);
    const int rank = rank_of(piece.square);
    constexpr std::array<std::pair<int, int>, 4> directions{{{0, 1}, {0, -1}, {1, 0}, {-1, 0}}};
    for (const auto& [df, dr] : directions) {
        if (!valid_square(file + df, rank + dr)) continue;
        if (!position.occupied(make_square(file + df, rank + dr), id)) ++mobility;
    }
    return mobility;
}

}  // namespace

Value evaluate_white(const Position& position) {
    Value value = CaptureValue *
                  (position.captures(Color::White) - position.captures(Color::Black));

    // Small positional terms break otherwise equal leaves without overriding the official
    // score scale. They intentionally avoid unverified opening/center/gesture preferences.
    value += 12 * (position.alive_count(Color::White) - position.alive_count(Color::Black));

    for (int i = 0; i < PieceCount; ++i) {
        const auto id = static_cast<PieceId>(i);
        if (!position.piece(id).alive()) continue;
        const int mobility = 2 * first_step_mobility(position, id);
        value += piece_color(id) == Color::White ? mobility : -mobility;
    }

    return value;
}

Value evaluate(const Position& position) {
    const Value value = evaluate_white(position);
    return position.side_to_move() == Color::White ? value : -value;
}

}  // namespace rpsc
