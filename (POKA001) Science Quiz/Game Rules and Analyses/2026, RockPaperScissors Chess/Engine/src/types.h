#pragma once
#include <array>
#include <chrono>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace rpsc {

enum class Side : uint8_t { White = 0, Black = 1 };
enum class Gesture : uint8_t { Scissors = 0, Rock = 1, Paper = 2 };
enum class Dir : uint8_t { N = 0, S = 1, E = 2, W = 3 };
enum class Item : uint8_t { None = 0, Push, RoN, RoS, RoE, RoW, RoL, RoR, StepShort, StepLong };

inline Side other(Side s) { return s == Side::White ? Side::Black : Side::White; }
inline int side_index(Side s) { return s == Side::White ? 0 : 1; }

struct Piece {
  int id = 0;                // 0..3 White, 4..7 Black
  bool alive = true;
  int x = 0, y = 0;
  int orientation = 0;       // exact 24-state cube orientation
};

struct Inventory {
  int push = 0;
  int rotation = 0;
  int step = 0;
};

struct Move {
  int piece = -1;
  Item item = Item::None;
  int push_x = -1, push_y = -1;
  std::vector<Dir> dirs;
  std::vector<std::pair<int,int>> path; // starts at pre-move square
};

struct ScoredMove {
  Move move;
  int tactical = 0; // capture-score delta from mover's perspective
  bool reset = false;
};

struct Candidate {
  Move move;
  int value = 0;
  int depth = 0;
  std::vector<Move> pv;
};

struct SearchLimits {
  int depth = 64;
  int movetime_ms = 10000;
  int multipv = 3;
};

struct SearchResult {
  Move best;
  bool has_best = false;
  int value = 0;
  int depth = 0;
  int seldepth = 0;
  uint64_t nodes = 0;
  int elapsed_ms = 0;
  std::vector<Candidate> candidates;
};

std::string side_name(Side s);
std::string gesture_name(Gesture g);
std::string dir_name(Dir d);
std::string item_name(Item i);
Dir opposite(Dir d);
int base_distance(Gesture g);
int fight(Gesture a, Gesture b); // +1 a wins, -1 b wins, 0 tie

} // namespace rpsc
