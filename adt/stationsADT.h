#ifndef _STATIONSADT_H_
#define _STATIONSADT_H_

/*
** Contrato de ADT que permite controlar un sistema de alquiler
** de bicicletas que permite almacenar datos y hacer consultas
** variadas explicadas en detalle en las funciones query1, query2 y query3.
 */

#include <stddef.h>

/* Cantidad de meses del año para la Query 3 */
#define MONTHS_QTY 12

typedef struct stationsCDT * stationsADT;

/* Estructura de los nodos de la lista utilizada para devolver los datos solicitados en la query 1 */
typedef struct query1Node
{
    char * name; /* Nombre de la estacion */
    size_t startedTrips; /* Cantidad de viajes empezados por MIEMBROS  en la estacion */
    struct query1Node * tail;
} query1Node;

typedef struct query1Node * query1List;

/* Estructura de los elementos del vector utilizado para devolver los datos solicitados en la query 2 */
typedef struct query2Elem
{
    char * stationA; /* Nombre de la estacion A */
    char * stationB; /* Nombre de la estacion B */
    size_t AtoB; /* Cantidad de viajes desde A hasta B */
    size_t BtoA; /* Cantidad de viajes desde B hasta A */
} query2Elem;

/* Estructura de los elementos del vector utilizado para devolver los datos solicitados en la query 3 */
typedef struct query3Elem
{
    char * name; /* Nombre de la estacion */
    /*
    ** Vector con la cantidad de viajes segun el mes del año
    ** donde cada indice simboliza un mes siendo 0 ENERO, 1 FEBRERO
    ** y asi sucesivamente hasta 11 DICIEMBRE.
     */
    size_t mv[MONTHS_QTY];
} query3Elem;

/*
** @returns Un nuevo ADT de control de estaciones.
**          En caso de error retorna NULL.
 */
stationsADT newStationsADT(void);

/*
** Agrega una nueva estacion (no repetida) al sistema.
**
** @returns 1 si se agrego con exito, 0 si estaba repetida
**          -1 ante un error de memoria.
**
** @param st Sistema de estaciones sobre el que se quiere agregar.
** @param id ID de la estacion a agregar.
** @param name Nombre de la estacion a agregar.
 */
int addStation(stationsADT st, size_t id, const char * name);


/*
** Agrega un nuevo recorrido al sistema.
**
** @returns 1 si pudo agregar un nuevo recorrido, 0 si no 
**          existe la estacion de inicio, -1 ante un error
**          de memoria o de tipo de dato recibido.
**
** @param st Sistema de estaciones sobre el que se quiere agregar.
** @param startId ID de la estacion donde inicio del recorrido.
** @param endId ID de la estacion donde finalizo del recorrido.
** @param isMember Flag de si el usuario es miembro (1) o no (0).
** @param startDate Fecha de inicio y hora de inicio del recorrido.
*/
int addRide(stationsADT st, size_t startId, size_t endId, int isMember, const char * startDate);

/*
** @returns Una lista ordenada por cantidad de viajes de miembros
**          y desempatada por nombre de estacion para cada una de
**          las estaciones del ADT. Ver estructura query1List.
**
** @param flag Parametro de salida. Sera 1 si se completo correctamente
**             la query, caso contrario sera -1.
 */
query1List query1(stationsADT st, int * flag);

/*
** Libera la lista de tipo query1 que recibe como parametro.
 */
void freeQuery1(query1List list);

/*
** @returns Un vector de query2Elem (ver estructura en el contrato)
**          ordenado por nombre de la estacion A y desempatado por
**          nombre de la estacion B con los viajes desde A hasta B
**          y desde B hasta A.
**
**  @param st El stationsADT sobre el que se quiere hacer la consulta.
**  @param qty Parametro de salida en el que se devuelve la dimension del vector.
**             En caso de error de memoria qty sera -1.
**
 */
query2Elem * query2(stationsADT st, int * qty);

/*
** Libera el vector de query2Elem que recibe como parametro
** siendo qty su dimension.
 */
void freeQuery2(query2Elem * vec, size_t qty);

/*
** @returns Un vector de query3Elem (ver estructura en el contrato)
**          ordenado por nombre de la estacion.
**
**  @param st El stationsADT sobre el que se quiere hacer la consulta.
**  @param qty Parametro de salida en el que se devuelve la dimension del vector.
**             En caso de error de memoria qty sera -1.
 */
query3Elem * query3(stationsADT st, int * qty);

/*
** Libera el vector de query3Elem que recibe como parametro
** siendo qty su dimension.
 */
void freeQuery3(query3Elem * vec, size_t qty);

/*
** Libera el ADT que recibe como parametro.
*/
void freeStations(stationsADT st);

#endif
