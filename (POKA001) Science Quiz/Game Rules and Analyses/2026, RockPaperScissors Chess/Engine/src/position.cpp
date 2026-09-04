#include "position.h"

#include <cmath>
#include <sstream>
#include <stdexcept>

namespace rpsc {
namespace {
Direction direction_between(Square a, Square b) {
    const int dx = file_of(b) - file_of(a), dy = rank_of(b) - rank_of(a);
    if (dx == 0 && dy == 1) return Direction::North;
    if (dx == 0 && dy == -1) return Direction::South;
    if (dx == 1 && dy == 0) return Direction::East;
    if (dx == -1 && dy == 0) return Direction::West;
    throw std::invalid_argument("non-orthogonal step");
}
Direction opposite_dir(Direction d) {
    switch (d) {
        case Direction::North: return Direction::South;
        case Direction::South: return Direction::North;
        case Direction::East: return Direction::West;
        case Direction::West: return Direction::East;
    }
    return Direction::North;
}
bool adjacent(Square a, Square b) {
    return std::abs(file_of(a) - file_of(b)) + std::abs(rank_of(a) - rank_of(b)) == 1;
}
int combat(Gesture a, Gesture b) {
    if (a == b) return 0;
    if ((a == Gesture::Scissors && b == Gesture::Paper) ||
        (a == Gesture::Rock && b == Gesture::Scissors) ||
        (a == Gesture::Paper && b == Gesture::Rock))
        return 1;
    return -1;
}
std::uint64_t mix(std::uint64_t x) {
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return x ^ (x >> 31);
}
Orientation modifier_orientation(Orientation o, Item item) {
    return OrientationTable::instance().apply_rotation(o, item);
}
int required_rolls(Orientation original, Item item) {
    const auto& t = OrientationTable::instance();
    Orientation o = modifier_orientation(original, item);
    int n = base_roll_length(t.top_gesture(o));
    if (item == Item::StepShort) --n;
    if (item == Item::StepLong) ++n;
    return n;
}
}  // namespace

Position::Position() { reset(); }

void Position::reset_pieces() {
    const auto& t = OrientationTable::instance();
    pieces_[0] = {make_square(0, 0), t.canonical(Gesture::Scissors, WristDirection::South)};
    pieces_[1] = {make_square(2, 0), t.canonical(Gesture::Rock, WristDirection::South)};
    pieces_[2] = {make_square(4, 0), t.canonical(Gesture::Paper, WristDirection::South)};
    pieces_[3] = {make_square(6, 0), t.canonical(Gesture::Scissors, WristDirection::South)};
    pieces_[4] = {make_square(7, 7), t.canonical(Gesture::Scissors, WristDirection::North)};
    pieces_[5] = {make_square(5, 7), t.canonical(Gesture::Rock, WristDirection::North)};
    pieces_[6] = {make_square(3, 7), t.canonical(Gesture::Paper, WristDirection::North)};
    pieces_[7] = {make_square(1, 7), t.canonical(Gesture::Scissors, WristDirection::North)};
}

void Position::reset() {
    reset_pieces();
    side_to_move_ = Color::White;
    captures_white_ = captures_black_ = 0;
    items_ = {};
    quiz_white_ = quiz_black_ = 0;
    remaining_board_plies_ = -1;
}

bool Position::occupied(Square s, PieceId ex) const {
    for (int i = 0; i < PieceCount; ++i)
        if (i != piece_index(ex) && pieces_[i].alive() && pieces_[i].square == s) return true;
    return false;
}

int Position::adjacent_enemies(Square s, Color side, PieceId ex) const {
    int count = 0;
    for (int i = 0; i < PieceCount; ++i) {
        const auto id = PieceId(i);
        if (id == ex || !pieces_[i].alive() || piece_color(id) == side) continue;
        if (adjacent(s, pieces_[i].square)) ++count;
    }
    return count;
}

PieceId Position::single_adjacent_enemy(Square s, Color side, PieceId ex) const {
    PieceId found = PieceId::W1;
    int count = 0;
    for (int i = 0; i < PieceCount; ++i) {
        const auto id = PieceId(i);
        if (id == ex || !pieces_[i].alive() || piece_color(id) == side) continue;
        if (adjacent(s, pieces_[i].square)) {
            found = id;
            ++count;
        }
    }
    if (count != 1) throw std::logic_error("expected one enemy");
    return found;
}

int Position::alive_count(Color side) const {
    int count = 0;
    for (int i = 0; i < PieceCount; ++i)
        if (pieces_[i].alive() && piece_color(PieceId(i)) == side) ++count;
    return count;
}

bool Position::is_legal_path(const Move& m) const {
    const auto id = m.piece;
    const auto& p = piece(id);
    if (!p.alive() || piece_color(id) != side_to_move_ || m.path_length < 2 || m.from() != p.square) return false;
    if (m.path_length > MaxMoveSquares) return false;
    const int bucket = item_bucket(m.item);
    if (bucket >= 0 && item_count(side_to_move_, bucket) <= 0) return false;
    if (m.item != Item::Push && m.push_to != NoSquare) return false;

    Orientation o = modifier_orientation(p.orientation, m.item);
    if (m.roll_count() != required_rolls(p.orientation, m.item)) return false;

    Square from = m.from();
    Direction last = Direction::North;
    bool has_last = false;
    if (m.item == Item::Push) {
        if (m.push_to == NoSquare || !adjacent(m.from(), m.push_to) || occupied(m.push_to, id)) return false;
        try { last = direction_between(m.from(), m.push_to); }
        catch (...) { return false; }
        has_last = true;  // Push participates in immediate-backtracking prohibition.
        from = m.push_to;
    }

    for (std::uint8_t i = 1; i < m.path_length; ++i) {
        Direction d;
        try { d = direction_between(from, m.path[i]); }
        catch (...) { return false; }
        if (has_last && d == opposite_dir(last)) return false;
        if (occupied(m.path[i], id)) return false;
        if (i + 1 == m.path_length && adjacent_enemies(m.path[i], side_to_move_, id) >= 2) return false;
        o = OrientationTable::instance().roll(o, d);
        from = m.path[i];
        last = d;
        has_last = true;
    }
    return true;
}

MoveOutcome Position::do_move(const Move& m, UndoState& undo) {
    if (!is_legal_path(m)) throw std::invalid_argument("illegal RPSC move");
    undo = {pieces_, side_to_move_, captures_white_, captures_black_, items_, quiz_white_, quiz_black_, remaining_board_plies_};

    auto& moving = piece(m.piece);
    const Color mover = side_to_move_;
    Orientation o = modifier_orientation(moving.orientation, m.item);
    Square from = m.item == Item::Push ? m.push_to : m.from();
    const int bucket = item_bucket(m.item);
    if (bucket >= 0) --items_[color_index(mover)][bucket];

    for (std::uint8_t i = 1; i < m.path_length; ++i) {
        const auto d = direction_between(from, m.path[i]);
        o = OrientationTable::instance().roll(o, d);
        from = m.path[i];
    }
    moving.square = m.to();
    moving.orientation = o;

    MoveOutcome out;
    if (adjacent_enemies(moving.square, mover, m.piece) == 1) {
        const auto enemy = single_adjacent_enemy(moving.square, mover, m.piece);
        const auto& t = OrientationTable::instance();
        const int result = combat(t.top_gesture(moving.orientation), t.top_gesture(piece(enemy).orientation));
        if (result != 0) {
            out.has_capture = true;
            out.captured = result > 0 ? enemy : m.piece;
            const Color winner = result > 0 ? mover : opposite(mover);
            piece(out.captured).square = NoSquare;
            if (winner == Color::White) ++captures_white_;
            else ++captures_black_;
        }
    }

    if (alive_count(Color::White) == 0 || alive_count(Color::Black) == 0) {
        reset_pieces();
        out.reset = true;
    }
    side_to_move_ = opposite(side_to_move_);
    if (remaining_board_plies_ > 0) --remaining_board_plies_;
    return out;
}

void Position::undo_move(const UndoState& u) {
    pieces_ = u.pieces;
    side_to_move_ = u.side_to_move;
    captures_white_ = u.captures_white;
    captures_black_ = u.captures_black;
    items_ = u.items;
    quiz_white_ = u.quiz_white;
    quiz_black_ = u.quiz_black;
    remaining_board_plies_ = u.remaining_board_plies;
}

Key Position::key() const {
    Key k = 0;
    for (int i = 0; i < PieceCount; ++i) {
        const auto& p = pieces_[i];
        const std::uint64_t sq = p.alive() ? unsigned(p.square) : 64u;
        // A removed piece has no rule-relevant orientation until a Reset, which
        // recreates all pieces in the canonical initial orientation. Canonicalize
        // dead pieces so equivalent capture paths share the same exact key.
        const std::uint64_t orientation = p.alive() ? p.orientation : 0u;
        const std::uint64_t f = (std::uint64_t(i) * 65u + sq) * 24u + orientation;
        k ^= mix(0x123456789abcdef0ULL + f);
    }
    if (side_to_move_ == Color::Black) k ^= mix(0x3141592653589793ULL);
    k ^= mix(0x1111000000000000ULL + unsigned(captures_white_));
    k ^= mix(0x2222000000000000ULL + unsigned(captures_black_));
    k ^= mix(0x2323000000000000ULL + unsigned(quiz_white_));
    k ^= mix(0x2424000000000000ULL + unsigned(quiz_black_));
    k ^= mix(0x2525000000000000ULL + unsigned(remaining_board_plies_ + 1));
    for (int s = 0; s < 2; ++s)
        for (int b = 0; b < 3; ++b)
            k ^= mix(0x5555000000000000ULL + std::uint64_t(s * 3 + b) * 0x10000ULL + unsigned(items_[s][b]));
    return k;
}

Key Position::search_key() const {
    Key k = 0;
    const auto& t = OrientationTable::instance();
    for (int i = 0; i < PieceCount; ++i) {
        const auto& p = pieces_[i];
        const std::uint64_t sq = p.alive() ? unsigned(p.square) : 64u;
        // The reduced search state likewise ignores the orientation of a removed
        // piece; retaining it would split one strategic state into several TT keys.
        const std::uint64_t st = p.alive() ? t.gesture_state_id(p.orientation) : 0u;
        const std::uint64_t f = (std::uint64_t(i) * 65u + sq) * 32u + st;
        k ^= mix(0xabcdef0123456789ULL + f);
    }
    if (side_to_move_ == Color::Black) k ^= mix(0x2718281828459045ULL);
    k ^= mix(0x3333000000000000ULL + unsigned(captures_white_));
    k ^= mix(0x4444000000000000ULL + unsigned(captures_black_));
    k ^= mix(0x4545000000000000ULL + unsigned(quiz_white_));
    k ^= mix(0x4646000000000000ULL + unsigned(quiz_black_));
    k ^= mix(0x4747000000000000ULL + unsigned(remaining_board_plies_ + 1));
    for (int s = 0; s < 2; ++s)
        for (int b = 0; b < 3; ++b)
            k ^= mix(0x7777000000000000ULL + std::uint64_t(s * 3 + b) * 0x10000ULL + unsigned(items_[s][b]));
    return k;
}

std::string Position::debug_string() const {
    std::ostringstream out;
    out << (side_to_move_ == Color::White ? "White" : "Black") << " to move | captures "
        << captures_white_ << '-' << captures_black_ << " | quiz " << quiz_white_ << '-' << quiz_black_
        << " | remaining plies " << remaining_board_plies_ << '\n';
    return out.str();
}
}  // namespace rpsc
