# 2026, RockPaperScissors Chess

2026 POSTECH-KAIST Science War Science Quiz game handbook, Analysis Board, and classical RPSC Engine.

## Files

- `RockPaperScissorsChess.tex` / `.pdf`: handbook and compiled edition
- `Rules.tex`: official-rule model used by the handbook
- `NotationandGameRecording.tex`: canonical Game Record and analysis language
- `MovementandItems.tex`: exact/reduced cube analysis and item movement
- `RockPaperScissorsChess.html`: offline single-file Analysis Board
- `Engine/`: native C++ RPSC Engine, regression tests, verification, and strength notes

The official Science Quiz rules and final RockPaperScissors Chess plan define the game. The exact 24-orientation physical cube model is authoritative; Gesture States are derived analysis/search information. Quiz Results are always user input.

Build: Draft 32 (2026-09-01 KST)

Source base: Draft 31 / Engine 0.11.0, preserving the established project structure.

## Draft 32

Draft 32 advances the native Engine to 0.12.0. The main goal is practical RPSC analysis strength under the existing 10-second normal / 20-second Analyze workflow, without changing the official rules or the established notation/UI structure.

- The official dice-net remains hard-anchored: horizontal `P-S-P-S`, with Rock immediately above and below the left-hand `S`. Rotation remains an in-place 90-degree turn around the board-vertical axis with square/top fixed.
- Search move generation reuses fixed scratch storage for partial-state and reduced-successor deduplication instead of repeatedly allocating large temporary tables on the hot path.
- A shallow TT move can be searched before the complete compound-move set is materialized; a cutoff can therefore avoid unnecessary Roll-path generation and sorting.
- Root search now uses conservative progressive widening / verification LMR across Normal, Push, Rotation, and Step families. Every family receives full-depth candidates before late quiet actions are reduced; any reduced candidate that challenges alpha is re-searched at full depth.
- The leaf evaluator remains score-first but adds a small RPSC-specific geometric reach signal. Item reserve value is no longer completely type-blind: Push, Rotation, and Step receive a cheap position-dependent latent value based on how many additional reduced Roll outcomes the item can create from the current pieces. The search, not a fixed item hierarchy, still decides whether an item should be acquired, conserved, or used.
- In the current verification environment, the native depth-4 no-item control has a median wall time of about 0.86 s versus about 1.04 s for Draft 31, while preserving the established best root move. This is a throughput/control result, not an Elo claim.
- More importantly for practical analysis, the symmetric initial item-rich 10-second control (`Pu/Ro/St = 1/1/1` for both sides) completes depth 3 in Draft 32; Draft 31 completed depth 2 in the same environment.
- The browser Worker mirrors the item-aware geometric evaluation and root progressive-widening policy while retaining its exhaustive canonical generator and reduced search cache.
- Engine results now appear progressively at completed iterations. Board Analysis keeps the current Top 3 visible while displaying `Analyzing` / `Deepening`, elapsed target time, Depth/SelDepth, nodes, and NPS. `Analyze` continues the same decision toward a total of about 20 seconds instead of presenting a visually separate analysis.
- Item Choice and Initial Decision also publish stable intermediate rankings after their complete screening stage and subsequent refinement passes, rather than appearing only at the end.
- Candidate 1 receives only restrained emphasis; selected Candidate Moves remain tied to the existing non-destructive Roll-by-Roll preview. The board-first design is otherwise preserved.
- Confirmed Quiz score/items remain part of Match Context. Unplayed Quiz is still treated symmetrically (`Q[1, 1]` / `Q[0, 0]`), so the Engine never predicts a future Quiz winner or invents a future item.

## Shared language

The handbook, Analysis Board, and Engine use the same terms and notation: Game Record, Analysis Line, M-number, Candidate Action, Candidate Move, PV, MultiPV, Main Line, Variation, Match Context, Evaluation, `Pu`, `RoL`, `RoR`, `StS`, `StL`, and canonical `Q[1, 1]`-style Quiz notation. Full Roll Path remains part of the move itself.

## Reference discipline

Chess/PGN analysis systems are references for notation hierarchy, PV/MultiPV presentation, and completed-iteration engine UX. Stockfish/Fishtest, shogi engines, Arimaa/Amazons work, Othello engines, and other classical game-engine research are search/testing references only. No external game's rules, opening knowledge, or copied engine code defines RPSC behavior.

The C++ Engine remains the native reference implementation; the offline Analysis Board uses a parallel JavaScript Worker. Strength claims remain conservative and are tied to reproducible controls in `Engine/StrengthTesting.md` and `Engine/Verification.md`.
