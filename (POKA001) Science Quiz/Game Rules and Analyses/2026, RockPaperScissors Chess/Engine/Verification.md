# Verification

Draft 30 / Engine 0.10.0 is verified as one RPSC handbook / Analysis Board / native Engine package. The verification goal is twofold: exact rule agreement and functional search/UI behavior.

## Native rule regression

The Release CTest binary explicitly undefines `NDEBUG` before `<cassert>`, so regression assertions remain active in Release verification.

Initial no-item checks:

- canonical full-path legal moves: 161
- exact successor positions: 145
- reduced strategic search successors: 84
- perft(1): 161
- perft(2): 25,575
- perft(3): 4,215,782

Initial position with both sides holding Push/Rotation/Step = 1/1/1:

- canonical full-path legal moves: 1,472
- exact successor positions: 1,019
- reduced strategic search successors: 427
- all five item-use actions appear: Push, RoL, RoR, StS, StL

The suite also checks:

- all 24 exact cube orientations;
- Rotation keeps the top Gesture fixed;
- RoL/RoR inverse behavior and fourfold Rotation identity;
- exact orientations in the same reduced Gesture State have identical reduced Roll/Rotation transitions;
- RoL and RoR map to the same reduced Rotation state while remaining distinct exact actions;
- North/South and East/West Roll inverses;
- Push uses the base Roll count, Rotation uses the base Roll count, StS uses base-1, and StL uses base+1;
- each item action consumes only its correct inventory bucket and undo restores it;
- notation round trips;
- exact/search-key restoration after make/undo;
- exhaustive-vs-optimized reduced-successor equality in no-item, item-rich, and deterministic playout positions;
- tactical-generator equality against score-changing successors from the full search generator;
- distinct legal root MultiPV lines;
- Item Choice non-mutation;
- all six First/Second + Push/Rotation/Step initial branches.

## Native build and protocol smoke

The clean Release build and CTest are run before packaging. Protocol smoke includes:

- identity `RPSC Engine 0.10`;
- `perft 2` = 25,575;
- `chooseinitial` returning six ranked Order+Item candidates with legal continuation PVs;
- `chooseitem W` returning three ranked item candidates with legal continuation PVs;
- item-rich `go ... multipv 3` returning three distinct board recommendations.

Draft 30 additionally verifies that decision PVs are formatted from the correct hypothetical inventory/order state. This prevents an item-using hypothetical continuation from being parsed against an inventory that has not actually received that item.

## Browser game-flow regression

The Analysis Board keeps an exhaustive canonical generator and a separate optimized search generator. The optimized browser generator reproduces the native initial search counts: 84 reduced successors without items and 427 with both sides holding 1/1/1.

Browser interaction smoke is performed with a real Chromium page context, not syntax checking alone. Required behaviors include:

- initial Quiz buttons visible with exact labels `[1, 1]`, `[1, 0]`, `[0, 1]`, `[0, 0]`;
- `Q[1, 0]` / `Q[0, 1]` entering the solo-correct order/item path when appropriate;
- `Q[1, 1]` / `Q[0, 0]` entering the equal-result board-move path with first/second order resolution when needed;
- Human vs Engine and Engine vs Engine remaining in QUIZ until the user clicks a Quiz Result;
- Engine-controlled initial decision returning to the next QUIZ after Order+Item is applied;
- Engine-controlled Item Choice returning to the next QUIZ after acquisition;
- no page errors during these flows.

A separate analysis smoke verifies item-rich background analysis at about 10 seconds and `Analyze` toward about 20 seconds, with Top-3 lines and item use appearing inside continuations.

## Notation and UI invariants

Canonical Quiz notation uses a space after the comma: `Q[1, 1]`, `Q[1, 0]`, `Q[0, 1]`, `Q[0, 0]`. Compact variants are rejected during package checks.

Quiz input remains user-controlled in all three play modes. Fixed `Q[0, 0]` is only an engine-development control condition and is not a user-facing play mode.

## Packaging

The deliverable preserves the established `2026, RockPaperScissors Chess/` directory layout. CMake build products, browser-test artifacts, and LaTeX auxiliary files are excluded from the ZIP.
