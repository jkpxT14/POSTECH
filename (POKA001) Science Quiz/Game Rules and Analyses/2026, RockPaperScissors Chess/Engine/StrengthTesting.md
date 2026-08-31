# Strength Testing

Engine 0.10.0 is a RPSC-specific decision/search release. Current evidence supports correctness, stable board-search behavior, and stronger allocation of computation across item/order decisions; it is not an Elo estimate.

## Board-search control

The native no-item depth-4 `bench` remains a useful fixed control because Draft 30 intentionally does not manufacture a board-strength delta by retuning unrelated evaluation terms. In the current Release environment Engine 0.10.0 searches 196,968 nodes from the initial position and selects the established best root move.

Any future board-search patch should first demonstrate that it does not regress this control or the exact/perft suite.

## Decision-search development

Draft 29 divided a finite Item Choice budget equally among three independent probes and a finite Initial Decision budget equally among six probes. Draft 30 changes the finite-budget policy without removing any candidate:

- Item Choice screens all 3 candidates, then refines the ranked candidates with 25% / 18% / 12% of the total budget after the 45% screening stage.
- Initial Decision screens all 6 candidates, then refines the top 3 with 24% / 16% / 12% after the 48% screening stage.
- the transposition table is shared throughout each decision;
- fixed-depth diagnostics continue to compare every candidate at equal requested depth.

A 10-second native `chooseinitial` smoke reached depth 3 on the strongest branches and produced legal continuations containing actual item use. This is evidence that the available decision budget is being spent on deeper plausible alternatives, not a statistical game-strength claim.

## Rejected strength patch

A broader quiet-threat quiescence experiment was tested during Draft 30 work. In the initial no-item depth-4 control it increased the search from roughly 197k nodes / about 1.2 s to roughly 380k nodes / about 5.5 s in the development environment without sufficient tactical/playing evidence to justify the cost. It was removed rather than shipped on theoretical appeal alone.

The same rule applies to future RPSC features: keep them only after rule regression and relevant strength evidence.

## Promotion tests for future strength patches

1. fixed `Q[0, 0]` no-item control to isolate core board-search changes;
2. fixed `Q[0, 0]` with preset/asymmetric inventories to isolate item use and conservation;
3. fixed Quiz scripts, identical for compared engines, to test item acquisition plus later use without asking either engine to predict Quiz Results;
4. Push-, Rotation-, Step-, horizon-, Reset-, and late-round tactical regression positions;
5. paired starts/order assignments and larger samples before any Elo/strength claim.

The long-term target is not generic chess-engine strength. It is stronger RPSC decisions under the game's exact Roll-path, orientation, item-economy, combat, and Quiz-driven phase structure.
