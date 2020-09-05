#ifndef OPTIMIZATION_H_
#define OPTIMIZATION_H_

#include "spmat.h"

double calc_score(int* s, spmat* Ag, int* g_ranks, int M, int i, int* row);
void modularity_maximization(int* s, int* unmoved, int* indices, int* g_ranks, spmat* Ag, int M, int* row);

#endif /* OPTIMIZATION_H_ */
