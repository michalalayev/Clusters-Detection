#ifndef OPTIMIZATION_H_
#define OPTIMIZATION_H_

#include "spmat.h"

double calc_score(int* s, spmat* Ag, int* g_ranks, int M, int i, int* row);
void modularity_maximization(int* s, int* unmoved, int* indices, int* g_ranks, spmat* Ag, int M, int* row);
void fill_with_ones(int* s, int len);
void create_s(int* s, double* v, int len);

#endif /* OPTIMIZATION_H_ */
