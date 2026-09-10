# RPSC Engine

Version: **0.19.0 checkpoint**

Ruleset: `2026-rpsc-rotation6-push-return`.

This version is the tested exact-fast successor of the GitHub 0.18.0 native engine. It intentionally retains the proven 0.18.0 search/evaluation structure while accelerating rule-exact move generation and generated-move application.

## Build

```text
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
```

The bundled `rpsc-engine` executable is a Linux x86-64 Release build. On Windows, build the same source with CMake/MSVC or MinGW; the old 0.18.0 Windows executable is intentionally not relabeled as 0.19.0.

The text protocol remains compatible with the 0.18.0 command set. `go ... multipv 3` is the normal analysis mode.
