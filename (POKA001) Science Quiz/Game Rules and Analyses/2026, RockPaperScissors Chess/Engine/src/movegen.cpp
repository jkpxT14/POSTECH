#include "movegen.h"

#include <unordered_set>

namespace rpsc {
namespace {

Direction opposite_dir(Direction d) {
    switch (d) {
        case Direction::North: return Direction::South;
        case Direction::South: return Direction::North;
        case Direction::East: return Direction::West;
        case Direction::West: return Direction::East;
    }
    return Direction::North;
}

bool step(Square from, Direction d, Square& to) {
    int f = file_of(from), r = rank_of(from);
    switch (d) {
        case Direction::North: ++r; break;
        case Direction::South: --r; break;
        case Direction::East: ++f; break;
        case Direction::West: --f; break;
    }
    if (!valid_square(f, r)) return false;
    to = make_square(f, r); return true;
}

void dfs(const Position& pos, PieceId id, Move& move, int remaining, bool has_last,
         Direction last, std::vector<Move>& out) {
    if (remaining == 0) { out.push_back(move); return; }
    const bool final_step = remaining == 1;
    for (Direction d : {Direction::North, Direction::South, Direction::East, Direction::West}) {
        if (has_last && d == opposite_dir(last)) continue;
        Square next;
        if (!step(move.path[move.path_length - 1], d, next)) continue;
        if (pos.occupied(next, id)) continue;
        if (final_step && pos.adjacent_enemies(next, pos.side_to_move(), id) >= 2) continue;
        move.path[move.path_length++] = next;
        dfs(pos, id, move, remaining - 1, true, d, out);
        --move.path_length;
    }
}

}  // namespace

std::vector<Move> generate_legal_moves(const Position& pos) {
    std::vector<Move> moves;
    const auto& ot = OrientationTable::instance();
    for (int i = 0; i < PieceCount; ++i) {
        const auto id = static_cast<PieceId>(i); const auto& p = pos.piece(id);
        if (!p.alive() || piece_color(id) != pos.side_to_move()) continue;
        Move move; move.piece = id; move.path[0] = p.square; move.path_length = 1;
        dfs(pos, id, move, base_roll_length(ot.top_gesture(p.orientation)), false, Direction::North, moves);
    }
    return moves;
}

std::vector<Move> generate_unique_moves(Position& pos) {
    const auto all = generate_legal_moves(pos);
    std::vector<Move> unique; unique.reserve(all.size());
    std::unordered_set<Key> seen; seen.reserve(all.size() * 2);
    for (const auto& move : all) {
        UndoState undo; pos.do_move(move, undo); const Key k = pos.key(); pos.undo_move(undo);
        if (seen.insert(k).second) unique.push_back(move);
    }
    return unique;
}

}  // namespace rpsc

namespace rpsc {
std::vector<Move> generate_search_moves(Position& pos) {
    const auto all = generate_legal_moves(pos);
    std::vector<Move> unique; unique.reserve(all.size());
    std::unordered_set<Key> seen; seen.reserve(all.size() * 2);
    for (const auto& move : all) {
        UndoState undo; pos.do_move(move, undo); const Key k = pos.search_key(); pos.undo_move(undo);
        if (seen.insert(k).second) unique.push_back(move);
    }
    return unique;
}
}  // namespace rpsc
