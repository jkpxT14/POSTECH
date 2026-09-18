# RPSC Engine 0.25.0 - 2026 Science War Edition

Edition date: **2026-09-17**.

Standalone engine source for the 2026 RockPaperScissors Chess project. The production Analysis Board is `../RockPaperScissorsChess.html`; its embedded JavaScript worker is exactly `rpsc_engine_0.25.0.js`. The native C++ engine preserves the same RPSC rules boundary and the same selected low-material transition search safeguard while retaining the full protocol surface used for testing and external tooling.

## Build

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

No compiled binaries are included in the source package.

## Science War search policy

- MultiPV remains 3 in the Analysis Board.
- Broad root search is followed by a primary-biased championship search.
- Quiet and tactical/item challengers remain protected at the root.
- Captures receive selective extension; Reset lines remain protected.
- A capture that crosses from more than two living pieces to two living pieces receives a deeper, capped transition extension. This directly targets the recapture/Reset horizon exposed by the handbook-game regression corpus.
- Top 3 are stabilized before final root verification.
- Default analysis is 10 s; Analyze continues the cached search toward 20 s cumulative.

## Score and role boundary

- One Quiz answer = **1 RPSC score unit**.
- One capture = **2 RPSC score units**.
- `W` / `B` are board roles only.
- `P` / `K` mean POSTECH / KAIST.
- `matchpk <POSTECHQuiz> <KAISTQuiz> <remainingPlies>` is always P/K ordered and is converted to W/B only after `teams`.
- Format 3 quiz notation remains `Q[POSTECH, KAIST]` regardless of school-to-role assignment.

## Resignation boundary

Practice/analysis resignation belongs to the Analysis Board and record layer. The engine never auto-resigns, never converts an evaluation threshold into a result, and continues searching for the best move in arbitrarily unfavorable positions.

Use `help` in `rpsc-engine` for the supported protocol commands.
