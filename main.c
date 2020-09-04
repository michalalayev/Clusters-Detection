/*
 * main.c
 *
 *  Created on: 18 ???? 2020
 *      Author: Michal
 */
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "spmat.h"
#include "errors.h"
#include "group.h"
#include "stack.h"
#include "mult.h"


int main (int argc, char* argv[])
{
	char* filename;
	spmat *A, *Ag;
	int *ranks, *g_ranks, *A_row, *s; //A_row is also for g_vec
	double *f, *b_curr, *b_next, *result, eigen_val;
	int n, M, first_partition, ng;
	ArrayMat *arr_mat;
	group *initial_g, *g;
	stack *P, *O;

	argc += 0;
	filename = argv[1];
	A = create_A(filename);
	ranks = get_ranks();
	n = A->n;
	arr_mat = (ArrayMat*) A->private;
	M = arr_mat->rowptr[n];
	g_ranks = (int*) malloc(sizeof(int)*n);
	check_alloc(g_ranks);
	A_row = (int*) calloc(n,sizeof(int));
	check_alloc(A_row);
	s = (int*) malloc(sizeof(int)*n);
	check_alloc(s);

	f = (double*) malloc(sizeof(double)*(n+2));
	check_alloc(f);
	b_curr = (double*) malloc(sizeof(double)*n);
	check_alloc(b_curr);
	b_next = (double*) malloc(sizeof(double)*n);
	check_alloc(b_next);
	result = (double*) malloc(sizeof(double) * n);
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
		//create initial b_curr here//
		if (first_partition) {
			first_partition = 0;
			power_iteration(A, result, M, ranks, f, b_curr, b_next); //after this b_curr contains the leading eigenvector
			eigen_val = calc_leading_eigenvalue(A, result, M, ranks, f, b_curr, b_next); //eigen_val is the leading eigenvalue
		}
		else {
			ng = g->len;
			Ag = create_Ag(A, g, f[ng+1], A_row);
			fill_g_ranks(g, ranks, g_ranks);
			power_iteration(Ag, result, M, g_ranks, f, b_curr, b_next);
			eigen_val = calc_leading_eigenvalue(Ag, result, M, g_ranks, f, b_curr, b_next);
		}

	}


	return 0;
}

