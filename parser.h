#ifndef PARSER_H_
#define PARSER_H_

#include "spmat.h"
#include "stack.h"

spmat* create_A(char* filename);
int* get_ranks();
void output_groups(char* filename, stack* stk, int* arr);


#endif /* PARSER_H_ */
