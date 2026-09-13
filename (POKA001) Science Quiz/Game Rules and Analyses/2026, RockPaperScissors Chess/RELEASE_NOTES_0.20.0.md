# RPSC 0.20.0 — Format 3 Semantics Release

Date: 2026-09-14  
Ruleset: `2026-rpsc-rotation6-push-return`

This release is a synchronized handbook / Analysis Board / native-engine migration. The game rules and verified game states are not redefined; the change removes the ambiguity between school identity, board role, and controller identity.

## Canonical identity model

- **School:** POSTECH / KAIST.
- **Board role:** White / Black. White is the first-mover role and Black is the second-mover role after the opening order is fixed.
- **Controller:** the player, squad, human, or engine controlling that board role.
- Every quiz token is fixed school order: **`Q[POSTECH, KAIST]`**.
- `W1`…`W4`, `B1`…`B4`, `W+...`, `B+...`, and `1-0` / `0-1` remain White/Black role notation.
- `Score`, `Quiz`, and `Captures` headers remain White-Black aggregate order.

## `.rpsc` Format 3

New files use:

```text
[Format "3"]
[White "<White controller>"]
[Black "<Black controller>"]
[WhiteTeam "POSTECH|KAIST"]
[BlackTeam "KAIST|POSTECH"]
[QOrder "POSTECH, KAIST"]
```

Format 2 remains load-compatible. Its legacy `Q[a,b]` is interpreted as `Q[White, Black]` using the legacy White/Black school headers, converted to school-indexed state, and emitted as Format 3 on the next save. No Format 2 file is silently reinterpreted with Format 3 semantics.

## Game records

Games 1–5 retain `WhiteTeam=POSTECH`, `BlackTeam=KAIST`. Game 6 is the high-risk reversed assignment: `WhiteTeam=KAIST`, `BlackTeam=POSTECH`. Therefore, for example, `Q[1,0] B+St` and `Q[0,1] W+Pu` are canonical in Game 6. The underlying item ownership, moves, captures, resets, scores, and result are unchanged.

## Analysis Board

The self-contained offline Analysis Board is updated to 0.20.0 and uses the same Format 3 contract. It includes exact 24-orientation cube state, all six Rotation actions, Push-return legality, Step, capture/reset replay, Human vs Human / Human vs Engine / Engine vs Engine modes, save/load, session history, legacy Format 2 migration, and three-line browser MultiPV board analysis.

## Native engine

The search core remains intentionally White/Black based. School identity is handled only at the protocol boundary:

```text
teams W KAIST B POSTECH
matchpk 11 10 6
```

`matchpk` is always `<POSTECHQuiz> <KAISTQuiz> <remainingPlies>`. In the example above it maps to internal `quiz_white=10`, `quiz_black=11`. The old `match <Wquiz> <Bquiz> <remainingPlies>` command is retained as an explicitly role-ordered compatibility command.

## Verification gate

The release is packaged only after all of the following pass: native engine regressions and exact endgame ranking, browser Format 3 / Format 2 migration regressions, six-game replay, the 120-round handbook school-to-role validator, the reversed-assignment protocol test, a paired self-play smoke test, and a clean two-pass XeLaTeX handbook build plus rendered-PDF inspection.
