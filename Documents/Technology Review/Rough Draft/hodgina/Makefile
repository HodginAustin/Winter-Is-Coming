default: all

doc:
	pdflatex writeup.tex
	bibtex writeup
	pdflatex writeup.tex
	pdflatex writeup.tex
	#dvips writeup.dvi
	#ps2pdf writeup.ps

all: doc clean


clean:
	rm -f *.o *.exe
	rm -f writeup.aux
	rm -f writeup.dvi
	rm -f writeup.log
	rm -f writeup.out
	rm -f writeup.ps
	rm -f writeup.toc
	rm -f writeup.bbl
	rm -f writeup.blg
