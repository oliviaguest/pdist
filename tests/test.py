"""Test C against static pre-calculated examples and Python implementations."""

from __future__ import division, print_function

import unittest
import sklearn.datasets
import numpy as np

from pdist import cdist, c_mean_dist
from scipy.spatial.distance import pdist
from geopy.distance import great_circle


class TestPdist(unittest.TestCase):
    """Test pdist."""

    def test_sum_of_distances(self):
        """Test the sum of distances calculation."""
        N = 10
        centers = [[0, 0], [1, 0], [0.5, np.sqrt(0.75)]]
        cluster_std = [0.3, 0.3, 0.3]
        n_samples = int(0.75 * N)
        data, labels_true = \
            sklearn.datasets.make_blobs(n_samples=n_samples,
                                        centers=centers,
                                        cluster_std=cluster_std)
        centers = [[0.5, np.sqrt(0.75)]]
        cluster_std = [0.3]
        extra, labels_true = \
            sklearn.datasets.make_blobs(n_samples=int(0.25 * N),
                                        centers=centers,
                                        cluster_std=cluster_std)
        X = np.concatenate((data, extra), axis=0)
        N = X.shape[0]

        # Pick some random floats for the counts/weights:
        counts = np.random.random_sample((N,)) * 10

        # SciPy:
        Y = pdist(X, metric=cdist)
        weights = [counts[i] * counts[j]
                   for i in xrange(N - 1) for j in xrange(i + 1, N)]
        scipy_sum = np.sum(weights * Y)
        N = counts.sum()
        N_unique_pairs = N * (N - 1.0) / 2.0
        scipy_mean = scipy_sum / N_unique_pairs

        # C & Cython:
        c_mean = c_mean_dist(X, counts)

        # There is minor rounding error, but check for equality:
        self.assertTrue(np.isclose(c_mean, scipy_mean))
        # Even though above is comparing the means, it is actually checking the
        # sums are correct as the means are calculated in the same way, i.e.,
        # by dividing by the same number, N_unique_pairs.

    def test_mean_of_distances(self):
        """Test the mean of distances calculation (and the sum)."""
        X = np.array([[0.3, 0.4],
                      [0.1, 4.0],
                      [2.0, 1.0],
                      [0.0, 0.5]])
        counts = np.array([3, 2, 1, 2])
        scipy_X = []
        for c, count in enumerate(counts):
            for i in range(count):
                scipy_X.append(X[c])

        # SciPy:
        Y = pdist(scipy_X, metric=cdist)
        scipy_N = np.sum(counts)
        N_unique_pairs = scipy_N * (scipy_N - 1.0) / 2.0
        scipy_mean = Y.mean()
        self.assertTrue(Y.shape[0] == N_unique_pairs)
        self.assertTrue(scipy_mean == (np.sum(Y) / N_unique_pairs))

        # C & Cython:
        c_mean = c_mean_dist(X, counts)
        self.assertTrue(np.isclose(c_mean, scipy_mean))

    def test_dist_itslef(self):
        """Compary cdist to GeoPy."""
        X = [[0, 10], [4, 2]]  # Just some points. I've no idea where on globe.
        c = cdist(X[0], X[1])
        string_geopy = '{}'.format(great_circle(X[0], X[1]))
        float_geopy = float(string_geopy[:-3])
        self.assertTrue(np.round(c) == np.round(float_geopy))

        X = [[34.0522, 118.2437],  # Lon Angeles
             [37.7749, 122.4194]]  # San Francisco
        c = cdist(X[0], X[1])
        string_geopy = '{}'.format(great_circle(X[0], X[1]))
        float_geopy = float(string_geopy[:-3])
        self.assertTrue(np.round(c) == np.round(float_geopy))


if __name__ == '__main__':
    unittest.main()
