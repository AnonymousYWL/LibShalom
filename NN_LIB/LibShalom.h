#ifndef __TEST_H__ 
#define __TEST_H__ 

#include<stdio.h>
#include <vector>

using namespace std;

// No transpose
#define NoTrans 0
// transpose
#define Trans 1

//All divisors of the total number of threads
extern vector<int>vec;
extern int Tm, Tn, T;

void LibShalom_sgemm(int transa, int transb, float *C, float *A, float *B, long M, long N, long K);
void SGEMM_NN(float *C, float *A, float *B, long M, long N, long K, float *SB);
void SGEMM_NN_L1(float *C, float *A, float *B, long M, long N, long K);
void SGEMM_NT(float *C, float *A, float *B, long M, long N, long K);

void LibShalom_sgemm_mp(int transa, int transb, float *C, float *A, float *B, long M, long N, long K);
void SGEMM_NT_mp(float *C, float *A, float *B, long M, long N, long K);
void SGEMM_NT_kernel_exist_1(float *C, float *A, float *B, long	M, long N, long K, 
			 	long LN, long LK, float *SB, long k_tag);

void DGEMM_NN(double *C, double *A, double *B, long M, long N, long K);
void LibShalom_dgemm(int transa, int transb, double *C, double *A, double *B, long M, long N, long K);

void LibShalom_set_thread_nums(int num);

void LibShalom_set_thread_ways(int tm, int tn);
#endif