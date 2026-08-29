# 2026, RockPaperScissors Chess

2026 POSTECH-KAIST Science War Science Quiz game handbook, analysis board, and classical engine.

## Files

- `RockPaperScissorsChess.tex`: main handbook source
- `RockPaperScissorsChess.pdf`: compiled handbook
- `RockPaperScissorsChess.html`: offline single-file RPSC analysis board
- `Rules.tex`: game rules
- `NotationandGameRecording.tex`: canonical notation and analysis notation
- `MovementandItems.tex`: movement, items, and Gesture-State analysis
- `Tactics.tex`, `Strategy.tex`, `Openings.tex`, `Puzzles.tex`, `Games.tex`: reserved handbook chapters
- `Figures/`: TikZ figure sources and styles
- `Engine/`: classical C++ RPSC engine source and tests

The exact 24-orientation physical cube model remains the source of truth. Gesture States are a derived analysis layer. Game Record remains the canonical persistent representation of an actual game.

Build: Draft 21 (2026-08-29 KST)

Base package: `(POKA001) RockPaperScissors Chess, Draft 20.zip`.

## Draft 21

- standardized the engine directory name as `Engine/` and reviewed file, target, function, and display naming for consistency
- returned Chapters 4-8 to their intentionally open state; no unverified tactics, strategy, opening theory, puzzles, or games are inserted
- consolidated optional analysis notation into one `Analysis` section instead of fragmenting Chapter 2 into small subsections
- retained only useful move annotations, position-evaluation symbols, M-number/PV notation, and core engine-analysis terms
- preserved the existing restrained Analysis Board layout while simplifying redundant explanatory text
- strengthened the classical search with better tactical move ordering, killer/history ordering, aspiration windows, conservative late-move reductions, and bounded quiescence
- kept machine learning, neural networks, and NNUE out of the engine
- kept every Quiz turn in every mode; engine-containing modes remain fixed to `Q[0, 0]` in the initial implementation

No rating or claim of verified superhuman strength is attached to this draft. Engine strength is treated as an empirical property to be established by benchmarks, regression matches, self-play, and human testing.
