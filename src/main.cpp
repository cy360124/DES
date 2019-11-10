#include <stdio.h>
#include <string.h>
#include "global.h"
#include "assist.h"
#include "work.h"

int main()
{
    welcome();    // introduce the DES
    // loop
    while (1) 
    {
        wait();                                    // wait for instr
        read_instr(instr, MAX_STR);                // read instr
        // help
        if (strcmp(instr, "help") == 0) {
            help();
        }
        // prepare
        else if (strcmp(instr, "prepare") == 0) {
            // select mode
            printf_mode();                                         // intro
            wait();                                                // wait for input
            if (read_mode(&mode)) continue;                        // if wrong, exit prepare

            // get primaryKey
            printf_primaryKey();                                   // introduce 
            wait();                                                // wait for input
            if (read_primaryKey_or_vector(primaryKey))  continue;  // if wrong, exit prepare
            
            // if do encryption, get initial vector
            if (mode == 'E') {
                printf_vector();                                   // introduce 
                wait();                                            // wait for input
                if (read_primaryKey_or_vector(vector))  continue;  // if wrong, exit prepare
            }

            // set prepare to true
            prepare = true;
            printf("(DES)--> Prepare DES successfully!\n");
        } 
        else if (strcmp(instr, "run") == 0) {
            // judge whether prepare
            if (!prepare) {
                printf("(DES)--> You have not prepared the DES before running. Please prepare it.\n");
                continue;
            }
            // try open file
            if (open_file(0, "data/input.txt")) continue;          // if error, exit running
            if (open_file(1, "data/output.txt")) continue;         // if error, exit running
            
            //  do work
            if (mode == 'E') {                                     // do encryption
                if (encryption_CBC()) continue;                    // if error, exit running
            }  
            else {                                                 // do decryption
                if (decryption_CBC()) continue;                    // if error, exit running
            }
            
            // close file
            fclose(in_fp);                                         // close input.txt
            fclose(out_fp);                                        // close output.txt

            // after running, set prepare to false
            prepare = false;
            printf("(DES)--> Run DES successfully!\n");
        }
        // quit
        else if (strcmp(instr, "quit") == 0) {
            break;
        }
        // error input
        else {
            printf("(DES)--> Error, your input is wrong. Please try again.\n");
        }
    }

    return 0;
}

