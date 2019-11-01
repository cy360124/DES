#ifndef CODE_GLOBAL_H
#define CODE_GLOBAL_H

#include <stdio.h>
#define MAX_STR 100
#define ROUND_TIMES 8  

char instr[MAX_STR];
char input[64];
char output[64];
char primaryKey[64];
char subKey[ROUND_TIMES][48]; 
char vector[64];
char mode;
FILE * in_fp;
FILE * out_fp;

#endif  /* CODE_GLOBAL_H */