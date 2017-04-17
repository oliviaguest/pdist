#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

const char* get_columns(char* line, int column_lat, int column_lon,
                                    double* lat, double* lon, int num)
{
    const char* tok;
    for (tok = strtok(line, ",");
            tok && *tok;
            tok = strtok(NULL, ",\n"))
    {

        if (!--column_lat) // if we are at position for latitude
        {
          // printf("%s\n", tok);
          lat[num] = strtof(tok,0);
        }
        if (!--column_lon) // if we are at position for longitude
        {
          lon[num] = strtof(tok,0);
        }
        // if (!--pos_pop) // if we are at position for population
        // {
        //   printf("%s\n", tok);
        // }
    }
    return NULL;
 }

int main()
{
    FILE* stream = fopen("input.csv", "r");
    char line[1024];

    int N = -1; // we want to skip first line
    while (fgets(line, 1024, stream))
    {
      N++;
    }
    printf("%i\n", N);
    stream = fopen("input.csv", "r"); // re-open to go back to start of file

    double lon[N];
    double lat[N];
    int counter = 0;
    int lat_column = 3, lon_column = 4;
    while (fgets(line, 1024, stream))
    {
        char* tmp = strdup(line);
        if (counter) // skip first row of csv
        {
          lon[counter-1] = 999.0;
          get_columns(tmp, lat_column, lon_column, lat, lon, counter-1);
          // printf("%f,%f\n", lat[counter-1],lon[counter-1]);

        }
        // NOTE strtok clobbers tmp
        free(tmp);
        counter++;

    }
    fclose(stream);
    // Start the actual calculation of distances here:
    clock_t start = clock(), diff;

    int i, j;
    double d;
    int N_unique_pairs = (((N - 1)*(N - 1) + (N + 1)) / 2);
    double sum_dist[N_unique_pairs];
    counter = 0;
    for (i = 0; i < N-1; i++)
    {
      for (j = i+1; j < N; j++)
      {
        sum_dist[counter] = dist(lat[i],lon[i],lat[j],lon[j]);
        counter++;
      }
    }
    // d = dist(36.12, -86.67, 33.94, -118.4);
    // printf("dist: %.1f km (%.1f mi.)\n", d, d / 1.609344);

    diff = clock() - start;
    int msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Time taken %d seconds %d milliseconds", msec/1000, msec%1000);
    return 0;
}
