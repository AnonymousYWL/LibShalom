#include "LibShalom.h"
#include "./Kernel/SMM_m8n8_NT_new.h"
#include "./Kernel/SMM_m8n8_NT_MGN_new.h"
#include <stdlib.h>
#include <omp.h>

//N >> M
void Small_NGM_NT_SGEMM(float *C, float *A, float *B, long M, long N,
                        long K, long LD)
{

	long LK = K;
	void *ptr;
	int NUM = T;
	float *Ac = ( float * ) malloc( K * M * sizeof( float ) );
	posix_memalign(&ptr, 64, NUM * GEMM_K * 8 * sizeof( float ));
	float *SSB = (float *)ptr;

	long Num_K_block = K / GEMM_K;
	long Edge_K = K % GEMM_K;
	long Num_M_block = M / 8;
	long Num_blocks0 = Num_K_block * Num_M_block;
	long Num_blocks = Num_blocks0;
	if (Edge_K > 0)
	{
		Num_blocks = Num_blocks0 + Num_M_block;
	}

	#pragma omp parallel num_threads(NUM)
	{

		int i, j;
		int id = omp_get_thread_num();

		for ( i = id; i < Num_blocks; i = i + NUM)
		{

			long start_M = (i % Num_M_block) * 8;					// note
			long start_K = (i / Num_M_block) * GEMM_K;		// note
			float *AA = A + start_M * K + start_K;        // address
			float *AAc;

			if ( Edge_K > 0 && i >= Num_blocks0)
			{
				AAc = Ac + start_K * M + start_M * Edge_K;  // note
				NPACK(AA, AAc, 8, Edge_K, K);
			}
			else
			{
				AAc = Ac + start_K * M + start_M * GEMM_K;  // note
				NPACK(AA, AAc, 8, GEMM_K, K);
			}
		}

		#pragma omp barrier
		// finish packing matrix A


		long ii, jj, kk, iis, jjs, kks, n_to, m_to, k_to;
		long nb = N / NUM;
		long nc, mc, kc;
		jjs = id * nb;
		n_to = jjs + nb;
		iis = 0;
		m_to = M;
		kks = 0;
		k_to = K;

		for ( jj = jjs; jj < n_to; jj = jj + nc)
		{

			nc = GEMM_N;
			if (n_to - jj < GEMM_N)
			{
				nc = n_to - jj;
			}


			for (kk = kks; kk < k_to; kk = kk + kc)
			{
				kc = GEMM_K;
				if (k_to - kk < GEMM_K)
					kc = k_to - kk;

				float *BB = B + jj * K + kk;

				for ( ii = iis ; ii < m_to; ii = ii + mc)
				{
					mc = GEMM_M;
					if (m_to - ii < GEMM_M)
					{
						mc = m_to - ii;
					}

					float *CC = C + ii * N + jj;
					float *AAc = Ac + kk * M + ii * kc;

					long EDGE_nc = nc % 8;
					long LEN_nc = nc - EDGE_nc;
					SMM_NT_KERNELMx8(CC, AAc, BB, mc, LEN_nc, kc, N, K, 
						&SSB[id * GEMM_K * 8], kk);

					if(EDGE_nc !=0)
					{
						float *temp_BB = BB + LEN_nc * K;
						float *temp_CC = CC + LEN_nc;

						if(EDGE_nc >= 4)
						{
							SMM_NT_KERNELMx4(temp_CC, AAc, temp_BB, mc, 4, kc, N, K, 
								&SSB[id * GEMM_K * 8], kk);
							EDGE_nc = EDGE_nc - 4;
							temp_BB = temp_BB + 4 * K;
							temp_CC = temp_CC + 4;
						}
					}

				}
			}
		}

	}
	free(SSB);
	free(Ac);
}



//M >> N
void Small_MGN_NT_SGEMM(float *C, float *A, float *B, long M, long N,
                        long K, long LD)
{

	long LK = K;
	void *ptr;
	int NUM = T;
	float *Bc = ( float * ) malloc( K * N * sizeof( float ) );
	posix_memalign(&ptr, 64, NUM * GEMM_K * 8 * sizeof( float ));
	float *SSA = (float *)ptr;

	long Num_K_block = K / GEMM_K;
	long Edge_K = K % GEMM_K;
	long Num_N_block = N / 8;
	long Num_blocks0 = Num_K_block * Num_N_block;
	long Num_blocks = Num_blocks0;
	if (Edge_K > 0)
	{
		Num_blocks = Num_blocks0 + Num_N_block;
	}

	#pragma omp parallel num_threads(NUM)
	{

		int i, j;
		int id = omp_get_thread_num();

		for ( i = id; i < Num_blocks; i = i + NUM)
		{

			long start_N = (i % Num_N_block) * 8;					// note
			long start_K = (i / Num_N_block) * GEMM_K;		// note
			float *BB = B + start_N * K + start_K;        // address
			float *BBc;

			if ( Edge_K > 0 && i >= Num_blocks0)
			{
				BBc = Bc + start_K * N + start_N * Edge_K;  // note
				NPACK(BB, BBc, 8, Edge_K, K);
			}
			else
			{
				BBc = Bc + start_K * N + start_N * GEMM_K;  // note
				NPACK(BB, BBc, 8, GEMM_K, K);
			}
		}

		#pragma omp barrier
		// finish packing matrix A


		long ii, jj, kk, iis, jjs, kks, n_to, m_to, k_to;
		long mb = M / Tm;
		long me = M % Tm;
		long nb = N / Tn;
		long ne = N % Tn;
		long nc, mc, kc;

		kks = 0;
		k_to = K;
		iis = (id % Tm) * mb;
		m_to = iis + mb;

		if(id % Tm < me)
		{
			iis = id % Tm * (mb +1 );
			m_to = iis + mb + 1;
		}
		else
		{
			iis = id % Tm * mb + me;
			m_to = iis + mb ;
		}

		if(id / Tm < ne)
		{
			jjs = id / Tm * (nb +1 );
			n_to = jjs + nb + 1;
		}
		else
		{
			jjs = id / Tm * nb + ne;
			n_to = jjs + nb;
		}


		for ( ii = iis; ii < m_to; ii = ii + mc)
		{

			mc = GEMM_N;
			if (m_to - ii < GEMM_N)
			{
				mc = m_to - ii;
			}


			for (kk = kks; kk < k_to; kk = kk + kc)
			{
				kc = GEMM_K;
				if (k_to - kk < GEMM_K)
					kc = k_to - kk;

				float *AA = A + ii * K + kk;

				for ( jj = jjs ; jj < n_to; jj = jj + nc)
				{
					nc = GEMM_M;
					if (n_to - jj < GEMM_M)
					{
						nc = n_to - jj;
					}

					float *CC = C + ii * N + jj;
					float *BBc = Bc + kk * N + jj * kc;

					long EDGE_mc = mc % 8;
					long LEN_mc = mc - EDGE_mc;
					SMM_NT_KERNEL8xN(CC, AA, BBc, LEN_mc, nc, kc, N, K, 
						&SSA[id * GEMM_K * 8], kk);

					if(EDGE_mc !=0)
					{
						float *temp_AA = AA + LEN_mc * K;
						float *temp_CC = CC + LEN_mc * N;

						if(EDGE_mc >= 4)
						{
							SMM_NT_KERNEL4xN(temp_CC, temp_AA, BBc, 4, nc, kc, N, K, 
								&SSA[id * GEMM_K * 8], kk);
							EDGE_mc = EDGE_mc - 4;
							temp_AA = temp_AA + 4 * K;
							temp_CC = temp_CC + 4 * N;
						}
						if(EDGE_mc == 1)
						{
							SMM_NT_KERNEL1xN(temp_CC, temp_AA, BBc, 1, nc, kc, N, K, 
								&SSA[id * GEMM_K * 8], kk);
						}
					}

				}
			}
		}

	}
	free(SSA);
	free(Bc);
}