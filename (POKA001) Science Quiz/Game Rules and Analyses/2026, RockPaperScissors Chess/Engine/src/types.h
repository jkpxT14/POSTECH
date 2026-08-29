#ifndef RPSC_TYPES_H_INCLUDED
#define RPSC_TYPES_H_INCLUDED

#include <cstdint>

namespace rpsc {

using Key = std::uint64_t;
using Value = int;
using Depth = int;
using Square = std::int8_t;
using Orientation = std::uint8_t;

constexpr Square NoSquare = -1;
constexpr Value ScoreUnit = 100;
constexpr Value CaptureValue = 2 * ScoreUnit;
constexpr Value Infinity = 1000000;

constexpr int BoardSize = 8;
constexpr int PieceCount = 8;
constexpr int MaxMoveSquares = 7;

enum class Color : std::uint8_t { White = 0, Black = 1 };
enum class Gesture : std::uint8_t { Scissors = 0, Rock = 1, Paper = 2 };
enum class Direction : std::uint8_t { North = 0, South = 1, East = 2, West = 3 };
enum class PieceId : std::uint8_t { W1, W2, W3, W4, B1, B2, B3, B4 };
enum class Bound : std::uint8_t { None, Upper, Lower, Exact };

constexpr Color opposite(Color c) { return c == Color::White ? Color::Black : Color::White; }
constexpr int color_index(Color c) { return static_cast<int>(c); }
constexpr int piece_index(PieceId p) { return static_cast<int>(p); }
constexpr Color piece_color(PieceId p) { return piece_index(p) < 4 ? Color::White : Color::Black; }

constexpr int file_of(Square s) { return static_cast<int>(s) & 7; }
constexpr int rank_of(Square s) { return static_cast<int>(s) >> 3; }
constexpr Square make_square(int file, int rank) { return static_cast<Square>(rank * 8 + file); }
constexpr bool valid_square(int file, int rank) {
    return file >= 0 && file < BoardSize && rank >= 0 && rank < BoardSize;
}

constexpr int base_roll_length(Gesture g) {
    return g == Gesture::Scissors ? 3 : g == Gesture::Rock ? 4 : 5;
}

}  // namespace rpsc

#endif
