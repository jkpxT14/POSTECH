#include "position.h"

#include <cmath>
#include <sstream>
#include <stdexcept>

namespace rpsc {
namespace {

Direction direction_between(Square from, Square to) {
    const int dx = file_of(to) - file_of(from);
    const int dy = rank_of(to) - rank_of(from);
    if (dx == 0 && dy == 1) return Direction::North;
    if (dx == 0 && dy == -1) return Direction::South;
    if (dx == 1 && dy == 0) return Direction::East;
    if (dx == -1 && dy == 0) return Direction::West;
    throw std::invalid_argument("non-orthogonal one-square path step");
}

Direction opposite_dir(Direction direction) {
    switch (direction) {
        case Direction::North: return Direction::South;
        case Direction::South: return Direction::North;
        case Direction::East: return Direction::West;
        case Direction::West: return Direction::East;
    }
    return Direction::North;
}

bool orthogonally_adjacent(Square first, Square second) {
    return std::abs(file_of(first) - file_of(second)) +
               std::abs(rank_of(first) - rank_of(second)) == 1;
}

int combat(Gesture first, Gesture second) {
    if (first == second) return 0;
    if ((first == Gesture::Scissors && second == Gesture::Paper) ||
        (first == Gesture::Rock && second == Gesture::Scissors) ||
        (first == Gesture::Paper && second == Gesture::Rock))
        return 1;
    return -1;
}

std::uint64_t splitmix64(std::uint64_t value) {
    value += 0x9e3779b97f4a7c15ULL;
    value = (value ^ (value >> 30)) * 0xbf58476d1ce4e5b9ULL;
    value = (value ^ (value >> 27)) * 0x94d049bb133111ebULL;
    return value ^ (value >> 31);
}

Orientation apply_rotation_item(Orientation orientation, Item item) {
    const auto& table = OrientationTable::instance();
    if (item == Item::RotateLeft) return table.rotate_left(orientation);
    if (item == Item::RotateRight) return table.rotate_right(orientation);
    return orientation;
}

int required_rolls(Orientation orientation, Item item) {
    int rolls = base_roll_length(OrientationTable::instance().top_gesture(orientation));
    if (item == Item::StepShort) --rolls;
    if (item == Item::StepLong) ++rolls;
    return rolls;
}

}  // namespace

Position::Position() { reset(); }

void Position::reset_pieces() {
    const auto& orientations = OrientationTable::instance();
    const Orientation white_scissors = orientations.canonical(Gesture::Scissors, WristDirection::South);
    const Orientation white_rock = orientations.canonical(Gesture::Rock, WristDirection::South);
    const Orientation white_paper = orientations.canonical(Gesture::Paper, WristDirection::South);
    const Orientation black_scissors = orientations.canonical(Gesture::Scissors, WristDirection::North);
    const Orientation black_rock = orientations.canonical(Gesture::Rock, WristDirection::North);
    const Orientation black_paper = orientations.canonical(Gesture::Paper, WristDirection::North);

    pieces_[0] = {make_square(0, 0), white_scissors};
    pieces_[1] = {make_square(2, 0), white_rock};
    pieces_[2] = {make_square(4, 0), white_paper};
    pieces_[3] = {make_square(6, 0), white_scissors};
    pieces_[4] = {make_square(7, 7), black_scissors};
    pieces_[5] = {make_square(5, 7), black_rock};
    pieces_[6] = {make_square(3, 7), black_paper};
    pieces_[7] = {make_square(1, 7), black_scissors};
}

void Position::reset() {
    reset_pieces();
    side_to_move_ = Color::White;
    captures_white_ = 0;
    captures_black_ = 0;
    items_ = {};
}

bool Position::occupied(Square square, PieceId exclude) const {
    for (int i = 0; i < PieceCount; ++i) {
        if (i == piece_index(exclude)) continue;
        if (pieces_[i].alive() && pieces_[i].square == square) return true;
    }
    return false;
}

int Position::adjacent_enemies(Square square, Color side, PieceId exclude) const {
    int count = 0;
    for (int i = 0; i < PieceCount; ++i) {
        const auto id = static_cast<PieceId>(i);
        const auto& piece = pieces_[i];
        if (id == exclude || !piece.alive() || piece_color(id) == side) continue;
        const int distance = std::abs(file_of(piece.square) - file_of(square)) +
                             std::abs(rank_of(piece.square) - rank_of(square));
        if (distance == 1) ++count;
    }
    return count;
}

PieceId Position::single_adjacent_enemy(Square square, Color side, PieceId exclude) const {
    PieceId found = PieceId::W1;
    int count = 0;
    for (int i = 0; i < PieceCount; ++i) {
        const auto id = static_cast<PieceId>(i);
        const auto& piece = pieces_[i];
        if (id == exclude || !piece.alive() || piece_color(id) == side) continue;
        const int distance = std::abs(file_of(piece.square) - file_of(square)) +
                             std::abs(rank_of(piece.square) - rank_of(square));
        if (distance == 1) { found = id; ++count; }
    }
    if (count != 1) throw std::logic_error("expected exactly one adjacent enemy");
    return found;
}

int Position::alive_count(Color side) const {
    int count = 0;
    for (int i = 0; i < PieceCount; ++i) {
        const auto id = static_cast<PieceId>(i);
        if (pieces_[i].alive() && piece_color(id) == side) ++count;
    }
    return count;
}

bool Position::is_legal_path(const Move& move) const {
    const PieceId id = move.piece;
    const auto& piece_state = piece(id);
    if (!piece_state.alive() || piece_color(id) != side_to_move_ || move.path_length < 2 ||
        move.from() != piece_state.square) return false;

    const int bucket = item_bucket(move.item);
    if (bucket >= 0 && item_count(side_to_move_, bucket) <= 0) return false;
    if (move.item != Item::Push && move.push_to != NoSquare) return false;

    Orientation orientation = apply_rotation_item(piece_state.orientation, move.item);
    if (move.roll_count() != required_rolls(orientation, move.item)) return false;

    Square roll_from = move.from();
    if (move.item == Item::Push) {
        if (move.push_to == NoSquare || !orthogonally_adjacent(move.from(), move.push_to) || occupied(move.push_to, id)) return false;
        roll_from = move.push_to;
    }

    Direction last{};
    bool has_last = false;
    for (std::uint8_t i = 1; i < move.path_length; ++i) {
        Direction direction;
        try { direction = direction_between(roll_from, move.path[i]); }
        catch (...) { return false; }
        if (has_last && direction == opposite_dir(last)) return false;
        if (occupied(move.path[i], id)) return false;
        if (i + 1 == move.path_length && adjacent_enemies(move.path[i], side_to_move_, id) >= 2) return false;
        orientation = OrientationTable::instance().roll(orientation, direction);
        roll_from = move.path[i]; last = direction; has_last = true;
    }
    return true;
}

MoveOutcome Position::do_move(const Move& move, UndoState& undo) {
    if (!is_legal_path(move)) throw std::invalid_argument("illegal RPSC move");
    undo.pieces = pieces_; undo.side_to_move = side_to_move_; undo.captures_white = captures_white_;
    undo.captures_black = captures_black_; undo.items = items_;

    auto& moving_piece = piece(move.piece);
    const Color mover = side_to_move_;
    Orientation orientation = apply_rotation_item(moving_piece.orientation, move.item);
    Square roll_from = move.item == Item::Push ? move.push_to : move.from();
    const int bucket = item_bucket(move.item);
    if (bucket >= 0) --items_[color_index(mover)][bucket];
    for (std::uint8_t i = 1; i < move.path_length; ++i) {
        const Direction direction = direction_between(roll_from, move.path[i]);
        orientation = OrientationTable::instance().roll(orientation, direction);
        roll_from = move.path[i];
    }
    moving_piece.square = move.to(); moving_piece.orientation = orientation;

    MoveOutcome outcome;
    if (adjacent_enemies(moving_piece.square, mover, move.piece) == 1) {
        const PieceId enemy_id = single_adjacent_enemy(moving_piece.square, mover, move.piece);
        auto& enemy = piece(enemy_id);
        const auto& orientations = OrientationTable::instance();
        const int result = combat(orientations.top_gesture(moving_piece.orientation), orientations.top_gesture(enemy.orientation));
        if (result != 0) {
            outcome.has_capture = true; outcome.captured = result > 0 ? enemy_id : move.piece;
            const Color winner = result > 0 ? mover : opposite(mover);
            piece(outcome.captured).square = NoSquare;
            if (winner == Color::White) ++captures_white_; else ++captures_black_;
        }
    }
    if (alive_count(Color::White) == 0 || alive_count(Color::Black) == 0) { reset_pieces(); outcome.reset = true; }
    side_to_move_ = opposite(side_to_move_);
    return outcome;
}

void Position::undo_move(const UndoState& undo) {
    pieces_ = undo.pieces; side_to_move_ = undo.side_to_move; captures_white_ = undo.captures_white;
    captures_black_ = undo.captures_black; items_ = undo.items;
}

Key Position::key() const {
    Key key = 0;
    for (int i = 0; i < PieceCount; ++i) {
        const auto& piece = pieces_[i];
        const std::uint64_t square = piece.alive() ? static_cast<unsigned>(piece.square) : 64u;
        const std::uint64_t feature = (static_cast<std::uint64_t>(i) * 65u + square) * 24u + piece.orientation;
        key ^= splitmix64(0x123456789abcdef0ULL + feature);
    }
    if (side_to_move_ == Color::Black) key ^= splitmix64(0x3141592653589793ULL);
    key ^= splitmix64(0x1111000000000000ULL + static_cast<unsigned>(captures_white_));
    key ^= splitmix64(0x2222000000000000ULL + static_cast<unsigned>(captures_black_));
    for (int side = 0; side < 2; ++side) for (int bucket = 0; bucket < 3; ++bucket) {
        const std::uint64_t base = 0x5555000000000000ULL + static_cast<std::uint64_t>(side * 3 + bucket) * 0x10000ULL;
        key ^= splitmix64(base + static_cast<unsigned>(items_[side][bucket]));
    }
    return key;
}

Key Position::search_key() const {
    Key key = 0;
    const auto& orientations = OrientationTable::instance();
    for (int i = 0; i < PieceCount; ++i) {
        const auto& piece = pieces_[i];
        const std::uint64_t square = piece.alive() ? static_cast<unsigned>(piece.square) : 64u;
        const std::uint64_t state = orientations.gesture_state_id(piece.orientation);
        const std::uint64_t feature = (static_cast<std::uint64_t>(i) * 65u + square) * 32u + state;
        key ^= splitmix64(0xabcdef0123456789ULL + feature);
    }
    if (side_to_move_ == Color::Black) key ^= splitmix64(0x2718281828459045ULL);
    key ^= splitmix64(0x3333000000000000ULL + static_cast<unsigned>(captures_white_));
    key ^= splitmix64(0x4444000000000000ULL + static_cast<unsigned>(captures_black_));
    for (int side = 0; side < 2; ++side) for (int bucket = 0; bucket < 3; ++bucket) {
        const std::uint64_t base = 0x7777000000000000ULL + static_cast<std::uint64_t>(side * 3 + bucket) * 0x10000ULL;
        key ^= splitmix64(base + static_cast<unsigned>(items_[side][bucket]));
    }
    return key;
}

std::string Position::debug_string() const {
    std::ostringstream out;
    out << (side_to_move_ == Color::White ? "White" : "Black") << " to move | captures "
        << captures_white_ << '-' << captures_black_ << " | items W " << items_[0][0] << ','
        << items_[0][1] << ',' << items_[0][2] << " B " << items_[1][0] << ',' << items_[1][1]
        << ',' << items_[1][2] << '\n';
    const auto& orientations = OrientationTable::instance();
    for (int i = 0; i < PieceCount; ++i) {
        const auto& piece = pieces_[i];
        out << (i < 4 ? 'W' : 'B') << (i % 4 + 1) << ' ';
        if (!piece.alive()) out << "off";
        else out << char('a' + file_of(piece.square)) << (rank_of(piece.square) + 1) << " top="
                 << "SRP"[static_cast<int>(orientations.top_gesture(piece.orientation))]
                 << " ori=" << int(piece.orientation);
        out << '\n';
    }
    return out.str();
}

}  // namespace rpsc
