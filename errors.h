/**
 * errors summary:
 *
 * A module for handling errors that may occur throughout the program.
 * When an error occurs, a suitable message is printed out and the program run is
 * terminated with the return value 1.
 *
 * check_alloc            - Checks if a memory allocation succeeded.
 * check_fread            - Checks if the function fread successfully read the expected number of elements
 *                          from a file.
 * check_fwrite           - Checks if the function fwrite successfully wrote the expected number of elements
 *                          to a file.
 * check_fopen            - Checks if the function fopen successfully opened a file.
 * check_M                - Checks if M is not zero.
 * size_error             - Points out that the size of the binary file could not be determined if so.
 * check_devision_by_zero - Checks if there was a division by zero.
 * infinite_loop_error    - Points out that the code got into an infinite loop if so, and terminates it.
 *
 */

#ifndef ERRORS_H_
#define ERRORS_H_

#include <stdio.h>

/* Checks if an allocation succeeded, by checking the value of the pointer ptr
 * that points to the allocated memory. If it is NULL then the allocation failed,
 * the function prints a message and exits the program. */
void check_alloc(void* ptr);

/* Checks if fread successfully read the expected number of elements from a file,
 * by comparing the number of expected and read elements.
 * If they differ, fread fialed, the function prints a message and exits the program. */
void check_fread(int read, int exp);

/* Checks if fwrite successfully wrote the expected number of elements to a file,
 * by comparing the number of expected and written elements.
 * If they differ, fwrite fialed, the function prints a message and exits the program. */
void check_fwrite(int written, int exp);

/* Checks if the function fopen successfully opened a file, by checking the value it retured.
 * If it is NULL, then fopen failed, the function prints a message and exits the program. */
void check_fopen(FILE* input);

/* Checks if the value M is not zero.
 * If it is, the function prints a message and exits the program. */
void check_M(int M);

/* If the size of the binary file named filename could not be determined, this function
 * is called, prints a message and exits the program. */
void size_error(const char* filename);

/* Checks if there was a division by zero, by checking if the value by which we divide (val)
 * equals zero (is between 0.00001 and -0.00001) */
void check_devision_by_zero(double val);

/* If the code got into an infinite loop, this function is called,
 * prints a message and exits the program. */
void infinite_loop_error();


#endif /* ERRORS_H_ */
