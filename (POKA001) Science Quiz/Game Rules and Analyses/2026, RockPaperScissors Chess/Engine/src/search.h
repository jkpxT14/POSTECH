#pragma once
#include "evaluate.h"
#include "movegen.h"
#include "tt.h"
#include "types.h"
#include <chrono>
#include <unordered_map>

namespace rpsc {
class Searcher {
public:
  SearchResult search(const Position& root,const SearchLimits& limits);
private:
  using Clock=std::chrono::steady_clock;
  uint64_t nodes_=0;int sel_=0;bool stop_=false;Clock::time_point deadline_{};TranspositionTable tt_;
  bool timed_out();
  int qsearch(Position& p,int alpha,int beta,int ply);
  int alphabeta(Position& p,int depth,int alpha,int beta,int ply,bool pv,int ext_used);
  struct RootPass{Move best;bool has_best=false;int value=-1000000000;std::vector<std::pair<Move,int>>scores;};
  RootPass root_pass(const Position& p,const std::vector<ScoredMove>& roots,const Move*preferred,int depth,int alpha,int beta,const std::vector<Move>&skip,bool multipv);
  std::vector<Move> pvline(Position p,const Move& best,int depth);
};
} // namespace rpsc
