cdef extern from "cdist.h":
  double dist(double, double, double, double)
  double calculate_mean_distances(int, double*, double*, double*)

from stdlib cimport *

def cdist(a, b):
  d = dist(a[0], a[1], b[0], b[1])
  return d

def c_mean_dist(X, w):
  N = X.shape[0]
  lat = <double *>malloc(N*sizeof(double))
  for i in range(N):
    lat[i] = float(X[i,0])
  lon = <double *>malloc(N*sizeof(double))
  for i in range(N):
    lon[i] = float(X[i,1])
  weight = <double *>malloc(N*sizeof(double))
  for i in range(N):
    weight[i] = float(w[i])
  mean = calculate_mean_distances(N, lat, lon, weight)
  free(lat)
  free(lon)
  free(weight)
  return mean
