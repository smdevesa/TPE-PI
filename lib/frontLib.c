#include "frontLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DELIM ";\n"
#define MAXBUFFER 256
#define BLOCK 15

/*
** Si el puntero recibido es nulo imprime el mensaje recibido
** y aborta en ejecucion.
 */
static void checkMem(void * ptr, const char * message)
{
    if(ptr == NULL)
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
            ans = realloc(ans, (BLOCK+i) * sizeof(char));
            checkMem((void *)ans, "ERROR: Memory cant be allocated.\n");
        }
        ans[i] = s[i];
    }

    ans = realloc(ans, (i+1) * sizeof(char));
    checkMem((void *)ans, "ERROR: Memory cant be allocated.\n");
    ans[i] = 0;
    return ans;
}

char ** getField(char * line, int fields)
{
    char ** ans = malloc(fields * sizeof(char *));
    checkMem((void *) ans, "ERROR: Memory cant be allocated.\n");

    /* Separamos el string */
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
    int digits = num > 0 ? 0 : 1;
    size_t aux = num;

    while(aux > 0)
    {
        digits++;
        aux /= 10;
    }
    char * ans = malloc((digits + 1) * sizeof(char));
    checkMem((void *) ans, "ERROR: Memory cant be allocated.\n");

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
        fprintf(stderr,"ERROR: file cant be opened.\n");
        exit(1);
    }
}
