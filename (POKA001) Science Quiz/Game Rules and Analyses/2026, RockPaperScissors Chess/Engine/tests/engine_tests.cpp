#include <cassert>
#include <iostream>
#include <unordered_set>
#include "engine.h"
#include "movegen.h"
#include "notation.h"
#include "orientation.h"

int main() {
    using namespace rpsc;
    const auto& ot = OrientationTable::instance();
    assert(ot.size() == 24);
    for (Gesture g : {Gesture::Scissors, Gesture::Rock, Gesture::Paper}) {
        auto o = ot.canonical(g, WristDirection::South);
        assert(ot.top_gesture(o) == g);
        auto n = ot.roll(o, Direction::North);
        auto back = ot.roll(n, Direction::South);
        assert(back == o);
        auto q = o; for (int i = 0; i < 4; ++i) q = ot.rotate_right(q); assert(q == o);
    }

    Position p;
    const auto legal = generate_legal_moves(p);
    const auto unique = generate_unique_moves(p);
    const auto strategic = generate_search_moves(p);
    std::cout << "initial canonical moves: " << legal.size() << "\n";
    std::cout << "initial exact positions: " << unique.size() << "\n";
    std::cout << "initial strategic search positions: " << strategic.size() << "\n";
    assert(legal.size() == 161);
    assert(unique.size() == 145);
    assert(strategic.size() == 84);
    assert(perft(p, 1) == 161);
    assert(perft(p, 2) == 25575);

    Move sample;
    assert(parse_move(p, "W3: e1-e2-e3-e4-d4-c4", sample));
    assert(format_move(sample) == "W3: e1-e2-e3-e4-d4-c4");

    Engine engine; SearchLimits limits; limits.depth = 4;
    const auto result = engine.go(limits);
    assert(result.has_move);
    assert(result.depth == 4);
    assert(!result.pv.empty());
    assert(engine.position().is_legal_path(result.best_move));
    std::cout << "depth 4 best: " << format_move(result.best_move) << " score "
              << static_cast<double>(result.value) / ScoreUnit << " nodes " << result.nodes << "\n";
    return 0;
}
