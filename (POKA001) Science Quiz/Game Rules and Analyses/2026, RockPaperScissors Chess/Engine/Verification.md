# Verification

Engine 0.15.0 was built in Release mode and the regression suite passed.

Measured new-rules baselines:

- initial legal / exact-successor / reduced-search: `161 / 145 / 84`
- initial perft depth 1 / 2 / 3: `161 / 25,575 / 4,215,782`
- item-rich legal / exact-successor / reduced-search: `2,029 / 1,424 / 568`
- all 24 exact orientations and all six Rotation transitions checked against the handbook Gesture-State reductions
- Push immediate return rejected; make/undo and notation round-trip checked
- handbook Games 1--4 replayed without a legality error
- Analysis Board and native Engine exact legal-move sets and reduced successor sets matched on 21 deterministic mixed-item positions

These checks do not establish Elo, global optimality, or a guaranteed completed depth for a fixed wall-clock budget.
