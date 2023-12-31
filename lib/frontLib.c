/*
**  Autores: smdevesa y jrambau
**  Version: 1.1
**  Fecha: 10/07/2023
**  
**  Codigo fuente de la libreria de frontend para los programas de MON y NYC.
 */

#include "frontLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define DELIM ";\n"
#define MAXBUFFER 256
#define BLOCK 15

/*
** Si el puntero recibido es nulo imprime el mensaje recibido
** y aborta en ejecucion.
 */
static void checkMem(void * ptr, const char * message);

/*
** Copia por bloques el string recibido y devuelve en su
** nombre la direccion de la copia. En caso de error de
** memoria imprime error en stderr y devuelve NULL.
 */
static char * copyStr(const char * s);

static void checkMem(void * ptr, const char * message)
{
    if(ptr == NULL || errno == ENOMEM)
    {
        fprintf(stderr, "%s", message);
        exit(1);
    }
}

/*
** Copia por bloques el string recibido y devuelve en su
** nombre la direccion de la copia. En caso de error de
** memoria imprime error en stderr y devuelve NULL.
 */
static char * copyStr(const char * s)
{
    char * ans = NULL;
    int i;

    for(i=0; s[i] != 0; i++)
    {
        if(i % BLOCK == 0)
        {
            errno = 0;
            ans = realloc(ans, (BLOCK+i) * sizeof(char));
            checkMem((void *)ans, "ERROR: Memory cant be allocated. Aborting.\n");
        }
        ans[i] = s[i];
    }

    errno = 0;
    ans = realloc(ans, (i+1) * sizeof(char));
    checkMem((void *)ans, "ERROR: Memory cant be allocated. Aborting.\n");
    ans[i] = 0;
    return ans;
}

char ** getField(char * line, int fields)
{
    errno = 0;
    char ** ans = malloc(fields * sizeof(char *));
    checkMem((void *) ans, "ERROR: Memory cant be allocated. Aborting.\n");

    /* Separamos el string y copiamos el primer campo*/
    ans[0] = copyStr(strtok(line, DELIM));  
    for(int i=1; i < fields; i++)
    {
        /* Copiamos campo a campo */
        ans[i] = copyStr(strtok(NULL, DELIM));
    }
    return ans;
}

char * sizeToString(size_t num)
{
    /* Si el numero es 0 tiene un digito */
    int digits = (num == 0);
    size_t aux = num;

    /* Contamos la cantidad de digitos */
    while(aux > 0)
    {
        digits++;
        aux /= 10;
    }
    errno = 0;
    char * ans = malloc((digits + 1) * sizeof(char));
    checkMem((void *) ans, "ERROR: Memory cant be allocated. Aborting.\n");

    /* Cargamos los numeros en el string al reves de como se reciben */
    for(int i=(digits-1); i>=0; i--)
    {
        ans[i] = num % 10 + '0';
        num /= 10;
    }
    ans[digits] = 0;
    return ans;
}

void checkFile(FILE * file)
{
    if(file == NULL)
    {
        fprintf(stderr,"ERROR: file cant be opened. Aborting.\n");
        exit(1);
    }
}
