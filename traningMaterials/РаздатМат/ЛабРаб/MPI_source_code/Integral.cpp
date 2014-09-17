#include <stdio.h>
#include <math.h>
#include "mpi.h"

/* определение функции */
//double realtime(void);

double F (double x)

{

double c = x*x*x;

return c;

}


int main (int argc, char **argv)

{

/* объ€вление рабочих переменных */

double sum, s, dx, a, b, x0, x1;

int i;
float n;
	 double t1,t2;
	 double t3, t4, t5, t6,t7;

FILE *out;

		out=fopen("out.txt", "at");
/* объ€вление переменных дл€ MPI */

int ierror, myid, numproc;

MPI_Status status;


/* инициализаци€ параллельной части приложени€ */
// t1=realtime();		 

ierror = MPI_Init (&argc, &argv);
 //t2=realtime();

if (ierror != 0)

{

printf ("MPI initialization error: %d", ierror);

return 0;

}

/* определение числа параллельных процессов в группе */

MPI_Comm_size (MPI_COMM_WORLD, &numproc);


/* определение идентификатора процесса */

MPI_Comm_rank (MPI_COMM_WORLD, &myid);

/* определение параметров интегрировани€ */

a = 0.0;

b = 2.5;

if (myid==0)
{
		printf ("Calculation of integral funtion f(x)=x^3 ot %.3lf do %.3lf:\n", a,b);
		printf ("Number of intervals: ");
		fprintf (out,"\n\nCalculation of integral funtion f(x)=x^3 ot %.3lf do %.3lf:\n", a,b);
		fprintf (out,"Number of intervals: ");
		scanf ("%f", &n);
		fprintf(out, "%f\n",n);
//		t3 = MPI_Wtime();
}

	 MPI_Barrier (MPI_COMM_WORLD);  /* make sure all MPI tasks are running */
	 if (myid == 0)               /* I am the master */
	 {
		/* distribute parameter */
		printf ("Master: Sending # of intervals to MPI-Processes \n");
		fprintf (out,"Master: Sending # of intervals to MPI-Processes \n");
		t3 = MPI_Wtime();
		for (i = 1; i < numproc; i++)
		{
				MPI_Send (&n, 1, MPI_FLOAT, i, 98, MPI_COMM_WORLD);
			}
	 }
	 else{	/* I am a slave */
	 /* receive parameters */
		  MPI_Recv (&n, 1, MPI_FLOAT, 0, 98, MPI_COMM_WORLD, &status);
	 }


//n = 20;

//dx = (b - a)/n/numproc;
dx = (b - a)/n;

/* вычисление интеграла */

sum = 0.0;

//x0 = a + dx*n*myid;

//for (i = 0; i < n; i++)
for (i = myid; i < n; i+=numproc)

{
x0=a+i*dx;

x1 = x0 + dx;

//sum += (F(x0) + F(x1))*dx/2.0;
sum += dx*F(x0)+abs(F(x0) - F(x1))*dx/2.0;

//x0 = x1;

}


/* отправка результата всеми процессами */

if (myid > 0)

{

MPI_Send(&sum, 1, MPI_DOUBLE, 0, 000+myid, MPI_COMM_WORLD);

}


/* получение результатов процессом 0 и суммирование */

if (myid == 0)

{

s = sum;

for (i = 0; i < numproc-1; i++)

{

MPI_Recv(&sum, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

s += sum;

}
t7 = MPI_Wtime(); 

	printf ("%lf \n", s);
	fprintf (out,"%lf \n", s);
	printf ("%ld tasks used - Execution time: %.6lf sec\n",numproc, t7-t3);
	fprintf (out,"%ld tasks used - Execution time: %.6lf sec\n",numproc, t7 -t3);
}


MPI_Finalize();

return 0;
}

//double realtime()			     /* returns time in seconds */
/*{
	struct _timeb tp;
	_ftime(&tp);
	return((double)(tp.time)*1000+(double)(tp.millitm));
}
*/
