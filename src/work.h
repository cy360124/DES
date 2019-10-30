#ifndef CODE_WORK_H
#define CODE_WORK_H

int open_file(int SE, char fname[]);
int read_mode(char * des);
int read_primaryKey_or_vector(char des[]);
int encryption_CBC();
int decryption_CBC();


#endif  /* !CODE_WORK_H */