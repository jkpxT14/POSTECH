# 2026, RockPaperScissors Chess

2026 POSTECH-KAIST Science War Science Quiz game handbook, Analysis Board, and classical Engine.

## Files

- `RockPaperScissorsChess.tex`: main handbook source
- `RockPaperScissorsChess.pdf`: compiled handbook
- `RockPaperScissorsChess.html`: offline single-file RPSC Analysis Board
- `Rules.tex`: game rules
- `NotationandGameRecording.tex`: canonical notation and analysis notation
- `MovementandItems.tex`: movement, items, and Gesture-State analysis
- `Tactics.tex`, `Strategy.tex`, `Openings.tex`, `Puzzles.tex`, `Games.tex`: reserved handbook chapters
- `Figures/`: TikZ figure sources and styles
- `Engine/`: classical C++ RPSC Engine source and tests

The exact 24-orientation physical cube model remains the source of truth. Gesture States are a derived analysis layer. Game Record is the canonical persistent representation of an actual game. Engine Analysis is a separate analysis layer and never changes the canonical Game Record.

Build: Draft 22 (2026-08-29 KST)

Source base: GitHub `main` at `73b9a5d2bc26c3ed77e751d0aff886f3fa729803` (Draft 21 source state).

## Draft 22

- keeps `Engine/` and all exposed component naming case-consistent while retaining conventional lowercase C++ source filenames and the `rpsc-engine` executable name
- keeps Chapters 4-8 intentionally open; no unverified tactic, strategy, opening, puzzle, or game content is inserted
- reduces unnecessary handbook fragmentation in Chapter 3 while preserving the complete Gesture-State and movement analysis system
- makes Engine Analysis independent of the play controller: Human vs Human, Human vs Engine, and Engine vs Engine all use the same numerical Evaluation, Candidate Move, MultiPV, and PV vocabulary
- gives Human vs Human automatic background analysis with three Candidate Moves and PVs; `Analyze` requests a deeper three-line search without changing the Game Record
- keeps numerical Engine Evaluation separate from `=`, `+=`, `±`, and related human analysis symbols until numerical thresholds are empirically calibrated
- models the current Push, Rotation, and Step inventories in the Engine core and in browser analysis, so Human vs Human recommendations may include legal item use
- animates each Engine move one Roll at a time so the displayed path, Top Gesture, Wrist Direction, Roll Word, Axis Word, and Gesture-State transition remain visually connected to the handbook notation
- allows mode changes after play has started; changing mode resets the current game after confirmation instead of locking the selector
- replaces the ambiguous Engine-vs-Engine `Step` control with `Advance`, which proceeds only to the next completed current-round boundary
- strengthens the native C++ Engine with item-aware move generation, lower-overhead search metadata, prior-iteration root ordering, conservative LMR, capture quiescence, and true root MultiPV support
- keeps every Quiz turn in every mode; Engine-containing play modes remain fixed to `Q[0, 0]`, so those modes do not acquire items during play even though the analysis Engine itself can evaluate item inventories
- preserves the restrained board-first Analysis Board design instead of adding a separate dashboard, evaluation bar, or decorative controls

The C++ Engine is the authoritative native Engine core. The offline single-file Analysis Board currently uses a classical JavaScript Web Worker bridge so that it runs without a server or local executable. The bridge follows the same exact-orientation source state, Gesture-State reduction, current item inventories, Evaluation scale, move generation, capture quiescence, Move Notation, M-number/PV notation, and Engine Analysis vocabulary. Browser search uses shorter time budgets than the native Engine, but it does not replace item positions with a different leaf-evaluation rule. It is not a claim that the native C++ binary has already been compiled to WebAssembly.

No rating or claim of verified superhuman strength is attached to this draft. Engine strength is treated as an empirical property to be established by regression matches, self-play, tactical suites, and human testing.
