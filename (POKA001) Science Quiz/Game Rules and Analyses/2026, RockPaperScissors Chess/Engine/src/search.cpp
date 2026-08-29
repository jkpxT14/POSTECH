#include "search.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <vector>

namespace rpsc {
namespace {

constexpr int MaxPly = 64;
constexpr Value AspirationWindow = 50;

struct OrderedMove {
    Move move;
    int score = 0;
    int capture_swing = 0;
};

bool is_killer(const Move& move, const std::array<Move, 2>& killers, int index) {
    return index >= 0 && index < 2 && killers[index].path_length != 0 && move == killers[index];
}

bool is_excluded(const Move& move, const std::vector<Move>& excluded) {
    return std::any_of(excluded.begin(), excluded.end(),
                       [&](const Move& other) { return move == other; });
}

Value prior_root_value(const Move& move, const std::vector<RootLine>& prior) {
    for (const auto& line : prior) {
        if (line.move == move) return line.value;
    }
    return -Infinity;
}

}  // namespace

struct Search::Context {
    SearchLimits limits;
    std::chrono::steady_clock::time_point start;
    std::uint64_t nodes = 0;
    Depth seldepth = 0;
    bool stopped = false;
    std::array<std::array<int, 64>, PieceCount> history{};
    std::array<std::array<Move, 2>, MaxPly> killers{};

    bool should_stop() {
        if (stopped) return true;
        if (limits.nodes && nodes >= limits.nodes) return stopped = true;
        if (limits.movetime.count() > 0 && (nodes & 1023ULL) == 0) {
            if (std::chrono::steady_clock::now() - start >= limits.movetime)
                return stopped = true;
        }
        return false;
    }

    void record_quiet_cutoff(const Move& move, Depth depth, int ply) {
        int& history_value = history[piece_index(move.piece)][move.to()];
        history_value += depth * depth;
        if (history_value > 100000) history_value /= 2;

        if (ply < 0 || ply >= MaxPly) return;
        if (killers[ply][0] != move) {
            killers[ply][1] = killers[ply][0];
            killers[ply][0] = move;
        }
    }
};

Search::Search(TranspositionTable& tt) : tt_(tt) {}

Value Search::quiescence(Position& position, Value alpha, Value beta, int ply, Context& context) {
    ++context.nodes;
    context.seldepth = std::max(context.seldepth, ply);
    if (ply >= MaxPly || context.should_stop()) return evaluate(position);

    const Value stand_pat = evaluate(position);
    if (stand_pat >= beta) return beta;
    if (stand_pat > alpha) alpha = stand_pat;

    const auto moves = generate_search_moves_info(position);
    std::vector<OrderedMove> tactical;
    tactical.reserve(moves.size());
    for (const auto& entry : moves) {
        if (entry.capture_swing != 0)
            tactical.push_back({entry.move, entry.capture_swing * 100000, entry.capture_swing});
    }
    std::stable_sort(tactical.begin(), tactical.end(),
                     [](const OrderedMove& lhs, const OrderedMove& rhs) {
                         return lhs.score > rhs.score;
                     });

    for (const auto& current : tactical) {
        UndoState undo;
        position.do_move(current.move, undo);
        const Value score = -quiescence(position, -beta, -alpha, ply + 1, context);
        position.undo_move(undo);

        if (context.stopped) return alpha;
        if (score >= beta) return beta;
        if (score > alpha) alpha = score;
    }
    return alpha;
}

Value Search::negamax(Position& position, Depth depth, Value alpha, Value beta, int ply,
                      Context& context, bool pv_node) {
    if (ply >= MaxPly) return evaluate(position);
    if (depth <= 0) return quiescence(position, alpha, beta, ply, context);

    ++context.nodes;
    context.seldepth = std::max(context.seldepth, ply);
    if (context.should_stop()) return evaluate(position);

    const Value original_alpha = alpha;
    const Value original_beta = beta;
    const Key key = position.search_key();
    const TTEntry* entry = tt_.probe(key);
    Move tt_move{};
    const bool has_tt_move = entry && entry->has_move;
    if (has_tt_move) tt_move = entry->best_move;

    if (entry && entry->depth >= depth && !pv_node) {
        if (entry->bound == Bound::Exact) return entry->value;
        if (entry->bound == Bound::Lower && entry->value >= beta) return entry->value;
        if (entry->bound == Bound::Upper && entry->value <= alpha) return entry->value;
    }

    const auto moves = generate_search_moves_info(position);
    if (moves.empty()) return evaluate(position);

    std::vector<OrderedMove> ordered;
    ordered.reserve(moves.size());
    for (const auto& entry_move : moves) {
        const auto& move = entry_move.move;
        const int swing = entry_move.capture_swing;
        int score = context.history[piece_index(move.piece)][move.to()];
        if (move.item != Item::None) score -= 1000;
        if (has_tt_move && move == tt_move) score += 1000000;
        if (swing > 0)
            score += 500000 + swing * 10000;
        else if (swing < 0)
            score -= 250000;
        if (ply < MaxPly && is_killer(move, context.killers[ply], 0))
            score += 200000;
        else if (ply < MaxPly && is_killer(move, context.killers[ply], 1))
            score += 150000;
        ordered.push_back({move, score, swing});
    }
    std::stable_sort(ordered.begin(), ordered.end(),
                     [](const OrderedMove& lhs, const OrderedMove& rhs) {
                         return lhs.score > rhs.score;
                     });

    Move best{};
    bool has_best = false;
    int move_index = 0;

    for (const auto& current : ordered) {
        UndoState undo;
        position.do_move(current.move, undo);

        Value score;
        if (move_index == 0) {
            score = -negamax(position, depth - 1, -beta, -alpha, ply + 1, context, pv_node);
        } else {
            const int history_value =
                context.history[piece_index(current.move.piece)][current.move.to()];
            const bool reducible = !pv_node && current.capture_swing == 0 &&
                                   current.move.item == Item::None && depth >= 4 &&
                                   move_index >= 4;
            if (reducible) {
                int reduction = 1;
                if (depth >= 7 && move_index >= 10 && history_value <= 0) reduction = 2;
                const Depth reduced_depth = std::max<Depth>(0, depth - 1 - reduction);
                score = -negamax(position, reduced_depth, -alpha - 1, -alpha, ply + 1,
                                 context, false);
                if (score > alpha)
                    score = -negamax(position, depth - 1, -alpha - 1, -alpha, ply + 1,
                                     context, false);
            } else {
                score = -negamax(position, depth - 1, -alpha - 1, -alpha, ply + 1, context,
                                 false);
            }

            if (score > alpha && score < beta)
                score = -negamax(position, depth - 1, -beta, -alpha, ply + 1, context, pv_node);
        }

        position.undo_move(undo);
        ++move_index;

        if (context.stopped) return alpha;
        if (score > alpha) {
            alpha = score;
            best = current.move;
            has_best = true;
        }
        if (alpha >= beta) {
            if (current.capture_swing == 0)
                context.record_quiet_cutoff(current.move, depth, ply);
            break;
        }
    }

    Bound bound = Bound::Exact;
    if (alpha <= original_alpha)
        bound = Bound::Upper;
    else if (alpha >= original_beta)
        bound = Bound::Lower;
    tt_.store(key, depth, alpha, bound, has_best ? &best : nullptr);
    return alpha;
}

SearchResult Search::run(Position root, const SearchLimits& limits) {
    Context context;
    context.limits = limits;
    context.limits.multipv = std::clamp(context.limits.multipv, 1, 8);
    context.start = std::chrono::steady_clock::now();

    SearchResult result;
    const auto root_info = generate_search_moves_info(root);
    if (root_info.empty()) return result;

    std::vector<Move> root_moves;
    root_moves.reserve(root_info.size());
    for (const auto& entry : root_info) root_moves.push_back(entry.move);

    auto root_swing = [&](const Move& move) {
        for (const auto& entry : root_info) {
            if (entry.move == move) return entry.capture_swing;
        }
        return 0;
    };

    auto build_pv = [&](const Move& first, Depth depth) {
        std::vector<Move> pv;
        Position position = root;
        Move move = first;
        for (int ply = 0; ply < depth && move.path_length != 0; ++ply) {
            if (!position.is_legal_path(move)) break;
            pv.push_back(move);
            UndoState undo;
            position.do_move(move, undo);
            const TTEntry* entry = tt_.probe(position.search_key());
            move = entry && entry->has_move ? entry->best_move : Move{};
        }
        return pv;
    };

    struct RootPass {
        Move best{};
        Value value = -Infinity;
        std::vector<RootLine> scores;
    };

    std::vector<RootLine> prior_scores;
    Move previous_best = root_moves.front();
    Value previous_value = 0;

    auto search_root = [&](Depth depth, Value alpha, Value beta,
                           const std::vector<Move>& excluded, const Move& preferred) {
        std::vector<OrderedMove> ordered;
        ordered.reserve(root_moves.size());
        for (const auto& move : root_moves) {
            if (is_excluded(move, excluded)) continue;
            const int swing = root_swing(move);
            int score = context.history[piece_index(move.piece)][move.to()];
            if (move.item != Item::None) score -= 1000;
            if (move == preferred) score += 1200000;
            const Value old_value = prior_root_value(move, prior_scores);
            if (old_value > -Infinity) score += 1000 * old_value;
            if (swing > 0)
                score += 500000 + swing * 10000;
            else if (swing < 0)
                score -= 250000;
            ordered.push_back({move, score, swing});
        }
        std::stable_sort(ordered.begin(), ordered.end(),
                         [](const OrderedMove& lhs, const OrderedMove& rhs) {
                             return lhs.score > rhs.score;
                         });

        RootPass pass;
        if (ordered.empty()) return pass;
        pass.best = ordered.front().move;
        int move_index = 0;
        Value current_alpha = alpha;

        for (const auto& current : ordered) {
            UndoState undo;
            root.do_move(current.move, undo);

            Value score;
            if (move_index == 0) {
                score = -negamax(root, depth - 1, -beta, -current_alpha, 1, context, true);
            } else {
                score = -negamax(root, depth - 1, -current_alpha - 1, -current_alpha, 1,
                                 context, false);
                if (score > current_alpha && score < beta)
                    score = -negamax(root, depth - 1, -beta, -current_alpha, 1, context, true);
            }
            root.undo_move(undo);
            ++move_index;

            if (context.stopped) return pass;
            pass.scores.push_back({current.move, score, {}});
            if (score > pass.value) {
                pass.value = score;
                pass.best = current.move;
            }
            if (score > current_alpha) current_alpha = score;
            if (current_alpha >= beta) break;
        }
        return pass;
    };

    for (Depth depth = 1; depth <= std::max(1, limits.depth); ++depth) {
        Value alpha = -Infinity;
        Value beta = Infinity;
        Value window = AspirationWindow;
        if (depth >= 4 && result.has_move) {
            alpha = std::max(-Infinity, previous_value - window);
            beta = std::min(Infinity, previous_value + window);
        }

        RootPass pass;
        while (true) {
            pass = search_root(depth, alpha, beta, {}, previous_best);
            if (context.stopped) break;
            if (pass.value <= alpha && alpha > -Infinity) {
                window *= 2;
                alpha = std::max(-Infinity, previous_value - window);
                beta = std::min(Infinity, previous_value + window);
                continue;
            }
            if (pass.value >= beta && beta < Infinity) {
                window *= 2;
                alpha = std::max(-Infinity, previous_value - window);
                beta = std::min(Infinity, previous_value + window);
                continue;
            }
            break;
        }

        if (context.stopped) break;
        previous_best = pass.best;
        previous_value = pass.value;
        prior_scores = pass.scores;
        std::stable_sort(prior_scores.begin(), prior_scores.end(),
                         [](const RootLine& lhs, const RootLine& rhs) {
                             return lhs.value > rhs.value;
                         });

        result.has_move = true;
        result.best_move = previous_best;
        result.value = previous_value;
        result.depth = depth;
        result.pv = build_pv(previous_best, depth);
    }

    if (result.has_move) {
        result.lines.push_back({result.best_move, result.value, result.pv});
        std::vector<Move> excluded{result.best_move};

        for (int rank = 1; rank < context.limits.multipv && !context.stopped; ++rank) {
            Move preferred{};
            for (const auto& line : prior_scores) {
                if (!is_excluded(line.move, excluded)) {
                    preferred = line.move;
                    break;
                }
            }
            if (preferred.path_length == 0) break;

            const RootPass pass =
                search_root(result.depth, -Infinity, Infinity, excluded, preferred);
            if (context.stopped || pass.value <= -Infinity || pass.best.path_length == 0) break;
            auto pv = build_pv(pass.best, result.depth);
            result.lines.push_back({pass.best, pass.value, std::move(pv)});
            excluded.push_back(pass.best);
        }
    }

    result.nodes = context.nodes;
    result.seldepth = context.seldepth;
    result.elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - context.start);
    return result;
}

}  // namespace rpsc
