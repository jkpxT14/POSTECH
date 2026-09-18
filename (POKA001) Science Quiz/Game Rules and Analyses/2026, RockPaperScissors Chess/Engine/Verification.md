# Verification - RPSC 0.24.1

## Browser engine strength gate

The Analysis Board keeps the 0.24 primary-biased MultiPV-3 JavaScript search core selected against the previous 0.23 baseline. The exact reference core is stored in `reference/rpsc_engine_0.24.0.js`; 0.24.1 is an integration/packaging revision and does not weaken that worker core.

Recorded paired accelerated regression:

- games: **56**
- wins: **34**
- losses: **21**
- draws: **1**
- raw win rate: **60.7%**
- score rate (draw = 0.5): **61.6%**

The records are in `reference/results/paired_regression_56.jsonl`. Controls are 30/50/80/150 ms per board move with paired role reversal. This is a candidate-selection/regression gate, not a guarantee that every machine reproduces the same percentage at literal 10-second wall-clock control.

## Analysis time policy

Default 10 s:

- 0-2 s broad root search
- 2-7 s primary-biased championship search
- 7-9 s Top-3 stabilization
- 9-10 s final #1 verification

Analyze continuation to 20 s cumulative:

- 10-11.5 s challenger refresh
- 11.5-17.5 s deep primary search
- 17.5-19 s deep Top-3 stabilization
- 19-20 s final verification

## Native engine regression gate

`ctest` checks:

- 24 exact cube orientations and the RPS cycle;
- legal initial position and move generation;
- Push -> first Roll return legality;
- W/B role separation;
- three distinct MultiPV recommendations;
- item and initial order/item choice APIs;
- one-ply exact search correctness;
- `teams W KAIST B POSTECH` + `matchpk 11 10 6` -> internal `quiz 10-11`;
- browser/analyzer contract: Top 3, 10 -> 20 s continuation, fixed `Q[POSTECH, KAIST]`, reverse school-role mapping, manual resignation record round-trip, no auto-resign in the embedded search worker, and embedded-worker identity;
- all 11 handbook games, including the 11-round resignation game for unilateral-item P/K -> W/B mapping.

The source package deliberately excludes compiled executables, build directories, logs, `.synctex.gz`, and historical patch artifacts.
