# RPSC Engine

`rpsc-engine` is the classical search Engine for RockPaperScissors Chess.

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
- search-move generation that carries capture-swing metadata so ordering does not repeat an extra make/unmake for every candidate
- true root MultiPV analysis for several fully searched Candidate Moves
- Evaluation scaled in RPSC score units (`100 = 1.00` point, one capture = `200`)

Engine-containing Analysis Board play modes use only `Q[0, 0]`, so they do not acquire items during play. The Engine core itself is item-aware because Human vs Human analysis must evaluate the actual current position, including any Push, Rotation, or Step resources already held by either side. Static Evaluation gives every held item the same small reserve term only to prevent finite-horizon searches from treating unused resources as free to spend. The term is intentionally much smaller than a scored capture and is not a calibrated claim that Push, Rotation, and Step have equal strategic value; item use is still judged primarily through search and its consequences.

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

`items W 1 1 1` sets White's current Push, Rotation, and Step counts in that order; use `B` for Black. Single-PV search concentrates computation on the best line. With `multipv N`, the Engine emits `info multipv 1`, `info multipv 2`, and so on for the completed root lines, followed by `bestmove`. Search information uses `depth`, `seldepth`, `nodes`, `nps`, `score`, and `pv`. Displayed `score` uses White's point of view to match the handbook and Analysis Board; the search core itself uses side-to-move negamax values internally.

## Analysis Board relationship

The C++ Engine is the native reference implementation. The offline single-file Analysis Board uses a classical JavaScript Web Worker bridge so that it can run without a server or local executable. The browser worker uses the same exact-orientation rule source, derived Gesture-State search key, current item inventories, score scale, Move Notation, PV/M-number notation, and Engine Analysis vocabulary. Browser search uses shorter time budgets than the native Engine but follows the same capture-quiescence principle, including legal item captures. A future WebAssembly build can replace the bridge without changing the handbook or Game Record format.

## Strength target

The target is a fast, accurate classical practice Engine that can become stronger than a typical human player. Strength is not assumed from architecture alone and should be established through regression matches, self-play, tactical suites, and human testing.
