/*
 * parser.h
 *
 *  Created on: 18 ???? 2020
 *      Author: Michal
 */

#ifndef PARSER_H_
#define PARSER_H_

#include "spmat.h"
#include <sys/types.h>

off_t calc_nnz(const char *filename, int n);

spmat* create_A(char* filename);
int* get_ranks();


#endif /* PARSER_H_ */
