# Verification

Draft 36 / Engine 0.14.0 is verified as one handbook / Analysis Board / native Engine package.

## Native regression suite

The Release build passes `rpsc-engine-tests`. Coverage includes:

- official dice-net anchor;
- all 24 exact cube orientations;
- Rotation top-face invariance and left/right inverses;
- Roll/inverse-Roll checks;
- six-state reduced-orientation equivalence;
- initial canonical/exact/reduced counts `161 / 145 / 84`;
- item-rich counts `1472 / 1019 / 427`;
- exhaustive-vs-search-generator reduced-successor equivalence;
- perft controls `161`, `25,575`, `4,215,782` at depths 1–3;
- canonical notation parse/format round trips;
- make/undo exact and search-key restoration;
- Push-return semantic freeze;
- item-family availability and consumption;
- evaluation symmetry controls;
- MultiPV distinctness and legal PV roots;
- Item Choice and Initial Decision coverage;
- same-key timed continuation retaining the last completed result.

## Analysis Board checks

The final HTML is a single offline file. Static release checks verify:

- unique DOM IDs;
- JavaScript syntax for both main UI and Web Worker;
- 24-orientation and six-state self-test code;
- canonical `Q[1, 1]`, `Q[1, 0]`, `Q[0, 1]`, `Q[0, 0]` labels;
- Human vs Human / Human vs Engine / Engine vs Engine modes;
- canonical Game Record rendering;
- history/variation navigation;
- Format 2 save/load metadata;
- board rotation as view-only;
- Web Worker board analysis with completed-iteration continuation.

A Chromium headless visual/runtime smoke test could not be completed in this container because the installed Chromium process cannot establish its required D-Bus/headless environment. This is an environment limitation rather than a reported page error; syntax/static checks and the board’s built-in self-tests remain in the release. Browser behavior should still be smoke-tested locally before a public deployment.

## Handbook checks

The handbook is built with XeLaTeX, then rendered page-by-page for visual inspection. The Draft 36 candidate is 26 pages. Representative Rules, Notation, Movement, figure-heavy, and Games pages were inspected for clipping, broken Hangul, table overflow, and figure placement.

The latest Games score breakdowns from source commit `5070395ae9fb26e35adc9c9dc14cafa09e9fb4ed` are retained.

## Semantic freezes

1. Rotation: top face fixed, in-place CW/CCW 90°.
2. Timeout: official rule in handbook; no Analysis Board/Engine adjudication.
3. Push: first Roll may immediately return to the pre-Push square.

These rules are not performance knobs.
