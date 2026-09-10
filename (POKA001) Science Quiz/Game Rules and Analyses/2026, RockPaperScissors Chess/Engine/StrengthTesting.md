# Strength Testing — Engine 0.19.0 Checkpoint

## Baseline

Frozen GitHub native Engine 0.18.0, ruleset `2026-rpsc-rotation6-push-return`.

## Paired match design

One scenario defines a deterministic 20-round Quiz Result sequence. A pair consists of:

1. Engine 0.19.0 as White, Engine 0.18.0 as Black.
2. Engine 0.18.0 as White, Engine 0.19.0 as Black.

The **same Quiz Result sequence** is used in both games. Thus White/Black move-order advantage and unilateral `Q[1,0]` / `Q[0,1]` item opportunities are each experienced once by each engine. The sequence generator uses 35% `Q[1,1]`, 35% `Q[0,0]`, 15% `Q[1,0]`, and 15% `Q[0,1]`.

Primary checkpoint run:

- 50 pairs / 100 games.
- 50 ms per normal board search.
- 20 ms per item-choice search.
- Candidate points: 56.0.
- Baseline points: 44.0.
- Candidate score: **56.0%**.

The full local run log used to select this checkpoint is included as `tools/checkpoint-match.log`.

The score is an empirical development checkpoint, not a confidence-bound claim. Future releases should add independent larger paired holdouts rather than reusing this set for tuning.
