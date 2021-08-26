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

void LibShalom_sgemm_mp(int transa, int transb, float *C, float *A, 
			float *B, long M, long N, long K)

{
	if(/*(transa == 0) && */(transb == 1))
	{

		SGEMM_NT_mp(C, A, B, M ,N , K);
	}
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
}