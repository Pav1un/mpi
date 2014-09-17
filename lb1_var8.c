#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void *check_memAlloc(unsigned int size);

int main(int argc, char *argv[])
{
	MPI_Init(&argc, &argv);

	MPI_Status status;
	int arrSize,i,rank,size;
	int *pDataArr;

	if(argc < 2)
		arrSize = 100;
	else
		arrSize = atoi(argv[1]);
	
	
    
	pDataArr = (int *) check_memAlloc(arrSize);
	for (i = 0; i < arrSize; i++)
		pDataArr[i] = i + 1;
	

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);	

	//MPI_Barrier(MPI_COMM_WORLD);
	if(rank == 0){
		int small1 = 999999;
		//printf("I'm process № %d\n", rank);
		for(i = 0; i < 50; i++){
			if(*(pDataArr + i) < small1)	
				small1 = *(pDataArr + i);
		}
		MPI_Send(&small1, 1, MPI_INT, 2, 1, MPI_COMM_WORLD);
		printf("small1 =  %d\n", small1);
		MPI_Barrier(MPI_COMM_WORLD);
	}

	printf("\n");
	if (rank == 1){
		//printf("I'm process № %d \n", rank);
		int small2 = 999999;
		for(i = 50; i < arrSize; i++){
			if(*(pDataArr + i) < small2)	
				small2 = *(pDataArr + i);
		}
		MPI_Send(&small2, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
		printf("small2 =%d\n", small2);
		MPI_Barrier(MPI_COMM_WORLD);
	}


//	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 2){
		int s1, s2;
		MPI_Recv(&s1, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(&s2, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
		if(s1 < s2)	
			printf("%d\n", s1);
		else
			printf(" very small%d\n", s2);
		MPI_Barrier(MPI_COMM_WORLD);
		free(pDataArr);
	}
	
	
	MPI_Finalize();
}

void *check_memAlloc(unsigned int size){
	void *ptr;

	ptr = malloc(size);
	if(ptr == NULL){
		perror("Mem allocation error..");
		MPI_Finalize();
		exit(1);
	}
	
	return ptr;
}
