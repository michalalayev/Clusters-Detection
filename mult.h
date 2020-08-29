/*
 * mult.h
 *
 *  Created on: 29 ���� 2020
 *      Author: ����
 */

#ifndef MULT_H_
#define MULT_H_

#include "spmat.h"
#include <math.h>

int mult_vectors_int(int* vec1, int* vec2, int n);
double mult_vectors_int_and_double(int* vec1, double* vec2, int n);
double mult_vectors_double(double* vec1, double* vec2, int n);
void power_iteration(spmat* Ag, double* result, int M, int* g_ranks, double* f, double* b_curr, double* b_next);
double calc_leading_eigenvalue(spmat* Ag, double* result, int M, int* g_ranks, double* f, double* u, double* Au);

#endif /* MULT_H_ */