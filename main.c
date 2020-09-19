#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "spmat.h"
#include "group.h"
#include "stack.h"
#include "parser.h"
#include "alg_imp.h"
#include "errors.h"

/*Allocates an array of ints of length len, pointed by *arr_ptr */
void allocate_int(int** arr_ptr, int len)
{
	*arr_ptr = (int*) malloc(sizeof(int)*len);
	check_alloc(*arr_ptr);
}

/*Allocates an array of doubles of length len, pointed by *arr_ptr */
void allocate_double(double** arr_ptr, int len)
{
	*arr_ptr = (double*) malloc(sizeof(double)*len);
	check_alloc(*arr_ptr);
}

void print_output(char* filename) {  /*####### delete before submition #########*/

	FILE* out;
	int i, *arr;

	arr = (int*) malloc(sizeof(int)*24);
	out = fopen(filename,"r");
	fread(arr, sizeof(int), 24, out);
	for (i = 0; i < 24; ++i) {
		printf("%d ",arr[i]);
	}
	free(arr);
}

void create_const_vector(double *b, int len) /*####### delete before submition #########*/
{
	int i;

	for (i = 0; i < len; ++i)
	{
		*b = i;
		b++;
	}
}

/* Running the whole devision algorithm */
int main (int argc, char* argv[])
{
	char *input, *output;
	spmat *A, *Ag, *Ag_tmp;
	int *ranks, *g_ranks, *A_row, *s, *tmp, *res_int, *indices, *unmoved; /*A_row is also for g_vec*/
	double *f, *b_curr, *b_next, *result, eigen_val, deltaQ;
	int n, M, first_partition, ng;
	ArrayMat *arr_mat;
	group *initial_g, *g, **splited_g;
	stack *P, *O;
	off_t nnz;

	clock_t start, end;
	double time_total;

	start = clock();
	srand(time(NULL));

	argc += 0;
	input = argv[1]; /*the name of the binary input file */
	A = create_A(input);
	ranks = get_ranks();
	n = A->n;
	arr_mat = (ArrayMat*) A->private;
	M = arr_mat->rowptr[n]; /* sum of all ranks */
	check_M(M);

	/* allocate needed memory: */
	allocate_int(&g_ranks, n);
	allocate_int(&A_row, n);
	allocate_int(&s, n);
	allocate_int(&res_int, n);
	allocate_int(&unmoved, n);
	allocate_int(&indices, n);
	allocate_double(&f, n+1);
	allocate_double(&b_curr, n);
	allocate_double(&b_next, n);
	allocate_double(&result, n);

	O = initialize_stack(); /*O is an empty stack*/
	P = initialize_stack();
	initial_g = initial_group(n);
	push(initial_g, P); /*P now contains the initial group with n nodes*/
	nnz = get_nnz();
	Ag = spmat_allocate_array(n, nnz); /*allocate memory for sparse matrix Ag of same size as A, to reuse */
	first_partition = 1;

	while (!empty(P))
	{
		g = pop(P);
		calc_f_and_1norm(A, A_row, g, ranks, M, f);
		ng = g->len;
		create_random_vector(b_curr, ng);
		/*create_const_vector(b_curr, ng);*/ /*############# delete this ##############3*/
		if (first_partition) { /*we want to work with the whole A and ranks*/
			Ag_tmp = Ag;
			Ag = A;
			tmp = g_ranks;
			g_ranks = ranks;
		}
		else { /*update Ag and g_ranks according to the group g*/
			Ag->n = ng;
			create_Ag(A, Ag, g, A_row); /*reuse of the allocated memory pointed by A_row as helping array*/
			fill_g_ranks(g, ranks, g_ranks);
		}

		power_iteration(Ag, result, M, g_ranks, f, b_curr, b_next); /*after this b_curr contains the leading eigenvector*/
		eigen_val = calc_leading_eigenvalue(Ag, result, M, g_ranks, f, b_curr, b_next); /*eigen_val is the leading eigenvalue*/
		if (!IS_POSITIVE(eigen_val)) {
			fill_with_ones(s, ng); /*trivial partition*/
		}
		else {
			create_s(s, b_curr, ng);
			deltaQ = calc_deltaQ(Ag, res_int, s, g_ranks, M, f);
			if (!IS_POSITIVE(deltaQ)) {
				fill_with_ones(s, ng); /*trivial partition*/
			}
		}
		modularity_maximization(s, unmoved, indices, result, g_ranks, Ag, M, A_row, res_int);
		/*reuse of the allocated memory pointed by result for the score array,
		 * and A_row and res_int as helping arrays*/
		if (first_partition) {
			g_ranks = tmp;
			Ag = Ag_tmp;
			first_partition = 0; /*from now on we use Ag and g_ranks*/
		}
		splited_g = split_group(s, g);
		put_groups_in_stacks(splited_g, P, O);
	}
	/*free all allocated memory:*/
	A->free(A);
	Ag->free(Ag);
	free(ranks);
	free(g_ranks);
	free(A_row);
	free(s);
	free(res_int);
	free(indices);
	free(result);
	free(f);
	free(b_curr);
	free(b_next);

	output = argv[2]; /*the name of the binary output file*/
	output_groups(output, O, unmoved); /*reuse of the allocated memory pointed by unmoved as helping array*/

	free(unmoved);
	free(P);
	free(O);
	/*print_output(output);*/

	end = clock();
	time_total = (double)(end-start) / CLOCKS_PER_SEC;
	printf("\n\nExecution took %f seconds\n", time_total);

	return 0;
}




