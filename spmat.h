/**
 * spmat summary:
 *
 * Implementaion of a sparse matrix with arrays. Used to represent a matrix in which
 * most of the elements are zero, in an economic memory usage manner.
 *
 * spmat_allocate_array - Allocates a new arrays sparse matrix.
 *
 */

#ifndef _SPMAT_H
#define _SPMAT_H

/* Struct for array implementation of sparse matrix */
typedef struct
{
	int *colind; /*the column index for each non-zero value in the original matrix.*/
	int *rowptr; /*for each row keeps the index of the first non-zero value from that row onwards.*/
} ArrayMat;


/* Struct for sparse matrix */
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

	/* Private field for inner implementation */
	void	*private;
} spmat;


/* Allocates a new arrays sparse matrix of size n with nnz non-zero elements,
 * and returns a pointer to it. */
spmat* spmat_allocate_array(int n, int nnz);


#endif
