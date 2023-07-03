#include "stationsADT.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define COPYBLOCK 10
#define BLOCK 50

typedef struct ride
{
    size_t endId; /* ID de la estacion en la que finalizo el viaje */
    unsigned short startMonth; /* Mes en el que se empezo el viaje */
    int isMember; /* Flag de si el usuario de la bicicleta era o no miembro */
} TRide;

typedef struct RNode
{
    TRide ride;
    struct RNode * tail;
} RNode;

typedef struct RNode * RList;

typedef struct station
{
    size_t id; /* ID de la estacion */
    char * name; /* Nombre de la estacion */
    RList rides; /* Vector de rides que salieron desde la estacion */
    size_t dim; /* Cantidad de rides que salieron desde la estacion */
} TStation;

typedef struct SNode
{
    TStation station;
    struct SNode * tail;
} SNode;

typedef struct SNode * SList;

typedef struct stationsCDT
{
    SList list; /* Lista de estaciones */
    SList it; /* Iterador */
    size_t qty; /* Cantidad de estaciones */
} stationsCDT;

static int checkMem(void * ptr, const char * message)
{
    if(ptr == NULL)
    {
        fprintf(stderr, "%s", message);
        return 1;
    }
    return 0;
}

stationsADT newStationsADT(void)
{
    stationsADT new = calloc(1, sizeof(struct stationsCDT));
    if(checkMem((void *)new, "ERROR: Cannot allocate memory."))
    {
        return NULL;
    }
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

static SList addStationRec(SList list, size_t id, char * name, int * flag)
{
    int c;
    if(list == NULL || (c = strcmp(name, list->station.name)) < 0)
    {
        SList newNode = malloc(sizeof(struct SNode));
        if(checkMem((void *)newNode, "ERROR: Cannot allocate memory."))
        {
            *flag = -1;
            return list;
        }
        newNode->station.id = id;
        newNode->station.rides = NULL;
        newNode->station.dim = 0;
        newNode->station.name = copyStr(name);
        if(newNode->station.name == NULL)
        {
            *flag = -1;
        }
        *flag = 1;
        newNode->tail = list;
        return newNode;
    }
    if(c == 0)
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

static RList addRideList(RList list, size_t endId, int isMember, unsigned short month, int *flag)
{
    if(list == NULL || list->ride.endId >= endId)
    {
        RList newNode = malloc(sizeof(struct RNode));
        if(checkMem((void *)newNode, "ERROR: Cannot allocate memory."))
        {
            *flag = -1;
            return list;
        }
        newNode->ride.endId = endId;
        newNode->ride.isMember = isMember;
        newNode->ride.startMonth = month;
        *flag = 1;
        newNode->tail = list;
        return newNode;
    }
    list->tail = addRideList(list->tail, endId, isMember, month, flag);
    return list;
}

static void addRideRec(SList list, size_t startId, size_t endId, int isMember, unsigned short month, int *flag)
{
    if(list == NULL)
    {
        *flag = 0;
        return;
    }
    if(list->station.id == startId)
    {
        list->station.rides = addRideList(list->station.rides, endId, isMember, month, flag);
        if(*flag == 1)
        {
            list->station.dim++;
        }
        return;
    }
    addRideRec(list->tail, startId, endId, isMember, month, flag);
}

int addRide(stationsADT st, size_t startId, size_t endId, int isMember, char * startDate)
{
    if(isMember != 0 && isMember != 1)
    {
        fprintf(stderr, "ERROR: invalid type of data\n");
        return -1;
    }
    int flag = 0;
    unsigned short month = atoi(startDate + 5); 
    addRideRec(st->list, startId, endId, isMember, month,&flag);
    return flag;
}

static void freeRList(RList list)
{
    if(list == NULL)
    {
        return;
    }

    freeRList(list->tail);
    free(list);
    return;
}

static void freeSList(SList list){
    if(list == NULL)
    {
        return;
    }
    freeSList(list->tail);
    freeRList(list->station.rides);
    free(list->station.name);
    free(list);
}

void freeStations(stationsADT st)
{
    freeSList(st->list);
    free(st);
}
