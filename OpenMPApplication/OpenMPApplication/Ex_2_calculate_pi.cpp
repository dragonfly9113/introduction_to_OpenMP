#include "pch.h"
#include <iostream>
#include <omp.h>

// Exercise 2: Calculate Pi
// Some useful omp functions:
// int omp_get_num_threads();  // get number of threads in the team
// int omp_get_thread_num();   // get the Thread ID or rank
// double omp_get_wtime();     // time in seconds since a fixed point in the past

static long num_steps = 1000000000;
double step;

// The original sequential version
int main_orig()
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

	pi = step * sum;
	end = omp_get_wtime();

	printf("pi = %f\n", pi);
	printf("Time used = %f s\n", end - start);

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
	printf("Time used = %f s\n", (end - start));

	//printf("\nNow call the parallel version:\n\n");
	//main_p();

	return 0;
}

// My OpenMP version
int main_my_openmp()
{
	int i, nthreads;
	double pi;
	double start_time, end_time;
	double total = 0.0;
	const int num_threads = 8;

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
		int ID, nthrds;
		long start, end;

		ID = omp_get_thread_num();
		nthrds = omp_get_num_threads();
		if (ID == 0) nthreads = nthrds;

		start = ID * num_steps_new;
		end = (ID + 1) * num_steps_new;

		for (int i = start; i < end; i++)
		{
			x = (i + 0.5) * step;
			sum += 4.0 / (1.0 + x * x);
		}

		total += sum;
	}

	pi = step * total;
	end_time = omp_get_wtime();

	printf("pi = %f\n", pi);
	printf("nthreads = %d and Time used = %f s\n", nthreads, end_time - start_time);

	return 0;
}

// First version from the class
#define NUM_THREADS 8
void main_first_ver_from_class()
{
	int i, nthreads;
	double pi, sum[NUM_THREADS], start_time, end_time;

	start_time = omp_get_wtime();

	step = 1.0 / (double)num_steps;
	omp_set_num_threads(NUM_THREADS);

#pragma omp parallel
	{
		int i, id, nthrds;
		double x;

		id = omp_get_thread_num();
		nthrds = omp_get_num_threads();
		if (id == 0) nthreads = nthrds;

		for (i = id, sum[id] = 0.0; i < num_steps; i += nthrds)
		{
			x = (i + 0.5) * step;
			sum[id] += 4.0 / (1.0 + x * x);
		}
	}

	for (i = 0, pi = 0.0; i < nthreads; i++)
		pi += sum[i] * step;

	end_time = omp_get_wtime();

	printf("pi = %f\n", pi);
	printf("nthreads = %d and Time used = %f s\n", nthreads, end_time - start_time);
}

// Second version from the class: add a pad to avoid "false sharing"
#define PAD 8	// Assume 64 byte L1 cache line size, remember sizeof(double) = 8
void main_add_padding()
{
	int i, nthreads;
	double pi, sum[NUM_THREADS][PAD], start_time, end_time;

	start_time = omp_get_wtime();

	step = 1.0 / (double)num_steps;
	omp_set_num_threads(NUM_THREADS);

#pragma omp parallel
	{
		int i, id, nthrds;
		double x;

		id = omp_get_thread_num();
		nthrds = omp_get_num_threads();
		if (id == 0) nthreads = nthrds;

		for (i = id, sum[id][0] = 0.0; i < num_steps; i += nthrds)
		{
			x = (i + 0.5) * step;
			sum[id][0] += 4.0 / (1.0 + x * x);
		}
	}

	for (i = 0, pi = 0.0; i < nthreads; i++)
		pi += sum[i][0] * step;

	end_time = omp_get_wtime();

	printf("pi = %f\n", pi);
	printf("nthreads = %d and Time used = %f s\n", nthreads, end_time - start_time);

	printf("size of double = %d\n", sizeof(double));
}
