#include "stationsADT.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define COPYBLOCK 10
#define BLOCK 5000
#define QUERY2BLOCK 50

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
    size_t memberRides; /* Cantidad de viajes hechos por miembros */
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
            ans = realloc(ans, (COPYBLOCK+i) * sizeof(char));
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
    int c;
    if(list == NULL || (c = strcmp(name, list->station.name)) < 0)
    {
        TList newNode = malloc(sizeof(struct node));
        if(checkMem((void *)newNode, "ERROR: Cannot allocate memory."))
        {
            *flag = -1;
            return list;
        }
        newNode->station.id = id;
        newNode->station.rides = NULL;
        newNode->station.dim = 0;
        newNode->station.size = 0;
        newNode->station.memberRides = 0;
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

static void addRideRec(TList list, size_t startId, size_t endId, int isMember, unsigned short month, int *flag)
{
    if(list == NULL)
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
        list->station.memberRides += isMember;
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
    addRideRec(st->list, startId, endId, isMember, month, &flag);
    return flag;
}

query1List query1Add(query1List list, char * name, size_t startedTrips, int * flag)
{
    if(list == NULL || list->startedTrips < startedTrips || (list->startedTrips == startedTrips && strcmp(name, list->name) < 0))
    {
        query1List newNode = malloc(sizeof(struct query1Node));
        if(checkMem((void *)newNode, "ERROR: Cannot allocate memory."))
        {
            *flag = -1;
            return list;
        }
        newNode->name = copyStr(name);
        if(name == NULL)
        {
            *flag = -1;
            return list;
        }
        *flag = 1;
        newNode->startedTrips = startedTrips;
        newNode->tail = list;
        return newNode;
    }
    list->tail = query1Add(list->tail, name, startedTrips, flag);
    return list;
}

query1List query1(stationsADT st)
{
    TList it = st->list;
    query1List ans = NULL;
    int flag;

    while(it != NULL)
    {
        flag = 0;
        ans = query1Add(ans, it->station.name, it->station.memberRides, &flag);
        if(flag == -1)
        {
            return ans;
        }
        it = it->tail;
    }
    return ans;
}

void freeQuery1List(query1List list)
{
    if(list == NULL)
        return;
    
    freeQuery1List(list->tail);
    free(list->name);
    free(list);
}

static size_t tripsToStation(TRide * vector, size_t dim,  size_t id)
{
    size_t count = 0;
    for(int i=0; i<dim; i++)
    {
        if(vector->endId == id)
        {
            count++;
        }
    }
    return count;
}

query2Elem * query2(stationsADT st, size_t * qty)
{
    TList it1 = st->list;
    TList it2;
    size_t dim = 0;
    size_t size = 0;
    query2Elem * ans = NULL;

    while(it1 != NULL)
    {
        it2 = st->list;
        while(it2 != NULL)
        {
            if(it1->station.id != it2->station.id) /* No uso los viajes circulares */
            {
                if(dim == size)
                {
                    ans = realloc(ans, (size + QUERY2BLOCK) * sizeof(query2Elem));
                    if(checkMem(ans, "ERROR: Memory cant be allocated."))
                    {
                        return NULL;
                    }
                    size += QUERY2BLOCK;
                }
                ans[dim].stationA = copyStr(it1->station.name);
                ans[dim].stationB = copyStr(it2->station.name);
                ans[dim].AtoB = tripsToStation(it1->station.rides, it1->station.dim, it2->station.id);
                ans[dim].BtoA = tripsToStation(it2->station.rides, it2->station.dim, it1->station.id);
                dim++;
            }
            it2 = it2->tail;
        }
        it1 = it1->tail;
    }

    ans = realloc(ans, dim * sizeof(query2Elem));
    if(checkMem(ans, "ERROR: Memory cant be allocated"))
    {
        return NULL;
    }
    *qty = dim;
    return ans;
}

void freeQuery2(query2Elem * vector, size_t qty)
{
    for(int i=0; i < qty ; i++)
    {
        free(vector[i].stationA);
        free(vector[i].stationB);
    }
    free(vector);
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
