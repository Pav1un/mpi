#include <stdio.h>
#include "mpi.h"
#include <sys\timeb.h>
//#include <alloc.h>
#include <math.h>
#include <conio.h>
#include <stdlib.h>

#define N 10
#define K 100
randomize();

double realtime(void);

void main(int argc, char ** argv)
{

        FILE *out;
//	 long intervals;
	 int rank, size;
	 double send[2],recv[2];
	
	 MPI_Status status;
	 double t1,t2;
	 double t3, t4, t5, t6,t7;
		int i, j, lg, pg, k, temp, kolvo, mas[N], n;
//        int *mas = new int;
        int *res;

   
	 t1=realtime();		 
	 MPI_Init (&argc, &argv);       /* initialize MPI system */
	 t2=realtime();
	 MPI_Comm_rank (MPI_COMM_WORLD, &rank);    /* my place in MPI system */
	 MPI_Comm_size (MPI_COMM_WORLD, &size);  /* size of MPI system */

/*	 if (myrank == 0)               /* I am the master */
/*	 {
		printf ("Calculation of PI by numerical Integration\n");
		printf ("Number of intervals: ");
		scanf ("%ld", &intervals);
	 }
*/
	 MPI_Barrier (MPI_COMM_WORLD);  /* make sure all MPI tasks are running */
	 if (rank == 0)               /* I am the master */
	 {
		out=fopen("res.txt", "wt");
		printf("Generation matrix\n");
        for (i=0;i<N;i++)
        {
                mas[i]=rand()%K;
                fprintf(out,"%d ", mas[i]);
        }
		/* distribute parameter */
		printf ("Master: Sending # of intervals to MPI-Processes \n");
		t3 = MPI_Wtime();
		for (i = 1; i < size; i++)
		{
				MPI_Send (mas, N, MPI_INT, i, 98, MPI_COMM_WORLD);
			}
	 }
	 else{	/* I am a slave */
	 /* receive parameters */
		  MPI_Recv (mas, N, MPI_INT, 0, 98, MPI_COMM_WORLD, &status);
	 }

	 /* compute my portion of interval */
	 t4 = MPI_Wtime();

        if (rank+1==size)
                pg=K;
        else
                pg=rank*floor(K/size)+floor(K/size);
        lg=rank*floor(K/size);
        printf("\n%d proc get %d-%d:\n", rank, lg, pg);

        res=(int *) malloc(sizeof(int));
        for (i=0, j=0;i<N;i++)
                if (mas[i]>lg && mas[i]<pg)
                {
                        printf("%d ", mas[i]);
                        res=(int *) realloc(res,sizeof(int));
                        *(res+j) = mas[i];
                        j++;
                }
        kolvo=j;
        for (i=0;i<=kolvo;i++)
                for (j=0;j<=kolvo;j++)
                        if (*(res+i)<*(res+j))
                        {
                                temp=*(res+i);
                                *(res+i)=*(res+j);
                                *(res+j)=temp;
                        }

        printf("\n");
        for (i=0;i<=kolvo;i++)
                printf("%d ",*(res+i));

	 t5 = MPI_Wtime();
	 MPI_Barrier (MPI_COMM_WORLD);
	 t6 = MPI_Wtime();

	 if (rank == 0)	/* I am the master */
	 /* collect results, add up, and print results */
	 {
		for (j=0;j<=kolvo;j++)
			mas[j]=*(res+j);

		for (i = 1; i < size; i++)
		{
			MPI_Recv (res, kolvo, MPI_INT, i, 99, MPI_COMM_WORLD, &status);
			MPI_Recv (&kolvo, 1, MPI_INT, i, 97, MPI_COMM_WORLD, &status);
			n=j+1;
			for (j=0;j<=kolvo;j++)
				mas[n+j]=*(res+j);
		}
		t7 = MPI_Wtime();
		printf ("Master: Has collected result from MPI-Processes \n");
		fprintf(out,"\n\n");
		
		for (i=0;i<N;i++)
			fprintf(out,"%d ",mas[i]);

		printf ("\nResult file gotov");
		
		printf ("%ld tasks used - Execution time: %.3lf sec\n",size, t7 -t3);

		printf("\nStatistics:\n");
		printf("Master: startup: %.0lf msec\n",t2-t1);
		printf("Master: time to send # of intervals:%.3lf sec\n",t4-t3);
		printf("Master: waiting time for sincro after calculation:%.2lf sec\n",t6-t5);
		printf("Master: time to collect: %.3lf sec\n",t7-t6);
		printf("Master: calculation time:%.3lf sec\n",t5-t4);

		MPI_Barrier (MPI_COMM_WORLD);
		/* collect there calculation time */
		for (i = 1; i < size; i++){
			MPI_Recv (recv, 2, MPI_DOUBLE, i, 100, MPI_COMM_WORLD, &status);
			printf("process %d: calculation time: %.3lf sec\twaiting time for sincro.: %.3lf sec\n",i,recv[0],recv[1]);
		}
	 }
	 else{	/* I am a slave */
	 /* send my result back to master */
		  MPI_Send (res, kolvo, MPI_INT, 0, 99, MPI_COMM_WORLD);
		  MPI_Send (&kolvo, 1, MPI_INT, 0, 97, MPI_COMM_WORLD);

		  MPI_Barrier (MPI_COMM_WORLD);
		  send[0]=t5-t4;
		  send[1]=t6-t5;
		  MPI_Send (send, 2, MPI_DOUBLE, 0, 100, MPI_COMM_WORLD);

	 }

	 MPI_Finalize ();
}

double realtime()			     /* returns time in seconds */
{
	struct _timeb tp;
	_ftime(&tp);
	return((double)(tp.time)*1000+(double)(tp.millitm));
}

