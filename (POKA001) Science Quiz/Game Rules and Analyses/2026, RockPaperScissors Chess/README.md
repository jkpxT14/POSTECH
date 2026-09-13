# 2026, RockPaperScissors Chess

Synchronized package revision **0.21.0** (2026-09-14), `.rpsc` **Format 3**, ruleset `2026-rpsc-rotation6-push-return`.

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
- `RockPaperScissorsChess.html`: self-contained Analysis Board with Format 2 migration, Format 3 save/load, replay validation, and three-line analysis.
- `Engine/`: native C++17 RPSC Engine 0.21.0, source, tests, and paired self-play harness.
- `Examples/`: legacy Format 2 and canonical Format 3 Game 6 records.

Format 2 files are read with their original `Q[White, Black]` semantics and migrated in memory. New saves use Format 3.
