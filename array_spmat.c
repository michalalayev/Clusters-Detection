#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "spmat.h"
#include "errors.h"
#include <math.h>
#include <string.h>

/*typedef struct
{
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

void reset_row(int* row, int n) {

	memset(row, 0, sizeof(int)*n);
}

spmat* create_Ag(spmat* A, group* g, int nnz, int* g_vector)
{
	spmat* Ag;
	int len, data, start, range, end, i, val, cnt;
	ELEM* node;
	ArrayMat *mat, *g_mat;
	int *rp, *colind, *g_rp, *g_colind;

	len = g->len;
	node = g->head;
	mat = (ArrayMat*) A->private;
	rp = mat->rowptr;
	colind = mat->colind;
	reset_row(g_vector, A->n);
	g_to_vector(g, g_vector);
	Ag = spmat_allocate_array(len, nnz);
	g_mat = (ArrayMat*) Ag->private;
	g_rp = g_mat->rowptr;
	g_rp++;
	g_colind = g_mat->colind;
	cnt = 0;

	for ( ; node != NULL; node = node->next){
		data = node->data;
		start = rp[data];
		range = rp[data+1] - start;
		end = start + range;
		for (i = start; i < end; ++i) {
			val = g_vector[colind[i]];
			if (val != 0) {
				if (val == -1) {
					*g_colind = 0;
				}
				else {
					*g_colind = val;
				}
				cnt++;
				g_colind++;
			}
		}
		*g_rp = cnt;
		g_rp++;
	}
	/*reset_row(g_vector, A->n);*/
	return Ag;
}

/*function for A*/
void build_full_row(spmat* A, int* A_row, int row_num)
{
	int start, range, i;
	ArrayMat *mat;
	int *rp, *colind;

	reset_row(A_row, A->n);
	mat = (ArrayMat*) A->private;
	rp = mat->rowptr;
	colind = mat->colind;
	start = rp[row_num];
	colind += start;
	range = rp[row_num+1] - start;
	for (i = 0; i < range; ++i) {
		A_row[*colind] = 1;
		colind++;
	}
}

/*f is a vector of len n+2, f[ng] = 1norm, f[ng+1] = nnz*/
void calc_f_1norm_and_nnz(spmat* A, int* A_row, group* g, int* ranks, int M, double* f)
{
	int nnz, g_row, g_col, add;
	double sumf, sum_norm, diag, max;
	ELEM *ptr_row, *ptr_col;

	nnz = 0;
	ptr_row = g->head;

	for (; ptr_row != NULL; ptr_row = ptr_row->next) {
		sumf = 0;
		sum_norm = 0;
		g_row = ptr_row->data;
		build_full_row(A, A_row, g_row);
		ptr_col = g->head;
		for (; ptr_col != NULL; ptr_col = ptr_col->next) {
			g_col = ptr_col->data;
			if (A_row[g_col] == 1) {
				nnz++;
				add = M - ranks[g_row]*ranks[g_col];
			}
			else {
				add = -(ranks[g_row]*ranks[g_col]);
			}
			sumf += add;
			if (g_row != g_col) {
				sum_norm += abs(add);
			}
		}
		*f = sumf/M;
		diag = (double) (-ranks[g_row]*ranks[g_row])/M - (*f);
		sum_norm = sum_norm/M + fabs(diag);
		f++;
		if (ptr_row == g->head) {
			max = sum_norm;
		}
		else {
			if (sum_norm > max) {
				max = sum_norm;
			}
		}
	}
	*f = max; /*this is 1-norm of B[g]_hat*/
	*(f+1) = nnz; /*this is the number of non-zero elements in A[g]*/
}

/*move to other module later*/
void fill_g_ranks(group* g, int* ranks, int* g_ranks)
{
	ELEM* node;

	node = g->head;
	for (; node != NULL; node = node->next) {
		*g_ranks = ranks[node->data];
		g_ranks++;
	}
}







