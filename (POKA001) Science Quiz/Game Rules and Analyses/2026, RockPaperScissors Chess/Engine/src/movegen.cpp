#include "movegen.h"

#include <unordered_set>
#include <vector>

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
    int file = file_of(from), rank = rank_of(from);
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
        if (!step(current, direction, next) || position.occupied(next, id)) continue;
        if (final_step &&
            position.adjacent_enemies(next, position.side_to_move(), id) >= 2)
            continue;
        move.path[move.path_length++] = next;
        generate_paths(position, id, move, next, remaining - 1, true, direction, moves);
        --move.path_length;
    }
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

std::uint64_t partial_key(Square square, Orientation orientation, int remaining, bool has_last,
                          Direction last) {
    const auto& table = OrientationTable::instance();
    const std::uint64_t state = table.gesture_state_id(orientation);
    const std::uint64_t last_code = has_last ? static_cast<unsigned>(last) : 4u;
    return static_cast<unsigned>(square) | (state << 6) |
           (static_cast<std::uint64_t>(remaining) << 10) | (last_code << 14);
}

struct SearchGeneration {
    Position& position;
    Color mover;
    Color opponent;
    int own_before;
    int opponent_before;
    bool tactical_only;
    std::unordered_set<Key> final_seen;
    std::vector<SearchMove> moves;
    std::vector<std::uint16_t> partial_marks = std::vector<std::uint16_t>(1u << 17, 0);
    std::uint16_t partial_generation = 0;
};

void emit_search_move(SearchGeneration& generation, Move& move, Square current,
                      Orientation orientation) {
    if (generation.tactical_only) {
        if (generation.position.adjacent_enemies(current, generation.mover, move.piece) != 1)
            return;
        const PieceId enemy =
            generation.position.single_adjacent_enemy(current, generation.mover, move.piece);
        const auto& table = OrientationTable::instance();
        if (table.top_gesture(orientation) ==
            table.top_gesture(generation.position.piece(enemy).orientation))
            return;
    }

    UndoState undo;
    generation.position.do_move(move, undo);
    const Key child_key = generation.position.search_key();
    const int swing =
        (generation.position.captures(generation.mover) - generation.own_before) -
        (generation.position.captures(generation.opponent) - generation.opponent_before);
    generation.position.undo_move(undo);

    if (generation.tactical_only && swing == 0) return;
    if (generation.final_seen.insert(child_key).second)
        generation.moves.push_back({move, swing});
}

void generate_search_paths(SearchGeneration& generation, PieceId id, Move& move, Square current,
                           Orientation orientation, int remaining, bool has_last, Direction last) {
    if (remaining == 0) {
        emit_search_move(generation, move, current, orientation);
        return;
    }

    const bool final_step = remaining == 1;
    for (Direction direction :
         {Direction::North, Direction::South, Direction::East, Direction::West}) {
        if (has_last && direction == opposite_dir(last)) continue;
        Square next;
        if (!step(current, direction, next) || generation.position.occupied(next, id)) continue;
        if (final_step &&
            generation.position.adjacent_enemies(next, generation.mover, id) >= 2)
            continue;

        const Orientation next_orientation =
            OrientationTable::instance().roll(orientation, direction);
        const auto key = partial_key(next, next_orientation, remaining - 1, true, direction);
        if (generation.partial_marks[key] == generation.partial_generation) continue;
        generation.partial_marks[key] = generation.partial_generation;

        move.path[move.path_length++] = next;
        generate_search_paths(generation, id, move, next, next_orientation, remaining - 1, true,
                              direction);
        --move.path_length;
    }
}

void generate_search_for_item(SearchGeneration& generation, PieceId id, const PieceState& piece,
                              Item item, Square push_to) {
    Move move;
    move.piece = id;
    move.item = item;
    move.push_to = push_to;
    move.path[0] = piece.square;
    move.path_length = 1;

    const Orientation orientation = item_orientation(piece.orientation, item);
    const int distance = item_distance(orientation, item);
    const Square start = item == Item::Push ? push_to : piece.square;

    ++generation.partial_generation;
    if (generation.partial_generation == 0) {
        std::fill(generation.partial_marks.begin(), generation.partial_marks.end(), 0);
        generation.partial_generation = 1;
    }
    const auto start_key = partial_key(start, orientation, distance, false, Direction::North);
    generation.partial_marks[start_key] = generation.partial_generation;
    generate_search_paths(generation, id, move, start, orientation, distance, false,
                          Direction::North);
}

std::vector<SearchMove> generate_search_moves_internal(Position& position, bool tactical_only) {
    const Color mover = position.side_to_move();
    SearchGeneration generation{position,
                                mover,
                                opposite(mover),
                                position.captures(mover),
                                position.captures(opposite(mover)),
                                tactical_only,
                                {},
                                {}};
    generation.final_seen.reserve(1024);
    generation.moves.reserve(512);

    for (int i = 0; i < PieceCount; ++i) {
        const auto id = static_cast<PieceId>(i);
        const auto& piece = position.piece(id);
        if (!piece.alive() || piece_color(id) != mover) continue;

        generate_search_for_item(generation, id, piece, Item::None, NoSquare);

        if (position.item_count(mover, 0) > 0) {
            for (Direction direction :
                 {Direction::North, Direction::South, Direction::East, Direction::West}) {
                Square pushed;
                if (!step(piece.square, direction, pushed) || position.occupied(pushed, id))
                    continue;
                generate_search_for_item(generation, id, piece, Item::Push, pushed);
            }
        }
        if (position.item_count(mover, 1) > 0) {
            generate_search_for_item(generation, id, piece, Item::RotateLeft, NoSquare);
            generate_search_for_item(generation, id, piece, Item::RotateRight, NoSquare);
        }
        if (position.item_count(mover, 2) > 0) {
            generate_search_for_item(generation, id, piece, Item::StepShort, NoSquare);
            generate_search_for_item(generation, id, piece, Item::StepLong, NoSquare);
        }
    }
    return generation.moves;
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
    return generate_search_moves_internal(position, false);
}

std::vector<SearchMove> generate_tactical_moves_info(Position& position) {
    return generate_search_moves_internal(position, true);
}

std::vector<Move> generate_search_moves(Position& position) {
    const auto info = generate_search_moves_info(position);
    std::vector<Move> moves;
    moves.reserve(info.size());
    for (const auto& entry : info) moves.push_back(entry.move);
    return moves;
}

}  // namespace rpsc
