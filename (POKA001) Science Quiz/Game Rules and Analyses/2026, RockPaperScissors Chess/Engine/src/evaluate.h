#pragma once
#include "position.h"
namespace rpsc {
int evaluate_white(const Position& p);
int evaluate(const Position& p);
int pressure_for(Position p,Side side,int cap=4);
} // namespace rpsc
