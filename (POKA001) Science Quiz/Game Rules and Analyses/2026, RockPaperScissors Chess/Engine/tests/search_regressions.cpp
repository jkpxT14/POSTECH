#include "engine.h"
#include "evaluate.h"
#include "move.h"
#include "movegen.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

using namespace rpsc;

int main() {
  Position p;
  p.items[0] = Inventory{1,1,1};
  p.items[1] = Inventory{1,1,1};
  p.quiz = {2,1};
  p.remaining_plies = 1;
  p.side = Side::White;

  std::vector<std::pair<std::string,int>> exact;
  for (const auto& sm : generate_moves(p, false)) {
    Position q = p;
    const bool ok = q.apply(sm.move);
    assert(ok);
    exact.push_back({move_signature(sm.move), -evaluate(q)});
  }
  std::sort(exact.begin(), exact.end(), [](const auto& a, const auto& b){
    if (a.second != b.second) return a.second > b.second;
    return a.first < b.first;
  });
  assert(exact.size() >= 3);

  Engine e;
  e.position() = p;
  SearchLimits lim;
  lim.depth = 4;
  lim.movetime_ms = 700;
  lim.multipv = 3;
  auto r = e.go(lim);
  assert(r.has_best && r.candidates.size() == 3);
  assert(r.candidates[0].value == exact[0].second);
  std::cout << "One-ply exact search regression passed.\n";
  return 0;
}
