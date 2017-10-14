# pdist

## Usage
### Mean of Pairwise Weighted Distances Using Great Circle
This code takes a set of 2D data points ```X``` and calculates the mean of the pairwise weighted distances between points using the great circle metric.

To call use:
``` python
mean_distances = c_mean_dist(X, weights)
```
where ```X``` are your data points, and ```weights``` are the weights or counts (depending on how you want to conceptualise them).
Weights affect the mean of the pairwise distances in the same as including more of the point which the weight corresponds to.
So if a data point with value (0, 1) has a weight of 2, the average pairwise distances will be affected in the same way as if you had added another data point with value (0, 1) to ```X``` and had set both their weights to 1.

### Great Circle Distance
It also implements great circle, also known as orthodromic or geodesic, distance metric faster than GeoPy in ```cdist```.

## Example
For an example of both functions see ```compare.py```.

## Information
The C and Python code were written by Olivia Guest — using [this tutorial]( https://equatorialmaths.wordpress.com/2010/10/16/python-extensions-with-c-libraries-made-easy-by-cython/) by Dmitrii V Pasechnik to call C functions from Python using Cython, and using the [haversine function from Rosetta Code
](https://rosettacode.org/wiki/Haversine_formula#C).

## Installation
Make sure you have Cython and its dependencies installed.
Run ```make```.
Subsequently, run ```python compare.py``` to confirm compilation, and to see the comparison between using the C version and using a Python-only way.
See ```requirements.txt``` in case you need to install GeoPy, etc.

If you want to use this function from outside this directory, e.g., ```import```. I have not yet found a way of doing so without adding the path to the library to ```LD_LIBRARY_PATH```, e.g., ```export LD_LIBRARY_PATH=/local/path/to/this/repo```.
For adding it permanently (so you do not have to do this this every time) add it to your ```~/.bashrc``` or whatever your set-up dictates.

## Notes
There were  many attenpts, see: https://github.com/oliviaguest/pairwise_distance to make this work Python-only.
Alas — none of them worked out, but feel free to play around with the various Python versions.
The main stumbling block was the GIL.

Can memory efficiency be improved even more? See: http://cython.readthedocs.io/en/latest/src/tutorial/numpy.html
