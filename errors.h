#ifndef ERRORS_H_
#define ERRORS_H_

#include <stdio.h>

void check_alloc(void* ptr);
void check_fread(int read, int exp);
void check_fwrite(int written, int exp);
void check_fopen(FILE* input);
void check_M(int M);
void size_error(const char* filename);

#endif /* ERRORS_H_ */
