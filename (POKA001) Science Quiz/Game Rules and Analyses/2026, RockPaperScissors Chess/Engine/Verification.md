# Verification

Package / Engine **0.17.0**, 2026-09-08. Based on `jkpxT14/POSTECH` commit `884b1eeae6691719ac0aebc5fc4416f301005747`.
Ruleset: `2026-rpsc-rotation6-push-return`.

Push does not seed Roll history. Its first Roll may return to the pre-Push square; the next Roll may not reverse that first Roll. Six exact Rotations, post-Rotation Roll length, Timeout sampling, Reset scheduling and notation remain as specified by the handbook.

## Rule and successor baselines

| Position | Legal paths | Exact successors | Reduced successors |
|---|---:|---:|---:|
| Initial, no items | 161 | 145 | 84 |
| Initial, one of each item per side | 2,146 | 1,459 | 568 |

Initial exhaustive perft depths 1 / 2 / 3: **161 / 25,575 / 4,215,782**.

## Completed checks

- Native C++17 Release regression suite: orientation inverses, all six Rotation/tau reductions, Push-return acceptance, subsequent Roll reversal rejection, make/undo, notation round-trip, MultiPV distinctness, item/order probes, timed continuation and Reset handing the scheduled next move to Black.
- Exhaustive and reduced native successor equality for all 24 exact starting orientations; initial and item-rich baseline counts are asserted in the shipped tests.
- JS embedded engine vs C++ exact legal paths and exact successor states, plus reduced successor equality, on **50 deterministic positions** with captures, replenished inventories and changed orientations.
- Games 1–4: **80 quiz rounds** replayed in the board and native Release engine; final capture pairs **3–3, 2–6, 3–1, 3–3** match.
- Handbook W2 Rotation examples and the W3 Push-return example validated. Three Push figures were regenerated with the 24-orientation model and independently checked: **25 / 36 / 49 destination cells** for Scissors / Rock / Paper.
- Browser self-tests in headless Microsoft Edge: Timeout live path/state/notation, unchanged committed game/history during animation and preview, disabled Back, Cancel, Confirm, dual Timeout chronology and session/notation replay. Loading is disabled during animation.
- Native optimized evaluation exactly matches the reference implementation on **246,758 legal successors over 120 positions**, including varying match context and item inventories.
- Handbook rebuilt with XeLaTeX: **26 pages**, with the existing chapter structure and diagram style preserved. The build has only the known local font fallback and underfull-box warnings; no missing-file, missing-glyph or overflow errors.

## Release time-budget checks

Fresh search, MultiPV 3. Wall times include protocol round-trip; each best move was accepted by native legality on application.

| Position | Budget | Measured wall time | Completed depth | Nodes |
|---|---:|---:|---:|---:|
| initial | 10 s | 10.0018 s | 5 | 2,389,249 |
| initial | 20 s | 20.0017 s | 5 | 5,286,977 |
| item-rich | 10 s | 10.0010 s | 3 | 2,769,793 |
| item-rich | 20 s | 20.0007 s | 3 | 5,215,681 |
| midgame | 10 s | 10.0006 s | 3 | 2,541,249 |
| midgame | 20 s | 20.0018 s | 3 | 2,874,241 |

Depth is position- and hardware-dependent. These checks verify deadline handling; they do not prove Elo, optimal play or a guaranteed depth under every 10/20-second search.

Release was compiled directly with GCC (`-std=c++17 -O3 -DNDEBUG -Wall -Wextra -Wpedantic -static`). The bundled executable is Windows x64. The HTML uses its embedded JS worker and does not launch the native executable.
