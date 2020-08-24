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
	LIST *lst;
	int i;
	int *new_g;
	lst = initial_list(10);
	node = lst->head;
	for(i = 0; i<10; i++)
		{
			printf("%d",node->data);
			node=node->next;

		}
	new_g = g_to_vector(lst,10);

	printf("\n");
	for(i = 0; i<10; i++)
	{
		printf("%d",new_g[i]);

	}
}

void check_split()
{
	LIST **pointer;
	/*int cnt1;
	int cnt2;*/
	int i, cnt1, cnt2;
	int* arr;
	LIST *lst2;
	ELEM *currNode2;

	lst2 = initial_list(10);
	arr = (int*) malloc(sizeof(int)*10);
	for(i = 0; i < 10; i++)
	{
		arr[i] = -1;
	}
	arr[0] = 1;
/*	arr[3] = 1;
	arr[5] = 1;
	arr[7] = 1;*/
	arr[9] = 1;
	pointer = split_list(arr,lst2);
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

int main (int argc, char* argv[]) {

	char* filename;

	argc += 0;
	filename = argv[1];

	print_A(filename,20);
	print_k(20);

	return 0;


}


