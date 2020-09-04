#define IS_POSITIVE(X) ((X) > 0.00001)

double calc_score(int* s, spmat* Ag, int* g_ranks, int M)
{

}

void modularity_maximization(int* s, int* unmoved, int* indices, int* g_ranks, spmat* Ag, int M)
{
	double deltaQ, score, max_score, improve, max_improve;
	int ng, i, k, first_move, max_score_index, max_imp_index, j;
	int *unmoved_start, *indices_start;

	unmoved_start = unmoved;
	indices_start = indices;
	ng = Ag->n;

	while (IS_POSITIVE(deltaQ))
	{
		memset(unmoved, 0, sizeof(int)*ng);
		indices = indices_start;
		for (i = 0; i < ng; ++i) {
			first_move = 1;
			for (k = 0; k < ng; ++k) {
				if(*unmoved == 0) {
					score = calc_score(s, Ag, g_ranks, M);
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
			unmoved[max_score_index] = 1;
		}
		/* end of loop */
		for (i = (max_imp_index+1); i < ng; ++i) {
			j = indices[i];
			s[j] = -s[j];
		}
		if (max_imp_index == (ng-1)) {
			deltaQ = 0;
		}
		else {
			deltaQ = max_improve;
		}
	}
}

