#include "pch.h"

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#ifndef N
#define N 5
#endif
#ifndef FS
#define FS 38
#endif

struct node {
	int data;
	int fibdata;
	struct node* next;
};

int fib(int n) {
	int x, y;
	if (n < 2) {
		return (n);
	}
	else {
		x = fib(n - 1);
		y = fib(n - 2);
		return (x + y);
	}
}

void processwork(struct node* p)
{
	int n;
	n = p->data;
	p->fibdata = fib(n);
}

struct node* init_list(struct node* p) {
	int i;
	struct node* head = NULL;
	struct node* temp = NULL;

	head = (struct node*)malloc(sizeof(struct node));
	p = head;
	p->data = FS;
	p->fibdata = 0;
	for (i = 0; i < N; i++) {
		temp = (struct node*)malloc(sizeof(struct node));
		p->next = temp;
		p = temp;
		p->data = FS + i + 1;
		p->fibdata = i + 1;
	}
	p->next = NULL;
	return head;
}

int main_linked(int argc, char *argv[]) {
	double start, end;
	struct node *p = NULL;
	struct node *temp = NULL;
	struct node *head = NULL;

	printf("Process linked list\n");
	printf("  Each linked list node will be processed by function 'processwork()'\n");
	printf("  Each ll node will compute %d fibonacci numbers beginning with %d\n", N, FS);

	p = init_list(p);
	head = p;

	start = omp_get_wtime();

// Original non-parallel version
#if 0
	while (p != NULL) {
		processwork(p);
		p = p->next;
	}
#endif

// Parallel version #1:
#if 0
	int count = 0;
	while (p != NULL) {
		p = p->next;
		count++;
	}
	p = head;
	struct node *parr[N];
	for (int i = 0; i < count; i++) {
		parr[i] = p;
		p = p->next;
	}
#pragma omp parallel for schedule(static, 1)
	for (int i = 0; i < count; i++) {
		processwork(parr[i]);
	}
#endif

// Parallel version #2: use task contruct
#pragma omp parallel
	{
#pragma omp single
		{
			//struct node *p = head;
			while (p) {
#pragma omp task firstprivate(p)
				processwork(p);
				p = p->next;
			}
		}
	}

	end = omp_get_wtime();
	p = head;
	while (p != NULL) {
		printf("%d : %d\n", p->data, p->fibdata);
		temp = p->next;
		free(p);
		p = temp;
	}
	free(p);

	printf("Compute Time: %f seconds\n", end - start);

	return 0;
}
