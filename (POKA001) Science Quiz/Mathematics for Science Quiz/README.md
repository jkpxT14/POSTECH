# Mathematics for Science Quiz

This folder contains the complete mathematics handbook for the Science Quiz
section of the POSTECH-KAIST Science War.

Edition: 2026 Science War Edition (September 15, 2026)

The time-sensitive material is current through September 15, 2026. The notation
follows `(MATH000) Mathematical Language` at commit
`e573739329291e71c779b4d9b4be5ff3fcd60920` (the latest `main` commit used for
this edition).

## Folder Structure

- `MathematicsforScienceQuiz.tex`: main document and shared formatting
- `Preface.tex`, `Acknowledgements.tex`, `NotationandConventions.tex`: front matter
- `MathematicalConcepts.tex`, `FactstoMemorize.tex`, `Mathematicsin2026.tex`:
  study material
- `PastProblems.tex`: past Science Quiz problems and solutions
- `Figures/`: editable figure sources, styles, and compiled figure files
- `MathematicsforScienceQuiz.pdf`: compiled handbook

Additional practice problems and their solutions are maintained separately in
the companion document `Expected Problems`.

## Provider

Jungwoo Kim (Department of Mathematics, Pohang University of Science and Technology)

## Release

Final edition; content cutoff: 2026-09-15.

Revision: FINAL-r3 (MATH000 span notation sync, 2026-09-16).
Previous revision: FINAL-r2 (Past Problems editorial review).
Baseline: GitHub commit `d73aca318128d6f8704a530171c06e2c75f40f4d`.

All 103 past problems and 106 solution blocks were reviewed. Problem IDs,
order, choices, and historical context are preserved. Four question passages
received limited grammar or definition corrections. Solutions were refined
for accuracy, first-year readability, notation, and explicit references to
the study material. The study chapters retain their content; twenty reference
labels were added without changing their printed statements or structure.

The September 15 content cutoff is unchanged. This editorial revision does
not add September 16 events.

Build with `latexmk -xelatex MathematicsforScienceQuiz.tex`.

FINAL-r3 changes only the printed linear-span operator from `Span` to `span`,
following MATH000 commit `271808b83999990c278b910fb6d36fd3a80b82f1`.
The command remains `\Span`; seven occurrences update through its definition.
The content cutoff and all other textbook source content are unchanged.
