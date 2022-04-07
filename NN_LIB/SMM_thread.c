#include <stdio.h>
#include "LibShalom.h"
#include <stdlib.h>
#include<algorithm>
#include<vector>
#include<iostream>
#include <math.h>
#include <omp.h>

using namespace std;

vector<int>vec;

void Dete_grad_N_threads_nums(int T, long M, long N, int transa, int transb)
{
	int i;
	if(Tm==0)
    {
		// Determines the number of threads to parallelize the N-dimension
		Tn= ceil(sqrt(T * N / M));
		for(i = 0;  i < vec.size(); i++)
		{
			if(Tn <= vec[i])
			{
				Tn = vec[i];
				break;
			}
		}
		if(Tn >= T)
			Tn = T;
		else
		{
			if(transa == 0 && transb ==0 && (M / N ) < 10)
			{
				if((M / Tm) < 2 * (N / Tn))
				{
					Tn = vec[i + 1];
				}
			}

		}
		// Determines the number of threads to parallelize the M-dimension
		Tm = T / Tn;
	}
}

void Dete_grad_M_threads_nums(int T, long M, long N, int transa, int transb)
{

	int i;
	if(Tm==0)
    {
		// Determines the number of threads to parallelize the M-dimension
		Tm= ceil(sqrt(T * M / N));
		for(i = 0;  i < vec.size(); i++)
		{
			if(Tm <= vec[i])
			{
				Tm = vec[i];
				break;
			}
		}
		if(Tm >= T)
			Tm = T;

		// Determines the number of threads to parallelize the N-dimension
		Tn = T / Tm;
	}

}


void LibShalom_sgemm_mp(int transa, int transb, float *C, float *A, 
			float *B, long M, long N, long K)

{

	
	if(/*(transa == 0) &&*/ (transb == 1))
	{
		Dete_grad_N_threads_nums(T, M, N, transa, transb);
		SGEMM_NT_mp(C, A, B, M ,N , K);
	}
	else if(/*(transa == 0) &&*/ (transb == 0))
	{

		// M >> N
		if((M / N)>= 10)
		{

			Dete_grad_M_threads_nums(T, M, N, transa, transb);
		//	printf("Tm = %d , Tn= %d\n", Tm, Tn);
			Small_MGN_NN_SGEMM(C, A, B, M, N, K, N);
		}
		else
		{
			Dete_grad_N_threads_nums(T, M, N, transa, transb);
			Small_NGM_NN_SGEMM(C, A, B, M, N, K, N);
		}
	}

	Tm= 0;

}

void LibShalom_set_thread_nums(int num)
{
	int i;
	T = num;
	for(i=1; i<=sqrt(num); i++) 
	{ 
		if(num % i==0) 
		{ 
			vec.push_back(i);
			if(num!=i*i) 
		    vec.push_back(num/i);
		} 
	}
	sort(vec.begin(), vec.end());
}

void LibShalom_set_thread_ways(int tm, int tn)
{
	Tm = tm;
	Tn = tn;
	T = Tm * Tn;
}

