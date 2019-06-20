#include "pch.h"
#include <iostream>
#include <omp.h>

// Exercise 1B: Hello world
int main_hello()
{
	printf("Num of CPU: %d\n", omp_get_num_procs());
	printf("Max threads: %d\n", omp_get_max_threads());

	//omp_set_num_threads(4);

	printf("There are %d threads.\n", omp_get_num_threads());

	#pragma omp parallel
	{
		int ID = omp_get_thread_num();
		printf("hello(%d)", ID);
		printf(" world(%d) \n", ID);
		printf("There are %d threads.\n", omp_get_num_threads());
	}

	printf("There are %d threads.\n", omp_get_num_threads());

	return 0;
}
