# Strength Testing

These are search controls, not Elo results.

## No-item initial position

Seven Release depth-4 runs completed with the same best root:

`W1: a1-a2-a3-b3`

- nodes: 203,165
- median elapsed time: 804 ms

The previous recorded control for the earlier search base was about 212k nodes and 858 ms. The measurements are useful as a local regression control, not as a playing-strength claim.

## Item-rich initial position

Both sides held Push / Rotation / Step = 1 / 1 / 1, MultiPV 3.

- 10 s: completed depth 3, 1,699,905 nodes, best `W4: g1-g2-g3-f3`
- repeated 20 s analysis: completed depth 3, 3,856,065 nodes, same best root

Depth 4 did not complete in the 20-second stress control, so no deeper-depth or optimality claim is made.
