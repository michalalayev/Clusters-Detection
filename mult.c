#include "mult.h"
#include <stdio.h> /*delete this later*/
#include <stdlib.h> /*for rand() */
#define epsilon 0.00001

/*getting two int vectors, mult them and return scalar.*/
int mult_vectors_int(int* vec1, int* vec2, int n)
{
	int result, i;
	result = 0;
	for(i = 0; i < n; i++)
	{
		result += (*vec1) * (*vec2);
		vec1++;
		vec2++;
	}
	return result;
}

/*getting two vectors, int and double, mult them and return scalar.*/
double mult_vectors_int_and_double(int* vec1, double* vec2, int n)
{
	int i;
	double result;
	result = 0;
	for(i = 0; i < n; i++)
	{
		result += (*vec1) * (*vec2);
		vec1++;
		vec2++;
	}
	return result;
}

/*getting two double vectors, mult them and return scalar.*/
double mult_vectors_double(double* vec1, double* vec2, int n)
{
	int i;
	double result;
	result = 0;
	for(i = 0; i < n; i++)
	{
		result += (*vec1) * (*vec2);
		vec1++;
		vec2++;
	}
	return result;
}

/*when the function ends, b_curr is the wanted leading vector (normalized)*/
void power_iteration(spmat* Ag, double* result, int M, int* g_ranks, double* f, double* b_curr, double* b_next)
{
	double x, curr, next, magn, norm1;
	double *b_curr_start, *b_next_start, *res_start, *f_start, *tmp;
	int ng, i, done, cnt;
	int *g_ranks_start;

	ng = Ag->n;
	norm1 = f[ng];
	b_curr_start = b_curr;
	b_next_start = b_next;
	res_start = result;
	f_start = f;
	g_ranks_start = g_ranks;
	done = 0; /*false*/
	cnt = 0;
	while (done == 0) {
		cnt++;
		done = 1; /*assume it's true*/
		magn = 0;
		Ag->mult_double(Ag, b_curr, result);
		x = mult_vectors_int_and_double(g_ranks, b_curr, ng);
		x = x/M;
		for (i = 0; i < ng; ++i) { /*the i'th iteration calculates the i element in b_next vector*/
			curr = *b_curr;
			/*if(cnt == 1) {
				printf("curr = %f\n",curr);
			}*/
			next = *result - x*(*g_ranks) - (*f)*curr + norm1*curr;
			/*if(cnt == 1) {
				printf("next = %f\n",next);
				printf("result = %f, x = %f, g_ranks = %d, f = %f, norm1 = %f\n",*result,x,*g_ranks,*f,norm1);
			}*/
			magn += (next*next);
			/*if (cnt == 1) {
				printf("magn = %f, next*next = %f\n", magn, next*next);
			}*/
			*b_next = next;
			b_next++;
			result++;
			g_ranks++;
			f++;
			b_curr++;
		} /*b_next is fully calculated now*/
		/*if (cnt == 1) {
			printf("magn = %f\n",magn);
		}*/
		f = f_start;
		result = res_start;
		g_ranks = g_ranks_start;
		b_next = b_next_start;
		b_curr = b_curr_start;
		/*standartize b_next:*/
		magn = sqrt(magn);
		for (i = 0; i < ng; ++i){
			*b_next /= magn;
			if (done != 0 && fabs(*b_next - *b_curr) > epsilon)
			{
				done = 0; /*false, we are not done*/
			}
			b_next++;
			b_curr++;
		}
		/*switch between the vectors:*/
		tmp = b_curr_start;
		b_curr = b_next_start;
		b_next = tmp;
		b_curr_start = b_curr;
		b_next_start = b_next;
		/*printf("b_curr:\n");
		for (i = 0; i < ng; ++i) {
			printf("%f ",b_curr[i]);
		}
		printf("\n");*/
	}
	/*printf("cnt = %d\n",cnt);*/
}

void power_iteration2(spmat* Ag, int M, int* g_ranks, double* f, double* b_curr, double* b_next)
{
	double x, curr, next, magn, norm1, result;
	double *b_curr_start, *b_next_start, *f_start, *tmp;
	int ng, i, done, cnt;
	int *g_ranks_start;

	ng = Ag->n;
	norm1 = f[ng];
	b_curr_start = b_curr;
	b_next_start = b_next;
	f_start = f;
	g_ranks_start = g_ranks;
	done = 0; /*false*/
	cnt = 0;
	while (done == 0) {
		cnt++;
		done = 1; /*assume it's true*/
		magn = 0;

		x = mult_vectors_int_and_double(g_ranks, b_curr, ng);
		x = x/M;
		for (i = 0; i < ng; ++i) { /*the i'th iteration calculates the i element in b_next vector*/
			curr = *b_curr;
			result = array_mult_double2(Ag, b_curr_start, i); /* ############# change this line to the following,
			and change the allocate function so mult_double is the function array_mult_double2
			Ag->mult_double(Ag, b_curr_start, i); ### */
			next = result - x*(*g_ranks) - (*f)*curr + norm1*curr;
			magn += (next*next);
			*b_next = next;
			b_next++;
			g_ranks++;
			f++;
			b_curr++;
		}
		f = f_start;
		g_ranks = g_ranks_start;
		b_next = b_next_start;
		b_curr = b_curr_start;
		/*standartize b_next:*/
		magn = sqrt(magn);
		for (i = 0; i < ng; ++i){
			*b_next /= magn;
			if (done != 0 && fabs(*b_next - *b_curr) > epsilon)
			{
				done = 0; /*false, we are not done*/
			}
			b_next++;
			b_curr++;
		}
		/*switch between the vectors:*/
		tmp = b_curr_start;
		b_curr = b_next_start;
		b_next = tmp;
		b_curr_start = b_curr;
		b_next_start = b_next;
	}
}

void power_iteration3(spmat* Ag, int M, int* g_ranks, double* f, double* b_curr, double* b_next)
{
	ArrayMat *arr_mat;
	double x, curr, next, magn, norm1, result;
	double *b_curr_start, *b_next_start, *f_start, *tmp;
	int ng, i, done, cnt, nnz_in_row;
	int *g_ranks_start, *rp, *colind, *rp_start, *colind_start;

	ng = Ag->n;
	arr_mat = (ArrayMat*) Ag->private;
	rp = arr_mat->rowptr;
	colind = arr_mat->colind;

	norm1 = f[ng];
	b_curr_start = b_curr;
	b_next_start = b_next;
	f_start = f;
	g_ranks_start = g_ranks;
	colind_start = colind;
	rp_start = rp;

	done = 0; /*false*/
	cnt = 0;
	while (done == 0) {
		cnt++;
		done = 1; /*assume it's true*/
		magn = 0;
		x = mult_vectors_int_and_double(g_ranks, b_curr, ng);
		x = x/M;
		for (i = 0; i < ng; ++i) { /*the i'th iteration calculates the i element in b_next vector*/
			curr = *b_curr;
			nnz_in_row = *(rp+1)-(*rp);
			rp++;
			result = array_mult_double3(b_curr_start, colind, nnz_in_row); /* ############# change this line to the following,
			and change the allocate function so mult_double is the function array_mult_double3
			Ag->mult_double(b_curr_start, colind, nnz_in_row); ### */
			colind += nnz_in_row;
			next = result - x*(*g_ranks) - (*f)*curr + norm1*curr;
			magn += (next*next);
			*b_next = next;
			b_next++;
			g_ranks++;
			f++;
			b_curr++;
		}
		f = f_start;
		g_ranks = g_ranks_start;
		b_next = b_next_start;
		b_curr = b_curr_start;
		colind = colind_start;
		rp = rp_start;
		/*standartize b_next:*/
		magn = sqrt(magn);
		for (i = 0; i < ng; ++i){
			*b_next /= magn;
			if (done != 0 && fabs(*b_next - *b_curr) > epsilon)
			{
				done = 0; /*false, we are not done*/
			}
			b_next++;
			b_curr++;
		}
		/*switch between the vectors:*/
		tmp = b_curr_start;
		b_curr = b_next_start;
		b_next = tmp;
		b_curr_start = b_curr;
		b_next_start = b_next;
	}
}

/*u is the leading eigenvector of Bg, it is saved on the memory allocated for b_curr*/
double calc_leading_eigenvalue(spmat* Ag, double* result, int M, int* g_ranks, double* f, double* u, double* Au)
{
	double lamda, numerator, denominator, x, curr, next, *Au_start, *u_start;
	int ng, norm1, i;

	Au_start = Au;
	u_start = u;
	ng = Ag->n;
	Ag->mult_double(Ag, u, result);

	x = mult_vectors_int_and_double(g_ranks, u, ng);
	x = x/M;
	norm1 = f[ng];

	for (i = 0; i < ng; ++i) { /*calculate Bg_hat mult u (the result is Au)*/
		curr = *u;
		next = *result - x*(*g_ranks) - (*f)*curr + norm1*curr;
		*Au = next;
		Au++;
		result++;
		g_ranks++;
		f++;
		u++;
	}

	Au = Au_start;
	u = u_start;
	numerator = mult_vectors_double(u, Au, ng);
	denominator = mult_vectors_double(u, u, ng);
	lamda = numerator/denominator;
	return (lamda - norm1);
}

double calc_leading_eigenvalue2(spmat* Ag, int M, int* g_ranks, double* f, double* u, double* Au)
{
	double result, lamda, numerator, denominator, x, curr, next, *Au_start, *u_start;
	int ng, norm1, i;

	Au_start = Au;
	u_start = u;
	ng = Ag->n;
	x = mult_vectors_int_and_double(g_ranks, u, ng);
	x = x/M;
	norm1 = f[ng];

	for (i = 0; i < ng; ++i) { /*calculate Bg_hat mult u (the result is Au)*/
		curr = *u;
		result = array_mult_double2(Ag, u_start, i); /* ############# change this line to the following,
		and change the allocate function so mult_double is the function array_mult_double2
		Ag->mult_double(Ag, u_start, i); ### */
		next = result - x*(*g_ranks) - (*f)*curr + norm1*curr;
		*Au = next;
		Au++;
		g_ranks++;
		f++;
		u++;
	}
	Au = Au_start;
	u = u_start;
	numerator = mult_vectors_double(u, Au, ng);
	denominator = mult_vectors_double(u, u, ng);
	lamda = numerator/denominator;
	return (lamda - norm1);
}

double calc_leading_eigenvalue3(spmat* Ag, int M, int* g_ranks, double* f, double* u, double* Au)
{
	ArrayMat *arr_mat;
	double result, lamda, numerator, denominator, x, curr, next, *Au_start, *u_start;
	int ng, norm1, i, *colind, *rp, nnz_in_row;

	Au_start = Au;
	u_start = u;
	ng = Ag->n;
	arr_mat = (ArrayMat*) Ag->private;
	rp = arr_mat->rowptr;
	colind = arr_mat->colind;

	x = mult_vectors_int_and_double(g_ranks, u, ng);
	x = x/M;
	norm1 = f[ng];


	for (i = 0; i < ng; ++i) { /*calculate Bg_hat mult u (the result is Au)*/
		curr = *u;
		nnz_in_row = *(rp+1)-(*rp);
		rp++;
		result = array_mult_double3(u_start, colind, nnz_in_row); /* ############# change this line to the following,
		and change the allocate function so mult_double is the function array_mult_double3
		Ag->mult_double(u_start, colind, nnz_in_row); ### */
		colind += nnz_in_row;
		next = result - x*(*g_ranks) - (*f)*curr + norm1*curr;
		*Au = next;
		Au++;
		g_ranks++;
		f++;
		u++;
	}
	Au = Au_start;
	u = u_start;
	numerator = mult_vectors_double(u, Au, ng);
	denominator = mult_vectors_double(u, u, ng);
	lamda = numerator/denominator;
	return (lamda - norm1);
}


double calc_deltaQ(spmat* Ag, int* result, int* s, int* g_ranks, int M, double* f)
{
	double x, deltaQ;
	int ng, i;

	ng = Ag->n;
	x = mult_vectors_int(s, g_ranks, ng);
	x /= M;
	Ag->mult_int(Ag, s, result);
	deltaQ = 0;
	for (i = 0; i < ng; ++i) {
		deltaQ += ( (*result - x*(*g_ranks) - (*f)*(*s)) * (*s) );
		result++;
		g_ranks++;
		f++;
		s++;
	}
	return deltaQ;
}

double calc_deltaQ2(spmat* Ag, int* s, int* g_ranks, int M, double* f)
{
	double x, deltaQ;
	int ng, i, result;

	ng = Ag->n;
	x = mult_vectors_int(s, g_ranks, ng);
	x /= M;
	deltaQ = 0;
	for (i = 0; i < ng; ++i) {
		result = array_mult_int2(Ag, s, i); /* ############# change this line to the following,
		and change the allocate function so mult_int is the function array_mult_int2
		Ag->mult_int(Ag, s, i); ### */
		deltaQ += ( (result - x*(*g_ranks) - (*f)*(*s)) * (*s) );
		g_ranks++;
		f++;
		s++;
	}
	return deltaQ;
}

double calc_deltaQ3(spmat* Ag, int* s, int* g_ranks, int M, double* f)
{
	double x, deltaQ;
	int ng, i, result, nnz_in_row;
	int *colind, *rp;
	ArrayMat *arr_mat;

	ng = Ag->n;
	arr_mat = (ArrayMat*) Ag->private;
	rp = arr_mat->rowptr;
	colind = arr_mat->colind;
	x = mult_vectors_int(s, g_ranks, ng);
	x /= M;
	deltaQ = 0;
	for (i = 0; i < ng; ++i) {
		nnz_in_row = *(rp+1)-(*rp);
		rp++;
		result = array_mult_int3(s, colind, nnz_in_row); /* ############# change this line to the following,
		and change the allocate function so mult_int is the function array_mult_int2
		Ag->mult_int(s, colind, nnz_in_row); ### */
		deltaQ += ( (result - x*(*g_ranks) - (*f)*(*s)) * (*s) );
		colind += nnz_in_row;
		g_ranks++;
		f++;
		s++;
	}
	return deltaQ;
}

void create_random_vector(double *b, int len)
{
	int i;

	for (i = 0; i < len; ++i)
	{
		*b = (double) rand();
		b++;
	}
}





