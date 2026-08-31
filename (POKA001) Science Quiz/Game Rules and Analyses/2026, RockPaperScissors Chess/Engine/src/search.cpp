#include "search.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <unordered_map>
#include <vector>

namespace rpsc {
namespace {

constexpr int MaxPly = 64;
constexpr int MaxSelectiveExtensions = 2;
constexpr Value AspirationWindow = 50;
constexpr int CountermoveBonus = 120000;
constexpr int ContinuationScale = 2;
constexpr int FollowupScale = 4;
constexpr int CaptureHistoryScale = 4;
constexpr std::size_t ItemActionCount = 6;
constexpr std::size_t HistoryMoveSlots = PieceCount * 64u * ItemActionCount;

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
    for (const auto& line : prior)
        if (line.move == move) return line.value;
    return -Infinity;
}

std::size_t move_slot(const Move& move) {
    const std::size_t base = static_cast<std::size_t>(piece_index(move.piece)) * 64u +
                             static_cast<unsigned>(move.to());
    return base * ItemActionCount + static_cast<unsigned>(move.item);
}

std::uint64_t continuation_key(const Move& previous, const Move& current) {
    return static_cast<std::uint64_t>(move_slot(previous)) * HistoryMoveSlots +
           move_slot(current);
}

void bounded_add(int& value, int delta) {
    value += delta;
    if (value > 100000 || value < -100000) value /= 2;
}

}  // namespace

struct Search::Context {
    SearchLimits limits;
    std::chrono::steady_clock::time_point start;
    std::uint64_t nodes = 0;
    Depth seldepth = 0;
    bool stopped = false;
    std::vector<int> history = std::vector<int>(HistoryMoveSlots, 0);
    std::vector<int> capture_history = std::vector<int>(HistoryMoveSlots, 0);
    std::array<std::array<Move, 2>, MaxPly> killers{};
    std::unordered_map<std::size_t, Move> countermoves;
    std::unordered_map<std::uint64_t, int> continuation;
    std::unordered_map<std::uint64_t, int> followup;
    std::unordered_map<Key, std::uint8_t> pressure_cache;

    bool should_stop() {
        if (stopped) return true;
        if (limits.nodes && nodes >= limits.nodes) return stopped = true;
        if (limits.movetime.count() > 0 && (nodes & 255ULL) == 0) {
            if (std::chrono::steady_clock::now() - start >= limits.movetime)
                return stopped = true;
        }
        return false;
    }

    int sparse_score(const std::unordered_map<std::uint64_t, int>& table,
                     std::uint64_t key) const {
        const auto found = table.find(key);
        return found == table.end() ? 0 : found->second;
    }

    void bounded_sparse_add(std::unordered_map<std::uint64_t, int>& table,
                            std::uint64_t key, int delta) {
        int& value = table[key];
        bounded_add(value, delta);
        if (table.size() > 250000) table.clear();
    }

    int history_score(const Move& move) const { return history[move_slot(move)]; }

    int continuation_score(const Move* previous, const Move* previous2, const Move& move) const {
        int score = 0;
        if (previous && previous->path_length != 0)
            score += sparse_score(continuation, continuation_key(*previous, move)) /
                     ContinuationScale;
        if (previous2 && previous2->path_length != 0)
            score += sparse_score(followup, continuation_key(*previous2, move)) /
                     FollowupScale;
        return score;
    }

    bool is_countermove(const Move* previous, const Move& move) const {
        if (!previous || previous->path_length == 0) return false;
        const auto found = countermoves.find(move_slot(*previous));
        return found != countermoves.end() && found->second == move;
    }

    int capture_history_score(const Move& move) const {
        return capture_history[move_slot(move)] / CaptureHistoryScale;
    }

    void record_quiet_cutoff(const Move& move, Depth depth, int ply, const Move* previous,
                             const Move* previous2, const std::vector<Move>& quiets_searched) {
        const int bonus = std::max(1, depth * depth);
        bounded_add(history[move_slot(move)], bonus);
        if (previous && previous->path_length != 0) {
            bounded_sparse_add(continuation, continuation_key(*previous, move), 2 * bonus);
            countermoves[move_slot(*previous)] = move;
            if (countermoves.size() > 16384) countermoves.clear();
        }
        if (previous2 && previous2->path_length != 0)
            bounded_sparse_add(followup, continuation_key(*previous2, move), bonus);

        const int penalty = std::max(1, bonus / 2);
        for (const Move& tried : quiets_searched) {
            if (tried == move) continue;
            bounded_add(history[move_slot(tried)], -penalty);
            if (previous && previous->path_length != 0)
                bounded_sparse_add(continuation, continuation_key(*previous, tried), -penalty);
            if (previous2 && previous2->path_length != 0)
                bounded_sparse_add(followup, continuation_key(*previous2, tried),
                                   -std::max(1, penalty / 2));
        }

        if (ply < 0 || ply >= MaxPly) return;
        if (killers[ply][0] != move) {
            killers[ply][1] = killers[ply][0];
            killers[ply][0] = move;
        }
    }

    void record_tactical_cutoff(const Move& move, Depth depth) {
        const int bonus = std::max(1, depth * depth * 2);
        bounded_add(capture_history[move_slot(move)], bonus);
    }

    int capture_pressure(Position& position) {
        const Key key = position.search_key();
        const auto found = pressure_cache.find(key);
        if (found != pressure_cache.end()) return found->second;
        int pressure = 0;
        for (const auto& move : generate_tactical_moves_info(position)) {
            if (move.capture_swing > 0 && ++pressure >= 4) break;
        }
        if (pressure_cache.size() > 50000) pressure_cache.clear();
        pressure_cache.emplace(key, static_cast<std::uint8_t>(pressure));
        return pressure;
    }

    int capture_pressure_for(const Position& position, Color side) {
        if (position.side_to_move() == side) {
            Position copy = position;
            return capture_pressure(copy);
        }
        Position copy = position;
        copy.set_side_to_move(side);
        return capture_pressure(copy);
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

    const auto moves = generate_tactical_moves_info(position);
    std::vector<OrderedMove> tactical;
    tactical.reserve(moves.size());
    for (const auto& entry : moves) {
        const int score = entry.capture_swing * 100000 + context.capture_history_score(entry.move);
        tactical.push_back({entry.move, score, entry.capture_swing});
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
        if (score >= beta) {
            context.record_tactical_cutoff(current.move, 1);
            return beta;
        }
        if (score > alpha) alpha = score;
    }
    return alpha;
}

Value Search::negamax(Position& position, Depth depth, Value alpha, Value beta, int ply,
                      Context& context, bool pv_node, const Move* previous_move,
                      const Move* previous2_move, int extensions_used) {
    if (ply >= MaxPly) return evaluate(position);
    if (depth <= 0) return quiescence(position, alpha, beta, ply, context);

    ++context.nodes;
    context.seldepth = std::max(context.seldepth, ply);
    if (context.should_stop()) return evaluate(position);

    const Value original_alpha = alpha;
    const Value original_beta = beta;
    const Value static_eval = evaluate(position);
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

    int own_pressure_before = 0;
    for (const auto& candidate : moves)
        if (candidate.capture_swing > 0 && ++own_pressure_before >= 4) break;
    const Color mover = position.side_to_move();
    const bool inspect_quiet_threats = depth >= 4;
    const int opponent_pressure_before =
        inspect_quiet_threats ? context.capture_pressure_for(position, opposite(mover)) : 0;

    std::vector<OrderedMove> ordered;
    ordered.reserve(moves.size());
    for (const auto& entry_move : moves) {
        const auto& move = entry_move.move;
        const int swing = entry_move.capture_swing;
        int score = context.history_score(move);
        score += context.continuation_score(previous_move, previous2_move, move);
        if (context.is_countermove(previous_move, move)) score += CountermoveBonus;
                if (has_tt_move && move == tt_move) score += 1000000;
        if (swing > 0)
            score += 500000 + swing * 10000 + context.capture_history_score(move);
        else if (swing < 0)
            score += -250000 + context.capture_history_score(move);
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
    std::vector<Move> quiets_searched;
    quiets_searched.reserve(16);

    for (const auto& current : ordered) {
        UndoState undo;
        position.do_move(current.move, undo);

        // A combat result changes the official score and is the RPSC analogue of a forcing
        // tactical event. Preserve one ply around at most two such events per line. This is
        // rule-derived and position-independent; it does not encode a square, Gesture, opening,
        // or known game sequence.
        const bool extend_score = current.capture_swing != 0 &&
                                  extensions_used < MaxSelectiveExtensions;
        const int next_extensions = extensions_used + (extend_score ? 1 : 0);
        const Depth full_child_depth = depth - 1 + (extend_score ? 1 : 0);

        Value score;
        if (move_index == 0) {
            score = -negamax(position, full_child_depth, -beta, -alpha, ply + 1, context,
                             pv_node, &current.move, previous_move, next_extensions);
        } else {
            const int history_value =
                context.history_score(current.move) +
                context.continuation_score(previous_move, previous2_move, current.move);
            const bool known_reply = context.is_countermove(previous_move, current.move) ||
                                     (ply < MaxPly &&
                                      (is_killer(current.move, context.killers[ply], 0) ||
                                       is_killer(current.move, context.killers[ply], 1))) ||
                                     history_value > 4 * depth * depth;
            bool reducible = !pv_node && current.capture_swing == 0 && depth >= 4 &&
                             move_index >= 4 && !known_reply;

            int reduction = 1;
            if (reducible) {
                const int opponent_pressure_after = context.capture_pressure(position);
                const int own_pressure_after = context.capture_pressure_for(position, mover);
                const bool defensive = opponent_pressure_before > 0 &&
                                       opponent_pressure_after < opponent_pressure_before;
                const bool creates_threat = own_pressure_after > own_pressure_before;
                if (defensive || creates_threat) reducible = false;

                if (reducible) {
                    const Value child_static_for_mover = -evaluate(position);
                    const bool improving = child_static_for_mover >= static_eval + 8;
                    if (!improving && depth >= 7 && move_index >= 10 && history_value <= 0)
                        reduction = 2;
                }
            }

            if (reducible) {
                const Depth reduced_depth = std::max<Depth>(0, full_child_depth - reduction);
                score = -negamax(position, reduced_depth, -alpha - 1, -alpha, ply + 1, context,
                                 false, &current.move, previous_move, next_extensions);
                if (score > alpha)
                    score = -negamax(position, full_child_depth, -alpha - 1, -alpha, ply + 1,
                                     context, false, &current.move, previous_move,
                                     next_extensions);
            } else {
                score = -negamax(position, full_child_depth, -alpha - 1, -alpha, ply + 1,
                                 context, false, &current.move, previous_move, next_extensions);
            }

            if (score > alpha && score < beta)
                score = -negamax(position, full_child_depth, -beta, -alpha, ply + 1, context,
                                 pv_node, &current.move, previous_move, next_extensions);
        }

        position.undo_move(undo);
        ++move_index;

        if (context.stopped) return alpha;
        if (current.capture_swing == 0) quiets_searched.push_back(current.move);
        if (score > alpha) {
            alpha = score;
            best = current.move;
            has_best = true;
        }
        if (alpha >= beta) {
            if (current.capture_swing == 0)
                context.record_quiet_cutoff(current.move, depth, ply, previous_move,
                                            previous2_move, quiets_searched);
            else
                context.record_tactical_cutoff(current.move, depth);
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
    tt_.new_search();
    Context context;
    context.limits = limits;
    context.limits.multipv = std::clamp(context.limits.multipv, 1, 8);
    context.start = std::chrono::steady_clock::now();
    context.pressure_cache.reserve(8192);

    SearchResult result;
    const auto root_info = generate_search_moves_info(root);
    if (root_info.empty()) return result;

    std::vector<Move> root_moves;
    root_moves.reserve(root_info.size());
    for (const auto& entry : root_info) root_moves.push_back(entry.move);

    auto root_swing = [&](const Move& move) {
        for (const auto& entry : root_info)
            if (entry.move == move) return entry.capture_swing;
        return 0;
    };

    auto build_pv = [&](const Move& first, Depth depth) {
        std::vector<Move> pv;
        Position position = root;
        Move move = first;
        // Selective extensions can make the meaningful PV longer than nominal depth.
        const int max_pv = std::min(MaxPly, depth + MaxSelectiveExtensions + 2);
        for (int ply = 0; ply < max_pv && move.path_length != 0; ++ply) {
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
            int score = context.history_score(move);
                        if (move == preferred) score += 1200000;
            const Value old_value = prior_root_value(move, prior_scores);
            if (old_value > -Infinity) score += 1000 * old_value;
            if (swing > 0)
                score += 500000 + swing * 10000 + context.capture_history_score(move);
            else if (swing < 0)
                score += -250000 + context.capture_history_score(move);
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
            const bool extend_score = current.capture_swing != 0;
            const Depth child_depth = depth - 1 + (extend_score ? 1 : 0);
            const int extensions = extend_score ? 1 : 0;

            Value score;
            if (move_index == 0) {
                score = -negamax(root, child_depth, -beta, -current_alpha, 1, context, true,
                                 &current.move, nullptr, extensions);
            } else {
                score = -negamax(root, child_depth, -current_alpha - 1, -current_alpha, 1,
                                 context, false, &current.move, nullptr, extensions);
                if (score > current_alpha && score < beta)
                    score = -negamax(root, child_depth, -beta, -current_alpha, 1, context, true,
                                     &current.move, nullptr, extensions);
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

        // Publish only a fully completed iteration. If time expires during the next depth,
        // callers keep this stable result rather than an interrupted partial root search.
        result.has_move = true;
        result.best_move = previous_best;
        result.value = previous_value;
        result.depth = depth;
        result.pv = build_pv(previous_best, depth);
    }

    if (result.has_move) {
        result.lines.push_back({result.best_move, result.value, result.pv});
        std::vector<Move> excluded{result.best_move};

        // MultiPV alternatives are searched only after the primary iterative-deepening line
        // reaches its final completed depth. The main line therefore receives the full depth
        // budget first instead of splitting every iteration across alternatives.
        for (int rank = 1; rank < context.limits.multipv && !context.stopped; ++rank) {
            Move preferred{};
            for (const auto& line : prior_scores) {
                if (!is_excluded(line.move, excluded)) {
                    preferred = line.move;
                    break;
                }
            }
            if (preferred.path_length == 0) break;

            const RootPass pass = search_root(result.depth, -Infinity, Infinity, excluded, preferred);
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
