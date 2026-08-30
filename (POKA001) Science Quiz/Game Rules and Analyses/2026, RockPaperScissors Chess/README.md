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

The exact 24-orientation physical cube model remains the source of truth. Gesture States are a derived analysis layer. Quiz is always part of the game and of the Game Record. `Only Q[0, 0]` does not remove Quiz; it fixes each Quiz Result to `Q[0, 0]`. Game Record is the canonical persistent representation of an actual game, while Engine Analysis and Variations are separate analysis layers and never rewrite the canonical Main Line.

Build: Draft 25 (2026-08-30 KST)

Source base: `(POKA001) RockPaperScissors Chess, Draft 24.zip` (SHA-256 `93510907a54b6bde647535fc70a021fd851d6eb9510c562195dac07944279ba6`).

## Draft 25

- keeps the Draft 24 board-first visual design and live canonical Move Notation, while adding compact Previous/Next history navigation beside the Game Record
- separates the canonical Game Head from the Analysis Cursor: browsing an earlier board-decision position never deletes or rewrites the actual Game Record
- preserves Quiz events inside the canonical history even when Previous/Next jumps directly between board-decision positions for convenient analysis
- pauses automatic `Q[0, 0]` progression and Engine play while an earlier position is being inspected; background Engine Analysis remains available
- lets a legal move played from an earlier position create an in-memory Variation rather than replacing the Main Line; `Current` returns directly to the canonical game head
- restores complete RPSC game state when navigating history, including Quiz state, score, items, move role, pieces, and exact 24-orientation cube states
- shows the historically played move together with the current Engine output and labels MultiPV candidates as `Alternatives`, without introducing uncalibrated `Best`/`Mistake` grades
- defines the special Analysis Board result convention for Engine-containing `Only Q[0, 0]` modes: after all 20 Quiz turns and their board moves, a tied score is a Draw instead of an artificial sequence of fixed-result overtime Quiz turns; official Human vs Human overtime rules are unchanged
- advances the native classical Engine to 0.5.0 and mirrors the same search ideas in the browser worker
- adds direct tactical move generation for quiescence and threat checks so capture tactics remain explicit instead of being weakened by the new quiet-move work
- adds continuation history and countermove ordering so useful quiet responses can be searched earlier in related positions
- makes late-move reduction threat-aware: quiet moves that remove an immediate opponent scoring possibility or create an immediate scoring possibility are retained at full depth rather than being treated as generic late quiet moves
- replaces the direct-mapped native transposition table with a four-way clustered table and generation-aware replacement
- keeps Evaluation conservative and rule-derived; no arbitrary center, opening, gesture, waiting-move, or named-strategy bonus is added
- keeps all machine learning, neural networks, and NNUE out of the Engine
- keeps Chapters 4-8 intentionally open; no unverified tactic, strategy, opening, puzzle, or game theory is added merely to document Engine development

The Analysis Board interaction is informed by established chess-analysis software: persistent analysis, MultiPV alternatives, position-history navigation, and non-destructive variation exploration. The Engine work is informed by classical ideas used in strong game engines such as Stockfish and Fairy-Stockfish (PVS, history/continuation ordering, conservative selective search), Edax Reversi (classical PVS/TT/selective-search discipline), and threat-oriented Gomoku engines. These are design references rather than claims of compatibility or copied game knowledge; RPSC rules, scoring, Quiz flow, exact cube orientation, legal paths, and notation remain authoritative.

The C++ Engine is the authoritative native Engine core. The offline single-file Analysis Board currently uses a parallel classical JavaScript Web Worker so that it runs without a server or local executable. Both implementations retain the same rule model, exact-orientation source model, Gesture-State reduction, current item inventories, numerical score scale, Move Notation, M-number/PV vocabulary, and the Draft 25 quiet/tactical search principles. A future WebAssembly bridge may remove the duplicated search implementation, but Draft 25 does not claim that integration already exists.

No rating or claim of verified superhuman strength is attached to this draft. Search efficiency and tactical/quiet-move handling have been improved structurally; actual playing-strength gains remain an empirical question for regression matches, self-play, tactical suites, and human testing.
