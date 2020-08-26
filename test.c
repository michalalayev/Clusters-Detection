#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "spmat.h"
#include "group.h"

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

void print_A(char* filename, int n){
	spmat* A;
	ArrayMat *arr_mat;
	int *rp, *colind, i;

	A = create_A(filename);
	arr_mat = (ArrayMat*) A->private;
	rp = arr_mat->rowptr;
	colind = arr_mat->colind;
	for (i = 0; i < n+1; ++i) {
		printf("col = %d, rp = %d\n", colind[i],rp[i+1]);
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
}

void check_create_Ag(char* filename) {
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

	g_vector = (int*) calloc(4,sizeof(int));
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
}

int main () {

	return 0;

}


