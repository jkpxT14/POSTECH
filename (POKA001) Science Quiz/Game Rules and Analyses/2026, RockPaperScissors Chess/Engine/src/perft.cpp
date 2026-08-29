#include "perft.h"

#include <ostream>

#include "movegen.h"
#include "notation.h"

namespace rpsc {

std::uint64_t perft(Position& position, int depth) {
    if (depth == 0) return 1;

    const auto moves = generate_legal_moves(position);
    std::uint64_t nodes = 0;
    for (const auto& move : moves) {
        UndoState undo;
        position.do_move(move, undo);
        nodes += perft(position, depth - 1);
        position.undo_move(undo);
    }
    return nodes;
}

void divide(Position& position, int depth, std::ostream& out) {
    const auto moves = generate_legal_moves(position);
    std::uint64_t total = 0;

    for (const auto& move : moves) {
        UndoState undo;
        position.do_move(move, undo);
        const auto nodes = perft(position, depth - 1);
        position.undo_move(undo);

        out << format_move(move) << "  " << nodes << '\n';
        total += nodes;
    }
    out << "Total  " << total << '\n';
}

}  // namespace rpsc
