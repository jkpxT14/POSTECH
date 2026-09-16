#pragma once
#include "search.h"
namespace rpsc {
class Engine{public:static constexpr const char* VERSION="0.24.0";SearchResult analyze(const Position&p,const SearchLimits&l){return searcher_.search(p,l);}private:Searcher searcher_;};
}
