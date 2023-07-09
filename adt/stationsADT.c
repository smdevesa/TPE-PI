/*
**
 */

#include "stationsADT.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define BIG_BLOCK 1000
#define COPY_BLOCK 15

#define MONTHS_QTY 12


typedef struct station
{
    char * name;
    size_t len; /* Longitud del string de nombre */
    size_t monthsVec[MONTHS_QTY]; /* Vector de viajes segun meses del año */
    size_t id;
    size_t index; /* Indice interno de la matriz de adyacencia */
    size_t memberTrips;
} TStation;

typedef struct stationsCDT
{
    size_t ** matrix; /* Matriz cuadrada de cantidad de viajes */
    TStation * stations; /* Vector dinamico de estaciones */
    size_t dim; /* Cantidad de estaciones */
    size_t size; /* Espacio reservado en el vector y en la matriz */
    char sortedName; /* Flag para saber si el vector de estaciones esta ordenado por nombre */
    char sortedId; /* Flag para saber si el vector de estaciones esta ordenado por ID */
} stationsCDT;

/* Funcion de comparacion general para hacer sorting */
typedef int(*TCompare)(const void *, const void *);

/*
** Funcion para chequear si se reservo bien la memoria
** si el puntero recibido es NULL o errno es ENOMEM
** imprime el mensaje recibido en stderr.
 */
static int checkMem(void * ptr, const char * message);

/*
** Copia por bloques de tipo COPYBLOCK el string s y devuelve
** en su nombre el vector que contiene el string.
** En caso de error de memoria imprime error y devuelve NULL.
 */
static char * copyStr(const char * s, size_t * len);

/*
** Devuelve el indice en el que esta situado el ID que recibe
** si no lo encuentra devuelve -1.
 */
static int getIdx(TStation vector[], size_t dim, size_t id);

/*
** Devuelve la comparacion del nombre de dos estaciones.
** Se adapta con punteros a void para utilizarla al hacer qsort.
 */
static int compareStationNames(const void * s1, const void * s2);

/*
** Ordena el vector de estaciones alfabeticamente.
 */
static void sortBy(TStation vector[], size_t dim, TCompare f);

/*
**
 */
static int compareStationId(const void * s1, const void * s2);

/*
**
 */
static query1List query1Add(query1List list, const char * name, size_t len, size_t startedTrips);

static int checkMem(void * ptr, const char * message)
{
    if(ptr == NULL || errno == ENOMEM)
    {
        fprintf(stderr, "%s", message);
        return 1;
    }
    return 0;
}

static char * copyStr(const char * s, size_t * len)
{
    char * ans = NULL;
    int i;

    for(i=0; s[i] != 0; i++)
    {
        if(i % COPY_BLOCK == 0)
        {
            errno = 0;
            ans = realloc(ans, (COPY_BLOCK+i) * sizeof(char));
            if(checkMem((void *)ans, "ERROR: Memory cant be allocated.\n"))
            {
                return NULL;
            }
        }
        ans[i] = s[i];
    }

    /* Se recorta el string para que solo ocupe lo necesario. */
    errno = 0;
    ans = realloc(ans, (i+1) * sizeof(char));
    if(checkMem((void *)ans, "ERROR: Memory cant be allocated.\n"))
    {
        return NULL;
    }
    ans[i] = 0;
    *len = i;
    return ans;
}

stationsADT newStationsADT(void)
{
    stationsADT new  = calloc(1, sizeof(struct stationsCDT));
    errno = 0;
    if(checkMem(new, "ERROR: Memory cant be allocated.\n"))
    {
        return NULL;
    }
    return new;
}

int addStation(stationsADT st, size_t id, const char * name)
{
    /* Si se agregan estaciones nuevas se asume que el vector de estaciones quedo desordenado */
    st->sortedName = 0;
    /* No queda espacio para agregar una estacion */
    if(st->dim == st->size)
    {
        /* Debo agrandar tanto la matriz como el vector pues usan el mismo size */
        errno = 0;
        st->stations = realloc(st->stations, (st->size + BIG_BLOCK) * sizeof(TStation));
        if(checkMem(st->stations, "ERROR: Memory cant be allocated.\n"))
        {
            return -1;
        }

        /* Creamos una nueva matriz que satisfaga el nuevo tamaño */
        errno = 0;
        size_t ** newMatrix = malloc((st->size + BIG_BLOCK) * sizeof(size_t *));
        if(checkMem(newMatrix, "ERROR: Memory cant be allocated.\n"))
        {
            return -1;
        }

        for(int i=0; i<(st->size + BIG_BLOCK); i++)
        {
            errno = 0;
            newMatrix[i] = calloc((st->size + BIG_BLOCK), sizeof(size_t));
            if(checkMem(newMatrix[i], "ERROR: Memory cant be allocated.\n"))
            {
                return -1;
            }
        }

        /* Copiamos los viejos elementos */
        for(int i=0; i<st->size; i++)
        {
            memcpy(newMatrix[i], st->matrix[i], st->size * sizeof(size_t));
        }

        /* Liberamos la vieja matriz */
        for(int i=0; i<st->size; i++)
        {
            free(st->matrix[i]);
        }
        free(st->matrix);

        /* Apuntamos a la nueva matriz */
        st->matrix = newMatrix;
        st->size += BIG_BLOCK;
    }

    /* Sabemos que hay espacio suficiente, agregamos la estacion */
    st->stations[st->dim].id = id;
    st->stations[st->dim].index = st->dim; /* Asignamos como indice interno el lugar donde lo guardamos */
    st->stations[st->dim].memberTrips = 0;
    
    /* Inicializamos su vector de meses */
    for(int i=0; i<MONTHS_QTY; i++)
    {
        st->stations[st->dim].monthsVec[i] = 0;
    }

    errno = 0;
    st->stations[st->dim].name = copyStr(name, &st->stations[st->dim].len);
    if(checkMem(st->stations[st->dim].name, "ERROR: Memory cant be allocated.\n"))
    {
        return -1;
    }
    st->dim++;
    return 1;
}

static int compareStationId(const void * s1, const void * s2)
{
    const TStation * station1 = (const TStation *)s1;
    const TStation * station2 = (const TStation *)s2;

    /* Evitamos errores por el tipo de dato size_t */
    return (int)station1->id - (int)station2->id;
}

static int getIdx(TStation vector[], size_t dim, size_t id)
{
    /* Creamos una key auxiliar con el id que estamos buscando */
    TStation * key = malloc(sizeof(TStation));
    key->id = id;

    /* Aprovechamos el orden del vector para aplicar busqueda binaria */
    TStation * ans = bsearch(key, vector, dim, sizeof(TStation), compareStationId);

    /*Liberamos el espacio auxiliar */
    free(key);

    if(ans == NULL)
    {
        return -1;
    }
    else
    {
        /* Devolvemos el indice del elemento en la matriz */
        return ans->index;
    }
}

int addRide(stationsADT st, size_t startId, size_t endId, int isMember, const char * startDate)
{
    /* Si el vector no esta ordenado lo ordenamos por nombre */
    if(st->sortedId == 0)
    {
        sortBy(st->stations, st->dim, compareStationId);
        /* El vector deja de estar ordenado por nombre si es que lo estaba */
        st->sortedName = 0;
        st->sortedId = 1;
    }

    int startIdx, endIdx; /* Indices internos de viaje de ida y de vuelta */

    /* Chequeamos que las estaciones de inicio y fin existan */
    startIdx = getIdx(st->stations, st->dim, startId);
    if(startIdx == -1)
    {
        return 0;
    }

    endIdx = getIdx(st->stations, st->dim, endId);
    if(endIdx == -1)
    {
        return 0;
    }

    st->matrix[startIdx][endIdx]++; /* Se agrega un viaje desde startIdx hasta endIdx */
    st->stations[startIdx].memberTrips += (isMember == 1);
    st->stations[startIdx].monthsVec[atoi(startDate + 5) - 1]++; /* Agregamos un viaje al mes correspondiente */
    return 1;
}

static int compareStationNames(const void * s1, const void * s2)
{
    const TStation * station1 = (const TStation *) s1;
    const TStation * station2 = (const TStation *) s2;

    return strcmp(station1->name, station2->name);
}

static void sortBy(TStation vector[], size_t dim, TCompare f)
{
    qsort(vector, dim, sizeof(TStation), f);
}

static query1List query1Add(query1List list, const char * name, size_t len, size_t startedTrips)
{
    if(list == NULL || startedTrips > list->startedTrips || (list->startedTrips == startedTrips && strcmp(name, list->name) < 0))
    {
        errno = 0;
        query1List newNode = malloc(sizeof(struct query1Node));
        if(checkMem(newNode, "ERROR: Memory cant be allocated.\n"))
        {
            return NULL;
        }
        newNode->startedTrips = startedTrips;
        
        errno = 0;
        newNode->name = malloc((len + 1) * sizeof(char));
        if(checkMem(newNode, "ERROR: Memory cant be allocated.\n"))
        {
            return NULL;
        }
        
        strcpy(newNode->name, name);
        
        newNode->tail = list;
        return newNode;
    }
    list->tail = query1Add(list->tail, name, len, startedTrips);
    return list;
}

query1List query1(stationsADT st, int * flag)
{
    /* Inicializamos la lista vacia para devolver */
    query1List ans = NULL;

    for(int i=0; i<st->dim; i++)
    {
        ans = query1Add(ans, st->stations[i].name, st->stations[i].len, st->stations[i].memberTrips);
        if(ans == NULL)
        {
            *flag = -1;
            return NULL;
        }
    }

    *flag = 1;
    return ans;
}

query2Elem * query2(stationsADT st, int * qty)
{
    /* Si el vector no estaba ordenado por nombre lo ordenamos por comodidad */
    if(st->sortedName == 0)
    {
        sortBy(st->stations, st->dim, compareStationNames);
        st->sortedName = 1;
        /* Si ordenamos por nombre deja de estar ordenado por ID */
        st->sortedId = 0;
    }

    /* La cantidad de elementos sera la misma que una matriz cuadrada pero sin la diagonal */
    size_t elems = (st->dim * st->dim) - st->dim;
    int k = 0; /* Indice para guardar los elementos en el vector */

    errno = 0;
    query2Elem * ans = malloc(elems * sizeof(query2Elem));
    if(checkMem(ans, "ERROR: Memory cant be allocated.\n"))
    {
        *qty = -1;
        return NULL;
    }

    for(int i=0; i<st->dim; i++)
    {
        for(int j=0; j<st->dim; j++)
        {
            if(st->stations[i].id != st->stations[j].id) /* Descartamos viajes circulares */
            {
                errno = 0;
                ans[k].stationA = malloc((st->stations[i].len + 1) * sizeof(char));
                if(checkMem(ans[k].stationA, "ERROR: Memory cant be allocated.\n"))
                {
                    *qty = -1;
                    return NULL;
                }
                strcpy(ans[k].stationA, st->stations[i].name);

                errno = 0;
                ans[k].stationB = malloc((st->stations[j].len + 1) * sizeof(char));
                if(checkMem(ans[k].stationB, "ERROR: Memory cant be allocated.\n"))
                {
                    *qty = -1;
                    return NULL;
                }
                strcpy(ans[k].stationB, st->stations[j].name);

                ans[k].AtoB = st->matrix[st->stations[i].index][st->stations[j].index];
                ans[k].BtoA = st->matrix[st->stations[j].index][st->stations[i].index];
                k++;
            }
        }
    }

    *qty = k;
    return ans;
}

query3Elem * query3(stationsADT st, int * qty)
{
    /* Si el vector no estaba ordenado por nombre lo ordenamos por comodidad */
    if(st->sortedName == 0)
    {
        sortBy(st->stations, st->dim, compareStationNames);
        st->sortedName = 1;
        /* Si ordenamos por nombre deja de estar ordenado por ID */
        st->sortedId = 0;
    }

    /* La cantidad de elementos sera la misma que la cantidad de estaciones */
    size_t elems = st->dim;

    errno = 0;
    query3Elem * ans = malloc(elems * sizeof(query3Elem));
    if(checkMem(ans, "ERROR: Memory cant be allocated.\n"))
    {
        *qty = -1;
        return NULL;
    }

    int k = 0; /* Indice para guardar los elementos en el vector */

    for(int i=0; i<st->dim; i++)
    {
        errno = 0;
        ans[k].name = malloc((st->stations[i].len + 1) * sizeof(char));
        if(checkMem(ans[k].name, "ERROR: Memory cant be allocated.\n"))
        {
            *qty = -1;
            return NULL;
        }
        strcpy(ans[k].name, st->stations[i].name);

        errno = 0;
        ans[k].mv = malloc(MONTHS_QTY * sizeof(size_t));
        if(checkMem(ans[k].mv, "ERROR: Memory cant be allocated.\n"))
        {
            *qty = -1;
            return NULL;
        }

        for(int j=0; j<MONTHS_QTY; j++)
        {
            ans[k].mv[j] = st->stations[i].monthsVec[j];
        }

        k++;
    }

    *qty = k;
    return ans;
}

void freeQuery1(query1List list)
{
    if(list == NULL)
    {
        return;
    }

    freeQuery1(list->tail);
    free(list->name);
    free(list);
}

void freeQuery2(query2Elem * vec, size_t qty)
{
    for(int i=0; i<qty; i++)
    {
        free(vec[i].stationA);
        free(vec[i].stationB);
    }
    free(vec);
}

void freeQuery3(query3Elem * vec, size_t qty)
{
    for(int i=0; i<qty; i++)
    {
        free(vec[i].name);
        free(vec[i].mv);
    }
    free(vec);
}

void freeStations(stationsADT st)
{
    for(int i=0; i<st->dim; i++)
    {
        free(st->stations[i].name);
    }
    free(st->stations);
    
    for(int i=0; i<st->size; i++)
    {
        free(st->matrix[i]);
    }
    free(st->matrix);

    free(st);
}
