# Strength Testing

Strength testing is separated into historical Draft 27 evidence and Draft 28 item-aware controls. The numbers below are development samples, not Elo estimates.

## Historical Draft 27: Engine 0.7.0 vs 0.6.0

The documented Draft 27 board-only `Q[0, 0]` proxy used 40 plies / 20 rounds, deterministic legal opening prefixes, and paired games with candidate color reversed.

### 6,000-node exploratory set

48 distinct opening prefixes, 96 games total:

- 0.7.0 wins: 47
- draws: 12
- 0.7.0 losses: 37
- score: 55.21%
- mean capture differential: +0.1875 capture/game

### 12,000-node confirmation sample

12 paired opening prefixes, 24 games total:

- 0.7.0 wins: 4
- draws: 11
- 0.7.0 losses: 9
- score: 39.58%
- mean capture differential: -0.3333 capture/game

The two budgets pointed in different directions, so Draft 27 did not claim a verified Elo gain.

## Draft 28: Engine 0.8.0 vs 0.7.0 controls

Draft 28 changes item search substantially, so the first question is whether the underlying no-item board engine was accidentally damaged.

### No-item microbench

Initial position, native `bench` depth 4, three runs each on the same environment:

- Engine 0.7.0: 196,974 nodes; 1,075 / 1,044 / 1,074 ms
- Engine 0.8.0: 196,968 nodes; 1,023 / 1,087 / 1,095 ms

Both versions chose the same initial best move in this microbench. The timings overlap closely enough that this sample does not support a speed claim in either direction.

### Item-rich depth-2 microbench

Initial position with `items W 1 1 1` and `items B 1 1 1`, three runs each:

- Engine 0.7.0: 66,594 nodes; approximately 111k-115k NPS
- Engine 0.8.0: 72,927 nodes; approximately 110k-112k NPS

Engine 0.8.0 searches more nodes at this shallow item-rich depth because item ordering/history/LMR semantics changed. NPS is similar in this small sample; this is not presented as a speedup.

### Paired 6,000-node smoke matches

A small deterministic opening-prefix harness was completed as a package smoke test. Each opening prefix was played twice with the candidate color reversed; games were capped at 40 plies.

No-item control, 4 paired prefixes / 8 games:

- 0.8.0 wins: 4
- draws: 0
- 0.8.0 losses: 4
- score: 50.00%
- mean capture differential: 0.0000

Preset-inventory control, 4 paired prefixes / 8 games using varied asymmetric/symmetric item inventories:

- 0.8.0 wins: 4
- draws: 0
- 0.8.0 losses: 4
- score: 50.00%
- mean capture differential: 0.0000

These 8-game samples are far too small to measure playing strength. Their purpose is to confirm that the 0.8.0 binary can be paired against the frozen 0.7.0 baseline in both no-item and item-rich conditions without protocol/rule failures.

## Interpretation and next tests

Engine 0.8.0 is accepted in Draft 28 primarily as an item-aware architecture/correctness release, not because the small match samples prove an Elo gain. Future search changes should be tested one idea at a time, following the same discipline used in mature engine projects: freeze a baseline, use paired starts/colors, keep no-item controls separate from item-rich tests, promote only surviving changes to larger/longer samples, and eventually use a sequential statistical test once the custom RPSC harness is mature enough.

The planned test matrix is:

1. no-item `Q[0, 0]` control - isolates pure board-search regression;
2. preset-inventory `Q[0, 0]` control - isolates item-use/conservation search without item acquisition;
3. fixed scripted Quiz Result sequences - tests item acquisition choice plus later item use while keeping Quiz outcomes identical between compared Engines.

`Q[0, 0]` therefore remains valuable as an internal development control even though it is no longer a user-facing Analysis Board mode.
