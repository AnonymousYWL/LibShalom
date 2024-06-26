#include "LibShalom.h"
#include "./Kernel/SMM_m8n8_new.h"
#include "./Kernel/SMM_m8n12_new.h"
#include <stdlib.h>
#include <omp.h>


void SGEBP_NGM_NN(float *CC, float *AAc, float *BB, float *Bc, float *SSB, 
				long mc, long nc, long kc, long N, long K,
				long id, long jjs,
				long k_tag, long m_tag, long id_tag)
{

	//printf("m_tag = %d, id_tag = %d\n", m_tag, id_tag);
	long EDGE_nc = nc % 24;
	long LEN_nc = nc - EDGE_nc;
	if(LEN_nc > 0)
	{
		SMM(CC, AAc, BB, mc, LEN_nc, kc, N, K, 
			&Bc[jjs * GEMM_K], &SSB[id * GEMM_K * 24], kc * 12 * 4,
			k_tag, m_tag, id_tag);

		//printf("m_tag = %d\n", m_tag);
	}

	if(EDGE_nc !=0)
	{
		float *temp_BB = BB + LEN_nc;
		float *temp_CC = CC + LEN_nc;
		float *temp_Bc = Bc + jjs * GEMM_K + LEN_nc * kc;

		if(EDGE_nc >= 12)
		{
			SMM(temp_CC, AAc, temp_BB, mc, 12, kc, N, K, 
				temp_Bc, &SSB[id * GEMM_K * 24], kc * 12 * 4,
				k_tag, m_tag, id_tag);
			EDGE_nc = EDGE_nc - 12;
			temp_BB = temp_BB + 12;
			temp_CC = temp_CC + 12;
			temp_Bc = temp_Bc + 12 * kc;
		}

		if(EDGE_nc >= 8)
		{

			SMM_NN_KERNEL8x8(temp_CC, AAc, temp_BB, mc, 8, kc, N, K, 
				temp_Bc, &SSB[id * GEMM_K * 24], 
				k_tag, m_tag, id_tag);

			EDGE_nc = EDGE_nc - 8;
			temp_BB = temp_BB + 8;
			temp_CC = temp_CC + 8;
			temp_Bc = temp_Bc + 8 * kc;
		}

		if(EDGE_nc >= 4)
		{

			SMM_NN_KERNEL8x4(temp_CC, AAc, temp_BB, mc, 4, kc, N, K, 
				temp_Bc, &SSB[id * GEMM_K * 24], 
				k_tag, m_tag, id_tag);

			EDGE_nc = EDGE_nc - 4;
			temp_BB = temp_BB + 4;
			temp_CC = temp_CC + 4;
			temp_Bc = temp_Bc + 4 * kc;
		}

		if(EDGE_nc >= 2)
		{
			SMM_NN_KERNEL8x2(temp_CC, AAc, temp_BB, mc, 2, kc, N, K, 
				temp_Bc, &SSB[id * GEMM_K * 24], 
				k_tag, m_tag, id_tag);

			EDGE_nc = EDGE_nc - 2;
			temp_BB = temp_BB + 2;
			temp_CC = temp_CC + 2;
			temp_Bc = temp_Bc + 2 * kc;
		}

		if(EDGE_nc >= 1)
		{
			SMM_NN_KERNEL8x1(temp_CC, AAc, temp_BB, mc, 1, kc, N, K, 
				temp_Bc, &SSB[id * GEMM_K * 24], 
				k_tag, m_tag, id_tag);
		}
	}
}


//N >> M
void Small_NGM_NN_SGEMM(float *C, float *A, float *B, long M, long N, 
					long K, long LD)
{

	long LK = K;
  	void *ptr, *ptr1;
  	int NUM = T;
  	float *Ac = ( float * ) malloc( K* M * sizeof( float ) );
  	posix_memalign(&ptr, 64, NUM * GEMM_K * 24 *sizeof( float ));
  	float *SSB = (float *)ptr;
  	posix_memalign(&ptr1, 64,  N * GEMM_K *sizeof( float ));
  	float *Bc = (float *)ptr1;
  	bool *Barrier_tag = ( bool * ) malloc( Tn * ((K + GEMM_K -1) / GEMM_K) * sizeof(bool));
  	bool *Barrier_tag1 = ( bool * ) malloc( Tn * Tm * ((K + GEMM_K -1) / GEMM_K) * sizeof(bool));

  	long Num_K_block = K / GEMM_K;
	long Edge_K = K % GEMM_K;
	long Num_M_block = M / 8;
	long Edge_M = M % 8;
	long Num_blocks0 = Num_K_block * Num_M_block;
	if(Edge_M > 0)
	{
		Num_blocks0 = Num_blocks0 + Num_K_block;
		Num_M_block = Num_M_block + 1;
	}
	long Num_blocks = Num_blocks0; 
	if(Edge_K > 0)
	{
		Num_blocks = Num_blocks0 + Num_M_block;
	}

	
	#pragma omp parallel num_threads(NUM)
	{


		int i, j;
		int id = omp_get_thread_num();
		long size_block_m;
		//double start = omp_get_wtime();
		
		for( i = id; i < Num_blocks; i = i + NUM)
		{


			long start_M = (i % Num_M_block) * 8;				// note
			long start_K = (i / Num_M_block) * GEMM_K;			// note
			size_block_m = 8;

			float *AA = A + start_M * K + start_K;        		// address
			float *AAc; 

			if(Edge_M > 0 && (i % Num_M_block)==(Num_M_block-1))
			{
				size_block_m = Edge_M;
			}

			if( Edge_K > 0 && i >= Num_blocks0)
			{
				AAc = Ac + start_K * M + start_M * Edge_K;  // note
				NPACK(AA, AAc, size_block_m, Edge_K, K);
			}
			else
			{
				AAc = Ac + start_K * M + start_M * GEMM_K;  // note
				NPACK(AA, AAc, size_block_m, GEMM_K, K);
			}
		}
		
		long ii, jj, kk, iis, jjs, kks, n_to, m_to, k_to;
		long nb = N / Tn;
		long mb = M / Tm;
		long ne = N % Tn;
		long me = M % Tm;
		long nc, mc, kc;
		int index, k_index;


		if(id % Tn < ne)
		{
			nb = nb + 1;
			jjs = (id % Tn) * nb;
			n_to = jjs + nb;
		}
		else
		{
			jjs = (id % Tn) * nb + ne;
			n_to = jjs + nb;
		}
		iis = id / Tn * mb;
		m_to = iis + mb;

		kks = 0;
		k_to = K;
		long id_tag = (id < Tn)? 1:0;
		if((m_to - iis) <= GEMM_M)
			id_tag = 0;

		for(i = id; i < Tn * ((K + GEMM_K -1) / GEMM_K); i = i + NUM)
			Barrier_tag[ i ] = 0;

		for(i = id; i < Tn * Tm * ((K + GEMM_K -1) / GEMM_K); i = i + NUM)
			Barrier_tag1[ i ] = 0;
 
		#pragma omp barrier
	


		for( jj = jjs; jj < n_to; jj = jj + nc)
		{

			nc = GEMM_N;
			if(n_to - jj < GEMM_N)
			{
				nc = n_to - jj;
			}

			for(kk = kks; kk < k_to; kk = kk + kc)
			{
  				kc = GEMM_K;
  				k_index = (kk + GEMM_K -1) / GEMM_K;
  				index = NUM * k_index +	(id % Tn * Tm);
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
					long m_tag = ii - iis;

					SGEBP_NGM_NN(CC, AAc, BB, Bc, SSB, mc, nc, kc, N, K, 
								id, jjs,
								kk, m_tag, id_tag);
// barrier

					if(Tn != NUM && (m_to - iis) > GEMM_M )
					{
						if(m_tag == 0)
						{
							if(id < Tn)
								Barrier_tag[Tn * k_index + id] = 1;
							while( Barrier_tag[Tn * k_index + id % Tn] == 0)
							{
								asm volatile("nop \n");
							}
						}
						else
						{
							if(ii + mc >= m_to)
							{
								Barrier_tag1[NUM * k_index +	
											id / Tn + (id % Tn * Tm)] = 1;	
							}
						}
					}
					
				}
// barrier
				
				if(Tn != NUM && (m_to - iis) > GEMM_M )
				{
					for(i = 0; i < Tm; i++)
					{
						while(Barrier_tag1[index + i] == 0)
						{
							asm volatile("nop \n");
						}
					}
				}
				
				
			}				
		}
    }
		
	free(SSB);
	free(Ac);
	free(Bc);
	free(Barrier_tag);
	free(Barrier_tag1);
	
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
	long Num_blocks0 = Num_K_block * Tn;
	long Num_blocks = Num_blocks0; 
	if(Edge_K > 0)
	{
		Num_blocks = Num_blocks0 + Tn;
	}
	int b = 2;
	if(NUM==1)
	{
		b = 1;
	}

	#pragma omp parallel num_threads(NUM)
	{

		//double start = dclock();
		int i , j;
		int id = omp_get_thread_num();
		long nb = N / Tn;
		long ne = N % Tn;
		long mb = M / Tm;
		long me = M % Tm;
		long ii, jj, kk, iis, jjs, kks, n_to, m_to, k_to;
		long nc, mc, kc;

		iis = (id % Tm) * mb;
		m_to = iis + mb;

		if(id % Tm <me)
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

		kks = 0;
		k_to = K;

//---------------------------------------------------------------------------------------------
		for( i = id / 2; i < Num_blocks; i = i + b)
		{
			long start_N;
			if( i % Tn < ne)
			{
				nb = nb + 1;
				start_N = (i % Tn) * nb;
			}
			else
			{
				start_N = (i % Tn) * nb + ne;                 // note
			}
			 							
			long start_K = (i / Tn) * GEMM_K;					    // note
			float *BB = B + start_K * N + start_N;        		    // address_B
			float *BBc = Bc + start_K * N;       										// note address_Bc
			long Edge_N = nb % 8;

			if( Edge_K > 0 && i >= Num_blocks0)
			{

				BBc = BBc + start_N * Edge_K;       // note address_Bc
				float *addr = BBc + (nb - Edge_N) * Edge_K;


				if( id % 2 ==1)
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

					TPACK(BB, BBc, addr, Edge_K - (Edge_K / 2), nb, Edge_K, N);
				}
				
				else
				{

					TPACK(BB, BBc, addr, Edge_K / b, nb, Edge_K, N);
				}
				
				
			}
			else
			{
				BBc = BBc + start_N * GEMM_K;       		// note address_Bc
				float *addr = BBc + (nb - Edge_N) * GEMM_K;
				
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
					
					TPACK(BB, BBc, addr, GEMM_K - (GEMM_K / 2), nb, GEMM_K, N);
							
					
				}
				
				else
				{

					TPACK(BB, BBc, addr, GEMM_K/b, nb, GEMM_K, N);

				}
				
			}
		}

		#pragma omp barrier
		
		for(ii = iis; ii < m_to; ii = ii + mc)
		{

			mc = GEMM_N;
			if( m_to - ii < GEMM_N)
				mc = m_to - ii;

			for(kk = kks; kk < k_to; kk = kk + kc)
			{ 
				kc = GEMM_K;
				if(k_to - kk < GEMM_K)
					kc = k_to - kk;

				float *AA = A + ii * K + kk;

				for(jj = jjs; jj < n_to; jj = jj + nc)
				{

					nc = GEMM_M;
					if(n_to - jj < GEMM_M)
						nc= n_to - jj;
					float *BBc = Bc + kk * N + jj * kc;
					float *CC = C + ii * N + jj;

					long EDGE_mc = mc % 8;
					long LEN_mc = mc - EDGE_mc;

					
					SMM_NN_KERNEL8xN(CC, AA, BBc, LEN_mc, nc, kc, N, K, 
						&SSA[id * GEMM_K * 8], kk);

					if(EDGE_mc !=0)
					{

						float *temp_AA = AA + LEN_mc * K;
						float *temp_CC = CC + LEN_mc * N;
						if(EDGE_mc >= 4)
						{
							SMM_NN_KERNEL4xN(temp_CC, temp_AA, BBc, 4, nc, kc, N, K, 
								&SSA[id * GEMM_K * 8], kk);
							EDGE_mc = EDGE_mc - 4;
							temp_AA = temp_AA + 4 * K;
							temp_CC = temp_CC + 4 * N;
						}

						if(EDGE_mc >= 2)
						{
							SMM_NN_KERNEL2xN(temp_CC, temp_AA, BBc, 2, nc, kc, N, K, 
								&SSA[id * GEMM_K * 8], kk);
							EDGE_mc = EDGE_mc - 2;
							temp_AA = temp_AA + 2 * K;
							temp_CC = temp_CC + 2 * N;
						}

						if(EDGE_mc >= 1)
						{
							SMM_NN_KERNEL1xN(temp_CC, temp_AA, BBc, 1, nc, kc, N, K, 
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