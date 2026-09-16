# Mathematics for Science Quiz

This folder contains the complete mathematics handbook for the Science Quiz
section of the POSTECH-KAIST Science War.

Edition: 2026 Science War Edition (September 15, 2026)

The time-sensitive material is current through September 15, 2026. The notation
follows `(MATH000) Mathematical Language`, including the lowercase `span`
operator synchronized in FINAL-r3 at MATH000 commit
`271808b83999990c278b910fb6d36fd3a80b82f1`.

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

Revision: FINAL-r4 (problem-based study-material supplements, 2026-09-16).
Previous revision: FINAL-r3 (MATH000 span notation sync).
Baseline: GitHub commit `568c78e43f1ab7a3fc206e6602f4cafede6adaae`.

The preceding FINAL-r2 review covered all 103 past problems and 106 solution blocks. Problem IDs,
order, choices, and historical context are preserved. Four question passages
received limited grammar or definition corrections. Solutions were refined
for accuracy, first-year readability, notation, and explicit references to
the study material. In FINAL-r2, twenty reference labels were added to the study chapters without
changing their printed statements or structure.

The September 15 content cutoff is unchanged. This editorial revision does
not add September 16 events.

Build with `latexmk -xelatex MathematicsforScienceQuiz.tex`.

FINAL-r3 changes only the printed linear-span operator from `Span` to `span`,
following MATH000 commit `271808b83999990c278b910fb6d36fd3a80b82f1`.
The command remains `\Span`; seven occurrences update through its definition.
The content cutoff and all other textbook source content are unchanged.

## FINAL-r4 supplements

This revision adds or expands reusable study material identified by the PPTX,
Problem-set, and Past Problems audit. There are 20 targeted insertions or
expansions across MathematicalConcepts.tex and Mathematicsin2026.tex.
No Chapter or Section was added, removed, or renamed. PastProblems.tex, the
notation source, and the main formatting source remain byte-identical to the
pinned baseline. Final Review and Problem sets are outside this package.

See REVIEW.md for validation.
This clean package retains the FINAL-r4 textbook content and omits auxiliary
release records.
The compiled PDF is included; build inputs and editable figures are retained.
Temporary build products and SyncTeX files are omitted.
