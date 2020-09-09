/*
 * mult.h
 *
 *  Created on: 29 באוג 2020
 *      Author: רוני
 */

#ifndef MULT_H_
#define MULT_H_

#include "spmat.h"
#include <math.h>

int mult_vectors_int(int* vec1, int* vec2, int n);
double mult_vectors_int_and_double(int* vec1, double* vec2, int n);
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


#endif /* MULT_H_ */
