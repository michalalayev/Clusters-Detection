#include <stdio.h>
#include <stdlib.h>
#include "spmat.h"
#include "group.h"
/*typedef struct _ELEM
{*/
	/*int data; the name of the node*/
/*	struct _ELEM *next;
} ELEM;

typedef struct
{
	ELEM* head;
	int len;
} LIST;

typedef struct
{
	LIST* list;*/
	/*spmat* Ag;
	int* ranks;*/
/*} group;*/

LIST* create_list(ELEM* head, int len)
{
	LIST* lst;

	lst = (LIST*) malloc(sizeof(LIST));
	/*### assert lst != NULL ###*/
	lst->head = head;
	lst->len = len;
	return lst;
}

LIST* initial_list(int n)
{
	ELEM *head, *tail, *node;
	int i;
	LIST *lst;

	head = (ELEM*) malloc(sizeof(ELEM));
	/*###assert(head!=NULL); ###*/
	printf("hereinitial");
	head -> data = 0;
	tail = head;
	for (i = 1; i < n; ++i) {
		node = (ELEM*) malloc(sizeof(ELEM));
		/*###assert(node!=NULL); ###*/
		node->data = i;
		tail->next = node;
		tail = tail->next;
	}
	tail->next = NULL;

	lst = (LIST*) malloc(sizeof(LIST));
	/*###assert(lst!=NULL); ###*/
	lst->len = n;
	lst->head = head;
	return lst;
}

group* create_group(LIST* lst)
{
	group* g;

	g = (group*) malloc(sizeof(group));
	/*###assert(g!=NULL); ###*/
	/*g->Ag = mat;*/
	g->list = lst;
	/*g->ranks = ranks;*/
	return g;
}

LIST** split_list(int* s, LIST* list)
{
	int len, i, first, second, cnt1, cnt2, curr ,prev;
	ELEM *head2, *last1, *last2;
	LIST *empty_group, *list2;
	LIST** splited_g;

	splited_g = (LIST**) malloc(sizeof(LIST*)*2);
	/*###assert(splited_g!=NULL); ###*/
	len = list->len;
	cnt1 = cnt2 = 0;
	first = *s;
	second = -first;
	while (*s == first && cnt1 < len+1) {
		cnt1++;
		s++;
	}
	if (cnt1 == len) {
		empty_group = create_list(NULL,0);
		splited_g[0] = list;
		splited_g[1] = empty_group;
		return splited_g;
	}
	last1 = list->head;
	for (i = 0; i < cnt1-1; ++i) {
		last1 = last1->next;
	}
	head2 = last1->next;
	last2 = last1->next;
	for(i = cnt1; i < len; i++){
		curr = *s;
		prev = *(s-1);
		if (curr == prev) {
			if (prev == second) {
				last2 = last2->next;
				cnt2++;
			}
			else { /*prev == first*/
				last1 = last1->next;
				cnt1++;
			}
		}
		else {
			if (curr == first) {
				if (last2->next != last1) {
					last1->next = last2->next;
					last1 = last2->next;
				}
				cnt1++;
			}
			else { /*curr == second*/
				if (last1->next != last2) {
					last2->next = last1->next;
					last2 = last1->next;
				}
				cnt2++;
			}
		}
		s++;
	}
	last1->next = NULL;
	last2->next = NULL;
	list->len = cnt1;
	list2 = create_list(head2, cnt2);
	splited_g[0] = list;
	splited_g[1] = list2;
	return splited_g;
}

int* g_to_vector(LIST* g_list, int n)
{
	int nodeNum;
	int* g_vector;
	int i;
	int val;
	int g_len;
	ELEM* currNode;
	currNode = g_list->head;
	g_len=g_list->len;
	g_vector = (int*) calloc(n,sizeof(int));

	for(i = 0; i < g_len; i++)
	{
		val = i;
	    nodeNum = currNode->data;
		if(i == 0)
		{
			val = -1;
		}
		g_vector[nodeNum] = val;
		currNode = currNode->next;
	}

	return g_vector;
}



