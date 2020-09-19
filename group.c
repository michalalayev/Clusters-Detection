#include <stdlib.h>
#include "group.h"
#include "errors.h"


group* set_up_group(ELEM* head, int len)
{
	group* g;
	g = (group*) malloc(sizeof(group));
	check_alloc(g);
	g->head = head;
	g->len = len;
	return g;
}

group* initial_group(int n)
{
	ELEM *head, *tail, *node;
	int i;
	group *g;

	head = (ELEM*) malloc(sizeof(ELEM));
	check_alloc(head);
	head -> data = 0;
	tail = head;
	for (i = 1; i < n; ++i) {
		node = (ELEM*) malloc(sizeof(ELEM));
		check_alloc(node);
		node->data = i;
		tail->next = node;
		tail = tail->next;
	}
	tail->next = NULL;

	g = (group*) malloc(sizeof(group));
	check_alloc(g);
	g->len = n;
	g->head = head;
	return g;
}

group** split_group(int* s, group* g)
{
	int len, i, first, second, cnt1, cnt2, curr ,prev;
	ELEM *head2, *last1, *last2;
	group *empty_group, *g2;
	group** splited_g;

	splited_g = (group**) malloc(sizeof(group*)*2);
	check_alloc(splited_g);
	len = g->len;
	cnt1 = cnt2 = 0;
	first = *s;
	second = -first;
	while (*s == first && cnt1 < len) {
		cnt1++;
		s++;
	}
	if (cnt1 == len) {
		empty_group = set_up_group(NULL,0);
		splited_g[0] = g;
		splited_g[1] = empty_group;
		return splited_g;
	}
	last1 = g->head;
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
	g->len = cnt1;
	g2 = set_up_group(head2, cnt2);
	splited_g[0] = g;
	splited_g[1] = g2;
	return splited_g;
}





