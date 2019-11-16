#ifndef CODE_ASSIST_H
#define CODE_ASSIST_H

void wait();
void welcome();
void help();
void printf_mode();
void printf_primaryKey();
void printf_vector();
void copy(char des[], const char src[], int size);
void my_xor(char des[], char src1[], char src2[], int size);
int Char2Int(char src[], int size);
void Int2Char(char des[], int src, int size);
void H2B(char des[], char src[], int size);

#endif    /* !CODE_ASSIST_H */
