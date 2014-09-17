#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <time.h>


void *check_memAlloc(int size);

int main(int argc, char *argv[])
{
	MPI_Init(&argc, &argv);

	int rank, size;
	int memSize;
	int bigInt = 999999;
	int part1, part2;
	MPI_Status status;

	if (argc < 2)
		memSize = 20;
	else
		memSize = atoi(argv[1]);


	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);	

	if(rank == 0){
		int *pDataArr;
		int i;
		int res1, res2;

		pDataArr = (int *)check_memAlloc(memSize);
		srand(time(0));

		printf("%s\n", "Array have next value: \n");
		for(i=0; i<memSize; i++){
			pDataArr[i] = rand()%100;
			printf("%d\n", *(pDataArr + i));
		}

		if(memSize % 2 == 0) {
			part1 = memSize/2;
			part2 = memSize/2;
		}
		else {
			part1 = memSize/2;
			part2 = memSize/2 + 1;
		}

		printf("part1:  %d part2 : %d\n",part1, part2);
		MPI_Send(pDataArr, part1, MPI_INT, 1, 1, MPI_COMM_WORLD);
		MPI_Send((pDataArr + part1),part2, MPI_INT, 2, 2, MPI_COMM_WORLD);
		free(pDataArr);

		MPI_Recv(&res1, 1, MPI_INT, 1, 3, MPI_COMM_WORLD, &status);
		MPI_Recv(&res2, 1, MPI_INT, 2, 4, MPI_COMM_WORLD, &status);

		if(res1 < res2)
			printf("Smallest number: %d\n", res1);
		else
			printf("Smallest number: %d\n", res2);

	}
	
	if(rank == 1){
		int *partArray1;
		int count, i;

		MPI_Probe(0, 1, MPI_COMM_WORLD, &status );
		MPI_Get_count(&status, MPI_INT, &count);

		partArray1 = (int *)check_memAlloc(sizeof(int ) * count);

		MPI_Recv(partArray1, count, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

		printf("%s\n", "I'm process 1 \n");
		for(i = 0; i < count; i++){
			if(*(partArray1 + i) < bigInt )
				bigInt = *(partArray1 + i);
			printf("index is : %d,  value is : %d\n",i, *(partArray1 + i) );
		}

		MPI_Send(&bigInt, 1, MPI_INT, 0, 3, MPI_COMM_WORLD);

		free(partArray1);

	}

	if(rank == 2) {
		int *partArray2;
		int count, i;

		MPI_Probe(0, 2, MPI_COMM_WORLD, &status );
		MPI_Get_count(&status, MPI_INT, &count);

		partArray2 = (int *)check_memAlloc(sizeof(int ) * count);

		MPI_Recv(partArray2, count, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
		
		printf("%s\n", "I'm process 2 \n");
		for(i = 0; i < count; i++){
			if(*(partArray2 + i) < bigInt )
				bigInt = *(partArray2 + i);
			printf("index is : %d,  value is : %d\n",i, *(partArray2 + i) );
		}

		MPI_Send(&bigInt, 1, MPI_INT, 0, 4, MPI_COMM_WORLD);

		free(partArray2);

	}

	MPI_Finalize();

}

void *check_memAlloc(int size){
	void *ptr;

	ptr = malloc(size*sizeof(int));
	if(ptr == NULL){
		perror("Error memory alloc..");
		MPI_Finalize();
		exit(1);
	}

	return ptr;
}