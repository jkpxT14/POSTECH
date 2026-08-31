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

Build: Draft 30 (2026-08-31 KST)

Source base: Draft 29 bug-fixed package / Engine 0.9.0, preserving the established GitHub folder and file structure.

## Draft 30

Draft 30 advances the native Engine to 0.10.0 and locks the handbook, Analysis Board, and Engine to one RPSC rule/notation model before further strength work.

- Rotation is fixed operationally across all three layers: the cube remains on its current square, the board-vertical axis and top face are fixed, the cube rotates exactly 90 degrees left or right, Top Gesture and Base Roll Length remain unchanged, and Wrist Direction / exact orientation rotate.
- Quiz Result input is a permanent Analysis Board control in Human vs Human, Human vs Engine, and Engine vs Engine. The four canonical buttons are `[1, 1]`, `[1, 0]`, `[0, 1]`, `[0, 0]`.
- Board analysis treats Normal, Push, RoL, RoR, StS, and StL as competing RPSC actions in one search tree. Top-3 continuations can therefore contain item acquisition/use consequences rather than showing items in a separate recommendation layer.
- Item acquisition and the first solo-correct Order+Item decision use a shared decision-search budget. All candidates are screened, then the strongest candidates receive more of the remaining budget while sharing the TT; fixed-depth diagnostics still compare candidates at equal depth.
- Native decision PV formatting now uses the correct hypothetical inventory/order state. Item-using continuations can therefore be emitted safely for `chooseitem` and `chooseinitial`.
- The C++ and browser search generators retain the RPSC-specific partial Roll-state reduction while exhaustive canonical full paths remain available for legality, notation, perft, and regression.
- The handbook extends the existing chess-inspired analysis language with Candidate Action, PV/MultiPV, Main Line/Variation, Forced Line, Critical Position, Transposition, evaluation symbols, optional annotations, and the RPSC-specific Full Roll Path distinction.
- The Analysis Board keeps the established board-first layout. It adds no evaluation graph, heatmap, dashboard, or automatic move-quality labels; information is concentrated in the existing Current Action and Live Analysis areas.
- Normal analysis targets about 10 seconds. `Analyze` continues the same position toward about 20 seconds; completed search information and the Worker TT are reused rather than intentionally restarting from a blank state.
- User-facing `Q[0, 0]`-only play is not a mode. Fixed `Q[0, 0]` remains useful only as an internal board-search control in engine development.

## Reference discipline

Chess textbooks and PGN conventions are references for notation hierarchy, variations, comments, annotations, and analysis vocabulary. Chess.com-style analysis is a UI reference for a board-first layout, a small number of ranked engine lines, and concise continuation display. Stockfish/Fishtest, Fairy-Stockfish, Arimaa research, and other classical engines are architecture/testing references for iterative search, move ordering, transpositions, compound-action branching, and controlled strength testing. No chess opening knowledge, copied engine code, or generic board-game abstraction defines RPSC behavior.

## Development principle

The Engine is built specifically for RPSC: compound full Roll paths, exact cube orientation, combat and Reset, finite Push/Rotation/Step inventories, Quiz-driven action phases, and the White-then-Black Board Event structure. A proposed optimization is retained only if rule regressions pass and it improves the relevant RPSC search/test evidence; a broadly applicable technique is not valuable merely because it is successful in chess.

The C++ Engine is the native reference implementation. The offline Analysis Board remains a parallel JavaScript Worker implementation in Draft 30; no WebAssembly bridge is claimed. Strength claims remain conservative. See `Engine/Verification.md` and `Engine/StrengthTesting.md`.
