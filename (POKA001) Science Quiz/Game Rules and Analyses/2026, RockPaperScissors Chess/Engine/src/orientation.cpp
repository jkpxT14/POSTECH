#include "orientation.h"

#include <algorithm>
#include <array>
#include <queue>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace rpsc {
namespace {
struct Vec3 { int x, y, z; };
struct Face { char id; int copy; Gesture gesture; Vec3 normal, wrist; };
using Physical = std::array<Face, 6>;
enum class Op { N, S, E, W, CW, CCW };

Vec3 transform(Vec3 v, Op op) {
    switch (op) {
        case Op::N: return {v.x, -v.z, v.y};
        case Op::S: return {v.x, v.z, -v.y};
        case Op::E: return {v.z, v.y, -v.x};
        case Op::W: return {-v.z, v.y, v.x};
        case Op::CW: return {v.y, -v.x, v.z};
        case Op::CCW: return {-v.y, v.x, v.z};
    }
    return v;
}
Physical transformed(const Physical& p, Op op) {
    auto out = p;
    for (auto& f : out) {
        f.normal = transform(f.normal, op);
        f.wrist = transform(f.wrist, op);
    }
    return out;
}
bool eq(Vec3 a, Vec3 b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
std::string key(const Physical& p) {
    auto q = p;
    std::sort(q.begin(), q.end(), [](const auto& a, const auto& b) {
        return a.id == b.id ? a.copy < b.copy : a.id < b.id;
    });
    std::string out;
    for (const auto& f : q) {
        out += f.id;
        out += char('0' + f.copy);
        out += ':';
        out += std::to_string(f.normal.x) + ',' + std::to_string(f.normal.y) + ',' + std::to_string(f.normal.z);
        out += '/';
        out += std::to_string(f.wrist.x) + ',' + std::to_string(f.wrist.y) + ',' + std::to_string(f.wrist.z) + '|';
    }
    return out;
}
const Face& face_on(const Physical& p, Vec3 n) {
    for (const auto& f : p)
        if (eq(f.normal, n)) return f;
    throw std::logic_error("invalid cube orientation");
}
WristDirection wrist_dir(Vec3 w) {
    if (eq(w, {0, 1, 0})) return WristDirection::North;
    if (eq(w, {0, -1, 0})) return WristDirection::South;
    if (eq(w, {1, 0, 0})) return WristDirection::East;
    if (eq(w, {-1, 0, 0})) return WristDirection::West;
    throw std::logic_error("top wrist is not horizontal");
}
Op dir_op(Direction d) {
    switch (d) {
        case Direction::North: return Op::N;
        case Direction::South: return Op::S;
        case Direction::East: return Op::E;
        case Direction::West: return Op::W;
    }
    return Op::N;
}
}  // namespace

const OrientationTable& OrientationTable::instance() {
    static const OrientationTable table;
    return table;
}

OrientationTable::OrientationTable() {
    const Physical base{{
        {'S', 1, Gesture::Scissors, {0, 0, 1}, {0, -1, 0}},
        {'S', 2, Gesture::Scissors, {0, 0, -1}, {0, -1, 0}},
        {'R', 1, Gesture::Rock, {0, 1, 0}, {0, 0, 1}},
        {'R', 2, Gesture::Rock, {0, -1, 0}, {0, 0, -1}},
        {'P', 1, Gesture::Paper, {-1, 0, 0}, {0, -1, 0}},
        {'P', 2, Gesture::Paper, {1, 0, 0}, {0, -1, 0}},
    }};

    std::vector<Physical> all;
    std::unordered_map<std::string, Orientation> index;
    std::queue<Orientation> queue;
    all.push_back(base);
    index.emplace(key(base), Orientation(0));
    queue.push(Orientation(0));

    while (!queue.empty()) {
        const auto i = queue.front();
        queue.pop();
        for (Op op : {Op::N, Op::S, Op::E, Op::W}) {
            auto next = transformed(all[i], op);
            auto k = key(next);
            if (!index.count(k)) {
                auto j = Orientation(all.size());
                index.emplace(k, j);
                all.push_back(next);
                queue.push(j);
            }
        }
    }
    if (all.size() != 24) throw std::logic_error("cube must have 24 orientations");

    for (Orientation i = 0; i < 24; ++i) {
        for (Direction d : {Direction::North, Direction::South, Direction::East, Direction::West})
            roll_[i][int(d)] = index.at(key(transformed(all[i], dir_op(d))));
        rotate_right_[i] = index.at(key(transformed(all[i], Op::CW)));
        rotate_left_[i] = index.at(key(transformed(all[i], Op::CCW)));
        const auto& top = face_on(all[i], {0, 0, 1});
        top_[i] = top.gesture;
        wrist_[i] = wrist_dir(top.wrist);
        state_[i] = {
            face_on(all[i], {0, 0, 1}).gesture,
            face_on(all[i], {0, 1, 0}).gesture,
            face_on(all[i], {1, 0, 0}).gesture,
        };
        state_id_[i] = std::uint8_t(int(state_[i].ud) * 9 + int(state_[i].ns) * 3 + int(state_[i].ew));
        if (top.copy == 1) canonical_[int(top_[i])][int(wrist_[i])] = i;
    }
}

Orientation OrientationTable::roll(Orientation o, Direction d) const { return roll_[o][int(d)]; }
Orientation OrientationTable::rotate_left(Orientation o) const { return rotate_left_[o]; }
Orientation OrientationTable::rotate_right(Orientation o) const { return rotate_right_[o]; }
Orientation OrientationTable::apply_rotation(Orientation o, Item item) const {
    switch (item) {
        case Item::RotateNorth: return roll(o, Direction::North);
        case Item::RotateSouth: return roll(o, Direction::South);
        case Item::RotateEast: return roll(o, Direction::East);
        case Item::RotateWest: return roll(o, Direction::West);
        case Item::RotateLeft: return rotate_left(o);
        case Item::RotateRight: return rotate_right(o);
        default: return o;
    }
}
Gesture OrientationTable::top_gesture(Orientation o) const { return top_[o]; }
WristDirection OrientationTable::wrist_direction(Orientation o) const { return wrist_[o]; }
GestureState OrientationTable::gesture_state(Orientation o) const { return state_[o]; }
std::uint8_t OrientationTable::gesture_state_id(Orientation o) const { return state_id_[o]; }
Orientation OrientationTable::canonical(Gesture g, WristDirection w) const { return canonical_[int(g)][int(w)]; }
}  // namespace rpsc
