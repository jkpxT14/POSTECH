#include "movegen.h"

#include <array>
#include <cstdint>
#include <unordered_set>

namespace rpsc {
namespace {
constexpr std::array<Direction, 4> Dirs{
    Direction::North, Direction::South, Direction::East, Direction::West};
constexpr std::array<Direction, 4> Opp{
    Direction::South, Direction::North, Direction::West, Direction::East};
constexpr std::array<Item, 6> Rotations{
    Item::RotateNorth, Item::RotateSouth, Item::RotateEast,
    Item::RotateWest, Item::RotateLeft, Item::RotateRight};

struct StepTable {
    std::array<std::array<Square, 4>, 64> next{};
    StepTable() {
        for (int s = 0; s < 64; ++s) {
            for (int di = 0; di < 4; ++di) {
                int f = file_of(Square(s)), r = rank_of(Square(s));
                switch (Dirs[di]) {
                    case Direction::North: ++r; break;
                    case Direction::South: --r; break;
                    case Direction::East: ++f; break;
                    case Direction::West: --f; break;
                }
                next[s][di] = valid_square(f, r) ? make_square(f, r) : NoSquare;
            }
        }
    }
};
const StepTable& steps() { static const StepTable table; return table; }

Direction direction_between(Square a, Square b) {
    int df = file_of(b) - file_of(a), dr = rank_of(b) - rank_of(a);
    if (df == 0 && dr == 1) return Direction::North;
    if (df == 0 && dr == -1) return Direction::South;
    if (df == 1 && dr == 0) return Direction::East;
    return Direction::West;
}

Orientation modifier_orientation(Orientation o, Item item) {
    return OrientationTable::instance().apply_rotation(o, item);
}
int modifier_distance(Orientation original, Item item) {
    const auto& t = OrientationTable::instance();
    Orientation o = modifier_orientation(original, item);
    int n = base_roll_length(t.top_gesture(o));
    if (item == Item::StepShort) --n;
    if (item == Item::StepLong) ++n;
    return n;
}

void exhaustive_paths(const Position& p, PieceId id, Move& m, Square cur, int rem,
                      bool has_last, Direction last, std::vector<Move>& out) {
    if (rem == 0) {
        out.push_back(m);
        return;
    }
    const bool final = rem == 1;
    for (int di = 0; di < 4; ++di) {
        Direction d = Dirs[di];
        if (has_last && d == Opp[int(last)]) continue;
        Square next = steps().next[int(cur)][di];
        if (next == NoSquare || p.occupied(next, id)) continue;
        if (final && p.adjacent_enemies(next, p.side_to_move(), id) >= 2) continue;
        m.path[m.path_length++] = next;
        exhaustive_paths(p, id, m, next, rem - 1, true, d, out);
        --m.path_length;
    }
}

void exhaustive_item(const Position& p, PieceId id, const PieceState& pc, Item item,
                     Square push, std::vector<Move>& out) {
    Move m;
    m.piece = id;
    m.item = item;
    m.push_to = push;
    m.path[0] = pc.square;
    m.path_length = 1;
    Orientation o = modifier_orientation(pc.orientation, item);
    const int distance = modifier_distance(pc.orientation, item);
    Square start = pc.square;
    bool has_last = false;
    Direction last = Direction::North;
    if (item == Item::Push) {
        start = push;
        has_last = true;
        last = direction_between(pc.square, push);  // first Roll may not reverse Push
    }
    (void)o;  // exhaustive path legality needs only distance; exact orientation is reconstructed by Position.
    exhaustive_paths(p, id, m, start, distance, has_last, last, out);
}

std::uint32_t partial_index(Square s, Orientation o, int rem, bool has_last, Direction last) {
    const auto st = OrientationTable::instance().gesture_state_id(o);
    const auto lc = has_last ? unsigned(last) : 4u;
    return unsigned(s) | (unsigned(st) << 6) | (unsigned(rem) << 11) | (lc << 15);
}

struct Scratch {
    static constexpr std::size_t FinalSize = 1u << 14, Mask = FinalSize - 1;
    std::array<std::uint32_t, 1u << 18> partial{};
    std::uint32_t partial_generation = 1;
    std::array<Key, FinalSize> keys{};
    std::array<std::uint32_t, FinalSize> marks{};
    std::uint32_t final_generation = 1;

    std::uint32_t next_partial() {
        if (++partial_generation == 0) { partial.fill(0); partial_generation = 1; }
        return partial_generation;
    }
    std::uint32_t next_final() {
        if (++final_generation == 0) { marks.fill(0); final_generation = 1; }
        return final_generation;
    }
    bool insert(Key k, std::uint32_t generation) {
        std::size_t slot = std::size_t((k * 11400714819323198485ULL) >> 50) & Mask;
        for (std::size_t probe = 0; probe < FinalSize; ++probe) {
            if (marks[slot] != generation) {
                marks[slot] = generation;
                keys[slot] = k;
                return true;
            }
            if (keys[slot] == k) return false;
            slot = (slot + 1) & Mask;
        }
        return true;
    }
};
Scratch& scratch() { static thread_local Scratch value; return value; }

struct Gen {
    Position& p;
    Color mover, opponent;
    int own_before, opp_before;
    bool tactical;
    Scratch& scratch;
    std::uint32_t partial_generation = 0, final_generation = 0;
    std::vector<SearchMove> out;
};

void emit(Gen& g, Move& m, Square cur, Orientation o) {
    if (g.tactical) {
        if (g.p.adjacent_enemies(cur, g.mover, m.piece) != 1) return;
        auto enemy = g.p.single_adjacent_enemy(cur, g.mover, m.piece);
        const auto& t = OrientationTable::instance();
        if (t.top_gesture(o) == t.top_gesture(g.p.piece(enemy).orientation)) return;
    }
    UndoState undo;
    g.p.do_move(m, undo);
    const Key k = g.p.search_key();
    const int swing = (g.p.captures(g.mover) - g.own_before) -
                      (g.p.captures(g.opponent) - g.opp_before);
    g.p.undo_move(undo);
    if (g.tactical && swing == 0) return;
    if (g.scratch.insert(k, g.final_generation)) g.out.push_back({m, swing});
}

void reduced_paths(Gen& g, PieceId id, Move& m, Square cur, Orientation o, int rem,
                   bool has_last, Direction last) {
    if (rem == 0) {
        emit(g, m, cur, o);
        return;
    }
    const bool final = rem == 1;
    const auto& t = OrientationTable::instance();
    for (int di = 0; di < 4; ++di) {
        Direction d = Dirs[di];
        if (has_last && d == Opp[int(last)]) continue;
        Square next = steps().next[int(cur)][di];
        if (next == NoSquare || g.p.occupied(next, id)) continue;
        if (final && g.p.adjacent_enemies(next, g.mover, id) >= 2) continue;
        Orientation next_o = t.roll(o, d);
        auto idx = partial_index(next, next_o, rem - 1, true, d);
        if (g.scratch.partial[idx] == g.partial_generation) continue;
        g.scratch.partial[idx] = g.partial_generation;
        m.path[m.path_length++] = next;
        reduced_paths(g, id, m, next, next_o, rem - 1, true, d);
        --m.path_length;
    }
}

void reduced_item(Gen& g, PieceId id, const PieceState& pc, Item item, Square push) {
    Move m;
    m.piece = id;
    m.item = item;
    m.push_to = push;
    m.path[0] = pc.square;
    m.path_length = 1;
    Orientation o = modifier_orientation(pc.orientation, item);
    const int distance = modifier_distance(pc.orientation, item);
    Square start = pc.square;
    bool has_last = false;
    Direction last = Direction::North;
    if (item == Item::Push) {
        start = push;
        has_last = true;
        last = direction_between(pc.square, push);
    }
    g.partial_generation = g.scratch.next_partial();
    const auto idx = partial_index(start, o, distance, has_last, last);
    g.scratch.partial[idx] = g.partial_generation;
    reduced_paths(g, id, m, start, o, distance, has_last, last);
}

std::vector<SearchMove> generate_search_internal(Position& p, bool tactical) {
    const Color mover = p.side_to_move();
    Gen g{p, mover, opposite(mover), p.captures(mover), p.captures(opposite(mover)),
          tactical, scratch(), 0, 0, {}};
    g.final_generation = g.scratch.next_final();
    g.out.reserve(768);

    for (int i = 0; i < PieceCount; ++i) {
        auto id = PieceId(i);
        const auto& pc = p.piece(id);
        if (!pc.alive() || piece_color(id) != mover) continue;
        reduced_item(g, id, pc, Item::None, NoSquare);

        if (p.item_count(mover, 0) > 0) {
            for (int di = 0; di < 4; ++di) {
                Square push = steps().next[int(pc.square)][di];
                if (push != NoSquare && !p.occupied(push, id)) reduced_item(g, id, pc, Item::Push, push);
            }
        }
        if (p.item_count(mover, 1) > 0) {
            // Opposite quarter-turns are different exact orientations but the
            // handbook's Gesture State reduction makes each inverse pair
            // strategically identical. Search only one representative of each
            // reduced Rotation class; exhaustive legality still generates all six.
            std::array<bool, 27> seen_rotation_state{};
            const auto& t = OrientationTable::instance();
            for (Item rotation : Rotations) {
                Orientation ro = t.apply_rotation(pc.orientation, rotation);
                auto st = t.gesture_state_id(ro);
                if (seen_rotation_state[st]) continue;
                seen_rotation_state[st] = true;
                reduced_item(g, id, pc, rotation, NoSquare);
            }
        }
        if (p.item_count(mover, 2) > 0) {
            reduced_item(g, id, pc, Item::StepShort, NoSquare);
            reduced_item(g, id, pc, Item::StepLong, NoSquare);
        }
    }
    return g.out;
}
}  // namespace

std::vector<Move> generate_legal_moves(const Position& p) {
    std::vector<Move> out;
    out.reserve(4096);
    const Color mover = p.side_to_move();
    for (int i = 0; i < PieceCount; ++i) {
        auto id = PieceId(i);
        const auto& pc = p.piece(id);
        if (!pc.alive() || piece_color(id) != mover) continue;
        exhaustive_item(p, id, pc, Item::None, NoSquare, out);
        if (p.item_count(mover, 0) > 0) {
            for (int di = 0; di < 4; ++di) {
                Square push = steps().next[int(pc.square)][di];
                if (push != NoSquare && !p.occupied(push, id)) exhaustive_item(p, id, pc, Item::Push, push, out);
            }
        }
        if (p.item_count(mover, 1) > 0)
            for (Item rotation : Rotations) exhaustive_item(p, id, pc, rotation, NoSquare, out);
        if (p.item_count(mover, 2) > 0) {
            exhaustive_item(p, id, pc, Item::StepShort, NoSquare, out);
            exhaustive_item(p, id, pc, Item::StepLong, NoSquare, out);
        }
    }
    return out;
}

std::vector<Move> generate_unique_moves(Position& p) {
    auto all = generate_legal_moves(p);
    std::vector<Move> unique;
    unique.reserve(all.size());
    std::unordered_set<Key> seen;
    seen.reserve(all.size() * 2);
    for (const auto& move : all) {
        UndoState undo;
        p.do_move(move, undo);
        const Key k = p.key();
        p.undo_move(undo);
        if (seen.insert(k).second) unique.push_back(move);
    }
    return unique;
}

std::vector<SearchMove> generate_search_moves_info(Position& p) { return generate_search_internal(p, false); }
std::vector<SearchMove> generate_tactical_moves_info(Position& p) { return generate_search_internal(p, true); }
std::vector<Move> generate_search_moves(Position& p) {
    auto info = generate_search_moves_info(p);
    std::vector<Move> moves;
    moves.reserve(info.size());
    for (const auto& entry : info) moves.push_back(entry.move);
    return moves;
}
}  // namespace rpsc
