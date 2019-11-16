#ifndef CODE_DES_H
#define CODE_DES_H

void conduct_subKey(char primaryKey[]);
void DES_encryption(char des[], const char src[]);
void DES_decryption(char des[], const char src[]);

#endif    /* !CODE_DES_H */
