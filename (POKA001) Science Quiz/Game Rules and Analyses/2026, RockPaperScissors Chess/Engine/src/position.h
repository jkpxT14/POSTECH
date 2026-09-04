#ifndef RPSC_POSITION_H_INCLUDED
#define RPSC_POSITION_H_INCLUDED

#include <array>
#include <string>
#include "move.h"
#include "orientation.h"

namespace rpsc {
struct PieceState {
    Square square = NoSquare;
    Orientation orientation = 0;
    bool alive() const { return square != NoSquare; }
};

struct UndoState {
    std::array<PieceState, PieceCount> pieces;
    Color side_to_move = Color::White;
    int captures_white = 0, captures_black = 0;
    std::array<std::array<int, 3>, 2> items{};
    int quiz_white = 0, quiz_black = 0, remaining_board_plies = -1;
};

struct MoveOutcome {
    PieceId captured = PieceId::W1;
    bool has_capture = false, reset = false;
};

class Position {
   public:
    Position();
    void reset();
    Color side_to_move() const { return side_to_move_; }
    void set_side_to_move(Color c) { side_to_move_ = c; }
    const PieceState& piece(PieceId id) const { return pieces_[piece_index(id)]; }
    PieceState& piece(PieceId id) { return pieces_[piece_index(id)]; }
    const auto& pieces() const { return pieces_; }
    bool occupied(Square, PieceId) const;
    int adjacent_enemies(Square, Color, PieceId) const;
    PieceId single_adjacent_enemy(Square, Color, PieceId) const;
    int captures(Color c) const { return c == Color::White ? captures_white_ : captures_black_; }
    int quiz(Color c) const { return c == Color::White ? quiz_white_ : quiz_black_; }
    int score(Color c) const { return quiz(c) + 2 * captures(c); }
    void set_match_context(int qw, int qb, int r) { quiz_white_ = qw; quiz_black_ = qb; remaining_board_plies_ = r; }
    int remaining_board_plies() const { return remaining_board_plies_; }
    int alive_count(Color) const;
    int item_count(Color s, int b) const { return items_[color_index(s)][b]; }
    int item_count(Color s, Item i) const { int b = item_bucket(i); return b < 0 ? 0 : item_count(s, b); }
    void set_items(Color s, int p, int r, int st) { items_[color_index(s)] = {p, r, st}; }
    const std::array<int, 3>& items(Color s) const { return items_[color_index(s)]; }
    MoveOutcome do_move(const Move&, UndoState&);
    void undo_move(const UndoState&);
    bool is_legal_path(const Move&) const;
    Key key() const;
    Key search_key() const;
    std::string debug_string() const;

   private:
    void reset_pieces();
    std::array<PieceState, PieceCount> pieces_{};
    Color side_to_move_ = Color::White;
    int captures_white_ = 0, captures_black_ = 0;
    std::array<std::array<int, 3>, 2> items_{};
    int quiz_white_ = 0, quiz_black_ = 0, remaining_board_plies_ = -1;
};
}  // namespace rpsc

#endif
