# Code by: Dima Pasechnik (@dimpase)
# Modified by: Olivia Guest
# Original code here:
# https://github.com/oliviaguest/pairwise_distance/issues/2#issuecomment-294340333

from __future__ import division, print_function
from dist import c_mean_dist
from dist import cdist

def pdistsum(X, w, dist, k=1):
    """
    compute distances, given by dist(,), between elements of X, weighted by w
    using k threads
    """
    import threading
    from math import sqrt, ceil
    N = len(X)
    results = [None]*k

    def rsum(r0, r1, num):
        s = 0.0 # partial sums
        c = 0 # partial counts
        # print("rsum instance:", str(num) + " from " + str(r0)+ " to " + str(r1))
        for i in xrange(r0,r1):
            for j in xrange(i+1,N):
                c += 1
                s += dist(X[i],X[j]) * w[i] * w[j]
        results[num]=(s,c)
        return

    # k+1 ranges: 0,N(1-sqrt((k-1)/k)),N(1-sqrt((k-2)/k)),...,N
    r = [0]+map(lambda i: int(ceil(N*(1-sqrt((k-i)/k)))), xrange(1,k))+[N]
    threads = []
    for i in range(k):
        t = threading.Thread(target=rsum, args=(r[i],r[i+1],i,))
        threads.append(t)
        t.start()
    for i in range(k):
        threads[i].join()
    total_sum = 0
    for result in results:
        total_sum += result[0]
    N = w.sum()
    total_mean = total_sum / (((N - 1)**2 + (N + 1)) / 2 + N)
    return total_sum, total_mean

### testing ####
def dfun(a,b): #  example of dist() between 2 numbers
    # from geopy.distance import great_circle
    # return great_circle(a,b).km
    return cdist(a, b)

if __name__ == "__main__":
    import sklearn.datasets
    import scipy.spatial.distance

    import numpy as np

    from time import time
    from scipy.spatial.distance import pdist

    # Generate some data:
    N = 4000
    centers = [[0, 0], [1, 0], [0.5, np.sqrt(0.75)]]
    cluster_std = [0.3, 0.3, 0.3]
    n_clusters = len(centers)
    n_samples = int(0.75 * N)
    data, labels_true = sklearn.datasets.make_blobs(n_samples=n_samples,
        centers=centers, cluster_std=cluster_std)
    centers = [[0.5, np.sqrt(0.75)]]
    cluster_std = [0.3]
    n_clusters = len(centers)
    extra, labels_true = sklearn.datasets.make_blobs(n_samples=int(0.25 * N),
        centers=centers, cluster_std=cluster_std)
    X = np.concatenate((data, extra), axis=0)
    N = X.shape[0]

    # Pick some random floats for the counts/weights:
    counts = np.random.random_sample((N,)) * 10
    # counts = np.ones((N,))
    ##########################################################################
    # Parallel:
    # t = time()
    # parallel_sum, parallel_mean = pdistsum(X, counts, dfun, k=10) # example with 4 threads
    # print('parallel:\t{} s'.format(time() - t))
    ##########################################################################

    ##########################################################################
    # Serial:
    # Comment this out if you use a high N as it will eat RAM!
    # t = time()
    # Y = pdist(X, metric=dfun)
    # weights = [counts[i] * counts[j]
    #            for i in xrange(N - 1) for j in xrange(i + 1, N)]
    # serial_sum = np.sum(weights * Y)
    # N = counts.sum()
    # print(N, (((N - 1)**2 + (N + 1)) / 2 + N))
    # serial_mean = serial_sum / (((N - 1)**2 + (N + 1)) / 2 + N)
    # print('serial:\t\t{} s'.format(time() - t))
    ##########################################################################


    ##########################################################################
    # C & Cython:
    print('C time')
    t = time()
    c_mean = c_mean_dist(X, counts)
    print('C:\t\t{} s'.format(time() - t))
    # print(c_sum, serial_sum, parallel_sum)


    # There is minor rounding error, but check for equality:
    # assert np.round(serial_sum) == np.round(c_sum)
    # print('sum = {}'.format(c_sum))
    print(serial_mean, c_mean)
    assert np.round(serial_mean) == np.round(c_mean)
    print('mean = {}'.format(c_mean))
