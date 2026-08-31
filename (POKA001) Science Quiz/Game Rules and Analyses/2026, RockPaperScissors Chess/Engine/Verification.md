# Verification

Draft 28 develops Draft 27 as one handbook / Analysis Board / Engine package and changes the user-facing Engine flow from a fixed `Q[0, 0]` analysis game to the original RPSC Quiz-driven game.

## Rule and move-generation regression

The native regression suite retains the exact initial-position checks:

- canonical full-path legal moves: 161
- exact successor positions: 145
- reduced strategic search successors: 84
- perft(1): 161
- perft(2): 25,575
- perft(3): 4,215,782

With both sides holding one Push, one Rotation, and one Step at the initial position, it additionally checks:

- canonical full-path legal moves: 1,472
- exact successor positions: 1,019
- reduced strategic search successors: 427
- presence of Push, Rotation Left, Rotation Right, Step Short, and Step Long legal actions

The suite checks all 24 physical cube orientations, inverse Rolls, fourfold Rotation, notation round trips, exact/search-key restoration after make/undo, deterministic mixed-item play, tactical-generator consistency, distinct legal root MultiPV lines, and item-choice non-mutation.

## Optimized item search generator

The native search generator may merge search-equivalent partial Roll states to avoid materializing every completed path. This optimization is regression-checked against the exhaustive canonical generator: the set of reduced successor keys produced directly by the search generator must equal the set obtained by making every exhaustive legal move. The equality check is run for the initial no-item state, the initial item-rich state, and periodic positions in a deterministic item-enabled playout.

The exhaustive generator remains authoritative for legality and notation. The search generator is not allowed to define a new rule set merely to save nodes.

## Engine 0.8.0 decisions

The protocol regression/smoke path verifies:

- Engine identity `RPSC Engine 0.8`
- normal item-aware `go` search and legal MultiPV
- `gain W/B Pu|Ro|St` inventory updates
- `chooseitem W/B` returns three distinct item candidates and does not mutate the real position
- `chooseorder` searches the canonical initial role and compares first/second value without a future Quiz assumption

Item choice uses a bounded next-board-event probe. It is explicitly not a stochastic Quiz model.

A deterministic 10-round protocol smoke script mixing solo-correct and equal-result Quiz events was also replayed through Engine 0.8.0. Item acquisition completed without state/protocol failures, and later board search actually selected and consumed acquired Push items (`B2[Pu]`) in two separate rounds. The final state retained the expected remaining inventories rather than treating item support as display-only metadata.

## Analysis Board

The Analysis Board has no user-facing `Only Q[0, 0]` mode. Human vs Human, Human vs Engine, and Engine vs Engine all wait for a user-entered Quiz Result. The automation layer may decide first/second order, item acquisition, and board moves only when the relevant team is Engine-controlled. A Human-controlled team retains direct item and move controls.

The browser Worker keeps exact cube orientation state and current inventories, generates item moves as normal search candidates, and includes item action in history/continuation identity. Browser source validation during packaging checks both embedded scripts with Node syntax checking, runs the embedded self-test suite, and smoke-checks the Worker's initial reduced successor counts (84 no-item and 427 with one of each item).

The official Human-game score/Quiz-count/overtime procedure is used in all modes; Draft 27's special Analysis-mode tied `Q[0, 0]` draw convention is removed.

## Strength and performance controls

No-item and preset-inventory 0.7.0-vs-0.8.0 controls are kept separate so an item-search change cannot hide a regression in the underlying board search. See `StrengthTesting.md` for the actual small-sample results and timing measurements used for this package. They are development evidence, not an Elo claim.

## Packaging

The deliverable excludes CMake build products and LaTeX auxiliary files. The C++ Engine remains the native reference implementation; the offline Analysis Board remains a parallel JavaScript Worker implementation rather than claiming an unimplemented WebAssembly bridge.
