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

The exact 24-orientation physical cube model remains the source of truth. Gesture States are a derived search/analysis layer. Quiz Results are always external user input: the Engine never predicts or invents a future Quiz Result.

Build: Draft 29 (2026-08-31 KST)

Source base: Draft 28 / Engine 0.8.0, preserving the current GitHub folder and file structure.

## Draft 29

Draft 29 advances the Engine from 0.8.0 to 0.9.0 and concentrates on RPSC-specific decision quality rather than adding a general board-game framework.

- Normal, Push, Rotation Left/Right, and Step Short/Long remain first-class actions in the same search tree.
- Item acquisition is no longer capped at a two-ply probe. Push, Rotation, and Step receive iterative search under the supplied total decision budget, with the TT shared between candidates.
- The first solo-correct decision is evaluated jointly as six alternatives: First/Second combined with Push/Rotation/Step. The Engine no longer greedily decides order first and item second.
- Native `chooseinitial` and strengthened `chooseitem` protocol commands expose ranked choices and board-continuation PVs without assuming future Quiz Results.
- Timed MultiPV keeps Top-3 recommendations from the last fully completed root iteration even when the next iteration reaches the clock. Deep search therefore does not discard useful alternatives merely because the timer stopped a later iteration.
- The browser Worker now mirrors the C++ partial Roll-state generator: exhaustive canonical paths remain available for rules/notation, while search merges equivalent partial states before compound Roll paths fully expand.
- Board Analysis, Item Choice, and the initial Order+Item decision use the same restrained recommendation language: ranked choices, score, and a short continuation line.
- Normal analysis targets about 10 seconds. `Analyze` extends the current Board, Item, or Initial Decision analysis toward about 20 seconds and reuses the completed decision result/TT where supported.
- Human-controlled choices are also analyzed. The Engine recommends without taking control; Engine-controlled choices use the same analysis to act automatically.
- The Analysis Board keeps its existing board-first layout and removes repeated `Evaluation` / `Best Move` text when the same information is already present in Candidate 1.
- Displayed continuation lines are intentionally short even when the internal PV is longer.
- C++ Release tests explicitly keep assertions active, and the orientation suite now checks all 24 exact orientations for Rotation and Roll inverses.

`Q[0, 0]` remains useful only as an internal control condition for board-search regression. It is not a user-facing Analysis Board mode.

## Development principle

The Engine is built for RPSC itself: compound full Roll paths, exact cube orientation, combat score events, Reset, finite Push/Rotation/Step inventories, and the White-then-Black Board Event structure. Established engine techniques are used only where they improve this specific game. No future-Quiz probability model, chess opening knowledge, arbitrary preferred squares, or fixed hard-coded item hierarchy is introduced.

The C++ Engine is the native reference implementation. The offline Analysis Board remains a parallel JavaScript Worker implementation; Draft 29 narrows an important performance gap by bringing the partial-state Roll generator to the browser, but does not claim a WebAssembly bridge.

Strength claims remain conservative. Draft 29 is primarily an integrated-decision and browser-search release. See `Engine/Verification.md` and `Engine/StrengthTesting.md`.
