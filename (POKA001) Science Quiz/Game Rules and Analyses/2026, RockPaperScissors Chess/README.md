# 2026, RockPaperScissors Chess

Package revision **0.18.0** (2026-09-09), ruleset `2026-rpsc-rotation6-push-return`.

2026 KAIST-POSTECH Science War Science Quiz RockPaperScissors Chess handbook, offline Analysis Board, and native C++ Engine.

- `RockPaperScissorsChess.tex` / `.pdf`: handbook
- `RockPaperScissorsChess.html`: offline Analysis Board
- `Engine/`: native Engine and tests

The exact 24-orientation cube state is authoritative; Gesture State is a derived six-state reduction. Quiz Results are user input.

Build the handbook with `latexmk RockPaperScissorsChess.tex`. Build the Engine with CMake; run its checks with `ctest --test-dir Engine/build --output-on-failure`.

