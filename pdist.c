#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* get_columns(char* line, int pos_lat, int pos_lon,
                                    float* lat, float* lon, int num)
{
    const char* tok;
    for (tok = strtok(line, ",");
            tok && *tok;
            tok = strtok(NULL, ",\n"))
    {

        if (!--pos_lat) // if we are at position for latitude
        {
          // printf("%s\n", tok);
          lat[num] = strtof(tok,0);
        }
        if (!--pos_lon) // if we are at position for longitude
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

    int length = -1; // we want to skip first line
    while (fgets(line, 1024, stream))
    {
      length++;
    }
    printf("%i\n", length);
    // fclose(stream);
    stream = fopen("input.csv", "r"); // re-open to go back to start of file

    float lon[length];
    float lat[length];
    int counter = 0;
    while (fgets(line, 1024, stream))
    {
        char* tmp = strdup(line);
        if (counter) // skip first row of csv
        {
          lon[counter-1] = 999.0;
          get_columns(tmp, 3, 4, lat, lon, counter-1);
          printf("%f,%f\n", lat[counter-1],lon[counter-1]);

        }
        // printf("Field 3 would be %s\n", getfield(tmp, 3));

        // NOTE strtok clobbers tmp
        free(tmp);
        counter++;

    }
    // printf("%f\n", lon[0]);
}
