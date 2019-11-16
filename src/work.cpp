#include <stdio.h>
#include "global.h"
#include "assist.h"
#include "DES.h"
#include "RSA.h"

char input[64];                 // input
char output[64];                // output

// read the instruction
void read_instr(char str[])
{
	scanf("%s", str);      // get instr
	str[MAX_STR- 1] = '\0';  // make sure a string
	fflush(stdin);         // clear the input buffer
}

// try opening a file
// SE: 0 is input file, 1 is output file
// return value: 0 is success, 1 is fail
int open_file(int SE, const char * fname)
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
void out_to_file(const char src[])
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
int read_mode(char *des)
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
// return value: 0 is successful; 1 is fail;
int read_primaryKey_or_vector(char des[])
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

// read the input from input file
// return value: 0 is not at the end, 1 is at the end, 2 is error char
// num is the number of read chars in this time
int read_input(char des[], int *num)
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

// use DES to encrypt the initial vector and output
void encrypt_IV(const char src[])
{
    DES_encryption(output, src);
    out_to_file(output);
}

// get initial vector from input file
void decrypt_IV(char des[])
{
    int num;
    char temp[64];
    // call read_input to get initial vector from input file
    read_input(temp, &num);
    // decryption
    DES_decryption(des, temp);
}

// use RSA to encrypt the primaryKey and ouput
void encrypt_primaryKey(const char src[])
{
    char temp[128];
    // RSA encryption
    RSA_encryption(temp, src);
    // output
    out_to_file(temp);
    out_to_file(&temp[64]);
}

// use RSA to decrypt the primaryKey from input file
void decrypt_primaryKey(char des[])
{
    int num;
    char temp[128];
    // call read_input to get primaryKey from input file
    read_input(temp, &num);
    read_input(&temp[64], &num);
    RSA_decryption(des, temp);
}

// add some information at the end of output file
void add_mark(int num)
{
    Int2Char(input, num, 64);                              // conduct input[]
    my_xor(input, input, vec, 64);                         // input = input xor vector, equal to (a + b) mod 2
    DES_encryption(output, input);                         // encryption
    out_to_file(output);                                   // output to file
}

// achieve the encryption in CBC
int CBC_encryption()
{
    int num;        // to count
    int judge;      // to judge

    // conduct eight sub keys
    conduct_subKey(primaryKey);

    // use RSA to encrypt the primaryKey and ouput
    encrypt_primaryKey(primaryKey);

    // use DES to encrypt initial vector and output
    encrypt_IV(vec);

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
            my_xor(input, input, vec, 64);   // input = input xor vector, equal to (a + b) mod 2
            DES_encryption(output, input);          // encryption
            copy(vec, output, 64);           // update vector
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
int CBC_decryption()
{
    int num;    // to count
    int judge;  // to judge

    // get primaryKey
    decrypt_primaryKey(primaryKey);

    // conduct eight sub keys
    conduct_subKey(primaryKey);

    // get initial vector
    decrypt_IV(vec);

    // check
    printf("primarkKey is:\n");
    for (int i = 0; i < 64; i++) printf("%c", primaryKey[i]);
    printf("\n");
    printf("initial vector is:\n");
    for (int i = 0; i < 64; i++) printf("%c", vec[i]);
    printf("\n");

    // read and deal with
    while (1) {
        judge = read_input(input, &num);
        // if success
        if (judge < 2) {
            // if at the end but read no char, do not need to deal, just break
            if (judge == 1 && num == 0) break;
            // if not at the end
            DES_decryption(output, input);            // decryption
            my_xor(output, output, vec, 64);   // output = output xor vector, equal to (a + b) mod 2
            copy(vec, input, 64);              // update vector
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



