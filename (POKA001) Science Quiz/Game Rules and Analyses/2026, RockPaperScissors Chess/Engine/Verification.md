# Verification — RPSC 0.20.0 / .rpsc Format 3

The release gate covers the shared rules and notation boundary, not only search code.

- Native Release build succeeds.
- Native regression suite: initial 161 legal paths, 145 exact successors, 84 reduced successors; perft 1/2/3 = 161 / 25,575 / 4,215,782; item-rich 2,146 / 1,459 / 568; MultiPV=3 remains distinct.
- Native endgame exact-rank regression passes.
- Protocol boundary regression: `teams W KAIST B POSTECH` + `matchpk 11 10 6` produces internal `quiz 10-11`.
- Browser regression validates Format 3, Format 2 migration, reverse school assignment, illegal unilateral item recipients, Format 3 round-trip, all six handbook games, interactive first-order selection, and three distinct recommendations.
- Handbook validator checks 6 games / 120 rounds and every unilateral `Q` -> `W+` / `B+` mapping against the game’s `WhiteTeam` / `BlackTeam` assignment.
- Critical invariant: `WhiteTeam=KAIST`, `BlackTeam=POSTECH`, `Q[1,0]` => `B+...`.
