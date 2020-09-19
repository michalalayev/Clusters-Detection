#include <stdlib.h> /* for rand() */
#include <string.h> /* for memset */
#include <math.h>
#include "alg_imp.h"
#include "errors.h"


void g_to_vector(group* g, int* g_vector)
{
	int node_num;
	int i;
	int val;
	int g_len;
	ELEM* curr_node;

	curr_node = g->head;
	g_len = g->len;

	for(i = 0; i < g_len; i++)
	{
		val = i;
	    node_num = curr_node->data;
		if(i == 0)
		{
			val = -1;
		}
		g_vector[node_num] = val;
		curr_node = curr_node->next;
	}
}

/* Fills with zeroes the pre-allocated array row, of length n */
void reset_row(int* row, int n) {

	memset(row, 0, sizeof(int)*n);
}


void create_Ag(spmat* A, spmat* Ag, group* g, int* g_vector)
{
	int data, start, range, end, i, val, cnt;
	ELEM* node;
	ArrayMat *mat, *g_mat;
	int *rp, *colind, *g_rp, *g_colind;

	node = g->head;
	mat = (ArrayMat*) A->private;
	rp = mat->rowptr;
	colind = mat->colind;
	reset_row(g_vector, A->n);
	g_to_vector(g, g_vector);
	g_mat = (ArrayMat*) Ag->private;
	g_rp = g_mat->rowptr;
	g_rp++;
	g_colind = g_mat->colind;
	cnt = 0;

	for ( ; node != NULL; node = node->next){
		data = node->data;
		start = rp[data];
		range = rp[data+1] - start;
		end = start + range;
		for (i = start; i < end; ++i) {
			val = g_vector[colind[i]];
			if (val != 0) {
				if (val == -1) {
					*g_colind = 0;
				}
				else {
					*g_colind = val;
				}
				cnt++;
				g_colind++;
			}
		}
		*g_rp = cnt;
		g_rp++;
	}
}

/* Builds full row of the matrix A from its sparse representation, into the pre-allocated array A_row.
 * row_num is the index of the row in A to build. */
void build_full_row(spmat* A, int* A_row, int row_num)
{
	int start, range, i;
	ArrayMat *mat;
	int *rp, *colind;

	reset_row(A_row, A->n);
	mat = (ArrayMat*) A->private;
	rp = mat->rowptr;
	colind = mat->colind;
	start = rp[row_num];
	colind += start;
	range = rp[row_num+1] - start;
	for (i = 0; i < range; ++i) {
		A_row[*colind] = 1;
		colind++;
	}
}



/*f is a vector of len ng+1, f[ng] = 1-norm */
void calc_f_and_1norm(spmat* A, int* A_row, group* g, int* ranks, int M, double* f)
{
	int g_row, g_col, add;
	double sumf, sum_norm, diag, max;
	ELEM *ptr_row, *ptr_col;

	ptr_row = g->head;
	for (; ptr_row != NULL; ptr_row = ptr_row->next) {
		sumf = 0;
		sum_norm = 0;
		g_row = ptr_row->data;
		build_full_row(A, A_row, g_row);
		ptr_col = g->head;
		for (; ptr_col != NULL; ptr_col = ptr_col->next) {
			g_col = ptr_col->data;
			if (A_row[g_col] == 1) {
				add = M - ranks[g_row]*ranks[g_col];
			}
			else {
				add = -(ranks[g_row]*ranks[g_col]);
			}
			sumf += add;
			if (g_row != g_col) {
				sum_norm += abs(add);
			}
		}
		*f = sumf/M;
		diag = (double) (-ranks[g_row]*ranks[g_row])/M - (*f);
		sum_norm = sum_norm/M + fabs(diag);
		f++;
		if (ptr_row == g->head) {
			max = sum_norm;
		}
		else {
			if (sum_norm > max) {
				max = sum_norm;
			}
		}
	}
	*f = max; /*this is 1-norm of B[g]_hat*/
}


void fill_g_ranks(group* g, int* ranks, int* g_ranks)
{
	ELEM* node;

	node = g->head;
	for (; node != NULL; node = node->next) {
		*g_ranks = ranks[node->data];
		g_ranks++;
	}
}

/* Multiplies two vectors of type int and length n, and returns a scalar */
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

/* Multiplies two vectors of length n, first of type int and second of type double, and returns a scalar */
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

/* Multiplies two vectors of type double and length n, and returns a scalar */
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
	int ng, i, done, cnt, cnt_limit;
	int *g_ranks_start;

	ng = Ag->n;
	norm1 = f[ng];
	b_curr_start = b_curr;
	b_next_start = b_next;
	res_start = result;
	f_start = f;
	g_ranks_start = g_ranks;
	cnt = 0;
	cnt_limit = (int) 0.5*ng*ng + 10000*ng + 300000;
	done = 0; /*false*/

	while (done == 0) {
		done = 1; /*assume it's true*/
		magn = 0;
		Ag->mult_double(Ag, b_curr, result);
		x = mult_vectors_int_and_double(g_ranks, b_curr, ng);
		x = x/M;
		for (i = 0; i < ng; ++i) { /*the i'th iteration calculates the i element in b_next vector*/
			curr = *b_curr;
			next = *result - x*(*g_ranks) - (*f)*curr + norm1*curr;
			magn += (next*next);
			*b_next = next;
			b_next++;
			result++;
			g_ranks++;
			f++;
			b_curr++;
		}
		f = f_start;
		result = res_start;
		g_ranks = g_ranks_start;
		b_next = b_next_start;
		b_curr = b_curr_start;

		/*standartize b_next:*/
		magn = sqrt(magn);
		check_devision_by_zero(magn);
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

		cnt++;
		if(cnt >= cnt_limit) {
			infinite_loop_error();
		}
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
	check_devision_by_zero(denominator);
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


void create_random_vector(double *b, int len)
{
	int i;

	for (i = 0; i < len; ++i)
	{
		*b = (double) rand();
		b++;
	}
}


/*i is the index of the row of Ag, we want to claculate score[i]*/
double calc_score(int* s, spmat* Ag, int* g_ranks, int M, int i, int* row)
{
	double sum, x, score;
	int ng, j;
	int *s_start, *g_ranks_start;

	build_full_row(Ag, row, i);
	ng = Ag->n;
	x = (double) g_ranks[i]/M;
	s_start = s;
	g_ranks_start = g_ranks;

	sum = 0;
	for (j = 0; j < ng; ++j) {
		if (j == i) {
			sum += ( (*row - x*(*g_ranks))*(-(*s)) );
		}
		else {
			sum += ( (*row - x*(*g_ranks))*(*s) );
		}
		g_ranks++;
		row++;
		s++;
	}
	score =  4*(-s_start[i])*sum + 4*x*g_ranks_start[i];
	return score;
}



void initiate_score(spmat* Ag, double* score, int* g_ranks, int* s, int M, int* result)
{
	double a;
	int ng, i, rank;

	ng = Ag->n;
	a = mult_vectors_int(s, g_ranks, ng);
	a /= M;
	Ag->mult_int(Ag, s, result);
	for (i = 0; i < ng; ++i) {
		rank = *g_ranks;
		*score = (-2) * ((*s) * (*result - a*rank) +  (double) (rank*rank)/M );
		score++;
		s++;
		result++;
		g_ranks++;
	}
}


/*k is max_score_index*/
void update_score(spmat* Ag, double* score, int* g_ranks, int* s, int M, int* row, int k)
{
	int ng, sk, i;
	double x;

	build_full_row(Ag, row, k);
	ng = Ag->n;
	sk = s[k];
	x = (double) g_ranks[k]/M;
	for (i = 0; i < ng; ++i) {
		if (i == k) {
			(*score) *= (-1);
		}
		else {
			(*score) -= 4*(*s)*sk*(*row - (*g_ranks)*x);
		}
		score++;
		s++;
		row++;
		g_ranks++;
	}
}

int find_max_score_index(double* score, int* unmoved, int unmoved_len)
{
	int i, max_score_index, first_unmoved;
	double max_score;

	first_unmoved = 1;
	for (i = 0; i < unmoved_len; ++i) {
		if(*unmoved == 0) {
			if (first_unmoved) {
				first_unmoved = 0;
				max_score = *score;   /*initialize*/
				max_score_index = i; /*initialize*/
			}
			else {
				if (*score >= max_score) {
					max_score = *score;   /*update*/
					max_score_index = i; /*update*/
				}
			}
		}
		unmoved++;
		score++;
	}
	return max_score_index;
}


void modularity_maximization(int* s, int* unmoved, int* indices, double* score, int* g_ranks, spmat* Ag, int M, int* row, int* result)
{
	double deltaQ, improve, max_improve;
	int ng, i, max_imp_index, j;
	int *indices_start;

	indices_start = indices;
	ng = Ag->n;
	deltaQ = 1;
	initiate_score(Ag, score, g_ranks, s, M, result);

	while (IS_POSITIVE(deltaQ))
	{
		reset_row(unmoved, ng);

		for (i = 0; i < ng; ++i) {
			if (i != 0) {
				update_score(Ag, score, g_ranks, s, M, row, j);
			}
			j = find_max_score_index(score, unmoved, ng);
			s[j] = -s[j];
			*indices = j;
			indices++;
			if (i == 0) {
				improve = score[j];   /*initialize improve*/
				max_improve = improve; /*initialize max_improve*/
				max_imp_index = 0;     /*initialize max_imp_index*/
			}
			else {
				improve += score[j];
				if (improve >= max_improve) {
					max_improve = improve;
					max_imp_index = i;
				}
			}
			unmoved[j] = 1;
		}
		/* end of for loop */
		update_score(Ag, score, g_ranks, s, M, row, j);
		indices = indices_start;
		for (i = (max_imp_index+1); i < ng; ++i) {
			j = indices[i];
			s[j] = -s[j];
			update_score(Ag, score, g_ranks, s, M, row, j);
		}
		if (max_imp_index == (ng-1)) {
			deltaQ = 0;
		}
		else {
			deltaQ = max_improve;
		}
	}
}


void fill_with_ones(int* s, int len)
{
	int i;

	for (i = 0; i < len; ++i)
	{
		*s = 1;
		s++;
	}
}

void create_s(int* s, double* v, int len)
{
	int i;

	for (i = 0; i < len; ++i) {
		if (IS_POSITIVE(*v)) {
			*s = 1;
		}
		else {
			*s = (-1);
		}
		v++;
		s++;
	}
}

void put_groups_in_stacks(group** splited_g, stack* P, stack* O)
{
	group *g1, *g2;

	g1 = splited_g[0];
	g2 = splited_g[1];


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
	free(splited_g);
}






