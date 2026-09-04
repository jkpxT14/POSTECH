# Strength Testing

Engine 0.15.0 keeps the previous PVS/TT/quiescence/history search and adapts it to the six-Rotation rules. Search now canonicalizes removed-piece state and searches one representative of each rule-equivalent reduced Rotation class while the legal generator still exposes all six actions.

On a local item-rich 1.5 s MultiPV=3 regression, the three-run median search throughput rose from about `100.8k` to `119.5k` NPS (`+18.6%`) versus the pre-optimization new-rules baseline. This is a throughput measurement, not an Elo claim.
