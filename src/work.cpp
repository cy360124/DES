#include <stdio.h>
#include "global.h"
#include "assist.h"
#include "DES.h"

// try open a file
// SE: 0 is input file, 1 is output file
// return value: 0 is success, 1 is fail
int 
open_file(int SE, char fname[])
{
    if (SE == 0) {
        in_fp = fopen(fname, "r");
        if (in_fp == NULL) {
            printf("(DES)--> Error, can not open the file %s.\n", fname);
            printf("(DES)--> Please check it and run again.\n");
        return 1;
        }
    }
    else {
        out_fp = fopen(fname, "w");
        if (out_fp == NULL) {
            printf("(DES)--> Error, can not open the file %s.\n", fname);
            printf("(DES)--> Please check it and run again.\n");
        return 1;
        }
    }
    
    return 0;
}

// printf to file
void
out_to_file(char src[])
{
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++)
            fputc(src[i * 8 + j], out_fp);
        if (i != 7)
            fputc(' ', out_fp);
        else
            fputc('\n', out_fp);
    }
}


// read the selection of mode
// return value: 0 is success, 1 is fail
int 
read_mode(char *des)
{
    int i;                              // index
    char str[MAX_STR];                  // store the input

    scanf("%s", str);                   // get input
    str[MAX_STR - 1] ='\0';             // be sure a string
    fflush(stdin);                      // clear the input buffer
    
    // find the first valid char
    for (i = 0; str[i]; i++)  
        if (str[i] != ' ') break;
    
    // judge whether illegal
    if (str[i] == 'E' || str[i] == 'e') {
        *des = 'E'; return 0;
    }
    else if (str[i] == 'D' || str[i] == 'd') {
        *des = 'D'; return 0;
    }
    else {
        printf("(DES)--> Error, your input is wrong. Please prepare again.\n");
        return 1;
    }
}

// read the primaryKey or vector
// return value: 
// 0 is successful; 1 is fail; 
int 
read_primaryKey_or_vector(char des[])
{
    char str[MAX_STR];             // store the input 
    int i, head, end;              // index, the first valid char, the '\0'

    // get input
    scanf("%s", str);              // get input
    fflush(stdin);                 // clear the input buffer
    str[MAX_STR - 1] = '\0';       // make sure being a string
    
    // find head
    for(i = 0; str[i]; i++) { if(str[i] != ' ') break; }
    head = i;
    // find end
    for(i = 0; str[i]; i++);
    end = i;

    // check whether legal
    for(i = head; i < end; i++)
        if (!(('0' <= str[i] && str[i] <= '9') || ('A' <= str[i] && str[i] <= 'F') || ('a' <= str[i] && str[i] <= 'f'))) {
            printf("(DES)--> Error, your input contains illegal chars.\n");
            printf("(DES)--> Please check it and prepare again.\n");
            return 1;
        }
    
    // check the length. If ok, turn into binary
    if (end - head < 16) {
        printf("(DES)--> Error, your input is less than 16 chars.\n");
        printf("(DES)--> Please check it and prepare again.\n");
        return 1;
    }
    else if (end - head > 16) {
        printf("(DES)--> Error, your input is more than 16 chars.\n");
        printf("(DES)--> Please check it and prepare again.\n");
        return 1;
    }
    else {
        H2B(des, &str[head], 16);
        return 0;
    }

}

// read the input
// return value: 0 is not at the end, 1 is at the end, 2 is error char
// num is the number of read chars in this time
int 
read_input(char des[], int *num)
{
    *num = 0;          // set count to zero
    char c;            // store char
    // read chars in group of 64-bits
    for (int i = 0; i < 64; i++) {
        c = fgetc(in_fp);     
        // if at the end                             
        if (c == EOF) {                                  
            for (int j = i; j < 64; j++) des[j] = '0';    // add '0' into the rest of des[]
            return 1;
        }
        // if not at the end, and c is 0 or 1
        else if (c == '0' || c == '1') {              
            des[i] = c; *num = *num + 1;                  // store and num++
        }
        // ignore blank char
        else if (c == ' ' || c == '\n') {
            i--;                                          // roll back
        }
        // illegal char
        else {
            printf("(DES)--> Error, the input file contains illegal chars.\n");
            printf("(DES)--> Please check it and run again.\n");
            return 2;
        }
    }
    
    // success
    return 0;
}

// get initial vector at the head of input file
void 
get_IV(char des[])
{
    int num;
    char temp[64];
    // call read_input to get initial vector at the head of input file
    read_input(temp, &num);
    // decryption
    decryption(des, temp);
}

// add some information at the end of output file
void
add_mark(int num)
{
    char add[64];
    Int2Char(add, num, 64);         // conduct add[]
    my_xor(add, add, vector, 64);   // input = input xor vector, equal to (a + b) mod 2
    encryption(output, add);        // encryption
    out_to_file(output);            // output to file
}

// achieve the encryption in CBC
int 
encryption_CBC()
{
    int num;    // to count
    int judge;  // to judge

    // conduct eight sub keys
    conduct_subKey(subKey, primaryKey);

    // for initial vector, do encryption 
    encryption(output, vector);
    out_to_file(output);

    // read the file and deal with
    while (1) {
        judge = read_input(input, &num);
        // if success
        if (judge < 2) {
            // if at the end but read no char, it means 64-bit asign 
            if (judge == 1 && num == 0) {
                add_mark(64); break;           // just add_mark and break
            }
            // once read chars, deal with them
            my_xor(input, input, vector, 64);   // input = input xor vector, equal to (a + b) mod 2
            encryption(output, input);          // encryption
            copy(vector, output, 64);           // update vector
            out_to_file(output);                // output
            // if at the end and read some chars
            if (judge == 1) {
                add_mark(num); break;           // add_mark and break
            }
        }
        // if any error
        else {
            return 1;  
        } 
    }

    return 0;
}

// achieve the decryption in CBC
int
decryption_CBC()
{
    int num;    // to count
    int judge;  // to judge

    // conduct eight sub keys
    conduct_subKey(subKey, primaryKey);

    // get initial vector
    get_IV(vector);

    // read and deal with
    while (1) {
        judge = read_input(input, &num);
        // if success
        if (judge < 2) {
            // if at the end but read no char, do not need to deal, just break
            if (judge == 1 && num == 0) break; 
            // if not at the end
            decryption(output, input);            // decryption
            my_xor(output, output, vector, 64);   // output = output xor vector, equal to (a + b) mod 2
            copy(vector, input, 64);              // update vector
            out_to_file(output);                  // output to file
            // if at the end and read some chars, after dealing, break
            if (judge == 1) break;
        }
        // if any error
        else {
            return 1;  
        } 
    }

    return 0;
}

    
        
            