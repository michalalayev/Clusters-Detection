/**
 * alg_imp summary:
 *
 * This module contains all the functions that executes the division algorithm, as described
 * in the project instructions.
 *
 * create_Ag               - Creates a sub-matrix A[g] of A according to the nodes of group g.
 * calc_f_and_1norm        - Calculates the vector f and the 1-norm of B[g].
 * fill_g_ranks            - Fills an array with the ranks of the nodes in the group g.
 * power_iteration         - Executes the power iteration (PI) algorithm to find leading eigen vector of B[g]_hat.
 * calc_leading_eigenvalue - Calculates the leading eigen value of B[g]_hat.
 * calc_deltaQ             - Calculates the change in Q (the modularity) after a change in s.
 * create_random_vector    - Creates a vector with random values, that used at the beginning of the PI.
 * modularity_maximization - Executes the modularity maximization algorithm that optimizes a division.
 * fill_with_ones          - Fills an entire vector with the value 1.
 * put_groups_in_stacks    - Adds the new two groups (splited from g) to P or O according to their length.
 * create_s                - Assigning the values of vector s to be 1 or -1 according to the values of the
 *                           leading eigen vector v (if v[i] positive then s[i]=1, otherwise s[i]= -1).
 */

#ifndef ALG_IMP_H_
#define ALG_IMP_H_

#include "group.h"
#include "spmat.h"
#include "stack.h"

#define epsilon 0.00001
#define IS_POSITIVE(X) ((X) > 0.00001)


/* Clarification:
 * In all of the functions, the array ranks contains the ranks of all nodes,
 * where ranks[i] is the rank of the node i, and M is the sum of the ranks.
 * Also, the array g_ranks contains the ranks of the nodes in a group g,
 * where g_ranks[i] is the rank of the node in g[i].
 */


/* Creates a sub-matrix Ag of A according to the nodes in the group g.
 * Ag is pre-allocated, and g_vector is a pre-allocated helping array. */
void create_Ag(spmat* A, spmat* Ag, group* g, int* g_vector);


/* Calculates the sum of every row i in the matrix B[g] (of size n*n),
 * using the matrix A, the relevant group g, ranks, M, and the pre-allocated
 * helping array A_row, and saves it into the pre-allocated array f at index i.
 * The function also calculates the 1-norm of B[g]_hat (saved into f[n]) */
void calc_f_and_1norm(spmat* A, int* A_row, group* g, int* ranks, int M, double* f);


/* Fills the pre-allocated array g_ranks with the ranks of the nodes in the group g */
void fill_g_ranks(group* g, int* ranks, int* g_ranks);


/* Executes the power iteration algorithm to find the leading eigenvector
 * of B[g]_hat, by using the sub-matrix A[g], g_ranks, M, the pre-calculated
 * vector f, and the pre-allocated helping array result.
 * b_curr is initialized as a random vector, and b_next used to save the next
 * vector that's created in the process of PI.
 * When the function ends, b_curr is the wanted leading eigenvector (normalized). */
void power_iteration(spmat* Ag, double* result, int M, int* g_ranks, double* f, double* b_curr, double* b_next);


/* Calculates the leading eigenvalue of B[g]_hat and return it.
 * Uses the sub-matrix A[g], g_ranks, M, the pre-calculated vector f, and the
 * pre-allocated helping array result.u is the leading eigenvector of B[g]_hat,
 * and Au is a pre-allocated array to save the result of B[g]_hat*u. */
double calc_leading_eigenvalue(spmat* Ag, double* result, int M, int* g_ranks, double* f, double* u, double* Au);


/* Calculates the change in Q (the modularity), using the sub-matrix A[g],
 * the vector s, g_ranks, M, the pre-calculated vector f, and the pre-allocated
 * helping array result. deltaQ = sT*B[g]_hat*s. */
double calc_deltaQ(spmat* Ag, int* result, int* s, int* g_ranks, int M, double* f);


/* Creates a random vector of length len, by filling the pre-allocated
 * array b with random values. */
void create_random_vector(double *b, int len);


/* Executes the modularity maximization algorithm that optimizes a division, and changes
 * the vector s accordingly. Uses the pre-allocated arrays unmoved, indices, score,
 * as described in the algorithm. result and row are pre-allocated helping arrays.
 * Also uses the sub-matrix A[g], g_ranks, and M to calculate needed values of B[g]. */
void modularity_maximization(int* s, int* unmoved, int* indices, double* score, int* g_ranks, \
		spmat* Ag, int M, int* row, int* result);


/* Fills the first len cells of the pre-allocated array s with the value 1. */
void fill_with_ones(int* s, int len);


/* Assigning the values of vector s according to the values of the vector v:
 * if v[i] positive then s[i]=1, otherwise s[i]= -1. s and v are of length len.*/
void create_s(int* s, double* v, int len);


/* Recieves a pointer splited_g to an array of 2 pointers to groups, and adds these groups
 * to the right stack according to its length:
 * If one of those groups is of length 0, then the other group is added to O.
 * Else, a group of length 1 is added to O, and a group of length greater than 1 is added to P.*/
void put_groups_in_stacks(group** splited_g, stack* P, stack* O);


#endif /* ALG_IMP_H_ */
