#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>
#include <chrono>
#include <iostream>
#include <unordered_set>

#include "engine.h"
#include "evaluate.h"
#include "movegen.h"
#include "notation.h"
#include "orientation.h"
#include "perft.h"

using namespace rpsc;

static GestureState tau_ew(GestureState s) { return {s.ns, s.ud, s.ew}; }
static GestureState tau_ns(GestureState s) { return {s.ew, s.ns, s.ud}; }
static GestureState tau_ud(GestureState s) { return {s.ud, s.ew, s.ns}; }
static bool same_state(GestureState a, GestureState b) {
    return a.ud == b.ud && a.ns == b.ns && a.ew == b.ew;
}

static void verify_generator(Position& p) {
    const auto legal = generate_legal_moves(p);
    std::unordered_set<Key> expected, actual;
    expected.reserve(legal.size() * 2);
    for (const auto& move : legal) {
        UndoState undo;
        p.do_move(move, undo);
        expected.insert(p.search_key());
        p.undo_move(undo);
    }
    const auto search = generate_search_moves_info(p);
    actual.reserve(search.size() * 2);
    for (const auto& entry : search) {
        assert(p.is_legal_path(entry.move));
        UndoState undo;
        p.do_move(entry.move, undo);
        actual.insert(p.search_key());
        p.undo_move(undo);
    }
    assert(actual == expected);
}

static void verify_roundtrip(Position& p, const std::vector<Move>& moves) {
    const Key exact = p.key(), reduced = p.search_key();
    for (const auto& move : moves) {
        const std::string text = format_move(move);
        Move parsed;
        assert(parse_move(p, text, parsed));
        assert(parsed == move);
        UndoState undo;
        p.do_move(move, undo);
        p.undo_move(undo);
        assert(p.key() == exact && p.search_key() == reduced);
    }
}

static bool distinct(const std::vector<RootLine>& lines) {
    for (std::size_t i = 0; i < lines.size(); ++i)
        for (std::size_t j = i + 1; j < lines.size(); ++j)
            if (lines[i].move == lines[j].move) return false;
    return true;
}

static Gesture loses_to(Gesture winner) {
    if (winner == Gesture::Scissors) return Gesture::Paper;
    if (winner == Gesture::Rock) return Gesture::Scissors;
    return Gesture::Rock;
}

int main() {
    const auto& t = OrientationTable::instance();
    assert(t.size() == 24);

    // Exact six-Rotation model and Chapter 3 reduced tau model.
    for (int i = 0; i < 24; ++i) {
        const auto o = Orientation(i);
        assert(t.apply_rotation(t.apply_rotation(o, Item::RotateNorth), Item::RotateSouth) == o);
        assert(t.apply_rotation(t.apply_rotation(o, Item::RotateSouth), Item::RotateNorth) == o);
        assert(t.apply_rotation(t.apply_rotation(o, Item::RotateEast), Item::RotateWest) == o);
        assert(t.apply_rotation(t.apply_rotation(o, Item::RotateWest), Item::RotateEast) == o);
        assert(t.apply_rotation(t.apply_rotation(o, Item::RotateLeft), Item::RotateRight) == o);
        assert(t.apply_rotation(t.apply_rotation(o, Item::RotateRight), Item::RotateLeft) == o);

        const auto s = t.gesture_state(o);
        assert(same_state(t.gesture_state(t.apply_rotation(o, Item::RotateNorth)), tau_ew(s)));
        assert(same_state(t.gesture_state(t.apply_rotation(o, Item::RotateSouth)), tau_ew(s)));
        assert(same_state(t.gesture_state(t.apply_rotation(o, Item::RotateEast)), tau_ns(s)));
        assert(same_state(t.gesture_state(t.apply_rotation(o, Item::RotateWest)), tau_ns(s)));
        assert(same_state(t.gesture_state(t.apply_rotation(o, Item::RotateLeft)), tau_ud(s)));
        assert(same_state(t.gesture_state(t.apply_rotation(o, Item::RotateRight)), tau_ud(s)));
    }

    // Handbook W2 examples: RoN -> S/3, RoE -> P/5, RoR -> R/4.
    const auto w2 = t.canonical(Gesture::Rock, WristDirection::South);
    auto ron = t.apply_rotation(w2, Item::RotateNorth);
    auto roe = t.apply_rotation(w2, Item::RotateEast);
    auto ror = t.apply_rotation(w2, Item::RotateRight);
    assert(t.top_gesture(ron) == Gesture::Scissors && base_roll_length(t.top_gesture(ron)) == 3);
    assert(t.top_gesture(roe) == Gesture::Paper && base_roll_length(t.top_gesture(roe)) == 5);
    assert(t.top_gesture(ror) == Gesture::Rock && base_roll_length(t.top_gesture(ror)) == 4);

    Position p;
    auto legal = generate_legal_moves(p);
    const auto initial_legal = legal.size();
    const auto initial_unique = generate_unique_moves(p).size();
    const auto initial_search = generate_search_moves(p).size();
    const auto p1 = perft(p, 1), p2 = perft(p, 2), p3 = perft(p, 3);
    verify_generator(p);
    verify_roundtrip(p, legal);

    // New item rules: all six Rotation actions exist; Push immediate return never exists.
    p.set_items(Color::White, 1, 1, 1);
    p.set_items(Color::Black, 1, 1, 1);
    legal = generate_legal_moves(p);
    const auto item_legal = legal.size();
    const auto item_unique = generate_unique_moves(p).size();
    const auto item_search = generate_search_moves(p).size();
    bool push_return = false, rn = false, rs = false, re = false, rw = false, rl = false, rr = false;
    bool ss = false, sl = false;
    for (const auto& move : legal) {
        if (move.item == Item::Push && move.path_length > 1 && move.path[1] == move.from()) push_return = true;
        rn |= move.item == Item::RotateNorth;
        rs |= move.item == Item::RotateSouth;
        re |= move.item == Item::RotateEast;
        rw |= move.item == Item::RotateWest;
        rl |= move.item == Item::RotateLeft;
        rr |= move.item == Item::RotateRight;
        ss |= move.item == Item::StepShort;
        sl |= move.item == Item::StepLong;
    }
    assert(!push_return && rn && rs && re && rw && rl && rr && ss && sl);
    verify_generator(p);
    verify_roundtrip(p, legal);

    // Explicitly reject the now-illegal Push return e3>f3-e3... pattern at first Roll.
    Position push_test;
    push_test.set_items(Color::White, 1, 0, 0);
    Move illegal_push;
    illegal_push.piece = PieceId::W1;
    illegal_push.item = Item::Push;
    illegal_push.push_to = make_square(1, 0);  // a1>b1
    illegal_push.path[0] = make_square(0, 0);
    illegal_push.path[1] = make_square(0, 0);  // b1-a1: immediate return
    illegal_push.path[2] = make_square(0, 1);
    illegal_push.path[3] = make_square(1, 1);
    illegal_push.path_length = 4;
    assert(!push_test.is_legal_path(illegal_push));

    // Reset preserves score/items and hands the next scheduled ply to Black.
    Position reset_test;
    reset_test.set_items(Color::White, 1, 1, 1);
    reset_test.set_items(Color::Black, 2, 1, 0);
    reset_test.set_match_context(5, 4, 10);
    reset_test.piece(PieceId::B2).square = NoSquare;
    reset_test.piece(PieceId::B3).square = NoSquare;
    reset_test.piece(PieceId::B4).square = NoSquare;
    reset_test.piece(PieceId::B1).square = make_square(1, 3);  // b4
    // W1: a1-a2-a3-b3; choose B1's top so W1 wins the final adjacent combat.
    Orientation wo = reset_test.piece(PieceId::W1).orientation;
    wo = t.roll(wo, Direction::North);
    wo = t.roll(wo, Direction::North);
    wo = t.roll(wo, Direction::East);
    reset_test.piece(PieceId::B1).orientation = t.canonical(loses_to(t.top_gesture(wo)), WristDirection::North);
    Move finishing;
    assert(parse_move(reset_test, "W1: a1-a2-a3-b3", finishing));
    UndoState reset_undo;
    const auto before_items_w = reset_test.items(Color::White);
    const auto before_items_b = reset_test.items(Color::Black);
    auto outcome = reset_test.do_move(finishing, reset_undo);
    assert(outcome.has_capture && outcome.reset);
    assert(reset_test.alive_count(Color::White) == 4 && reset_test.alive_count(Color::Black) == 4);
    assert(reset_test.side_to_move() == Color::Black);
    assert(reset_test.items(Color::White) == before_items_w && reset_test.items(Color::Black) == before_items_b);
    assert(reset_test.quiz(Color::White) == 5 && reset_test.quiz(Color::Black) == 4);
    assert(reset_test.captures(Color::White) == 1);
    assert(reset_test.remaining_board_plies() == 9);

    // Removed-piece orientation is not a game state. Equivalent captured states
    // must share exact/reduced keys so TT and successor deduplication stay canonical.
    Position dead_a, dead_b;
    dead_a.piece(PieceId::W4).square = NoSquare;
    dead_b.piece(PieceId::W4).square = NoSquare;
    dead_b.piece(PieceId::W4).orientation = t.roll(dead_b.piece(PieceId::W4).orientation, Direction::North);
    assert(dead_a.key() == dead_b.key());
    assert(dead_a.search_key() == dead_b.search_key());

    Position symmetric;
    symmetric.set_match_context(0, 0, 20);
    assert(evaluate_white(symmetric) == 0);
    Position with_item = symmetric;
    with_item.set_items(Color::White, 1, 0, 0);
    assert(evaluate_white(with_item) > 0);

    Engine engine;
    SearchLimits d3;
    d3.depth = 3;
    d3.multipv = 3;
    auto search = engine.go(d3);
    assert(search.has_move && search.depth == 3 && search.lines.size() == 3 && distinct(search.lines));
    for (const auto& line : search.lines) assert(engine.position().is_legal_path(line.move));

    Engine item_engine;
    item_engine.position().set_items(Color::White, 1, 1, 1);
    item_engine.position().set_items(Color::Black, 1, 1, 1);
    SearchLimits d2;
    d2.depth = 2;
    d2.multipv = 3;
    auto item_result = item_engine.go(d2);
    assert(item_result.has_move && item_result.depth == 2 && item_result.lines.size() == 3 && distinct(item_result.lines));

    SearchLimits choice;
    choice.depth = 2;
    auto items = item_engine.choose_item(Color::White, choice);
    assert(items.lines.size() == 3 && items.best_bucket >= 0);
    Engine initial_engine;
    auto initial = initial_engine.choose_initial(choice);
    assert(initial.lines.size() == 6 && initial.best_bucket >= 0);

    // Same-state timed continuation must retain a completed result.
    Engine timed_engine;
    timed_engine.position().set_items(Color::White, 1, 1, 1);
    timed_engine.position().set_items(Color::Black, 1, 1, 1);
    SearchLimits timed;
    timed.depth = 84;
    timed.movetime = std::chrono::milliseconds(80);
    auto a = timed_engine.go(timed);
    auto b = timed_engine.go(timed);
    assert(a.has_move && b.has_move && b.depth >= a.depth);

    std::cout << "initial legal/unique/search " << initial_legal << '/' << initial_unique << '/' << initial_search << '\n';
    std::cout << "initial perft " << p1 << '/' << p2 << '/' << p3 << '\n';
    std::cout << "item-rich legal/unique/search " << item_legal << '/' << item_unique << '/' << item_search << '\n';
    std::cout << "RPSC Engine 0.15.0 new-rules regression suite passed\n";
}
