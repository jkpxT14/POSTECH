# Strength Testing

Engine 0.12.0 is an RPSC-specific search/evaluation release. The evidence below is intended to measure practical search progress and correctness; it is not an Elo estimate.

## Draft 31 baseline vs Draft 32

The fixed no-item depth-4 control was repeated seven times for each build in the same Release environment.

- Engine 0.11.0: 196,968 nodes; median 1,042 ms; established best root `W1: a1-a2-a3-b3`.
- Engine 0.12.0 candidate: about 212k nodes; median 858 ms; the established best root is preserved.

The node count changes because Draft 32 also changes the leaf ordering/evaluation signal. The useful measurement is that the fixed-depth control is materially faster while all exact/perft regressions still pass. This does not by itself prove game strength.

The more relevant item-rich timed control is the initial position with both sides holding `Pu/Ro/St = 1/1/1`, MultiPV 3:

- Engine 0.11.0, 10 s: last completed iteration depth 2.
- Engine 0.12.0, 10 s: last completed iteration depth 3 in the current verification environment.

This is the main practical reason for keeping the Draft 32 search-allocation changes: the same analysis budget reaches one additional completed nominal depth in a position whose reduced root has 427 successors.

## Search changes retained in Draft 32

1. **Allocation-free search-generation scratch.** Partial Roll-state marks and the reduced final-successor dedup table are reused per search thread. The exhaustive canonical generator remains unchanged.
2. **TT move before full materialization.** When a legal TT move is available but the stored bound cannot already cut, it is searched before generating/sorting the full compound move set. A cutoff avoids that work completely.
3. **Root progressive widening.** At depth 3+, each action family receives a full-depth quota before late quiet candidates are searched one ply shallower. Any candidate that challenges alpha is verified at full depth. Captures are not root-reduced; Push/Rotation/Step are represented as distinct families rather than hidden behind one generic item flag.
4. **RPSC geometric leaf signal.** A precomputed empty-board reduced reach profile gives a small orientation/square mobility term and a small marginal action-diversity term for each available item family. The official score remains dominant, and actual item use is still decided by alpha-beta continuation rather than a fixed `Pu > Ro > St` rule.
5. **Existing selective safeguards remain.** Score-changing captures receive bounded extension; broad quiet-threat quiescence remains rejected; the deepest inner LMR remains restricted to non-item quiet actions.

## Decision search

Item Choice still compares all `Push / Rotation / Step` branches. Initial Decision still compares all six `First/Second x Push/Rotation/Step` branches. Every candidate is screened, the strongest candidates receive refinement, and the TT is shared. Draft 32 benefits from the faster board search and item-aware leaf signal but does not falsely describe these probes as one monolithic root tree.

## Promotion controls

Any later strength patch should be tested against at least:

1. fixed no-item symmetric (`Q[0, 0]`) board search;
2. fixed symmetric positions with preset item inventories;
3. asymmetric inventories and confirmed Match Context;
4. fixed Quiz scripts, identical for compared engines;
5. Push-, Rotation-, Step-, capture/recapture-, Reset-, last-piece-, and late-round tactical positions;
6. paired self-play/order assignments before any Elo-style claim.

The target is not generic chess-engine strength. It is stronger decisions under RPSC's exact Roll paths, cube orientation, item economy, combat/Reset, finite rounds, and Quiz-driven action phases.
