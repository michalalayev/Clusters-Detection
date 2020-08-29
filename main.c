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
/*
int main (int argc, char* argv[])
{
	char* filename;
	spmat* A;
	int *ranks, *g_ranks, *A_row, *s; A_row is also for g_vec
	double *f, *b_curr, *b_next, *result;
	int n, M;
	ArrayMat *arr_mat;

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

}
*/
