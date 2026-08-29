# Verification

Draft 21 was built and tested from the same RPSC rule model used by the handbook and Analysis Board.

## Deterministic move-generation checks

From the initial position, with White to move and no item:

- canonical full-path legal moves at depth 1: `161`
- distinct exact successor positions with the 24-orientation state retained: `145`
- distinct no-item search successors after the derived Gesture-State reduction: `84`
- canonical full-path `perft(2)`: `25575`
- canonical full-path `perft(3)`: `4215782`

`perft` counts canonical paths rather than deduplicated search states. Exact physical orientation remains authoritative for rules, notation, and replay. The reduced search key is used only by the initial no-item engine, where future Roll and combat behavior depends on square, side to move, capture score, and derived Gesture State.

## Build and search checks

The package was compiled as a C++17 Release build with the included warning flags. The build completed without compiler warnings, and `rpsc-engine-tests` passed. The tests cover the 24-orientation table, inverse Rolls, fourfold Rotation, initial move counts, exact and reduced successor counts, `perft(2)`, notation parsing, best-move legality, and a depth-4 search smoke test.

On the package build machine, the deterministic depth-4 benchmark searched `34346` nodes. A fresh `go depth 5` search completed at depth 5 with `213893` nodes, and an 8-second time search completed depth 6. Timing and NPS are machine-dependent and are not a strength rating.

The embedded Analysis Board worker was also syntax-checked and exercised directly from the initial position. It produced the expected `84` reduced root moves and completed depth 4 in a short smoke search; with a longer time budget it completed depth 5.

Human-superior strength remains an empirical target to be established through regression matches, self-play, and human testing rather than asserted from search depth alone.
