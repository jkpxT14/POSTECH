# Final Review

Mathematics for Science Quiz - 2026 KAIST-POSTECH Science War.

Second Edition, September 16, 2026.

## Purpose

A result-first Korean final-review document designed for one 2-3 hour pass immediately before the Science Quiz. It is intentionally broader than the first edition: famous and reusable formulas, theorem conclusions, mathematician/prize/history associations, and the September 15, 2026 current-mathematics cutoff are retained, while routine basics, long proofs, highly technical detail, and olympiad-specific tricks are removed.

## Baselines

- `(MATH000) Mathematical Language`, current GitHub main on September 16, 2026.
- `Mathematics for Science Quiz`, 2026 Science War Edition, FINAL-r3, content cutoff September 15, 2026.
- All 103 past-problem directions were used as a coverage check; reusable kernels are absorbed into Parts I-II rather than reproduced as a Past Problems part.

## Language and notation

The document is Korean-first. English is retained for structural headings, canonical names, theorem/prize names, and mathematical terms when it improves recognition. Notation follows MATH000: bold lowercase vectors, bold uppercase matrices, upright differential, `span` as the linear-span operator, blackboard-bold probability/expectation, and the current probability/complex-analysis conventions.

## Build

```bash
latexmk -xelatex -interaction=nonstopmode -halt-on-error FinalReview.tex
```

XeLaTeX is required. The style prefers TeX Live's UnBatang/UnDotum and falls back to standard Windows Korean fonts. No font files are bundled.
