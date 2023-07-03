#include "frontLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DELIM ";"
#define MAXBUFFER 256
#define BLOCK 50

static void checkMem(void * ptr, const char * message)
{
    if(ptr == NULL)
    {
        fprintf(stderr, "%s", message);
        exit(1);
    }
}

static char * copyStr(char * s)
{
    char * ans = NULL;
    int i;

    for(i=0; s[i] != 0; i++)
    {
        if(i % BLOCK == 0)
        {
            ans = realloc(ans, (BLOCK+i) * sizeof(char));
            checkMem((void *)ans, "ERROR: Cannot allocate memory.");
        }
        ans[i] = s[i];
    }

    ans = realloc(ans, (i+1) * sizeof(char));
    checkMem((void *)ans, "ERROR: Cannot allocate memory.");
    ans[i] = 0;
    return ans;
}

char ** getField(char * line,int fields)
{
    char ** ans = malloc(fields * sizeof(char *));
    checkMem((void *) ans,"ERROR: Cannot allocate memory");

    ans[0] = copyStr(strtok(line, DELIM));  
    printf("%s\n",ans[0]);
    for(int i=1; i < fields; i++)
    {
        ans[i] = copyStr(strtok(NULL, DELIM));
        printf("%s\n",ans[i]);
    }

    return ans;
}
