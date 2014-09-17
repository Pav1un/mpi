#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<math.h>
#include "mpi.h"
#include <sys\timeb.h>

// matrix1
#define STR1 5000 //sroki
#define STL1 5000 //stolb

// matrix2
#define STR2 5000 //stroki
#define STL2 5000 //stolb

//Объявление модуля вычисления времени
double realtime(void);

// Функция main
int main(int argc, char **argv)
{

FILE *in,*out,*out2;

char name1[15], name2[15];
char key;
int ranksize, rank;
int i,j,i1,j1,j2;
int stl1,str1,str2,stl2;
/*int m1[STR1][STL1];
int m2[STR2][STL2];
double res[STR1][STL2]; //matrix result
*/
int **m1, **m2;
double **res;
	m1 = new int*[STR1];        // STEP 1: SET UP THE ROWS.
	m2 = new int*[STR1];
	res = new double*[STR1];
	for (j = 0; j < STR2; j++){
		m1[j] = new int[STR2];
		m2[j] = new int[STR2];
		res[j] = new double[STR2];
	}
     
	  double send[2],recv[2];
	 double t1,t2;
	 double t3, t4, t5, t6,t7;

out=fopen("result.txt","wt");
out2=fopen("out.txt","at");

if(STL1!=STR2)
{
	printf("\nError in define matrix\n");
	fprintf(out, "Error in define matrix");
	fprintf(out2, "Error in define matrix");
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
		in=fopen("m1.txt","rt");
		for (i=0;i<STR1;i++)
		{
			for (j=0;j<STL1;j++)
			{
				fscanf(in, "%d", &m1[i][j]);
			}
			fgetc(in);
		}
		fclose(in);

		// read matrix2 from file in2.txt
		in=fopen("m2.txt","rt");
		for (i=0;i<STR2;i++)
		{
			for (j=0;j<STL2;j++)
			{
				fscanf(in, "%d", &m2[i][j]);
			}
			fgetc(in);
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
		printf ("Multiplication two matrix\n");
		printf ("Size matrix1 - %d*%d \nSize matrix2 - %d*%d\n", STR1,STL1,STR2,STL2);
		fprintf (out2,"\n\nMultiplication two matrix\n");
		fprintf (out2,"Size matrix1 - %d*%d \nSize matrix2 - %d*%d\n", STR1,STL1,STR2,STL2);
		//scanf ("%ld", &intervals);
	 }

	 MPI_Barrier (MPI_COMM_WORLD);  /* make sure all MPI tasks are running */
	 if (rank == 0)               /* I am the master */
	 {
		/* distribute parameter */
		printf ("Master: Sending matrix rows and columns to MPI-Processes \n");
		fprintf (out2, "Master: Sending matrix rows and columns to MPI-Processes \n");
		t3 = MPI_Wtime();
		for (i = 1; i < ranksize; i++)
		{
			MPI_Send (res[STR1/ranksize*i], STR1/ranksize*STL2, MPI_DOUBLE, i, 98, MPI_COMM_WORLD);
		}
	 }
	 else /* I am a slave */
	 {	
	      /* receive parameters */
		/*for (i = 1; i < ranksize; i++)
		{
		  MPI_Recv (res[STR1/ranksize*i], STR1/ranksize*STL2, MPI_INT, 0, 98, MPI_COMM_WORLD, &status);
		}*/
	 MPI_Recv (res[STR1/ranksize*rank], STR1/ranksize*STL2, MPI_DOUBLE, 0, 98, MPI_COMM_WORLD, &status);
	 }


	 /* compute my portion of matrix */
	 t4 = MPI_Wtime();
	
	 for (i1=rank*(STR1/ranksize);i1<(rank+1)*(STR1/ranksize);i1++)
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
	 }

	 t5 = MPI_Wtime();
	 MPI_Barrier (MPI_COMM_WORLD);
	 t6 = MPI_Wtime();

if (rank==0)
{
// Описание действий для нулевого процесса.

	
	 /* collect results, add up, and print results */
	 
		for (i = 1; i < ranksize; i++)
		{
            MPI_Recv(res[STR1/ranksize*i], STR1/ranksize*STL2, MPI_DOUBLE, i,99,MPI_COMM_WORLD, &status);
		}
		t7 = MPI_Wtime();

		printf ("Master: Has collected rezults from MPI-Processes \n");
		printf("Master: Write result multiplication in out.txt \n");
		
		// write matrix result in file out.txt
		for (i=0;i<STR1;i++)
		{
			for (j=0;j<STL2;j++)
			{
				fprintf(out,"%.0lf ", res[i][j]);
			}
			fprintf(out, "\n");
		}
		fclose(out);

        printf("Master: Write success \n");
		printf ("%ld tasks used - Execution time: %.3lf sec\n",ranksize, t7 -t3);
        fprintf(out2,"Master: Write success \n");
		fprintf (out2,"%ld tasks used - Execution time: %.3lf sec\n",ranksize, t7 -t3);

		printf("\nStatistics:\n");
		printf("Master: startup: %.0lf msec\n",t2-t1);
		printf("Master: time to send # of intervals:%.3lf sec\n",t4-t3);
		printf("Master: waiting time for sincro after calculation:%.2lf sec\n",t6-t5);
		printf("Master: time to collect: %.3lf sec\n",t7-t6);
		printf("Master: calculation time:%.3lf sec\n",t5-t4);
		fprintf(out2,"\nStatistics:\n");
		fprintf(out2,"Master: startup: %.0lf msec\n",t2-t1);
		fprintf(out2,"Master: time to send # of intervals:%.3lf sec\n",t4-t3);
		fprintf(out2,"Master: waiting time for sincro after calculation:%.2lf sec\n",t6-t5);
		fprintf(out2,"Master: time to collect: %.3lf sec\n",t7-t6);
		fprintf(out2,"Master: calculation time:%.3lf sec\n",t5-t4);

		MPI_Barrier (MPI_COMM_WORLD);
		/* collect there calculation time */
		for (i = 1; i < ranksize; i++)
		{
			MPI_Recv (recv, 2, MPI_DOUBLE, i, 100, MPI_COMM_WORLD, &status);
			printf("process %d: calculation time: %.3lf sec\twaiting time for sincro.: %.3lf sec\n",i,recv[0],recv[1]);
			fprintf(out2,"process %d: calculation time: %.3lf sec\twaiting time for sincro.: %.3lf sec\n",i,recv[0],recv[1]);
		}

}
else 
{
// Описание действий для остальных процессов.

	MPI_Send(res[STR1/ranksize*rank], STR1/ranksize*STL2,MPI_DOUBLE, 0,99,MPI_COMM_WORLD);
	 /* send my result back to master */
//		  MPI_Send (&pi, 1, MPI_DOUBLE, 0, 99, MPI_COMM_WORLD);

		  MPI_Barrier (MPI_COMM_WORLD);
		  send[0]=t5-t4;
		  send[1]=t6-t5;
		  MPI_Send (send, 2, MPI_DOUBLE, 0, 100, MPI_COMM_WORLD);


}
	for (i = 0; i < STL1;  i++){
       delete[] m1[i];
       delete[] m2[i];
       delete[] res[i];
	}

	delete[] m1;
	delete[] m2;
	delete[] res;

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
