#ifndef _SPMAT_H
#define _SPMAT_H


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



/* Allocates a new arrays sparse matrix of size n with nnz non-zero elements */
spmat* spmat_allocate_array(int n, int nnz);


double array_mult_double2(const spmat *A, const double *v, int i);
double array_mult_double3(const double *v, int *colind, int nnz_in_row);

int array_mult_int2(const spmat *A, const int *v, int i);
int array_mult_int3(const int *v, int *colind, int nnz_in_row);

#endif
