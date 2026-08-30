#include "movegen.h"

#include <unordered_set>

namespace rpsc {
namespace {

Direction opposite_dir(Direction direction) {
    switch (direction) {
        case Direction::North: return Direction::South;
        case Direction::South: return Direction::North;
        case Direction::East: return Direction::West;
        case Direction::West: return Direction::East;
    }
    return Direction::North;
}

bool step(Square from, Direction direction, Square& to) {
    int file = file_of(from);
    int rank = rank_of(from);
    switch (direction) {
        case Direction::North: ++rank; break;
        case Direction::South: --rank; break;
        case Direction::East: ++file; break;
        case Direction::West: --file; break;
    }
    if (!valid_square(file, rank)) return false;
    to = make_square(file, rank);
    return true;
}

void generate_paths(const Position& position, PieceId id, Move& move, Square current,
                    int remaining, bool has_last, Direction last, std::vector<Move>& moves) {
    if (remaining == 0) {
        moves.push_back(move);
        return;
    }

    const bool final_step = remaining == 1;
    for (Direction direction :
         {Direction::North, Direction::South, Direction::East, Direction::West}) {
        if (has_last && direction == opposite_dir(last)) continue;

        Square next;
        if (!step(current, direction, next)) continue;
        if (position.occupied(next, id)) continue;
        if (final_step && position.adjacent_enemies(next, position.side_to_move(), id) >= 2)
            continue;

        move.path[move.path_length++] = next;
        generate_paths(position, id, move, next, remaining - 1, true, direction, moves);
        --move.path_length;
    }
}

Orientation item_orientation(Orientation orientation, Item item) {
    const auto& table = OrientationTable::instance();
    if (item == Item::RotateLeft) return table.rotate_left(orientation);
    if (item == Item::RotateRight) return table.rotate_right(orientation);
    return orientation;
}

int item_distance(Orientation orientation, Item item) {
    int distance = base_roll_length(OrientationTable::instance().top_gesture(orientation));
    if (item == Item::StepShort) --distance;
    if (item == Item::StepLong) ++distance;
    return distance;
}

void generate_for_item(const Position& position, PieceId id, const PieceState& piece, Item item,
                       Square push_to, std::vector<Move>& moves) {
    Move move;
    move.piece = id;
    move.item = item;
    move.push_to = push_to;
    move.path[0] = piece.square;
    move.path_length = 1;

    const Orientation orientation = item_orientation(piece.orientation, item);
    const int distance = item_distance(orientation, item);
    const Square start = item == Item::Push ? push_to : piece.square;
    generate_paths(position, id, move, start, distance, false, Direction::North, moves);
}


bool tactical_path(const Position& position, PieceId id, Move& move, Square current,
                   Orientation orientation, int remaining, bool has_last, Direction last,
                   std::vector<Move>& moves) {
    if (remaining == 0) {
        const Color mover = position.side_to_move();
        if (position.adjacent_enemies(current, mover, id) != 1) return false;
        const PieceId enemy_id = position.single_adjacent_enemy(current, mover, id);
        const auto& table = OrientationTable::instance();
        const Gesture own = table.top_gesture(orientation);
        const Gesture enemy = table.top_gesture(position.piece(enemy_id).orientation);
        const bool combat = own != enemy;
        if (combat) moves.push_back(move);
        return combat;
    }

    const bool final_step = remaining == 1;
    bool found = false;
    for (Direction direction :
         {Direction::North, Direction::South, Direction::East, Direction::West}) {
        if (has_last && direction == opposite_dir(last)) continue;
        Square next;
        if (!step(current, direction, next)) continue;
        if (position.occupied(next, id)) continue;
        if (final_step && position.adjacent_enemies(next, position.side_to_move(), id) >= 2)
            continue;

        move.path[move.path_length++] = next;
        const Orientation next_orientation = OrientationTable::instance().roll(orientation, direction);
        found = tactical_path(position, id, move, next, next_orientation, remaining - 1, true,
                              direction, moves) || found;
        --move.path_length;
    }
    return found;
}

void generate_tactical_for_item(const Position& position, PieceId id, const PieceState& piece,
                                Item item, Square push_to, std::vector<Move>& moves) {
    Move move;
    move.piece = id;
    move.item = item;
    move.push_to = push_to;
    move.path[0] = piece.square;
    move.path_length = 1;

    const Orientation orientation = item_orientation(piece.orientation, item);
    const int distance = item_distance(orientation, item);
    const Square start = item == Item::Push ? push_to : piece.square;
    tactical_path(position, id, move, start, orientation, distance, false, Direction::North, moves);
}

}  // namespace

std::vector<Move> generate_legal_moves(const Position& position) {
    std::vector<Move> moves;

    for (int i = 0; i < PieceCount; ++i) {
        const auto id = static_cast<PieceId>(i);
        const auto& piece = position.piece(id);
        if (!piece.alive() || piece_color(id) != position.side_to_move()) continue;

        generate_for_item(position, id, piece, Item::None, NoSquare, moves);

        if (position.item_count(position.side_to_move(), 0) > 0) {
            for (Direction direction :
                 {Direction::North, Direction::South, Direction::East, Direction::West}) {
                Square pushed;
                if (!step(piece.square, direction, pushed) || position.occupied(pushed, id))
                    continue;
                generate_for_item(position, id, piece, Item::Push, pushed, moves);
            }
        }
        if (position.item_count(position.side_to_move(), 1) > 0) {
            generate_for_item(position, id, piece, Item::RotateLeft, NoSquare, moves);
            generate_for_item(position, id, piece, Item::RotateRight, NoSquare, moves);
        }
        if (position.item_count(position.side_to_move(), 2) > 0) {
            generate_for_item(position, id, piece, Item::StepShort, NoSquare, moves);
            generate_for_item(position, id, piece, Item::StepLong, NoSquare, moves);
        }
    }
    return moves;
}

std::vector<Move> generate_unique_moves(Position& position) {
    const auto all = generate_legal_moves(position);
    std::vector<Move> unique;
    unique.reserve(all.size());
    std::unordered_set<Key> seen;
    seen.reserve(all.size() * 2);

    for (const auto& move : all) {
        UndoState undo;
        position.do_move(move, undo);
        const Key key = position.key();
        position.undo_move(undo);
        if (seen.insert(key).second) unique.push_back(move);
    }
    return unique;
}

std::vector<SearchMove> generate_search_moves_info(Position& position) {
    const auto all = generate_legal_moves(position);
    std::vector<SearchMove> unique;
    unique.reserve(all.size());
    std::unordered_set<Key> seen;
    seen.reserve(all.size() * 2);

    const Color mover = position.side_to_move();
    const Color opponent = opposite(mover);
    const int own_before = position.captures(mover);
    const int opponent_before = position.captures(opponent);

    for (const auto& move : all) {
        UndoState undo;
        position.do_move(move, undo);
        const Key key = position.search_key();
        const int swing = (position.captures(mover) - own_before) -
                          (position.captures(opponent) - opponent_before);
        position.undo_move(undo);
        if (seen.insert(key).second) unique.push_back({move, swing});
    }
    return unique;
}

std::vector<SearchMove> generate_tactical_moves_info(Position& position) {
    std::vector<Move> raw;
    for (int i = 0; i < PieceCount; ++i) {
        const auto id = static_cast<PieceId>(i);
        const auto& piece = position.piece(id);
        if (!piece.alive() || piece_color(id) != position.side_to_move()) continue;

        generate_tactical_for_item(position, id, piece, Item::None, NoSquare, raw);
        if (position.item_count(position.side_to_move(), 0) > 0) {
            for (Direction direction :
                 {Direction::North, Direction::South, Direction::East, Direction::West}) {
                Square pushed;
                if (!step(piece.square, direction, pushed) || position.occupied(pushed, id))
                    continue;
                generate_tactical_for_item(position, id, piece, Item::Push, pushed, raw);
            }
        }
        if (position.item_count(position.side_to_move(), 1) > 0) {
            generate_tactical_for_item(position, id, piece, Item::RotateLeft, NoSquare, raw);
            generate_tactical_for_item(position, id, piece, Item::RotateRight, NoSquare, raw);
        }
        if (position.item_count(position.side_to_move(), 2) > 0) {
            generate_tactical_for_item(position, id, piece, Item::StepShort, NoSquare, raw);
            generate_tactical_for_item(position, id, piece, Item::StepLong, NoSquare, raw);
        }
    }

    std::vector<SearchMove> unique;
    unique.reserve(raw.size());
    std::unordered_set<Key> seen;
    seen.reserve(raw.size() * 2 + 1);
    const Color mover = position.side_to_move();
    const Color opponent = opposite(mover);
    const int own_before = position.captures(mover);
    const int opponent_before = position.captures(opponent);
    for (const auto& move : raw) {
        UndoState undo;
        position.do_move(move, undo);
        const Key child_key = position.search_key();
        const int swing = (position.captures(mover) - own_before) -
                          (position.captures(opponent) - opponent_before);
        position.undo_move(undo);
        if (swing != 0 && seen.insert(child_key).second) unique.push_back({move, swing});
    }
    return unique;
}

std::vector<Move> generate_search_moves(Position& position) {
    const auto info = generate_search_moves_info(position);
    std::vector<Move> moves;
    moves.reserve(info.size());
    for (const auto& entry : info) moves.push_back(entry.move);
    return moves;
}

}  // namespace rpsc
