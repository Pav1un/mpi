#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<math.h>
#include "mpi.h"
#include <sys\timeb.h>

// matrix1
#define STR1 10 //sroki

// matrix2
#define STR2 10 //stroki

//Объявление модуля вычисления времени
double realtime(void);

// Функция main
int main(int argc, char **argv)
{

FILE *in,*out;

char name1[15], name2[15];
char key;
int ranksize, rank;
int i,j,i1,j1,j2;
int stl1,str1,str2,stl2;
int m1[STR1];
int m2[STR2];
double res[STR1]; //matrix result

     double send[2],recv[2];
	 double t1,t2;
	 double t3, t4, t5, t6,t7;

out=fopen("out.txt","wt");

if(STR1!=STR2)
{
	printf("\nError in define matrix\n");
	fprintf(out, "Error in define matrix");
	fclose(out);
	return 0;
}
/*
puts("Параллельная программа перемножения матриц");
puts("1 - Создать новые матрицы");
puts("2 - Открыть существующие матрицы");
puts("3 - Выход");
puts("Введите номер для продолжения...");
key=getch();
switch (key)
{
   case '1':
	    printf("Create new two matrix:\n");
	    printf("Input name matrix 1:");
	    scanf("%s", name1);
	    printf("Input number of rows in matrix 1:");
	    scanf("%ld", &str1);
	    printf("Input number of colums in matrix 1:");
	    scanf("%ld", &stl1);
	    printf("\nInput name matrix 2:");
        scanf("%s", name2);
	    printf("Input number of rows in matrix 2:");
	    scanf("%ld", &str2);
	    printf("Input number of colums in matrix 2:");
	    scanf("%ld", &stl2);
	    printf("\nGenerate items of matrixs...");

		in=fopen(name1,"wt");             //write in files m1.txt
		for(i=0;i<str1;i++)
		{
			for (j=0;j<stl1;j++)
			{
				m1[i][j]=rand()%10;
				fprintf(in,"%d ", m1[i][j]);
			}
			fprintf(in, "\n");
		}
		fclose(in);
		in=fopen(name2,"wt");            //write in files m2.txt
		for(i=0;i<str2;i++)
		{
			for (j=0;j<stl2;j++)
			{
				m2[i][j]=rand()%10;
				fprintf(in,"%d ", m2[i][j]);
			}
			fprintf(in, "\n");
		}
		fclose(in);	

		break;
   case '2':
*/	    // read matrix1 from file in1.txt
printf("read vector1 from file m1.txt\n");
		in=fopen("m1.txt","rt");
		for (i=0;i<STR1;i++)
		{
			fscanf(in, "%d", &m1[i]);
		}
//		fgetc(in);
		fclose(in);

		// read matrix2 from file in2.txt
printf("read vector2 from file m2.txt\n");
		in=fopen("m2.txt","rt");
		for (i=0;i<STR2;i++)
		{
			fscanf(in, "%d", &m2[i]);
//			fgetc(in);
		}
		fclose(in);
/*		
		break;
	default:
		puts("Выход");
		return 0;
		break;

}
*/
/*
// define marix1 and marix2 from RND
randomize();
in=fopen("m1.txt","wt");             //and write in files m1.txt
for(i=0;i<STR1;i++)
{
	for (j=0;j<STL1;j++)
	{
		m1[i][j]=random(10);
		fprintf(in,"%d ", m1[i][j]);
	}
	fprintf(in, "\n");
}
fclose(in);
in=fopen("m2.txt","wt");            //and write in files m2.txt
for(i=0;i<STR2;i++)
{
	for (j=0;j<STL2;j++)
	{
		m2[i][j]=random(10);
		fprintf(in,"%d ", m2[i][j]);
	}
	fprintf(in, "\n");
}
fclose(in);
*/


// multiplication


double sum=0;
//int sum1=0;
//int sum2=0;


//Переменная status типа MPI_Status.
MPI_Status status;

t1=realtime();		 
// Инициализация библиотеки MPI.
MPI_Init(&argc, &argv);

t2=realtime();

// Получение в переменную ranksize (размера) числа запущенных процессов
MPI_Comm_size(MPI_COMM_WORLD, &ranksize);
// Получение в переменную rank номера процесса.
MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	 if (rank == 0)               /* I am the master */
	 {
		printf ("Multiplication two vectors\n");
		printf ("Size vector1 - %d \nSize vector2 - %d\n", STR1,STR2);
		//scanf ("%ld", &intervals);
	 }

	 MPI_Barrier (MPI_COMM_WORLD);  /* make sure all MPI tasks are running */
	 if (rank == 0)               /* I am the master */
	 {
		/* distribute parameter */
		printf ("Master: Sending matrix rows and columns to MPI-Processes \n");
		t3 = MPI_Wtime();
		for (i = 1; i < ranksize; i++)
		{
			MPI_Send (&m1[STR1/ranksize*i], STR1/ranksize, MPI_DOUBLE, i, 97, MPI_COMM_WORLD);
			MPI_Send (&m2[STR2/ranksize*i], STR2/ranksize, MPI_DOUBLE, i, 98, MPI_COMM_WORLD);
		}
	 }
	 else /* I am a slave */
	 {	
	      /* receive parameters */
		/*for (i = 1; i < ranksize; i++)
		{
		  MPI_Recv (res[STR1/ranksize*i], STR1/ranksize*STL2, MPI_INT, 0, 98, MPI_COMM_WORLD, &status);
		}*/
		 MPI_Recv (&m1[STR1/ranksize*rank], STR1/ranksize, MPI_DOUBLE, 0, 97, MPI_COMM_WORLD, &status);
		 MPI_Recv (&m2[STR2/ranksize*rank], STR2/ranksize, MPI_DOUBLE, 0, 98, MPI_COMM_WORLD, &status);
	 }


	 /* compute my portion of matrix */
	 t4 = MPI_Wtime();
	
/*	 for (i1=rank*(STR1/ranksize);i1<(rank+1)*(STR1/ranksize);i1++)
	 {
		for (j2=0;j2<STL2;j2++)
		{
			for (j1=0;j1<STL1;j1++)
			{
				sum=sum+m1[i1][j1]*m2[j1][j2];
			}
			res[i1][j2]=sum;
			sum=0;
		}
	 }*/
	for (i=STR1/ranksize*rank;i<STR1/ranksize*rank+STR1/ranksize;i++)
		res[i]=m1[i]*m2[i];

	 t5 = MPI_Wtime();
	 MPI_Barrier (MPI_COMM_WORLD);
	 t6 = MPI_Wtime();

if (rank==0)
{
// Описание действий для нулевого процесса.

	
	 /* collect results, add up, and print results */
	 
		for (i = 1; i < ranksize; i++)
		{
            MPI_Recv(&res[STR1/ranksize*i], STR1/ranksize, MPI_DOUBLE, i,99,MPI_COMM_WORLD, &status);
		}
		t7 = MPI_Wtime();

		printf ("Master: Has collected rezults from MPI-Processes \n");
		printf("Master: Write result multiplication in out.txt \n");
		
		// write matrix result in file out.txt
		for (i=0;i<STR1;i++)
		{
			fprintf(out,"%.0lf ", res[i]);
		}
		fclose(out);

        printf("Master: Write success \n");
		printf ("%ld tasks used - Execution time: %.3lf sec\n",ranksize, t7 -t3);

		printf("\nStatistics:\n");
		printf("Master: startup: %.0lf msec\n",t2-t1);
		printf("Master: time to send # of intervals:%.3lf sec\n",t4-t3);
		printf("Master: waiting time for sincro after calculation:%.2lf sec\n",t6-t5);
		printf("Master: time to collect: %.3lf sec\n",t7-t6);
		printf("Master: calculation time:%.3lf sec\n",t5-t4);

		MPI_Barrier (MPI_COMM_WORLD);
		/* collect there calculation time */
		for (i = 1; i < ranksize; i++)
		{
			MPI_Recv (recv, 2, MPI_DOUBLE, i, 100, MPI_COMM_WORLD, &status);
			printf("process %d: calculation time: %.3lf sec\twaiting time for sincro.: %.3lf sec\n",i,recv[0],recv[1]);
		}

}
else 
{
// Описание действий для остальных процессов.

	MPI_Send(&res[STR1/ranksize*rank], STR1/ranksize,MPI_DOUBLE, 0,99,MPI_COMM_WORLD);
	 /* send my result back to master */
//		  MPI_Send (&pi, 1, MPI_DOUBLE, 0, 99, MPI_COMM_WORLD);

		  MPI_Barrier (MPI_COMM_WORLD);
		  send[0]=t5-t4;
		  send[1]=t6-t5;
		  MPI_Send (send, 2, MPI_DOUBLE, 0, 100, MPI_COMM_WORLD);


}

MPI_Finalize();
return 0;
}

// Модуль вычисления времени
double realtime()			     /* returns time in seconds */
{
	struct _timeb tp;
	_ftime(&tp);
	return((double)(tp.time)*1000+(double)(tp.millitm));
}
