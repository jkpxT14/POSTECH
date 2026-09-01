#include "protocol.h"

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "notation.h"

namespace rpsc {
namespace {

const char* item_name(int bucket) {
    return bucket == 0 ? "Pu" : bucket == 1 ? "Ro" : "St";
}

bool parse_side(char side, Color& color) {
    if (side == 'W') {
        color = Color::White;
        return true;
    }
    if (side == 'B') {
        color = Color::Black;
        return true;
    }
    return false;
}

Position item_probe_position(const Position& base, Color chooser, int bucket) {
    Position probe = base;
    auto items = probe.items(chooser);
    ++items[static_cast<std::size_t>(bucket)];
    probe.set_items(chooser, items[0], items[1], items[2]);
    probe.set_side_to_move(Color::White);
    return probe;
}

Position initial_probe_position(bool choose_first, int bucket) {
    Position probe;
    const Color chooser = choose_first ? Color::White : Color::Black;
    auto items = probe.items(chooser);
    ++items[static_cast<std::size_t>(bucket)];
    probe.set_items(chooser, items[0], items[1], items[2]);
    probe.set_match_context(choose_first ? 1 : 0, choose_first ? 0 : 1, 38);
    probe.set_side_to_move(Color::White);
    return probe;
}

}  // namespace

Protocol::Protocol(std::istream& in, std::ostream& out) : engine_(64), in_(in), out_(out) {}

void Protocol::print_search(const SearchResult& result) {
    const auto milliseconds = std::max<std::int64_t>(1, result.elapsed.count());
    const auto nps = static_cast<std::uint64_t>(
        result.nodes * 1000ULL / static_cast<std::uint64_t>(milliseconds));
    const bool white_to_move = engine_.position().side_to_move() == Color::White;

    if (!result.lines.empty()) {
        for (std::size_t i = 0; i < result.lines.size(); ++i) {
            const auto& line = result.lines[i];
            const Value white_value = white_to_move ? line.value : -line.value;
            out_ << "info multipv " << (i + 1) << " depth " << result.depth << " seldepth "
                 << result.seldepth << " nodes " << result.nodes << " nps " << nps << " score "
                 << std::fixed << std::setprecision(2)
                 << static_cast<double>(white_value) / ScoreUnit;
            if (!line.pv.empty()) out_ << " pv " << format_pv(engine_.position(), line.pv);
            out_ << '\n';
        }
    } else {
        const Value white_value = white_to_move ? result.value : -result.value;
        out_ << "info depth " << result.depth << " seldepth " << result.seldepth << " nodes "
             << result.nodes << " nps " << nps << " score " << std::fixed
             << std::setprecision(2) << static_cast<double>(white_value) / ScoreUnit;
        if (!result.pv.empty()) out_ << " pv " << format_pv(engine_.position(), result.pv);
        out_ << '\n';
    }
    out_ << "bestmove " << (result.has_move ? format_move(result.best_move) : "(none)") << '\n';
}

void Protocol::bench() {
    engine_.new_game();
    SearchLimits limits;
    limits.depth = 4;
    const auto start = std::chrono::steady_clock::now();
    const auto result = engine_.go(limits);
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start);
    out_ << "bench depth 4 nodes " << result.nodes << " time " << elapsed.count()
         << " ms bestmove " << (result.has_move ? format_move(result.best_move) : "(none)")
         << '\n';
}

void Protocol::command(const std::string& line) {
    if (line == "rpsc") {
        out_ << "id name RPSC Engine 0.12\nid author Jungwoo Kim\nrpscok\n";
        return;
    }
    if (line == "isready") {
        out_ << "readyok\n";
        return;
    }
    if (line == "newgame" || line == "position startpos") {
        engine_.new_game();
        return;
    }
    if (line == "d") {
        out_ << engine_.position().debug_string();
        return;
    }
    if (line == "bench") {
        bench();
        return;
    }
    if (line == "quit") {
        running_ = false;
        return;
    }
    if (line.rfind("perft ", 0) == 0) {
        const int depth = std::stoi(line.substr(6));
        out_ << "perft " << depth << ' ' << engine_.perft(depth) << '\n';
        return;
    }
    if (line.rfind("divide ", 0) == 0) {
        const int depth = std::stoi(line.substr(7));
        divide(engine_.position(), depth, out_);
        return;
    }
    if (line.rfind("match ", 0) == 0) {
        std::istringstream stream(line.substr(6));
        int quiz_white = 0, quiz_black = 0, remaining_plies = -1;
        if (!(stream >> quiz_white >> quiz_black >> remaining_plies) || quiz_white < 0 ||
            quiz_black < 0 || remaining_plies < -1) {
            out_ << "error invalid match context\n";
            return;
        }
        engine_.position().set_match_context(quiz_white, quiz_black, remaining_plies);
        return;
    }
    if (line.rfind("items ", 0) == 0) {
        std::istringstream stream(line.substr(6));
        char side;
        int push = 0, rotation = 0, step = 0;
        Color color;
        if (!(stream >> side >> push >> rotation >> step) || !parse_side(side, color) || push < 0 ||
            rotation < 0 || step < 0) {
            out_ << "error invalid items\n";
            return;
        }
        engine_.position().set_items(color, push, rotation, step);
        return;
    }
    if (line.rfind("gain ", 0) == 0) {
        std::istringstream stream(line.substr(5));
        char side;
        std::string item;
        Color color;
        if (!(stream >> side >> item) || !parse_side(side, color) ||
            (item != "Pu" && item != "Ro" && item != "St")) {
            out_ << "error invalid gain\n";
            return;
        }
        auto counts = engine_.position().items(color);
        const int bucket = item == "Pu" ? 0 : item == "Ro" ? 1 : 2;
        ++counts[bucket];
        engine_.position().set_items(color, counts[0], counts[1], counts[2]);
        return;
    }
    if (line.rfind("move ", 0) == 0) {
        Move move;
        if (!parse_move(engine_.position(), line.substr(5), move)) {
            out_ << "error illegal move\n";
            return;
        }
        UndoState undo;
        engine_.position().do_move(move, undo);
        return;
    }
    if (line == "chooseorder" || line.rfind("chooseorder ", 0) == 0) {
        std::istringstream stream(line.size() > 11 ? line.substr(12) : std::string{});
        SearchLimits limits;
        limits.depth = 4;
        std::string token;
        while (stream >> token) {
            if (token == "depth")
                stream >> limits.depth;
            else if (token == "nodes")
                stream >> limits.nodes;
            else if (token == "movetime") {
                long long ms;
                stream >> ms;
                limits.movetime = std::chrono::milliseconds(ms);
                limits.depth = 84;
            }
        }
        const auto choice = engine_.choose_order(limits);
        out_ << "info order first score " << std::fixed << std::setprecision(2)
             << static_cast<double>(choice.white_value) / ScoreUnit << " depth "
             << choice.probe.depth << " nodes " << choice.probe.nodes << '\n';
        out_ << "info order second score " << std::fixed << std::setprecision(2)
             << static_cast<double>(-choice.white_value) / ScoreUnit << '\n';
        out_ << "bestorder " << (choice.choose_first ? "first" : "second") << '\n';
        return;
    }
    if (line == "chooseinitial" || line.rfind("chooseinitial ", 0) == 0) {
        std::istringstream stream(line.size() > 13 ? line.substr(14) : std::string{});
        SearchLimits limits;
        limits.depth = 8;
        std::string token;
        while (stream >> token) {
            if (token == "depth")
                stream >> limits.depth;
            else if (token == "nodes")
                stream >> limits.nodes;
            else if (token == "movetime") {
                long long ms;
                stream >> ms;
                limits.movetime = std::chrono::milliseconds(ms);
                limits.depth = 84;
            }
        }
        const auto choice = engine_.choose_initial(limits);
        for (std::size_t rank = 0; rank < choice.lines.size(); ++rank) {
            const auto& candidate = choice.lines[rank];
            out_ << "info initial " << (rank + 1) << ' '
                 << (candidate.choose_first ? "first" : "second") << ' '
                 << item_name(candidate.bucket) << " score " << std::fixed
                 << std::setprecision(2)
                 << static_cast<double>(candidate.chooser_value) / ScoreUnit << " depth "
                 << candidate.probe.depth << " nodes " << candidate.probe.nodes;
            if (!candidate.probe.pv.empty()) {
                const Position probe =
                    initial_probe_position(candidate.choose_first, candidate.bucket);
                out_ << " pv " << format_pv(probe, candidate.probe.pv);
            }
            out_ << '\n';
        }
        out_ << "bestinitial " << (choice.choose_first ? "first" : "second") << ' '
             << (choice.best_bucket >= 0 ? item_name(choice.best_bucket) : "(none)") << '\n';
        return;
    }
    if (line.rfind("chooseitem ", 0) == 0) {
        std::istringstream stream(line.substr(11));
        char side;
        Color chooser;
        if (!(stream >> side) || !parse_side(side, chooser)) {
            out_ << "error invalid chooseitem\n";
            return;
        }
        SearchLimits limits;
        limits.depth = 8;
        std::string token;
        while (stream >> token) {
            if (token == "depth")
                stream >> limits.depth;
            else if (token == "nodes")
                stream >> limits.nodes;
            else if (token == "movetime") {
                long long ms;
                stream >> ms;
                limits.movetime = std::chrono::milliseconds(ms);
            }
        }
        const auto choice = engine_.choose_item(chooser, limits);
        for (std::size_t rank = 0; rank < choice.lines.size(); ++rank) {
            const auto& candidate = choice.lines[rank];
            const Value chooser_value =
                chooser == Color::White ? candidate.white_value : -candidate.white_value;
            out_ << "info item " << (rank + 1) << ' ' << item_name(candidate.bucket) << " score "
                 << std::fixed << std::setprecision(2)
                 << static_cast<double>(chooser_value) / ScoreUnit;
            out_ << " depth " << candidate.probe.depth << " nodes " << candidate.probe.nodes;
            if (!candidate.probe.pv.empty()) {
                const Position probe = item_probe_position(engine_.position(), chooser, candidate.bucket);
                out_ << " pv " << format_pv(probe, candidate.probe.pv);
            }
            out_ << '\n';
        }
        out_ << "bestitem "
             << (choice.best_bucket >= 0 ? item_name(choice.best_bucket) : "(none)") << '\n';
        return;
    }
    if (line.rfind("go ", 0) == 0) {
        std::istringstream stream(line.substr(3));
        std::string token;
        SearchLimits limits;
        limits.depth = 8;
        while (stream >> token) {
            if (token == "depth")
                stream >> limits.depth;
            else if (token == "nodes")
                stream >> limits.nodes;
            else if (token == "movetime") {
                long long ms;
                stream >> ms;
                limits.movetime = std::chrono::milliseconds(ms);
                limits.depth = 84;
            } else if (token == "multipv")
                stream >> limits.multipv;
        }
        print_search(engine_.go(limits));
        return;
    }
    if (!line.empty()) out_ << "error unknown command\n";
}

void Protocol::loop() {
    std::string line;
    while (running_ && std::getline(in_, line)) command(line);
}

}  // namespace rpsc
