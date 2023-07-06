/*
**  Autores: smdevesa y jrambau
**  Version: 1.0
**  Fecha: 05/07/2023
**  
**  Codigo fuente del ADT de estaciones.
 */

#include "stationsADT.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define COPYBLOCK 10
#define BLOCK 5000
#define QUERYBLOCK 50

#define MONTHS_QTY 12

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
    size_t len; /* Dimension del string de la estacion */
    TRide * rides; /* Vector de rides que salieron desde la estacion */
    size_t memberRides; /* Cantidad de viajes hechos por miembros */
    size_t dim; /* Cantidad de rides que salieron desde la estacion */
    size_t size; /* Espacio reservado en el vector de rides */
} TStation;

typedef struct node
{
    TStation station; /* Struct con todos los datos necesarios de la estacion */
    struct node * tail;
} TNode;

typedef struct node * TList;

typedef struct stationsCDT
{
    TList list; /* Lista de estaciones */
    size_t qty; /* Cantidad de estaciones */
} stationsCDT;

/*
** Funcion para chequear si se reservo bien la memoria
** si el puntero recibido es NULL imprime el mensaje recibido
** en stderr.
 */
static int checkMem(void * ptr, const char * message);

/*
** Copia por bloques de tipo COPYBLOCK el string s y devuelve
** en su nombre el vector que contiene el string.
** En caso de error de memoria imprime error y devuelve NULL.
 */
static char * copyStr(const char * s, size_t * len);

/*
** Auxiliar que agrega una estacion a la lista recibida ordenandola por NOMBRE
** de forma alfabetica. El flag queda en 0 si no se pudo agregar la estacion
** (tiene el nombre repetido), sera 1 si se agrego correctamente o sera
** -1 si no se pudo reservar memoria de manera correcta.
 */
static TList addStationRec(TList list, size_t id, const char * name, int * flag);

/*
** Funcion que busca la estacion de startId que recibe e ingresa un ride a su vector de
** alquileres con el endId, isMember y month recibido.
** El flag no se modifica si no se pudo agregar, sera 1 si se agrego o -1 si no se pudo
** reservar memoria.
 */
static void addRideRec(TList list, size_t startId, size_t endId, int isMember, unsigned short month, int *flag);

/*
** Funcion que agrega un elemento a la lista de tipo query1
** se ordenara segun startedTrips descendentemente y se desempata
** por orden alfabetico. El flag sera 1 si se pudo agregar o -1 si
** no se pudo reservar memoria.
** IMPORTANTE: La funcion no chequea que no hayan repetidos.
 */
query1List query1Add(query1List list, char * name, size_t len, size_t startedTrips, int * flag);

/*
** Devuelve la cantidad de viajes que encuentre en un vector de TRides
** que terminen en la estacion de la id recibida.
 */
static size_t tripsToStation(TRide vector[], size_t dim,  size_t id);

/*
** Recibe un vector de TRides y su dimension y a partir de eso
** rellena el monthsVec con las cantidades de viajes de cada mes
** siendo el indice 0 ENERO, 1 FEBRERO y asi sucesivamente hasta
** 11 DICIEMBRE.
 */
static void getMonthsVec(size_t monthsVec[], TRide vector[], size_t dim);

/*
** Libera la lista de tipo TList recibida.
 */
static void freeList(TList list);

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
    /* Se inicializa en NULL la lista y qty en cero */
    stationsADT new = calloc(1, sizeof(struct stationsCDT));
    if(checkMem((void *)new, "ERROR: Memory cant be allocated.\n"))
    {
        return NULL;
    }
    return new;
}

static char * copyStr(const char * s, size_t * len)
{
    char * ans = NULL;
    int i;

    for(i=0; s[i] != 0; i++)
    {
        if(i % COPYBLOCK == 0)
        {
            ans = realloc(ans, (COPYBLOCK+i) * sizeof(char));
            if(checkMem((void *)ans, "ERROR: Memory cant be allocated.\n"))
            {
                return NULL;
            }
        }
        ans[i] = s[i];
    }

    /* Se recorta el string para que solo ocupe lo necesario. */
    ans = realloc(ans, (i+1) * sizeof(char));
    if(checkMem((void *)ans, "ERROR: Memory cant be allocated.\n"))
    {
        return NULL;
    }
    ans[i] = 0;
    *len = i;
    return ans;
}

static TList addStationRec(TList list, size_t id, const char * name, int * flag)
{
    int c; /* Guardamos en c la comparacion para no hacerla dos veces */
    if(list == NULL || (c = strcmp(name, list->station.name)) < 0)
    {
        /* Inicializamos rides y name en NULL y el resto de los campos en 0 */
        TList newNode = calloc(1, sizeof(struct node));
        if(checkMem((void *)newNode, "ERROR: Memory cant be allocated.\n"))
        {
            *flag = -1;
            return list;
        }
        newNode->station.id = id;
        newNode->station.name = copyStr(name, &newNode->station.len);
        if(newNode->station.name == NULL)
        {
            *flag = -1;
            return list;
        }
        *flag = 1;
        newNode->tail = list;
        return newNode;
    }
    /* La estacion estaba repetida */
    if(c == 0)
    {
        return list;
    }
    list->tail = addStationRec(list->tail, id, name, flag);
    return list;
}


int addStation(stationsADT st, size_t id, const char * name)
{
    int flag = 0;
    st->list = addStationRec(st->list, id, name, &flag);
    /* Aumento la cantidad solo en caso de que se haya podido agregar una estacion*/
    if(flag == 1)
    {
        st->qty++;
    }
    return flag;
}

static void addRideRec(TList list, size_t startId, size_t endId, int isMember, unsigned short month, int *flag)
{
    /* La estacion a la que se quiere agregar no existe */
    if(list == NULL)
    {
        return;
    }
    if(list->station.id == startId)
    {
        if(list->station.dim == list->station.size)
        {
            list->station.rides = realloc(list->station.rides,sizeof(TRide) * (BLOCK + list->station.size));
            if(checkMem((void *)list->station.rides, "ERROR: Memory cant be allocated.\n"))
            {
                *flag = -1;
                return;
            }
            list->station.size += BLOCK;
        }
        list->station.rides[list->station.dim].endId = endId;
        list->station.rides[list->station.dim].isMember = isMember;
        list->station.rides[list->station.dim].startMonth = month;
        list->station.dim += 1;
        list->station.memberRides += isMember;
        *flag = 1;
        return;
    }
    addRideRec(list->tail,startId,endId,isMember,month,flag);
}

int addRide(stationsADT st, size_t startId, size_t endId, int isMember, const char * startDate)
{
    if(isMember != 0 && isMember != 1)
    {
        fprintf(stderr, "ERROR: isMember must be 1 or 0.\n");
        return -1;
    }
    int flag = 0;
    unsigned short month = atoi(startDate + 5); /* Extraemos el mes del formato de fecha enviado */
    addRideRec(st->list, startId, endId, isMember, month, &flag);
    return flag;
}

query1List query1Add(query1List list, char * name, size_t len, size_t startedTrips, int * flag)
{
    /* Ordenamos primero por startedTrips, luego alfabeticamente si empatan */
    if(list == NULL || list->startedTrips < startedTrips || (list->startedTrips == startedTrips && strcmp(name, list->name) < 0))
    {
        query1List newNode = malloc(sizeof(struct query1Node));
        if(checkMem((void *)newNode, "ERROR: Memory cant be allocated.\n"))
        {
            *flag = -1;
            return list;
        }
        newNode->name = malloc((len+1) * sizeof(char));
        if(checkMem((void *)newNode->name, "ERROR: Memory cant be allocated.\n"))
        {
            *flag = -1;
            return list;
        }
        strcpy(newNode->name, name);
        *flag = 1;
        newNode->startedTrips = startedTrips;
        newNode->tail = list;
        return newNode;
    }
    list->tail = query1Add(list->tail, name, len, startedTrips, flag);
    return list;
}

query1List query1(stationsADT st, int * flag)
{
    TList it = st->list;
    query1List ans = NULL;

    while(it != NULL)
    {
        ans = query1Add(ans, it->station.name, it->station.len, it->station.memberRides, flag);
        if(*flag == -1)
        {
            return ans;
        }
        it = it->tail;
    }
    *flag = 1;
    return ans;
}

void freeQuery1(query1List list)
{
    if(list == NULL)
        return;
    
    freeQuery1(list->tail);
    free(list->name);
    free(list);
}

static size_t tripsToStation(TRide vector[], size_t dim,  size_t id)
{
    size_t count = 0;
    for(int i=0; i < dim; i++)
    {
        if(vector[i].endId == id)
        {
            count++;
        }
    }
    return count;
}

query2Elem * query2(stationsADT st, int * qty)
{
    query2Elem * ans = malloc(sizeof(query2Elem));
    if(checkMem((void *)ans, "ERROR: Memory cant be allocated.\n"))
    {
        *qty = -1;
        return ans;
    }
    
    ans->trips = malloc(st->qty * sizeof(size_t *));
    if(checkMem((void *)ans->trips, "ERROR: Memory cant be allocated.\n"))
    {
        *qty = -1;
        return ans;
    }
    for(int i=0; i<st->qty; i++)
    {
        ans->trips[i] = malloc(st->qty * sizeof(size_t));
        if(checkMem((void *)ans->trips[i], "ERROR: Memory cant be allocated.\n"))
        {
            *qty = -1;
            return ans;
        }
    }

    ans->names = malloc(st->qty * sizeof(char *));
    TList it = st->list;

    for(int i=0; it != NULL; i++, it=it->tail)
    {
        ans->names[i] = malloc((it->station.len + 1) * sizeof(char));
        if(checkMem((void *)ans->names[i], "ERROR: Memory cant be allocated.\n"))
        {
            *qty = -1;
            return ans;
        }
        strcpy(ans->names[i], it->station.name);
    }

    it = st->list;

    for(int i=0)


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

static void getMonthsVec(size_t monthsVec[], TRide vector[], size_t dim)
{
    for(int i=0; i<dim; i++)
    {
        monthsVec[vector[i].startMonth - 1] += 1;
    }
}

query3Elem * query3(stationsADT st, int * qty)
{
    int i=0;
    int elems = st->qty;
    query3Elem * ans = malloc(elems * sizeof(query3Elem));
    if(checkMem((void *)ans, "ERROR: Memory cant be allocated.\n"))
    {
        *qty = -1;
        return NULL;
    }
    TList it = st->list;

    while(it != NULL)
    {
        ans[i].mv = calloc(MONTHS_QTY, sizeof(size_t));
        if(checkMem((void *)ans[i].mv, "ERROR: Memory cant be allocated.\n"))
        {
            *qty = -1;
            return NULL;
        }
        /* Rellenamos el vector de months */
        getMonthsVec(ans[i].mv, it->station.rides, it->station.dim);
        ans[i].name = malloc((it->station.len + 1) * sizeof(char));
        if(checkMem((void *)ans[i].name, "ERROR: Memory cant be allocated.\n"))
        {
            *qty = -1;
            return NULL;
        }
        strcpy(ans[i].name, it->station.name);
        i++;
        it = it->tail;
    }
    *qty = elems;
    return ans;
}

void freeQuery3(query3Elem * vec, size_t qty)
{
    for(int i = 0; i < qty; i++)
    {
        free(vec[i].name);
        free(vec[i].mv);
    }
    free(vec);
}

static void freeList(TList list)
{
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
