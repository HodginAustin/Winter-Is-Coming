default: all

doc:
	latex writeup.tex
	dvips writeup.dvi
	ps2pdf writeup.ps

all: doc clean


clean:
	rm -f *.o *.exe
	rm -f writeup.aux
	rm -f writeup.dvi
	rm -f writeup.log
	rm -f writeup.out
	rm -f writeup.ps