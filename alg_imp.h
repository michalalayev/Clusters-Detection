
#ifndef ALG_IMP_H_
#define ALG_IMP_H_

#include "group.h"
#include "spmat.h"
#include "stack.h"

#define epsilon 0.00001
#define IS_POSITIVE(X) ((X) > 0.00001)

/* In all of the functions, the array ranks contains the ranks of all nodes, when ranks[i] is the rank of the node i,
 * and M is the sum of the ranks.*/



/* Creates a sub-matrix Ag of A according to the nodes in the group g. Ag is pre-allocated,
 * and g_vector is a pre-allocated helping array. */
void create_Ag(spmat* A, spmat* Ag, group* g, int* g_vector);


/* Calculates the sum of every row i in the matrix B[g] (of size n*n), using the matrix A, the relevant group g,
 * and the pre-allocated helping array A_row, and saves it into the pre-allocated array f at index i.
 * The function also calculates the 1-norm of B[g]_hat (saved into f[n]) */
void calc_f_and_1norm(spmat* A, int* A_row, group* g, int* ranks, int M, double* f);

/* Fills the pre-allocated array g_ranks with the ranks of the nodes in the group g */
void fill_g_ranks(group* g, int* ranks, int* g_ranks);


void power_iteration(spmat* Ag, double* result, int M, int* g_ranks, double* f, double* b_curr, double* b_next);


double calc_leading_eigenvalue(spmat* Ag, double* result, int M, int* g_ranks, double* f, double* u, double* Au);


double calc_deltaQ(spmat* Ag, int* result, int* s, int* g_ranks, int M, double* f);


void create_random_vector(double *b, int len);


void modularity_maximization(int* s, int* unmoved, int* indices, double* score, int* g_ranks, spmat* Ag, int M, int* row, int* result);


void fill_with_ones(int* s, int len);


void create_s(int* s, double* v, int len);


void put_groups_in_stacks(group** splited_g, stack* P, stack* O);




#endif /* ALG_IMP_H_ */
