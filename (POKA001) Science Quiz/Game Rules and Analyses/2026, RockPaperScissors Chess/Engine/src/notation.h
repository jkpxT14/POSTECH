#pragma once
#include "position.h"
#include "types.h"
#include <string>
namespace rpsc {
std::string square_name(int x,int y);
std::string move_notation(const Position& before,const Move& m);
} // namespace rpsc
