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

Build: Draft 19 (2026-08-28 03:58 KST)

## This draft updates

- performed a full end-to-end review of the Draft 18 handbook and offline analysis board, preserving the existing visual design and overall structure
- polished Chapter 2 `Analysis Lines` terminology by naming the separate board-sequence counter the `M-number` and clarifying that it is a derived analysis label, not stored canonical game data
- retained the question-based canonical Game Record and all existing Move Notation rules, including the starting square and complete Roll path
- tightened the opening of Chapter 3 without changing its mathematical direction, and made the relation between exact Move Notation, item effects, Roll Words, Axis Words, and Gesture-State transitions more explicit
- reused the same W2 example when introducing Axis Projection to reduce unnecessary notation switching
- clarified Rotation wording throughout Chapter 3: Wrist Direction rotates by 90 degrees, Wrist Axis switches NS <-> EW, while Top Gesture and Base Roll Length remain unchanged
- connected the later practical item section back to the earlier analysis model and standardized a few small wording/spacing inconsistencies
- refined HTML Live Analysis to label the move-start state explicitly, show the starting square, show a live partial Move Notation once movement begins, and show the Rotation preview state before the first Roll
- preserved Game Record as the only persistent representation and kept the 24-orientation physical cube model as the engine source of truth
- expanded self-tests for `M1.` / `M1...` labeling and for starting-square preservation in live Move Notation
