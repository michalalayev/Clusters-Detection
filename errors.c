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

void check_fwrite(int written, int exp) {
	if (written != exp) {
		fprintf(stderr,"The number of elements written by fwrite is incorrect\n");
		exit(1);
	}
}

void check_fopen(FILE* input) {
	if (input == NULL) {
		fprintf(stderr,"Couldn't open file, No such file or directory\n");
		exit(1);
	}
}

void check_M(int M) {
	if (M == 0) {
		fprintf(stderr,"M equals 0, matrix B is not defined\n");
		exit(1);
	}
}
