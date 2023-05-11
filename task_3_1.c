#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	int provided;
	MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);

	int rank;
	int size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int cells = 128;
	int nodes = cells / size + 3;

	double totlen = 2.0 * M_PI;
	double loclen = totlen / (double)size;

	double dx = totlen / (double)cells;

	double* f = calloc(nodes, sizeof(double));
	double* dfdx = calloc(nodes, sizeof(double));

	for (int i = 1; i < nodes - 1; ++i)
	{
		f[i] = sin(loclen * rank + (i - 1) * dx);
	}

	int i0 = 0;
	int i1 = 2;
	int i2 = nodes - 3;
	int i3 = nodes - 1;

	int next = rank == (size - 1) ? 0 : rank + 1;
	int prev = rank == 0 ? (size - 1) : rank - 1;

	MPI_Sendrecv(&f[i1], 1, MPI_DOUBLE, prev, 0, &f[i3], 1, MPI_DOUBLE, next, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	MPI_Sendrecv(&f[i2], 1, MPI_DOUBLE, next, 0, &f[i0], 1, MPI_DOUBLE, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	for (int i = 1; i < nodes - 1; ++i)
	{
		dfdx[i] = (f[i + 1] - f[i - 1]) / (2.0 * dx);
	}

	if (rank == 0)
	{
		printf("Rank %d of %d:\n", rank, size);

		for (int i = 0; i < nodes; ++i)
		{
			double sinv = sin(loclen * rank + (i - 1) * dx);
			double cosv = cos(loclen * rank + (i - 1) * dx);
			printf("%+.5f (%.5f) %+.5f (%.5f)\n", f[i], fabs(f[i] - sinv), dfdx[i], fabs(dfdx[i] - cosv));
		}
	}

	MPI_Finalize();

	return 0;
}