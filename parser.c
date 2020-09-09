#include "parser.h"
#include "errors.h"
#include "group.h"
#include <sys/stat.h>
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
	check_fopen(input);
	k = fread(&n, sizeof(int), 1, input);
	check_fread(k,1);
	nnz = calc_nnz(filename, n);
	A = spmat_allocate_array(n, nnz);
	ranks_vec = (int*) malloc(sizeof(int)*n);
	check_alloc(ranks_vec);

	for (i = 0; i < n; ++i) {
		k = fread(&rank, sizeof(int), 1, input);
		check_fread(k,1);
		*ranks_vec = rank;
		ranks_vec++;
		nodes = (int*) malloc(sizeof(int) * rank);
		check_alloc(nodes);
		k = fread(nodes, sizeof(int), rank, input);
		check_fread(k,rank);
		A->add_row(A, nodes, rank, i);
		free(nodes);
	}
	fclose(input);
	ranks_vec -= n;
	return A;
}

int* get_ranks() {
	return ranks_vec;
}


/* This function receives a stack of groups, outputs them in ascending order to the output file named filename,
 * and frees the allocated memory of the stack and the groups.
 * arr is an array of length n, the nodes of the groups are saved there before they are written to the file*/
void output_groups(char* filename, stack* stk, int* arr)
{
	FILE* output;
	int num_of_groups, k, i, j, len;
	int *arr_start;
	group* g;
	ELEM *head, *node;

	arr_start = arr;
	output = fopen(filename, "w");
	num_of_groups = stk->cnt;
	k = fwrite(&num_of_groups, sizeof(int), 1, output);
	check_fwrite(k,1);

	for (i = 0; i < num_of_groups; ++i) {
		g = pop(stk);
		len = g->len;
		if (num_of_groups != 1) {
			*arr = len;
			arr++;
		}
		head = g->head;
		for (j = 0; j < len; ++j) {
			node = head;
			head = head->next;
			*arr = node->data;
			arr++;
			free(node);
		}
		free(g);
		arr = arr_start;
		if (num_of_groups != 1) {
			k = fwrite(arr, sizeof(int), len+1, output);
			check_fwrite(k,len+1);
		}
		else {
			k = fwrite(&len, sizeof(int), 1, output);
			check_fwrite(k,1);
			k = fwrite(arr, sizeof(int), len, output);
			check_fwrite(k,len);
		}
	}
	fclose(output);
}

