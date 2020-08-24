/*
 * errors.c
 *
 *  Created on: 23 ???? 2020
 *      Author: Michal
 */
#include <stdio.h>

void check_alloc(void* ptr) {
	if (ptr == NULL) {
		printf("error\n"); /*print the relevent error message, free all memory allocations, exit*/

	}
}

