#include "spmat.h"
#include "parser.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int* ranks_vec;

off_t calc_nnz(const char *filename, int n) {

	off_t size, ints;
	struct stat st;

    if (stat(filename, &st) == 0) {
        size = st.st_size;
        ints = size/sizeof(int);
        return ints-n-1;
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
	ranks_vec = (int*) malloc(sizeof(int)*n);
	/*### check if ranks != NULL ###*/

	for (i = 0; i < n; ++i) {
		k = fread(&rank, sizeof(int), 1, input);
		/*### check if k == 1 ###*/
		*ranks_vec = rank;
		ranks_vec++;
		nodes = (int*) malloc(sizeof(int) * rank);
		k = fread(nodes, sizeof(int), rank, input);
		/*### check if k == rank ###*/
		A->add_row(A, nodes, rank, i);
	}
	ranks_vec -= n;
	return A;
}

int* get_ranks() {
	return ranks_vec;
}

