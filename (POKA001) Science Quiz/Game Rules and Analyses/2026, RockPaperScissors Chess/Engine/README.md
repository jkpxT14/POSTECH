# RPSC Engine

`rpsc-engine` is the classical search Engine for RockPaperScissors Chess.

Version: 0.4.0

## Design

- C++17
- no machine learning, neural networks, or NNUE
- exact 24-orientation cube state as the rules and notation source of truth
- derived six-state Gesture State for strategically equivalent search-state reduction under the current rules
- canonical full-path legal move generation, including Push, Rotation, Step Short, and Step Long when available
- iterative deepening with principal-variation search
- aspiration windows
- transposition table with deterministic 64-bit position keys
- TT, tactical, killer, history, and prior-iteration root move ordering
- conservative late-move reductions for quiet non-item moves
- bounded capture quiescence search
- true root MultiPV analysis for several fully searched Candidate Moves
- Evaluation scaled in RPSC score units (`100 = 1.00` point, one capture = `200`)
- conservative rule-derived positional terms only: alive-piece balance, a small uncalibrated item reserve, legal first-Roll mobility, and legal non-reversing second-Roll continuation flexibility

The path-flexibility term is deliberately small relative to the official capture score. It does not assign an arbitrary bonus to the center, a named opening, or a particular gesture. Its purpose is to distinguish otherwise equal leaves using movement possibilities that follow directly from the RPSC Roll rules.

Engine-containing Analysis Board play modes use only `Q[0, 0]`, so they do not acquire items during play. The Engine core itself remains item-aware because Human vs Human analysis must evaluate the actual current position, including any Push, Rotation, or Step resources already held by either side.

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

The C++ Engine is the native reference implementation. The offline single-file Analysis Board uses a classical JavaScript Web Worker so that it can run without a server or local executable. Draft 24 keeps completed Engine results in a persistent analysis panel and lets the worker emit only completed iterative-deepening snapshots. The Current action panel now mirrors the canonical Move Notation live during both human drafting and Engine Roll animation; this UI change does not alter Engine search or Evaluation. The browser worker mirrors the native rule model and the Draft 23 local path-flexibility Evaluation term.

The user-facing analysis pattern is intentionally conservative: persistent current-position output, numerical Evaluation, Candidate Moves, PV/MultiPV, Depth, SelDepth, Nodes, and NPS. It borrows proven interaction ideas from Stockfish-compatible analysis GUIs, Chess.com Self Analysis, and En Croissant while retaining the RPSC handbook's own notation and board-first visual language.

A future WebAssembly build can replace the parallel worker implementation without changing the handbook or Game Record format. Draft 24 does not claim that the C++ core is already running in the browser.

## Strength target

The target is a fast, accurate classical practice Engine that can eventually exceed strong human play. Strength is not assumed from architecture alone and should be established through regression matches, self-play, tactical suites, and human testing.
