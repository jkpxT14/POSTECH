# Final Review

Mathematics for Science Quiz - 2026 KAIST-POSTECH Science War.

Second Edition. September 15, 2026.

## Purpose

A Korean-first, result-first competition-day review document designed for repeated reading and a complete 2-3 hour pass immediately before Science Quiz. It is not a miniature textbook: it keeps the definitions, formulas, theorems, exact values, historical facts, prizes, people, works, and current-mathematics anchors that are plausible quiz material, while removing routine basics, long proofs, overly local technicalities, and low-reuse problem-specific trivia.

The final selection is based on the completed handbook and was audited against the mathematics questions in the attached rehearsal/simulation PPTX files, the dated 2026 Problem sets, and all 103 Past Problems. The audit is kernel-based: the document is meant to contain the reusable fact, formula, theorem, recognition cue, or first idea needed for a problem, not the problem statement or its full solution.

## Baselines

- `(MATH000) Mathematical Language`: notation and mathematical-language constitution.
- `Mathematics for Science Quiz`, 2026 Science War Edition, FINAL-r4, content cutoff September 15, 2026.
- FINAL-r4 includes the problem-based study-material supplements identified from the PPTX / Problem-set / Past-Problem audit.
- Current-mathematics claims remain bounded by September 15, 2026.

## Editorial policy

- Part / Chapter / Section structure follows the completed handbook; no decorative hierarchy is invented merely to subdivide the page.
- Part I is result-first: immediately usable formulas, standard theorems, exact values, and short computational tools dominate.
- Part II is chronology-first: years, lifetimes, prize years, works, historical events, firsts, refusals, unusual records, and other distinctive quiz anchors are emphasized.
- The complete Fields Medal chronology and complete Abel Prize chronology are retained in compact form.
- Standard Korean names are used where established; original-language or English names are retained when they improve identification or when no stable Korean form is preferable.
- The body is Korean. English is used mainly for official names, canonical theorem/problem names, source-language book titles, and international recognition terms.

## Notation

Notation follows MATH000: bold lowercase vectors, bold uppercase matrices, upright differential, lowercase `span`, blackboard-bold probability/expectation, current geometric-distribution convention, and the current complex `Arg`/`Log` conventions.

## Build

```bash
latexmk -xelatex -interaction=nonstopmode -halt-on-error FinalReview.tex
```

XeLaTeX is required. The style prefers TeX Live's UnBatang/UnDotum and falls back to standard Windows Korean fonts. No font files are bundled.
