#ifndef CODE_GLOBAL_H
#define CODE_GLOBAL_H

#include <stdio.h>
#define MAX_STR 100
#define ROUND_TIMES 8  

extern char instr[MAX_STR];
extern char input[64];
extern char output[64];
extern char primaryKey[64];
extern char subKey[ROUND_TIMES][48]; 
extern char vector[64];
extern char mode;
extern FILE * in_fp;
extern FILE * out_fp;
extern bool prepare;

#endif  /* CODE_GLOBAL_H */