# RPSC Engine

`rpsc-engine` is the classical search Engine for RockPaperScissors Chess.

Version: 0.8.0

## Goal

The Engine treats the Quiz Result as external information. It does not predict Quiz correctness. Once a Quiz Result is supplied, the Engine is designed to handle the game decisions that follow: first/second order when the rules grant that choice, Push/Rotation/Step acquisition for an Engine-controlled team, conservation or use of existing items, and the complete legal board move including the exact Roll path.

## Position and move model

- C++17; no machine learning, neural networks, or NNUE
- exact 24-orientation cube state is the rule/notation source of truth
- six-state Gesture State is a derived search reduction under the current rules
- score, alive pieces, side to move, and Push/Rotation/Step inventories are position state and are included in the search key
- canonical full-path legality includes Normal, Push, Rotation Left, Rotation Right, Step Short, and Step Long
- item consumption is performed by the same make/undo position machinery as every other move
- Reset and capture score changes remain part of successor construction

A board move that uses an item is not selected by a separate heuristic before search. Normal and item moves compete in the same alpha-beta tree. This lets the Engine preserve a resource when an ordinary move is better and spend it when the resulting position justifies the cost.

## Item-aware move generation

Item-rich RPSC has a much larger compound-move tree. From the initial position with one Push, Rotation, and Step available to each side, the exhaustive generator contains 1,472 canonical full-path moves, 1,019 exact successor positions, and 427 reduced search successor positions, compared with 161 / 145 / 84 without items.

Version 0.8.0 therefore keeps two responsibilities separate:

1. `generate_legal_moves` remains exhaustive. It is authoritative for rules, canonical notation, exact successor checks, and perft.
2. the native search generator builds representative search successors directly and merges partial Roll branches when they reach the same search-equivalent partial state (square, reduced Gesture State, remaining Rolls, previous direction, and item context).

Regression tests compare the complete reduced-successor set from the optimized generator against the set obtained by exhaustively making every canonical legal move. Optimization is accepted only when those sets agree.

## Search

- iterative deepening with principal-variation search and aspiration windows
- four-way clustered transposition table with deterministic position keys and generation-aware replacement
- tactical, TT, killer, history, item-aware continuation/follow-up history, countermove, capture-history, and prior-iteration root ordering
- direct tactical generation and bounded tactical quiescence
- bounded selective extension around score-changing combat events
- conservative LMR with RPSC threat/defence and known-reply safeguards
- reduced fail-high moves are verified at full depth
- true root MultiPV

Version 0.8.0 removes the former blanket penalty for item moves. It also stops treating every quiet item move as automatically too important to reduce. An immediate capture, a direct defence, a newly created scoring threat, a TT/PV move, or a strong learned reply is searched cautiously; a late quiet item move may be reduced like another quiet candidate and is re-searched if it performs better than expected.

Move-history identity now includes the item action. A Normal move, Push, RoL, RoR, StS, and StL do not share a single learned history entry merely because they use the same piece and destination.

## Item acquisition

`chooseitem W` or `chooseitem B` compares the three legal acquisition choices:

- `Pu`: Push
- `Ro`: Rotation
- `St`: Step

The Engine does not assign a fixed strategic ranking such as "Rotation is worth more than Push". For each hypothetical acquisition it adds exactly that resource to the real current position and runs a bounded **next-board-event probe**. The nominal probe is capped at two board plies (White move and Black response); the existing bounded score-event extension may continue an immediate forcing combat sequence.

This policy deliberately does not invent future Quiz Results. If further solo-correct Quiz Results occur before the next board event, the user supplies them and the Engine evaluates the newly updated inventory again. Therefore `chooseitem` is a current-state resource decision, not a claim to have solved unknown future Quiz outcomes.

## First/second order

`chooseorder` evaluates the canonical initial board role. The chooser-as-White value is compared with the symmetric chooser-as-Black value, so the Engine can recommend `first` or `second` when the first solo-correct Quiz grants that choice. Coin-toss outcomes remain external events, not Engine decisions.

## Evaluation

Evaluation is scaled in RPSC score units (`100 = 1.00` point, one capture = `200`) and uses White's point of view for protocol display. It remains deliberately conservative: official capture score is dominant, with small alive-piece, item-reserve, and legal mobility/flexibility terms. Push, Rotation, and Step do not receive arbitrary positional bonuses. Much of an item's value is expected to be discovered by actually searching the legal item continuations.

## Build

```text
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
ctest --test-dir build --output-on-failure
```

## Protocol

The command-line interface is UCI-inspired but is not UCI-compatible.

```text
rpsc
isready
newgame
position startpos
items W 1 1 1
gain W Ro
chooseorder nodes 30000
chooseitem W nodes 60000
move W3[RoL]: e1-e2-e3-e4-d4-c4
go depth 6
go depth 5 multipv 3
go movetime 10000
perft 2
bench
quit
```

`items W 1 1 1` sets White's Push, Rotation, and Step inventories; use `B` for Black. `gain` applies an externally determined Quiz acquisition after the selected item is known. `chooseitem` analyzes which resource to acquire without mutating the position. `chooseorder` analyzes the initial first/second decision. `go` searches the complete board action space, including legal item use when the inventory permits it.

Search information uses `depth`, `seldepth`, `nodes`, `nps`, `score`, and `pv`. Displayed search score uses White's point of view; the search core itself is side-to-move negamax.

## Analysis Board relationship

The browser uses the original RPSC flow in all three user-facing modes: Human vs Human, Human vs Engine, and Engine vs Engine. Every Quiz Result is entered by the user. The Analysis Board never silently generates `Q[0, 0]` and never predicts a future Quiz.

After a solo-correct result, a Human-controlled team chooses Push/Rotation/Step manually while an Engine-controlled team uses an item-choice probe. After an equal-result Quiz, White and Black move in order; Engine-controlled players search Normal and item-using moves together. Candidate previews preserve the exact item action and full Roll path before `Confirm Move` stores the line.

The browser Worker is a parallel JavaScript implementation for offline use. It mirrors the item-aware histories, item-aware LMR treatment, item-choice semantics, and exact rule state, but the native C++ search generator currently has the more aggressive partial-state Roll-tree merging. A future WebAssembly bridge can eliminate this duplication without changing the Game Record or handbook vocabulary.

## Development references and testing discipline

The implementation is RPSC-specific and independently written. Development methodology is informed by:

- Stockfish/Fishtest: focused search changes, iterative PVS/TT/history concepts, staged move-ordering principles, paired tests, and separate short/long confirmation before strength claims
- Fairy-Stockfish: finite pocket/drop/gating resources treated as real position/move semantics rather than an evaluation-only afterthought
- Arimaa search research: very large compound moves can make a completed-move formulation prohibitively wide, motivating careful decomposition/partial-state reuse before unsafe pruning
- established threat-oriented game search: immediate scoring threats and required defensive replies should not be treated as generic late quiet moves

No Stockfish/Fairy-Stockfish/YaneuraOu source code is copied into this Engine. Their licensing and game-specific implementation remain separate; only general algorithmic ideas and testing practices are used as references.

## Strength claim

Engine 0.8.0 is a structural item-aware development release. Correctness, item-state coverage, and no-item regression controls are verified more strongly than before, but the current samples are not sufficient for an Elo estimate or a claim of verified superiority over strong human play. See `StrengthTesting.md` and `Verification.md`.
