# RPSC Engine

`rpsc-engine` is the classical search Engine for RockPaperScissors Chess.

Version: 0.7.0

## Design

- C++17; no machine learning, neural networks, or NNUE
- exact 24-orientation cube state as the rules and notation source of truth
- derived six-state Gesture State for strategically equivalent search-state reduction under the current rules
- canonical full-path legal move generation, including Push, Rotation, Step Short, and Step Long when available
- iterative deepening, principal-variation search, aspiration windows, and a four-way clustered transposition table
- TT, tactical, killer, history, continuation, two-ply follow-up, countermove, capture-history, and prior-iteration root ordering
- conservative late-move reductions with RPSC threat/defence and known-reply safeguards
- direct tactical move generation and bounded tactical quiescence
- bounded selective extension around score-changing combat events: at most two extra plies per line
- root MultiPV after the primary line is established
- Evaluation in RPSC score units (`100 = 1.00`, one capture = `200`), with only small rule-derived positional terms

Version 0.7.0 is intentionally not tuned to a named square, Gesture, opening, or supplied game. The score-event extension is derived from the rule that combat can change the official score and is bounded to prevent unbounded tactical growth. The evaluation function receives no special-case bonus for the regression position that motivated this search review.

Timed searches check their deadline more frequently than 0.6.0 so the 10 s / 20 s Analysis Board budgets are respected more closely.

## Build

```text
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
ctest --test-dir build --output-on-failure
```

## Protocol

The command-line interface is UCI-inspired but not UCI-compatible.

```text
rpsc
isready
newgame
position startpos
items W 1 1 1
move W3[RoL]: e1-e2-e3-e4-d4-c4
go depth 8
go depth 6 multipv 3
go movetime 10000
perft 2
bench
quit
```

A timed search uses a high depth ceiling and stops primarily on its time budget. Search information uses `depth`, `seldepth`, `nodes`, `nps`, `score`, and `pv`; displayed `score` is from White's point of view.

## Analysis Board relationship

The C++ Engine remains the native reference implementation. The offline single-file Analysis Board uses a parallel classical JavaScript Web Worker so it can run without a local executable.

Draft 27 gives normal current-position analysis up to about 10 seconds. `Analyze` raises the same position to a total deep-analysis budget of about 20 seconds. The browser retains completed position results in a bounded exact-position cache and, when possible, starts Deep Analysis after the last completed depth rather than repeating earlier iterations. The Worker also keeps its transposition table while the same analysis session continues. Primary-PV iterative deepening is prioritized before final-depth MultiPV alternatives so three shallow lines do not consume every iteration equally.

Historical Main-Line/Variation navigation, clickable Game Record positions, exact state restoration, and candidate board preview remain non-destructive analysis layers.

## Strength testing

Draft 27 follows the experimental discipline used in chess-engine development: compare a candidate against a fixed baseline over paired starting positions with colors swapped, and keep correctness tests separate from strength tests. See `StrengthTesting.md` for the current results and their limitations.

No Elo, human-superiority, or statistically proven strength claim is attached to 0.7.0.
