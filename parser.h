/**
 * parser summary:
 *
 * This module handles files. It parses the formatted input file and creates the initial
 * adjacency matrix A, represented as a sparse matrix.
 * Also it creates the output file formatted as requested.
 *
 * create_A      - Creates initial adjacency matrix A according to the input file, saved as a sparse matrix.
 * get_ranks     - Returns a pointer to an array with the ranks of the nodes in A.
 * get_nnz       - Returns the number of non-zero elements in the matrix A.
 * output_groups - Creates the formatted output file according to the results of the program.
 *
 */

#ifndef PARSER_H_
#define PARSER_H_

#include "spmat.h"
#include "stack.h"
#include <sys/types.h>


/* Receives the name of the input file filename, parses the file by its format,
 * and creates a sparse adjacency matrix A according to the information in the file.
 * Returns a pointer to A. */
spmat* create_A(char* filename);

/* Returns a pointer to the pre-allocaed array that contains the number of neighbors (ranks)
 * of the nodes in the matrix A. The array is filled during the function create_A.
 * ranks[i] is the rank of the node i. */
int* get_ranks();

/* Returns the number of non-zero elements in the matrix A.
 * This value is calculated during the function create_A and saved. */
off_t get_nnz();

/* Receives a stack of groups, outputs them in ascending order to the output file named filename,
 * and frees the allocated memory of the stack and groups.
 * arr is an array of length n, the nodes of the groups are saved there before they are written to the file. */
void output_groups(char* filename, stack* stk, int* arr);


#endif /* PARSER_H_ */
