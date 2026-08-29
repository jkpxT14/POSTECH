#ifndef RPSC_MOVE_H_INCLUDED
#define RPSC_MOVE_H_INCLUDED

#include <array>
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
    int roll_count() const { return path_length ? static_cast<int>(path_length) - 1 : 0; }

    bool operator==(const Move& other) const {
        if (piece != other.piece || item != other.item || push_to != other.push_to ||
            path_length != other.path_length)
            return false;
        for (std::uint8_t i = 0; i < path_length; ++i)
            if (path[i] != other.path[i]) return false;
        return true;
    }
    bool operator!=(const Move& other) const { return !(*this == other); }
};

struct MoveHash {
    std::size_t operator()(const Move& move) const noexcept {
        std::size_t hash = static_cast<std::size_t>(piece_index(move.piece) + 1);
        hash = hash * 7u + static_cast<unsigned>(move.item);
        hash = hash * 67u + static_cast<unsigned>(move.push_to + 2);
        for (std::uint8_t i = 0; i < move.path_length; ++i)
            hash = hash * 67u + static_cast<unsigned>(move.path[i] + 1);
        return hash;
    }
};

}  // namespace rpsc

#endif
