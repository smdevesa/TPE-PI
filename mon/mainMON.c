#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "../lib/frontLib.h"

/* Se piden los nombres de los archivos .csv como entrada */
int main(int argc, char ** argv)
{
    printf("%d",argc);
    if(argc != 3)
    {
        fprintf(stderr, "La cantidad de archivos enviada es incorrecta.");
        exit(1);
    }

    FILE * FBikes = fopen(argv[1], "r");

    if(FBikes == NULL)
        puts("No se pudo abrir el archivo.");
    
    int flag = INIT;
    char * field1 = readField(FBikes, &flag);
    printf("flag: %d, field = %s\n",flag, field1);
    flag = INIT;
    char * field2 = readField(FBikes, &flag);
    printf("flag: %d, field = %s\n",flag, field2);

    fclose(FBikes);
    free(field1);
    free(field2);
    return 0;
}
