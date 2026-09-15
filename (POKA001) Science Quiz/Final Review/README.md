# Final Review

**Subtitle:** Mathematics for Science Quiz  
**Version:** First Edition / v1.0.1  
**Date:** September 16, 2026

This package is the Korean final-review document for the 2026 KAIST-POSTECH Science War Science Quiz mathematics preparation pipeline:

1. `Mathematics for Science Quiz` - main handbook (English)
2. `Expected Problems` - N-set / expected problems (English)
3. `Problem sets` - mock examinations (Korean)
4. `Final Review` - final retrieval and memorization document (Korean)

## Source base

The content was selected from the GitHub `main` version of:

`(POKA001) Science Quiz/Mathematics for Science Quiz/`

- Edition: **2026 Science War Edition (September 15, 2026)**
- Content cutoff: **September 15, 2026**
- Repository state checked: **September 16, 2026**
- Latest repository commit checked while preparing this edition: `fd1826695f1d9b6d99466ed978ebaeabcff88fa2`
- The handbook README records the MATH000 notation baseline as commit `e573739329291e71c779b4d9b4be5ff3fcd60920`.

## Editorial policy

The handbook structure is preserved through Parts I-III and Chapters 1-20. The handbook's `Past Problems` part is not reproduced; reusable past-problem ideas are absorbed into the relevant mathematical or factual locations.

The Final Review deliberately prioritizes:

- reusable past-problem kernels;
- one standard step beyond past problems;
- short, distinctive formulas and structures;
- famous mathematicians with strong quiz anchors;
- memorable history and anecdotes;
- 2025-2026 mathematics news with precise status labels.

Proof-heavy, highly local, and olympiad-specific material is intentionally omitted even when it remains valuable in the full handbook.

## Files

- `FinalReview.tex` - main document
- `FinalReviewStyle.tex` - typography and layout
- `MathematicalConcepts.tex` - Part I
- `FactstoMemorize.tex` - Part II
- `Mathematicsin2026.tex` - Part III
- `FinalReview.pdf` - compiled document
- `.latexmkrc` - XeLaTeX build configuration

## Build

```bash
latexmk -xelatex FinalReview.tex
```

The source no longer requires Nanum or Noto CJK fonts to be installed in the operating system. It prefers `UnBatang` / `UnDotum`, which are distributed through TeX Live's `unfonts-core` package, and falls back to standard Windows Korean fonts when necessary. On a minimal TeX Live installation, if the Un fonts are absent, install them with:

```bash
tlmgr install unfonts-core
```

### v1.0.1 build fix

This patch changes only Korean-font portability. The selected mathematics content, structure, and editorial scope are unchanged from the First Edition v1.0.

The document uses English Part/Chapter/Section headings and Korean explanatory text. Korean personal names are used when a standard Korean form is established; the canonical English name is supplied when useful for identification.
