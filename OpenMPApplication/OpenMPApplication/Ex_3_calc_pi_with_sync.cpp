#include "pch.h"
#include <iostream>
#include <omp.h>

// Exercise 3: Calculate Pi with synchronization

#define NUM_THREADS 8

// Calculate pi using synchronization
void main()
{
	int i, nthreads;
	double pi, start_time, end_time, total = 0.0;

	const long num_steps = 1000000000;
	double step;

	start_time = omp_get_wtime();

	step = 1.0 / (double)num_steps;
	omp_set_num_threads(NUM_THREADS);

#pragma omp parallel
	{
		int i, id, nthrds;
		double x, sum = 0.0;

		id = omp_get_thread_num();
		nthrds = omp_get_num_threads();
		if (id == 0) nthreads = nthrds;

		for (i = id; i < num_steps; i += nthrds)
		{
			x = (i + 0.5) * step;
			sum += 4.0 / (1.0 + x * x);
		}
#pragma omp atomic
		total += sum;
	}

	pi = total * step;
	end_time = omp_get_wtime();

	printf("pi = %f\n", pi);
	printf("nthreads = %d and Time used = %f s\n", nthreads, end_time - start_time);
}
