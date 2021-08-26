#include<stdio.h>
#include<sys/time.h>
#include <stdlib.h>
#include "../../NN_LIB/LibShalom.h"


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

	int i,j,k;
	int loop= 100;
	long M, N, K;
	double start, cost;
	double gflops;
	int pc;

	FILE *fp;

	long Me[5]={5,13,13,23,26}, 
		Ne[5]={5,5,13,23,26}, 
		Ke[5]={5,13,13,23,13};

  	if( (fp=fopen("LibShalom_dgemm.txt","w")) == NULL )
  	{
    	puts("Fail to open file!");
    	exit(0);
  	}


  	printf("LibShalom: \n");
    for( j = 0; j < 5; j++)
    {

        M = Me[j];
        N = Ne[j];
        K = Ke[j];
    	double ops = M *N *K * 1.0e-09 * 2;
    	fprintf(fp, "%d %d %d", M,N,K);

    	for(pc =0 ;pc < 5; pc++)
    	{
	    	double *A = ( double * ) malloc( K* M * sizeof( double ) );
	    	double *B = ( double * ) malloc( K* N * sizeof( double ) );
	    	double *C = ( double * ) malloc( M* N * sizeof( double ) );

		    for( i =0 ;i< 5; i++)
		    	LibShalom_dgemm(NoTrans, NoTrans, C, A, B, M, N, K);

			start = dclock();
			for( i= 0; i< loop ;i++)
				LibShalom_dgemm(NoTrans, NoTrans, C, A, B, M, N, K);
			cost =(dclock()-start)/loop; 

			ops = M * N  * K * 1.0e-09 * 2;
			printf(" M= %d N=%d K=%d Gflops = %lf \n", M, N, K, 
					ops/cost);
			fprintf(fp, " %.3f", ops/cost);

		    free(A);
		    free(B);
		    free(C);
		}

		fprintf(fp, "\n");
	}

	fclose(fp);
    return 0;
}

