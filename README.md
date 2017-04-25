# pdist

## Usage
This code takes a set of 2D data points ```X``` and calculates the mean of the pairwise weighted distances between points using the great circle metric.

To call use:
``` python
mean_distances = c_mean_dist(X, counts)
```

## Basic Information
The C and Python code were written by Olivia Guest, using https://equatorialmaths.wordpress.com/2010/10/16/python-extensions-with-c-libraries-made-easy-by-cython/ to call C functions from Python.

If you want to use this function from outside this directory, e.g., ```import```. I have not yet found a way of doing so without adding the path to the library to ```LD_LIBRARY_PATH```, e.g., ```export LD_LIBRARY_PATH=/local/path/to/this/repo```.

## Background
There were  many attenpts, see: https://github.com/oliviaguest/pairwise_distance to make this work Python-only. Alas, none of them worked out, but feel free to play around with the various Python versions. The main stumbling block was the GIL.
