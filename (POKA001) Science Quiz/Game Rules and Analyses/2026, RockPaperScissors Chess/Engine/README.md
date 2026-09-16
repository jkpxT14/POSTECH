# RPSC Engine 0.24.0

This is the standalone engine component of the 2026 RockPaperScissors Chess project. The established native `Engine/src` + `Engine/tests` layout is preserved; the Analysis Board remains the separate `RockPaperScissorsChess.html` at the project root.

## Build

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

The executable is `rpsc-engine` (`rpsc-engine.exe` on Windows).

## Engine 0.24.0

The search keeps three recommendations while deliberately spending more compute on the first recommendation. It combines iterative deepening, alpha-beta/PVS-style re-search, a transposition table, quiescence search, tactical/Reset-aware extensions, selective 1v1-transition extension, late-move reductions for noncritical quiet moves, protected quiet/tactical challengers, Top-3 stabilization, and final root verification.

The strength-tested reference search core used by the Analysis Board is `reference/rpsc_engine_0.24.0.js`; its paired comparison records are documented in `Verification.md`.

## Role convention

- `W` / `B`: White / Black board roles.
- `P` / `K`: POSTECH / KAIST school identities.
- Analysis Board quiz notation: always `Q[POSTECH, KAIST]`, never White/Black order.
