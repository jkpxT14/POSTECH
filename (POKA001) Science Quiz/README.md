# Mathematics for Science Quiz

Archiving materials for the POSTECH-KAIST Science War Science Quiz event.

This folder includes past questions, study notes, game rules, and preparation
materials across mathematics and computer science. Its purpose is to provide a
structured archive that supports efficient review and preparation for the
competition.

Provider: Jungwoo Kim (Department of Mathematics, Pohang University of Science
and Technology)

## Main document

The complete mathematics handbook is built from
`MathematicsforScienceQuiz.tex`. The remaining top-level `.tex` files preserve
the actual structure of the book: Mathematical Concepts, Facts to Memorize,
Mathematics in 2026, Past Problems, Expected Problems, Notation and
Conventions, Preface, and Acknowledgements.

Compile from this folder with:

```bash
latexmk -pdf -synctex=1 MathematicsforScienceQuiz.tex
```

## Figures

Figure assets are stored under `Figures/` in a hierarchy parallel to the
book's actual files, chapters, sections, and problem identifiers. Each finished
figure keeps one editable vector source and one PDF used by the main document.
Shared TikZ styling is stored in `Figures/Styles/TikZ.tex`.

For example, compile the figure for Past Problem 2024.3 from this folder with:

```bash
pdflatex -interaction=nonstopmode -halt-on-error \
  Figures/PastProblems/Problem2024.3/ProblemFigure.tex
```
