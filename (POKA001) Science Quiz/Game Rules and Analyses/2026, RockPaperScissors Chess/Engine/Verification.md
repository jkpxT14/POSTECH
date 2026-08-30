# Verification

Draft 26 develops the current GitHub Draft 25 package as one handbook / Analysis Board / Engine project. The verification below is limited to implemented behavior and deterministic checks; it is not a playing-strength rating.

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

## Engine 0.6.0

The native Engine remains classical C++17 with no machine learning, neural networks, or NNUE. Draft 26 keeps immediate tactics explicit while developing quiet/defensive search ordering through:

- direct tactical move generation for quiescence and immediate scoring-pressure checks
- capture-priority ordering plus capture-history feedback from tactical cutoffs
- one-ply continuation history, two-ply follow-up history, countermove ordering, and killer/history replies
- bounded positive feedback for a successful quiet cutoff and negative feedback for inferior quiets searched before it
- threat-aware and history-aware late-move reduction: immediate scoring defence/counter-threats and established useful replies are not treated as generic late quiet moves
- four-way clustered transposition table with generation-aware replacement

These changes are intended to improve discovery of quiet defence, waiting responses, and context-dependent replies without reducing explicit capture/scoring tactics. No RPSC positional claim is encoded as an arbitrary waiting, center, opening, or Gesture bonus.

A representative initial-position depth-5 single-PV search on the package build machine completed at `252786` nodes with White-perspective Evaluation `+0.05` and principal move `W1: a1-a2-a3-b3`. Timing and node count are implementation/machine dependent and are not a strength claim.

## Analysis Board

The board retains the restrained board-first Draft 25 visual hierarchy while making the existing notation and Engine Analysis substantially more interactive:

- the canonical Game Head is visually independent from the Analysis Cursor; browsing history no longer makes the actual record appear truncated or replaced
- canonical Quiz and Move tokens in the Game Record are clickable and restore their complete RPSC snapshots
- Main-Line forward navigation remains canonical even after a Variation was explored
- Variations are rendered as compact parenthesized/indented notation rather than a separate graphical tree panel
- replaying the same historical alternative reuses the existing branch instead of creating duplicates
- complete state restoration includes Quiz state, score, items, move role, pieces, and exact cube orientations
- Engine play and automatic fixed-result Quiz progression pause while inspecting a historical Main-Line position, while Engine Analysis remains available
- an active Variation is separate from the canonical Game Head and may continue its own fixed `Q[0, 0]` flow in Engine-containing modes
- current-position MultiPV Alternatives can be selected to preview their complete legal move on the board and in Current Action
- candidate preview does not enter the Game Record or analysis tree until `Confirm Move`; `Cancel` stores nothing
- no uncalibrated `Brilliant`, `Best`, `Mistake`, or numerical-to-symbol move classification is generated

The embedded self-tests cover canonical-forward navigation after branch exploration, non-destructive branching, duplicate-branch reuse, Main Line plus Variation rendering, preserved Quiz events, historical Engine-player pause, and the `Only Q[0, 0]` Draw / Human-vs-Human overtime distinction. Browser JavaScript syntax and the Web Worker MultiPV path are checked separately during packaging.

## Handbook and package

The handbook records only the analysis vocabulary and conventions required to keep notation, the Analysis Board, and Engine output consistent. Chapters 4-8 remain intentionally unfilled; Draft 26 does not add unverified tactics, strategy, openings, puzzles, or games merely to document Engine development.

The final handbook is a 23-page A4 PDF; structural preflight passes and the final rendered Chapter 2/3 transition was visually checked. The deliverable excludes Engine build output and LaTeX auxiliary files. `Engine/` retains its required capitalized project-component name. The C++ Engine remains the native reference implementation; the offline Analysis Board uses a parallel classical JavaScript Web Worker and does not claim an unimplemented WebAssembly bridge.

No rating or claim of verified human-superior strength is attached to Draft 26. Strength changes should be judged by regression positions, tactical tests, self-play, and human analysis.
