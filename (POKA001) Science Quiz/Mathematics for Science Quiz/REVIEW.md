# FINAL-r2 review record

Edition cutoff: 2026-09-15. Editorial revision: 2026-09-16.
Baseline GitHub commit: d73aca318128d6f8704a530171c06e2c75f40f4d.
The baseline source files matched the preceding FINAL ZIP byte for byte.

## Review and changes

- Reviewed all 103 past problems and 106 solution blocks.
- Preserved every problem ID, order, choice, and historical answer context.
- Limited question corrections to 2011.1 (historical future tense), 2011.2
  (grammar), 2011.10 (orthogonality in the Hadamard-matrix hint), and 2025.13
  (maximality in the definition of a run).
- Refined explanations and wording in 57 problem blocks, including endpoint convergence,
  eigenvalue multiplicities, probability models, geometric waiting times,
  and the direct derivative argument in 2023.4.
- Clarified the historical intended answer in the 2020 Villani question;
  parliamentary service does not itself negate a professorship.
- Added 20 invisible labels to existing results for precise solution references.
- Aligned revised notation with MATH000, retaining explicit row-vector
  conventions when they are part of a question.

## Validation

- Question-block comparison confirmed that only the four passages above changed.
- All study-chapter printed text and Chapter/Section structure are unchanged.
- PDF text before Past Problems matches the preceding FINAL PDF exactly.
- Major time-sensitive September entries were spot-checked against official
  announcements, including the Navier--Stokes announcement and its pending
  Clay evaluation, Fermat formalization, and conditional PrimeGaps186 project.
  This is an editorial review, not a new exhaustive literature survey.
- Rebuilt with XeLaTeX/latexmk: 282 PDF pages, no build warnings, missing
  characters, undefined references, or overfull boxes in the final log.
- Inspected all 48 Past Problems PDF pages for layout and symbol rendering.
- Independently checked selected arithmetic/combinatorial results, including
  2011.7, 2011.8, 2023.2, 2024.1, 2025.6, and 2025.13.

## Rebuilding

Run `latexmk -xelatex MathematicsforScienceQuiz.tex` in this folder.
The source and compiled PDF in this package represent the same revision.
No September 16 events have been added to the September 15 edition.
