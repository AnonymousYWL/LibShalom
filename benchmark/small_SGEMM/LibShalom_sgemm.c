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

//	LibShalom_set_thread_nums(64);
	int i,j,k;
	int loop= 100;
	long M, N, K;
	double start, cost;
	double gflops;
	int pc;

	FILE *fp;


  	if( (fp=fopen("LibShalom_sgemm.txt","w")) == NULL )
  	{
    	puts("Fail to open file!");
    	exit(0);
  	}

  	printf("LibShalom:\n"); 
    for( j =8; j < 129; j = j + 8)
    {

 		M= N = K = j;
    	double ops = M *N *K * 1.0e-09 * 2;
    	fprintf(fp, "%d %d %d", M,N,K);

    	for(pc =0 ;pc < 5; pc++)
    	{


	    	float *A = ( float * ) malloc( K* M * sizeof( float ) );
	    	float *B = ( float * ) malloc( K* N * sizeof( float ) );
	    	float *C = ( float * ) malloc( M* N * sizeof( float ) );

	    	random_matrix(M,K,A);
	    	random_matrix(K,N,B);
//	    	transpose(K, N, B);

		    for( i =0 ;i< 1; i++)
		    	LibShalom_sgemm(NoTrans, NoTrans, C, A, B, M, N, K);

			start = dclock();
			for( i= 0; i< loop ;i++)
				LibShalom_sgemm(NoTrans, NoTrans, C, A, B, M, N, K);
			cost =(dclock()-start)/loop; 

			ops = M * N  * K * 1.0e-09 * 2;
			printf(" M= %d N=%d K=%d Gflops = %lf\n", M, N, K, 
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

