#include "stack.h"

void initialize(stack *stk)
{
   stk -> cnt = 0;
   stk -> top = NULL;
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
