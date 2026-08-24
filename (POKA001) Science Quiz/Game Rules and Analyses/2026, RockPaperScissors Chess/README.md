# 2026, RockPaperScissors Chess

2026 POSTECH-KAIST Science War Science Quiz game handbook.

## Files

- `RockPaperScissorsChess.tex`: main source
- `Acknowledgements.tex`: acknowledgements
- `Preface.tex`: preface
- `Rules.tex`: game rules
- `NotationandGameRecording.tex`: notation and game-record format
- `MovementandItems.tex`: completed Chapter 3 source
- `Tactics.tex`: chapter skeleton
- `Strategy.tex`: chapter skeleton
- `Openings.tex`: chapter skeleton
- `Puzzles.tex`: chapter skeleton
- `Games.tex`: chapter skeleton
- `Figures/`: TikZ figure sources and styles
- `RockPaperScissorsChess.pdf`: compiled handbook

The booklet is written in Korean, while the title, chapter names, section names, and notation use English.

Build: Draft 16 (2026-08-24 21:55 KST)

## This draft updates

- added the Gesture State and Roll-analysis system at the beginning of Chapter 3
- introduced the `UD/NS/EW` axis convention, Gesture States, Roll Words, Axis Words, and the `V`, `H`, and `rho` operators for practical cube analysis
- clarified that Rotation preserves the Top Gesture and Base Roll Length while changing the wrist axis and the cube orientation
- retained `RoL` and `RoR` as distinct game-record notation even though both induce the same reduced Gesture-State Rotation operator
- standardized the game-action term `Roll` with an initial capital throughout the handbook
- retained the current Push interpretation: the first Roll after Push may return to the pre-Push square, pending organizer confirmation
