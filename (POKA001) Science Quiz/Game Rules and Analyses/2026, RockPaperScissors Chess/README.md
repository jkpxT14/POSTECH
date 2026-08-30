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

Build: Draft 26 (2026-08-31 KST)

Source base: current GitHub Draft 25 at commit `1962a13d99e3b719dd9e8f5a1a4df300994efe6e`, matching `(POKA001) RockPaperScissors Chess, Draft 25.zip` (SHA-256 `b7c9a6998a385b474cad0be853a5248cc2b1546e4cc944b6d4380970b17be23d`).

## Draft 26

- preserves the Draft 25 board-first visual design; no separate tree panel, evaluation bar, dashboard, or automatic move-grade badges are added
- makes the existing Game Record the primary analysis navigator: Quiz and Move tokens are clickable and restore their exact recorded states
- keeps the canonical Main Line visible while an earlier position or Variation is inspected, instead of visually replacing the record with the current branch snapshot
- renders analysis branches in restrained chess-style parenthesized/indented Variations; replaying an already-existing alternative reuses that branch instead of duplicating it
- makes Previous/Next predictable: Main Line navigation follows the canonical continuation by default, while a selected Variation keeps its own analysis path
- removes the redundant Tools `Previous` button; the compact `‹`, `›`, and `Current` controls remain beside the Game Record
- keeps historical analysis non-destructive: Engine play and automatic fixed-result Quiz progression pause while browsing a prior Main-Line position, while Engine Analysis remains available
- keeps active Variations separate from the canonical Game Head; in Engine-containing modes, a continued Variation may still process its own fixed `Q[0, 0]` events without pulling the canonical game forward
- makes current-position MultiPV Alternatives interactive: selecting an available candidate previews its exact legal path on the board and in Current Action; only `Confirm Move` stores it as a move/Variation, while `Cancel` leaves the tree untouched
- preserves the live canonical Move Notation introduced in Draft 24, including item, Push, capture, and Reset notation
- preserves every Quiz event in history and retains the special Engine-mode convention that a tied score after all 20 `Q[0, 0]` Quiz turns is a Draw; official Human vs Human quiz-only overtime is unchanged
- advances the native classical Engine to 0.6.0 and mirrors the same search-ordering principles in the browser worker
- keeps direct tactical generation and tactical quiescence first-class so immediate captures and scoring tactics are not traded away for quiet-play improvements
- adds capture history, one- and two-ply continuation/follow-up history, successful-quiet rewards, inferior-quiet penalties, and history-aware LMR exemptions for known useful replies
- retains RPSC-specific threat/defence safeguards before reducing quiet moves, so direct defensive responses and quiet counter-threats are searched more carefully
- retains the four-way clustered native transposition table and true root MultiPV
- keeps Evaluation conservative and rule-derived; no arbitrary center, opening, gesture, waiting-move, or named-strategy bonus is added
- keeps all machine learning, neural networks, and NNUE out of the Engine
- keeps Chapters 4-8 intentionally open; no unverified tactic, strategy, opening, puzzle, or game theory is added merely to document Engine development

The Analysis Board interaction is informed by established analysis software such as Chess.com Analysis, KaTrain, Sabaki, and Scid-style notation navigation: the important ideas are non-destructive history browsing, board-entered alternate lines, persistent analysis, and compact variation notation rather than visual imitation. Engine work is informed by classical search ideas from Stockfish, Fairy-Stockfish, Ethereal, Edax Reversi, and threat-oriented Gomoku programs. These are references for search and analysis architecture, not copied game knowledge; RPSC legality, Quiz flow, scoring, exact cube orientation, items, Move Notation, and Game Record remain authoritative.

The C++ Engine is the authoritative native Engine core. The offline single-file Analysis Board currently uses a parallel classical JavaScript Web Worker so that it runs without a server or local executable. Both implementations retain the same rule model, exact-orientation source model, Gesture-State reduction, current item inventories, numerical score scale, Move Notation, M-number/PV vocabulary, and the Draft 26 tactical/quiet search principles. A future WebAssembly bridge may remove the duplicated search implementation, but Draft 26 does not claim that integration already exists.

No rating or claim of verified superhuman strength is attached to this draft. Search ordering and tactical/quiet handling have been developed structurally; actual playing-strength gains remain an empirical question for regression matches, self-play, tactical suites, and human testing.
