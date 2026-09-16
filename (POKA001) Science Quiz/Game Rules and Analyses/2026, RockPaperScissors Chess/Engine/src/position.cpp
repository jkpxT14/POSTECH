#include "position.h"
#include "move.h"
#include <cmath>
#include <sstream>

namespace rpsc {
Position::Position() {
  for (int i = 0; i < 8; ++i) pieces[i].id = i;
  reset_pieces();
}
void Position::reset_pieces() {
  const auto& o = OrientationTable::instance();
  const int sx[8] = {0, 2, 4, 6, 7, 5, 3, 1};
  const int sy[8] = {0, 0, 0, 0, 7, 7, 7, 7};
  const Gesture g[8] = {Gesture::Scissors, Gesture::Rock, Gesture::Paper, Gesture::Scissors,
                        Gesture::Scissors, Gesture::Rock, Gesture::Paper, Gesture::Scissors};
  for (int i = 0; i < 8; ++i) {
    pieces[i].alive = true;
    pieces[i].x = sx[i];
    pieces[i].y = sy[i];
    pieces[i].orientation = o.canonical(g[i], i < 4);
  }
}
bool Position::occupied(int x, int y, int skip) const {
  for (int i = 0; i < 8; ++i) {
    const auto& p = pieces[i];
    if (i != skip && p.alive && p.x == x && p.y == y) return true;
  }
  return false;
}
std::vector<int> Position::adjacent_enemies(int x, int y, Side mover, int skip) const {
  std::vector<int> a;
  for (int i = 0; i < 8; ++i) {
    const auto& p = pieces[i];
    const Side ps = i < 4 ? Side::White : Side::Black;
    if (i != skip && p.alive && ps != mover && std::abs(p.x - x) + std::abs(p.y - y) == 1) a.push_back(i);
  }
  return a;
}
int Position::alive_count(Side s) const {
  int n = 0;
  const int a = s == Side::White ? 0 : 4, b = a + 4;
  for (int i = a; i < b; ++i) n += pieces[i].alive;
  return n;
}
int Position::alive_total() const { return alive_count(Side::White) + alive_count(Side::Black); }
bool Position::apply(const Move& m, bool* did_reset, int* capture_delta) {
  if (m.piece < 0 || m.piece >= 8 || !pieces[m.piece].alive) return false;
  const Side mover = side;
  if ((m.piece < 4) != (mover == Side::White)) return false;

  const auto& o = OrientationTable::instance();
  auto& p = pieces[m.piece];
  int ori = o.rotate(p.orientation, m.item);
  const int b = item_bucket(m.item), si = side_index(mover);
  if (b == 1) {
    if (items[si].push <= 0) return false;
    --items[si].push;
  } else if (b == 2) {
    if (items[si].rotation <= 0) return false;
    --items[si].rotation;
  } else if (b == 3) {
    if (items[si].step <= 0) return false;
    --items[si].step;
  }

  for (auto d : m.dirs) ori = o.roll(ori, d);
  if (m.path.empty()) return false;
  const auto [x, y] = m.path.back();
  p.x = x;
  p.y = y;
  p.orientation = ori;

  int delta = 0;
  auto es = adjacent_enemies(x, y, mover, m.piece);
  if (es.size() == 1) {
    auto& e = pieces[es[0]];
    const int f = fight(o.top(p.orientation), o.top(e.orientation));
    if (f != 0) {
      Piece* loser = f > 0 ? &e : &p;
      const Side winner = f > 0 ? mover : other(mover);
      loser->alive = false;
      ++captures[side_index(winner)];
      delta = f > 0 ? 1 : -1;
    }
  }

  const bool r = alive_count(Side::White) == 0 || alive_count(Side::Black) == 0;
  if (r) reset_pieces();
  side = other(side);
  if (remaining_plies > 0) --remaining_plies;
  if (did_reset) *did_reset = r;
  if (capture_delta) *capture_delta = delta;
  return true;
}
std::string Position::key() const {
  std::ostringstream o;
  o << (side == Side::White ? 'W' : 'B') << '|'
    << captures[0] << ',' << captures[1] << '|'
    << quiz[0] << ',' << quiz[1] << '|'
    << remaining_plies << '|'
    << items[0].push << ',' << items[0].rotation << ',' << items[0].step << ','
    << items[1].push << ',' << items[1].rotation << ',' << items[1].step << '|';
  const auto& t = OrientationTable::instance();
  for (auto& p : pieces) {
    if (!p.alive) o << "x;";
    else o << p.x + 8 * p.y << '/' << t.reduced(p.orientation) << ';';
  }
  return o.str();
}
}  // namespace rpsc
