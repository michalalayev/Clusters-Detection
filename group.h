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
} group;

/*typedef struct
{
	LIST* list;
	spmat* Ag;
	int* ranks;
} group;*/

group* create_group(ELEM* head, int len);
group* initial_group(int n);
int* g_to_vector(group* g, int n);
group** split_group(int* s, group* g);

#endif /* GROUP_H_ */
