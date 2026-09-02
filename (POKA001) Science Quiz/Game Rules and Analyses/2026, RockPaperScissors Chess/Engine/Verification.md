# Verification

Release build and regression suite: **passed**.

Verified controls:

- 24 exact cube orientations
- Roll and Rotation inverses; Rotation preserves the top Gesture
- initial move counts: 161 canonical / 145 exact successors / 84 reduced search successors
- item-rich counts: 1,472 / 1,019 / 427
- perft: 161 / 25,575 / 4,215,782 at depths 1 / 2 / 3
- exhaustive and reduced search generators reach the same reduced successor set
- Push followed by an immediate Roll back to the pre-Push square is legal
- move notation round-trip and make/undo key restoration
- distinct MultiPV recommendations
- three Item Choice branches and six Initial Decision branches
- repeated timed analysis preserves the last completed result

The test suite does not establish Elo, global optimality, or a guaranteed completed depth for a fixed wall-clock budget.
