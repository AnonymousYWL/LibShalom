#include <stdio.h>
#include "LibShalom.h"
#include <stdlib.h>

void LibShalom_sgemm(int transa,int transb, float *C, float *A, 
				float *B, long M, long N, long K)
{
	if(/*(transa == 0) && */(transb == 0))
	{
		if(N > 96)
		{
			float *SB= ( float * ) malloc( K* 16 * sizeof( float ) );
			SGEMM_NN(C, A, B, M, N, K, SB);
			free(SB);
		}
		else
		{
			SGEMM_NN_L1(C, A, B, M, N, K);
		}
	}

	else
	{
		SGEMM_NT(C, A, B, M, N, K);
	}

}


void LibShalom_dgemm(int transa,int transb, double *C, double *A, 
				double *B, long M, long N, long K)
{
	if(/*(transa == 0) && */(transb == 0))
	{
		DGEMM_NN(C, A, B, M, N, K);

	}
}