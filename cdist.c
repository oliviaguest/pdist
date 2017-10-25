#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// IUGG mean earth radius in kilometers, from
// https://en.wikipedia.org/wiki/Earth_radius#Mean_radius.  Using a
// sphere with this radius results in an error of up to about 0.5%.
#define R 6371.009
#define TO_RAD (3.1415926536 / 180)

double dist(double th1, double ph1, double th2, double ph2)
// From: https://rosettacode.org/wiki/Haversine_formula#C
{
	double dx, dy, dz;
	ph1 -= ph2;
	ph1 *= TO_RAD, th1 *= TO_RAD, th2 *= TO_RAD;

	dz = sin(th1) - sin(th2);
	dx = cos(ph1) * cos(th1) - cos(th2);
	dy = sin(ph1) * cos(th1);
	return asin(sqrt(dx * dx + dy * dy + dz * dz) / 2) * 2 * R;
}

double calculate_mean_distances(int n,
  double* lat, double* lon, double* weight)
{
  int i, j;
  double sum_dist = 0, N = 0;

  for (i = 0; i < n-1; i++)
  {
    for (j = i+1; j < n; j++)
    {
      sum_dist += dist(lat[i],lon[i],lat[j],lon[j]) * weight[i] * weight[j];
    }
  }
  for (i = 0; i < n; i++)
  {
    N += weight[i];
  }
	// double N_unique_pairs = (((N - 1)*(N - 1) + (N + 1)) / 2.0 + N);
  double N_unique_pairs = N * (N - 1.0) / 2.0;
  return sum_dist / N_unique_pairs;
}
