// OpenMPApplication.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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

int main_p();

// The original sequential version
int main()
{
	int i;
	double x, pi, sum = 0.0;
	double start, end;

	printf("Num of CPU: %d\n", omp_get_num_procs());
	printf("Max threads: %d\n", omp_get_max_threads());

	start = omp_get_wtime();
	step = 1.0 / (double)num_steps;

	for (i = 0; i < num_steps; i++)
	{
		x = (i + 0.5) * step;
		sum += 4.0 / (1.0 + x * x);
	}

	printf("step = %f and sum = %f\n", step, sum);

	pi = step * sum;
	end = omp_get_wtime();

	printf("pi = %f\n", pi);
	printf("Time used = %f ms\n", (end - start) * 1000);

	printf("\nNow call the parallel version:\n\n");
	main_p();

	return 0;
}

// The OpenMP version
int main_p()
{
	//int i;
	double pi, sum = 0.0;
	double start, end;

	const int num_threads = 4;

	printf("Num of CPU: %d\n", omp_get_num_procs());
	printf("Max threads: %d\n", omp_get_max_threads());
	omp_set_num_threads(num_threads);

	start = omp_get_wtime();

	// The new num_steps and the new step:
	double old_step = 1.0 / (double)num_steps;
	num_steps /= num_threads;
	step = 1.0 / (double)num_steps;

#pragma omp parallel
	{
		int ID = omp_get_thread_num();
		double tmp_sum = 0.0;
		double x;

		for (int i = 0; i < num_steps; i++)
		{
			i += ID * num_steps;
			if (ID > 0) i += 1;

			x = (i + 0.5) * step;
			tmp_sum += 4.0 / (1.0 + x * x);
		}
		printf("ID = %d and tmp_sum = %f\n", ID, tmp_sum);
		sum += tmp_sum;
	}

	printf("step = %f and sum = %f\n", old_step, sum);

	pi = old_step * sum;
	end = omp_get_wtime();

	printf("pi = %f\n", pi);
	printf("Time used = %f ms\n", (end - start) * 1000);

	return 0;
}
