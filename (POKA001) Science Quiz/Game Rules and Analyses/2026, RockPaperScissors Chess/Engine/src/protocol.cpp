#include "protocol.h"
#include "engine.h"
#include "evaluate.h"
#include "movegen.h"
#include "move.h"
#include "notation.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace rpsc {
namespace {

const char* bucket_name(int bucket) {
  return bucket == 0 ? "Pu" : bucket == 1 ? "Ro" : "St";
}

bool parse_side(const std::string& text, Side& side) {
  if (text == "W") { side = Side::White; return true; }
  if (text == "B") { side = Side::Black; return true; }
  return false;
}

bool valid_school(const std::string& text) {
  return text == "POSTECH" || text == "KAIST";
}

void parse_limits(std::istringstream& in, SearchLimits& limits) {
  std::string key;
  while (in >> key) {
    if (key == "depth") in >> limits.depth;
    else if (key == "movetime") in >> limits.movetime_ms;
    else if (key == "multipv") in >> limits.multipv;
  }
  limits.depth = std::clamp(limits.depth, 1, 84);
  limits.movetime_ms = std::max(20, limits.movetime_ms);
  limits.multipv = std::clamp(limits.multipv, 1, 3);
}

int white_value(const Position& root, int root_value) {
  return root.side == Side::White ? root_value : -root_value;
}

void print_search(const Position& position, const SearchResult& result) {
  if (!result.has_best) {
    std::cout << "bestmove (none)\n" << std::flush;
    return;
  }
  for (std::size_t i = 0; i < result.candidates.size(); ++i) {
    const auto& c = result.candidates[i];
    std::cout << "info multipv " << (i + 1)
              << " depth " << c.depth
              << " seldepth " << result.seldepth
              << " nodes " << result.nodes
              << " time " << result.elapsed_ms
              << " score cp " << white_value(position, c.value)
              << " pv " << move_notation(position, c.move);
    Position p = position;
    if (p.apply(c.move)) {
      for (std::size_t j = 1; j < c.pv.size(); ++j) {
        std::cout << ' ' << move_notation(p, c.pv[j]);
        if (!p.apply(c.pv[j])) break;
      }
    }
    std::cout << '\n';
  }
  std::cout << "bestmove " << move_notation(position, result.best) << '\n' << std::flush;
}

void print_position(const Position& p, const std::string& white_school,
                    const std::string& black_school) {
  std::cout << "teams W " << white_school << " B " << black_school << '\n';
  std::cout << "side " << side_name(p.side) << '\n';
  std::cout << "quiz " << p.quiz[0] << '-' << p.quiz[1] << '\n';
  std::cout << "captures " << p.captures[0] << '-' << p.captures[1] << '\n';
  std::cout << "remaining " << p.remaining_plies << '\n';
  std::cout << "items W " << p.items[0].push << ' ' << p.items[0].rotation << ' ' << p.items[0].step << '\n';
  std::cout << "items B " << p.items[1].push << ' ' << p.items[1].rotation << ' ' << p.items[1].step << '\n';
  const auto& ot = OrientationTable::instance();
  for (int i = 0; i < 8; ++i) {
    const auto& pc = p.pieces[i];
    std::cout << (i < 4 ? 'W' : 'B') << (i % 4 + 1) << ' ';
    if (!pc.alive) std::cout << "x\n";
    else std::cout << square_name(pc.x, pc.y) << ' ' << gesture_name(ot.top(pc.orientation)) << '\n';
  }
  std::cout << std::flush;
}

bool apply_notation(Position& p, const std::string& text) {
  auto moves = generate_moves(p, false);
  for (const auto& sm : moves) {
    if (move_notation(p, sm.move) == text || move_signature(sm.move) == text)
      return p.apply(sm.move);
  }
  return false;
}

} // namespace

int run_protocol() {
  Engine engine;
  std::string white_school = "POSTECH";
  std::string black_school = "KAIST";
  std::string line;

  while (std::getline(std::cin, line)) {
    if (line.empty()) continue;
    std::istringstream in(line);
    std::string command;
    in >> command;

    if (command == "quit" || command == "exit") break;

    if (command == "rpsc" || command == "uci") {
      std::cout << "id name RPSC Engine " << Engine::VERSION
                << "\nid author Jungwoo Kim / POKA001\nrpscok\n" << std::flush;
    } else if (command == "isready") {
      std::cout << "readyok\n" << std::flush;
    } else if (command == "newgame" || line == "position startpos") {
      engine.new_game();
      white_school = "POSTECH";
      black_school = "KAIST";
      std::cout << "ok\n" << std::flush;
    } else if (command == "clear") {
      engine.clear_search();
      std::cout << "ok\n" << std::flush;
    } else if (command == "teams") {
      std::string wtag, wschool, btag, bschool;
      if (!(in >> wtag >> wschool >> btag >> bschool) || wtag != "W" || btag != "B" ||
          !valid_school(wschool) || !valid_school(bschool) || wschool == bschool) {
        std::cout << "error invalid teams; use: teams W POSTECH|KAIST B KAIST|POSTECH\n" << std::flush;
      } else {
        white_school = wschool;
        black_school = bschool;
        std::cout << "ok\n" << std::flush;
      }
    } else if (command == "items") {
      std::string side_text;
      int pu, ro, st;
      Side side;
      if (!(in >> side_text >> pu >> ro >> st) || !parse_side(side_text, side) ||
          pu < 0 || ro < 0 || st < 0) {
        std::cout << "error invalid items\n" << std::flush;
      } else {
        engine.position().items[side_index(side)] = Inventory{pu, ro, st};
        std::cout << "ok\n" << std::flush;
      }
    } else if (command == "gain") {
      std::string side_text, item;
      Side side;
      if (!(in >> side_text >> item) || !parse_side(side_text, side) ||
          (item != "Pu" && item != "Ro" && item != "St")) {
        std::cout << "error invalid gain\n" << std::flush;
      } else {
        auto& inv = engine.position().items[side_index(side)];
        if (item == "Pu") ++inv.push;
        else if (item == "Ro") ++inv.rotation;
        else ++inv.step;
        std::cout << "ok\n" << std::flush;
      }
    } else if (command == "side") {
      std::string side_text;
      Side side;
      if (!(in >> side_text) || !parse_side(side_text, side))
        std::cout << "error invalid side\n" << std::flush;
      else {
        engine.position().side = side;
        std::cout << "ok\n" << std::flush;
      }
    } else if (command == "matchpk") {
      int postech_quiz, kaist_quiz, remaining;
      if (!(in >> postech_quiz >> kaist_quiz >> remaining) || postech_quiz < 0 ||
          kaist_quiz < 0 || remaining < 0) {
        std::cout << "error invalid matchpk context\n" << std::flush;
      } else {
        engine.position().quiz[0] = white_school == "POSTECH" ? postech_quiz : kaist_quiz;
        engine.position().quiz[1] = black_school == "POSTECH" ? postech_quiz : kaist_quiz;
        engine.position().remaining_plies = remaining;
        std::cout << "ok\n" << std::flush;
      }
    } else if (command == "match") {
      int white_quiz, black_quiz, remaining;
      if (!(in >> white_quiz >> black_quiz >> remaining) || white_quiz < 0 ||
          black_quiz < 0 || remaining < 0) {
        std::cout << "error invalid match context\n" << std::flush;
      } else {
        engine.position().quiz = {white_quiz, black_quiz};
        engine.position().remaining_plies = remaining;
        std::cout << "ok\n" << std::flush;
      }
    } else if (command == "show" || command == "d") {
      print_position(engine.position(), white_school, black_school);
    } else if (command == "eval") {
      std::cout << "eval " << evaluate_white(engine.position()) << '\n' << std::flush;
    } else if (command == "legal" || command == "moves") {
      std::cout << generate_moves(engine.position(), false).size() << '\n' << std::flush;
    } else if (command == "perft") {
      int depth = 0;
      in >> depth;
      if (depth < 0) std::cout << "error invalid depth\n" << std::flush;
      else std::cout << "perft " << depth << ' ' << engine.perft(depth) << '\n' << std::flush;
    } else if (command == "divide") {
      int depth = 0;
      in >> depth;
      if (depth < 1) {
        std::cout << "error invalid depth\n" << std::flush;
      } else {
        std::uint64_t total = 0;
        for (const auto& sm : generate_moves(engine.position(), false)) {
          Position child = engine.position();
          child.apply(sm.move);
          const auto n = rpsc::perft(child, depth - 1);
          total += n;
          std::cout << move_notation(engine.position(), sm.move) << ' ' << n << '\n';
        }
        std::cout << "total " << total << '\n' << std::flush;
      }
    } else if (command == "move") {
      std::string rest;
      std::getline(in, rest);
      if (!rest.empty() && rest.front() == ' ') rest.erase(rest.begin());
      if (!apply_notation(engine.position(), rest)) std::cout << "error illegal move\n" << std::flush;
      else std::cout << "ok\n" << std::flush;
    } else if (command == "go") {
      SearchLimits limits;
      limits.movetime_ms = 1000;
      limits.multipv = 3;
      parse_limits(in, limits);
      print_search(engine.position(), engine.go(limits));
    } else if (command == "chooseorder") {
      SearchLimits limits;
      limits.movetime_ms = 1000;
      limits.multipv = 1;
      parse_limits(in, limits);
      const auto c = engine.choose_order(limits);
      std::cout << "info order first score cp " << c.white_value << '\n';
      std::cout << "info order second score cp " << -c.white_value << '\n';
      std::cout << "bestorder " << (c.choose_first ? "first" : "second") << '\n' << std::flush;
    } else if (command == "chooseitem") {
      std::string side_text;
      Side chooser;
      if (!(in >> side_text) || !parse_side(side_text, chooser)) {
        std::cout << "error invalid chooseitem\n" << std::flush;
        continue;
      }
      SearchLimits limits;
      limits.movetime_ms = 1000;
      limits.multipv = 1;
      parse_limits(in, limits);
      const auto c = engine.choose_item(chooser, limits);
      for (std::size_t i = 0; i < c.lines.size(); ++i) {
        const auto& x = c.lines[i];
        const int chooser_value = chooser == Side::White ? x.white_value : -x.white_value;
        std::cout << "info item " << (i + 1) << ' ' << bucket_name(x.bucket)
                  << " score cp " << chooser_value << '\n';
      }
      std::cout << "bestitem " << (c.best_bucket >= 0 ? bucket_name(c.best_bucket) : "(none)")
                << '\n' << std::flush;
    } else if (command == "chooseinitial") {
      SearchLimits limits;
      limits.movetime_ms = 1000;
      limits.multipv = 1;
      parse_limits(in, limits);
      const auto c = engine.choose_initial(limits);
      for (std::size_t i = 0; i < c.lines.size(); ++i) {
        const auto& x = c.lines[i];
        std::cout << "info initial " << (i + 1) << ' '
                  << (x.choose_first ? "first" : "second") << ' '
                  << bucket_name(x.bucket) << " score cp " << x.chooser_value << '\n';
      }
      std::cout << "bestinitial " << (c.choose_first ? "first" : "second") << ' '
                << (c.best_bucket >= 0 ? bucket_name(c.best_bucket) : "(none)")
                << '\n' << std::flush;
    } else if (command == "bench") {
      engine.new_game();
      SearchLimits limits;
      limits.movetime_ms = 250;
      limits.multipv = 3;
      const auto result = engine.go(limits);
      std::cout << "bench depth " << result.depth << " nodes " << result.nodes
                << " time " << result.elapsed_ms << " ms bestmove "
                << (result.has_best ? move_notation(engine.position(), result.best) : "(none)")
                << '\n' << std::flush;
    } else if (command == "help") {
      std::cout
          << "newgame | teams W POSTECH|KAIST B KAIST|POSTECH | "
          << "matchpk POSTECHQuiz KAISTQuiz remainingPlies | match Wquiz Bquiz remainingPlies | "
          << "items W/B pu ro st | gain W/B Pu/Ro/St | side W/B | move NOTATION | eval | "
          << "go [depth N] [movetime MS] [multipv 1..3] | chooseorder | chooseitem W/B | "
          << "chooseinitial | legal | moves | perft N | divide N | bench | show | clear | quit\n"
          << std::flush;
    } else {
      std::cout << "error unknown command\n" << std::flush;
    }
  }
  return 0;
}

} // namespace rpsc
