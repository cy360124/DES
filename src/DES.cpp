#include "global.h"
#include "matrix.h"
#include "assist.h"

// achieve the cyclic left shift of a char[]
// des: the char[]; size: the size of des[]; num: the shift bits
void
left_shift(char des[], int size, int num)
{
    char * temp = (char *)malloc(sizeof(char) * size);

    copy(temp, des, size);                                         // copy the des[]
    for(int i = 0; i < size; i++) des[i] = temp[(i + num) % size]; // left shift
    free(temp);                                                    // free temp[]
}

// achieve the substitution
// des: the target char[]; size1: the size of des[]; 
// src: the source char[]; size2: the size of src[];
// martix: the substitution martix
void 
substitution(char des[], int size1, char src[], int size2, const int martix[])
{
    char * temp = (char *)malloc(sizeof(char) * size2);  // the copy of src
    
    // if des and src point to the same char[]
    if (des == src) {
        copy(temp, src, size2);                                   // copy the src char[]
        for(int i = 0; i < size1; i++) des[i] = temp[martix[i]];  // substitute
    }
    // else
    else {
        for(int i = 0; i < size1; i++) des[i] = src[martix[i]];   // just substitute   
    }
    
    // free temp[]
    free(temp);
}

// conduct eight sub-keys
void
conduct_subKey(char (*subKey)[48], char primaryKey[])
{
    char temp[56];
    char C[28], D[28];

    // IPC substitution
    substitution(temp, 56, primaryKey, 64, IPC);
    for(int i = 0; i < 28; i++) C[i] = temp[i];
    for(int i = 0; i < 28; i++) D[i] = temp[i+28];
    
    // eight rounds
    for (int i  = 0; i < ROUND_TIMES; i++) {
        // left shift
        left_shift(C, 28, LS[i]);
        left_shift(D, 28, LS[i]);
        // copy to the temp[]        
        for(int i = 0; i < 28; i++) temp[i] = C[i];   
        for(int i = 28; i < 56; i++) temp[i] = D[i - 28];
        // PC substitution
        substitution(subKey[i], 48, temp, 56, PC);
    }
    
}

// achieve the function F()
// Note: result, R, K are all char[]
void
F(char result[], char R[], char K[])
{
    char temp[48];                         // store the middle char[]
    char all_out[32];                      // the output of 8 S boxes
    char out[4];                           // the output of a box
    char row_str[8][2], col_str[8][4];     // row, col coordinates in char[]
    int row, col;                          // row and col in integer

    // 选择扩展运算：将32位的R[]扩展为48位
    substitution(temp, 48, R, 32, E);

    // 与子密钥进行异或操作
    my_xor(temp, temp, K, 48);

    // 分为8组，产生行列下标
    for (int i = 0; i < 8; i++)
    {
        // conduct row_str
        row_str[i][0] = temp[i * 6];
        row_str[i][1] = temp[i * 6 + 5];
        // conduct col_str
        col_str[i][0] = temp[i * 6 + 1];
        col_str[i][1] = temp[i * 6 + 2];
        col_str[i][2] = temp[i * 6 + 3];
        col_str[i][3] = temp[i * 6 + 4];
    }

    // S盒变换
    for (int i = 0; i < 8; i++)
    {
        row = Char2Int(row_str[i], 2);
        col = Char2Int(col_str[i], 4);
        // 构造单个S盒的输出
        Int2Char(out, S[i][row][col], 4);
        // 将输出复制到all_out[]中
        for(int j = 0; j < 4; j++) all_out[i * 4 + j] = out[j];
    }

    // 结束时的置换P
    substitution(result, 32, all_out, 32, P);
}

// achieve the encryption of DES
// des[] is the destination char[], src[] is the source char[]
void 
encryption(char des[], char src[])
{
    char temp[64];
    char ptemp[32];
    char last_L[32], last_R[32];
    char now_L[32], now_R[32];

    // IP下标置换变换
    substitution(temp, 64, src, 64, IP);
    
    // initialize last_L and last_R
    for (int i = 0; i < 32; i++) last_L[i] = temp[i];
    for (int i = 0; i < 32; i++) last_R[i] = temp[i + 32];

    // 8次迭代
    for (int i = 0; i < ROUND_TIMES; i++) {
        // now_L = last_R
        copy(now_L, last_R, 32);
        // ptemp = F(last_R, subKey)
        F(ptemp, last_R, subKey[i]);
        // now_R = [ last_L + ptemp ] mod 2, equal to xor
        my_xor(now_R, last_L, ptemp, 32);
        // update
        copy(last_L, now_L, 32);
        copy(last_R, now_R, 32);
    }

    // connect together
    for (int i = 0; i < 32; i++) temp[i] = now_L[i];
    for (int i = 32; i < 64; i++) temp[i] = now_R[i - 32];

    // IP-1 substitution
    substitution(des, 64, temp, 64, IIP);
}

// achieve the decryption of DES
void
decryption(char des[], char src[])
{
    char temp[64];
    char ptemp[32];
    char last_L[32], last_R[32];
    char now_L[32], now_R[32];

    // IP下标置换变换
    substitution(temp, 64, src, 64, IP);
    
    // initialize L and R
    for (int i = 0; i < 32; i++) last_L[i] = temp[i];
    for (int i = 0; i < 32; i++) last_R[i] = temp[i + 32];

    // 8次迭代
    for (int i = 0; i < ROUND_TIMES; i++) {
        // now_R = last_L
        copy(now_R, last_L, 32);
        // ptemp = F(last_l, subKey)
        F(ptemp, last_L, subKey[ROUND_TIMES - 1 - i]);
        // now_L = [ last_R + ptemp ] mod 2, equal to xor
        my_xor(now_L, last_R, ptemp, 32);
        // update
        copy(last_L, now_L, 32);
        copy(last_R, now_R, 32);
    }

    // connect together
    for (int i = 0; i < 32; i++) temp[i] = now_L[i];
    for (int i = 32; i < 64; i++) temp[i] = now_R[i - 32];

    // IP-1 substitution
    substitution(des, 64, temp, 64, IIP);
}
