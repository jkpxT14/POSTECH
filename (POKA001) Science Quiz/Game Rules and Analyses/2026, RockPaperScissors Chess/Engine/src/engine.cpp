#include "engine.h"
namespace rpsc { Engine::Engine(std::size_t hash_megabytes):position_(),tt_(hash_megabytes){} void Engine::new_game(){position_.reset();tt_.clear();} SearchResult Engine::go(const SearchLimits& limits){Search search(tt_);return search.run(position_,limits);} std::uint64_t Engine::perft(int depth){return rpsc::perft(position_,depth);} void Engine::clear_search(){tt_.clear();} }
