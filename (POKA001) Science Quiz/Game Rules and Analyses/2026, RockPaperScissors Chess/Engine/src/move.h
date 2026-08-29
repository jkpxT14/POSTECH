#ifndef RPSC_MOVE_H_INCLUDED
#define RPSC_MOVE_H_INCLUDED

#include <array>
#include "types.h"

namespace rpsc {

struct Move {
    PieceId piece = PieceId::W1;
    std::array<Square, MaxMoveSquares> path{};
    std::uint8_t path_length = 0;

    Square from() const { return path_length ? path[0] : NoSquare; }
    Square to() const { return path_length ? path[path_length - 1] : NoSquare; }
    int roll_count() const { return path_length ? static_cast<int>(path_length) - 1 : 0; }

    bool operator==(const Move& other) const {
        if (piece != other.piece || path_length != other.path_length) return false;
        for (std::uint8_t i = 0; i < path_length; ++i)
            if (path[i] != other.path[i]) return false;
        return true;
    }
    bool operator!=(const Move& other) const { return !(*this == other); }
};

struct MoveHash {
    std::size_t operator()(const Move& m) const noexcept {
        std::size_t h = static_cast<std::size_t>(piece_index(m.piece) + 1);
        for (std::uint8_t i = 0; i < m.path_length; ++i)
            h = h * 67u + static_cast<unsigned>(m.path[i] + 1);
        return h;
    }
};

}  // namespace rpsc

#endif
