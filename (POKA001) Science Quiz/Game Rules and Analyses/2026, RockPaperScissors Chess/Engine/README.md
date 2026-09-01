# RPSC Engine

`rpsc-engine` is the native classical search Engine for RockPaperScissors Chess.

Version: 0.13.0

## Scope

Quiz correctness is never an Engine action. The user supplies every Quiz Result. Confirmed Quiz points, captures, order, remaining main-game board plies, and the inventories created by those Quiz Results form the current Match Context. Future Quiz Results are not predicted.

Once the current Quiz state is known, the Engine analyzes:

- First/Second + item acquisition at the first solo-correct decision;
- later Push / Rotation / Step acquisition;
- item conservation versus use;
- Normal / Push / RoL / RoR / StS / StL board actions;
- exact legal full Roll paths;
- combat, Reset, opponent replies, and continuation lines.

The Engine is RPSC-specific rather than a generic board-game framework.

## Rule invariants

The exact 24-orientation cube state is authoritative. The official dice-net is horizontal `P-S-P-S` with Rock immediately above and below the left-hand `S`, giving S/S, R/R, P/P opposite pairs.

Rotation is an in-place 90-degree rotation about the board-vertical axis with square and top face fixed. It preserves Top Gesture and Base Roll Length and changes Wrist Direction / exact orientation. RoL and RoR remain distinct exact actions even when the six-state Gesture-State reduction maps them to the same reduced operation.

Push translates one orthogonal square without rolling or changing orientation, then uses the original base Roll count. Step Short/Long changes only the Roll count by -1/+1. At most one item is consumed by a move.

## State and move generation

- C++17; no ML, neural network, or NNUE
- exact 24-orientation state for rules and notation
- six-state Gesture State only as a derived search reduction
- captures, Quiz, remaining board plies, side to move, and both inventories in the search state/key
- item consumption via normal make/undo
- combat/Reset as successor-state effects

The initial no-item position has 161 canonical full-path moves, 145 exact successors, and 84 reduced search successors. With one Push, Rotation, and Step available to each side these are 1,472 / 1,019 / 427.

Two generators are deliberately retained:

1. `generate_legal_moves` is exhaustive and authoritative for legality, canonical full paths, notation, exact successor checks, and perft.
2. the search generator merges equivalent partial Roll states using square, reduced cube state, remaining Rolls, previous direction, and item context while retaining a representative legal full path.

Draft 32 introduced thread-local fixed scratch storage for the partial-state marks and reduced final-successor dedup table, removing repeated large temporary allocations from the hot search path.

## Evaluation

Official score dominates the evaluation: confirmed Quiz is one score unit and each capture is two. At zero remaining board plies, only the official score and Quiz-count tie-break remain.

Before that terminal point, small positional terms estimate alive-piece balance and mobility. Draft 32 introduced an RPSC-specific geometric reach table: for every square/exact orientation it precomputes obstacle-free reduced outcomes for Normal play and the additional outcomes enabled by Push, Rotation, and Step. This supplies two deliberately small horizon signals:

- square/orientation Roll reach for each alive piece;
- marginal action diversity of each item family currently held.

This is not a fixed item hierarchy. The actual alpha-beta tree still decides whether an item should be used now, saved, or acquired; the leaf signal only prevents all unseen Pu/Ro/St resources from looking identical at the horizon.

## Search

- iterative deepening PVS with aspiration windows
- clustered transposition table
- TT / tactical / killer / history / continuation / follow-up / countermove / capture-history ordering
- item action included in learned move identity
- bounded score-event selective extensions
- tactical quiescence
- conservative inner LMR with RPSC scoring-pressure/defence safeguards and fail-high verification
- root MultiPV from completed iterations
- completed iteration remains authoritative at a time cutoff

Draft 32 introduced two search-allocation changes aimed at RPSC's large compound branching:

1. a legal TT move is searched before full compound-move materialization when the stored bound cannot already cut; a TT cutoff can therefore skip path generation/sorting entirely;
2. root progressive widening / verification LMR at depth 3+: Normal, Push, Rotation, and Step each receive a full-depth quota, then late quiet candidates may be searched one ply shallower. Any candidate that challenges alpha is re-searched at full depth. Captures are not root-reduced.

Broad quiet-threat quiescence remains rejected because prior experiments roughly doubled search work without sufficient playing evidence.

Draft 33 / Engine 0.13 adds search continuity on top of that rule/search base. History, capture-history, continuation/follow-up, and countermove tables live with the Engine search object instead of being recreated for every `go`, Item Choice, or Initial Decision probe. For an unchanged Engine search key (`Position::search_key()`), the last completed root ranking is also cached and used to seed the next root ordering. `newgame` and the Engine API's `clear_search()` reset this learned search memory. These are ordering/allocation aids only; they do not alter legality or evaluation semantics.

## Decision search

### Item acquisition

`chooseitem W/B` compares Push, Rotation, and Step from the exact current state. Every candidate is screened under one decision budget, then stronger alternatives receive refinement. The TT is shared. Fixed-depth diagnostics search all candidates to equal requested depth.

### First solo-correct decision

`chooseinitial` compares all six `First/Second x Push/Rotation/Step` combinations. Order and item are therefore evaluated jointly rather than greedily decomposed.

These finite-budget probes share the same RPSC board search but are not described as a false single monolithic root tree.

## Match Context and future Quiz

`match <whiteQuiz> <blackQuiz> <remainingBoardPlies>` supplies confirmed Quiz score and remaining symmetric board plies. Future Quiz is never sampled. The continuation is equivalent to all remaining Quiz being `Q[1, 1]` or `Q[0, 0]`: score difference is unchanged, no new item is created, and First-then-Second board play continues.

## Analysis Board

Human vs Human, Human vs Engine, and Engine vs Engine all keep Quiz Results under user control. Human-controlled strategic choices receive recommendations without automatic commitment; Engine-controlled choices may apply the same result after the user supplies Quiz.

Normal Board Analysis targets about 10 seconds. `Analyze` continues the same decision toward about 20 total seconds using the existing Worker/TT state and the last completed result as a seed. Top 3 Candidate Actions are updated only at completed iterations/refinement stages. Candidate board moves use non-destructive item-preprocessing + Roll-by-Roll preview.

## Testing and strength

Release tests cover official-net anchoring, 24 exact orientations, Rotation/Roll inverses, exact/reduced successor counts, perft, item lengths/consumption, make/undo, notation, generator equivalence, tactical generation, MultiPV, Match Context, Item Choice, Initial Decision, the item-aware evaluation symmetry check, and root-search regression controls. Search-memory reset wiring is additionally verified at the Engine/Search integration level: `new_game()` and `clear_search()` clear both the TT and `Search::clear_memory()`.

Engine 0.13.0 is not presented as an Elo result or a guarantee that a 10/20-second search has found the global best move. Draft 33 retains the measured Draft 32 controls and adds continuity intended to spend repeated analysis budgets more effectively. See `StrengthTesting.md` and `Verification.md`.
