#include "perft.h"
namespace rpsc {
std::uint64_t perft(Position& p, int depth) {
    if (depth <= 0) return 1;
    auto moves = generate_legal_moves(p);
    if (depth == 1) return moves.size();
    std::uint64_t nodes = 0;
    for (const auto& move : moves) {
        UndoState undo;
        p.do_move(move, undo);
        nodes += perft(p, depth - 1);
        p.undo_move(undo);
    }
    return nodes;
}
}  // namespace rpsc
