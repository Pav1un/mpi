#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int numprocs, p, i, rank, r = 1, result;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

#ifdef  INTERACTIVE
  if (!rank)
    fscanf(stdin, "%d", &p);
#else
  p = 31;
#endif

  MPI_Bcast(&p, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (p != 0 && rank < p / 2 - 1)
    for (i = 2 + rank * (p / 2 - 1) / numprocs;
         i < 2 + (rank + 1) * (p / 2 - 1) / numprocs;
         i++)
      {
        if (p % i == 0) {
          r = 0;
          break;
        }
      }

  MPI_Reduce(&r, &result, 1, MPI_INT, MPI_LAND, 0, MPI_COMM_WORLD);

  if (rank == 0)
    printf("%d is %sa prime.\n", p, result == 0 ? "not " : "");

  MPI_Finalize();
  return 0;
}
