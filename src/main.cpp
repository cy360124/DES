#include <stdio.h>
#include <string.h>
#include "global.h"
#include "assist.h"
#include "RSA.h"
#include "work.h"

char instr[MAX_STR];            // instruction
char mode;                      // mode

int main()
{
    printf("(DES)--> Be loading, please wait for a minute.\n");
    load_RSA();   // load RSA, conducting Keys
    printf("(DES)--> Loading is finished.\n");
    welcome();    // introduce the DES
    // loop
    while (1)
    {
        wait();                                    // wait for instr
        read_instr(instr);                         // read instr
        // help
        if (strcmp(instr, "help") == 0) {
            help();
        }
        else if (strcmp(instr, "run") == 0) {
            // select mode
            printf_mode();                                         // intro
            wait();                                                // wait for input
            if (read_mode(&mode)) continue;                        // if wrong, exit running

            // if do encryption
            if (mode == 'E') {
                // get primaryKey
                printf_primaryKey();                                   // introduce
                wait();                                                // wait for input
                if (read_primaryKey_or_vector(primaryKey))  continue;  // if wrong, exit prepare
                // get initial vector
                printf_vector();                                       // introduce
                wait();                                                // wait for input
                if (read_primaryKey_or_vector(vec))  continue;      // if wrong, exit prepare
            }

            // try open file
            if (open_file(0, "data/input.txt")) continue;          // if error, exit running
            if (open_file(1, "data/output.txt")) continue;         // if error, exit running

            //  do work
            if (mode == 'E') {                                     // do encryption
                if (CBC_encryption()) continue;                    // if error, exit running
            }
            else {                                                 // do decryption
                if (CBC_decryption()) continue;                    // if error, exit running
            }

            // close file
            fclose(in_fp);                                         // close input.txt
            fclose(out_fp);                                        // close output.txt

            // success
            printf("(DES)--> Run successfully!\n");
        }
        // exit
        else if (strcmp(instr, "exit") == 0) {
            break;
        }
        // error input
        else {
            printf("(DES)--> Error, your input is wrong. Please try again.\n");
        }
    }

    return 0;
}
