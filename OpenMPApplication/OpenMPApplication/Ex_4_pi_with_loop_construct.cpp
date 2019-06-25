#include "pch.h"
#include <iostream>
#include <omp.h>

#define NUM_THREADS 4

void main_ex4()
{
	int i, nthreads;
	double pi, sum = 0.0;
	double start, end;

	long num_steps = 1000000000;
	double step;

	printf("Num of CPU: %d\n", omp_get_num_procs());
	printf("Max threads: %d\n", omp_get_max_threads());

	start = omp_get_wtime();
	step = 1.0 / (double)num_steps;

	omp_set_num_threads(NUM_THREADS);

#pragma omp parallel
	{
		int id, nthrds;
		double x;

		id = omp_get_thread_num();
		nthrds = omp_get_num_threads();
		if (id == 0) nthreads = nthrds;

#pragma omp for reduction(+:sum) schedule(dynamic,10000)
		for (i = 0; i < num_steps; i++)
		{
			x = (i + 0.5) * step;
			sum += 4.0 / (1.0 + x * x);
		}
	}
	pi = step * sum;
	end = omp_get_wtime();

	printf("pi = %f\n", pi);
	printf("nthreads = %d and Time used = %f s\n", nthreads, end - start);
}
