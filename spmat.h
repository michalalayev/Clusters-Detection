#ifndef _SPMAT_H
#define _SPMAT_H

#include "group.h"

typedef struct
{
	int* values;
	int *colind;
	int *rowptr;
} ArrayMat;

typedef struct _spmat {
	/* Matrix size (n*n) */
	int		n;

	/* Adds row i the matrix. Called before any other call,
	 * exactly n times in order (i = 0 to n-1) */
	void	(*add_row)(struct _spmat *A, const int *nodes, int rank, int i);

	/* Frees all resources used by A */
	void	(*free)(struct _spmat *A);

	/* Multiplies matrix A by vector v, into result (result is pre-allocated) */
	void	(*mult)(const struct _spmat *A, const double *v, double *result);

	/* Private field for inner implementation.
	 * Should not be read or modified externally */
	void	*private;
} spmat;

/* Allocates a new linked-lists sparse matrix of size n
spmat* spmat_allocate_list(int n); */

/* Allocates a new arrays sparse matrix of size n with nnz non-zero elements */
spmat* spmat_allocate_array(int n, int nnz);
void reset_row(int* row, int n);
spmat* create_Ag(spmat* A, group* g, int nnz,  int* g_vector);
void build_full_row(spmat* A, int* A_row, int row_num);
void calc_f_1norm_and_nnz(spmat* A, int* A_row, group* g, int* ranks, int M, double* f);
void fill_g_ranks(group* g, int* ranks, int* g_ranks); /*move to other module later*/

#endif
