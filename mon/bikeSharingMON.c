#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/frontLib.h"
#include "../adt/stationsADT.h"
#include "../lib/htmlTable.h"

#define MAXBUFFER 256
#define BIKES_FIELDS 5
#define STATIONS_FIELDS 4 

#define QUERY1_TABLE_NAME "query1.html"
#define QUERY1_CSV_NAME "query1.csv"
#define QUERY1_COLS 2
#define QUERY1_COL1 "Station"
#define QUERY1_COL2 "StartedTrips"

int main(int argc, char ** argv)
{
    /* Se comprueba que los archivos enviados sean 2 */
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
   
   /* Se consume la primer linea de los documentos para no leer los nombres de los campos */
    char line[MAXBUFFER];
    fgets(line, MAXBUFFER, FBikes);
    fgets(line, MAXBUFFER, FStations);

    /* ADT de control de sistema de estaciones */
    stationsADT st = newStationsADT();

    char ** ans;
    int flag=0;

    /* Se lee linea por linea y se envia a la funcion de parseo para separar los campos */
    while(fgets(line,MAXBUFFER, FStations) != NULL)
    {
        ans = getField(line, STATIONS_FIELDS);
        /* Se crean las estaciones */
        flag = addStation(st, atoi(ans[0]), ans[1]);
        if(flag == -1)
        {
            fprintf(stderr, "ERROR: Memory cant be allocated");
            exit(1);
        }
        /* Liberamos el espacio reservado por auxiliares */
        for(int i=0; i < STATIONS_FIELDS; i++)
        {
            free(ans[i]);
        }
        free(ans);
    }
    /* Cerramos el archivo, se lee una unica vez */
    fclose(FStations);

    /* Se usa exactamente la misma logica con las estaciones pero para las rides */
    while(fgets(line,MAXBUFFER, FBikes) != NULL)
    {
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
    fclose(FBikes);

    query1List q1 = query1(st);
    query1List it = q1;
    
    htmlTable tableQ1 = newTable(QUERY1_TABLE_NAME, QUERY1_COLS, QUERY1_COL1, QUERY1_COL2);
    
    FILE * csvQ1;
    csvQ1 = fopen(QUERY1_CSV_NAME,"w");
    if(csvQ1 == NULL)
    {
        fprintf(stderr, "ERROR: The file cant be opened.");
        exit(1);
    }

    fprintf(csvQ1, "%s;%s\n", QUERY1_COL1,QUERY1_COL2);
    while(it != NULL)
    {
        char * tripsString = sizeToString(it->startedTrips);
        addHTMLRow(tableQ1, it->name, tripsString);
        fprintf(csvQ1, "%s;%s\n", it->name, tripsString);
        free(tripsString);
        it = it->tail;
    }

    closeHTMLTable(tableQ1);
    fclose(csvQ1);

    freeQuery1List(q1);
    

    /* Fin del programa, se libera el ADT de estaciones */
    freeStations(st);
    return 0;
}
