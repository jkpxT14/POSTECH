# Verification — Engine 0.24.0

## Strength gate

The exact 0.24.0 JavaScript search core embedded in `../RockPaperScissorsChess.html` is also stored as `reference/rpsc_engine_0.24.0.js`.
Its SHA-256 is:

`7724152e4c2f36eebe74d5fde7db8dbf8420462f0632862ef76ebdeb1de32271`

The embedded worker and the reference file were checked to be byte-for-byte identical.

It was compared against the 0.23.0 baseline core in `reference/baseline_0.23.0.js` using paired seeds with White/Black role reversal.

- Games: **56**
- 0.24.0: **34 wins, 21 losses, 1 draw**
- Raw win rate: **60.7%**
- Score rate (draw = 0.5): **61.6%**

The exact pair records are in `reference/results/paired_regression_56.jsonl`.
The set uses accelerated 30/50/80/150 ms board-move controls as a regression and candidate-selection gate; it is not a claim that every machine will reproduce 60.7% at literal 10-second wall-clock control.

## Time policy in the Analysis Board

MultiPV remains **3** throughout.

Default 10-second analysis:

- 0.0–2.0 s: broad root search
- 2.0–7.0 s: primary-biased championship search
- 7.0–9.0 s: Top-3 stabilization
- 9.0–10.0 s: final #1 verification

Analyze continuation, additional 10 seconds (10 -> 20 s cumulative):

- 10.0–11.5 s: challenger refresh
- 11.5–17.5 s: deep primary search
- 17.5–19.0 s: deep Top-3 stabilization
- 19.0–20.0 s: final verification

Final verification uses the best-vs-second gap: clear gaps focus the entire final window on #1; close races split #1/#2 approximately 60/40, and very close races 50/50.

## Correctness checks

The native `engine_tests` target checks:

- exact 24 cube orientations;
- RPS cycle;
- initial 4-v-4 setup;
- legal move generation;
- Push -> first Roll return legality;
- W/B role separation;
- MultiPV search smoke test.

The Analysis Board retains the Format 3 tests that enforce `Q[POSTECH, KAIST]` ordering even when KAIST is White and POSTECH is Black.
