# Package Status — 0.20.0 / Format 3

## Upstream lineage

- Repository: `jkpxT14/POSTECH`
- Baseline branch: `main`
- Baseline inspected commit: `a4fae5c40d4c7db56d3898676744e474a592ce51`
- Package path: `(POKA001) Science Quiz/Game Rules and Analyses/2026, RockPaperScissors Chess/`
- Previous native checkpoint: Engine 0.19.0
- Ruleset: `2026-rpsc-rotation6-push-return`

## Consistency migration

0.20.0 is a synchronized semantics release. The native search core remains White/Black-oriented; this release changes the school/role boundary and shared record semantics.

1. White/Black are board roles and order game names, result, score, quiz totals, and captures.
2. POSTECH/KAIST are school identities assigned to board roles per game.
3. Only the per-question token is fixed school order: `Q[POSTECH, KAIST]`.
4. A unilateral quiz is mapped from correct school to current board role exactly once when determining `W+` / `B+`.
5. `.rpsc` Format 3 separates `[White]/[Black]` controller identity from `[WhiteTeam]/[BlackTeam]` school mapping and includes `[QOrder "POSTECH, KAIST"]`.
6. Format 2 is legacy `Q[White, Black]`; it is supported for load/migration but never emitted by new saves.
7. Native search remains purely White/Black. `teams` + `matchpk` perform the school-to-board-role conversion at the protocol boundary.

## Fixed game fixtures

- Game 1: `18-17 (12-11 + 6-6)`
- Game 2: `15-23 (11-11 + 4-12)`
- Game 3: `17-13 (11-11 + 6-2)`
- Game 4: `20-18 (14-12 + 6-6)`
- Game 5: `32-34 (20-20 + 12-14)`
- Game 6: `25-23 (11-11 + 14-12)`

Game 6 is explicitly `WhiteTeam=KAIST`, `BlackTeam=POSTECH`; its unilateral Q strings were canonicalized without changing item recipients, moves, captures, Reset events, score, or result.

## Verification gate

Release acceptance requires the native CTest suite, browser Format 3/migration suite, protocol school-role mapping, six-game replay, 120-round handbook notation validator, paired self-play smoke test, and a clean two-pass handbook build/render to pass.

Critical invariant: `WhiteTeam=KAIST`, `BlackTeam=POSTECH`, `Q[1,0]` => **Black receives the item**.
