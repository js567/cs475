#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
	int numprocs = omp_get_num_procs();
	printf("%d", numprocs);
	return 0;
}
