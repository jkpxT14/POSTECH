#ifndef RPSC_MOVE_H_INCLUDED
#define RPSC_MOVE_H_INCLUDED

#include <array>
#include <cstddef>
#include "types.h"

namespace rpsc {
struct Move {
    PieceId piece = PieceId::W1;
    Item item = Item::None;
    Square push_to = NoSquare;
    std::array<Square, MaxMoveSquares> path{};
    std::uint8_t path_length = 0;

    Square from() const { return path_length ? path[0] : NoSquare; }
    Square to() const { return path_length ? path[path_length - 1] : NoSquare; }
    int roll_count() const { return path_length ? int(path_length) - 1 : 0; }

    bool operator==(const Move& o) const {
        if (piece != o.piece || item != o.item || push_to != o.push_to || path_length != o.path_length)
            return false;
        for (std::uint8_t i = 0; i < path_length; ++i)
            if (path[i] != o.path[i]) return false;
        return true;
    }
    bool operator!=(const Move& o) const { return !(*this == o); }
};

struct MoveHash {
    std::size_t operator()(const Move& m) const noexcept {
        std::size_t h = std::size_t(piece_index(m.piece) + 1);
        h = h * 11u + item_action_index(m.item);
        h = h * 67u + unsigned(m.push_to + 2);
        for (std::uint8_t i = 0; i < m.path_length; ++i) h = h * 67u + unsigned(m.path[i] + 1);
        return h;
    }
};
}  // namespace rpsc

#endif
