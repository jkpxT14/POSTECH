# Verification

Draft 22 was built and tested against the same RPSC rule, orientation, notation, and score model used by the handbook and Analysis Board.

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
- move breakdown: no item `161`, Push `523`, Rotation Left `161`, Rotation Right `161`, Step Short `60`, Step Long `406`

The native C++ Engine and the embedded browser worker produced exactly the same sets of canonical Move Notation for all four move-generation checks above: initial raw, initial reduced, item-aware raw, and item-aware reduced.

The C++ regression suite also checks all 24 physical cube orientations, inverse Rolls, fourfold Rotation, notation round trips, exact/search-key restoration after make/undo, all five item actions, a deterministic mixed-item playout, and distinct legal root lines for MultiPV analysis.

## Search and MultiPV checks

The native Engine completed a three-line MultiPV search at depth 4 from the initial position and a three-line item-aware search at depth 2. The browser worker completed MultiPV 3 at depth 3 from the initial position and depth 2 with current item inventories.

For the initial position at the tested completed depths, native and browser analysis agreed on the top three Candidate Moves and their numerical Evaluations. With White alone holding one of each item, they also agreed on the top three Candidate Moves and Evaluations, including the two leading Step Long lines. With symmetric item inventories, they agreed on the Best Move and Evaluation; equally evaluated lower candidates may be ordered differently because the native and browser search implementations are separate.

On the package build machine, the native Release Engine produced the following representative results:

- deterministic `bench` depth 4: `34350` nodes, about `0.52 s`
- single-PV depth 5: `239889` nodes, about `1.86 s`, Evaluation `+0.02`
- single-PV depth 6: `739965` nodes, about `5.91 s`, Evaluation `+0.00`
- White holding one of each item, MultiPV 3 depth 2: `12933` nodes; the leading lines use Step Long and evaluate to `+2.20`
- both sides holding one of each item, MultiPV 3 depth 2: `28730` nodes; the leading Evaluation is `+0.00`

Timing and NPS are machine-dependent and are not a strength rating.

## Build and package checks

The native Engine was compiled as a C++17 Release build with the warning flags in `CMakeLists.txt`. The final Release build completed without compiler warnings and `rpsc-engine-tests` passed. The browser scripts were syntax-checked with Node.js, the main Analysis Board self-tests passed, and worker smoke tests covered no-item and item-aware move generation, item consumption/undo, and MultiPV search.

The handbook was rebuilt with XeLaTeX/latexmk as a 23-page A4 PDF. The final LaTeX log contains no LaTeX/package warnings and no overfull or underfull boxes; the PDF was rendered page-by-page for visual inspection and passed PDF preflight.

The C++ Engine remains the native reference implementation. The single-file Analysis Board currently uses a parallel classical JavaScript Web Worker rather than a compiled WebAssembly copy of the C++ core. The two implementations share the same rule state, canonical notation, current item model, numerical score scale, and analysis vocabulary, and are regression-checked for move-generation parity. A future WebAssembly bridge can remove the remaining duplicated search implementation without changing the handbook or Game Record format.

No rating or claim of verified human-superior strength is attached to Draft 22. Strength remains an empirical property to be established through regression matches, self-play, tactical suites, and human testing.
