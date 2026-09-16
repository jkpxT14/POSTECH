# Final Review

Mathematics for Science Quiz - 2026 KAIST-POSTECH Science War.

Second Edition, September 15, 2026.

## Purpose

A Korean-first, result-first final-review document designed for one 2-3 hour pass immediately before the Science Quiz. The target is not textbook completeness: retain famous, reusable, distinctive, and directly applicable results; remove routine basics, long proofs, highly technical local details, and olympiad-specific tricks.

Coverage was checked against the handbook's 103 past-problem directions, the dated 2026 problem sets, and the Expected Problems collection. Reusable kernels are absorbed into the relevant chapters; problem statements and long solutions are not reproduced.

## Baselines

- `(MATH000) Mathematical Language`, current notation baseline used by the September 15 edition.
- `Mathematics for Science Quiz`, 2026 Science War Edition, FINAL-r3, content cutoff September 15, 2026.
- Current-mathematics claims are kept at the September 15, 2026 status boundary.

## Language and notation

This is a Korean document. English is retained mainly for Part/Chapter/Section titles, canonical theorem/prize/problem names, proper names, and mathematical keywords when recognition benefits from it. Explanatory prose, table headers, memory cues, and result statements are Korean-first.

Notation follows MATH000: bold lowercase vectors, bold uppercase matrices, upright differential, lowercase `span`, blackboard-bold probability/expectation, the current geometric-distribution convention, and the current complex `Arg`/`Log` conventions.

## Build

```bash
latexmk -xelatex -interaction=nonstopmode -halt-on-error FinalReview.tex
```

XeLaTeX is required. The style prefers TeX Live's UnBatang/UnDotum and falls back to standard Windows Korean fonts. No font files are bundled.
