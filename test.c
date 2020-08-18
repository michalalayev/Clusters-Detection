#include "parser.h"
#include <stdio.h>
#include <assert.h>
#include "spmat.h"

void check_nnz(const char* filename, int n) {
	int test,res;

	test = calc_nnz(filename, n);
	res = 82;
	if (test == res) {
		printf("well done");
	}
	else {
		printf("test is: %d",test);
	}
}

void print_A(char* filename, int n){
	spmat* A;
	ArrayMat *arr_mat;
	int *rp, *colind, *vals, i;

	A = create_A(filename);
	arr_mat = (ArrayMat*) A->private;
	rp = arr_mat->rowptr;
	vals = arr_mat->values;
	colind = arr_mat->colind;
	for (i = 0; i < n+1; ++i) {
		printf("val = %d, col = %d, rp = %d\n", vals[i],colind[i],rp[i+1]);
	}
}


int main (int argc, char* argv[]) {
	char* filename;
	FILE* input;
	int n;

	filename = argv[1];
	input = fopen(filename, "r");
	assert(input != NULL);
	fread(&n, sizeof(int), 1, input);
	/*check_nnz(filename, n);*/
	print_A(filename, n);
}


