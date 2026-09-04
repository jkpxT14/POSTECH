# RPSC Engine

Version: **0.15.0**

Native C++17 analysis engine for RockPaperScissors Chess. The handbook is the rule reference; Quiz Results are external input.

## Rule model

- exact 24-orientation cube state for rules and notation
- six-state Gesture State only as a safe search reduction
- six Rotation actions: `RoN`, `RoS`, `RoE`, `RoW`, `RoL`, `RoR`
- Rotation is applied before the Roll length is read from the new Top Gesture
- Push is a non-Roll one-square translation and the first Roll cannot return to the pre-Push square
- official Timeout adjudication belongs to the game controller, not Engine search
- exhaustive `generate_legal_moves` is authoritative for legality

## Search

Iterative-deepening PVS, aspiration windows, transposition table, tactical quiescence, bounded capture extensions, LMR/root reduction, MultiPV, and history/continuation/countermove ordering are retained. Rule-equivalent reduced Rotation branches are merged only inside search; exact legality and notation keep all six actions.

Build with CMake and run `ctest --test-dir build --output-on-failure`.
