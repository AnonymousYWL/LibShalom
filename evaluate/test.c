#include <stdio.h>
//#include <cblas.h>
#include <sys/time.h>
#include <stdlib.h>
#include <omp.h>
#include "LibShalom.h"


#define num 64
static double gtod_ref_time_sec = 0.0;

double dclock()
{
        double the_time, norm_sec;
        struct timeval tv;

        gettimeofday( &tv, NULL );

        if ( gtod_ref_time_sec == 0.0 )
                gtod_ref_time_sec = ( double ) tv.tv_sec;

        norm_sec = ( double ) tv.tv_sec - gtod_ref_time_sec;
        the_time = norm_sec + tv.tv_usec * 1.0e-6;
        return the_time;
}

void random_matrix( int m, int n, float *a)
{
  double drand48();

  //#pragma omp parallel for num_threads(num)
  for(int i =0 ; i < m * n; i ++)
  	a[i]= 2.0 * (float)drand48( ) - 1.0 + 0.0000001 * (i/m);

}


void random_matrix1( int m, int n, float *a)
{

  #pragma omp parallel for num_threads(num)
  for(int i =0 ; i < m * n; i ++)
  {
  	a[i] = 1.0;
  }
}


void transpose( int m, int n, float *a)
{
	float *temp_a = ( float * ) malloc( m* n * sizeof( float ) );

	int i, j;

	for( i =0 ;i< m; i++)
	{
		for( j= 0;j < n; j++)
		{
			temp_a[j * m + i] = a[i*n + j];
		}
	}


	for( i =0 ; i< n; i++)
	{
		for( j =0; j< m; j++)
		{
			a[ i * m+j] = temp_a[i*m+j];
		}
	}

	free(temp_a);
}

void Check_result(float *C, float *C1, long M, long N, int flag)
{

	int i, j;

		for( i= 0; i< M; i++)
		{
			for( j= 0 ;j < N;j++)
			{
				if((C[i*N+j]- C1[i*N+j] > 0.01 || C[i*N+j]- C1[i*N+j] < -0.01))
				{
						printf("i = %d, j= %d\n",i ,j );
						printf("C= %lf , C1= %lf\n", C[i*N+j], C1[i*N+j]);
						flag =1;
						printf("error\n");
						break;
				}
			}
				if(flag ==1)
					break;
		}


		if(flag == 0)
			printf("结果正确\n");


}


int main()
{



	//openblas_set_num_threads(1);
	LibShalom_set_thread_nums(num);
	int i,j,k,pc;
	int loop=100;
  long Me[]={5120, 128  , 320 , 1632,  2048, 31999, 84,   256,  1024,   35,   64,    128,  256,  512,  28269, 256,  64,   64,   512, 128};
	long Ne[]={16, 2048 , 3072, 36548, 4096, 1024,  1024, 256,  16,     8457, 18496, 4624, 1156, 289,  289,   3136, 3136, 3136, 784, 784};
	long Ke[]={5120, 4096 , 4096, 1024,  32,   88,    4096, 2048, 500000, 2560, 128,   256,  512,  1024, 1024,  64,   64,   256,  128, 512};
	float *A,*B,*C,*C1,*flush;
	double start, cost=0;
	double ops;
	int lda, ldb, ldc, flag;
	double total_cost=0;
	double total_cost1=0;
	double start1, cost1;
	long M, N, K;

	FILE *fp;
	if( (fp=fopen("LibShalom_thread_NN.txt","w")) == NULL )
	{
		puts("Fail to open file!");
		exit(0);
	}

	for (j = 0 ; j < 20; j++)
	{

		M = Me[j];
		N = Ne[j];
		K = Ke[j];

		lda = K;
		ldb = N;
		ldc = N;

		ops = (double )M * N  * K * 1.0e-09 * 2;
		fprintf(fp, "%d %d %d", M, N, K);
		for(pc = 0; pc < 5; pc++)
		{

			flag = 0;

			A = ( float * ) malloc( K*M* sizeof( float ) );  
			B = ( float * ) malloc( K* N * sizeof( float ) );
			C = ( float * ) malloc( M* N * sizeof( float ) );
			C1 = ( float * ) malloc( M* N * sizeof( float ) );
			flush = ( float * ) malloc( 5000* 5000 * sizeof( float ) );

			//random_matrix(M,N,C);
			random_matrix(M,K,A);
			random_matrix(K,N,B);


			// warm up
			for( i =0 ;i< 2; i++)
			  LibShalom_sgemm_mp(0, 0, C, A, B, M, N, K);

			start = dclock();
			for( i= 0; i< loop ;i++)
				LibShalom_sgemm_mp(0, 0, C, A, B, M, N, K);
			cost = (dclock()-start)/loop; 


			printf("SGEMM:  M= %d N=%d K=%d Gflops = %lf effic= %.3lf %\n", 
			M, N, K, ops/cost, ops/cost/41.6 * 100/num);
			fprintf(fp, " %lf", ops/cost);

			//cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, M, N, K, 
			//					1.0, A, lda, B, ldb, 0.0, C1, ldc);


			//Check_result(C, C1, M, N, flag);  // Check the correctness of the results
			//random_matrix1(5000,5000,flush);
			free(A);
			free(B);
			free(C);
			free(C1);
			free(flush);

	  }
	  fprintf(fp, "\n");
	}

	//cost1 = dclock() - start1;

	//printf("total_cost = %lf s, cost1 = %f s\n", total_cost, cost1);

  fclose(fp);
  return 0;

}
