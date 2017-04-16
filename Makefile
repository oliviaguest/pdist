# HEADERS = pdist.h headers.h

default: pdist

pdist.o: pdist.c #$(HEADERS)
	gcc -c pdist.c -o pdist.o

pdist: pdist.o
	gcc pdist.o -o pdist

clean:
	-rm -f pdist.o
	-rm -f pdist
