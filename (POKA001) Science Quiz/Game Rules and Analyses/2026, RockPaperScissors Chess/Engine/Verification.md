# Verification - RPSC 0.25.0

**2026 Science War Edition - 2026-09-17**

## Browser engine strength gate

The final JavaScript engine keeps the established primary-biased MultiPV-3 framework and adds the selected four-ply cap for low-material transition extensions. This change was selected from the handbook-game regression work because the previous core could remain horizon-blind in capture -> two-piece -> recapture/Reset sequences.

Paired accelerated measurements previously recorded for this transition candidate against the exact 0.24.1 production core were:

- 50 ms board-move control: **100 games, 58 wins, 40 losses, 2 draws (58.0% raw wins)**;
- 150 ms board-move control: **60 games, 34 wins, 24 losses, 2 draws (56.7% raw wins)**.

The benchmark uses paired role reversal and the same seeded exogenous Quiz/item streams. It is a candidate-selection gate, not a claim that literal 10-second wall-clock play on every machine reproduces the same percentage. Actual 10-second regression positions from Games 7, 9, 10, and 11 were also used to verify that the transition safeguard fixes the known horizon case without losing the established item, quiet-move, and Reset recommendations.

The previous production champion is preserved as `reference/rpsc_engine_0.24.1.js`.

## Analysis time policy

Default 10 s:

- 0-2 s broad root search;
- 2-7 s primary-biased championship search;
- 7-9 s Top-3 stabilization;
- 9-10 s final verification.

Analyze continues cached search toward 20 s cumulative rather than restarting from an empty position analysis.

## Regression gate

`ctest` checks the native rules/protocol suite plus the browser contract, including:

- 24 exact cube orientations and the RPS cycle;
- legal initial position and move generation;
- Push -> first Roll return legality;
- W/B role separation and fixed P/K Quiz order;
- three distinct MultiPV recommendations;
- item and initial order/item-choice APIs;
- reverse `teams W KAIST B POSTECH` mapping;
- manual resignation record round-trip with no engine auto-resign;
- embedded worker identity with `rpsc_engine_0.25.0.js`;
- the Science War low-material transition extension;
- all 11 handbook games, including the 11-round resignation game.

The release package excludes compiled executables, build directories, logs, SyncTeX files, and temporary research/candidate files.
