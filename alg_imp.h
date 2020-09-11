
#ifndef ALG_IMP_H_
#define ALG_IMP_H_

#include "group.h"
#include "spmat.h"
#include "stack.h"

#define epsilon 0.00001
#define IS_POSITIVE(X) ((X) > 0.00001)

/* In all of the functions, the array ranks contains the ranks of all nodes, when ranks[i] is the rank of the node i,
 * and M is the sum of the ranks.*/


void g_to_vector(group* g, int* g_vector);

/* Fills with zeroes the pre-allocated array row, of length n */
void reset_row(int* row, int n);

/* Creates a sub-matrix Ag of A, with nnz non-zero elements, according to the nodes in the group g.
 * g_vector is a pre-allocated helping array. */
spmat* create_Ag(spmat* A, group* g, int nnz, int* g_vector);

/* Builds full row of the matrix A from its sparse representation, into the pre-allocated array A_row.
 * row_num is the index of the row in A to build. */
void build_full_row(spmat* A, int* A_row, int row_num);

/* Calculates the sum of every row i in the matrix B[g], using the matrix A, the relevant group g,
 * and the pre-allocated helping array A_row, and saves it into the pre-allocated array f at index i.
 * Suppose B[g] is of size n. The function also calculates the 1-norm of B[g]_hat (saved into f[n]),
 * and the number of non-zero elements in A[g] (saved into f[n+1]) */
void calc_f_1norm_and_nnz(spmat* A, int* A_row, group* g, int* ranks, int M, double* f);

/* Fills the pre-allocated array g_ranks with the ranks of the nodes in the group g */
void fill_g_ranks(group* g, int* ranks, int* g_ranks);

/* Multiplies two vectors of type int and length n, and returns a scalar */
int mult_vectors_int(int* vec1, int* vec2, int n);

/* Multiplies two vectors of length n, first of type int and second of type double, and returns a scalar */
double mult_vectors_int_and_double(int* vec1, double* vec2, int n);

/* Multiplies two vectors of type double and length n, and returns a scalar */
double mult_vectors_double(double* vec1, double* vec2, int n);

void power_iteration(spmat* Ag, double* result, int M, int* g_ranks, double* f, double* b_curr, double* b_next);
void power_iteration2(spmat* Ag, int M, int* g_ranks, double* f, double* b_curr, double* b_next);
void power_iteration3(spmat* Ag, int M, int* g_ranks, double* f, double* b_curr, double* b_next);

double calc_leading_eigenvalue(spmat* Ag, double* result, int M, int* g_ranks, double* f, double* u, double* Au);
double calc_leading_eigenvalue2(spmat* Ag, int M, int* g_ranks, double* f, double* u, double* Au);
double calc_leading_eigenvalue3(spmat* Ag, int M, int* g_ranks, double* f, double* u, double* Au);

double calc_deltaQ(spmat* Ag, int* result, int* s, int* g_ranks, int M, double* f);
double calc_deltaQ2(spmat* Ag, int* s, int* g_ranks, int M, double* f);
double calc_deltaQ3(spmat* Ag, int* s, int* g_ranks, int M, double* f);

void create_random_vector(double *b, int len);

/*i is the index of the row of Ag, we want to claculate score[i]*/
double calc_score(int* s, spmat* Ag, int* g_ranks, int M, int i, int* row);
void modularity_maximization(int* s, int* unmoved, int* indices, int* g_ranks, spmat* Ag, int M, int* row);
void fill_with_ones(int* s, int len);
void create_s(int* s, double* v, int len);

void put_groups_in_stacks(group** splited_g, stack* P, stack* O);


#endif /* ALG_IMP_H_ */
