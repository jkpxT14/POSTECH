#include "engine.h"
#include "move.h"
#include "movegen.h"
#include "orientation.h"
#include "position.h"
#include "types.h"
#include <cassert>
#include <iostream>
#include <set>

using namespace rpsc;

int main() {
  const auto& o = OrientationTable::instance();
  assert(o.size() == 24);
  assert(fight(Gesture::Rock, Gesture::Scissors) == 1);
  assert(fight(Gesture::Scissors, Gesture::Paper) == 1);
  assert(fight(Gesture::Paper, Gesture::Rock) == 1);

  Position p;
  assert(p.alive_count(Side::White) == 4);
  assert(p.alive_count(Side::Black) == 4);
  assert(p.side == Side::White);

  auto roots = generate_moves(p, false);
  assert(!roots.empty());

  // Push -> first Roll may return to the pre-Push square.
  p.items[0].push = 1;
  auto with_push = generate_moves(p, false);
  bool push_return = false;
  for (const auto& sm : with_push) {
    const auto& m = sm.move;
    if (m.item == Item::Push && m.path.size() > 1 &&
        m.path[1].first == p.pieces[m.piece].x &&
        m.path[1].second == p.pieces[m.piece].y) {
      push_return = true;
      break;
    }
  }
  assert(push_return);

  // W/B are board roles only.
  p.quiz = {3, 1};
  assert(p.score(Side::White) == 3);
  assert(p.score(Side::Black) == 1);

  Engine e;
  e.position() = p;
  SearchLimits lim;
  lim.movetime_ms = 160;
  lim.multipv = 3;
  auto r = e.go(lim);
  assert(r.has_best);
  assert(r.candidates.size() == 3);
  std::set<std::string> sigs;
  for (const auto& c : r.candidates) sigs.insert(move_signature(c.move));
  assert(sigs.size() == r.candidates.size());

  SearchLimits choice_lim;
  choice_lim.movetime_ms = 180;
  choice_lim.multipv = 1;
  auto item = e.choose_item(Side::White, choice_lim);
  assert(item.lines.size() == 3);
  assert(item.best_bucket >= 0 && item.best_bucket < 3);
  auto initial = e.choose_initial(choice_lim);
  assert(initial.lines.size() == 6);
  assert(initial.best_bucket >= 0 && initial.best_bucket < 3);

  std::cout << "RPSC Engine " << Engine::VERSION
            << " tests passed; legal roots=" << roots.size()
            << ", search depth=" << r.depth << "\n";
  return 0;
}
