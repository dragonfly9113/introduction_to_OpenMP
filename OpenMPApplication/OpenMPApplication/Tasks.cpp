// Visual Studio only supports up to OpenMP 2.0
// The task feature is added to OpenMP in 3.0
// To compile this program, use MINGW instead.

#include "pch.h"
#include <stdio.h>
#include <omp.h>

int main_tasks(void)
{
#if 0
#pragma omp parallel
	{
		int x = 0;

#pragma omp single
		{
#pragma omp task shared(x)
			{
				x++;
				printf("from task 1: x = %d\n", x);
			}

#pragma omp taskwait

#pragma omp task shared(x)
			{
				x++;
				printf("from task 2: x = %d\n", x);
			}
		}
	}
#endif

// In the following example, one task spawns another task, with the inner task accessing a variable local
// to the outer task. There is a problem with this however, even with only one task modifying the varialbe;
// x exists on the stack of the outer task, and the outer task can finish before the inner task begins.
// Sharing data on the stack like this is not monitored or prevented by the compiler or runtime, and is entirely
// up to the programmer to insert a taskwait where needed.
#pragma omp parallel
	{
#pragma omp single
		{
#pragma omp task
			{
				int x = 0;
#pragma omp task
				{
					x++;
					printf("x = %d\n", x);
				}
			}
		}
	}

	return 0;
}