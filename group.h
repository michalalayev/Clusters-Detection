/*
 * group.h
 *
 *  Created on: 23 ???? 2020
 *      Author: Michal
 */

#ifndef GROUP_H_
#define GROUP_H_

typedef struct _ELEM
{
	int data;  /*the name of the node*/
	struct _ELEM *next;
} ELEM;

typedef struct
{
	ELEM* head;
	int len;
} LIST;

typedef struct
{
	LIST* list;
	/*spmat* Ag;
	int* ranks;*/
} group;

LIST* create_list(ELEM* head, int len);
LIST* initial_list(int n);
group* create_group(LIST* lst);
int* g_to_vector(LIST* g_list, int n);
LIST** split_list(int* s, LIST* list);

#endif /* GROUP_H_ */
