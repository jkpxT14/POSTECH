# RPSC Engine

Version: **0.14.0**

Native C++17 search engine for RockPaperScissors Chess. Quiz Results are external input; the Engine analyzes only the confirmed board and Match Context.

## Rule model

- exact 24-orientation cube state for rules and notation
- six-state Gesture State only as a search reduction
- Rotation keeps the top face fixed and rotates the cube in place by 90 degrees
- Push is a non-Roll one-square translation; the first Roll may return to the pre-Push square
- timeout adjudication is not implemented by the Engine
- exhaustive `generate_legal_moves` remains authoritative for legality

## Search

The Engine uses iterative deepening PVS, aspiration windows, a clustered transposition table, tactical quiescence, bounded capture extensions, LMR, root reduction, MultiPV, and persistent history/continuation/countermove ordering. Repeated timed analysis of the same search state reuses the previous completed root ordering and transposition data.

## Commands

```text
newgame
items W 1 1 1
items B 1 1 1
go movetime 10000 multipv 3
go movetime 20000 multipv 3
chooseitem W
chooseinitial
perft 3
```

See `Verification.md` and `StrengthTesting.md` for the controls actually run for this release.
