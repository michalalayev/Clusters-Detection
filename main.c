#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "spmat.h"
#include "group.h"
#include "stack.h"
#include "parser.h"
#include "alg_imp.h"
#include "errors.h"

/*
int main (int argc, char* argv[])
{
	char *input, *output;
	spmat *A, *Ag;
	int *ranks, *g_ranks, *A_row, *s, *tmp; //A_row is also for g_vec
	double *f, *b_curr, *b_next, *result, eigen_val, deltaQ;
	int n, M, first_partition, ng;
	ArrayMat *arr_mat;
	group *initial_g, *g, **splited_g;
	stack *P, *O;

	srand(time(NULL));
	argc += 0;
	input = argv[1];
	A = create_A(input);
	ranks = get_ranks();
	n = A->n;
	arr_mat = (ArrayMat*) A->private;
	M = arr_mat->rowptr[n];
	check_M(M);
	g_ranks = (int*) malloc(sizeof(int)*n);
	check_alloc(g_ranks);
	A_row = (int*) malloc(sizeof(int)*n);
	check_alloc(A_row);
	s = (int*) malloc(sizeof(int)*n);
	check_alloc(s);

	f = (double*) malloc(sizeof(double)*(n+2));
	check_alloc(f);
	b_curr = (double*) malloc(sizeof(double)*n);
	check_alloc(b_curr);
	b_next = (double*) malloc(sizeof(double)*n);
	check_alloc(b_next);
	result = (double*) malloc(sizeof(double)*n);
	check_alloc(result);

	O = initialize_stack(); //O is an empty stack
	P = initialize_stack();
	initial_g = initial_group(n);
	push(initial_g, P); //P contains the initial group with n nodes
	first_partition = 1;

	while (!empty(P))
	{
		g = pop(P);
		calc_f_1norm_and_nnz(A, A_row, g, ranks, M, f);
		ng = g->len;
		create_random_vector(b_curr, ng);
		if (first_partition) {
			Ag = A;
			tmp = g_ranks;
			g_ranks = ranks;
		}
		else {
			Ag = create_Ag(A, g, f[ng+1], A_row);
			fill_g_ranks(g, ranks, g_ranks);
		}
		power_iteration(Ag, result, M, g_ranks, f, b_curr, b_next); //after this b_curr contains the leading eigenvector
		eigen_val = calc_leading_eigenvalue(Ag, result, M, g_ranks, f, b_curr, b_next); //eigen_val is the leading eigenvalue
		if (!IS_POSITIVE(eigen_val)) {
			fill_with_ones(s, ng);
		}
		else {
			create_s(s, b_curr, ng);
			deltaQ = calc_deltaQ(Ag, int* result_int, s, g_ranks, M, f);
			if (!IS_POSITIVE(deltaQ)) {
				fill_with_ones(s, ng);
			}
		}
		modularity_maximization(s, int* unmoved, int* indices, g_ranks, Ag, M, int* row);

		if (!first_partition) {
			Ag->free(Ag);
		}
		else {
			g_ranks = tmp;
			first_partition = 0;
		}

		splited_g = split_group(s, g);
		put_groups_in_stacks(splited_g, P, O);
	}

	output = argv[2];
	output_groups(output, O, int* arr);
	free(P);
	free(O);

	return 0;
}

*/

