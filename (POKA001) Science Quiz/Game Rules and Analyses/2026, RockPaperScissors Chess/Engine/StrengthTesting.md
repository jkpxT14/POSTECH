# Strength Testing

## 0.17.0 comparison

The control is the extracted 0.16.0 Release engine with the same new Push-return rules. The shipped 0.17.0 engine keeps the search core, evaluation semantics, reductions and ordering policy, while carrying the generator's already computed final Orientation into the hot make/undo edge. This removes a second full path validation and orientation walk for generated search moves. TT moves and user-entered moves still take the complete legality path.

The RPSC-specific evaluation retains occupancy masks and equivalent Gesture-State reach-profile reuse. PVS, TT, quiescence, aspiration, MultiPV, LMR, histories and evaluation weights are unchanged. A result-Gesture-State indexed history/capture/continuation/countermove candidate was rejected after a 32-game screen (**8 wins / 17 draws / 7 losses**); it is not shipped and no Elo gain is claimed from it.

## Throughput benchmark

Windows x64, GCC Release, one engine process at a time. Six deterministic legal positions from the existing opening set were searched at depth 3, MultiPV 1, with fresh search memory. Every paired run returned identical nodes, depth, score, best move and PV.

| Position | Control NPS | 0.17.0 NPS | Ratio |
|---:|---:|---:|---:|
| 1 | 559,074 | 639,723 | 1.144 |
| 2 | 294,027 | 278,122 | 0.946 |
| 3 | 127,943 | 152,786 | 1.194 |
| 4 | 110,135 | 100,882 | 0.916 |
| 5 | 263,270 | 279,913 | 1.063 |
| 6 | 188,438 | 246,434 | 1.308 |

Across the six paired positions, the arithmetic mean of the position ratios is **+9.5%** and the aggregate NPS ratio is **+10.0%**, with identical searched node counts and results. These are local throughput measurements, hardware- and load-dependent; they are evidence of the retained optimization's speed on this host, not an Elo or playing-strength certification.

The browser reach-profile table still contains **6,144** entries and the JS/C++ differential suite found no changed legal successor state. The full rule, UI and time-budget checks are recorded in `Verification.md`.
