#include "../lib/frontLib.h"
#include "../adt/stationsADT.h"
#include "../lib/htmlTable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXBUFFER 256
#define BIKES_FIELDS 6
#define STATIONS_FIELDS 4

#define QUERY1_TABLE_NAME "query1.html"
#define QUERY1_CSV_NAME "query1.csv"
#define QUERY1_COLS 2
#define QUERY1_COL1 "Station"
#define QUERY1_COL2 "StartedTrips"
#define QUERY2_TABLE_NAME "query2.html"
#define QUERY2_CSV_NAME "query2.csv"
#define QUERY2_COLS 4
#define QUERY2_COL1 "StationA"
#define QUERY2_COL2 "StationB"
#define QUERY2_COL3 "Trips A->B"
#define QUERY2_COL4 "Trips B->A"


int main(int argc, char ** argv)
{
    /* Se esperan dos documentos .csv */
    if(argc != 3)
    {
        fprintf(stderr, "ERROR: The amount of files recieved is invalid.");
        exit(1);
    }

    FILE * FBikes = fopen(argv[1], "r");
    FILE * FStations = fopen(argv[2], "r");
    
    checkFile(FBikes);
    checkFile(FStations);

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
        flag = addStation(st, atoi(ans[3]), ans[0]);
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

    /* Flag para cambiar la cadena de miembro a un numero 1/0 (por requisito del backend) */
    int isMember;
    /* Se usa exactamente la misma logica con las estaciones pero para las rides */
    while(fgets(line,MAXBUFFER, FBikes) != NULL)
    {
        ans = getField(line, BIKES_FIELDS);
        if(strcmp("member", ans[5]) == 0) /* Asumimos que los datos del csv estan bien, entonces basta con preguntar si la primer letra es una m */
        {
            isMember = 1;
        }
        else
        {
            isMember = 0;
        }
        flag = addRide(st, atoi(ans[1]), atoi(ans[3]), isMember, ans[0]);
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
    checkFile(csvQ1);

    fprintf(csvQ1, "%s;%s\n", QUERY1_COL1,QUERY1_COL2);
    while(it != NULL)
    {
        char * tripsString = sizeToString(it->startedTrips);
        addHTMLRow(tableQ1, it->name, tripsString);
        fprintf(csvQ1, "%s;%s\n", it->name, tripsString);
        free(tripsString);
        it = it->tail;
    }

    freeQuery1(q1);

    closeHTMLTable(tableQ1);
    fclose(csvQ1);

    size_t qty=0;
    query2Elem * q2 = query2(st, &qty);

    htmlTable tableQ2 = newTable(QUERY2_TABLE_NAME, QUERY2_COLS, QUERY2_COL1, QUERY2_COL2, QUERY2_COL3, QUERY2_COL4);

    FILE * csvQ2;
    csvQ2 = fopen(QUERY2_CSV_NAME,"w");
    checkFile(csvQ2);

    fprintf(csvQ2, "%s;%s;%s;%s\n", QUERY2_COL1, QUERY2_COL2, QUERY2_COL3, QUERY2_COL4);
    for(int i=0; i < qty; i++)
    {
        char * stringAtoB = sizeToString(q2[i].AtoB);
        char * stringBtoA = sizeToString(q2[i].BtoA);
        addHTMLRow(tableQ2, q2[i].stationA, q2[i].stationB, stringAtoB, stringBtoA);
        fprintf(csvQ2, "%s;%s;%s;%s\n", q2[i].stationA, q2[i].stationB, stringAtoB, stringBtoA);
        free(stringAtoB);
        free(stringBtoA);
    }

    freeQuery2(q2, qty);

    closeHTMLTable(tableQ2);
    fclose(csvQ2);
    
    /* Fin del programa, se libera el ADT de estaciones */
    freeStations(st);
    return 0;
}
