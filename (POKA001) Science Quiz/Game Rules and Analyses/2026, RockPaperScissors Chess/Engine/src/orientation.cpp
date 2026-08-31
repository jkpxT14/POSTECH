#include "orientation.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <queue>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace rpsc {
namespace {

struct Vec3 { int x, y, z; };
struct Face { char id; int copy; Gesture gesture; Vec3 normal; Vec3 wrist; };
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
    Physical out = p;
    for (auto& f : out) {
        f.normal = transform(f.normal, op);
        f.wrist = transform(f.wrist, op);
    }
    return out;
}

std::string key(const Physical& p) {
    auto q = p;
    std::sort(q.begin(), q.end(), [](const Face& a, const Face& b) {
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

bool eq(Vec3 a, Vec3 b) { return a.x == b.x && a.y == b.y && a.z == b.z; }

const Face& face_on(const Physical& p, Vec3 n) {
    for (const auto& f : p) if (eq(f.normal, n)) return f;
    throw std::logic_error("invalid cube orientation");
}

WristDirection wrist_dir(Vec3 w) {
    if (eq(w, {0, 1, 0})) return WristDirection::North;
    if (eq(w, {0, -1, 0})) return WristDirection::South;
    if (eq(w, {1, 0, 0})) return WristDirection::East;
    if (eq(w, {-1, 0, 0})) return WristDirection::West;
    throw std::logic_error("top wrist is not horizontal");
}

int wrist_index(WristDirection d) { return static_cast<int>(d); }
int gesture_index(Gesture g) { return static_cast<int>(g); }

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
    index.emplace(key(base), 0);
    queue.push(0);
    while (!queue.empty()) {
        const auto i = queue.front(); queue.pop();
        for (Op op : {Op::N, Op::S, Op::E, Op::W}) {
            auto next = transformed(all[i], op);
            auto k = key(next);
            if (!index.count(k)) {
                const auto n = static_cast<Orientation>(all.size());
                index.emplace(k, n); all.push_back(next); queue.push(n);
            }
        }
    }
    if (all.size() != 24) throw std::logic_error("cube must have exactly 24 orientations");

    for (Orientation i = 0; i < 24; ++i) {
        for (Direction d : {Direction::North, Direction::South, Direction::East, Direction::West})
            roll_[i][static_cast<int>(d)] = index.at(key(transformed(all[i], dir_op(d))));
        rotate_right_[i] = index.at(key(transformed(all[i], Op::CW)));
        rotate_left_[i] = index.at(key(transformed(all[i], Op::CCW)));

        const auto& top_face = face_on(all[i], {0, 0, 1});
        top_[i] = top_face.gesture;
        wrist_[i] = wrist_dir(top_face.wrist);
        state_[i] = {face_on(all[i], {0, 0, 1}).gesture,
                     face_on(all[i], {0, 1, 0}).gesture,
                     face_on(all[i], {1, 0, 0}).gesture};
        state_id_[i] = static_cast<std::uint8_t>(
            static_cast<int>(state_[i].ud) * 9 + static_cast<int>(state_[i].ns) * 3 + static_cast<int>(state_[i].ew));
        if (top_face.copy == 1)
            canonical_[gesture_index(top_[i])][wrist_index(wrist_[i])] = i;
    }
}

Orientation OrientationTable::roll(Orientation orientation, Direction direction) const {
    return roll_[orientation][static_cast<int>(direction)];
}
Orientation OrientationTable::rotate_left(Orientation orientation) const { return rotate_left_[orientation]; }
Orientation OrientationTable::rotate_right(Orientation orientation) const { return rotate_right_[orientation]; }
Gesture OrientationTable::top_gesture(Orientation orientation) const { return top_[orientation]; }
WristDirection OrientationTable::wrist_direction(Orientation orientation) const { return wrist_[orientation]; }
GestureState OrientationTable::gesture_state(Orientation orientation) const { return state_[orientation]; }
std::uint8_t OrientationTable::gesture_state_id(Orientation orientation) const { return state_id_[orientation]; }
Orientation OrientationTable::canonical(Gesture gesture, WristDirection wrist) const { return canonical_[gesture_index(gesture)][wrist_index(wrist)]; }

}  // namespace rpsc
