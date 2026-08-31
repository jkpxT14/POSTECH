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
    int captures_white = 0;
    int captures_black = 0;
    std::array<std::array<int, 3>, 2> items{};
    int quiz_white = 0;
    int quiz_black = 0;
    int remaining_board_plies = -1;
};

struct MoveOutcome {
    PieceId captured = PieceId::W1;
    bool has_capture = false;
    bool reset = false;
};

class Position {
   public:
    Position();

    void reset();
    Color side_to_move() const { return side_to_move_; }
    void set_side_to_move(Color color) { side_to_move_ = color; }
    const PieceState& piece(PieceId id) const { return pieces_[piece_index(id)]; }
    PieceState& piece(PieceId id) { return pieces_[piece_index(id)]; }
    const std::array<PieceState, PieceCount>& pieces() const { return pieces_; }

    bool occupied(Square square, PieceId exclude) const;
    int adjacent_enemies(Square square, Color side, PieceId exclude) const;
    PieceId single_adjacent_enemy(Square square, Color side, PieceId exclude) const;
    int captures(Color side) const { return side == Color::White ? captures_white_ : captures_black_; }
    int quiz(Color side) const { return side == Color::White ? quiz_white_ : quiz_black_; }
    int score(Color side) const { return quiz(side) + 2 * captures(side); }
    void set_match_context(int quiz_white, int quiz_black, int remaining_board_plies) {
        quiz_white_ = quiz_white; quiz_black_ = quiz_black; remaining_board_plies_ = remaining_board_plies;
    }
    int remaining_board_plies() const { return remaining_board_plies_; }
    int alive_count(Color side) const;

    int item_count(Color side, int bucket) const { return items_[color_index(side)][bucket]; }
    int item_count(Color side, Item item) const {
        const int bucket = item_bucket(item);
        return bucket < 0 ? 0 : item_count(side, bucket);
    }
    void set_items(Color side, int push, int rotation, int step) {
        items_[color_index(side)] = {push, rotation, step};
    }
    const std::array<int, 3>& items(Color side) const { return items_[color_index(side)]; }

    MoveOutcome do_move(const Move& move, UndoState& undo);
    void undo_move(const UndoState& undo);
    bool is_legal_path(const Move& move) const;

    Key key() const;
    Key search_key() const;
    std::string debug_string() const;

   private:
    void reset_pieces();

    std::array<PieceState, PieceCount> pieces_{};
    Color side_to_move_ = Color::White;
    int captures_white_ = 0;
    int captures_black_ = 0;
    std::array<std::array<int, 3>, 2> items_{};
    int quiz_white_ = 0;
    int quiz_black_ = 0;
    int remaining_board_plies_ = -1;
};

}  // namespace rpsc

#endif
