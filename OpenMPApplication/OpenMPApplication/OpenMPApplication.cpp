// OpenMPApplication.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <omp.h>

int main()
{
	#pragma omp parallel
	{
		int ID = omp_get_thread_num();
		printf("hello(%d)", ID);
		printf(" world(%d) \n", ID);
	}
}
