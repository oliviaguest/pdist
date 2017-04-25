# pdist

The C and Python code were written by Olivia Guest, using https://equatorialmaths.wordpress.com/2010/10/16/python-extensions-with-c-libraries-made-easy-by-cython/ to call C functions from Python.

If you want to use this function from outside this directory, e.g., ```import```. I have not yet found a way of doing so without adding the path to the library to ```LD_LIBRARY_PATH```, e.g., ```export LD_LIBRARY_PATH=/local/path/to/this/repo```.
