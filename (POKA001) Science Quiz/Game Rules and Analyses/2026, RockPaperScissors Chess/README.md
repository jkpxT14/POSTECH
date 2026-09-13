# 2026, RockPaperScissors Chess

Full synchronized package revision **0.20.0** (2026-09-14), `.rpsc` **Format 3**, ruleset `2026-rpsc-rotation6-push-return`.

The handbook, offline Analysis Board, browser parser/serializer, native C++ engine protocol, examples, and regression tests follow one canonical separation:

- White / Black = board roles.
- POSTECH / KAIST = schools.
- `White`, `Black` headers = players/controllers.
- `WhiteTeam`, `BlackTeam` = school assignment.
- `Result`, `Score`, `Quiz`, `Captures` = White-Black order.
- Every per-question quiz token = **`Q[POSTECH, KAIST]`**.

The critical reverse-assignment regression is `WhiteTeam=KAIST`, `BlackTeam=POSTECH`: `Q[1,0] B+St` and `Q[0,1] W+Ro` are valid, while `Q[1,0] W+...` is invalid.

## Contents

- `RockPaperScissorsChess.pdf` / `.tex`: handbook and source.
- `RockPaperScissorsChess.html`: self-contained interactive Analysis Board with exact cube orientation, full item movement, three play modes, Format 2 loader / Format 3 writer, session history, replay validation, and three-line browser MultiPV analysis.
- `Engine/`: native C++17 RPSC Engine 0.20.0, source, regression tests, protocol tests, and paired self-play harness.
- `Examples/`: Game 6 legacy Format 2 and canonical Format 3 examples.
- `RELEASE_NOTES_0.20.0.md`: the synchronized migration contract and verification scope.
- `PackageStatus.md`: concise release status.

Format 2 files are read with their original legacy semantics (`Q[White, Black]`) and migrated in memory to school-indexed quiz state. New saves are Format 3 only.
