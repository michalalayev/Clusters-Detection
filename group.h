/**
 * group summary:
 *
 * A linked list implementation of a group of nodes. Each element in the group
 * represents a node of the input graph that belongs to the group.
 *
 * set_up_group  - Creates a new group and sets up its fields.
 * initial_group - Creates a new group with all of the nodes of the input graph.
 * split_group   - Splits a group into 2 groups, according to received information how to split.
 *
 */

#ifndef GROUP_H_
#define GROUP_H_


/* Struct for an element in the group */
typedef struct _ELEM
{
	int data;            /* the name of the node */
	struct _ELEM *next;  /* pointer to the next element in the group */
} ELEM;

/* Struct for a group */
typedef struct
{
	ELEM* head; /* pointer to the first element */
	int len;    /* number of the elements in the group */
} group;


/* Creates a new group and sets up its fields to be the received head and len.
 * Returns a pointer to the created group. */
group* set_up_group(ELEM* head, int len);

/* Creates a new group with n elements, where the data of element i is i.
 * Returns a pointer to the created group. */
group* initial_group(int n);

/* Receives pointer to a group g and splits it into two groups, according to the values in the vector s:
 * if s[i] = 1 then g[i] goes to one group, and if s[i] = -1 then g[i] goes to another group.
 * Returns a pointer to an array that contains pointers to the two new groups.
 * If s contains only 1 or -1, then the new groups are the full original g and an empty group. */
group** split_group(int* s, group* g);


#endif /* GROUP_H_ */
