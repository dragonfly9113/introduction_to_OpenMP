/*
**  PROGRAM: Mandelbrot area
**
**  PURPOSE: Program to compute the area of a  Mandelbrot set.
**           Correct answer should be around 1.510659.
**           WARNING: this program may contain errors
**
**  USAGE:   Program runs without input ... just run the executable
**
**  HISTORY: Written:  (Mark Bull, August 2011).
**           Changed "comples" to "d_comples" to avoid collsion with
**           math.h complex type (Tim Mattson, September 2011)
*/

#include "pch.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define NUM_THREADS 8

# define NPOINTS 1000
# define MAXITER 1000

void testpoint(struct d_complex);

struct d_complex {
	double r;
	double i;
};

struct d_complex c;
int numoutside = 0;

int main_Mandel() {
	int i, j;
	double area, error, eps = 1.0e-5;

	int nthreads;

	printf("Num of CPU: %d\n", omp_get_num_procs());
	printf("Max threads: %d\n", omp_get_max_threads());

	omp_set_num_threads(NUM_THREADS);

	//   Loop over grid of points in the complex plane which contains the Mandelbrot set,
	//   testing each point to see whether it is inside or outside the set.

//#pragma omp parallel for default(shared) private(c,j) firstprivate(eps)
#pragma omp parallel default(shared)
	{
		int id, nthrds;

		id = omp_get_thread_num();
		nthrds = omp_get_num_threads();
		if (id == 0) nthreads = nthrds;

#pragma omp for private(c,j) firstprivate(eps)
		for (int i = 0; i < NPOINTS; i++) {
			for (int j = 0; j < NPOINTS; j++) {
				c.r = -2.0 + 2.5*(double)(i) / (double)(NPOINTS)+eps;
				c.i = 1.125*(double)(j) / (double)(NPOINTS)+eps;
				testpoint(c);
			}
		}
	}

	// Calculate area of set and error estimate and output the results

	area = 2.0*2.5*1.125*(double)(NPOINTS*NPOINTS - numoutside) / (double)(NPOINTS*NPOINTS);
	error = area / (double)NPOINTS;

	printf("nthreads = %d\n", nthreads);
	printf("Area of Mandlebrot set = %12.8f +/- %12.8f\n", area, error);
}

void testpoint(struct d_complex c) {

	// Does the iteration z=z*z+c, until |z| > 2 when point is known to be outside set
	// If loop count reaches MAXITER, point is considered to be inside the set

	struct d_complex z;
	int iter;
	double temp;

	z = c;
	for (iter = 0; iter < MAXITER; iter++) {
		temp = (z.r*z.r) - (z.i*z.i) + c.r;
		z.i = z.r*z.i * 2 + c.i;
		z.r = temp;
		if ((z.r*z.r + z.i*z.i) > 4.0) {
#pragma omp atomic
			numoutside++;
			break;
		}
	}

}