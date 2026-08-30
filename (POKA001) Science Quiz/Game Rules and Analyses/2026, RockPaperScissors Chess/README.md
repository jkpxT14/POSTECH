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

Build: Draft 24 (2026-08-30 KST)

Source base: `(POKA001) RockPaperScissors Chess, Draft 23.zip` (SHA-256 `4c095468da79978e060f730183eb7aa0427265751b58004154489dcae7624c13`).

## Draft 24

- adds live canonical Move Notation to `Current action`: after a piece is selected the line begins at its starting square, then grows with every Push/Roll during both human drafting and Engine animation; item codes are shown immediately, and completed drafts preview the same capture/`Reset` suffixes that will be committed to the Game Record
- keeps live notation provisional until `Confirm Move`; `Back` rewinds it, `Cancel` returns it to the selected-piece preview, and only a confirmed move enters the canonical Game Record
- reuses the same live Move Notation formatter in `Current action` and `Live Analysis` so the two panels cannot drift into different path strings
- keeps `Engine/` and all exposed component naming case-consistent while retaining conventional lowercase C++ source filenames and the `rpsc-engine` executable name
- keeps Chapters 4-8 intentionally open; no unverified tactic, strategy, opening, puzzle, or game content is inserted
- restores the worked Chapter 3 bridge from canonical Move Notation through Roll Word and Axis Word to a complete Gesture-State transition, while retaining the coarser Draft 22 heading structure
- slows Engine Roll animation to about 300 ms per Roll so exact orientation, Top Gesture, Wrist Direction, Roll Word, Axis Word, and Gesture-State changes can be followed more comfortably
- separates live piece-state rendering from Engine Analysis rendering so Roll animation does not repeatedly rebuild an unchanged Engine panel
- keeps the last completed Engine Analysis snapshot visible while a new position is being searched, then replaces it only with a completed-depth result
- streams only completed iterative-deepening snapshots from the browser worker; background and deeper analysis therefore update progressively instead of disappearing and reappearing
- snapshots the exact Engine state used for each analysis result so a retained PV is always formatted against the position that was actually searched
- keeps request IDs and position fingerprints as stale-result guards across move animation, reset, load, undo, and mode changes
- uses automatic background MultiPV 3 analysis on Human vs Human positions and a deeper MultiPV 3 search for `Analyze`; Engine play remains concentrated on a single PV
- keeps numerical Engine Evaluation separate from `=`, `+=`, `±`, and related human analysis symbols until numerical thresholds are empirically calibrated
- adds a small rule-derived local path-flexibility term to both native and browser Evaluation: legal first Rolls plus legal non-reversing second-Roll continuations, with no arbitrary center, opening, or gesture preference
- preserves item-aware move generation and analysis for Push, Rotation, Step Short, and Step Long
- keeps every Quiz turn in every mode; Engine-containing play modes remain fixed to `Q[0, 0]`
- preserves the restrained board-first Analysis Board design instead of adding a dashboard, large evaluation bar, or decorative controls

The Engine Analysis interaction takes cues from established chess-analysis interfaces without copying their visual design: Stockfish-style completed-depth information and MultiPV vocabulary, Chess.com-style persistent current-position analysis, and En Croissant-style separation between an analysis GUI and its Engine. RPSC-specific notation, scoring, Quiz flow, exact cube orientation, and Gesture-State analysis remain authoritative.

The C++ Engine is the authoritative native Engine core. The offline single-file Analysis Board currently uses a parallel classical JavaScript Web Worker so that it runs without a server or local executable. The two implementations share the same rule state, exact-orientation source model, Gesture-State reduction, current item inventories, numerical score scale, local path-flexibility Evaluation term, Move Notation, M-number/PV notation, and Engine Analysis vocabulary. A future WebAssembly bridge may remove the duplicated search implementation, but Draft 24 does not claim that integration already exists.

No rating or claim of verified superhuman strength is attached to this draft. Engine strength remains an empirical property to be established through regression matches, self-play, tactical suites, and human testing.
