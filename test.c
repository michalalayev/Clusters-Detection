#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "parser.h"
#include "spmat.h"
#include "group.h"
#include "stack.h"
#include "mult.h"

void check_nnz(const char* filename, int n) {
	int test,res;

	test = calc_nnz(filename, n);
	res = 82;
	if (test == res) {
		printf("well done");
	}
	else {
		printf("test is: %d",test);
	}
}

void print_A(spmat* A, int nnz){

	ArrayMat *arr_mat;
	int *rp, *colind, i, n;

	n = A->n;
	arr_mat = (ArrayMat*) A->private;
	rp = arr_mat->rowptr;
	colind = arr_mat->colind;
	printf("colind:\n");
	for (i = 0; i < nnz; ++i) {
		printf("%d ", colind[i]);
	}
	printf("\nrowptr:\n");
	for (i = 0; i < n+1; ++i) {
		printf("%d ", rp[i]);
	}
}

void print_k(int n) {
	int* k;
	int i;

	k = get_ranks();
	for (i = 0; i < n; ++i) {
		printf("k%d = %d ",i,k[i]);
	}
}

void check_g_to_array()
{
	ELEM *node;
	group *g;
	int i;
	int *new_g;
	g = initial_group(5);
	node = g->head;
	node->data = 1;
	node->next->data = 3;
	node->next->next->data = 4;
	node->next->next->next->data = 6;
	node->next->next->next->next->data = 7;
	for(i = 0; i<5; i++)
		{
			printf("%d",node->data);
			node=node->next;
		}
	new_g = (int*) calloc(10,sizeof(int));
	g_to_vector(g,new_g);

	printf("\n");
	for(i = 0; i<10; i++)
	{
		printf("%d",new_g[i]);

	}
	free(new_g);
}

void check_split()
{
	group **pointer;
	/*int cnt1;
	int cnt2;*/
	int i, cnt1, cnt2;
	int* arr;
	group *g2;
	ELEM *currNode2;

	g2 = initial_group(10);
	arr = (int*) malloc(sizeof(int)*10);
	for(i = 0; i < 10; i++)
	{
		arr[i] = -1;
	}
	/*arr[0] = 1;*/
	arr[3] = 1;
	arr[5] = 1;
	arr[7] = 1;
	/*arr[9] = 1;*/
	pointer = split_group(arr,g2);
	cnt1 = pointer[0]->len;
	printf("cnt1 = %d\n",cnt1);
	cnt2 = pointer[1]->len;
	printf("cnt2 = %d\n",cnt2);
	currNode2 = pointer[0]->head;
	printf("list1:\n");
	for(i=0 ; i<cnt1 ; i++)
	{
		printf("%d\n", currNode2->data);
		currNode2 = currNode2->next;
	}

	currNode2 = pointer[1]->head;
	printf("list2:\n");
	for(i=0 ; i<cnt2 ; i++)
	{
		printf("%d\n", currNode2->data);
		currNode2=currNode2->next;
	}
	free(arr);
}
void check_create_group_by_array()
{
	int* arr;
	int i;
	group *g;
	ELEM *node;
	arr = (int*) malloc(sizeof(int)*10);
	for(i = 0; i < 10; i++)
	{
		arr[i] = i+3;
	}
	arr[4] = 17;
	g = create_group_by_array(arr, 10);
	node = g->head;
	for(i = 0; i < 10; i++)
	{
		printf("%d\n", node->data);
		node = node->next;
	}
	free(arr);
}

spmat* check_create_Ag(char* filename) {
	spmat *A, *Ag;
	group* g;
	ELEM* node;
	ArrayMat *g_mat, *A_mat;
	int *g_rp, *g_colind, i, *A_rp, *g_vector;

	g = initial_group(4);
	node = g->head;
	node->data = 3;
	node->next->data = 7;
	node->next->next->data = 8;
	node->next->next->next->data = 12;
	printf("g:\n");
	for(i = 0; i < 4; ++i )
	{
		printf("%d ",node->data);
		node = node->next;
	}
	printf("\n");
	A = create_A(filename);
	A_mat = (ArrayMat*) A->private;
	A_rp = A_mat->rowptr;
	for (i = 0; i < 4; ++i) {
		printf("%d ",A_rp[i]);
	}
	printf("\n");

	g_vector = (int*) calloc(20,sizeof(int));
	assert(g_vector != NULL);
	Ag = create_Ag(A, g, 4, g_vector);

	g_mat = (ArrayMat*) Ag->private;
	g_rp = g_mat->rowptr;
	g_colind = g_mat->colind;
	printf("colind:\n");
	for (i = 0; i < 4; ++i) {
		printf("%d ",g_colind[i]);
	}
	printf("\nrowptr:\n");
	for (i = 0; i < 5; ++i) {
		printf("%d ",g_rp[i]);
	}
	free(g_vector);
	return Ag;
}

void check_build_full_row(char* filename) {

	spmat *A;
	int* A_row;
	int i, j;

	A = create_A(filename);
	A_row = (int*) calloc(A->n, sizeof(int));

	for (j = 0; j < A->n; ++j) {
		printf("j = %d  ",j);
		build_full_row(A, A_row, j);
		for (i = 0; i < A->n; ++i) {
			printf("%d ", A_row[i]);
		}
		printf("\n");
	}
	free(A_row);

}

void check_calc_f_1norm_and_nnz(char* filename)
{
	spmat *A;
	int *g_array, nnz;
	ArrayMat *private_A;
	int  *A_row, *ranks_vector, m , i, size;
	group *g;
	double *f;

	size = 5;
	g_array = (int*) malloc(sizeof(int)*size);
	f = (double*) malloc(sizeof(double)*(size+2));
	A = create_A(filename);
	A_row = (int*) malloc(sizeof(int)*A->n);
	g_array[0] = 1;
	g_array[1] = 2;
	g_array[2] = 4;
	g_array[3] = 11;
	g_array[4] = 12;
	g = create_group_by_array(g_array, size);
	ranks_vector = get_ranks();
	private_A = A->private;
	m = private_A->rowptr[A->n];
	printf("M = %d\n", m);
	calc_f_1norm_and_nnz( A, A_row, g, ranks_vector, m, f);
	for (i = 0; i < size; ++i)
	{
		printf("%f ", f[i]);
	}
	printf("\n ");
	printf("%f ",f[size]);
	nnz = f[size+1];
	printf("%d ",nnz);

	free(g_array);
	free(f);
}

void check_stack()
{
	group *g,*g2,*g3;
	stack* st;
	int *arr, *arr2;
	st = (stack*)malloc(sizeof(stack));
	initialize(st);
	printf("%d ",st->cnt);
	printf("\n ");
	arr = (int*) malloc(sizeof(int)*3);;
	arr[0] = 1;
	arr[2] = 2;
	arr[3] = 3;
	g = create_group_by_array(arr, 3);
	push(g,st);
	printf("%d ",st->top->g->len);
	printf("\n ");
	printf("%d ",st->top->g->head->data);
	printf("\n ");
	arr2 = (int*) malloc(sizeof(int)*3);;
	arr2[0] = 4;
	arr2[2] = 5;
	arr2[3] = 6;
	g2 = create_group_by_array(arr2, 3);
	push(g2,st);
	printf("%d ",st->top->g->len);
	printf("\n ");
	printf("%d ",st->top->g->head->data);
	printf("\n ");
	printf("%d ",st->cnt);
	printf("\n ");
	printf("%d ",st->top->next->g->head->data);
	printf("\n ");
	g3 = pop(st);
	printf("%d ",g3->head->data);
	printf("\n ");
	printf("%d ",st->cnt);
	printf("\n ");
	printf("%d ",empty(st));
}

void check_power_iteration(char* filename) {

	spmat* A;
	int *ranks, *A_row;
	ArrayMat *arr_mat;
	int i, n, M;
	double *f, *v, *result, *v_next, eval;
	group* g;

	A = create_A(filename);
	print_A(A,2);
	ranks = get_ranks();
	n = A->n;
	arr_mat = (ArrayMat*) A->private;
	M = arr_mat->rowptr[n];
	f = (double*) malloc(sizeof(double)*(n+2));
	A_row = (int*) malloc(sizeof(int)*n);
	v = (double*) malloc(sizeof(double)*n);
	v_next = (double*) malloc(sizeof(double)*n);
	result = (double*) malloc(sizeof(double)*n);
	for (i = 0; i < n; ++i) {
		v[i] = i+3;
	}
	g = initial_group(n);
	calc_f_1norm_and_nnz(A, A_row, g, ranks, M, f);
	printf("f:\n");
	for (i = 0; i < n+2; ++i) {
		printf("%f ",f[i]);
	}
	printf("\n");
	power_iteration(A, result, M, ranks, f, v, v_next);
	for (i = 0; i < n; ++i) {
		printf("%f ",v[i]);
	}
	eval = calc_leading_eigenvalue(A, result, M, ranks, f, v, v_next);
	printf("\neval = %f\n",eval);
}

int main (int argc, char* argv[]) {

	char* filename;
	/*spmat* Ag;
	double *b, *res;
	int ng, i;*/

	argc += 0;
	filename = argv[1];
	check_power_iteration(filename);
	/*Ag = check_create_Ag(filename);
	ng = Ag->n;
	printf("ng = %d\n", ng);
	res = (double*) malloc(sizeof(double)*ng);
	assert(res != NULL);
	b = (double*) malloc(sizeof(double)*ng);
	assert(b != NULL);
	for (i = 0; i < ng; ++i) {
		b[i] = i + (double) 5/(i+1);
		printf("%f ",b[i]);
	}
	printf("\n");
	Ag->mult(Ag, b, res);
	for (i = 0; i < ng; ++i)
	{
		printf("%f ", res[i]);
	}*/

	return 0;

}


