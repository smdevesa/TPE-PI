#include "../lib/frontLib.h"
#include "../adt/stationsADT.h"
#include "../lib/htmlTable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXBUFFER 256
#define BIKES_FIELDS 6
#define STATIONS_FIELDS 4
#define MONTHS 12

#define QUERY1_TABLE_NAME "query1.html"
#define QUERY1_CSV_NAME "query1.csv"
#define QUERY1_COLS 2
#define QUERY2_TABLE_NAME "query2.html"
#define QUERY2_CSV_NAME "query2.csv"
#define QUERY2_COLS 4
#define QUERY3_TABLE_NAME "query3.html"
#define QUERY3_CSV_NAME "query3.csv"
#define QUERY3_COLS 13


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

    query1List Q1 = query1(st);
    query1List it = Q1;
    
    htmlTable tableQ1 = newTable(QUERY1_TABLE_NAME, QUERY1_COLS, "Station", "StartedTrips");
    
    FILE * csvQ1;
    csvQ1 = fopen(QUERY1_CSV_NAME,"w");
    checkFile(csvQ1);

    fprintf(csvQ1, "%s;%s\n", "Station", "StartedTrips");
    while(it != NULL)
    {
        char * tripsString = sizeToString(it->startedTrips);
        addHTMLRow(tableQ1, it->name, tripsString);
        fprintf(csvQ1, "%s;%s\n", it->name, tripsString);
        free(tripsString);
        it = it->tail;
    }

    freeQuery1(Q1);

    closeHTMLTable(tableQ1);
    fclose(csvQ1);

    size_t qtyQ2 = 0;
    query2Elem * Q2 = query2(st, &qtyQ2);

    htmlTable tableQ2 = newTable(QUERY2_TABLE_NAME, QUERY2_COLS, "StationA", "StationB", "Trips A->B", "Trips B->A");

    FILE * csvQ2;
    csvQ2 = fopen(QUERY2_CSV_NAME,"w");
    checkFile(csvQ2);

    fprintf(csvQ2, "%s;%s;%s;%s\n", "StationA", "StationB", "Trips A->B", "Trips B->A");
    for(int i=0; i < qtyQ2; i++)
    {
        char * stringAtoB = sizeToString(Q2[i].AtoB);
        char * stringBtoA = sizeToString(Q2[i].BtoA);
        addHTMLRow(tableQ2, Q2[i].stationA, Q2[i].stationB, stringAtoB, stringBtoA);
        fprintf(csvQ2, "%s;%s;%s;%s\n", Q2[i].stationA, Q2[i].stationB, stringAtoB, stringBtoA);
        free(stringAtoB);
        free(stringBtoA);
    }

    freeQuery2(Q2, qtyQ2);

    closeHTMLTable(tableQ2);
    fclose(csvQ2);

    size_t qtyQ3 = 0; 
    query3Elem * Q3 = query3(st, qtyQ3);

    htmlTable tableQ3 = newTable(QUERY3_TABLE_NAME, QUERY3_COLS, "J", "F", "M", "A", "M", "J", "J", "A", "S", "O", "N", "D", "Station");

    FILE * csvQ3;
    csvQ3 = fopen(QUERY3_CSV_NAME, "w");
    checkFile(csvQ3);

    fprintf(csvQ3, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s\n", "J", "F", "M", "A", "M", "J", "J", "A", "S", "O", "N", "D", "Station");
    for(int i = 0; i < qtyQ3; i++)
    {
       char * vec[MONTHS];
       for(int j = 0; j < MONTHS; j++)
       {
        vec[j] = sizeToString(Q3[i].mv[j]);
       }
       addHTMLRow(tableQ3, vec[0], vec[1], vec[2], vec[3], vec[4], vec[5], vec[6], vec[7], vec[8], vec[9], vec[10], vec[11], Q3[i].name);
       fprintf(csvQ3, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s\n", vec[0], vec[1], vec[2], vec[3], vec[4], vec[5], vec[6], vec[7], vec[8], vec[9], vec[10], vec[11], Q3[i].name);
       for(int k=0; k < MONTHS; k++)
       {
        free(vec[k]);
       }
    }

    freeQuery3(Q3,qtyQ3);

    closeHTMLTable(tableQ3);
    fclose(csvQ3);
    
    /* Fin del programa, se libera el ADT de estaciones */
    freeStations(st);
    return 0;
}
