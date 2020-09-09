/*
 * stack.h
 *
 *  Created on: 24 ???? 2020
 *      Author: Michal
 */

#ifndef STACK_H_
#define STACK_H_

/* A linked list implementation of a stack. */

#include <stdio.h>
#include <stdlib.h>
#include "group.h"

#define   EMPTY      0
#define   FULL       5000000 /*five million*/

typedef enum {false, true} boolean;

typedef struct elem {  /* an element on the stack */
   group* g;
   struct elem* next;
} s_elem;


typedef struct {
   int cnt;      /* a count of the elements */
   s_elem* top;  /* ptr to the top element */
} stack;


stack* initialize_stack();
void push(group *g, stack *stk);
group* pop(stack *stk);
group* top(const stack *stk);
boolean empty(const stack *stk);
boolean full(const stack *stk);
void put_groups_in_stacks(group** splited_g, stack* P, stack* O);

#endif /* STACK_H_ */
