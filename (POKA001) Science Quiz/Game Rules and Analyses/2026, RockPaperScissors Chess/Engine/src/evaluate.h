#ifndef RPSC_EVALUATE_H_INCLUDED
#define RPSC_EVALUATE_H_INCLUDED

#include "position.h"

namespace rpsc {

Value evaluate_white(const Position& position);
Value evaluate(const Position& position);

}  // namespace rpsc

#endif
