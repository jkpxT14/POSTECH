#ifdef NDEBUG
#undef NDEBUG
#endif
#include <array>
#include <cassert>
#include <iostream>
#include <utility>
#include <unordered_set>
#include <vector>

#include "engine.h"
#include "movegen.h"
#include "notation.h"
#include "orientation.h"

namespace {

void verify_round_trip_and_undo(rpsc::Position& position, const std::vector<rpsc::Move>& moves) {
    using namespace rpsc;
    const Key exact_before = position.key();
    const Key search_before = position.search_key();
    for (const auto& move : moves) {
        assert(position.is_legal_path(move));
        const std::string text = format_move(move);
        Move reparsed;
        assert(parse_move(position, text, reparsed));
        assert(reparsed == move);
        UndoState undo;
        position.do_move(move, undo);
        position.undo_move(undo);
        assert(position.key() == exact_before);
        assert(position.search_key() == search_before);
    }
}

bool all_distinct(const std::vector<rpsc::RootLine>& lines) {
    for (std::size_t i = 0; i < lines.size(); ++i)
        for (std::size_t j = i + 1; j < lines.size(); ++j)
            if (lines[i].move == lines[j].move) return false;
    return true;
}

void verify_search_generator_against_exact(rpsc::Position& position) {
    using namespace rpsc;
    const auto legal = generate_legal_moves(position);
    std::unordered_set<Key> expected;
    for (const auto& move : legal) {
        UndoState undo;
        position.do_move(move, undo);
        expected.insert(position.search_key());
        position.undo_move(undo);
    }

    const auto search = generate_search_moves_info(position);
    std::unordered_set<Key> actual;
    for (const auto& entry : search) {
        UndoState undo;
        position.do_move(entry.move, undo);
        actual.insert(position.search_key());
        position.undo_move(undo);
    }
    assert(actual == expected);
}

void verify_tactical_generator(rpsc::Position& position) {
    using namespace rpsc;
    const auto full = generate_search_moves_info(position);
    const auto tactical = generate_tactical_moves_info(position);
    std::unordered_set<Key> expected;
    for (const auto& entry : full) {
        if (entry.capture_swing == 0) continue;
        UndoState undo;
        position.do_move(entry.move, undo);
        expected.insert(position.search_key());
        position.undo_move(undo);
    }
    std::unordered_set<Key> actual;
    for (const auto& entry : tactical) {
        assert(entry.capture_swing != 0);
        UndoState undo;
        position.do_move(entry.move, undo);
        actual.insert(position.search_key());
        position.undo_move(undo);
    }
    assert(actual == expected);
}


void verify_reduced_orientation_equivalence() {
    using namespace rpsc;
    const auto& table = OrientationTable::instance();
    for (int a = 0; a < 24; ++a) {
        const auto oa = static_cast<Orientation>(a);
        for (int b = a + 1; b < 24; ++b) {
            const auto ob = static_cast<Orientation>(b);
            if (table.gesture_state_id(oa) != table.gesture_state_id(ob)) continue;
            assert(table.top_gesture(oa) == table.top_gesture(ob));
            assert(table.gesture_state_id(table.rotate_left(oa)) ==
                   table.gesture_state_id(table.rotate_left(ob)));
            assert(table.gesture_state_id(table.rotate_right(oa)) ==
                   table.gesture_state_id(table.rotate_right(ob)));
            for (Direction direction : {Direction::North, Direction::South, Direction::East,
                                        Direction::West}) {
                assert(table.gesture_state_id(table.roll(oa, direction)) ==
                       table.gesture_state_id(table.roll(ob, direction)));
            }
        }
        // RoL and RoR preserve the top face and are the same reduced rho operation even though
        // their exact wrist directions differ.
        assert(table.gesture_state_id(table.rotate_left(oa)) ==
               table.gesture_state_id(table.rotate_right(oa)));
    }
}

void verify_item_roll_lengths_and_consumption() {
    using namespace rpsc;
    Position position;
    position.set_items(Color::White, 1, 1, 1);
    const auto& table = OrientationTable::instance();
    const auto before_items = position.items(Color::White);
    const auto moves = generate_legal_moves(position);
    bool checked_push = false, checked_left = false, checked_right = false;
    bool checked_short = false, checked_long = false;
    for (const auto& move : moves) {
        if (move.piece != PieceId::W1 || move.item == Item::None) continue;
        const auto& piece = position.piece(move.piece);
        const int base = base_roll_length(table.top_gesture(piece.orientation));
        const int rolls = static_cast<int>(move.path_length) - 1;
        if (move.item == Item::Push) {
            assert(rolls == base);
            assert(move.push_to != NoSquare);
            checked_push = true;
        } else if (move.item == Item::RotateLeft) {
            assert(rolls == base);
            assert(table.top_gesture(table.rotate_left(piece.orientation)) ==
                   table.top_gesture(piece.orientation));
            checked_left = true;
        } else if (move.item == Item::RotateRight) {
            assert(rolls == base);
            assert(table.top_gesture(table.rotate_right(piece.orientation)) ==
                   table.top_gesture(piece.orientation));
            checked_right = true;
        } else if (move.item == Item::StepShort) {
            assert(rolls == base - 1);
            checked_short = true;
        } else if (move.item == Item::StepLong) {
            assert(rolls == base + 1);
            checked_long = true;
        }

        UndoState undo;
        position.do_move(move, undo);
        const auto after_items = position.items(Color::White);
        const int bucket = move.item == Item::Push
                               ? 0
                               : (move.item == Item::RotateLeft || move.item == Item::RotateRight)
                                     ? 1
                                     : 2;
        for (int i = 0; i < 3; ++i)
            assert(after_items[static_cast<std::size_t>(i)] ==
                   before_items[static_cast<std::size_t>(i)] - (i == bucket ? 1 : 0));
        position.undo_move(undo);
        assert(position.items(Color::White) == before_items);

        if (checked_push && checked_left && checked_right && checked_short && checked_long) break;
    }
    assert(checked_push && checked_left && checked_right && checked_short && checked_long);
}

void verify_official_dice_net_anchor() {
    using namespace rpsc;
    const auto& table = OrientationTable::instance();
    const auto s = table.canonical(Gesture::Scissors, WristDirection::South);
    assert(table.top_gesture(s) == Gesture::Scissors);
    assert(table.wrist_direction(s) == WristDirection::South);
    // Official Figure 2: horizontal P-S-P-S, with R immediately above and below the
    // left-hand S. With that S as the top face, N/S expose Rock and E/W expose Paper.
    assert(table.top_gesture(table.roll(s, Direction::North)) == Gesture::Rock);
    assert(table.top_gesture(table.roll(s, Direction::South)) == Gesture::Rock);
    assert(table.top_gesture(table.roll(s, Direction::East)) == Gesture::Paper);
    assert(table.top_gesture(table.roll(s, Direction::West)) == Gesture::Paper);
}

void verify_match_context() {
    using namespace rpsc;
    Position p;
    p.set_match_context(7, 5, 1);
    assert(p.quiz(Color::White) == 7 && p.quiz(Color::Black) == 5);
    assert(p.score(Color::White) == 7 && p.score(Color::Black) == 5);
    const auto before = p.search_key();
    const auto moves = generate_search_moves(p);
    assert(!moves.empty());
    UndoState u;
    p.do_move(moves.front(), u);
    assert(p.remaining_board_plies() == 0);
    assert(p.search_key() != before);
    p.undo_move(u);
    assert(p.remaining_board_plies() == 1);
    assert(p.search_key() == before);
}

void verify_all_orientations() {
    using namespace rpsc;
    const auto& table = OrientationTable::instance();
    assert(table.size() == 24);
    for (int i = 0; i < 24; ++i) {
        const auto o = static_cast<Orientation>(i);
        assert(table.rotate_left(table.rotate_right(o)) == o);
        assert(table.rotate_right(table.rotate_left(o)) == o);
        assert(table.top_gesture(table.rotate_left(o)) == table.top_gesture(o));
        assert(table.top_gesture(table.rotate_right(o)) == table.top_gesture(o));
        auto r = o;
        for (int q = 0; q < 4; ++q) r = table.rotate_right(r);
        assert(r == o);
        const std::array<std::pair<Direction, Direction>, 4> inverses{{
            {Direction::North, Direction::South}, {Direction::South, Direction::North},
            {Direction::East, Direction::West}, {Direction::West, Direction::East}}};
        for (const auto& [d, inverse] : inverses) {
            assert(table.roll(table.roll(o, d), inverse) == o);
        }
    }
}

}  // namespace

int main() {
    using namespace rpsc;
    verify_official_dice_net_anchor();
    verify_all_orientations();
    verify_match_context();
    verify_reduced_orientation_equivalence();
    verify_item_roll_lengths_and_consumption();

    Position position;
    const auto legal = generate_legal_moves(position);
    const auto unique = generate_unique_moves(position);
    const auto strategic = generate_search_moves(position);
    const auto strategic_info = generate_search_moves_info(position);
    std::cout << "initial canonical moves: " << legal.size() << "\n";
    std::cout << "initial exact positions: " << unique.size() << "\n";
    std::cout << "initial strategic search positions: " << strategic.size() << "\n";
    assert(legal.size() == 161);
    assert(unique.size() == 145);
    assert(strategic.size() == 84);
    assert(strategic_info.size() == 84);
    assert(perft(position, 1) == 161);
    assert(perft(position, 2) == 25575);
    assert(perft(position, 3) == 4215782);
    verify_round_trip_and_undo(position, legal);
    verify_search_generator_against_exact(position);
    verify_tactical_generator(position);

    Position item_position;
    item_position.set_items(Color::White, 1, 1, 1);
    item_position.set_items(Color::Black, 1, 1, 1);
    const auto item_moves = generate_legal_moves(item_position);
    const auto item_unique = generate_unique_moves(item_position);
    const auto item_strategic = generate_search_moves(item_position);
    assert(item_moves.size() == 1472);
    assert(item_unique.size() == 1019);
    assert(item_strategic.size() == 427);
    bool saw_push = false, saw_rotate_left = false, saw_rotate_right = false;
    bool saw_step_short = false, saw_step_long = false;
    for (const auto& move : item_moves) {
        saw_push = saw_push || move.item == Item::Push;
        saw_rotate_left = saw_rotate_left || move.item == Item::RotateLeft;
        saw_rotate_right = saw_rotate_right || move.item == Item::RotateRight;
        saw_step_short = saw_step_short || move.item == Item::StepShort;
        saw_step_long = saw_step_long || move.item == Item::StepLong;
    }
    assert(saw_push && saw_rotate_left && saw_rotate_right && saw_step_short && saw_step_long);
    verify_round_trip_and_undo(item_position, item_moves);
    verify_search_generator_against_exact(item_position);
    verify_tactical_generator(item_position);

    Position playout = item_position;
    for (int ply = 0; ply < 16; ++ply) {
        if (ply % 4 == 0) {
            verify_search_generator_against_exact(playout);
            verify_tactical_generator(playout);
        }
        const auto moves = generate_search_moves(playout);
        assert(!moves.empty());
        const Move move = moves[static_cast<std::size_t>((ply * 37 + 11) % moves.size())];
        const Key before = playout.key();
        const std::string text = format_move(move);
        Move reparsed;
        assert(parse_move(playout, text, reparsed));
        assert(reparsed == move);
        UndoState undo;
        playout.do_move(move, undo);
        assert(playout.key() != before);
        playout.undo_move(undo);
        assert(playout.key() == before);
        playout.do_move(move, undo);
    }

    Engine engine;
    SearchLimits limits;
    limits.depth = 4;
    limits.multipv = 3;
    const auto result = engine.go(limits);
    assert(result.has_move);
    assert(result.depth == 4);
    assert(!result.pv.empty());
    assert(result.lines.size() == 3);
    assert(all_distinct(result.lines));
    assert(engine.position().is_legal_path(result.best_move));
    for (const auto& line : result.lines) {
        assert(engine.position().is_legal_path(line.move));
        assert(!line.pv.empty());
        assert(line.pv.front() == line.move);
    }

    Engine item_engine;
    item_engine.position().set_items(Color::White, 1, 1, 1);
    item_engine.position().set_items(Color::Black, 1, 1, 1);
    SearchLimits item_limits;
    item_limits.depth = 2;
    item_limits.multipv = 3;
    const auto item_result = item_engine.go(item_limits);
    assert(item_result.has_move);
    assert(item_result.depth == 2);
    assert(item_result.lines.size() == 3);
    assert(all_distinct(item_result.lines));

    const Key before_choice = item_engine.position().key();
    SearchLimits choice_limits;
    choice_limits.depth = 3;
    choice_limits.nodes = 90000;
    const auto choice = item_engine.choose_item(Color::White, choice_limits);
    assert(choice.best_bucket >= 0 && choice.best_bucket < 3);
    assert(choice.lines.size() == 3);
    std::unordered_set<int> choice_buckets;
    for (const auto& line : choice.lines) {
        assert(line.bucket >= 0 && line.bucket < 3);
        choice_buckets.insert(line.bucket);
        assert(line.probe.depth >= 1);
        assert(!line.probe.pv.empty());
    }
    assert(choice_buckets.size() == 3);
    assert(item_engine.position().key() == before_choice);

    Engine opening_engine;
    SearchLimits opening_limits;
    opening_limits.depth = 2;
    opening_limits.nodes = 120000;
    const auto opening = opening_engine.choose_initial(opening_limits);
    assert(opening.best_bucket >= 0 && opening.best_bucket < 3);
    assert(opening.lines.size() == 6);
    bool saw_first = false, saw_second = false;
    std::unordered_set<int> first_items, second_items;
    for (const auto& line : opening.lines) {
        saw_first = saw_first || line.choose_first;
        saw_second = saw_second || !line.choose_first;
        (line.choose_first ? first_items : second_items).insert(line.bucket);
        assert(line.probe.depth >= 1);
        assert(!line.probe.pv.empty());
    }
    assert(saw_first && saw_second);
    assert(first_items.size() == 3 && second_items.size() == 3);

    std::cout << "depth 4 best: " << format_move(result.best_move) << " score "
              << static_cast<double>(result.value) / ScoreUnit << " nodes " << result.nodes
              << "\n";
    return 0;
}
