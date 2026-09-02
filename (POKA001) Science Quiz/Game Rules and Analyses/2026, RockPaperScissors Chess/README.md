# RockPaperScissors Chess

Build: **Draft 36** (2026-09-02 KST)  
Native Engine: **0.14.0**  
Source base: GitHub `main` at `5070395ae9fb26e35adc9c9dc14cafa09e9fb4ed`.

This directory is one RPSC system rather than three independent artifacts:

**Canonical Rule Model → Notation / State Model → Handbook / Analysis Board / Native Engine**

The handbook defines the human-readable rules and notation, the offline Analysis Board uses the same exact cube and move semantics interactively, and the native Engine provides the reference classical search implementation and regression suite.

## Draft 36

Draft 36 is a readability, analysis-workspace, and search-performance release.

### Handbook

- Keeps the 11 pt book layout, restrained academic styling, Latin Modern English/mathematics, and existing figure language.
- Uses `NanumMyeongjo` for Korean body text when installed, with `Noto Serif CJK KR` as the fallback. The change gives long Korean paragraphs slightly stronger visual weight without making body text bold.
- Tightens the flow of Rules / Notation / Movement explanations and makes the Exact Orientation → Gesture State relationship explicit.
- Preserves the latest three internal-league game records and detailed score breakdowns from the source commit.

### Analysis Board

`RockPaperScissorsChess.html` remains an offline single-file analysis workspace. Draft 36 keeps the project’s white, line-based academic visual language while making the analysis hierarchy clearer: the primary candidate receives a restrained left rule, depth/nodes/time appear as compact status chips, and history controls remain adjacent to the Game Record. The design direction takes useful interaction patterns from established chess analysis tools—large stable board, nearby engine lines, forward/back navigation, board rotation, and MultiPV-style alternatives—without copying their visual branding.

The board supports Human vs Human, Human vs Engine, and Engine vs Engine control modes. Quiz Results remain explicit user input. Board analysis runs in a Web Worker so long searches do not intentionally block the interface; `Analyze` first targets about 10 seconds and a repeated request reuses the completed same-position result while extending toward about 20 total seconds.

Format 2 saves the readable canonical Main Line together with the current analysis-history tree. Session metadata is checked against the readable Main Line before restoration.

### Native Engine 0.14.0

Engine 0.14.0 retains the exact rule model and the 0.13 search base while adding two allocation/search-budget improvements:

1. board one-step transitions are precomputed in the hot move-generation path;
2. same-key timed continuation stores the last completed root depth/ranking and resumes from the next unfinished depth instead of repeating already completed iterative-deepening passes.

The 0.13 ordering stack is retained: TT move first, history, capture history, continuation/follow-up, countermove, killers, tactical quiescence, score-event extensions, pressure-aware LMR, aspiration, and root family quotas with verification.

In the release environment the seven-run no-item depth-4 control used 203,165 nodes with a median of 812 ms and preserved the established best root `W1: a1-a2-a3-b3`. The documented 0.12/0.13-era control was about 212k nodes / 858 ms. The item-rich 10-second control still completes depth 3; a subsequent 20-second same-position search spends its budget directly on the unfinished depth 4 but does not complete depth 4 in this stress position. No stronger claim is made.

See `Engine/StrengthTesting.md` and `Engine/Verification.md`.

## Semantic freezes

These meanings are release invariants and are regression-tested:

1. **Rotation:** in-place 90° CW/CCW rotation about the board-vertical axis. Square and top face remain fixed. `RoL` and `RoR` remain distinct exact actions.
2. **Timeout:** the handbook records the official timeout/random-fallback rule. Analysis Board and Engine do not adjudicate timeout/random replacement.
3. **Push return:** Push is not a Roll. The first Roll after Push may immediately return to the square occupied before Push.

The exact 24-orientation cube state is authoritative. The six-state Gesture State is a derived reduction only.

## Build

### Handbook

XeLaTeX is required.

```bash
latexmk RockPaperScissorsChess.tex
```

### Engine

```bash
cmake -S Engine -B Engine/build -DCMAKE_BUILD_TYPE=Release
cmake --build Engine/build
ctest --test-dir Engine/build --output-on-failure
```

The packaged release intentionally excludes build directories and LaTeX auxiliary files.
