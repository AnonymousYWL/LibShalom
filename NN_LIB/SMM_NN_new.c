#include "LibShalom.h"
#include "./Kernel/SMM_m8n8_new.h"
#include "./Kernel/SMM_m8n12_new.h"
#include <stdlib.h>
#include <omp.h>


//N >> M
void Small_NGM_NN_SGEMM(float *C, float *A, float *B, long M, long N, 
					long K, long LD)
{

	long LK = K;
  	void *ptr;
  	int NUM = T;
  	float *Ac = ( float * ) malloc( K* M * sizeof( float ) );
  	posix_memalign(&ptr, 64, NUM * GEMM_K * 12 *sizeof( float ));
  	float *SSB = (float *)ptr;

  	long Num_K_block = K / GEMM_K;
	long Edge_K = K % GEMM_K;
	long Num_M_block = M / 8;
	long Num_blocks0 = Num_K_block * Num_M_block;
	long Num_blocks = Num_blocks0; 
	if(Edge_K > 0)
	{
		Num_blocks = Num_blocks0 + Num_M_block;
	}


	#pragma omp parallel num_threads(NUM)
	{


		int i, j;
		int id = omp_get_thread_num();

		//double start = dclock();

		for( i = id; i < Num_blocks; i = i + NUM)
		{


			long start_M = (i % Num_M_block) * 8;					// note
			long start_K = (i / Num_M_block) * GEMM_K;		// note

			float *AA = A + start_M * K + start_K;        // address
			float *AAc; 

			if( Edge_K > 0 && i >= Num_blocks0)
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


		long ii, jj, kk, iis, jjs, kks, n_to, m_to, k_to;
		long nb = N / Tn;
		long mb = M / Tm;
		long ne = N % Tn;
		long me = M % Tm;
		long nc, mc, kc;


		jjs = (id % Tn) * nb;
		n_to = jjs + nb;
		iis = id / Tn * mb;
		m_to = iis + mb;
		kks = 0;
		k_to = K;

		for( jj = jjs; jj < n_to; jj= jj + nc)
		{

			nc = GEMM_N;
  		if(n_to - jj < GEMM_N)
  		{
  			nc = n_to - jj;
  		}

  		for(kk= kks; kk < k_to; kk = kk + kc)
  		{
  			kc = GEMM_K;
    		if(k_to - kk < GEMM_K)
				kc = k_to - kk;

				float *BB = B + kk * N + jj;

				for( ii = iis ; ii < m_to; ii = ii + mc)
				{
					mc = GEMM_M;
					if(m_to - ii < GEMM_M)
					{
						mc = m_to - ii;
					}
					float *CC = C + ii * N + jj;
					float *AAc = Ac + kk * M + ii * kc;

					long EDGE_nc = nc % 12;
					long LEN_nc = nc - EDGE_nc;
					SMM(CC, AAc, BB, mc, LEN_nc, kc, N, K, &SSB[id * GEMM_K * 12], kk);
					if(EDGE_nc !=0)
					{
						float *temp_BB = BB + LEN_nc;
						float *temp_CC = CC + LEN_nc;

						if(EDGE_nc >= 4)
						{

							SMM_NN_KERNEL8x4(temp_CC, AAc, temp_BB, mc, 4, kc, N, K, 
								&SSB[id * GEMM_K * 12], kk);

							EDGE_nc = EDGE_nc - 4;
							temp_BB = temp_BB + 4;
							temp_CC = temp_CC + 4;
						}

						if(EDGE_nc >= 2)
						{
								SMM_NN_KERNEL8x2(temp_CC, AAc, temp_BB, mc, 2, kc, N, K, 
								&SSB[id * GEMM_K * 12], kk);

								EDGE_nc = EDGE_nc - 2;
								temp_BB = temp_BB + 2;
								temp_CC = temp_CC + 2;
						}

						if(EDGE_nc >= 1)
						{
								SMM_NN_KERNEL8x1(temp_CC, AAc, temp_BB, mc, 1, kc, N, K, 
								&SSB[id * GEMM_K * 12], kk);
						}
					}	
				}
			}
    }


// Edge_case
    if(ne > 0)
    {

    	mb = M / NUM;
    	iis = mb * id;
    	m_to = iis + mb;

    	for(kk= kks; kk < k_to; kk = kk + kc)
  		{
  			kc = GEMM_K;
    		if(k_to - kk < GEMM_K)
				kc = k_to - kk;

				float *BB = B + kk * N + (N - ne);

				for( ii = iis ; ii < m_to; ii = ii + mc)
				{
					mc = GEMM_M;
					if(m_to - ii < GEMM_M)
					{
						mc = m_to - ii;
					}

					float *CC = C + ii * N + (N - ne);
					float *AAc = Ac + kk * M + ii * kc;

					long temp_ne = ne;

					if(temp_ne >= 4)
					{
								SMM_NN_KERNEL8x4(CC, AAc, BB, mc, 4, kc, N, K, 
								&SSB[id * GEMM_K * 12], kk);
								temp_ne = temp_ne - 4;
					}
				}
			}
		}
		
	}

	free(SSB);
	free(Ac);
	
}

//M >> N
void Small_MGN_NN_SGEMM(float *C, float *A, float *B, long M, long N, 
					long K, long LD)
{

	long LK = K;
	int NUM = T;
	void *ptr, *ptr1;
	posix_memalign(&ptr, 64, NUM * GEMM_K * 8 *sizeof( float ));
	posix_memalign(&ptr1, 64, K * N *sizeof( float ));
	float *SSA = (float *)ptr;
	float *Bc = (float *)ptr1;

	long Num_K_block = K / GEMM_K;
	long Edge_K = K % GEMM_K;
	long Num_blocks0 = Num_K_block;
	long Num_blocks = Num_blocks0; 
	if(Edge_K > 0)
	{
		Num_blocks = Num_blocks0 + 1;
	}

	#pragma omp parallel num_threads(NUM)
	{

		//double start = dclock();
		int i , j;
		int id = omp_get_thread_num();
		int flag =0;

		for( i = id / 2; i < Num_blocks; i = i + NUM/2)
		{

			long start_N = 0;															// note
			long start_K = i * GEMM_K;										// note

			float *BB = B + start_K * N;        // address
			float *BBc = Bc + start_K * N;      // note

			long Edge_N = N % 8;

			if( Edge_K > 0 && i == Num_blocks0)
			{

				float *addr = BBc + (N - Edge_N) * Edge_K;

				if(id % 2 ==1)
				{
					BB = BB + (Edge_K / 2) * N;
					BBc = BBc + (Edge_K / 2) * 8;
					if(Edge_N > 0)
					{
						if(Edge_N >= 4)
							addr = addr + 4 * (Edge_K / 2);
						else if(Edge_N < 4 && Edge_N >=2)
							addr = addr + 2 * (Edge_K / 2);
						else
							addr = addr + (Edge_K / 2);
					}
					TPACK(BB, BBc, addr,Edge_K - (Edge_K / 2), N, Edge_K, N);
				}
				else
				{
					TPACK(BB, BBc, addr,Edge_K / 2, N, Edge_K, N);
				}
				
			}
			else
			{
				float *addr = BBc + (N - Edge_N) * GEMM_K;
				if(id % 2 == 1)
				{
					BB = BB + (GEMM_K / 2) * N;
					BBc = BBc + (GEMM_K / 2) * 8;

					if(Edge_N > 0)
					{
						if(Edge_N >= 4)
							addr = addr + 4 * (GEMM_K / 2);
						else if(Edge_N < 4 && Edge_N >=2)
							addr = addr + 2 * (GEMM_K / 2);
						else
							addr = addr + (GEMM_K / 2);
					}
					//printf("id = %d, addr = %ld\n", id, BBc);
					TPACK(BB, BBc, addr, GEMM_K - (GEMM_K / 2), N, GEMM_K, N);
				}
				else
				{
					TPACK(BB, BBc, addr, GEMM_K / 2, N, GEMM_K, N);
				}
			}
		}

		#pragma omp barrier

		long ii, jj, kk, iis, jjs, kks, n_to, m_to, k_to;
		long nb = N / Tn;
		long mb = M / Tm;
		long nc, mc, kc;


		iis = (id % Tm) * mb;
		m_to = iis + mb;
		jjs = id / Tm * nb;
		n_to = jjs + nb;
		kks = 0;
		k_to = K;

		for(ii = iis; ii < m_to; ii = ii + mc)
		{

			mc = GEMM_M;
			if( m_to - ii < GEMM_M)
				mc = m_to - ii;

			for(kk = kks; kk < k_to; kk = kk + kc)
			{ 
				kc = GEMM_K;
				if(k_to - kk < GEMM_K)
					kc = k_to - kk;

				float *AA = A + ii * K + kk;

				for(jj = jjs; jj < n_to; jj = jj + nc)
				{

					nc = GEMM_N;
					if(n_to - jj < GEMM_N)
						nc= n_to - jj;
					float *BBc = Bc + kk * N + jj * kc;
					float *CC = C + ii * N + jj;

					SMM_NN_KERNEL8xN(CC, AA, BBc, mc, nc, kc, N, K, 
						&SSA[id * GEMM_K * 8], kk);
				}
			}
		}

	}

	free(SSA);
	free(Bc);
	
}