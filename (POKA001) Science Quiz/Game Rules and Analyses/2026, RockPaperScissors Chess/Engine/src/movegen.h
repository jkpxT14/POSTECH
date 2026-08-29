#ifndef RPSC_MOVEGEN_H_INCLUDED
#define RPSC_MOVEGEN_H_INCLUDED

#include <vector>

#include "position.h"

namespace rpsc {

struct SearchMove {
    Move move{};
    int capture_swing = 0;
};

std::vector<Move> generate_legal_moves(const Position& position);
std::vector<Move> generate_unique_moves(Position& position);
std::vector<Move> generate_search_moves(Position& position);
std::vector<SearchMove> generate_search_moves_info(Position& position);

}  // namespace rpsc

#endif
