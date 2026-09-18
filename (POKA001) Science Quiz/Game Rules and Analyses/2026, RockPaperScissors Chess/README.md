# 2026, RockPaperScissors Chess

**2026 Science War Edition** - **2026-09-17**  
Synchronized release **0.25.0**, `.rpsc` **Format 3**, ruleset `2026-rpsc-rotation6-push-return`.

This directory is one synchronized project: the handbook, the self-contained Analysis Board, and the RPSC engine share the same rules, notation, school/role mapping, score units, and search-state semantics.

## Invariants

- White / Black (`W` / `B`) are board roles.
- POSTECH / KAIST (`P` / `K`) are schools.
- Every quiz token is `Q[POSTECH, KAIST]`, independent of White/Black assignment.
- `WhiteTeam` / `BlackTeam` record the school-to-role assignment.
- Quiz correct = **1 point**; capture = **2 points** throughout the handbook, Analysis Board, records, and engine evaluation scale.
- Numeric engine evaluation is from the White-role perspective.
- Practice/analysis resignation is a manual record/UI action, not an official Science War rule.
- The engine never auto-resigns.
- Push permits the first Roll to return to the pre-Push square.
- All six Rotation directions are distinguished in exact orientation and notation.

## Contents

- `RockPaperScissorsChess.pdf` / `.tex`: 2026 Science War Edition handbook, including Games 1-11.
- `RockPaperScissorsChess.html`: self-contained Analysis Board with school-colored pieces, live Top 3, history/variations, cache reuse, manual Resign, 10 s default analysis, and cumulative 20 s Analyze continuation.
- `Engine/`: RPSC Engine 0.25.0 JavaScript production core, native C++ source, regression tests, and compact reference material.
- `Examples/`: legacy Format 2 and canonical Format 3 Game 6 records.

## Final engine

The 0.25.0 Science War engine keeps the proven primary-biased MultiPV framework and strengthens the capture -> low-material -> recapture/Reset horizon with a capped four-ply transition extension. The previous 0.24.1 production core is retained under `Engine/reference/` as the direct strength baseline. See `Engine/Verification.md` for the measured paired gate and regression scope.

The source package contains no compiled binaries, build directories, SyncTeX files, temporary candidate engines, benchmark scratch files, or patch-note files.
