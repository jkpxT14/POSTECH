# RPSC Engine

Version: **0.14.0**

`rpsc-engine` is the native classical-search Engine for RockPaperScissors Chess. Quiz correctness is external input. Confirmed Quiz points, captures, remaining board plies, move order, and inventories form the Match Context; future Quiz Results are not predicted.

## Rule model

- C++17; no ML / NNUE.
- Exact 24-orientation cube state is authoritative for rules and notation.
- Gesture State is a six-state derived search reduction.
- Rotation is an in-place 90° rotation with square and top face fixed; `RoL` and `RoR` remain distinct exact actions.
- Push translates one orthogonal square without rolling or changing orientation, then performs the original base number of Rolls. The first Roll may return to the pre-Push square.
- Step Short/Long changes only the Roll count by -1/+1.
- At most one item is consumed by a move.
- Timeout/random adjudication is intentionally not implemented.

## Move generation

Two generators remain deliberately separate:

1. `generate_legal_moves` is exhaustive and authoritative for legality, canonical full paths, exact-successor checks, notation, and perft.
2. the search generator merges equivalent partial Roll states by square, reduced cube state, remaining Rolls, previous direction, and item context, then deduplicates final reduced search successors.

Draft 36 keeps fixed scratch storage for partial/final deduplication and additionally precomputes one-square board transitions used repeatedly in both generators.

Reference counts:

- initial no-item: 161 canonical moves / 145 exact successors / 84 reduced search successors;
- both sides `Pu/Ro/St = 1/1/1`: 1,472 / 1,019 / 427.

## Evaluation

Official score dominates. Confirmed Quiz is one score unit and a capture is two. Small horizon terms cover surviving pieces, occupancy mobility, obstacle-free orientation/square reach, and marginal item action diversity. There is no fixed `Push > Rotation > Step` hierarchy.

At zero remaining board plies, only official score and the Quiz tie-break remain.

## Search

- iterative-deepening PVS with aspiration;
- clustered transposition table;
- TT / tactical / killer / history / continuation / follow-up / countermove / capture-history ordering;
- tactical quiescence;
- bounded score-event selective extensions;
- conservative inner LMR with RPSC capture-pressure / defensive safeguards and fail-high verification;
- root progressive widening by Normal / Push / Rotation / Step family;
- completed iteration is authoritative at time cutoff;
- root MultiPV from completed root scores;
- persistent ordering memory across related searches.

### 0.14 continuation change

For an unchanged `Position::search_key()`, the Engine now stores both the completed root ranking and the last completed nominal depth. A later timed `go` reuses that ranking and begins from the next unfinished depth. This is especially useful for the Analysis Board’s 10 s → 20 s continuation: the second budget is not spent rebuilding completed shallow iterations.

`newgame` and `clear_search()` clear TT and persistent search memory.

## Decision search

`chooseitem W/B` compares Push / Rotation / Step under one decision budget. `chooseinitial` compares all six First/Second × Push/Rotation/Step combinations. Screening and refinement share the same Engine search object and TT.

## Protocol examples

```text
rpsc
newgame
items W 1 1 1
items B 1 1 1
match 7 5 18
go movetime 10000 multipv 3
chooseitem W movetime 10000
chooseinitial movetime 10000
```

See `Verification.md` and `StrengthTesting.md`.
