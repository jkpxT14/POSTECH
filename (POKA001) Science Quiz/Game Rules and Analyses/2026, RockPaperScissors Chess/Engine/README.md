# RPSC Engine

`rpsc-engine` is the native classical search Engine for RockPaperScissors Chess.

Version: 0.11.0

## Scope

Quiz correctness is never an Engine action. The user supplies every Quiz Result. Confirmed Quiz points and the item inventory they created are part of the current Match Context; future Quiz Results are not predicted. Once that result is known, the Engine analyzes the strategic RPSC choices that follow:

- first/second order together with item acquisition at the first solo-correct decision;
- Push / Rotation / Step acquisition later in the game;
- item conservation versus use;
- Normal / Push / RoL / RoR / StS / StL board actions;
- the exact legal full Roll path;
- opponent replies and continuation lines.

The Engine is intentionally RPSC-specific rather than a general board-game framework.

## Rule invariants

The exact 24-orientation cube state is authoritative. In particular, Rotation is an in-place 90-degree rotation about the board-vertical axis with the top face fixed. It changes Wrist Direction / exact orientation while preserving square, Top Gesture, and Base Roll Length. RoL and RoR remain distinct exact actions even when a reduced Gesture-State transition is strategically equivalent.

Push translates one orthogonal square without rolling or changing orientation, then performs the original base Roll count. Step Short/Long changes only the Roll count by -1/+1. At most one item is consumed by a move.

## Exact state and move model

- C++17; no ML, neural network, or NNUE
- exact 24-orientation cube state for rules and notation
- six-state Gesture State only as a derived search reduction
- captures, alive pieces, side to move, and both inventories in the search state/key
- item consumption handled by normal make/undo transitions
- combat, score-changing captures, and Reset handled as successor-state effects

Normal and item moves compete in one alpha-beta tree. There is no rule such as “use an item first” or a fixed Push/Rotation/Step hierarchy.

## RPSC compound-move generation

The initial no-item position has 161 canonical full-path moves, 145 exact successors, and 84 reduced search successors. With one Push, Rotation, and Step available to each side, these become 1,472 / 1,019 / 427.

Two generators are retained deliberately:

1. `generate_legal_moves` is exhaustive and authoritative for legality, canonical full paths, notation, exact successor checks, and perft.
2. the search generator merges equivalent partial Roll states before the full compound path tree expands, using square, reduced cube state, remaining Rolls, previous direction, and item context while retaining a representative legal full path.

Regression tests compare the complete reduced-successor set from the optimized generator with the exhaustive generator.

## Search

- iterative deepening PVS with aspiration windows
- clustered transposition table
- TT / tactical / killer / history / continuation / follow-up / countermove / capture-history ordering
- item action included in learned move identity
- bounded score-event selective extensions
- tactical quiescence
- conservative LMR with RPSC scoring-pressure/defence safeguards and fail-high verification
- root MultiPV
- completed iterations remain authoritative at a time cutoff

Broad quiet-threat quiescence was experimentally rejected during Draft 30 work because it increased search cost substantially without enough evidence to justify inclusion. RPSC-specific selective search remains deliberately bounded.

## Decision search

### Item acquisition

`chooseitem W` and `chooseitem B` compare Push, Rotation, and Step from the exact current board/inventories. Future Quiz Results are never generated. With a finite total time/node budget, the Draft 30 policy retained in Draft 31 first screens all three candidates, ranks them, then spends the remaining budget preferentially on the stronger candidates while sharing the TT. Fixed-depth mode searches all candidates at equal requested depth.

The continuation for each branch is formatted from the correct hypothetical inventory state, so item-using PVs are legal and reproducible.

### First solo-correct decision

`chooseinitial` evaluates all six combinations:

- First + Push / Rotation / Step
- Second + Push / Rotation / Step

All six are screened under one total decision budget, then the top candidates receive deeper refinement. The chooser's perspective ranks the alternatives. This avoids a greedy `chooseorder` followed by `chooseitem` decomposition while allocating more computation to plausible best decisions.

`chooseorder` remains available only as a lower-level diagnostic command.

## Match Context and symmetric future Quiz

`match <whiteQuiz> <blackQuiz> <remainingBoardPlies>` supplies the already-confirmed Quiz score and the maximum remaining board-move plies under symmetric future Quiz continuation. `-1` keeps the native diagnostic search unbounded by match length. The official total score is `Quiz + 2 * Captures`. Once `remainingBoardPlies` reaches zero, unused positional/item potential no longer counts; only the official score and, on an exact score tie, the Quiz-count tie-break remain.

Future Quiz Results are never sampled. Conceptually the continuation is equivalent to all remaining Quiz events being `Q[1, 1]` or `Q[0, 0]`: both preserve the score difference, create no new item, and lead to First-then-Second board play.

## Protocol examples

```text
rpsc
position startpos
match 7 5 12
chooseinitial movetime 10000
chooseitem W movetime 10000
items W 1 1 1
items B 1 1 1
go movetime 10000 multipv 3
perft 2
bench
quit
```

Ordinary board-analysis scores are displayed from White's point of view. Decision output is ranked from the chooser's point of view and includes continuation PVs.

## Analysis Board

Human vs Human, Human vs Engine, and Engine vs Engine all keep Quiz Results under user control. During a Human-controlled RPSC choice, the Worker recommends without committing it; during an Engine-controlled choice, the same analysis result may be applied automatically.

The Worker mirrors the native partial Roll-state search principle and retains an exhaustive canonical generator separately. Board Move, Item Choice, and Initial Order+Item decisions expose ranked alternatives and short continuations inside the existing board-first interface. Item usage appears directly inside candidate moves/PVs.

Normal board analysis targets about 10 seconds. `Analyze` extends the same current position toward about 20 seconds while reusing the completed result and Worker TT where applicable. The browser passes confirmed Quiz scores plus the number of remaining symmetric board plies into search; no future solo-correct item gain is generated.

## Testing and strength

Release tests explicitly keep assertions active. The regression suite covers exact/reduced move counts, perft, all 24 exact orientations, Rotation/Roll inverses, reduced-orientation transition equivalence, exhaustive-vs-optimized successor equality, item Roll lengths/consumption, make/undo, notation, MultiPV, Item Choice, and six-way Initial Decision search.

Engine 0.11.0 is not presented as a measured Elo breakthrough. Draft 31 adds official-net anchoring, confirmed Match Context, finite remaining-ply evaluation, endgame item-reserve tapering, and less aggressive deep LMR on quiet item actions while preserving the established RPSC search core. See `Verification.md` and `StrengthTesting.md`.
