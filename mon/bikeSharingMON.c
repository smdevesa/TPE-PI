/*
**  Autores: smdevesa y jrambau
**  Version: 1.1
**  Fecha: 10/07/2023
**  
**  Codigo fuente del programa de leido de datos y realizacion de queries de Montreal.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/frontLib.h"
#include "../adt/stationsADT.h"
#include "../lib/htmlTable.h"

#define MAXBUFFER 256
#define BIKES_FIELDS 5
#define STATIONS_FIELDS 4

int main(int argc, char ** argv)
{
    /* Se esperan dos documentos .csv */
    if(argc != ARGS_EXPECTED)
    {
        fprintf(stderr, "ERROR: The amount of files recieved is invalid.\n");
        exit(1);
    }

    /*Se abre cada archivo .csv en base al orden recibido*/
    FILE * FBikes = fopen(argv[1], "r"); 
    checkFile(FBikes);

    FILE * FStations = fopen(argv[2], "r");
    checkFile(FStations);
   
    /*Se consume la primer linea de los documentos para no leer los nombres de los campos */
    char line[MAXBUFFER];
    fgets(line, MAXBUFFER, FBikes);
    fgets(line, MAXBUFFER, FStations);

    stationsADT st = newStationsADT();

    char ** ans;
    int flag=0;

    /* Se lee linea por linea y se parsea para separar los campos */
    while(fgets(line,MAXBUFFER, FStations) != NULL)
    {
        ans = getField(line, STATIONS_FIELDS);
        /* Se envian los indices segun el formato del CSV */
        flag = addStation(st, atoi(ans[0]), ans[1]);
        if(flag == -1)
        {
            fprintf(stderr, "ERROR: Memory cant be allocated.\n");
            exit(1);
        }
        /* Liberamos el espacio reservado por auxiliares */
        for(int i = 0; i < STATIONS_FIELDS; i++)
        {
            free(ans[i]);
        }
        free(ans);
    }

    fclose(FStations);

    while(fgets(line,MAXBUFFER, FBikes) != NULL)
    {  
        ans = getField(line, BIKES_FIELDS);
         /* Convertimos los strings necesarios en numeros */
        flag = addRide(st, atoi(ans[1]), atoi(ans[3]),atoi(ans[4]),ans[0]);
        if(flag == -1)
        {
            fprintf(stderr, "ERROR: Memory cant be allocated.\n");
            exit(1);
        }
        for(int i = 0; i < BIKES_FIELDS; i++)
        {
            free(ans[i]);
        }
        free(ans);
    }
    fclose(FBikes);

    int flagQ1;
    query1List Q1 = query1(st, &flagQ1);
    query1List it = Q1; /* Iterador para la lista de la query 1 */

    if(flagQ1 != 1)
    {
        fprintf(stderr, "ERROR: Cannot finish query 1. Aborting.\n");
        exit(1);
    }
   
    /* Se crea tabla HTML con los campos requeridos para la query 1 */
    htmlTable tableQ1 = newTable(QUERY1_TABLE_NAME, QUERY1_COLS, QUERY1_F1, QUERY1_F2);
    
    /* Se crea un archivo CSV para la query 1 */
    FILE * csvQ1;
    csvQ1 = fopen(QUERY1_CSV_NAME, "w");
    checkFile(csvQ1);

    /* Campos del archivo CSV */
    fprintf(csvQ1, "%s;%s\n", QUERY1_F1, QUERY1_F2);

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

    /* Dimension del vector de la query 2 */
    int qtyQ2; 
    query2Elem * Q2 = query2(st, &qtyQ2);
    
    if(qtyQ2 < 0)
    {
        fprintf(stderr, "ERROR: Cannot finish query 2. Aborting.\n");
        exit(1);
    }

    /* Archivo HTML con los campos necesarios de la query 2 */
    htmlTable tableQ2 = newTable(QUERY2_TABLE_NAME, QUERY2_COLS, QUERY2_F1, QUERY2_F2, QUERY2_F3, QUERY2_F4);

    /* Archivo CSV para la query 2 */
    FILE * csvQ2;
    csvQ2 = fopen(QUERY2_CSV_NAME,"w");
    checkFile(csvQ2);

    /* Campos del archivo CSV */
    fprintf(csvQ2, "%s;%s;%s;%s\n", QUERY2_F1, QUERY2_F2, QUERY2_F3, QUERY2_F4);
    for(int i = 0; i < qtyQ2; i++)
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

    /* Dimension del vector de la query 3 */
    int qtyQ3;
    query3Elem * Q3 = query3(st, &qtyQ3);

    if(qtyQ3 < 0)
    {
        fprintf(stderr, "ERROR: Cannot finish query 3. Aborting.\n");
        exit(1);
    }

    /* Archivo HTML con los campos de la query 3 */
    htmlTable tableQ3 = newTable(QUERY3_TABLE_NAME, QUERY3_COLS, QUERY3_F1, QUERY3_F2, QUERY3_F3, QUERY3_F4, QUERY3_F5, QUERY3_F6, QUERY3_F7,\
    QUERY3_F8, QUERY3_F9, QUERY3_F10, QUERY3_F11, QUERY3_F12, QUERY3_F13);

    /* Archivo CSV para la query 3 */
    FILE * csvQ3;
    csvQ3 = fopen(QUERY3_CSV_NAME, "w");
    checkFile(csvQ3);

    /* Campos CSV */
    fprintf(csvQ3, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s\n", QUERY3_F1, QUERY3_F2, QUERY3_F3, QUERY3_F4, QUERY3_F5, QUERY3_F6, QUERY3_F7,\
    QUERY3_F8, QUERY3_F9, QUERY3_F10, QUERY3_F11, QUERY3_F12, QUERY3_F13);

    for(int i = 0; i < qtyQ3; i++)
    {
        /* Vector con los meses convertidos a string */ 
        char * vec[MONTHS_QTY];
        /* Se rellena el vector con los viajes en cada mes*/
        for(int j = 0; j < MONTHS_QTY; j++)
        {
            vec[j] = sizeToString(Q3[i].mv[j]);
        }

        addHTMLRow(tableQ3, vec[0], vec[1], vec[2], vec[3], vec[4], vec[5], vec[6], vec[7], vec[8], vec[9], vec[10], vec[11], Q3[i].name);

        /* Imprimimos mes a mes */
        for(int k=0; k < MONTHS_QTY; k++)
        {
            fprintf(csvQ3, "%s;", vec[k]);
            free(vec[k]);
        }

        /* Imprimimos el nombre al final */
        fprintf(csvQ3, "%s\n", Q3[i].name);
    }

    freeQuery3(Q3,qtyQ3);
    closeHTMLTable(tableQ3);
    fclose(csvQ3);
    
    /* Fin del programa, se libera el ADT de estaciones */
    freeStations(st);
    return 0;
}
