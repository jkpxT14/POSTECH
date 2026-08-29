# Verification

Draft 23 was rebuilt from the Draft 22 GitHub source state and tested against the same RPSC rule, orientation, notation, item, and score model used by the handbook and Analysis Board.

## Deterministic rule and move-generation checks

From the initial position, with White to move and no item:

- canonical full-path legal moves: `161`
- distinct exact successor positions with the 24-orientation state retained: `145`
- distinct reduced search successors: `84`
- canonical full-path `perft(2)`: `25575`
- canonical full-path `perft(3)`: `4215782`

With both sides holding one Push, one Rotation, and one Step at the initial position:

- canonical legal moves: `1472`
- distinct reduced search successors: `427`

The C++ regression suite checks all 24 physical cube orientations, inverse Rolls, fourfold Rotation, notation round trips, exact/search-key restoration after make/undo, all five item actions, deterministic mixed-item play, and distinct legal root lines for MultiPV analysis. Draft 23 additionally asserts the item-aware raw and reduced move counts above.

A Node-based smoke test of the embedded browser worker reproduced the same four raw/reduced counts used above: `161`, `84`, `1472`, and `427`. The main Analysis Board self-tests also passed.

## Search and analysis checks

The final native C++17 Release build completed without compiler warnings and `rpsc-engine-tests` passed.

Representative native single-PV searches from the initial position on the package build machine:

- depth 5: `287646` nodes, Evaluation `+0.05`
- depth 6: `877910` nodes, Evaluation `+0.02`

Timing and NPS are machine-dependent and are not a strength rating.

The browser worker was syntax-checked with Node.js and exercised directly with an initial-position MultiPV 3 search. It emitted completed-depth snapshots at depths `1`, `2`, and `3`, then one final depth-3 result with three Candidate Moves. The worker's no-item and item-aware move-generation smoke checks passed.

Draft 23 keeps the previous completed Engine snapshot visible while a new request is in flight. Analysis results carry the exact searched Engine-state snapshot and analysis move number used to format their Move Notation and PV, while request IDs and position fingerprints reject stale results from older positions.

## Evaluation change

The native C++ Engine and browser worker both retain the official score difference as the dominant Evaluation term. Draft 23 adds only a small local path-flexibility term based on:

- legal first Rolls from each live piece
- legal second-Roll continuations after those first Rolls
- the rule that a Roll may not immediately reverse to the previous square

No center bonus, opening preference, gesture preference, or automatic numerical-to-symbol threshold is introduced.

## Handbook and package checks

The handbook source retains the coarse Draft 22 Chapter 3 structure while restoring one complete worked example from Move Notation through Roll Word, Axis Word, and Gesture-State transition. Chapters 4-8 remain intentionally unfilled.

The handbook was rebuilt with XeLaTeX/latexmk as a 23-page A4 PDF. A final no-op latexmk pass reported the targets up to date with no LaTeX/package warnings and no overfull or underfull boxes. The PDF passed structural preflight and was rendered page-by-page for visual inspection, including the restored Chapter 3 worked example.

The final package excludes generated Engine build directories, LaTeX auxiliary files, and SyncTeX output. The C++ Engine remains the native reference implementation; the Analysis Board still uses a parallel JavaScript Web Worker rather than claiming an unimplemented WebAssembly bridge.

No rating or claim of verified human-superior strength is attached to Draft 23.
