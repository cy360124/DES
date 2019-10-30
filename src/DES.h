#ifndef CODE_DES_H
#define CODE_DES_H   

void conduct_subKey(char subKey[][48], char primaryKey[]);
void encryption(char des[], char src[]);
void decryption(char des[], char src[]);

#endif    /* !CODE_DES_H */
