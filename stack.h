/**
 * stack summary:
 *
 * A linked list implementation of a stack, each element in the stack is a group.
 * Used for representing a group of groups.
 *
 * initialize_stack - creates new stack and initializes its feilds
 * push             - Adds new group to the top of the stack
 * pop              - Deletes the top group in the stack
 * top              - Returns the top group in the stack
 * empty            - Checks if the stack is empty
 *
 */

#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>
#include <stdlib.h>
#include "group.h"

#define EMPTY 0

/* Type used for returning a boolean in the function empty */
typedef enum {false, true} boolean;

/* Struct for an element on the stack */
typedef struct elem {
   group* g;          /* the group contained in the element */
   struct elem* next; /* pointer to the next element on the stack */
} s_elem;

/* Struct for a stack */
typedef struct {
   int cnt;      /* a count of the elements */
   s_elem* top;  /* pointer to the top element */
} stack;

/* Creates and initializes a new empty stack, and returns a pointer to it. */
stack* initialize_stack();

/* Adds the group pointed by g to the top of the stack pointed by stk. */
void push(group *g, stack *stk);

/* Deletes the top group from the stack pointed by stk,
 * and returns a pointer to this group. */
group* pop(stack *stk);

/* Returns a pointer to the top group in the stack pointed by stk. */
group* top(const stack *stk);

/* Returns true if the stack is empty (of length 0), and false otherwise. */
boolean empty(const stack *stk);


#endif /* STACK_H_ */
