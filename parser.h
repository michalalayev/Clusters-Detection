#ifndef PARSER_H_
#define PARSER_H_

#include "spmat.h"
#include "stack.h"
#include <sys/types.h>

void calc_nnz(const char *filename, int n);
spmat* create_A(char* filename);
int* get_ranks();
off_t get_nnz();
void output_groups(char* filename, stack* stk, int* arr);


#endif /* PARSER_H_ */
