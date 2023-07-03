#include "stationsADT.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define COPYBLOCK 10

typedef struct ride
{
    size_t endId; /* ID de la estacion en la que finalizo el viaje */
    unsigned short startMonth; /* Mes en el que se empezo el viaje */
    char * bikeType; /* Tipo de bicicleta, sera NULL si hasMultipleTypes es cero */
    int isMember; /* Flag de si el usuario de la bicicleta era o no miembro */
} TRide;

typedef struct station
{
    size_t id; /* ID de la estacion */
    char * name; /* Nombre de la estacion */
    TRide * rides; /* Vector de rides que salieron desde la estacion */
    size_t dim; /* Cantidad de rides que salieron desde la estacion */
    size_t size; /* Espacio reservado en el vector de rides */
} TStation;

typedef struct node
{
    TStation station;
    struct node * tail;
} TNode;

typedef struct node * TList;

typedef struct stationsCDT
{
    TList list; /* Lista de estaciones */
    TList it; /* Iterador */
    size_t qty; /* Cantidad de estaciones */
    char hasMultipleTypes; /* Flag de si hay mas de un tipo de bicicleta en las estaciones, 1 si, 0 no */
} stationsCDT;

static int checkMem(void * ptr, char * message)
{
    if(ptr == NULL)
    {
        fprintf(stderr, message);
        return 1;
    }
    return 0;
}

stationsADT newStationsADT(int hasMultipleTypes)
{
    if(hasMultipleTypes != 1 && hasMultipleTypes != 0)
    {
        fprintf(stderr, "ERROR: Invalid use of function newStationsADT.\n");
        return NULL;
    }
    stationsADT new = calloc(1, sizeof(struct stationsCDT));
    if(checkMem((void *)new, "ERROR: Cannot allocate memory."))
    {
        return NULL;
    }
    new->hasMultipleTypes = hasMultipleTypes;
    return new;
}

static char * copyStr(char * s)
{
    char * ans = NULL;
    int i;

    for(i=0; s[i] != 0; i++)
    {
        if(i % COPYBLOCK == 0)
        {
            ans = realloc(ans, COPYBLOCK * sizeof(char));
            if(checkMem((void *)ans, "ERROR: Cannot allocate memory."))
            {
                return NULL;
            }
        }
        ans[i] = s[i];
    }

    ans = realloc(ans, (i+1) * sizeof(char));
    if(checkMem((void *)ans, "ERROR: Cannot allocate memory."))
    {
        return NULL;
    }
    ans[i] = 0;
    return ans;
}

static TList addStationRec(TList list, size_t id, char * name, int * flag)
{
    if(list == NULL || list->station.id > id)
    {
        TList newNode = malloc(sizeof(struct node));
        if(checkMem((void *)newNode, "ERROR: Cannot allocate memory."))
        {
            *flag = -1;
            return list;
        }
        newNode->station.id = id;
        newNode->station.name = copyStr(name);
        if(newNode->station.name == NULL)
        {
            *flag = -1;
        }
        *flag = 1;
        return newNode;
    }
    if(list->station.id == id)
    {
        return list;
    }
    list->tail = addStationRec(list->tail, id, name, flag);
    return list;
}


int addStation(stationsADT st, size_t id, char * name)
{
    int flag = 0;
    st->list = addStationRec(st->list, id, name, &flag);
    if(flag == 1)
    {
        st->qty++;
    }
    return flag;
}