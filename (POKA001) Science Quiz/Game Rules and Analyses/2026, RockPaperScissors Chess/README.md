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
- `Engine/`: classical C++ RPSC Engine source, tests, verification, and strength-testing notes

The exact 24-orientation physical cube model remains the source of truth. Gesture States are a derived analysis layer. Quiz is always part of the game and Game Record. Main Line, Variations, and Engine Analysis remain separate layers.

Build: Draft 27 (2026-08-31 KST)

Source base: GitHub Draft 26 at commit `5f2f006434d2ec08407211d1428ed555d5bf8345`.

## Draft 27

- preserves Draft 26's restrained board-first design and non-destructive Main Line / Variation navigation
- keeps clickable historical Quiz/Move positions, Previous/Next/Current navigation, and interactive candidate preview
- advances the native classical Engine from 0.6.0 to 0.7.0
- adds a bounded, rule-derived selective extension around score-changing combat events, with no position-specific or named-pattern bonus
- checks timed-search deadlines more frequently
- moves browser search to primary-PV-first iterative deepening; MultiPV alternatives are completed after the primary line reaches its final completed depth
- raises ordinary current-position analysis / Engine move thinking to about 10 seconds
- makes `Analyze` a Deep Analysis action toward about 20 seconds total for the same exact position
- caches completed exact-position analysis results and continues Deep Analysis after the previous completed depth when possible, reducing repeated calculation
- keeps completed iteration results authoritative when a timed search ends
- mirrors the Engine 0.7 selective-search principle in the browser Worker
- keeps Evaluation conservative and rule-derived; no arbitrary center, opening, Gesture, waiting-move, or supplied-game bonus is added
- keeps Chapters 4-8 intentionally open
- adds explicit version-vs-version strength-testing documentation

Engine development is informed by established classical search practice from Stockfish/Fishtest-style baseline testing, Fairy-Stockfish, Ethereal, Edax, and threat-oriented game engines. These are architecture and testing references, not copied chess knowledge. RPSC rules, full Roll paths, exact cube orientation, Quiz flow, scoring, items, notation, and Game Record remain implemented independently.

The C++ Engine is the native reference implementation. The offline Analysis Board still uses a parallel JavaScript Web Worker. Draft 27 does not claim a WebAssembly bridge.

Strength testing is intentionally reported conservatively. A broad 96-game 6,000-node paired set scored 47-12-37 (55.21%) for 0.7.0, but a smaller 24-game 12,000-node sample favored 0.6.0. Draft 27 therefore does not claim a verified Elo gain; see `Engine/StrengthTesting.md`.
