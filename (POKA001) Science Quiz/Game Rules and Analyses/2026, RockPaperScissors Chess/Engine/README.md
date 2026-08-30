# RPSC Engine

`rpsc-engine` is the classical search Engine for RockPaperScissors Chess.

Version: 0.5.0

## Design

- C++17
- no machine learning, neural networks, or NNUE
- exact 24-orientation cube state as the rules and notation source of truth
- derived six-state Gesture State for strategically equivalent search-state reduction under the current rules
- canonical full-path legal move generation, including Push, Rotation, Step Short, and Step Long when available
- iterative deepening with principal-variation search
- aspiration windows
- four-way clustered transposition table with deterministic 64-bit position keys and generation-aware replacement
- TT, tactical, killer, history, continuation-history, countermove, and prior-iteration root move ordering
- conservative late-move reductions for quiet non-item moves, with RPSC threat/defence safeguards before reduction
- direct tactical move generation for quiescence and immediate scoring-pressure checks
- bounded tactical quiescence search that retains both winning and losing combat continuations when they are legal tactical moves
- true root MultiPV analysis for several fully searched Candidate Moves
- Evaluation scaled in RPSC score units (`100 = 1.00` point, one capture = `200`)
- conservative rule-derived positional terms only: official score, a small alive-piece term, a small uncalibrated item reserve, legal first-Roll mobility, and legal non-reversing second-Roll continuation flexibility

The search distinguishes `quiet` from `unimportant`. Before reducing a late quiet move, the Engine checks immediate RPSC scoring pressure. A quiet move that removes an opponent's immediate scoring possibility or creates a new immediate scoring possibility for the mover is searched at full depth. Continuation history and countermoves also allow useful defensive or waiting responses to move earlier in the search after related preceding moves. There is deliberately no arbitrary `waiting move`, center, opening, or gesture bonus.

Basic tactics remain first-class. Captures receive explicit move-ordering priority, and quiescence uses a dedicated tactical generator rather than relying on the general quiet-move machinery. The goal of the Draft 25 search changes is therefore to improve quiet attack, defence, and waiting play without trading away immediate tactical vision.

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

The C++ Engine is the native reference implementation. The offline single-file Analysis Board uses a parallel classical JavaScript Web Worker so that it can run without a server or local executable. Draft 25 keeps the persistent completed-depth analysis behavior from Draft 24 and mirrors continuation/countermove ordering plus threat-aware quiet-move reduction in the browser worker.

The Analysis Board now separates canonical game history from analysis navigation. Previous/Next moves an Analysis Cursor among complete board-decision states, while Quiz events remain part of the stored RPSC history. At an earlier position, Engine play and automatic fixed-result Quiz progression pause, but Engine Analysis continues. Playing a different legal move creates a Variation rather than rewriting the canonical Main Line.

A future WebAssembly build can replace the parallel worker implementation without changing the handbook, Game Record, or analysis vocabulary. Draft 25 does not claim that the C++ core is already running in the browser.

## Design references

Draft 25 uses established classical-engine ideas as references, adapted to RPSC rather than copied as chess knowledge:

- Stockfish: iterative PVS, TT discipline, quiet-move history, continuation history, countermove-style ordering, and conservative late-move reduction concepts
- Fairy-Stockfish: separation of a strong search framework from variant-specific rules and state semantics
- Edax Reversi: classical PVS, transposition-table, mobility, and selective-search discipline
- threat-oriented Gomoku engines: the principle that forcing threats and their required defensive replies deserve different treatment from generic quiet moves

RPSC-specific legality, full Roll paths, exact orientation, combat, Reset, Quiz flow, official scoring, and items are implemented independently from these references.

## Strength target

The target is a fast, accurate classical practice Engine that can eventually exceed strong human play. Version 0.5.0 does not claim measured superiority. Search changes should be retained only when rule regression, tactical testing, self-play, and human analysis support them.
