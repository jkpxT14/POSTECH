# Verification

Draft 25 develops the Draft 24 package as one handbook / Analysis Board / Engine project. The verification below is limited to implemented behavior and deterministic checks; it is not a playing-strength rating.

## Rule and move-generation regression

From the initial position, White to move, no items:

- canonical full-path legal moves: `161`
- distinct exact successor positions with the 24-orientation state retained: `145`
- distinct reduced search successors: `84`
- canonical full-path `perft(2)`: `25575`
- canonical full-path `perft(3)`: `4215782`

With both sides holding one Push, one Rotation, and one Step at the initial position:

- canonical legal moves: `1472`
- distinct reduced search successors: `427`

The native regression suite checks the 24 physical cube orientations, inverse Rolls, fourfold Rotation, notation round trips, exact/search-key restoration after make/undo, item actions, deterministic mixed-item play, and distinct legal root lines for MultiPV.

## Engine 0.5.0

The native Engine remains classical C++17 with no machine learning, neural networks, or NNUE. Draft 25 keeps immediate tactics explicit while improving quiet play through:

- direct tactical move generation for quiescence and immediate scoring-pressure checks
- capture-priority ordering and bounded tactical quiescence
- continuation-history and countermove ordering
- threat-aware late-move reduction: a quiet move that removes an immediate opponent scoring possibility or creates an immediate scoring possibility is not treated as an ordinary late quiet move
- four-way clustered transposition table with generation-aware replacement

A representative initial-position depth-5 single-PV search on the package build machine completed at `287675` nodes with White-perspective Evaluation `+0.05` and principal move `W1: a1-a2-a3-b3`. Timing and NPS are machine-dependent and are not a strength claim.

## Analysis Board

The board keeps the Draft 24 board-first visual hierarchy and live canonical Move Notation. Draft 25 adds non-destructive history analysis:

- Previous/Next moves an Analysis Cursor among complete board-decision positions
- the canonical Game Head and actual Game Record are not rewritten by history browsing
- Quiz events remain in stored history even when navigation jumps directly between board-decision states
- an earlier position restores the complete RPSC state, including Quiz state, score, items, side/role, pieces, and exact cube orientations
- Engine play and automatic fixed-result Quiz progression pause while inspecting history, while Engine Analysis remains available
- playing a different legal move from an earlier position creates a Variation instead of replacing the Main Line
- `Current` returns to the canonical game head
- Engine Analysis can show the historically played move together with MultiPV alternatives without assigning uncalibrated `Best`, `Mistake`, or numerical-to-symbol grades

The embedded self-tests also cover the `Only Q[0, 0]` navigation case: fixed-result Quiz events are preserved, history navigation is not immediately pulled forward by automatic Quiz progression, and a tied score after all 20 main Quiz turns ends as a Draw in Engine-containing `Only Q[0, 0]` modes. Human vs Human retains the official quiz-only overtime path.

## Handbook and package

The handbook records only analysis vocabulary and conventions needed to keep notation, Analysis Board behavior, and Engine output consistent. Chapters 4-8 remain intentionally unfilled; Draft 25 does not add unverified tactics, strategy, openings, puzzles, or games merely to document Engine development.

The final handbook compiles as a 23-page A4 PDF. The final XeLaTeX/latexmk build reports no LaTeX/package warnings and no overfull or underfull boxes. Structural PDF preflight passes and the rendered pages were visually checked after the final pagination adjustment.

The deliverable excludes Engine build output and LaTeX auxiliary files. `Engine/` retains its required capitalized project-component name. The C++ Engine remains the native reference implementation; the offline Analysis Board uses a parallel classical JavaScript Web Worker and does not claim an unimplemented WebAssembly bridge.

No rating or claim of verified human-superior strength is attached to Draft 25. Strength changes should be judged by regression positions, tactical tests, self-play, and human analysis.
