#include "evaluate.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <utility>

namespace rpsc {
namespace {
constexpr Value BaseItemReserve = 6;
constexpr int ReducedStateCount = 27;
constexpr std::array<std::pair<int, int>, 4> Delta{{{0, 1}, {0, -1}, {1, 0}, {-1, 0}}};
constexpr std::array<Direction, 4> Dir{{Direction::North, Direction::South, Direction::East, Direction::West}};
constexpr std::array<Item, 6> Rotations{{Item::RotateNorth, Item::RotateSouth, Item::RotateEast,
                                         Item::RotateWest, Item::RotateLeft, Item::RotateRight}};

Direction opposite_dir(Direction d) {
    return d == Direction::North ? Direction::South
         : d == Direction::South ? Direction::North
         : d == Direction::East ? Direction::West : Direction::East;
}
Direction dir_from_delta(int df, int dr) {
    if (df == 0 && dr == 1) return Direction::North;
    if (df == 0 && dr == -1) return Direction::South;
    if (df == 1 && dr == 0) return Direction::East;
    return Direction::West;
}

struct ReachProfile { std::uint16_t normal = 0, push = 0, rotation = 0, step = 0; };
using ReachTable = std::array<std::array<ReachProfile, 24>, 64>;
using Marks = std::array<bool, 64 * ReducedStateCount>;

void collect(int f, int r, Orientation o, int rem, bool has_last, Direction last, Marks& marks) {
    const auto& t = OrientationTable::instance();
    if (!rem) {
        const int st = t.gesture_state_id(o);
        marks[std::size_t(make_square(f, r)) * ReducedStateCount + std::size_t(st)] = true;
        return;
    }
    for (int i = 0; i < 4; ++i) {
        auto d = Dir[i];
        if (has_last && d == opposite_dir(last)) continue;
        int nf = f + Delta[i].first, nr = r + Delta[i].second;
        if (!valid_square(nf, nr)) continue;
        collect(nf, nr, t.roll(o, d), rem - 1, true, d, marks);
    }
}
void add(Square square, Orientation o, int rolls, Marks& marks,
         bool has_last = false, Direction last = Direction::North) {
    collect(file_of(square), rank_of(square), o, rolls, has_last, last, marks);
}
std::uint16_t count(const Marks& marks) {
    return std::uint16_t(std::count(marks.begin(), marks.end(), true));
}

ReachTable build() {
    ReachTable out{};
    const auto& t = OrientationTable::instance();
    for (int si = 0; si < 64; ++si) {
        for (int oi = 0; oi < 24; ++oi) {
            Square square = Square(si);
            Orientation o = Orientation(oi);
            const int base = base_roll_length(t.top_gesture(o));

            Marks normal{};
            add(square, o, base, normal);

            Marks push = normal;
            for (auto [df, dr] : Delta) {
                int f = file_of(square) + df, r = rank_of(square) + dr;
                if (!valid_square(f, r)) continue;
                Direction push_dir = dir_from_delta(df, dr);
                add(make_square(f, r), o, base, push, true, push_dir);
            }

            Marks rotation = normal;
            for (Item item : Rotations) {
                Orientation ro = t.apply_rotation(o, item);
                int rolls = base_roll_length(t.top_gesture(ro));
                add(square, ro, rolls, rotation);
            }

            Marks step = normal;
            add(square, o, base - 1, step);
            add(square, o, base + 1, step);

            out[si][oi] = {count(normal), count(push), count(rotation), count(step)};
        }
    }
    return out;
}
const ReachTable& table() { static const ReachTable t = build(); return t; }
const ReachProfile& reach(const PieceState& p) { return table()[std::size_t(p.square)][std::size_t(p.orientation)]; }

int first_mob(const Position& p, PieceId id) {
    const auto& pc = p.piece(id);
    if (!pc.alive()) return 0;
    int c = 0;
    for (auto [df, dr] : Delta) {
        int f = file_of(pc.square) + df, r = rank_of(pc.square) + dr;
        if (valid_square(f, r) && !p.occupied(make_square(f, r), id)) ++c;
    }
    return c;
}
int second_mob(const Position& p, PieceId id) {
    const auto& pc = p.piece(id);
    if (!pc.alive()) return 0;
    int c = 0;
    for (auto [df, dr] : Delta) {
        int f = file_of(pc.square) + df, r = rank_of(pc.square) + dr;
        if (!valid_square(f, r) || p.occupied(make_square(f, r), id)) continue;
        for (auto [ef, er] : Delta) {
            if (ef == -df && er == -dr) continue;
            int sf = f + ef, sr = r + er;
            if (valid_square(sf, sr) && !p.occupied(make_square(sf, sr), id)) ++c;
        }
    }
    return c;
}

Value terminal(const Position& p) {
    int score_diff = p.score(Color::White) - p.score(Color::Black);
    if (score_diff) return ScoreUnit * score_diff;
    int quiz_diff = p.quiz(Color::White) - p.quiz(Color::Black);
    return quiz_diff > 0 ? ScoreUnit / 4 : quiz_diff < 0 ? -ScoreUnit / 4 : 0;
}
int reserve(const Position& p) {
    int r = p.remaining_board_plies();
    if (r < 0) return BaseItemReserve;
    return std::max(0, std::min<int>(BaseItemReserve, 2 + r));
}
int added(const ReachProfile& r, int bucket) {
    return bucket == 0 ? int(r.push) - int(r.normal)
         : bucket == 1 ? int(r.rotation) - int(r.normal)
                       : int(r.step) - int(r.normal);
}
int inventory(const Position& p, Color side, int reserve_value) {
    int value = 0;
    for (int bucket = 0; bucket < 3; ++bucket) {
        int n = p.item_count(side, bucket);
        if (n <= 0) continue;
        value += reserve_value * n;
        int best = 0;
        for (int i = 0; i < PieceCount; ++i) {
            auto id = PieceId(i);
            if (piece_color(id) == side && p.piece(id).alive())
                best = std::max(best, added(reach(p.piece(id)), bucket));
        }
        int latent = std::min(9, best / 10);
        if (reserve_value) value += latent * reserve_value / BaseItemReserve;
    }
    return value;
}
}  // namespace

Value evaluate_white(const Position& p) {
    if (p.remaining_board_plies() == 0) return terminal(p);
    Value value = ScoreUnit * (p.score(Color::White) - p.score(Color::Black));
    value += 12 * (p.alive_count(Color::White) - p.alive_count(Color::Black));
    const int rv = reserve(p);
    value += inventory(p, Color::White, rv) - inventory(p, Color::Black, rv);
    for (int i = 0; i < PieceCount; ++i) {
        auto id = PieceId(i);
        const auto& pc = p.piece(id);
        if (!pc.alive()) continue;
        int mobility = 2 * first_mob(p, id) + second_mob(p, id) / 2 + int(reach(pc).normal) / 20;
        value += piece_color(id) == Color::White ? mobility : -mobility;
    }
    return value;
}
Value evaluate(const Position& p) {
    Value value = evaluate_white(p);
    return p.side_to_move() == Color::White ? value : -value;
}
}  // namespace rpsc
