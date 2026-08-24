# 2026, RockPaperScissors Chess

2026 POSTECH-KAIST Science War Science Quiz game handbook and analysis board.

## Files

- `RockPaperScissorsChess.tex`: main handbook source
- `Acknowledgements.tex`: acknowledgements
- `Preface.tex`: preface
- `Rules.tex`: game rules
- `NotationandGameRecording.tex`: Chapter 2 notation and canonical game-record format
- `MovementandItems.tex`: Chapter 3 movement, items, and Gesture-State analysis
- `Tactics.tex`: chapter skeleton
- `Strategy.tex`: chapter skeleton
- `Openings.tex`: chapter skeleton
- `Puzzles.tex`: chapter skeleton
- `Games.tex`: chapter skeleton
- `Figures/`: TikZ figure sources and styles
- `RockPaperScissorsChess.pdf`: compiled handbook
- `RockPaperScissorsChess.html`: offline single-file RPSC analysis board

The handbook is written in Korean, while the title, chapter names, section names, and notation use English. The HTML analysis board preserves the same restrained line-based visual language and uses the exact physical cube orientation as its source of truth.

Build: Draft 17 (2026-08-25 00:14 KST)

## This draft updates

- refined Chapter 2 so the full game path remains the canonical, lossless record and Chapter 3 analysis notation is explicitly derived from it rather than duplicated in it
- refined Chapter 3 `Gesture States and Roll Analysis` from motivation through practical use, while preserving the existing chapter structure and visual style
- standardized mathematical notation with the handbook conventions: Scissors -> Rock -> Paper / S -> R -> P ordering, set-builder `|`, and function application `V(s)`, `H(s)`, `rho(s)`
- added a compact comparison of Vertical Roll, Horizontal Roll, and Rotation, emphasizing the exchanged axes and whether the Top Gesture can change
- emphasized that Rotation is an in-place 90-degree yaw: it preserves the Top Gesture and Base Roll Length, changes the exact Wrist Direction, and switches the Wrist Axis NS <-> EW
- retained `RoL` and `RoR` as distinct canonical game-record notation even though both induce the same reduced Gesture-State Rotation operator
- retained the current Push interpretation: the first Roll after Push may return to the pre-Push square, pending organizer confirmation
- preserved the existing HTML layout and visual design while separating free piece selection from an active move draft, so the selected piece may be changed until the first Roll or Push
- added a live, derived analysis panel beside Game Record showing the selected piece's Gesture State, Wrist Direction/Axis, Base Roll Length, Roll Word, Axis Word, Gesture State Transition, and `V/H/rho` reference
- kept Game Record as the only persistent representation; Live Analysis is regenerated from the exact board state and current draft and is not saved separately
- added `Jungwoo Kim (POSTECH)` to the analysis-board header without changing the existing visual hierarchy
- expanded HTML self-tests to verify the 24 physical orientations, six Gesture States, `V/H/rho` reductions, Rotation invariants, and existing movement maps
- standardized the game-action term `Roll` with an initial capital throughout the handbook and user-facing analysis-board text
