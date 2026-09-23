# 20260913 Problem set build configuration.
# LaTeX Workshop commonly invokes latexmk -pdf; redirect that rule to XeLaTeX
# so Korean text uses the intended OpenType fonts without editor-specific setup.
$pdflatex = 'xelatex -synctex=1 %O %S';
