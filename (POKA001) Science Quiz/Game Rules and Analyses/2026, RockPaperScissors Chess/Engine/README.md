# RPSC Engine

Version: **0.21.1**  
Ruleset: `2026-rpsc-rotation6-push-return`

The search core uses White/Black board roles. School identity is converted at the protocol boundary.

## School-aware protocol

```text
teams W KAIST B POSTECH
matchpk 11 10 6
```

`matchpk <POSTECHQuiz> <KAISTQuiz> <remainingPlies>` always uses POSTECH/KAIST order. With the mapping above, the internal context is `quiz_white=10`, `quiz_black=11`. The compatibility command `match <Wquiz> <Bquiz> <remainingPlies>` is explicitly White/Black ordered.

The 0.21.1 engine retains the strengthened 0.21 search core: RPSC-scaled selective pruning and exact tactical-reach pruning while preserving the complete legal move set. The strengthened core was benchmarked against 0.20.0 at 6,000 move-search nodes and 1,800 item-search nodes per decision, scoring **55 wins, 43 losses, 2 draws in 100 games** (56.0% match score).

Build with CMake and run `ctest --test-dir build --output-on-failure`. The bundled `rpsc-engine` is a Linux x86-64 Release build from this source.
