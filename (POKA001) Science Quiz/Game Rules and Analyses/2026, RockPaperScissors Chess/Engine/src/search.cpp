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

int capture_swing(Position& position, const Move& move) {
    const Color mover = position.side_to_move();
    const Color opponent = opposite(mover);
    const int own_before = position.captures(mover);
    const int opp_before = position.captures(opponent);

    UndoState undo;
    position.do_move(move, undo);
    const int swing = (position.captures(mover) - own_before) -
                      (position.captures(opponent) - opp_before);
    position.undo_move(undo);
    return swing;
}

bool is_killer(const Move& move, const std::array<Move, 2>& killers, int index) {
    return index >= 0 && index < 2 && killers[index].path_length != 0 && move == killers[index];
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
        int& h = history[piece_index(move.piece)][move.to()];
        h += depth * depth;
        if (h > 100000) h /= 2;

        if (ply < 0 || ply >= MaxPly) return;
        if (killers[ply][0] != move) {
            killers[ply][1] = killers[ply][0];
            killers[ply][0] = move;
        }
    }
};

Search::Search(TranspositionTable& tt) : tt_(tt) {}

Value Search::quiescence(Position& position, Value alpha, Value beta, int ply, Context& ctx) {
    ++ctx.nodes;
    ctx.seldepth = std::max(ctx.seldepth, ply);
    if (ply >= MaxPly || ctx.should_stop()) return evaluate(position);

    const Value stand_pat = evaluate(position);
    if (stand_pat >= beta) return beta;
    if (stand_pat > alpha) alpha = stand_pat;

    auto moves = generate_search_moves(position);
    std::vector<OrderedMove> tactical;
    tactical.reserve(moves.size());
    for (const auto& move : moves) {
        const int swing = capture_swing(position, move);
        if (swing != 0) tactical.push_back({move, swing * 100000, swing});
    }
    std::stable_sort(tactical.begin(), tactical.end(),
                     [](const OrderedMove& a, const OrderedMove& b) { return a.score > b.score; });

    for (const auto& ordered : tactical) {
        UndoState undo;
        position.do_move(ordered.move, undo);
        const Value score = -quiescence(position, -beta, -alpha, ply + 1, ctx);
        position.undo_move(undo);

        if (ctx.stopped) return alpha;
        if (score >= beta) return beta;
        if (score > alpha) alpha = score;
    }
    return alpha;
}

Value Search::negamax(Position& position, Depth depth, Value alpha, Value beta, int ply,
                      Context& ctx, bool pv_node) {
    if (ply >= MaxPly) return evaluate(position);
    if (depth <= 0) return quiescence(position, alpha, beta, ply, ctx);

    ++ctx.nodes;
    ctx.seldepth = std::max(ctx.seldepth, ply);
    if (ctx.should_stop()) return evaluate(position);

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

    auto moves = generate_search_moves(position);
    if (moves.empty()) return evaluate(position);

    std::vector<OrderedMove> ordered;
    ordered.reserve(moves.size());
    for (const auto& move : moves) {
        const int swing = capture_swing(position, move);
        int score = ctx.history[piece_index(move.piece)][move.to()];
        if (has_tt_move && move == tt_move) score += 1000000;
        if (swing > 0) score += 500000 + swing * 10000;
        else if (swing < 0) score -= 250000;
        if (ply < MaxPly && is_killer(move, ctx.killers[ply], 0)) score += 200000;
        else if (ply < MaxPly && is_killer(move, ctx.killers[ply], 1)) score += 150000;
        ordered.push_back({move, score, swing});
    }
    std::stable_sort(ordered.begin(), ordered.end(),
                     [](const OrderedMove& a, const OrderedMove& b) { return a.score > b.score; });

    Move best{};
    bool has_best = false;
    int move_index = 0;

    for (const auto& current : ordered) {
        UndoState undo;
        position.do_move(current.move, undo);

        Value score;
        if (move_index == 0) {
            score = -negamax(position, depth - 1, -beta, -alpha, ply + 1, ctx, pv_node);
        } else {
            const bool reducible = !pv_node && current.capture_swing == 0 && depth >= 4 &&
                                   move_index >= 4;
            if (reducible) {
                score = -negamax(position, depth - 2, -alpha - 1, -alpha, ply + 1, ctx, false);
                if (score > alpha)
                    score = -negamax(position, depth - 1, -alpha - 1, -alpha, ply + 1, ctx, false);
            } else {
                score = -negamax(position, depth - 1, -alpha - 1, -alpha, ply + 1, ctx, false);
            }

            if (score > alpha && score < beta)
                score = -negamax(position, depth - 1, -beta, -alpha, ply + 1, ctx, pv_node);
        }

        position.undo_move(undo);
        ++move_index;

        if (ctx.stopped) return alpha;
        if (score > alpha) {
            alpha = score;
            best = current.move;
            has_best = true;
        }
        if (alpha >= beta) {
            if (current.capture_swing == 0) ctx.record_quiet_cutoff(current.move, depth, ply);
            break;
        }
    }

    Bound bound = Bound::Exact;
    if (alpha <= original_alpha) bound = Bound::Upper;
    else if (alpha >= original_beta) bound = Bound::Lower;
    tt_.store(key, depth, alpha, bound, has_best ? &best : nullptr);
    return alpha;
}

SearchResult Search::run(Position root, const SearchLimits& limits) {
    Context ctx;
    ctx.limits = limits;
    ctx.start = std::chrono::steady_clock::now();

    SearchResult result;
    auto root_moves = generate_search_moves(root);
    if (root_moves.empty()) return result;

    Move previous_best = root_moves.front();
    Value previous_value = 0;

    auto search_root = [&](Depth depth, Value alpha, Value beta, Move& best, Value& best_value) {
        std::vector<OrderedMove> ordered;
        ordered.reserve(root_moves.size());
        for (const auto& move : root_moves) {
            const int swing = capture_swing(root, move);
            int score = ctx.history[piece_index(move.piece)][move.to()];
            if (move == previous_best) score += 1000000;
            if (swing > 0) score += 500000 + swing * 10000;
            else if (swing < 0) score -= 250000;
            ordered.push_back({move, score, swing});
        }
        std::stable_sort(ordered.begin(), ordered.end(),
                         [](const OrderedMove& a, const OrderedMove& b) { return a.score > b.score; });

        best = previous_best;
        best_value = -Infinity;
        int move_index = 0;
        for (const auto& current : ordered) {
            UndoState undo;
            root.do_move(current.move, undo);

            Value score;
            if (move_index == 0) {
                score = -negamax(root, depth - 1, -beta, -alpha, 1, ctx, true);
            } else {
                score = -negamax(root, depth - 1, -alpha - 1, -alpha, 1, ctx, false);
                if (score > alpha && score < beta)
                    score = -negamax(root, depth - 1, -beta, -alpha, 1, ctx, true);
            }
            root.undo_move(undo);
            ++move_index;

            if (ctx.stopped) return;
            if (score > best_value) {
                best_value = score;
                best = current.move;
            }
            if (score > alpha) alpha = score;
            if (alpha >= beta) break;
        }
    };

    for (Depth depth = 1; depth <= std::max(1, limits.depth); ++depth) {
        Value alpha = -Infinity;
        Value beta = Infinity;
        Value window = AspirationWindow;
        if (depth >= 4 && result.has_move) {
            alpha = std::max(-Infinity, previous_value - window);
            beta = std::min(Infinity, previous_value + window);
        }

        Move best = previous_best;
        Value best_value = previous_value;

        while (true) {
            search_root(depth, alpha, beta, best, best_value);
            if (ctx.stopped) break;
            if (best_value <= alpha && alpha > -Infinity) {
                window *= 2;
                alpha = std::max(-Infinity, previous_value - window);
                beta = std::min(Infinity, previous_value + window);
                continue;
            }
            if (best_value >= beta && beta < Infinity) {
                window *= 2;
                alpha = std::max(-Infinity, previous_value - window);
                beta = std::min(Infinity, previous_value + window);
                continue;
            }
            break;
        }

        if (ctx.stopped) break;
        previous_best = best;
        previous_value = best_value;
        result.has_move = true;
        result.best_move = best;
        result.value = best_value;
        result.depth = depth;

        result.pv.clear();
        Position position = root;
        for (int i = 0; i < depth; ++i) {
            const TTEntry* pv_entry = tt_.probe(position.search_key());
            Move move = i == 0 ? best : (pv_entry && pv_entry->has_move ? pv_entry->best_move : Move{});
            if (i != 0 && (!pv_entry || !pv_entry->has_move)) break;
            if (!position.is_legal_path(move)) break;
            result.pv.push_back(move);
            UndoState undo;
            position.do_move(move, undo);
        }
    }

    result.nodes = ctx.nodes;
    result.seldepth = ctx.seldepth;
    result.elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - ctx.start);
    return result;
}

}  // namespace rpsc
