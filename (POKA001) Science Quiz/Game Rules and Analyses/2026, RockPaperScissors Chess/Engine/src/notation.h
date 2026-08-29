#ifndef RPSC_NOTATION_H_INCLUDED
#define RPSC_NOTATION_H_INCLUDED

#include <string>
#include <vector>

#include "position.h"

namespace rpsc {

std::string square_name(Square square);
std::string piece_name(PieceId piece);
std::string format_move(const Move& move);
std::string format_move(const Position& before, const Move& move);
bool parse_move(const Position& position, const std::string& text, Move& move);
std::string format_pv(const Position& position, const std::vector<Move>& pv, int m_number = 1);

}  // namespace rpsc

#endif
