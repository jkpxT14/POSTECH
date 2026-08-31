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

The exact 24-orientation physical cube model remains the source of truth. Gesture States are a derived search/analysis layer. Quiz is always part of the game and Game Record, but Quiz Results are external inputs: the Engine never predicts or invents a future Quiz Result. Main Line, Variations, and Engine Analysis remain separate layers.

Build: Draft 28 (2026-08-31 KST)

Source base: current GitHub Draft 27 at commit `370766d8779d0b4833eaf2f230973eb45d976edb`.

## Draft 28

- preserves Draft 27's restrained board-first Analysis Board design, exact-state history navigation, Main Line / Variation separation, and interactive Engine-candidate preview
- advances the native classical Engine from 0.7.0 to 0.8.0 and makes finite items first-class search resources rather than dormant rule support
- keeps the exhaustive canonical full-path generator authoritative for legality, notation, exact perft, and regression while adding a native search generator that merges search-equivalent partial Roll states before the full path tree explodes
- validates the optimized search generator against the exhaustive legal generator by comparing the complete reduced-successor set, including item-rich positions
- makes history, continuation/follow-up history, countermoves, and capture history item-aware so Normal, Push, Rotation Left/Right, and Step Short/Long do not collapse into the same learned reply merely because the mover and destination match
- removes the old blanket item-move ordering penalty and allows late quiet item moves to participate in conservative LMR; tactical, defensive, TT/PV, and strong-history item moves retain the same full-depth safeguards and reduced fail-high moves are verified at full depth
- retains exact item-aware tactical generation and quiescence, so immediate score-changing Push/Rotation/Step moves remain first-class tactics
- adds native `chooseitem` analysis: Push, Rotation, and Step are compared by a bounded next-board-event probe from the actual current board and inventories; future Quiz Results are never assumed
- adds native `chooseorder` analysis for the initial solo-correct first/second decision without predicting a future Quiz
- removes the user-facing `Only Q[0, 0]` Engine modes from the Analysis Board; Human vs Human, Human vs Engine, and Engine vs Engine all use the original game flow and wait for the user to enter every Quiz Result
- after a user-entered solo-correct Quiz Result, an Engine-controlled team chooses its item automatically; after an equal-result Quiz, Engine-controlled White/Black players choose their complete board move including whether to conserve or use any available item
- preserves manual item choice and item use for Human-controlled teams
- restores the official score/Quiz-count/overtime resolution in every Analysis Board mode; the old Analysis-mode 20-round `Q[0, 0]` draw convention is removed
- keeps `Q[0, 0]` as an internal benchmark/control condition for Engine development, where it remains useful for isolating pure board-search regressions from item-choice policy
- keeps ordinary current-position Engine thinking near the Draft 27 10-second target and Deep Analysis near the 20-second target; item acquisition uses its own bounded current-state probe
- keeps Evaluation conservative and rule-derived; no hard-coded item hierarchy, supplied-position bonus, named opening, preferred square, or future-Quiz probability is introduced
- keeps Chapters 4-8 intentionally open rather than turning experimental Engine behavior into unverified handbook theory

## Engine-development references

Draft 28 is implemented independently for RPSC, but its architecture and testing discipline are informed by several established game-engine traditions: Stockfish/Fishtest for iterative alpha-beta development, staged move ordering, focused patches, paired testing, and short/long confirmation; Fairy-Stockfish for treating finite pocket/drop/gating resources as part of position and move semantics; and Arimaa research for the practical importance of decomposing very large compound-move trees rather than blindly sorting every completed move. These are conceptual references only. RPSC legality, exact Roll paths, cube orientation, items, combat, Reset, Quiz flow, notation, and all source code in this package are implemented for this project rather than copied from those engines.

The C++ Engine remains the native reference implementation. The offline Analysis Board continues to use a parallel classical JavaScript Web Worker. Draft 28 mirrors the item-aware action model and selective-search principles in that Worker, but does not claim an unimplemented WebAssembly bridge or byte-for-byte identity between the two search implementations.

Strength testing remains deliberately conservative. Draft 28's correctness and item-state regressions are stronger than Draft 27's; small paired 0.7.0-vs-0.8.0 control samples are documented in `Engine/StrengthTesting.md`, but no Elo or verified-superhuman claim is attached to Engine 0.8.0.
