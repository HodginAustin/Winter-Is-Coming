default: all

doc:
	pdflatex techreview.tex
	bibtex techreview
	pdflatex techreview.tex
	pdflatex techreview.tex
	#dvips techreview.dvi
	#ps2pdf techreview.ps

all: doc clean


clean:
	rm -f *.o *.exe
	rm -f techreview.aux
	rm -f techreview.dvi
	rm -f techreview.log
	rm -f techreview.out
	rm -f techreview.ps
	rm -f techreview.toc
	rm -f techreview.bbl
	rm -f techreview.blg
