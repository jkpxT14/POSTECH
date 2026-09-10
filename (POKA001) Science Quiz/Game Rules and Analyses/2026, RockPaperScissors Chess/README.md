# 2026, RockPaperScissors Chess

Checkpoint package revision **0.19.0** (2026-09-10), ruleset `2026-rpsc-rotation6-push-return`.

This ZIP is an **engine-strength checkpoint** for the 2026 KAIST-POSTECH Science War Science Quiz RockPaperScissors Chess project. It preserves the repository package root and `Engine/` capitalization so it can be overlaid directly on the current GitHub package.

## Included in this checkpoint

- `Engine/`: native C++17 RPSC Engine 0.19.0, source, regression tests, paired self-play harness, and a Linux x86-64 Release executable.
- `PackageStatus.md`: exact scope of this interim checkpoint and the upstream package lineage.

The handbook and offline Analysis Board are deliberately **not duplicated in this interim ZIP**. Their current GitHub 0.18.0 copies remain authoritative until the native 0.19.0 engine is embedded and cross-verified in the browser engine. This avoids shipping an older or partially reconstructed Analysis Board under a current filename.

For a full local package, overlay this folder onto the current GitHub directory
`(POKA001) Science Quiz/Game Rules and Analyses/2026, RockPaperScissors Chess/`.

Build the Engine with CMake and run:

```text
cmake -S Engine -B Engine/build -DCMAKE_BUILD_TYPE=Release
cmake --build Engine/build
ctest --test-dir Engine/build --output-on-failure
```
