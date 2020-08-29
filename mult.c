/*
 * mult.c
 *
 *  Created on: 29 באוג 2020
 *      Author: רוני
 */

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
double mult_vectors_int_and_double(double* vec1, double* vec2, int n)
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


