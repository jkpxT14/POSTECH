#ifndef RPSC_MOVEGEN_H_INCLUDED
#define RPSC_MOVEGEN_H_INCLUDED
#include <vector>
#include "position.h"
namespace rpsc {struct SearchMove{Move move{};int capture_swing=0;};std::vector<Move>generate_legal_moves(const Position&);std::vector<Move>generate_unique_moves(Position&);std::vector<Move>generate_search_moves(Position&);std::vector<SearchMove>generate_search_moves_info(Position&);std::vector<SearchMove>generate_tactical_moves_info(Position&);}
#endif
