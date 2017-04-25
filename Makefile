# This Makefile is based on: https://github.com/dimpase/dyn/blob/master/Makefile
# Original version written by Dima Pasechnik
CC=cc
LFLAGS = -L. -Wl,-rpath=. -lcmean

# uncomment the following for building universal libraries on MacOSX
#CFLAGS = -arch ppc -arch i386
# (strictly speaking, not needed, but prevents a warning being shown)

# the following might be .out, or .exe, or still something esle, but usually empty...
EXE=

all:	dist.so

# building a shared C library libcmean.so
libcdist.so: cdist.c
	$(CC) $(CFLAGS) -fPIC -shared cdist.c -o libcdist.so

# building python extension calling a function from shared C library
dist.so:	cython_dist.pyx libcdist.so setup.py
	python setup.py build_ext --inplace --rpath=.

# running a Python test
runpy:	dist.so
	python test.py

# the following is just for comparison
# buiding a C test calling a function from C library
dync: main.c libcdist.so
	$(CC) $(CFLAGS) main.c -o distc$(EXE) $(LFLAGS)

# running a C test
runc: distc libcdist.so
	./distc$(EXE)

clean:
	rm -rf core *.out *.o *.so *.pyc *.dSYM distc$(EXE) m.c *~ build/
