#pragma once
#include "orientation.h"
#include "types.h"
#include <array>
#include <string>

namespace rpsc {
class Position {
public:
  Side side=Side::White;
  std::array<int,2> captures{0,0};
  std::array<int,2> quiz{0,0};
  int remaining_plies=40;
  std::array<Inventory,2> items{};
  std::array<Piece,8> pieces{};

  Position();
  void reset_pieces();
  bool occupied(int x,int y,int skip=-1) const;
  std::vector<int> adjacent_enemies(int x,int y,Side mover,int skip=-1) const;
  int alive_count(Side s) const;
  int alive_total() const;
  bool apply(const Move& m, bool* reset=nullptr, int* capture_delta=nullptr);
  std::string key() const;
  int score(Side s) const {int i=side_index(s);return quiz[i]+2*captures[i];}
};
} // namespace rpsc
