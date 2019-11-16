#ifndef CODE_WORK_H
#define CODE_WORK_H

void read_instr(char str[]);
int open_file(int SE, const char * fname);
int read_mode(char * des);
int read_primaryKey_or_vector(char des[]);
int CBC_encryption();
int CBC_decryption();

#endif  /* !CODE_WORK_H */
