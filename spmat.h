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

	/* Adds row i to the matrix A, when rank is the number of non-zero elements in the row,
	 * and nodes are the indices of them in the row. */
	void	(*add_row)(struct _spmat *A, const int *nodes, int rank, int i);

	/* Frees all resources used by A */
	void	(*free)(struct _spmat *A);

	/* Multiplies matrix A by vector v of ints, into result (result is pre-allocated) */
	void	(*mult_int)(const struct _spmat *A, const int *v, int *result);

	/* Multiplies matrix A by vector v of doubles, into result (result is pre-allocated) */
	void	(*mult_double)(const struct _spmat *A, const double *v, double *result);

	/* Private field for inner implementation.
	 * Should not be read or modified externally */
	void	*private;
} spmat;

/* In all of the functions, the array ranks contains the ranks of all nodes, when ranks[i] is the rank of the node i,
 * and M is the sum of the ranks.*/

/* Allocates a new arrays sparse matrix of size n with nnz non-zero elements */
spmat* spmat_allocate_array(int n, int nnz);

/* Fills with zeroes the pre-allocated array row, of length n */
void reset_row(int* row, int n);

/* Creates a sub-matrix Ag of A, with nnz non-zero elements, according to the nodes in the group g.
 * g_vector is a pre-allocated helping array. */
spmat* create_Ag(spmat* A, group* g, int nnz, int* g_vector);

/* Builds full row of the matrix A from its sparse representation, into the pre-allocated array A_row.
 * row_num is the index of the row in A to build. */
void build_full_row(spmat* A, int* A_row, int row_num);

/* Calculates the sum of every row i in the matrix B[g], using the matrix A, the relevant group g,
 * and the pre-allocated helping array A_row, and saves it into the pre-allocated array f at index i.
 * Suppose B[g] is of size n. The function also calculates the 1-norm of B[g]_hat (saved into f[n]),
 * and the number of non-zero elements in A[g] (saved into f[n+1]) */
void calc_f_1norm_and_nnz(spmat* A, int* A_row, group* g, int* ranks, int M, double* f);

/* Fills the pre-allocated array g_ranks with the ranks of the nodes in the group g */
void fill_g_ranks(group* g, int* ranks, int* g_ranks);

double array_mult_double2(const spmat *A, const double *v, int i);
double array_mult_double3(const double *v, int *colind, int nnz_in_row);

int array_mult_int2(const spmat *A, const int *v, int i);
int array_mult_int3(const int *v, int *colind, int nnz_in_row);

#endif
