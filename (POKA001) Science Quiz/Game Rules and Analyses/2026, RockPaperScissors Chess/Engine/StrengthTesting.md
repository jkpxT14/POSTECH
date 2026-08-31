# Strength Testing

Engine 0.9.0 is an integrated RPSC-decision release. The current evidence verifies correctness and preserves the board-search baseline; it is not an Elo estimate.

## Historical baseline

Draft 27 / Engine 0.7.0 had mixed small-sample results against 0.6.0, so no verified Elo claim was made. Draft 28 / Engine 0.8.0 introduced item-aware search and passed small paired no-item and preset-inventory smoke matches without demonstrating statistical superiority.

Those older samples remain development context, not a strength certificate for 0.9.0.

## 0.8.0 vs 0.9.0 no-item microbench

Initial position, native Release `bench` depth 4, two runs each in the same packaging environment:

- Engine 0.8.0: 196,968 nodes; 1,038 ms / 1,102 ms
- Engine 0.9.0: 196,968 nodes; 1,041 ms / 1,088 ms

Both versions selected the same best move. This is expected: Draft 29 does not deliberately retune the core no-item evaluator/search merely to manufacture a strength delta. The measurements support a no-obvious-regression conclusion only.

## What 0.9.0 changes for practical strength

The main strength-facing changes are decision quality and usable search breadth:

- first solo-correct order and item are searched jointly rather than greedily;
- item acquisition is no longer restricted to a nominal depth-2 probe;
- browser search reduces compound Roll branching before full paths are materialized;
- timed MultiPV retains Top-3 alternatives from the last completed root iteration instead of frequently collapsing to one line at the deadline.

These changes should be judged with RPSC-specific matches, not generic chess-style benchmarks.

## Required promotion tests

Future strength patches should be accepted only after separate controls:

1. no-item fixed-Quiz (`Q[0,0]`) control - isolates board-search regression;
2. preset-inventory fixed-Quiz control - isolates item use/conservation;
3. fixed Quiz scripts - tests item acquisition plus later item use with identical Quiz inputs for both compared Engines;
4. asymmetric inventories and Rotation/Step/Push tactical positions;
5. longer paired starts/colors before any statistical strength claim.

`Q[0,0]` remains an internal scientific control, not an Analysis Board game mode.
