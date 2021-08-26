#include <stdlib.h>
#include <stdlib.h>
#include "LibShalom.h"

int main()
{

	int i,j,k;
	int loop= 100;
	long M, N, K;
        M= N = K = 80;
        /* row-major */   	
	float *A = ( float * ) malloc( K* M * sizeof( float ) );
	float *B = ( float * ) malloc( K* N * sizeof( float ) );
	float *C = ( float * ) malloc( M* N * sizeof( float ) );

	double drand48();
	/* initialize input matrix and B*/
	for ( i = 0; i < M; i++ )
	{
		for ( j = 0; j < K; j++ )
			A [i* K + j]= 2.0 * (float)drand48( ) - 1.0 ;
	}

	for ( i = 0; i < K; i++ )
	{
		for ( j = 0; j < N; j++ )
			B [i * K + j]= 2.0 * (float)drand48( ) - 1.0 ;
	}

	// warm up
	//perform C = A * B (B is transposed)
	for( i =0 ;i< 5; i++)
		LibShalom_sgemm(NoTrans, Trans, C, A, B, M, N, K);

	for( i= 0; i< loop ;i++)
		LibShalom_sgemm(NoTrans, Trans, C, A, B, M, N, K);


    free(A);
    free(B);
    free(C);
    return 0;
}
