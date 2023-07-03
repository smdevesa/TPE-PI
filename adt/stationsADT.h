#ifndef _STATIONSADT_H_
#define _STATIONSADT_H_

#include <ctype.h>

typedef struct stationsCDT * stationsADT;


/*
** @returns Un nuevo ADT de control de estaciones.
**          En caso de error retorna NULL.
** 
** @param hasMultipleTypes Flag para usar diferentes tipos de bicicletas
**                         en el sistema. Utilice 1 para utilizar distintos tipos, caso contrario 0.
 */
stationsADT newStationsADT(int hasMultipleTypes);

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

#endif