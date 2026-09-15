# RPSC Engine

Version: **0.23.0**  
Ruleset: `2026-rpsc-rotation6-push-return`

0.23.0 is a strength-focused RPSC search update. It keeps the 0.22.3 rules, evaluation scale, Format 3 school/board-role boundary, and Top 3 MultiPV analysis, while improving the efficiency of secondary root-line search.

The search core uses White/Black board roles. School identity is converted at the protocol boundary.

## School-aware protocol

```text
teams W KAIST B POSTECH
matchpk 11 10 6
```

`matchpk <POSTECHQuiz> <KAISTQuiz> <remainingPlies>` always uses POSTECH/KAIST order. The compatibility command `match <Wquiz> <Bquiz> <remainingPlies>` is explicitly White/Black ordered.

## 0.23 search update

The first root pass still searches the complete legal successor set. For MultiPV ranks 2 and 3, 0.23 reuses that completed root ordering and concentrates the full secondary searches on the strongest 24-score frontier, while retaining tied frontier moves and previously completed PV candidates. Top 3 output is preserved.

This is RPSC-specific search engineering: the move generator still treats Push, six Rotations, Step Short/Long, cube orientation, captures, recaptures, and Reset as native game semantics. No chess-specific positional model is introduced.

The 0.22 selective-search safeguards remain in place: tactical extension is bounded, qsearch keeps reset sacrifices searchable, and the late-move-reduction threat/defense guard is evaluated lazily.

### Research acceptance against 0.22.3

The selected candidate exceeded the requested 55% threshold in paired research matches while keeping MultiPV=3. In the main research sample it scored **46 wins, 3 draws, 31 losses**, or **59.375% match score**. The packaged 0.23.0 build was independently rechecked over 40 paired games at **1,000 board-search nodes / 300 item-search nodes** with MultiPV=3 and scored **22 wins, 2 draws, 16 losses**, or **57.5% match score** against 0.22.3.

Build with CMake and run the engine/browser regression tests from this source. The bundled `rpsc-engine` is a Linux x86-64 Release build.
