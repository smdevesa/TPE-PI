#ifndef _FRONTLIB_H_
#define _FRONTLIB_H_

#include <stdio.h>

#define BLOCK 10
#define INIT -1
enum {SEMICOLON = 0, NEWLINE, END};

void checkMem(void * dir);

char * readField(FILE * file, int * flag);

#endif
