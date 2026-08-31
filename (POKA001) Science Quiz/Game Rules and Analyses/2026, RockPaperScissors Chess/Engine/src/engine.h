#ifndef RPSC_ENGINE_H_INCLUDED
#define RPSC_ENGINE_H_INCLUDED
#include "perft.h"
#include "search.h"
namespace rpsc { class Engine { public: explicit Engine(std::size_t hash_megabytes=64); void new_game(); Position& position(){return position_;} const Position& position()const{return position_;} SearchResult go(const SearchLimits& limits); std::uint64_t perft(int depth); void clear_search(); private: Position position_; TranspositionTable tt_;}; }
#endif
