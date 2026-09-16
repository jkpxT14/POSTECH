# RPSC Engine 0.24.1

Standalone native engine and reproducible strength-reference material for the 2026 RockPaperScissors Chess project.

The production Analysis Board is `../RockPaperScissorsChess.html`. Its embedded JavaScript search core is the strength-tested engine used by the browser; the same production core is also exposed directly as `rpsc_engine_0.24.1.js`. The native C++ engine mirrors the same RPSC search priorities while preserving the full protocol surface used for debugging, item/order probes, regression tests, and external tooling.

## Build

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

No compiled binaries are committed in this package; build them locally from source.

## 0.24 search policy

- MultiPV remains 3 for analysis.
- Broad root search first; compute then shifts toward recommendation #1.
- Protected quiet and tactical/item challengers remain searchable.
- Tactical/Reset and selective low-material-transition extensions are guarded from aggressive reduction.
- Top 3 are stabilized before final #1 verification.
- The browser uses 10 s default analysis and an Analyze continuation to 20 s cumulative.

## Protocol boundary

- `W` / `B` are board roles only.
- `P` / `K` mean POSTECH / KAIST.
- `matchpk <POSTECHQuiz> <KAISTQuiz> <remainingPlies>` is always P/K ordered and is converted to W/B only after reading `teams W ... B ...`.
- Format 3 quiz notation in the Analysis Board remains `Q[POSTECH, KAIST]` regardless of which school is White.

Use `help` in `rpsc-engine` for supported commands.
