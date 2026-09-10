# Package Status — 0.19.0 Checkpoint

## Upstream lineage

- Repository: `jkpxT14/POSTECH`
- Package path: `(POKA001) Science Quiz/Game Rules and Analyses/2026, RockPaperScissors Chess/`
- Upstream native baseline: Engine 0.18.0
- Ruleset: `2026-rpsc-rotation6-push-return`
- Exact 24-orientation cube state remains authoritative.

## Checkpoint objective

The purpose of 0.19.0 is to leave a tested, clearly stronger native-engine checkpoint before further strength work. No rule approximation was introduced to obtain the speedup.

The selected candidate keeps the 0.18.0 search/evaluation semantics and improves the exact hot path: precomputed board-neighbor masks, occupancy/enemy bitboards during move generation, direct collision-free reduced-successor signatures inside one parent position, generated capture/reset metadata, a generated-move application fast path, a capture-side-only Reset check, and power-of-two transposition-table indexing.

## Strength gate

Primary checkpoint match: **50 paired scenarios / 100 games**, candidate 0.19.0 versus frozen GitHub 0.18.0 baseline. Each pair used the same 20-round quiz schedule and swapped only which engine played White/Black. Quiz schedules were generated with 70% symmetric results and 30% unilateral results (15% `Q[1,0]`, 15% `Q[0,1]`). Search budget was 50 ms per board move and 20 ms per item-choice probe.

Observed score: **56.0 / 100 = 56.0%** for Engine 0.19.0.

This is an observed checkpoint score, not a statistical guarantee that the true win expectation is at least 55%. It is sufficient for the requested interim checkpoint; larger holdouts remain part of subsequent development.

## Rule regression gate

The native Release suite passes both shipped tests. The inherited baselines remain:

- Initial: 161 legal paths, 145 exact successors, 84 reduced successors.
- Initial perft depths 1 / 2 / 3: 161 / 25,575 / 4,215,782.
- Initial with one Push, Rotation, Step per side: 2,146 legal paths, 1,459 exact successors, 568 reduced successors.

The tests cover the 24 exact orientations, six Rotation actions and their reduction, Push-return semantics, consecutive-Roll reversal prohibition, make/undo, notation, Reset, finite-horizon search, and MultiPV distinctness.

## Evaluation convention

The evaluator scores the current board, accumulated quiz/capture score, current inventories and remaining board horizon. For analysis from the current position, future quiz outcomes are treated as symmetric board-progress rounds (`Q[0,0]` or `Q[1,1]`), so it does not speculate about future unilateral item grants. Previously acquired items remain part of the position.

## Next development target

Continue from 0.19.0 rather than restarting from 0.18.0. The next priorities are larger independent paired holdouts, deeper item-choice policy, and then synchronization of the 0.19 engine into the current single-file Analysis Board with native/browser successor and MultiPV regression before the next full package release.
