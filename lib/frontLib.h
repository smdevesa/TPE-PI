#ifndef _FRONTLIB_H_
#define _FRONTLIB_H_

#include <stdio.h>

/*
** @param line Linea con los campos sin separar.
** @param fields Cantidad de campos que tendra cada linea del archivo
**
** @returns Vector de strings con cada campo separado. En la posicion 0
**          estaria el primero que encuentre y asi sucesivamente.
*/
char ** getField(char * line, int fields);

/*
** @returns Un string con el numero recibido.
 */
char * sizeToString(size_t num);

#endif
