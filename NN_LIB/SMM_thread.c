#include <stdio.h>
#include "LibShalom.h"
#include <stdlib.h>
#include<algorithm>
#include<vector>
#include<iostream>
#include <math.h>
#include <omp.h>

using namespace std;

vector<int>vec{1,2,4,8,16,32,64};

void Dete_grad_N_threads_nums(int T, long M, long N, int transa, int transb)
{
	int i, temp;
	if(Tm==0)
    {
		// Determines the number of threads to parallelize the N-dimension
		if(transb == 0)
		{
			Tn= ceil(sqrt(4 *T * (float)N / (float)M));
		}
		else
			Tn = ceil(sqrt(T * (float)N / (float)M));
		for(i = 0;  i < vec.size(); i++)
		{
			if(Tn <= vec[i])
			{
				temp = vec[i];
				if(i > 0 && (Tn - vec[i-1] <= vec[i] - Tn))
				{	
					Tn = vec[i-1];			
				}
				else
				{
					Tn = temp;
				}
				break;
			}
		}
		if(Tn >= T)
			Tn = T;
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
		Tm= ceil(sqrt(T * (float)M / (float)N));
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
		if(M < N)
		{
			Dete_grad_N_threads_nums(T, M, N, transa, transb);
			Small_NGM_NT_SGEMM(C, A, B, M ,N ,K, N);
		}
		else
		{
			Dete_grad_M_threads_nums(T, M, N, transa, transb);
			Small_MGN_NT_SGEMM(C, A, B, M ,N ,K, N);
		}
	}
	else if(/*(transa == 0) &&*/ (transb == 0))
	{

		// M >> N
		if((M / N)>=5)
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

