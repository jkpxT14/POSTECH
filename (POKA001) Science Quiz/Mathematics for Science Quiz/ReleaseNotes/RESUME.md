# FINAL-r4 completion checkpoint

Status: COMPLETE. No outstanding authoring or validation step.

Baseline: GitHub jkpxT14/POSTECH @ 568c78e43f1ab7a3fc206e6602f4cafede6adaae.
Edition cutoff: 2026-09-15. Revision date: 2026-09-16.
Scope: main handbook only.

The source and 285-page PDF are the completed FINAL-r4 revision.
Do not reapply insertion patches to these sources. The two .patch files document
changes against the pinned baseline; CHANGELOG.md maps every item to its source
and insertion location. BASELINE.json and SHA256.json identify baseline/current
files; verify_package.py checks integrity, structure, and historical problem counts.

To continue in a new session:
1. Extract the package and run python3 ReleaseNotes/verify_package.py.
2. Read README.md, REVIEW.md, and ReleaseNotes/CHANGELOG.md.
3. If GitHub has changed, compare it against the pinned baseline and this release
   before editing. Preserve any later user changes.
4. Rebuild with latexmk -xelatex MathematicsforScienceQuiz.tex after source edits,
   inspect affected PDF pages, and regenerate hashes for a new revision.

Preserve MATH000 notation and all existing Past Problems. Final Review is a
separate task and is not included here. No September 16 news was added.
