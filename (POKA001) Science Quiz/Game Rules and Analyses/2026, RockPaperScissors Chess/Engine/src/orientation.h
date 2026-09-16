#pragma once
#include "types.h"
#include <array>
#include <string>
#include <vector>

namespace rpsc {
struct Vec3 { int x=0,y=0,z=0; bool operator==(const Vec3&) const = default; };
struct Face { int id=0; Gesture gesture=Gesture::Scissors; Vec3 normal{}, wrist{}; };
using Cube = std::array<Face,6>;

class OrientationTable {
public:
  static const OrientationTable& instance();
  int roll(int o, Dir d) const { return roll_[o][static_cast<int>(d)]; }
  int rotate(int o, Item item) const;
  Gesture top(int o) const { return top_[o]; }
  std::string reduced(int o) const { return reduced_[o]; }
  int canonical(Gesture g, bool white) const;
  int size() const { return 24; }
private:
  OrientationTable();
  std::vector<Cube> cubes_;
  std::array<std::array<int,4>,24> roll_{};
  std::array<int,24> rot_l_{}, rot_r_{};
  std::array<Gesture,24> top_{};
  std::array<std::string,24> reduced_{};
  std::array<int,6> canonical_{};
};
} // namespace rpsc
