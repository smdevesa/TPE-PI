#ifndef _FRONTLIB_H_
#define _FRONTLIB_H_

#include <stdio.h>

#define BLOCK 10
#define INIT -1 /* Inicializador para el flag */
enum {SEMICOLON = 0, NEWLINE, END}; 

/* 
** Chequea que dir no sea NULL, 
** en caso de serlo aborta y escribe un error en stderr 
 */
void checkMem(void * dir);

/* 
** Lee un campo del archivo hasta un ';',un'\n' o un EOF
** y devuelve una copia de todo lo que leyo 
** en flag deja la razon de su corte (ver ENUM)
 */
char * readField(FILE * file, int * flag);

#endif
