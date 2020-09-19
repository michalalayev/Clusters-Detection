#include <stdlib.h>
#include <math.h>
#include "errors.h"

#define epsilon 0.00001

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

void size_error(const char* filename) {
	fprintf(stderr, "Cannot determine size of %s\n", filename);
	exit(1);
}

void check_devision_by_zero(double val)
{
	if (fabs(val) <= epsilon) {
		fprintf(stderr, "Math error, devision by zero\n");
		exit(1);
	}
}

void infinite_loop_error()
{
	fprintf(stderr, "Infinite loop in power iteration\n");
	exit(1);
}
