# RPSC Engine

`rpsc-engine` is the classical search engine for RockPaperScissors Chess.

## Design

- C++17
- no machine learning, neural networks, or NNUE
- exact 24-orientation cube state as the rules and notation source of truth
- full-path legal move generation
- iterative deepening with principal-variation search
- aspiration windows
- transposition table with deterministic 64-bit position keys
- TT, tactical, killer, and history move ordering
- conservative late-move reductions for quiet moves
- bounded capture quiescence search
- evaluation scaled in RPSC score units (`100 = 1.00` point, one capture = `200`)

The initial engine is item-free because the two engine-containing Analysis Board modes use only `Q[0, 0]`. This is an implementation scope, not a different ruleset: every Quiz turn still exists and is recorded as `Q[0, 0]`.

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
move W3: e1-e2-e3-e4-d4-c4
go depth 6
go movetime 3000
perft 2
bench
quit
```

Search output uses `info depth`, `seldepth`, `nodes`, `nps`, `score`, and `pv`, followed by `bestmove`. Displayed `score` uses White's point of view to match the handbook and Analysis Board; the search core itself uses side-to-move negamax values internally.

## Strength target

The target is a fast, accurate classical practice engine that can become stronger than a typical human player. Strength is not assumed from architecture alone and should be established through regression matches, self-play, tactical suites, and human testing.
