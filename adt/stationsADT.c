#include "stationsADT.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define COPYBLOCK 10
#define BLOCK 50

typedef struct ride
{
    size_t endId; /* ID de la estacion en la que finalizo el viaje */
    unsigned short startMonth; /* Mes en el que se empezo el viaje */
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
        newNode->station.rides = NULL;
        newNode->station.size = 0;
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

static void addRideRec(TList list, size_t startId, size_t endId, int isMember, unsigned short month, int *flag)
{
    if(list == NULL || list->station.id > startId)
    {
        return;
    }
    if(list->station.id == startId)
    {
        if(list->station.dim == list->station.size)
        {
            list->station.rides = realloc(list->station.rides,sizeof(TRide) * (BLOCK + list->station.size));
            if(checkMem((void *)list->station.rides, "ERROR: Cannot allocate memory."))
            {
                *flag = -1;
                return;
            }
            list->station.size += BLOCK;
        }
        list->station.rides[list->station.dim].endId = endId;
        list->station.rides[list->station.dim].isMember = isMember;
        list->station.rides[list->station.dim].startMonth = month;
        list->station.dim++;
        *flag = 1;
        return;
    }
    addRideRec(list->tail,startId,endId,isMember,month,flag);
    return;
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

static void freeList(TList list){
    if(list == NULL){
        return;
    }
    freeList(list->tail);
    free(list->station.rides);
    free(list->station.name);
    free(list);
}

void freeStations(stationsADT st)
{
    freeList(st->list);
    free(st);
}