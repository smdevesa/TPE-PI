#ifndef _FRONTLIB_H_
#define _FRONTLIB_H_

#include <stdio.h>

#define ARGS_EXPECTED 3

/*Constantes para escritura de archivos .csv y .html*/
#define QUERY1_TABLE_NAME "query1.html"
#define QUERY1_CSV_NAME "query1.csv"
#define QUERY1_COLS 2
#define QUERY1_F1 "Station"
#define QUERY1_F2 "StartedTrips"
#define QUERY2_TABLE_NAME "query2.html"
#define QUERY2_CSV_NAME "query2.csv"
#define QUERY2_COLS 4
#define QUERY2_F1 "StationA"
#define QUERY2_F2 "StationB"
#define QUERY2_F3 "Trips A->B"
#define QUERY2_F4 "Trips B->A"
#define QUERY3_TABLE_NAME "query3.html"
#define QUERY3_CSV_NAME "query3.csv"
#define QUERY3_COLS 13
#define QUERY3_F1 "J"
#define QUERY3_F2 "F"
#define QUERY3_F3 "M"
#define QUERY3_F4 "A"
#define QUERY3_F5 "M"
#define QUERY3_F6 "J"
#define QUERY3_F7 "J"
#define QUERY3_F8 "A"
#define QUERY3_F9 "S"
#define QUERY3_F10 "O"
#define QUERY3_F11 "N"
#define QUERY3_F12 "D"
#define QUERY3_F13 "Station"

/*
** 
** En caso de haber un error al alocar memoria, el programa aborta.
**
** @returns Vector de strings con cada campo separado. En la posicion 0
**          estaria el primero que encuentre y asi sucesivamente.
**
** @param line Linea con los campos sin separar.
** @param fields Cantidad de campos que tendra cada linea del archivo
*/
char ** getField(char * line, int fields);

/*
**
** En caso de haber un error al alocar memoria, el programa aborta.
**
** @returns Un string con el numero positivo recibido.
 */
char * sizeToString(size_t num);

/*
** Verifica que un archivo se haya abierto de forma correcta
** en caso de que no se haya podido abrir imprime un error en
** stderr y aborta en ejecucion.
 */
void checkFile(FILE * file);

#endif
