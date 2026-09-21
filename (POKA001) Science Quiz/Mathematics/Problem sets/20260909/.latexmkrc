# 20260909 Problem set build configuration.
# LaTeX Workshop commonly invokes `latexmk -pdf`; redirect that rule to XeLaTeX
# so Korean text uses the intended OpenType fonts without any editor changes.
$pdflatex = 'xelatex %O %S';
