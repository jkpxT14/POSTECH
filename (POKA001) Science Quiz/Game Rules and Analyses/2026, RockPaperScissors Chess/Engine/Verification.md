# Verification

Draft 33 / Engine 0.13.0 is verified as one RPSC handbook / Analysis Board / native Engine package. Rule correctness is a release blocker; search improvements are accepted only on top of the exact rule model.

## Native rule regression

Release assertions remain enabled. Initial controls remain:

- canonical full-path legal moves: 161
- exact successor positions: 145
- reduced strategic search successors: 84
- perft(1): 161
- perft(2): 25,575
- perft(3): 4,215,782

Initial position with both sides holding Push/Rotation/Step = 1/1/1 remains:

- canonical full-path legal moves: 1,472
- exact successor positions: 1,019
- reduced strategic search successors: 427
- all item-use actions present: Push, RoL, RoR, StS, StL

The suite checks the official dice-net anchor (`P-S-P-S` horizontally with Rock above/below the left-hand S), all 24 exact orientations, Roll/Rotation inverses, Rotation top-face invariance, exact-vs-reduced transition equivalence, Push/Rotation/Step Roll lengths, inventory consumption/undo, notation round trips, key restoration, exhaustive-vs-optimized successor equality, tactical-generator equality, MultiPV distinctness, Match Context make/undo, Item Choice non-mutation, and all six Initial Decision branches.

The retained Draft 32 evaluation regression checks that the symmetric initial Match Context evaluates to zero, each White item family has positive reserve value, at least two item families are distinguished by the current geometric action-diversity heuristic, and the mirrored Black inventory reverses the sign.

The twelve Basic/Push/Rotation/Step movement diagrams are also machine-checked against the same exact cube transform used by the Analysis Board: all legal no-immediate-backtrack Roll words are enumerated on an unbounded reference grid, with Push translation, Rotation preprocessing, and Step Short/Long handled according to their actual movement semantics. All twelve TeX endpoint Gesture sets match exactly. The DiceNet and InitialPosition sources independently match the official `P-S-P-S`/Rock anchor and W1/W2/W3/W4 = S/R/P/S at a1/c1/e1/g1 with the mirrored Black setup.

## Native build / protocol

Required clean smoke:

- identity `RPSC Engine 0.13`;
- `perft 2` = 25,575;
- `chooseinitial` returns six ranked legal Order+Item branches;
- `chooseitem W` returns three ranked legal item branches;
- item-rich `go ... multipv 3` returns three distinct recommendations and legal PVs;
- fixed depth-4 bench preserves the established best root;
- item-rich 10-second control completes depth 3 in the current verification environment;
- a consecutive 20-second item-rich control also completes depth 3 (3,898,369 nodes), so no stronger depth/optimality claim is made.

## Browser regression

The browser keeps an exhaustive canonical generator and a reduced Worker search. Required interaction checks include:

- exact Quiz buttons `Q[1, 1]`, `Q[1, 0]`, `Q[0, 1]`, `Q[0, 0]`;
- Quiz remains manual in Human vs Human, Human vs Engine, and Engine vs Engine;
- equal-result, solo-correct, first Order+Item, later Item Choice, board move, combat, and Reset phase transitions;
- Rotate Board cycles all four 90-degree CCW view states without changing canonical game state;
- Candidate preview animates item preprocessing and each Roll without mutating the Game Record until Confirm;
- normal Engine Analysis publishes Top 3 at completed iterations and targets about 10 seconds;
- Analyze continues the same decision toward about 20 total seconds, preserving the earlier completed result while deepening;
- Item Choice and Initial Decision can display screened/refined partial rankings before completion;
- no page/console errors.
- Format 2 save/load restores incomplete confirmed phases, the full Variation tree, and the current sub-line node;
- stale Format 2 metadata is rejected when the readable Main Line has been edited, with replayed body state authoritative over Score/Quiz/Captures headers;
- legacy Draft 32 flat `.rpsc` records remain loadable;
- unconfirmed move preview/draft frames are excluded from saved session state.

The Worker mirrors Draft 32's small geometric reach/item signal and root progressive-widening policy, and Draft 33 carries the previous completed root ranking across related deepening/Analyze searches. The native C++ implementation remains authoritative for engine-development controls.

## Notation and Match Context

Compact Quiz variants such as `Q[1,1]` are rejected in package checks. Confirmed Quiz points/current inventories are real Match Context. Future Quiz is not predicted; remaining Quiz is treated symmetrically (`Q[1, 1]` / `Q[0, 0]`) and creates no unearned item.

## Deferred rule clarifications

Draft 33 intentionally does not add timeout adjudication and does not change the current Push-followed-by-first-Roll return behavior. Those two rule points remain frozen until official confirmation is supplied.

## Packaging

The deliverable preserves the established `2026, RockPaperScissors Chess/` layout. CMake build directories, development backups, browser-test screenshots, and LaTeX auxiliary files are excluded from the ZIP.


## Draft 33 documentation audit

The continuity cache is keyed by `Position::search_key()`, the Engine's reduced RPSC search-state key; documentation therefore calls it the **Engine search key** rather than implying the exact physical-orientation key returned by `Position::key()`. The protocol exposes `newgame`; the C++ Engine API additionally exposes `clear_search()`. Both reset the TT and persistent ordering memory through `Search::clear_memory()`.
