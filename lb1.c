#include<stdio.h>
#include<mpi.h>

int main(int argc, char **argv)

{

int size, rank, count;

float a,b,c,d,e;
d = 0;
MPI_Status status;

MPI_Init(&argc, &argv);

MPI_Comm_size(MPI_COMM_WORLD, &size);

MPI_Comm_rank(MPI_COMM_WORLD, &rank);

a=8; b=3;

if (rank==0){
	c=a+b;
	printf("I have rank: %d \t and c contain: %g \n", rank, c);
	printf("I have rank: %d \t and d contain: %g \n", rank, d);
	printf("I have rank: %d \t and a contain: %g \n", rank, a);
	printf("I have rank: %d \t and b contain: %g \n", rank, b);	
	printf(" size  = %d\n",size );
	MPI_Recv(&d, 1,MPI_FLOAT, 1,0,MPI_COMM_WORLD, &status);
	printf("I have rank: %d \t and contain: %g \n", rank, d);
}

if (rank==1){

	d=a/9;
	printf("I have rank: %d \t and contain: %g \n", rank, d);
	printf("I have rank: %d \t and c contain: %g \n", rank, c);
	//Посылка результатов нулевому процессу.
	MPI_Send(&d, 1,MPI_FLOAT, 0,0,MPI_COMM_WORLD);
}

MPI_Finalize();

return 0;

} 