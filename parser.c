#include "spmat.h"
#include "parser.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>


/*int* create_row(int rank, int* nodes) {
	int* row
}*/

off_t calc_nnz(const char *filename, int n) {

	off_t size, bytes;
	struct stat st;

    if (stat(filename, &st) == 0) {
        size = st.st_size;
        bytes = size/sizeof(int);
        return bytes-n-1;
    }
    printf("Cannot determine size of %s\n", filename);
    return -1;
}


spmat* create_A(char* filename)
{
	int n, k, i, rank, nnz;
	int *nodes;
	FILE* input;
	spmat* A;

	input = fopen(filename, "r");
	/*### check if input != NULL ###*/
	k = fread(&n, sizeof(int), 1, input);
	/*### check if k == 1 ###*/
	k++; /*delete this later*/
	nnz = calc_nnz(filename, n);
	A = spmat_allocate_array(n, nnz);

	for (i = 0; i < n; ++i) {
		k = fread(&rank, sizeof(int), 1, input);
		/*### check if k == 1 ###*/
		nodes = (int*) malloc(sizeof(int) * rank);
		k = fread(nodes, sizeof(int), rank, input);
		/*### check if k == rank ###*/
		A->add_row(A, nodes, rank, i);

	}
	return A;
}

