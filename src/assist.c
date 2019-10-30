#include <stdio.h>

// wait for instructions
void
wait()
{
    printf("(DES)--> "); 
}

// printf some message at the begin
void 
welcome()
{
    printf("(DES)--> Welcome to my DES!\n");
    printf("(DES)--> Note: it works in the mode of CBC.\n");
    printf("(DES)--> It means that you should input both primaryKey and initial vector.\n");
    printf("(DES)--> If you are unfammiliar with it, please enter \"help\" to get something.\n");
}

// printf the introduction about instructions of DES
void
help()
{
    printf("(DES)--> (1) enter \"help\" to get this message;\n");
    printf("(DES)--> (1) enter \"prepare\" to do some prepare before running;\n");
    printf("(DES)--> (3) enter \"run\" to run it whenever you want to produce plaintext or cipher;\n");
    printf("(DES)--> (4) enter \"quit\" to exit.\n");
}

// read the instruction 
void
read_instr(char str[], int size)
{
    scanf("%s", str);      // get instr
    str[size - 1] = '\0';  // make sure a string
    fflush(stdin);         // clear the input buffer
}

// printf message for the selection of mode
void
printf_mode()
{
    printf("(DES)--> Please select the mode of DES:\n");
    printf("(DES)--> 1) enter \"E\" for encryption; 2) enter \"D\" for decryption.\n");
}

// printf message for primaryKey
void 
printf_primaryKey()
{
    printf("(DES)--> Please input the primaryKey in hexadecimal.\n");
    printf("(DES)--> Note: It must be at the length of 16 chars.\n");
    printf("(DES)--> For example, 0102030405060708\n");
}

// printf message for vector
void 
printf_vector()
{
    printf("(DES)--> Please input the initial vector in hexadecimal.\n");
    printf("(DES)--> Note: It must be at the length of 16 chars.\n");
    printf("(DES)--> For example, 0102030405060708\n");
}

// copy the char src[] to the char des[]
// size is the length
void
copy(char des[], char src[], int size)
{
    for (int i = 0; i < size; i++) des[i] = src[i];
}

// achieve the xor between src[] and des[]
// size is the length
void 
my_xor(char des[], char src1[], char src2[], int size)
{
    for (int i = 0; i < size; i++)
        des[i] = (src1[i] == src2[i]) ? '0' : '1';
}

// turn char[] into integer
// size is the length of src[]
int
Char2Int(char src[], int size)
{
    int value = 0;
    int temp = 0;
    for (int i = 0; i < size; i++)
    {
        temp = src[i] - '0';
        value = value * 2 + temp;
    }
    return value;
}

// turn integer into char[]
// src should be in [0, 16)，des[] do not contains '\0'
// size is the length of char[]
void
Int2Char(char des[], int src, int size)
{
    // 短除法变换
    int index = size - 1;    // 下标
    int rem;                 // 余数
    while(index >= 0)
    {
        rem = src % 2;
        src = src / 2;
        des[index--] = '0' + rem;
    }
}

// trun the src[] in hexadecimal into des[] in binary
// the size is the length of src[]
void 
H2B(char des[], char src[], int size)
{
    char c;

    for (int i = 0; i < size; i++)
    {
        c = src[i];
        switch (c)
        {
        case '0': des[i * 4 + 0] = '0'; des[i * 4 + 1] = '0'; des[i * 4 + 2] = '0'; des[i * 4 + 3] = '0'; break;
        case '1': des[i * 4 + 0] = '0'; des[i * 4 + 1] = '0'; des[i * 4 + 2] = '0'; des[i * 4 + 3] = '1'; break;
        case '2': des[i * 4 + 0] = '0'; des[i * 4 + 1] = '0'; des[i * 4 + 2] = '1'; des[i * 4 + 3] = '0'; break;
        case '3': des[i * 4 + 0] = '0'; des[i * 4 + 1] = '0'; des[i * 4 + 2] = '1'; des[i * 4 + 3] = '1'; break;
        case '4': des[i * 4 + 0] = '0'; des[i * 4 + 1] = '1'; des[i * 4 + 2] = '0'; des[i * 4 + 3] = '0'; break;
        case '5': des[i * 4 + 0] = '0'; des[i * 4 + 1] = '1'; des[i * 4 + 2] = '0'; des[i * 4 + 3] = '1'; break;
        case '6': des[i * 4 + 0] = '0'; des[i * 4 + 1] = '1'; des[i * 4 + 2] = '1'; des[i * 4 + 3] = '0'; break;
        case '7': des[i * 4 + 0] = '0'; des[i * 4 + 1] = '1'; des[i * 4 + 2] = '1'; des[i * 4 + 3] = '1'; break;
        case '8': des[i * 4 + 0] = '1'; des[i * 4 + 1] = '0'; des[i * 4 + 2] = '0'; des[i * 4 + 3] = '0'; break;
        case '9': des[i * 4 + 0] = '1'; des[i * 4 + 1] = '0'; des[i * 4 + 2] = '0'; des[i * 4 + 3] = '1'; break;
        case 'a': 
        case 'A': des[i * 4 + 0] = '1'; des[i * 4 + 1] = '0'; des[i * 4 + 2] = '1'; des[i * 4 + 3] = '0'; break;
        case 'b': 
        case 'B': des[i * 4 + 0] = '1'; des[i * 4 + 1] = '0'; des[i * 4 + 2] = '1'; des[i * 4 + 3] = '1'; break;
        case 'c': 
        case 'C': des[i * 4 + 0] = '1'; des[i * 4 + 1] = '1'; des[i * 4 + 2] = '0'; des[i * 4 + 3] = '0'; break;
        case 'd': 
        case 'D': des[i * 4 + 0] = '1'; des[i * 4 + 1] = '1'; des[i * 4 + 2] = '0'; des[i * 4 + 3] = '1'; break;
        case 'e': 
        case 'E': des[i * 4 + 0] = '1'; des[i * 4 + 1] = '1'; des[i * 4 + 2] = '1'; des[i * 4 + 3] = '0'; break;
        case 'f': 
        case 'F': des[i * 4 + 0] = '1'; des[i * 4 + 1] = '1'; des[i * 4 + 2] = '1'; des[i * 4 + 3] = '1'; break;
        default:  break;  
        }
    }
}