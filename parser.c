#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "errors.h"
#include "group.h"

int* ranks_vec; /*pointer to an array that contains the ranks of the nodes in the graph*/
off_t nnz;      /*the number of non-zero elements in the adjacency matrix A*/


/* Calcultes the number of non-zero elements in the matrix A,
 * and saves this value in the variable nnz */
void calc_nnz(const char *filename, int n) {

	off_t size, ints;
	struct stat st;

    if (stat(filename, &st) == 0) {
        size = st.st_size;
        ints = size/sizeof(int);
        nnz = ints-n-1;
    }
    else {
    	size_error(filename);
    }
}

spmat* create_A(char* filename)
{
	int n, k, i, rank;
	int *nodes;
	FILE* input;
	spmat* A;

	input = fopen(filename, "r");
	check_fopen(input);
	k = fread(&n, sizeof(int), 1, input);
	check_fread(k,1);
	calc_nnz(filename, n);
	A = spmat_allocate_array(n, nnz);
	ranks_vec = (int*) malloc(sizeof(int)*n);
	check_alloc(ranks_vec);
	nodes = (int*) malloc(sizeof(int) * n);
	check_alloc(nodes);

	for (i = 0; i < n; ++i) {
		k = fread(&rank, sizeof(int), 1, input);
		check_fread(k,1);
		*ranks_vec = rank;
		ranks_vec++;
		k = fread(nodes, sizeof(int), rank, input);
		check_fread(k,rank);
		A->add_row(A, nodes, rank, i);
	}
	fclose(input);
	free(nodes);
	ranks_vec -= n;
	return A;
}

int* get_ranks() {
	return ranks_vec;
}

off_t get_nnz() {
	return nnz;
}

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
		for (j = 0; j < len; ++j) { /*save the values to the array before writing it to the file*/
			node = head;
			head = head->next;
			*arr = node->data;
			arr++;
			free(node); /*free the memory allocated for the node*/
		}
		free(g); /*free the memory allocated for the group*/
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

