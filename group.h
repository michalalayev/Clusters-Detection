
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


group* set_up_group(ELEM* head, int len);
group* initial_group(int n);
group** split_group(int* s, group* g);

#endif /* GROUP_H_ */
