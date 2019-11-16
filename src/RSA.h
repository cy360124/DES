#ifndef CODE_RSA_H
#define CODE_RSA_H

#include "Bignum.h"

void load_RSA();
void RSA_encryption(char des[], const char src[]);
void RSA_decryption(char des[], const char src[]);

#endif /* !CODE_RSA_H */

