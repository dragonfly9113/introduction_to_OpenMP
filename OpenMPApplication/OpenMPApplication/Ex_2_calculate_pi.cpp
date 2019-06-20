#include "pch.h"
#include <iostream>
#include <omp.h>

// Exercise 2: Calculate Pi
// Some useful omp functions:
// int omp_get_num_threads();  // get number of threads in the team
// int omp_get_thread_num();   // get the Thread ID or rank
// double omp_get_wtime();     // time in seconds since a fixed point in the past

static long num_steps = 100000;
double step;

// The original sequential version
int main_seq()
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

	return 0;
}

// Test more on the sequential version
int main_seq_2_section()
{
	int i;
	double x, pi, sum = 0.0;
	double start, end;
	double total = 0.0;

	printf("Num of CPU: %d\n", omp_get_num_procs());
	printf("Max threads: %d\n", omp_get_max_threads());

	start = omp_get_wtime();
	step = 1.0 / (double)num_steps;
	// The new num_steps
	long num_steps_new = num_steps / 2;

	for (i = 0; i < num_steps_new; i++)
	{
		x = (i + 0.5) * step;
		sum += 4.0 / (1.0 + x * x);
	}
	total += sum;
	printf("sum = %f and total = %f\n", sum, total);

	sum = 0.0;
	for (i = num_steps_new; i < 2 * num_steps_new; i++)
	{
		x = (i + 0.5) * step;
		sum += 4.0 / (1.0 + x * x);
	}
	total += sum;
	printf("sum = %f and total = %f\n", sum, total);

	pi = step * total;
	end = omp_get_wtime();

	printf("pi = %f\n", pi);
	printf("Time used = %f ms\n", (end - start) * 1000);

	//printf("\nNow call the parallel version:\n\n");
	//main_p();

	return 0;
}

// My OpenMP version
int main()
{
	int i;
	double pi;
	double start_time, end_time;
	double total = 0.0;
	const int num_threads = 4;

	printf("Num of CPU: %d\n", omp_get_num_procs());
	printf("Max threads: %d\n", omp_get_max_threads());
	omp_set_num_threads(num_threads);

	start_time = omp_get_wtime();

	step = 1.0 / (double)num_steps;
	// The new num_steps
	long num_steps_new = num_steps / num_threads;

#pragma omp parallel
	{
		double x, sum = 0.0;
		int ID = omp_get_thread_num();
		long start = ID * num_steps_new, end = (ID + 1) * num_steps_new;

		for (int i = start; i < end; i++)
		{
			x = (i + 0.5) * step;
			sum += 4.0 / (1.0 + x * x);
		}

		total += sum;
		printf("ID = %d and tmp_sum = %f and total = %f\n", ID, sum, total);
	}

	pi = step * total;
	end_time = omp_get_wtime();

	printf("pi = %f\n", pi);
	printf("Time used = %f ms\n", (end_time - start_time) * 1000);

	return 0;
}
