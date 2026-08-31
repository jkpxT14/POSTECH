# Verification

Draft 27 develops Draft 26 as one handbook / Analysis Board / Engine package.

## Rule and move-generation regression

The native regression suite retains these initial-position checks:

- canonical full-path legal moves: 161
- exact successor positions: 145
- reduced strategic search successors: 84
- perft(1): 161
- perft(2): 25,575
- perft(3): 4,215,782

The suite also checks the 24 physical cube orientations, inverse Rolls, fourfold Rotation, notation round trips, exact/search-key restoration after make/undo, item actions, deterministic mixed-item play, tactical-generator consistency, and distinct legal root MultiPV lines.

## Engine 0.7.0

The engine remains classical C++17. Draft 27 adds bounded score-event selective extension (maximum two per line) and more frequent timed-search deadline checks. No square, opening, Gesture, waiting move, or supplied game sequence is encoded as a positional bonus.

## Analysis Board

The browser Worker mirrors the bounded score-event extension and keeps the Draft 26 exact orientation / reduced Gesture-State model. Normal Analysis uses up to about 10 seconds; `Analyze` extends the same exact position toward a 20-second total budget. A bounded per-position result cache avoids restarting already completed ordinary analysis when revisiting the same exact position. Primary PV iterative deepening is completed first; MultiPV alternatives are filled at the final completed depth.

Browser JavaScript syntax and embedded self-tests are checked during packaging.

## Strength testing

See `StrengthTesting.md`. The 96-game 6,000-node paired set scored 47-12-37 (55.21%) for 0.7.0, while a 24-game 12,000-node confirmation sample scored 4-11-9. These results are reported as exploratory and mixed, not as a verified Elo improvement.

## Packaging

The deliverable excludes CMake build products and LaTeX auxiliary files. The C++ Engine remains the native reference implementation; the offline Analysis Board remains a parallel JavaScript Worker implementation rather than claiming an unimplemented WebAssembly bridge.
