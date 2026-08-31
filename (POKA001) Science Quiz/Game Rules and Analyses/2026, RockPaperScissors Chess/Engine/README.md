# RPSC Engine

`rpsc-engine` is the native classical search Engine for RockPaperScissors Chess.

Version: 0.9.0

## Scope

Quiz correctness is not an Engine action. The user supplies every Quiz Result. Once that result is known, the Engine analyzes the strategic RPSC choices that follow:

- first/second order when the first solo-correct team receives that choice;
- Push / Rotation / Step acquisition;
- item conservation versus use;
- Normal / Push / RoL / RoR / StS / StL board actions;
- the exact legal full Roll path;
- opponent replies and the continuation line.

The Engine is intentionally RPSC-specific. It is not a general board-game framework.

## Exact state and move model

- C++17; no ML, neural network, or NNUE
- exact 24-orientation cube state is authoritative for rules and notation
- six-state Gesture State is a derived search reduction
- captures, alive pieces, side to move, and both inventories are search state
- Push, Rotation, and Step are consumed by normal make/undo state transitions
- combat, score changes, and Reset are successor-state effects

Normal and item moves compete in one alpha-beta tree. There is no separate heuristic that first decides to spend an item and only then chooses a move.

## RPSC compound-move generation

The initial no-item position has 161 canonical full-path moves, 145 exact successors, and 84 reduced search successors. With one Push, Rotation, and Step available to each side, these become 1,472 / 1,019 / 427.

The Engine therefore keeps two generators:

1. `generate_legal_moves` is exhaustive and authoritative for legality, canonical paths, notation, exact successor checks, and perft.
2. the search generator merges equivalent partial Roll states identified by square, reduced cube state, remaining Rolls, previous direction, and item context before the full path tree expands.

Regression tests compare the complete reduced-successor set from the optimized generator against the exhaustive generator.

## Search

- iterative deepening PVS with aspiration windows
- clustered transposition table
- TT / tactical / killer / history / continuation / follow-up / countermove / capture-history ordering
- item action is part of learned move identity
- bounded score-event selective extensions
- tactical quiescence
- conservative LMR with RPSC scoring-pressure and defence safeguards
- reduced fail-high verification
- root MultiPV

Timed MultiPV in 0.9.0 publishes the best alternatives already scored by the last fully completed root iteration. A later timed-out iteration therefore does not erase Candidate 2/3. If time remains, alternatives can still be re-searched at the completed depth.

## Item acquisition

`chooseitem W` and `chooseitem B` compare Push, Rotation, and Step from the exact current board and inventories. Each hypothetical item is added to the chooser's inventory, White is set as the next Board Event's first mover, and normal RPSC search evaluates the resulting board potential.

The total node/time budget is divided among the three root alternatives while the transposition table is shared. Version 0.9.0 removes the former nominal depth-2 cap. Future Quiz Results are never generated.

## Initial solo-correct decision

The first solo-correct team simultaneously controls order and item acquisition. `chooseinitial` therefore evaluates all six combinations:

- First + Push / Rotation / Step
- Second + Push / Rotation / Step

The chooser's perspective is used to rank those six branches. This avoids a greedy `chooseorder` -> `chooseitem` decomposition that can miss interactions between move order and the acquired resource.

`chooseorder` remains available as a lower-level diagnostic command.

## Protocol examples

```text
rpsc
position startpos
chooseinitial movetime 10000
chooseitem W movetime 10000
items W 1 1 1
items B 1 1 1
go movetime 10000 multipv 3
perft 2
bench
quit
```

Search scores printed for ordinary board analysis use White's point of view. `chooseinitial` ranks by the solo-correct chooser's point of view. Item-choice output includes the candidate's board continuation.

## Analysis Board

The offline Analysis Board keeps every Quiz Result under user control. Human vs Human, Human vs Engine, and Engine vs Engine use the same original Quiz-driven game flow.

For a Human-controlled decision the Worker analyzes and recommends without committing the choice. For an Engine-controlled decision the same result is used automatically.

The Worker mirrors the C++ partial Roll-state search generator in Draft 29. The exhaustive browser move generator is retained separately for canonical-path/rule checks. Board, Item, and Initial Decision analysis all expose ranked alternatives and short continuation lines while preserving the existing board-first UI.

Normal analysis targets about 10 seconds. `Analyze` extends the current analysis toward about 20 seconds; decision analysis carries forward its completed candidate searches and the Worker TT instead of intentionally inventing a new Quiz scenario.

## Testing and strength

The Release regression binary keeps assertions active explicitly. Tests cover exact and reduced move counts, perft, all 24 exact orientations, Rotation/Roll inverses, exhaustive-vs-optimized successor equality, item actions, make/undo, notation, MultiPV, item acquisition, and six-way initial decision search.

Engine 0.9.0 does not claim a measured Elo gain. Its board-search core deliberately remains close to 0.8.0 while decision integration and browser search are improved. See `Verification.md` and `StrengthTesting.md`.
