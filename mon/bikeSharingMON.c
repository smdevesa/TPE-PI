#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "../lib/frontLib.h"
#include "../adt/stationsADT.h"
#define MAXBUFFER 256
#define BIKES_FIELDS 5
#define STATIONS_FIELDS 4 

/* Se piden los nombres de los archivos .csv como entrada */
int main(int argc, char ** argv)
{
    if(argc != 3)
    {
        fprintf(stderr, "ERROR: The amount of files recieved is invalid.");
        exit(1);
    }

    FILE * FBikes = fopen(argv[1], "r");
    FILE * FStations = fopen(argv[2], "r");
    
    if(FBikes == NULL || FStations == NULL)
    {
        fprintf(stderr, "ERROR: The file cant be opened.");
        exit(1);
    }
   
    char line[MAXBUFFER];
    fgets(line, MAXBUFFER, FBikes);
    fgets(line, MAXBUFFER, FStations);

    stationsADT st = newStationsADT();

    char ** ans;
    int flag=0;
    int j=0;

    while(fgets(line,MAXBUFFER, FStations) != NULL)
    {
        printf("%d\n",j);
        j++;
        ans = getField(line, STATIONS_FIELDS);
        flag = addStation(st, atoi(ans[0]), ans[1]);
        if(flag == -1)
        {
            fprintf(stderr, "ERROR: Memory cant be allocated");
            exit(1);
        }
        for(int i=0; i < STATIONS_FIELDS; i++)
        {
            free(ans[i]);
        }
        free(ans);
    }

    while(fgets(line,MAXBUFFER, FBikes) != NULL)
    {
        printf("%d\n",j);
        j++;
        ans = getField(line, BIKES_FIELDS);
        flag = addRide(st, atoi(ans[1]), atoi(ans[3]),atoi(ans[4]),ans[0]);
        if(flag == -1)
        {
            fprintf(stderr, "ERROR: Memory cant be allocated");
            exit(1);
        }
        for(int i=0; i < BIKES_FIELDS; i++)
        {
            free(ans[i]);
        }
        free(ans);
    }

    freeStations(st);
    fclose(FStations);
    fclose(FBikes);

    return 0;
}
