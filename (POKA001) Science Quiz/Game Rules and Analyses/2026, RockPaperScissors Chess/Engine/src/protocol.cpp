#include "protocol.h"

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "engine.h"
#include "notation.h"
#include "perft.h"

namespace rpsc {
namespace {
const char* item_name(int bucket) { return bucket == 0 ? "Pu" : bucket == 1 ? "Ro" : "St"; }
bool parse_side(const std::string& text, Color& color) {
    if (text == "W") { color = Color::White; return true; }
    if (text == "B") { color = Color::Black; return true; }
    return false;
}
void parse_limits(std::istringstream& in, SearchLimits& limits) {
    std::string key;
    while (in >> key) {
        if (key == "depth") in >> limits.depth;
        else if (key == "movetime") {
            long long ms;
            in >> ms;
            limits.movetime = std::chrono::milliseconds(ms);
            limits.depth = 84;
        } else if (key == "nodes") in >> limits.nodes;
        else if (key == "multipv") in >> limits.multipv;
    }
}
void print_search(const Position& position, const SearchResult& result) {
    if (!result.has_move) {
        std::cout << "bestmove (none)\n";
        return;
    }
    const auto ms = std::max<std::int64_t>(1, result.elapsed.count());
    const auto nps = result.nodes * 1000ULL / static_cast<std::uint64_t>(ms);
    const bool white = position.side_to_move() == Color::White;
    if (!result.lines.empty()) {
        for (std::size_t i = 0; i < result.lines.size(); ++i) {
            const auto& line = result.lines[i];
            Value value = white ? line.value : -line.value;
            std::cout << "info multipv " << i + 1 << " depth " << result.depth
                      << " seldepth " << result.seldepth << " nodes " << result.nodes
                      << " nps " << nps << " score " << std::fixed << std::setprecision(2)
                      << double(value) / ScoreUnit;
            if (!line.pv.empty()) std::cout << " pv " << format_pv(position, line.pv);
            std::cout << '\n';
        }
    } else {
        Value value = white ? result.value : -result.value;
        std::cout << "info depth " << result.depth << " seldepth " << result.seldepth
                  << " nodes " << result.nodes << " nps " << nps << " score "
                  << std::fixed << std::setprecision(2) << double(value) / ScoreUnit;
        if (!result.pv.empty()) std::cout << " pv " << format_pv(position, result.pv);
        std::cout << '\n';
    }
    std::cout << "bestmove " << format_move(result.best_move) << '\n';
}
}  // namespace

int run_protocol() {
    Engine engine;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        std::istringstream in(line);
        std::string command;
        in >> command;
        if (command == "quit" || command == "exit") break;
        if (command == "rpsc") {
            std::cout << "id name RPSC Engine 0.15.0\nid author Jungwoo Kim\nrpscok\n";
        } else if (command == "isready") std::cout << "readyok\n";
        else if (command == "newgame" || line == "position startpos") {
            engine.new_game(); std::cout << "ok\n";
        } else if (command == "clear") {
            engine.clear_search(); std::cout << "ok\n";
        } else if (command == "items") {
            std::string side; int pu, ro, st; Color color;
            if (!(in >> side >> pu >> ro >> st) || !parse_side(side, color) || pu < 0 || ro < 0 || st < 0)
                std::cout << "error invalid items\n";
            else { engine.position().set_items(color, pu, ro, st); std::cout << "ok\n"; }
        } else if (command == "gain") {
            std::string side, item; Color color;
            if (!(in >> side >> item) || !parse_side(side, color) ||
                (item != "Pu" && item != "Ro" && item != "St"))
                std::cout << "error invalid gain\n";
            else {
                auto inventory = engine.position().items(color);
                int bucket = item == "Pu" ? 0 : item == "Ro" ? 1 : 2;
                ++inventory[bucket];
                engine.position().set_items(color, inventory[0], inventory[1], inventory[2]);
                std::cout << "ok\n";
            }
        } else if (command == "side") {
            std::string side; Color color;
            if (!(in >> side) || !parse_side(side, color)) std::cout << "error invalid side\n";
            else { engine.position().set_side_to_move(color); std::cout << "ok\n"; }
        } else if (command == "match") {
            int white_quiz, black_quiz, remaining;
            if (!(in >> white_quiz >> black_quiz >> remaining) || white_quiz < 0 || black_quiz < 0 || remaining < -1)
                std::cout << "error invalid match context\n";
            else { engine.position().set_match_context(white_quiz, black_quiz, remaining); std::cout << "ok\n"; }
        } else if (command == "show" || command == "d") std::cout << engine.position().debug_string();
        else if (command == "legal") std::cout << generate_legal_moves(engine.position()).size() << '\n';
        else if (command == "moves") std::cout << generate_search_moves_info(engine.position()).size() << '\n';
        else if (command == "perft") {
            int depth; in >> depth; std::cout << "perft " << depth << ' ' << engine.perft(depth) << '\n';
        } else if (command == "divide") {
            int depth; in >> depth;
            if (depth < 1) std::cout << "error invalid depth\n";
            else {
                std::uint64_t total = 0;
                auto moves = generate_legal_moves(engine.position());
                for (const auto& move : moves) {
                    UndoState undo;
                    engine.position().do_move(move, undo);
                    auto n = perft(engine.position(), depth - 1);
                    engine.position().undo_move(undo);
                    total += n;
                    std::cout << format_move(move) << ' ' << n << '\n';
                }
                std::cout << "total " << total << '\n';
            }
        } else if (command == "move") {
            std::string rest; std::getline(in, rest);
            if (!rest.empty() && rest.front() == ' ') rest.erase(rest.begin());
            Move move;
            if (!parse_move(engine.position(), rest, move)) std::cout << "error illegal move\n";
            else { UndoState undo; engine.position().do_move(move, undo); std::cout << "ok\n"; }
        } else if (command == "go") {
            SearchLimits limits; limits.depth = 8; parse_limits(in, limits);
            print_search(engine.position(), engine.go(limits));
        } else if (command == "chooseorder") {
            SearchLimits limits; limits.depth = 4; parse_limits(in, limits);
            auto choice = engine.choose_order(limits);
            std::cout << "info order first score " << std::fixed << std::setprecision(2)
                      << double(choice.white_value) / ScoreUnit << " depth " << choice.probe.depth
                      << " nodes " << choice.probe.nodes << '\n';
            std::cout << "info order second score " << std::fixed << std::setprecision(2)
                      << double(-choice.white_value) / ScoreUnit << '\n';
            std::cout << "bestorder " << (choice.choose_first ? "first" : "second") << '\n';
        } else if (command == "chooseitem") {
            std::string side; Color chooser;
            if (!(in >> side) || !parse_side(side, chooser)) {
                std::cout << "error invalid chooseitem\n"; continue;
            }
            SearchLimits limits; limits.depth = 8; parse_limits(in, limits);
            auto choice = engine.choose_item(chooser, limits);
            for (std::size_t i = 0; i < choice.lines.size(); ++i) {
                const auto& x = choice.lines[i];
                Value value = chooser == Color::White ? x.white_value : -x.white_value;
                std::cout << "info item " << i + 1 << ' ' << item_name(x.bucket) << " score "
                          << std::fixed << std::setprecision(2) << double(value) / ScoreUnit
                          << " depth " << x.probe.depth << " nodes " << x.probe.nodes << '\n';
            }
            std::cout << "bestitem " << (choice.best_bucket >= 0 ? item_name(choice.best_bucket) : "(none)") << '\n';
        } else if (command == "chooseinitial") {
            SearchLimits limits; limits.depth = 8; parse_limits(in, limits);
            auto choice = engine.choose_initial(limits);
            for (std::size_t i = 0; i < choice.lines.size(); ++i) {
                const auto& x = choice.lines[i];
                std::cout << "info initial " << i + 1 << ' ' << (x.choose_first ? "first" : "second")
                          << ' ' << item_name(x.bucket) << " score " << std::fixed << std::setprecision(2)
                          << double(x.chooser_value) / ScoreUnit << " depth " << x.probe.depth
                          << " nodes " << x.probe.nodes << '\n';
            }
            std::cout << "bestinitial " << (choice.choose_first ? "first" : "second") << ' '
                      << (choice.best_bucket >= 0 ? item_name(choice.best_bucket) : "(none)") << '\n';
        } else if (command == "bench") {
            engine.new_game(); SearchLimits limits; limits.depth = 4;
            auto result = engine.go(limits);
            std::cout << "bench depth 4 nodes " << result.nodes << " time " << result.elapsed.count()
                      << " ms bestmove " << (result.has_move ? format_move(result.best_move) : "(none)") << '\n';
        } else if (command == "help") {
            std::cout << "newgame | items W/B pu ro st | gain W/B Pu/Ro/St | match Wquiz Bquiz remainingPlies | side W/B | move NOTATION | go depth N | go movetime MS [multipv N] | chooseorder | chooseitem W/B | chooseinitial | legal | moves | perft N | divide N | bench | show | clear | quit\n";
        } else std::cout << "error unknown command\n";
    }
    return 0;
}
}  // namespace rpsc
