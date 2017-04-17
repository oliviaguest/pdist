#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define LINE_LENGTH 1024
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

int calculate_mean_distances(int N,
  double* lat, double* lon, double* membership, double* population)
{
  int i, j, counter = 0;
  double sum_dist;
  int N_unique_pairs = (((N - 1)*(N - 1) + (N + 1)) / 2);
  printf("%i\n", N_unique_pairs);
  printf("%i\n", N);


  for (i = 0; i < N-1; i++)
  {
    for (j = i+1; j < N; j++)
    {
      sum_dist += dist(lat[i],lon[i],lat[j],lon[j])
                  * population[i] * population[j];
      counter++;

    }
  }
  return sum_dist / N_unique_pairs;
}
void get_N_clusters(char* line, int num, int membership_column, double* membership)
{
  const char* tok;
  for (tok = strtok(line, ",");
          tok && *tok;
          tok = strtok(NULL, ",\n"))
  {
    if (!--membership_column) // if we are at position for membership
    {
      membership[num] = strtof(tok,0);
      printf("%f\n", membership[num]);
    }
  }
}
const char* get_columns(char* line, int num,
  int column_lat, int column_lon, int membership_column, int population_column,
  double* lat, double* lon, double* membership, double* population)
{
    const char* tok;
    for (tok = strtok(line, ",");
            tok && *tok;
            tok = strtok(NULL, ",\n"))
    {

        if (!--column_lat) // if we are at position for latitude
        {
          lat[num] = strtof(tok,0);
        }
        if (!--column_lon) // if we are at position for longitude
        {
          lon[num] = strtof(tok,0);
        }
        if (!--membership_column) // if we are at position for membership
        {
          membership[num] = strtof(tok,0);
          printf("%f\n", membership[num]);
        }
        if (!--population_column) // if we are at position for population
        {
          population[num] = strtof(tok,0);
        }
    }
    return NULL;
 }

int main()
{
    FILE* stream = fopen("RI.csv", "r");
    char line[1024];

    int counter = 0;
    int lat_column = 4, lon_column = 5,
      membership_column = 6, population_column = 9;
    int N = -1; // we want to skip first line
    while (fgets(line, LINE_LENGTH, stream))
    {

      N++; // we need to know the number of points/blocks


    }
    printf("%i\n", N);
    double lon[N], lat[N], membership[N], population[N];

    stream = fopen("RI.csv", "r"); // re-open to go back to start of file

    while (fgets(line, LINE_LENGTH, stream))
    {
      char* tmp = strdup(line);

      free(tmp);
      get_N_clusters(tmp, counter-1, membership_column, membership);


    }
    printf("%i\n", N);
    stream = fopen("RI.csv", "r"); // re-open to go back to start of file
    while (fgets(line, LINE_LENGTH, stream))
    {
        char* tmp = strdup(line);
        if (counter) // skip first row of csv
        {
          // Columns/first line has structure:
          // ,Alpha,Beta,Centroid Latitude,Centroid Longitude,Cluster,Congressional District,GEOID,Predicted 2015 Population
          // We are interested in the 4th, 5th, 6th or 7th, and 9th:
          get_columns(tmp, counter-1,
            lat_column, lon_column, membership_column, population_column,
            lat, lon, membership, population);
          printf("%f,%f,%f,%f\n",
            lat[counter-1],lon[counter-1],
            membership[counter-1], population[counter-1]);

        }
        // else // this line (the 1st) tells us about what each column is
        // {
          // lat_column, lon_column, membership_column, population_column,
          // NULL; // fill in as appropriate
        // }
        // NOTE strtok clobbers tmp
        free(tmp);
        counter++;

    }

    fclose(stream);
    int mean_distances;
    // Start the (timing of) calculation of distances here:
    clock_t start = clock(), diff;
    mean_distances = calculate_mean_distances(N, lat, lon, membership, population);
    diff = clock() - start;
    int msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Time taken %d seconds %d milliseconds", msec/1000, msec%1000);
    return 0;
}
