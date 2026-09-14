# RPSC Engine

Version: **0.22.1**  
Ruleset: `2026-rpsc-rotation6-push-return`

0.22.1 is a synchronized patch release; the native search core is unchanged from 0.22.0.

The search core uses White/Black board roles. School identity is converted at the protocol boundary.

## School-aware protocol

```text
teams W KAIST B POSTECH
matchpk 11 10 6
```

`matchpk <POSTECHQuiz> <KAISTQuiz> <remainingPlies>` always uses POSTECH/KAIST order. With the mapping above, the internal context is `quiz_white=10`, `quiz_black=11`. The compatibility command `match <Wquiz> <Bquiz> <remainingPlies>` is explicitly White/Black ordered.

## 0.22 search update

0.22.0 keeps the 0.21 search architecture and exact tactical-reach pruning, while tightening selective search for RPSC:

- at most one additional tactical extension is carried down a line;
- quiescence omits non-reset negative-swing sacrifices, while reset sacrifices remain searchable;
- the threat/defense guard for late-move reductions is evaluated lazily, preserving the guard while avoiding unnecessary tactical generation.

The same selective-search changes are embedded in `RockPaperScissorsChess.html` for browser analysis.

### Acceptance match against 0.21.1

Four independent deterministic paired suites were run at **1,000 board-search nodes / 300 item-search nodes per decision**. The quiz schedule is paired and the engines swap White/Black (and therefore POSTECH/KAIST) within every pair.

- Suite A, 100 games: **53 wins, 45 losses, 2 draws** - 53.0% wins, 54.0% match score.
- Suite B, 100 games: **60 wins, 36 losses, 4 draws** - 60.0% wins, 62.0% match score.
- Suite C, 100 games: **58 wins, 37 losses, 5 draws** - 58.0% wins, 60.5% match score.
- Suite D, 100 games: **61 wins, 37 losses, 2 draws** - 61.0% wins, 62.0% match score.
- Combined, 400 games: **232 wins, 155 losses, 13 draws** - **58.0% win rate**, 59.625% match score.

This benchmark is fixed-node rather than wall-clock, so it compares search decisions without machine-timing noise.

Build with CMake and run `ctest --test-dir build --output-on-failure`. The bundled `rpsc-engine` is a Linux x86-64 Release build from this source.
