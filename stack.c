#include "stack.h"
#include "errors.h"

stack* initialize_stack()
{
	stack *stk;

	stk = (stack*) malloc(sizeof(stack));
	check_alloc(stk);
	stk -> cnt = 0;    /* no elements in the stack */
	stk -> top = NULL; /* no top element, empty stack*/
	return stk;
}

void push(group *g, stack *stk)
{
   s_elem* p;

   p = (s_elem*) malloc(sizeof(s_elem));
   check_alloc(p);
   p -> g = g;
   p -> next = stk -> top;
   stk -> top = p; /*p is now at the top of the stack*/
   stk -> cnt++;   /*one element addded*/
}

group* pop(stack *stk)
{
   group* g;
   s_elem* p;

   g = stk -> top -> g; /*save pointer to the top group*/
   p = stk -> top;
   stk -> top = stk -> top -> next;
   stk -> cnt--; /*one element removed*/
   free(p);
   return g;
}

group* top(const stack *stk)
{
   return (stk -> top -> g);
}

boolean empty(const stack *stk)
{
   return ((boolean) (stk -> cnt == EMPTY));
}




