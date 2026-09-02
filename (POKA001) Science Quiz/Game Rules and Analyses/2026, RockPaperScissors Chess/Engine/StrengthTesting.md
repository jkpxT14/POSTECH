# Strength Testing

Engine **0.14.0** is promoted for measured search-budget improvement while preserving the exact RPSC rule model. These measurements are controls, not an Elo claim.

## Historical control

The established Draft 32/33 control recorded approximately:

- no-item depth 4: about 212k nodes, median 858 ms, best root `W1: a1-a2-a3-b3`;
- item-rich 10 s, MultiPV 3: depth 3, 1,840,897 nodes, best root `W4: g1-g2-g3-f3`;
- subsequent 20 s, MultiPV 3: depth 3, 3,898,369 nodes, same best root.

## Draft 36 / Engine 0.14.0

Release build, seven repeated no-item depth-4 runs:

| Run | Nodes | Time |
| ---: | ---: | ---: |
| 1 | 203,165 | 812 ms |
| 2 | 203,165 | 827 ms |
| 3 | 203,165 | 819 ms |
| 4 | 203,165 | 818 ms |
| 5 | 203,165 | 802 ms |
| 6 | 203,165 | 803 ms |
| 7 | 203,165 | 811 ms |

Median: **812 ms**. Best root remains **`W1: a1-a2-a3-b3`**.

This control is about 5% faster than the previously documented 858 ms median and searches fewer nodes because the verified root-family reduction threshold is slightly tighter (Normal quota 28, item-family quota 20; late-root threshold 56). Any reduced root candidate that challenges alpha is re-searched at full depth.

Item-rich initial position with both sides `Pu/Ro/St = 1/1/1`, MultiPV 3:

- 10 s: last completed depth 3, about 1.74M nodes, best root `W4: g1-g2-g3-f3`;
- same-position subsequent 20 s: last completed depth remains 3; about 3.97M nodes are spent on the unfinished depth-4 attempt, same completed best root.

The second search begins at depth 4 using the cached completed depth/ranking. It therefore does not repeat depths 1–3. Depth 4 still does not finish inside the stress-position budget, so Engine 0.14.0 does **not** claim a deeper completed nominal depth at 20 seconds.

## Promotion controls

Every later strength patch should continue to cover:

1. no-item symmetric positions;
2. item-rich symmetric positions;
3. asymmetric inventories and confirmed Match Context;
4. fixed Quiz scripts;
5. Push, Rotation, Step, capture/recapture, Reset, last-piece, and late-round positions;
6. generator-equivalence and exact-orientation checks;
7. paired self-play/order assignments before any Elo-style claim.

Correctness is a release blocker. A speed patch that changes legal paths, exact orientation, item semantics, canonical notation, or replay state is rejected.
