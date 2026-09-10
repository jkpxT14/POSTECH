# Verification — Engine 0.19.0 Checkpoint

Ruleset: `2026-rpsc-rotation6-push-return`.

0.19.0 changes the exact execution hot path, not the game rules. The same 24-orientation cube model, six Rotation actions, post-Rotation move length, Push-return rule, Step rules, combat, Reset scheduling, score context, notation and final-ply semantics are retained.

## Asserted rule/successor baselines

| Position | Legal paths | Exact successors | Reduced successors |
|---|---:|---:|---:|
| Initial, no items | 161 | 145 | 84 |
| Initial, one of each item per side | 2,146 | 1,459 | 568 |

Initial exhaustive perft depths 1 / 2 / 3: **161 / 25,575 / 4,215,782**.

## 0.19.0 exact-fast changes

- Precomputed orthogonal step and adjacency bitmasks.
- Parent occupancy/enemy bitboards reused across path generation.
- Reduced-successor duplicate detection uses a collision-free compact signature for the state components that can change within one parent, avoiding repeated Position make/hash/undo at every endpoint.
- `SearchMove` carries exact final orientation and capture/reset metadata.
- Search applies already-generated legal moves through a dedicated fast path.
- Reset exhaustion is checked only for the side that actually lost a captured piece.
- Transposition-table indexing uses a power-of-two table mask.

## Release checks

A clean CMake Release build from the checkpoint source passes:

- `rpsc-engine-tests`
- `rpsc-search-regressions`

The checkpoint executable also completes the depth-4 initial benchmark with the same searched node count as its source build. Browser embedding is intentionally deferred to the next full-package synchronization; therefore this ZIP does not claim native/browser 0.19.0 equivalence yet.
