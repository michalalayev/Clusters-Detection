#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "spmat.h"


/*typedef struct
{
	int *values;
	int *colind;
	int *rowptr;
} ArrayMat;*/

/*add the nodes who are neighbors of node i to A*/
void array_add_row(spmat *A, const int *nodes, int rank, int i)
{
	int j, insertind;
	ArrayMat *arr_mat;
	int col;

	arr_mat = (ArrayMat*) A -> private;
	insertind = arr_mat -> rowptr[i]; /*from which cell in values array we start filling*/

	for (j = 0; j < rank; ++j) {
		col = *nodes;
		arr_mat -> colind[insertind] = col;
		insertind++;
		nodes++;
	}
	arr_mat -> rowptr[i+1] = insertind; /*the number of non-zero elements in arr_mat currently*/
}

void array_free(spmat *A)
{
	ArrayMat *arr_mat;

	arr_mat = (ArrayMat*) A -> private;
	free(arr_mat -> colind);
	free(arr_mat -> rowptr);
	free(arr_mat);
	free(A);
}


void array_mult(const spmat *A, const double *v, double *result)
{
	ArrayMat *arr_mat;
	int *rp, *colind, *vals, n, i, j, a, b, nnz_in_row;
	double sum;

	arr_mat = (ArrayMat*) A->private;
	rp = arr_mat->rowptr;
	vals = arr_mat->values;
	colind = arr_mat->colind;
	n = A->n;
	for(i = 0; i < n; ++i)
	{
		a = *rp;
		b = *(++rp);
		nnz_in_row = b-a;
		sum = 0;
		for (j = 0; j < nnz_in_row; ++j)
		{
			sum += (*vals) * (v[*colind]);
			vals++;
			colind++;
		}
		*result = sum;
		result++;
	}
}


spmat* spmat_allocate_array(int n, int nnz)
{
	spmat *sp;
	ArrayMat *mat;

	sp = (spmat*) malloc(sizeof(spmat));
	assert(sp != NULL);
	/* ### change the assert ### */

	sp->n = n;
	sp->add_row = array_add_row;
	sp->free = array_free;
	sp->mult = array_mult;

	mat = (ArrayMat*) malloc(sizeof(ArrayMat));
	if (mat == NULL) {
		free(sp);
		return NULL; /*### return error message ###*/
	}

	/*mat->values = (int*) malloc(sizeof(int) * nnz);
	if (mat->values == NULL) {
		free(mat);
		free(sp);
		return NULL;  ### return error message ###
	} */
	mat->colind = (int*) malloc(sizeof(int) * nnz);
	if (mat->colind == NULL) {
		/*free(mat->values);*/
		free(mat);
		free(sp);
		return NULL;  /*### return error message ###*/
	}
	mat->rowptr = (int*) malloc(sizeof(int) * (n+1));
	if (mat->rowptr == NULL) {
		free(mat->colind);
		/*free(mat->values);*/
		free(mat);
		free(sp);
		return NULL;  /*### return error message ###*/
	}
	mat->rowptr[0] = 0;

	sp->private = (void*) mat;
	return sp;
}

