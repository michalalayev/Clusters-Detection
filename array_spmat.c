#include <stdio.h>
#include <stdlib.h>
#include "spmat.h"
#include "errors.h"


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


void array_mult_double(const spmat *A, const double *v, double *result)
{
	ArrayMat *arr_mat;
	int *rp, *colind, n, i, j, a, b, nnz_in_row;
	double sum;

	arr_mat = (ArrayMat*) A->private;
	rp = arr_mat->rowptr;
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
			sum += v[*colind];
			colind++;
		}
		*result = sum;
		result++;
	}
}

void array_mult_int(const spmat *A, const int *v, int *result)
{
	ArrayMat *arr_mat;
	int *rp, *colind, n, i, j, a, b, nnz_in_row;
	int sum;

	arr_mat = (ArrayMat*) A->private;
	rp = arr_mat->rowptr;
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
			sum += v[*colind];
			colind++;
		}
		*result = sum;
		result++;
	}
}

/*i is the index of row in A that we want to mult by v*/
double array_mult_double2(const spmat *A, const double *v, int i)
{
	ArrayMat *arr_mat;
	int *rp, *colind, j, nnz_in_row;
	double sum;

	arr_mat = (ArrayMat*) A->private;
	rp = arr_mat->rowptr;
	colind = arr_mat->colind;
	colind += rp[i];
	nnz_in_row = rp[i+1]-rp[i];
	sum = 0;
	for (j = 0; j < nnz_in_row; ++j)
	{
		sum += v[*colind];
		colind++;
	}
	return sum;
}

double array_mult_double3(const double *v, int *colind, int nnz_in_row)
{
	int i;
	double sum;

	sum = 0;
	for (i = 0; i < nnz_in_row; ++i)
	{
		sum += v[*colind];
		colind++;
	}
	return sum;
}

/*i is the index of row in A that we want to mult by v*/
int array_mult_int2(const spmat *A, const int *v, int i)
{
	ArrayMat *arr_mat;
	int *rp, *colind, j, nnz_in_row;
	int sum;

	arr_mat = (ArrayMat*) A->private;
	rp = arr_mat->rowptr;
	colind = arr_mat->colind;
	colind += rp[i];
	nnz_in_row = rp[i+1]-rp[i];
	sum = 0;
	for (j = 0; j < nnz_in_row; ++j)
	{
		sum += v[*colind];
		colind++;
	}
	return sum;
}

int array_mult_int3(const int *v, int *colind, int nnz_in_row)
{
	int i, sum;

	sum = 0;
	for (i = 0; i < nnz_in_row; ++i)
	{
		sum += v[*colind];
		colind++;
	}
	return sum;
}


spmat* spmat_allocate_array(int n, int nnz)
{
	spmat *sp;
	ArrayMat *mat;

	sp = (spmat*) malloc(sizeof(spmat));
	check_alloc(sp);

	sp->n = n;
	sp->add_row = array_add_row;
	sp->free = array_free;
	sp->mult_int = array_mult_int;
	sp->mult_double = array_mult_double;

	mat = (ArrayMat*) malloc(sizeof(ArrayMat));
	check_alloc(mat);
	/*if (mat == NULL) {
		free(sp);
		return NULL;
	}*/

	/*mat->values = (int*) malloc(sizeof(int) * nnz);
	if (mat->values == NULL) {
		free(mat);
		free(sp);
		return NULL;  ### return error message ###
	} */
	mat->colind = (int*) malloc(sizeof(int) * nnz);
	check_alloc(mat->colind);
	/*if (mat->colind == NULL) {
		free(mat);
		free(sp);
		return NULL;  ### return error message ###
	}*/
	mat->rowptr = (int*) malloc(sizeof(int) * n);
	check_alloc(mat->rowptr);
	/*if (mat->rowptr == NULL) {
		free(mat->colind);
		free(mat);
		free(sp);
		return NULL;  ### return error message ###
	}*/
	mat->rowptr[0] = 0;

	sp->private = (void*) mat;
	return sp;
}








