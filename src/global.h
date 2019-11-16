#ifndef CODE_GLOBAL_H
#define CODE_GLOBAL_H

#include <stdio.h>
#define MAX_STR 100      // the max length of input from console
#define ROUND_TIMES 8    // the round times of DES
#define RSABITS 128      // the max bits of RSA
#define TESTTIMES 32     // test times of Miller_Rabin

extern char primaryKey[64];            // the primary Key
extern char vec[64];                // initial vector

extern FILE * in_fp;                   // the pointer of input file
extern FILE * out_fp;                  // the pointer of output file


#endif  /* CODE_GLOBAL_H */
