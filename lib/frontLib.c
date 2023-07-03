#include "frontLib.h"
#include <stdio.h>
#include <stdlib.h>

void checkMem(void * dir)
{
    if(dir == NULL)
    {
        fprintf(stderr, "Error al asignar memoria");
        exit(1);
    }
}

char * readField(FILE * file, int * flag)
{
    int condition = 1, aux = *flag;
    int size = 0, len = 0;
    char * ans = NULL;
    while(condition)
    {
        int c = fgetc(file);

        if(c == ';')
        {
            *flag = SEMICOLON;
        }
        else if(c == '\n')
        {
            *flag = NEWLINE;
        }
        else if(c == EOF)
        {
            *flag = END;
        }
        else
        {
            if(size == len)
            {
                ans = realloc(ans, sizeof(char) * (size + BLOCK));
                checkMem((void *)ans);
                size += BLOCK;
            }
            ans[len++] = c;
        }
        if(aux != *flag)
        {
            condition = 0;
        }
    }
    ans = realloc(ans, sizeof(char) * (len+1));
    checkMem((void *)ans);
    ans[len] = 0;
    return ans;
}

