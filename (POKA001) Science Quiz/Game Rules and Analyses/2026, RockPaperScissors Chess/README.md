# 2026, RockPaperScissors Chess

2026 POSTECH-KAIST Science War Science Quiz game handbook, Analysis Board, and classical RPSC Engine.

## Files

- `RockPaperScissorsChess.tex`: main handbook source
- `RockPaperScissorsChess.pdf`: compiled handbook
- `RockPaperScissorsChess.html`: offline single-file RPSC Analysis Board
- `Rules.tex`: game rules
- `NotationandGameRecording.tex`: canonical notation and analysis notation
- `MovementandItems.tex`: movement, items, and Gesture-State analysis
- `Tactics.tex`, `Strategy.tex`, `Openings.tex`, `Puzzles.tex`, `Games.tex`: reserved handbook chapters
- `Figures/`: TikZ figure sources and styles
- `Engine/`: native C++ RPSC Engine source, tests, verification, and strength-testing notes

The official Science Quiz rules and the final RockPaperScissors Chess plan define the game. The exact 24-orientation physical cube model is the implementation source of truth; Gesture States are only a derived analysis/search layer. Quiz Results are always external user input. The Engine never predicts or invents a future Quiz Result.

Build: Draft 31 (2026-08-31 KST)

Source base: Draft 30 / Engine 0.10.0, preserving the established GitHub folder and file structure.

## Draft 31

Draft 31 advances the native Engine to 0.11.0 and develops the handbook, Analysis Board, and Engine as one RPSC analysis language grounded in the official 2026 rules and final game plan.

- The official dice-net is regression-anchored directly: horizontal `P-S-P-S`, with Rock immediately above and below the left-hand `S`. Folding this net gives opposite pairs S/S, R/R, P/P; native and browser cube states are tested from this anchor rather than merely checking that 24 orientations exist.
- Rotation remains a strict invariant: current square and top face fixed, exactly 90 degrees around the board-vertical axis, Top Gesture/Base Roll Length unchanged, Wrist Direction/exact orientation rotated.
- Canonical Quiz UI now displays `Q[1, 1]`, `Q[1, 0]`, `Q[0, 1]`, `Q[0, 0]`. Quiz Results remain manual in all Human/Engine modes.
- Engine Evaluation now includes all confirmed Quiz points and current inventories. Future Quiz Results are not predicted; remaining rounds use the Symmetric Quiz Assumption, equivalent to future `Q[1, 1]`/`Q[0, 0]`, so no unearned items are created.
- Remaining board plies are part of the search state. Item reserve value tapers near the end of the main game, and the final known board ply resolves to official Score = Quiz + 2 x Captures with the Quiz-count tie-break.
- Quiet item actions may still receive normal LMR, but Draft 31 prevents the most aggressive two-ply reduction from being applied to an item action. This spends more of a fixed 10 s / 20 s budget on strategically unusual Push/Rotation/Step continuations without exempting all item moves from pruning.
- Item acquisition, item conservation/use, and the first First/Second + Item decision remain direct Engine decisions. Finite decision budgets use all-candidate screening plus TT-sharing refinement; this is intentionally described as a shared decision budget rather than a false claim of one monolithic root tree.
- `Flip Board` becomes `Rotate Board`: each click rotates the view 90 degrees counterclockwise through all four orientations. Coordinates, piece direction markers, and side labels rotate visually while canonical squares, exact cube state, Game Record, and Engine state remain unchanged.
- Clicking an Engine Candidate now previews its Push/Rotation preprocessing and each Roll sequentially instead of jumping directly to the final square. The preview is non-destructive until Confirm Move.
- Move-quality annotations are the six standard `!!`, `!`, `!?`, `?!`, `?`, `??`. `Only Move` remains an analysis term rather than a seventh quality symbol.
- Normal analysis targets about 10 seconds; `Analyze` extends the same current decision toward about 20 seconds while retaining completed information and Worker TT state where applicable.

## Reference discipline

Chess textbooks and PGN conventions are references for notation hierarchy, variations, comments, annotations, and analysis vocabulary. Chess.com-style analysis is a UI reference for a board-first layout, a small number of ranked engine lines, and concise continuation display. Stockfish/Fishtest, Fairy-Stockfish, Arimaa research, and other classical engines are architecture/testing references for iterative search, move ordering, transpositions, compound-action branching, and controlled strength testing. No chess opening knowledge, copied engine code, or generic board-game abstraction defines RPSC behavior.

## Development principle

The Engine is built specifically for RPSC: compound full Roll paths, exact cube orientation, combat and Reset, finite Push/Rotation/Step inventories, Quiz-driven action phases, and the White-then-Black Board Event structure. A proposed optimization is retained only if rule regressions pass and it improves the relevant RPSC search/test evidence; a broadly applicable technique is not valuable merely because it is successful in chess.

The C++ Engine is the native reference implementation. The offline Analysis Board remains a parallel JavaScript Worker implementation in Draft 31; no WebAssembly bridge is claimed. Strength claims remain conservative. See `Engine/Verification.md` and `Engine/StrengthTesting.md`.
