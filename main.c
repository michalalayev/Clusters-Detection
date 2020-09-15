#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "spmat.h"
#include "group.h"
#include "stack.h"
#include "parser.h"
#include "alg_imp.h"
#include "errors.h"

void allocate_int(int** arr_ptr, int len)
{
	*arr_ptr = (int*) malloc(sizeof(int)*len);
	check_alloc(*arr_ptr);
}

void allocate_double(double** arr_ptr, int len)
{
	*arr_ptr = (double*) malloc(sizeof(double)*len);
	check_alloc(*arr_ptr);
}

void print_output(char* filename) {

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



int main (int argc, char* argv[])
{
	char *input, *output;
	spmat *A, *Ag;
	int *ranks, *g_ranks, *A_row, *s, *tmp, *res_int, *indices, *unmoved; /*A_row is also for g_vec*/
	double *f, *b_curr, *b_next, *result, eigen_val, deltaQ, check_deltaQ;
	int n, M, first_partition, ng/*, i*/;
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

	allocate_int(&g_ranks, n);
	allocate_int(&A_row, n);
	allocate_int(&s, n);
	allocate_int(&res_int, n);
	allocate_int(&unmoved, n);

	allocate_double(&f, n+2);
	allocate_double(&b_curr, n);
	allocate_double(&b_next, n);
	allocate_double(&result, n);

	O = initialize_stack(); /*O is an empty stack*/
	printf("O len: %d\n",O->cnt);
	P = initialize_stack();
	initial_g = initial_group(n);
	push(initial_g, P); /*P now contains the initial group with n nodes*/
	first_partition = 1;
	printf("before while\n");
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
		printf("before PI\n");
		power_iteration(Ag, result, M, g_ranks, f, b_curr, b_next); /*after this b_curr contains the leading eigenvector*/
		printf("after PI\n");
		printf("b_curr: ");
		/*for (i = 0; i < ng; ++i) {
			printf("%f ",b_curr[i]);
		}*/
		printf("\n");
		eigen_val = calc_leading_eigenvalue(Ag, result, M, g_ranks, f, b_curr, b_next); /*eigen_val is the leading eigenvalue*/
		printf("eigen_val= %f\n",eigen_val);
		printf("after eigenval\n");
		if (!IS_POSITIVE(eigen_val)) {
			fill_with_ones(s, ng);
		}
		else {
			create_s(s, b_curr, ng);
			deltaQ = calc_deltaQ(Ag, res_int, s, g_ranks, M, f);
			printf("deltaQ = %f\n",deltaQ);
			if (!IS_POSITIVE(deltaQ)) {
				fill_with_ones(s, ng);
			}
		}
		printf("after deltaQ\n");
		printf("s: ");
		/*for (i = 0; i < ng; ++i) {
			printf("%d ",s[i]);
		}*/
		printf("\n");
		indices = res_int; /*reuse of the allocated memory pointed by res_int for indices*/
		printf("before MM\n");
		modularity_maximization(s, unmoved, indices, g_ranks, Ag, M, A_row); /*reuse of the allocated memory pointed by A_row*/
		printf("after MM\n");
		check_deltaQ = calc_deltaQ(Ag, res_int, s, g_ranks, M, f);
		printf("new deltaQ = %f\n",check_deltaQ);
		if (!first_partition) {
			Ag->free(Ag);
		}
		else {
			g_ranks = tmp;
			first_partition = 0;
		}

		splited_g = split_group(s, g);
		printf("g1 len: %d, g2 len: %d\n",splited_g[0]->len, splited_g[1]->len);
		put_groups_in_stacks(splited_g, P, O);
		printf("end of while iteration\n");
	}

	printf("after while\n");
	A->free(A);
	free(ranks);
	free(g_ranks);
	free(A_row);
	free(s);
	free(res_int);
	free(result);
	free(f);
	free(b_curr);
	free(b_next);
	printf("before output\n");
	output = argv[2];
	output_groups(output, O, unmoved); /*reuse of the allocated memory pointed by unmoved*/
	printf("after output\n");
	free(unmoved);
	free(P);
	free(O);
	print_output(output);
	return 0;
}



