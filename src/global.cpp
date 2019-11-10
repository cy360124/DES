#include "global.h"

char instr[MAX_STR];            // instruction
char input[64];                 // input
char output[64];                // output
char primaryKey[64];            // the primary Key
char subKey[ROUND_TIMES][48];   // sub keys
char vector[64];                // initial vector
char mode;                      // mode
FILE * in_fp;                   // the pointer of input file
FILE * out_fp;                  // the pointer of output file
bool prepare = false;           // prepare status, initial value: false