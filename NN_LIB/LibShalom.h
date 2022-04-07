#ifndef __TEST_H__ 
#define __TEST_H__ 

#include<stdio.h>
#include <vector>

using namespace std;

// No transpose
#define NoTrans 0
// transpose
#define Trans 1

#define GEMM_K 320
#define GEMM_N 4096
#define GEMM_M 256


//All divisors of the total number of threads
extern vector<int>vec;
extern int Tm, Tn, T;

void LibShalom_sgemm(int transa, int transb, float *C, float *A, float *B, long M, long N, long K);
void SGEMM_NN(float *C, float *A, float *B, long M, long N, long K, float *SB);
void SGEMM_NN_L1(float *C, float *A, float *B, long M, long N, long K);
void SGEMM_NT(float *C, float *A, float *B, long M, long N, long K);

void LibShalom_sgemm_mp(int transa, int transb, float *C, float *A, float *B, long M, long N, long K);
void SGEMM_NT_mp(float *C, float *A, float *B, long M, long N, long K);
void SGEMM_NN_mp(float *C, float *A, float *B, long M, long N, long K);
void Small_NGM_NN_SGEMM(float *C, float *A, float *B, long M, long N, long K, long LD);
void Small_MGN_NN_SGEMM(float *C, float *A, float *B, long M, long N, long K, long LD);
void SGEMM_NT_kernel_exist_1(float *C, float *A, float *B, long	M, long N, long K, 
			 	long LN, long LK, float *SB, long k_tag);

void DGEMM_NN(double *C, double *A, double *B, long M, long N, long K);
void LibShalom_dgemm(int transa, int transb, double *C, double *A, double *B, long M, long N, long K);

void LibShalom_set_thread_nums(int num);
void LibShalom_set_thread_ways(int tm, int tn);
void Dete_grad_M_threads_nums(int T, long M, long N);
void Dete_grad_N_threads_nums(int T, long M, long N);

void NPACK(float* A, float* Ac, long M, long K, long LK);
void TPACK(float* B, float* Bc, float* addr, long K, long N, long LK,long LN);
void Sin_NPACK(float* A, float* Ac, long M, long K, long LK);
#endif