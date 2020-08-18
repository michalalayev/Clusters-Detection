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


#endif /* PARSER_H_ */
