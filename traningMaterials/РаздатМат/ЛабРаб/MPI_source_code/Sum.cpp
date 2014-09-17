#include <stdio.h>
#include "mpi.h"
#include <sys\timeb.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>

/****************************************************************
*                                                               *
*  compute_interval - numerical integration over                *
*                     part of interval                          *
*                                                               *
****************************************************************/

#define N 10000000

double realtime(void);
randomize();

void main(int argc, char ** argv)
{
	FILE *in, *out;
	//	 long intervals;
	 int myrank, ranksize;
	 double send[2],recv[2];
//	 int i, s, kolv;
__int64 i, s, kolv;
	 MPI_Status status;
	 double t1,t2;
	 double t3, t4, t5, t6,t7;
//	 int mas[10]={1,2,3,4,5,6,7,8,9,10};
	 long *mas,di;
	 float rg,lg ;
	 long sum=0;
	 mas=(long *)malloc(sizeof(long)*N);

//	 random();

	in=fopen("in.txt", "rb");
	
	out=fopen("out.txt", "at");

	 t1=realtime();		 
	 MPI_Init (&argc, &argv);       /* initialize MPI system */
	 t2=realtime();
	 MPI_Comm_rank (MPI_COMM_WORLD, &myrank);    /* my place in MPI system */
	 MPI_Comm_size (MPI_COMM_WORLD, &ranksize);  /* size of MPI system */

//	 if (myrank == 0)               /* I am the master */
/*	 {
		printf ("Calculation of PI by numerical Integration\n");
		printf ("Number of intervals: ");
		scanf ("%ld", &intervals);
	 }

*/	 MPI_Barrier (MPI_COMM_WORLD);  /* make sure all MPI tasks are running */
	 if (myrank == 0)               /* I am the master */
	 {
/*
	 out=fopen("m2.txt", "rt");
	 for (i=0;i<N;i++)
		fscanf(out,"%d ", *(mas+i);
	 fclose(out);
*/
/*		 for (i=0;i<N;i++)
		 {  
			*(mas+i)=rand()%10;
			fprintf(out,"%ld ",*(mas+i));
		 }
	     fprintf(out,"\n");
*/		 

		 /* distribute parameter */
	fprintf(out,"Rank=%d N=%d\n",myrank,N);
	printf("Rank=%d N=%d\n",myrank,N);

	fprintf(out,"Read data from file ...\n");
	printf("Read data from file ...\n");

	fread(mas,sizeof(long),N,in);
/*	 for (i=0;i<N;i++)
	 {
		fscanf(in,"%ld ", mas+i);
//		fprintf(out,"%ld ",*(mas+i));
//		printf("%ld ",*(mas+i));
	 }
*/	 fclose(in);


		printf ("Master: Sending # of intervals to MPI-Processes \n");
		fprintf (out,"\nMaster: Sending # of intervals to MPI-Processes \n");
		t3 = MPI_Wtime();
		for (i = 1; i < ranksize; i++)
		{
			kolv=floor(N/ranksize);
			s=	floor(N/ranksize)*i;
//			fprintf(out,"Send %d processu v=%d-%d; kolv=%d, s=%d\n",i,*(mas+s),*(mas+s+kolv), kolv, s);
			
//			MPI_Send (&mas, 5, MPI_LONG, i, 98, MPI_COMM_WORLD);
//		fprintf(out,"Master proc%d send v=%ld\n",myrank,*(mas+s+kolv));
//		printf("Master proc%d send v=%ld\n",myrank,*(mas+s+kolv));

			MPI_Send ((mas + s), kolv, MPI_LONG, i, 98, MPI_COMM_WORLD);
//			MPI_Send (mas + (N/ranksize)*i*sizeof(int), N/ranksize, MPI_LONG, i, 98, MPI_COMM_WORLD);
			}
	 }
	 else{	/* I am a slave */
	 /* receive parameters */
//		fprintf(out,"Slave %d receive parameters\n", myrank);
		s=floor(N/ranksize)*myrank;
		kolv=floor(N/ranksize);
//		fprintf(out,"Index ot %d;  kolv=%d\n", s,kolv);
		MPI_Recv ( (mas+s), kolv, MPI_LONG, 0, 98, MPI_COMM_WORLD, &status);
//		fprintf(out,"Slave proc%d receive v=%ld\n",myrank,*(mas+s+kolv));
//		fprintf(out,"Value: ot %d do %d\n", *(mas+s),*(mas+s+kolv));

//		printf("Slave proc%d receive v=%ld\n",myrank,*(mas+s+kolv));
//		printf("Value: ot %d do %d\n", *(mas+s),*(mas+s+kolv));
//		MPI_Recv ( (&mas), 5, MPI_LONG, 0, 98, MPI_COMM_WORLD, &status);
//		fprintf(out,"Value: ot %d do %d\n", *(mas),*(mas+kolv));

//		 MPI_Recv (mas + (N/ranksize)*myrank*sizeof(int), N/ranksize, MPI_LONG, 0, 98, MPI_COMM_WORLD, &status);
	 }

	 /* compute my portion of interval */
	 t4 = MPI_Wtime();
//	 pi = compute_interval (myrank, ranksize, intervals);
	 fprintf(out,"rank %d\n",myrank);
	 printf("rank %d\n",myrank);
	 sum=0;
	 
	 	 lg=floor(N/ranksize)*myrank;
	 if (myrank+1==ranksize)
		 rg=N-1;
	 else
		 rg=floor(N/ranksize)*myrank+floor(N/ranksize);
	 printf("N=%d\n", rg-lg);
	 fprintf(out,"N=%d\n", rg-lg);
//	 for (i=(N/ranksize)*myrank;i<(N/ranksize)*myrank+(N/ranksize); i++)
	 for (i=lg;i<rg; i++)
	 {
//		fprintf(out,"sum mas_index=%d el=%ld\n",i, *(mas+i));
//		printf("sum mas_index=%d el=%ld\n",i, *(mas+i));
		sum=sum+ *(mas+i);
	 }
	 if (myrank==0)
		sum=sum+*(mas+N-1);
	 free(mas);
	 t5 = MPI_Wtime();
	 MPI_Barrier (MPI_COMM_WORLD);
	 t6 = MPI_Wtime();

	 if (myrank == 0)	/* I am the master */
	 /* collect results, add up, and print results */
	 {
		for (i = 1; i < ranksize; i++)
		{
			MPI_Recv (&di, 1, MPI_LONG, i, 99, MPI_COMM_WORLD, &status);
			sum += di;
		}
		fprintf(out,"\nSum=%ld\n", sum);
		printf("\nSum=%ld\n", sum);

	    t7 = MPI_Wtime();
		printf ("Master: Has collected sum from MPI-Processes \n");
		fprintf (out,"Master: Has collected sum from MPI-Processes \n");


//		printf ("\nPi estimation: %.20lf\n", pi);
		printf ("%ld tasks used - Execution time: %.3lf sec\n",ranksize, t7 -t3);
		fprintf (out,"%ld tasks used - Execution time: %.3lf sec\n",ranksize, t7 -t3);

		fprintf(out,"\nStatistics:\n");
		fprintf(out,"Master: startup: %.0lf msec\n",t2-t1);
		fprintf(out,"Master: time to send # of intervals:%.3lf sec\n",t4-t3);
		fprintf(out,"Master: waiting time for sincro after calculation:%.2lf sec\n",t6-t5);
		fprintf(out,"Master: time to collect: %.3lf sec\n",t7-t6);
		fprintf(out,"Master: calculation time:%.3lf sec\n",t5-t4);

		printf("\nStatistics:\n");
		printf("Master: startup: %.0lf msec\n",t2-t1);
		printf("Master: time to send # of intervals:%.3lf sec\n",t4-t3);
		printf("Master: waiting time for sincro after calculation:%.2lf sec\n",t6-t5);
		printf("Master: time to collect: %.3lf sec\n",t7-t6);
		printf("Master: calculation time:%.3lf sec\n",t5-t4);

		MPI_Barrier (MPI_COMM_WORLD);
		/* collect there calculation time */
		for (i = 1; i < ranksize; i++){
			MPI_Recv (recv, 2, MPI_DOUBLE, i, 100, MPI_COMM_WORLD, &status);
			fprintf(out,"process %d: calculation time: %.3lf sec\twaiting time for sincro.: %.3lf sec\n",i,recv[0],recv[1]);
			printf("process %d: calculation time: %.3lf sec\twaiting time for sincro.: %.3lf sec\n",i,recv[0],recv[1]);
		}
	 }
	 else{	/* I am a slave */
	 /* send my result back to master */
		  MPI_Send (&sum, 1, MPI_LONG, 0, 99, MPI_COMM_WORLD);

		  MPI_Barrier (MPI_COMM_WORLD);
		  send[0]=t5-t4;
		  send[1]=t6-t5;
		  MPI_Send (send, 2, MPI_DOUBLE, 0, 100, MPI_COMM_WORLD);

	 }
			 fclose(out);

	 MPI_Finalize ();

}

double realtime()			     /* returns time in seconds */
{
	struct _timeb tp;
	_ftime(&tp);
	return((double)(tp.time)*1000+(double)(tp.millitm));
}
