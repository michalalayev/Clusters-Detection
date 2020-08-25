/*
 * errors.c
 *
 *  Created on: 23 ???? 2020
 *      Author: Michal
 */
#include <stdio.h>
#include <stdlib.h>

void check_alloc(void* ptr) {
	if (ptr == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(1);
	}
}

void check_fread(int read, int exp) {
	if (read != exp) {
		fprintf(stderr,"The number of elements read by fread is incorrect\n");
		exit(1);
	}
}

void check_fopen(FILE* input) {
	if (input == NULL) {
		fprintf(stderr,"Couldn't open file, No such file or directory\n");
		exit(1);
	}
}

