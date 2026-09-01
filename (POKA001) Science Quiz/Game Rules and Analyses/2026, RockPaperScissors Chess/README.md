# 2026, RockPaperScissors Chess

2026 KAIST-POSTECH Science War Science Quiz game handbook, Analysis Board, and classical RPSC Engine.

## Files

- `RockPaperScissorsChess.tex` / `.pdf`: handbook and compiled edition
- `Rules.tex`: official-rule model used by the handbook
- `NotationandGameRecording.tex`: canonical Game Record and analysis language
- `MovementandItems.tex`: exact/reduced cube analysis and item movement
- `RockPaperScissorsChess.html`: offline single-file Analysis Board
- `Engine/`: native C++ RPSC Engine, regression tests, verification, and strength notes

The official Science Quiz rules and final RockPaperScissors Chess plan define the game. The exact 24-orientation physical cube model is authoritative; Gesture States are derived analysis/search information. Quiz Results are always user input.

Build: Draft 35 (2026-09-02 KST)

Source base: GitHub `main` at `b8ef4862a4acfc69057dab2099dafa18563ba2a1` (Draft 34 / Engine 0.13.0).

## Draft 35

Draft 35 is a presentation and cross-project consistency update on top of the current GitHub Draft 34 source. The RPSC rule implementation, Analysis Board behavior, and Engine 0.13.0 search/game logic are intentionally unchanged.

- 2026-specific competition naming is normalized to the away-home order: `KAIST-POSTECH` in English and `카포전` in Korean.
- Korean `포항공과대학교` references in `Acknowledgements` now use the established POSTECH RED institutional treatment while names, departments, and roles remain black.
- `Games` is reflowed as a continuous, page-breakable game-record section instead of forcing one game per page; the three canonical records themselves are unchanged.
- The handbook's structural headings remain English and its descriptive content remains Korean.
- The handbook, Analysis Board, and Engine were re-audited as one RPSC system for established notation and non-deferred rules; no rule behavior is changed in this draft.

## Draft 34

Draft 34 is a handbook-recording update on top of Draft 33; the Analysis Board, Engine 0.13.0, and rule implementation are unchanged.

- `Games` now records three 2026-09-01 squad internal-league games in the canonical RPSC Game Record notation, using White-Black player ordering and the verified converted records.
- `Acknowledgements` is expanded and localized to Korean for names, affiliations, and 2026 Science Quiz roles while retaining the English structural heading.
- The title-page role is refined to `2026 Science Quiz Mathematics Representative`.
- No board diagrams are added to the game records; the chapter intentionally preserves the compact notation-first presentation.

## Draft 33

Draft 33 advances the native Engine to 0.13.0 and makes the Analysis Board a resumable analysis workspace without changing the official rule model.

- `.rpsc` Format 2 saves incomplete confirmed positions as well as completed rounds. Equal Quiz-only records restore White-to-move; equal Quiz plus a White move restores Black-to-move; solo-correct Quiz-only records restore the pending Item Choice.
- A Format 2 session preserves the full Main Line/Variation tree, current variation node, exact cube orientations, score, inventories, order, and phase. Unconfirmed previews/drafts are deliberately excluded.
- The readable Game Record remains authoritative. If the Main Line is edited externally and no longer matches the session payload, stale session metadata is ignored and the body is replayed; Score/Quiz/Captures are recomputed from that replay. Legacy flat Draft 32 `.rpsc` records remain loadable.
- Native Engine 0.13 keeps history, capture-history, continuation/follow-up, and countermove information across related searches and decision probes. It also caches the previous completed root ranking for the same Engine search key and reuses it for first-iteration move ordering.
- The browser Worker likewise carries a completed root ranking into later iterations and the normal 10-second → Analyze 20-second continuation, while completed iterations remain authoritative.
- Draft 32's exact move generator, root progressive widening, geometric reach/item signal, Match Context model, and conservative completed-iteration behavior are retained. These changes improve search allocation and continuity; they are not an Elo or best-move guarantee.
- All twelve Basic/Push/Rotation/Step reference movement figures, the official dice net, and the initial placement are rechecked against the same exact orientation/rule model during release verification.
- Timeout handling and the unresolved Push-followed-by-return-to-the-pre-Push-square clarification are intentionally unchanged pending official confirmation.

## Shared language

The handbook, Analysis Board, and Engine use the same terms and notation: Game Record, Analysis Line, M-number, Candidate Action, Candidate Move, PV, MultiPV, Main Line, Variation, Match Context, Evaluation, `Pu`, `RoL`, `RoR`, `StS`, `StL`, and canonical `Q[1, 1]`-style Quiz notation. Full Roll Path remains part of the move itself.

## Reference discipline

Chess/PGN analysis systems are references for notation hierarchy, PV/MultiPV presentation, and completed-iteration engine UX. Stockfish/Fishtest, shogi engines, Arimaa/Amazons work, Othello engines, and other classical game-engine research are search/testing references only. No external game's rules, opening knowledge, or copied engine code defines RPSC behavior.

The C++ Engine remains the native reference implementation; the offline Analysis Board uses a parallel JavaScript Worker. Strength claims remain conservative and are tied to reproducible controls in `Engine/StrengthTesting.md` and `Engine/Verification.md`.
