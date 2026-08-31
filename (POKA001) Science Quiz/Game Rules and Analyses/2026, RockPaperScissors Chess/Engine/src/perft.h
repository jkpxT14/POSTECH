#ifndef RPSC_PERFT_H_INCLUDED
#define RPSC_PERFT_H_INCLUDED
#include <cstdint>
#include <iosfwd>
#include "position.h"
namespace rpsc { std::uint64_t perft(Position& position,int depth); void divide(Position& position,int depth,std::ostream& out); }
#endif
