#pragma once
#include "types.h"
#include <string>
namespace rpsc {
bool same_move(const Move& a,const Move& b);
bool is_rotation(Item i);
int item_bucket(Item i); // 0 none, 1 push, 2 rotation, 3 step
int destination_square(const Move& m);
std::string move_signature(const Move& m);
} // namespace rpsc
