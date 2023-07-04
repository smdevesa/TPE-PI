#ifndef _STATIONSADT_H_
#define _STATIONSADT_H_

#include <stddef.h>

typedef struct stationsCDT * stationsADT;


/* Estructura de la lista utilizada para devolver los datos solicitados en la query 1 */
typedef struct query1Node
{
    char * name;
    size_t startedTrips;
    struct query1Node * tail;
} query1Node;

typedef struct query1Node * query1List;
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
int addStation(stationsADT st, size_t id, char * name);


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
int addRide(stationsADT st, size_t startId, size_t endId, int isMember, char * startDate);

/*
** @returns Una lista ordenada por cantidad de viajes de miembros
**          y desempatada por nombre de estacion para cada una de
**          las estaciones del ADT. Ver estructura query1List.
 */
query1List query1(stationsADT st);

/*
** Libera la lista de tipo query1 que recibe como parametro.
 */
void freeQuery1List(query1List list);


/*
** Libera el ADT que recibe como parametro.
*/
void freeStations(stationsADT st);



#endif
