#include "../lib/frontLib.h"
#include "../adt/stationsADT.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXBUFFER 256
#define BIKES_FIELDS 6
#define STATIONS_FIELDS 4

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
        if(ans[5][0] == 'm') /* Asumimos que los datos del csv estan bien, entonces basta con preguntar si la primer letra es una m */
        {
            isMember = 1;
        }
        else
        {
            isMember = 0;
        }
        printf("id inicio: %s, id fin: %s, ismember: %s (%d), fecha: %s\n",ans[1],ans[3],ans[5], isMember,ans[0]);
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

    /* Fin del programa, se libera el ADT de estaciones */
    freeStations(st);
    return 0;
}
