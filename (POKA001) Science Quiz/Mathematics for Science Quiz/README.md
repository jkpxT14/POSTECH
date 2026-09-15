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

Build with `latexmk -xelatex MathematicsforScienceQuiz.tex`.
