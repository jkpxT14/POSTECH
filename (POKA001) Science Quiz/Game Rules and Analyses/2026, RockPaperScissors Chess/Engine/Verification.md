# Verification

Draft 29 / Engine 0.9.0 is verified as one RPSC handbook / Analysis Board / native Engine package.

## Native rule regression

The Release CTest binary explicitly undefines `NDEBUG` before `<cassert>`, so the regression assertions remain active in the packaged Release verification build.

Initial no-item checks:

- canonical full-path legal moves: 161
- exact successor positions: 145
- reduced strategic search successors: 84
- perft(1): 161
- perft(2): 25,575
- perft(3): 4,215,782

Initial position with both sides holding Push/Rotation/Step = 1/1/1:

- canonical full-path legal moves: 1,472
- exact successor positions: 1,019
- reduced strategic search successors: 427
- all five item move actions appear: Push, RoL, RoR, StS, StL

The suite also checks:

- all 24 exact cube orientations;
- left/right Rotation inverse and fourfold Rotation identity;
- top Gesture unchanged by vertical Rotation;
- North/South and East/West Roll inverses;
- notation round trips;
- exact/search-key restoration after make/undo;
- exhaustive-vs-optimized reduced-successor equality in no-item, item-rich, and deterministic playout positions;
- tactical-generator equality against score-changing successors from the full search generator;
- distinct legal root MultiPV lines;
- item-choice non-mutation;
- all six First/Second + Push/Rotation/Step initial branches.

## Protocol smoke

The packaged binary was exercised with:

- identity `RPSC Engine 0.9`;
- `perft 2` = 25,575;
- `chooseinitial` returning six ranked order+item combinations with continuation PVs;
- `chooseitem W` returning three ranked item candidates with continuation PVs;
- item-rich `go ... multipv 3` returning three distinct root recommendations.

## Browser Worker

The browser Worker keeps an exhaustive canonical generator and a separate optimized search generator. Draft 29's search generator merges equivalent partial Roll states using reduced cube state while retaining a representative exact full path.

Packaging smoke checks confirmed:

- main embedded JavaScript self-tests: 0 failures;
- initial exhaustive/search counts: 161 / 84;
- initial item-rich exhaustive/search counts: 1,472 / 427;
- Item Choice returns 3 candidates;
- Initial Decision returns 6 candidates.

Board Analysis, Item Choice, and Initial Decision show short continuation lines. Human-controlled choices remain manual; Engine-controlled choices may be committed automatically after the same analysis.

## Quiz boundary

The Analysis Board never generates a future Quiz Result. `Q[1,1]`, `Q[1,0]`, `Q[0,1]`, and `Q[0,0]` are entered by the user. The Engine only evaluates the game decisions that become available after that external result.

## Packaging

The deliverable keeps the Draft 28 directory layout. CMake build products and LaTeX auxiliary files are excluded from the ZIP.
