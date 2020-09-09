#define IS_POSITIVE(X) ((X) > 0.00001)
#include "optimization.h"
#include <string.h>
#include <stdio.h>

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
	/*printf("i = %d\n", i);*/
	for (j = 0; j < ng; ++j) {
		if (j == i) {
			sum += ( (*row - x*(*g_ranks))*(-(*s)) );
		}
		else {
			sum += ( (*row - x*(*g_ranks))*(*s) );
		}
		/*printf("j = %d, *row = %d, *s = %d, D = %f, sum = %f\n", j, *row, *s, x*(*g_ranks), sum);*/
		g_ranks++;
		row++;
		s++;
	}
	score =  4*(-s_start[i])*sum + 4*x*g_ranks_start[i];
	return score;
}

void modularity_maximization(int* s, int* unmoved, int* indices, int* g_ranks, spmat* Ag, int M, int* row)
{
	double deltaQ, score, max_score, improve, max_improve;
	int ng, i, k, first_move, max_score_index, max_imp_index, j, var;
	int *unmoved_start, *indices_start;

	unmoved_start = unmoved;
	indices_start = indices;
	ng = Ag->n;
	deltaQ = 1;

	while (IS_POSITIVE(deltaQ))
	{
		memset(unmoved, 0, sizeof(int)*ng);
		indices = indices_start;
		for (i = 0; i < ng; ++i) {
			first_move = 1;
			for (k = 0; k < ng; ++k) {
				if(*unmoved == 0) {
					score = calc_score(s, Ag, g_ranks, M, k, row);
					if (first_move) {
						first_move = 0;
						max_score = score;   /*initialize*/
						max_score_index = k; /*initialize*/
					}
					else {
						if (score >= max_score) {
							max_score = score;   /*update*/
							max_score_index = k; /*update*/
						}
					}
				}
				unmoved++;
			}
			unmoved = unmoved_start;
			s[max_score_index] = -s[max_score_index];
			printf("s: ");
			for (var = 0; var < ng; ++var) {
				printf("%d ",s[var]);
			}
			printf("\n");
			*indices = max_score_index;
			indices++;
			if (i == 0) {
				improve = max_score;   /*initialize improve*/
				max_improve = improve; /*initialize max_improve*/
				max_imp_index = 0;     /*initialize max_imp_index*/
			}
			else {
				improve += max_score;
				if (improve >= max_improve) {
					max_improve = improve;
					max_imp_index = i;
				}
			}
			printf("i = %d, improve = %f, max_improve = %f, max_imp_index = %d\n",i, improve, max_improve, max_imp_index);
			printf("max_score_index = %d\n",max_score_index);
			unmoved[max_score_index] = 1;
		}
		/* end of loop */
		for (i = (max_imp_index+1); i < ng; ++i) {
			j = indices[i];
			printf("i = %d, j = %d\n",i, j);
			s[j] = -s[j];
		}
		printf("s after move: ");
		for (var = 0; var < ng; ++var) {
			printf("%d ",s[var]);
		}
		printf("\n");
		if (max_imp_index == (ng-1)) {
			deltaQ = 0;
			printf("here\n");
		}
		else {
			deltaQ = max_improve;
		}
		printf("deltaQ = %f\n",deltaQ);
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






