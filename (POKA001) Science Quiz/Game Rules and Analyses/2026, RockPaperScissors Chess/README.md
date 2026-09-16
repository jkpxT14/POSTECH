# 2026, RockPaperScissors Chess

Synchronized release **0.24.1** (2026-09-17), `.rpsc` **Format 3**, ruleset `2026-rpsc-rotation6-push-return`.

This package contains the latest handbook, the self-contained Analysis Board, and the restored/upgraded native engine source. The Analysis Board keeps the established visual design and Top-3 workflow while using the strengthened primary-biased MultiPV engine and the 10 s -> 20 s continuation policy.

## Invariants

- White / Black (`W` / `B`) are board roles.
- POSTECH / KAIST (`P` / `K`) are schools.
- Every quiz token is `Q[POSTECH, KAIST]`, independent of White/Black assignment.
- `WhiteTeam` / `BlackTeam` record the school-to-role assignment.
- Numeric engine evaluation is from the White-role perspective.
- Push permits the first Roll to return to the pre-Push square.
- All six Rotation directions are distinguished in exact orientation and notation.

## Contents

- `RockPaperScissorsChess.pdf` / `.tex`: current handbook and source, including Games 1–9.
- `RockPaperScissorsChess.html`: self-contained Analysis Board with the strengthened embedded 0.24 search core, Top 3 live analysis, history/variations, cache reuse, and 10 s -> 20 s continuation.
- `Engine/`: native C++ engine source with restored full RPSC protocol, item/order/initial-choice interfaces, regression tests, and strength-reference material.
- `Examples/`: legacy Format 2 and canonical Format 3 Game 6 records.

## Engine strength gate

The exact JavaScript search core embedded in the Analysis Board was selected against the 0.23.0 baseline in a paired accelerated regression: **56 games, 34 wins, 21 losses, 1 draw (60.7% raw wins)**. The saved benchmark uses 30/50/80/150 ms board-move controls and role reversal; it is a regression/candidate-selection result, not a guarantee of the same percentage at literal 10-second wall-clock control on every machine.

The native engine retains the same RPSC rules boundary and upgraded search family while restoring the 0.23 protocol surface (`teams`, `matchpk`, `items`, `gain`, `move`, `chooseitem`, `chooseinitial`, etc.). Source-level regression tests verify the critical POSTECH/KAIST vs White/Black mapping.

No compiled engine binaries, build directories, SyncTeX files, checkpoint logs, or temporary patch files are included in this release package.
