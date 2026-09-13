# 2026, RockPaperScissors Chess

Synchronized package revision **0.21.1** (2026-09-14), `.rpsc` **Format 3**, ruleset `2026-rpsc-rotation6-push-return`.

The Analysis Board keeps the established SVG board, move-path animation, history/variation workflow, Gesture-State analysis, 10 s → 20 s analysis flow, and Jungwoo Kim (POSTECH) byline. It adds a live position evaluation and retains stable Top 3 recommendations with PVs on the main line and analysis variations.

The handbook, offline Analysis Board, native engine, examples, and regression tests use one notation contract:

- White / Black = board roles.
- POSTECH / KAIST = schools.
- `White`, `Black` headers = players/controllers.
- `WhiteTeam`, `BlackTeam` = school assignment.
- `Result`, `Score`, `Quiz`, `Captures` = White-Black order.
- Every per-question quiz token = `Q[POSTECH, KAIST]`.

For a reverse assignment (`WhiteTeam=KAIST`, `BlackTeam=POSTECH`), `Q[1,0] B+...` and `Q[0,1] W+...` are the canonical item-gain records.

## Contents

- `RockPaperScissorsChess.pdf` / `.tex`: handbook and source.
- `RockPaperScissorsChess.html`: self-contained Analysis Board with Format 2 migration, Format 3 save/load, live evaluation, variation analysis, move animation, and Top 3 PV analysis.
- `Engine/`: native C++17 RPSC Engine 0.21.1, source, tests, and paired self-play harness.
- `Examples/`: legacy Format 2 and canonical Format 3 Game 6 records.

Format 2 files are read with their original `Q[White, Black]` semantics and migrated in memory. New saves use Format 3.
