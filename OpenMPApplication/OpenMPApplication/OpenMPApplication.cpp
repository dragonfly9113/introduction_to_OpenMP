// OpenMPApplication.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <omp.h>

// Exercise 1B: Hello world
int main()
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
	}

	printf("There are %d threads.\n", omp_get_num_threads());

	return 0;
}

// Module 3: creating threads
/*
int main()
{
	double A[1000];
	omp_set_num_threads(4);

	#pragma omp parallel
	{
		int ID = omp_get_thread_num();
		pooh(ID, A);
	}

	printf("all done\n");
}
*/

// Exercise 2: Calculate Pi
// Some useful omp functions:
// int omp_get_num_threads();  // get number of threads in the team
// int omp_get_thread_num();   // get the Thread ID or rank
// double omp_get_wtime();     // time in seconds since a fixed point in the past

static long num_steps = 100000;
double step;

int main_pi()
{
	int i;
	double x, pi, sum = 0.0;

	step = 1.0 / (double)num_steps;

	for (i = 0; i < num_steps; i++)
	{
		x = (i + 0.5) * step;
		sum += 4.0 / (1.0 + x * x);
	}

	pi = step * sum;

	printf("pi = %f\n", pi);

	return 0;
}