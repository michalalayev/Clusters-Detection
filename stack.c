#include "stack.h"

stack* initialize_stack()
{
	stack *stk;

	stk = (stack*) malloc(sizeof(stack));
	stk -> cnt = 0;
	stk -> top = NULL;
	return stk;
}

void push(group *g, stack *stk)
{
   s_elem* p;

   p = (s_elem*) malloc(sizeof(s_elem));
   p -> g = g;
   p -> next = stk -> top;
   stk -> top = p;
   stk -> cnt++;
}

group* pop(stack *stk)
{
   group* g;
   s_elem* p;

   g = stk -> top -> g;
   p = stk -> top;
   stk -> top = stk -> top -> next;
   stk -> cnt--;
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

boolean full(const stack *stk)
{
   return ((boolean) (stk -> cnt == FULL));
}

void put_groups_in_stacks(group** splited_g, stack* P, stack* O)
{
	group *g1, *g2;

	g1 = splited_g[0];
	g2 = splited_g[1];
	free(splited_g);

	if (g2->len == 0) { /*g2 is empty, and g1 is the full g*/
		push(g1, O);
		free(g2);
	}
	else {
		if (g2->len == 1) {
			push(g2, O);
		}
		else {
			push(g2, P);
		}
		if (g1->len == 1) {
			push(g1, O);
		}
		else {
			push(g1, P);
		}
	}
}

