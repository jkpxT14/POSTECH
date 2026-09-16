#pragma once
#include "position.h"
#include "types.h"
#include <vector>
namespace rpsc {
std::vector<ScoredMove> generate_moves(const Position& p,bool tactical_only=false);
} // namespace rpsc
