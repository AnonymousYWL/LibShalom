#include<stdio.h>
#include<sys/time.h>
#include <stdlib.h>

#include <cblas.h>

#include "LibShalom.h"


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
  int i,j;

  for ( i=0; i<m; i++ )
    for ( j=0; j<n; j++ )
		a[i*n+j]= 2.0 * (float)drand48( ) - 1.0 ;
}


void random_matrix1( int m, int n, float *a)
{
  double drand48();
  int i,j;

  for ( i=0; i<m; i++ )
    for ( j=0; j<n; j++ )
		a[i*n+j]= 1.0 ;
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
}


int main()
{

	LibShalom_set_thread_nums(64);
	int i,j,k;
	int loop= 5 ;
	long M, N, K;
	float *C1;
	double start, cost;
	double gflops;
	int flag =0 ;
	char transa = 'N', transb = 'N';
	int pc;

	FILE *fp;


  	if( (fp=fopen("NSMM_small.txt","w")) == NULL )
  	{
    	puts("Fail to open file!");
    	exit(0);
  	}

    for( j =8; j < 9; j = j + 8)
    {

 
        M = 256 ;
        N = 4096;
        K = 5000;
        //M= N = K = j;
    	double ops = M *N *K * 1.0e-09 * 2;
    	fprintf(fp, "%d %d %d", M,N,K);

    	for(pc =0 ;pc < 1; pc++)
    	{
    		int flag =0 ;
    		int lda = K;
			int ldb = K;
			int ldc = N;


	    	float *A = ( float * ) malloc( K* M * sizeof( float ) );
	    	float *B = ( float * ) malloc( K* N * sizeof( float ) );
	    	float *C = ( float * ) malloc( M* N * sizeof( float ) );
	    	float *C1 = ( float * ) malloc( M* N * sizeof( float ) );
	    	float *SB= ( float * ) malloc( K* N * sizeof( float ) );


	    	random_matrix(M,K,A);
	    	random_matrix(K,N,B);

	    	transpose(K, N, B);

	    	for( i =0 ;i< 2; i++)
			cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasTrans, M, N, K, 1.0, A, lda, B, ldb, 0.0, C1, ldc);

			start = dclock();
		    for(i= 0; i<loop ;i++)
		    {

		     	cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasTrans, M, N, K, 1.0, A, lda, B, ldb, 0.0, C1, ldc);
		    }

		    for( i =0 ;i< 5; i++)
		    	LibShalom_sgemm_mp(NoTrans, Trans, C, A, B, M, N, K);

			start = dclock();
			for( i= 0; i< loop ;i++)
				LibShalom_sgemm_mp(NoTrans, Trans, C, A, B, M, N, K);
			cost =(dclock()-start)/loop; 

			ops = M * N  * K * 1.0e-09 * 2;
			printf("N_SMM:  M= %d N=%d K=%d flops = %lf effic= %.3lf %\n", M, N, K, 
					ops/cost, ops/cost/17.6 * 100/ 64);
			fprintf(fp, " %.3f", ops/cost);


		for( i= 0; i< M; i++)
		{
			for( j= 0 ;j<N;j++)
			{
				if((C[i*N+j]- C1[i*N+j] > 0.0001 || C[i*N+j]- C1[i*N+j] < -0.0001) )
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


		    free(A);
		    free(B);
		    free(C);
		    free(SB);
		}

		fprintf(fp, "\n");
	}

	fclose(fp);
    return 0;
}



/*

int main()
{

	LibShalom_thread_nums(32);
//	printf("%d\n", vec.size());
	int i,j,k;
	int loop= 100 ;
	long M, N, K;
	float *C1;
	double start, cost;
	double gflops;
	int flag =0 ;
	char transa = 'N', transb = 'N';
	int pc;

	FILE *fp;

	long Me[5]={5,13,13,23,26}, 
		Ne[5]={5,5,13,23,26}, 
		Ke[5]={5,13,13,23,13};

  	if( (fp=fopen("NSMM_small.txt","w")) == NULL )
  	{
    	puts("Fail to open file!");
    	exit(0);
  	}

    for( j = 0; j < 5; j++)
    {

 
        M = Me[j];
        N = Ne[j];
        K = Ke[j];
        //M= N = K = j;
    	double ops = M *N *K * 1.0e-09 * 2;
    	fprintf(fp, "%d %d %d", M,N,K);

    	for(pc =0 ;pc < 5; pc++)
    	{
	    	double *A = ( double * ) malloc( K* M * sizeof( double ) );
	    	double *B = ( double * ) malloc( K* N * sizeof( double ) );
	    	double *C = ( double * ) malloc( M* N * sizeof( double ) );
	    	double *SB= ( double * ) malloc( K* N * sizeof( double ) );

		    for( i =0 ;i< 5; i++)
		    	LibShalom_dgemm(NoTrans, NoTrans, C, A, B, M, N, K);

			start = dclock();
			for( i= 0; i< loop ;i++)
				LibShalom_dgemm(NoTrans, NoTrans, C, A, B, M, N, K);
			cost =(dclock()-start)/loop; 

			ops = M * N  * K * 1.0e-09 * 2;
			printf("N_SMM:  M= %d N=%d K=%d flops = %lf effic= %.3lf %\n", M, N, K, 
					ops/cost, ops/cost/8.8 * 100);
			fprintf(fp, " %.3f", ops/cost);

		    free(A);
		    free(B);
		    free(C);
		    free(SB);
		}

		fprintf(fp, "\n");
	}

	fclose(fp);
    return 0;
}
*/