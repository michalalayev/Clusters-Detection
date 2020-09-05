#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "parser.h"
#include "spmat.h"
#include "group.h"
#include "stack.h"
#include "mult.h"
#include <time.h>
#include "optimization.h"

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

	g = initial_group(5);
	node = g->head;
	node->data = 1;
	node->next->data = 2;
	node->next->next->data = 4;
	node->next->next->next->data = 11;
	node->next->next->next->next->data = 12;
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
	Ag = create_Ag(A, g, 6, g_vector);

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
	int nnz;
	ArrayMat *private_A;
	int  *A_row, *ranks_vector, m , i, size;
	group *g;
	double *f;

	A = create_A(filename);
	size = A->n;
	f = (double*) malloc(sizeof(double)*(size+2));
	A_row = (int*) malloc(sizeof(int)*A->n);
	g = initial_group(A->n);
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

	free(A_row);
	free(f);
}

void check_stack()
{
	group *g,*g2,*g3;
	stack* st;
	int *arr, *arr2;
	st = initialize_stack();
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
	arr2 = (int*) malloc(sizeof(int)*2);
	arr2[0] = 4;
	arr2[2] = 5;
	g2 = create_group_by_array(arr2, 2);
	push(g2,st);
	printf("g2 len: %d ",st->top->g->len);
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
	if (!empty(st)) {
		printf("bi\n");
	}
	pop(st);
	printf("%d\n",empty(st));
	if (empty(st)) {
		printf("hi");
	}
}

void check_power_iteration(char* filename) {

	spmat* A;
	int *ranks, *A_row;
	ArrayMat *arr_mat;
	int i, n, M;
	double *f, *v, *result, *v_next/*, eval*/;
	group* g;

	A = create_A(filename);
	n = A->n;
	ranks = get_ranks();
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
	/*printf("f:\n");
	for (i = 0; i < n+2; ++i) {
		printf("%f ",f[i]);
	}
	printf("\n");*/
	power_iteration(A, result, M, ranks, f, v, v_next);
	for (i = 0; i < n; ++i) {
		printf("%f ",v[i]);
	}
	/*eval = calc_leading_eigenvalue(A, result, M, ranks, f, v, v_next);
	printf("\neval = %f\n",eval);*/
}

void check_power_iteration2(char* filename) {

	spmat* A;
	int *ranks, *A_row;
	ArrayMat *arr_mat;
	int i, n, M;
	double *f, *v, *v_next;
	group* g;

	A = create_A(filename);
	n = A->n;
	ranks = get_ranks();

	arr_mat = (ArrayMat*) A->private;
	M = arr_mat->rowptr[n];
	f = (double*) malloc(sizeof(double)*(n+2));
	A_row = (int*) malloc(sizeof(int)*n);
	v = (double*) malloc(sizeof(double)*n);
	v_next = (double*) malloc(sizeof(double)*n);
	for (i = 0; i < n; ++i) {
		v[i] = i+3;
	}
	g = initial_group(n);
	calc_f_1norm_and_nnz(A, A_row, g, ranks, M, f);
	/*printf("f:\n");
	for (i = 0; i < n+2; ++i) {
		printf("%f ",f[i]);
	}
	printf("\n");*/
	power_iteration2(A, M, ranks, f, v, v_next);
	/*for (i = 0; i < n; ++i) {
		printf("%f ",v[i]);
	}
	eval = calc_leading_eigenvalue(A, result, M, ranks, f, v, v_next);
	printf("\neval = %f\n",eval);*/
}

void check_fill_g_ranks(char* filename) {

	group* g;
	int arr[8];
	spmat* A;
	int *ranks, *g_ranks, i, ng;

	arr[0] = 0;
	arr[1] = 2;
	arr[2] = 3;
	arr[3] = 7;
	arr[4] = 10;
	arr[5] = 11;
	arr[6] = 12;
	arr[7] = 19;

	g = create_group_by_array(arr, 8);
	A = create_A(filename);
	ranks = get_ranks();
	g_ranks = (int*) malloc(sizeof(int)*(A->n));
	fill_g_ranks(g, ranks, g_ranks);
	ng = g->len;
	for (i = 0; i < ng; ++i) {
		printf("%d ",g_ranks[i]);
	}

}

void check_calc_score(char* filename) {

	/*double score;*/
	int *s, *ranks, *row, *unmoved, *indices;
	int M, i, n;
	spmat* A;
	ArrayMat* arr_mat;

	A = create_A(filename);
	n = A->n;
	ranks = get_ranks();
	arr_mat = (ArrayMat*) A->private;
	M = arr_mat->rowptr[n];

	s = (int*) malloc(sizeof(int)*n);
	row = (int*) malloc(sizeof(int)*n);
	unmoved = (int*) malloc(sizeof(int)*n);
	indices = (int*) malloc(sizeof(int)*n);
	for (i = 0; i < n; ++i) {
		s[i] = 1;
	}
	s[1] = -1;

	/*for (i = 0; i < n; ++i) {
		score = calc_score(s, A, ranks, M, i, row);
		printf("score[%d] = %f\n", i,score);
	}*/
	modularity_maximization(s, unmoved, indices, ranks, A, M, row);
	for (i = 0; i < n; ++i) {
		printf("s[%d] = %d\n", i,s[i]);
	}
}

void check_calc_deltaQ(char* filename) {

	double *f, nnz, deltaQ;
	int *s, *ranks, *result, *A_row, arr[5], *g_ranks;
	int M, i, n, ng;
	spmat *A, *Ag;
	ArrayMat* arr_mat;
	group* g;

	A = create_A(filename);
	n = A->n;
	ranks = get_ranks();
	arr_mat = (ArrayMat*) A->private;
	M = arr_mat->rowptr[n];

	arr[0] = 1;
	arr[1] = 2;
	arr[2] = 4;
	arr[3] = 11;
	arr[4] = 12;

	s = (int*) malloc(sizeof(int)*n);
	g_ranks = (int*) malloc(sizeof(int)*n);
	result = (int*) malloc(sizeof(int)*n);
	A_row = (int*) malloc(sizeof(int)*n);
	f = (double*) malloc(sizeof(double)*(n+2));
	g = create_group_by_array(arr, 5);
	ng = g->len;
	for (i = 0; i < ng; ++i) {
		s[i] = 1;
	}
	s[ng-1] = -1;

	calc_f_1norm_and_nnz(A, A_row, g, ranks, M, f);
	nnz = f[ng+1];
	Ag = create_Ag(A, g, nnz, A_row);
	fill_g_ranks(g, ranks, g_ranks);
	deltaQ = calc_deltaQ(Ag, result, s, g_ranks, M, f);
	printf("%f",deltaQ);
}



int main (int argc, char* argv[]) {

	char* filename;

	argc += 0;
	filename = argv[1];
	check_calc_deltaQ(filename);

	/*clock_t start,end;
	srand(time(NULL));
	argc += 0;
	filename = argv[1];
	start = clock();
	for (i = 0; i < 20; ++i) {
		check_power_iteration(filename);
	}
	end = clock();
	printf("\nExecution took %f seconds\n", ((double)(end-start) / CLOCKS_PER_SEC));*/

	/*check_stack();
	Ag = check_create_Ag(filename);
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


