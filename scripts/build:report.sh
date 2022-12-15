#!/usr/bin/env bash
mkdir -p ./build/report &&
cd docs/report &&
export $(cat .env | xargs) &&
pdflatex -output-directory=../../build/report ./report.tex;
bibtex ../../build/report/report.aux &&     # Whoever wrote bibtex needs to just stop.
pdflatex -output-directory=../../build/report ./report.tex && # Yep. Needed for references.
pdflatex -output-directory=../../build/report ./report.tex;   # Yep. Needed for bibtex.
