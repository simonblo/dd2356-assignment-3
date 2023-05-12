#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define SEED 921
#define ITERATIONS 1280000000

int main(int argc, char* argv[])
{
	int provided;
	MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);

	int rank;
	int size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	srand(SEED * rank);
	int count = 0;

	double t0 = MPI_Wtime();

	for (int i = 0; i != (ITERATIONS / size); ++i)
	{
		double x = (double)random() / (double)RAND_MAX;
		double y = (double)random() / (double)RAND_MAX;
		count += (sqrt(x * x + y * y) <= 1.0);
	}

	double t1 = MPI_Wtime();

	if (rank == 0)
	{
		int total = count;

		for (int i = 1; i < size; ++i)
		{
			MPI_Recv(&count, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			total += count;
		}

		double pi = 4.0 * ((double)total / (double)ITERATIONS);
		printf("Computed %.10f (%+.10f) in %.9f seconds.\n", pi, pi - M_PI, t1 - t0);
	}

	else
	{
		MPI_Send(&count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();

	return 0;
}