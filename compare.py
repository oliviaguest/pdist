"""Compare C and Python implementations."""

from __future__ import division, print_function

from dist import c_mean_dist
from dist import cdist

import sklearn.datasets

import numpy as np

from time import time
from scipy.spatial.distance import pdist
from geopy.distance import great_circle


if __name__ == "__main__":
    # Generate some data:
    N = 3000
    centers = [[0, 0], [1, 0], [0.5, np.sqrt(0.75)]]
    cluster_std = [0.3, 0.3, 0.3]
    n_clusters = len(centers)
    n_samples = int(0.75 * N)
    data, labels_true = sklearn.datasets.make_blobs(n_samples=n_samples,
                                                    centers=centers,
                                                    cluster_std=cluster_std)
    centers = [[0.5, np.sqrt(0.75)]]
    cluster_std = [0.3]
    n_clusters = len(centers)
    extra, labels_true = sklearn.datasets.make_blobs(n_samples=int(0.25 * N),
                                                     centers=centers,
                                                     cluster_std=cluster_std)
    X = np.concatenate((data, extra), axis=0)
    N = X.shape[0]

    # Pick some random floats for the counts/weights:
    counts = np.random.random_sample((N,)) * 10
    # counts = np.ones((N,))

    ##########################################################################
    # SciPy:
    # Comment this out if you use a high N as it will eat RAM!
    t = time()
    Y = pdist(X, metric=cdist)
    weights = [counts[i] * counts[j]
               for i in xrange(N - 1) for j in xrange(i + 1, N)]
    scipy_sum = np.sum(weights * Y)
    N = counts.sum()
    N_unique_pairs = N * (N - 1.0) / 2.0
    scipy_mean = scipy_sum / N_unique_pairs
    print('SciPy:\t\t{} s'.format(time() - t))
    ##########################################################################

    ##########################################################################
    # C & Cython:
    t = time()
    c_mean = c_mean_dist(X, counts)
    print('C:\t\t{} s'.format(time() - t))

    # There is minor rounding error, but check for equality:
    assert np.round(scipy_mean) == np.round(c_mean)
    print('Both:\tmean = {}'.format(c_mean))

    # Compary to GeoPy:
    X = [[0, 10], [4, 2]]
    t = time()
    print('C:\t\tDistance = {} km'.format(cdist(X[0], X[1])))
    print('C:\t\t{} s'.format(time() - t))
    t = time()
    print('GeoPy:\t\tDistance = {}'.format(great_circle(X[0], X[1])))
    print('GeoPy:\t\t{} s'.format(time() - t))
