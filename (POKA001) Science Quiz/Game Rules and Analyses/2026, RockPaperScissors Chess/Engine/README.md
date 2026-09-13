# RPSC Engine

Version: **0.20.0**  
Ruleset: `2026-rpsc-rotation6-push-return`

The search core is intentionally board-role based (`White` / `Black`). School identity is external match metadata and is converted exactly once at the protocol boundary.

## School-aware protocol

```text
teams W KAIST B POSTECH
matchpk 11 10 6
```

`matchpk <POSTECHQuiz> <KAISTQuiz> <remainingPlies>` always uses POSTECH/KAIST order. With the mapping above it becomes internal `quiz_white=10`, `quiz_black=11`. The older `match <Wquiz> <Bquiz> <remainingPlies>` command remains only as an explicit White/Black compatibility command.

Build with CMake and run `ctest --test-dir build --output-on-failure`. The bundled `rpsc-engine` is a Linux x86-64 Release build from this 0.20.0 source.
