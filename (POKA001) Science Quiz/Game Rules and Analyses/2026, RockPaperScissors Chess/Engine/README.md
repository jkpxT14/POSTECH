# RPSC Engine

`rpsc-engine` is the classical search Engine for RockPaperScissors Chess.

Version: 0.6.0

## Design

- C++17
- no machine learning, neural networks, or NNUE
- exact 24-orientation cube state as the rules and notation source of truth
- derived six-state Gesture State for strategically equivalent search-state reduction under the current rules
- canonical full-path legal move generation, including Push, Rotation, Step Short, and Step Long when available
- iterative deepening with principal-variation search and aspiration windows
- four-way clustered transposition table with deterministic 64-bit position keys and generation-aware replacement
- TT, tactical, killer, history, continuation, two-ply follow-up, countermove, capture-history, and prior-iteration root move ordering
- bounded history updates that reward successful quiet replies and gently demote quiet alternatives searched earlier at the same cutoff
- conservative late-move reductions for quiet non-item moves, with RPSC threat/defence and known-reply safeguards before reduction
- direct tactical move generation for quiescence and immediate scoring-pressure checks
- bounded tactical quiescence search that keeps legal combat continuations explicit and uses capture history for ordering
- true root MultiPV analysis for several fully searched Candidate Moves
- Evaluation scaled in RPSC score units (`100 = 1.00` point, one capture = `200`)
- conservative rule-derived positional terms only: official score, a small alive-piece term, a small uncalibrated item reserve, legal first-Roll mobility, and legal non-reversing second-Roll continuation flexibility

The search distinguishes `quiet` from `unimportant`. Before reducing a late quiet move, the Engine checks immediate RPSC scoring pressure. A quiet move that removes an opponent's immediate scoring possibility or creates a new immediate scoring possibility is searched at full depth. Version 0.6.0 also treats established countermoves, killer replies, and sufficiently strong history replies more cautiously under LMR. One-ply continuation history, two-ply follow-up history, and negative feedback for inferior quiets improve ordering without declaring a particular square, opening, Gesture, or waiting pattern intrinsically good.

Basic tactics remain first-class. Captures receive explicit move-ordering priority, tactical cutoffs update capture history, and quiescence uses a dedicated tactical generator rather than relying on generic quiet-move machinery. Quiet, defensive, and waiting play is strengthened by search context rather than by weakening immediate capture/scoring tactics or by adding an arbitrary `waiting move` bonus.

Engine-containing Analysis Board play modes use `Q[0, 0]` for every Quiz; Quiz is not removed. These modes therefore do not acquire items through normal play. The Engine core itself remains item-aware because Human vs Human analysis can contain actual Push, Rotation, or Step inventories.

## Build

```text
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
ctest --test-dir build --output-on-failure
```

## Protocol

The command-line interface is deliberately small and UCI-inspired, but it is not UCI-compatible.

```text
rpsc
isready
newgame
position startpos
items W 1 1 1
move W3[RoL]: e1-e2-e3-e4-d4-c4
go depth 6
go depth 5 multipv 3
go movetime 3000
perft 2
bench
quit
```

`items W 1 1 1` sets White's current Push, Rotation, and Step counts in that order; use `B` for Black. Single-PV search concentrates computation on the best line. With `multipv N`, the Engine emits numbered root lines followed by `bestmove`. Search information uses `depth`, `seldepth`, `nodes`, `nps`, `score`, and `pv`. Displayed `score` uses White's point of view to match the handbook and Analysis Board; the search core itself uses side-to-move negamax values internally.

## Analysis Board relationship

The C++ Engine is the native reference implementation. The offline single-file Analysis Board uses a parallel classical JavaScript Web Worker so that it can run without a server or local executable. Draft 26 mirrors capture/continuation/follow-up history, countermove ordering, and threat-aware quiet-move reduction principles in that worker.

The Analysis Board separates the canonical Game Head from the Analysis Cursor. The canonical Game Record remains visible while the cursor visits an earlier Main-Line position or a Variation. Quiz events remain real state/history events even though Previous/Next conveniently moves among complete board-decision positions. At a historical position Engine play pauses and Engine Analysis continues.

MultiPV Alternatives are connected directly to the board. When the current position is human-controllable, selecting a candidate produces a complete canonical Move preview; the path remains hypothetical until `Confirm Move`. Confirming from a historical position stores/reuses a Variation; cancelling stores nothing. This keeps Engine exploration useful without filling the analysis tree with every candidate merely viewed.

A future WebAssembly build can replace the parallel worker implementation without changing the handbook, Game Record, or analysis vocabulary. Draft 26 does not claim that the C++ core is already running in the browser.

## Design references

Version 0.6.0 adapts established classical-engine ideas rather than chess-specific knowledge:

- Stockfish: iterative PVS, TT discipline, quiet/continuation/correction-style history concepts, and conservative selective-search design
- Fairy-Stockfish: separation of a strong search framework from variant-specific rules and state semantics
- Ethereal: follow-up/continuation-style history and history-aware selective-search ideas
- Edax Reversi: classical PVS, transposition-table, mobility, and selective-search discipline
- threat-oriented Gomoku engines: forcing threats and required defensive replies deserve different treatment from generic quiet moves

RPSC-specific legality, full Roll paths, exact orientation, combat, Reset, Quiz flow, official scoring, and items are implemented independently from these references.

## Strength target

The target is a fast, accurate classical practice Engine that can eventually exceed strong human play. Version 0.6.0 does not claim measured superiority. Search changes should be retained only when rule regression, tactical testing, self-play, and human analysis support them.
