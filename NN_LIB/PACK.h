#include <stdio.h>
#include <cblas.h>
#include <sys/time.h>
#include <stdlib.h>
#include <omp.h>


#define NUM 64

#define GEMM_K 320
#define GEMM_N 4096
#define GEMM_M 256

void PACKA(float* A, float* Ac, long M, long K, long LK)
{
	long ii, jj, kk;
	for( ii = 0 ; ii < M; ii = ii + 8)
	{


		float *temp = A + ii * LK + kk;

		asm volatile(
				"	ldr		x0, %[Ac]							\n"
				"	ldr		x1, %[K]							\n"
				"	ldr		x2, %[temp]						\n"
				"	ldr		x30, %[LK]						\n"

				"	add		x3, x2, x30, lsl #2		\n"
				"	add		x4, x2, x30, lsl #3		\n"
				"	add		x5, x3, x30, lsl #3		\n"
				"	add		x6, x4, x30, lsl #3		\n"
				"	add		x7, x5, x30, lsl #3		\n"
				"	add		x8, x6, x30, lsl #3		\n"
				"	add		x9, x7, x30, lsl #3		\n"



				"	lsr		x21, x1, #3						\n"
				"	cmp		x21, #0								\n"
				"	beq		PACKA_END							\n"

				"PACKA:												\n"

				"	prfm	PLDL1KEEP, [x2, #128]	\n"
				"	prfm	PLDL1KEEP, [x3, #128]	\n"

				"	ldr		q0, [x2], #16					\n"
				"	ldr		q1, [x3], #16					\n"
				"	ldr		q2, [x4], #16					\n"
				"	ldr		q3, [x5], #16					\n"

				"	prfm	PLDL1KEEP, [x4, #128]	\n"
				"	prfm	PLDL1KEEP, [x5, #128]	\n"

				"	st4		{v0.s, v1.s, v2.s, v3.s}[0], [x0], #16	\n"

				"	ldr		q4, [x6], #16					\n"
				"	ldr		q5, [x7], #16					\n"
				"	ldr		q6, [x8], #16					\n"
				"	ldr		q7, [x9], #16					\n"

				"	st4		{v4.s, v5.s, v6.s, v7.s}[0], [x0], #16	\n"

				"	prfm	PLDL1KEEP, [x6, #128]	\n"
				"	prfm	PLDL1KEEP, [x7, #128]	\n"

				"	ldr		q8, [x2], #16					\n"
				"	st4		{v0.s, v1.s, v2.s, v3.s}[1], [x0], #16	\n"
				"	ldr		q9, [x3], #16					\n"
				"	st4		{v4.s, v5.s, v6.s, v7.s}[1], [x0], #16	\n"
				"	ldr		q10, [x4], #16				\n"
				"	st4		{v0.s, v1.s, v2.s, v3.s}[2], [x0], #16	\n"
				"	ldr		q11, [x5], #16				\n"
				"	st4		{v4.s, v5.s, v6.s, v7.s}[2], [x0], #16	\n"

				"	prfm	PLDL1KEEP, [x8, #128]	\n"
				"	prfm	PLDL1KEEP, [x9, #128]	\n"


				"	ldr		q12, [x6], #16				\n"
				"	st4		{v0.s, v1.s, v2.s, v3.s}[3], [x0], #16		\n"
				"	ldr		q13, [x7], #16				\n"
				"	st4		{v4.s, v5.s, v6.s, v7.s}[3], [x0], #16		\n"
				"	ldr		q14, [x8], #16				\n"
				"	st4		{v8.s, v9.s, v10.s, v11.s}[0], [x0], #16	\n"
				"	ldr		q15, [x9], #16				\n"
				"	st4		{v12.s, v13.s, v14.s, v15.s}[0], [x0], #16	\n"

				"	subs	x21, x21, #1					\n"

				"	st4		{v8.s, v9.s, v10.s, v11.s}[1], [x0], #16	\n"
				"	st4		{v12.s, v13.s, v14.s, v15.s}[1], [x0], #16	\n"
				"	st4		{v8.s, v9.s, v10.s, v11.s}[2], [x0], #16	\n"
				"	st4		{v12.s, v13.s, v14.s, v15.s}[2], [x0], #16	\n"
				"	st4		{v8.s, v9.s, v10.s, v11.s}[3], [x0], #16	\n"
				"	st4		{v12.s, v13.s, v14.s, v15.s}[3], [x0], #16	\n"

				"	bgt		PACKA 						\n"

				"	ands	x22, x1, #7				\n"
				"	beq		PACKA_END					\n"

				"	cmp		x22, #4						\n"
				"	blt 	K1_PACKA					\n"


				"K4_PACKA:								\n"

				"	ldr		q0, [x2], #16			\n"
				"	ldr		q1, [x3], #16			\n"
				"	ldr		q2, [x4], #16			\n"
				"	ldr		q3, [x5], #16			\n"

				"	st4		{v0.s, v1.s, v2.s, v3.s}[0], [x0], #16	\n"
				"	ldr		q4, [x6], #16			\n"
				"	st4		{v0.s, v1.s, v2.s, v3.s}[1], [x0], #16	\n"
				"	ldr		q5, [x7], #16			\n"
				"	st4		{v0.s, v1.s, v2.s, v3.s}[2], [x0], #16	\n"
				"	ldr		q6, [x8], #16			\n"
				"	st4		{v0.s, v1.s, v2.s, v3.s}[3], [x0], #16	\n"
				"	ldr		q7, [x9], #16			\n"

				"	st4		{v4.s, v5.s, v6.s, v7.s}[0], [x0], #16	\n"
				"	st4		{v4.s, v5.s, v6.s, v7.s}[1], [x0], #16	\n"
				"	st4		{v4.s, v5.s, v6.s, v7.s}[2], [x0], #16	\n"
				"	st4		{v4.s, v5.s, v6.s, v7.s}[3], [x0], #16	\n"

				"	subs	x22, x22, #4			\n"
				"	beq		PACKA_END					\n"

				"K1_PACKA:								\n"

				"	ldr		s0, [x2], #4			\n"
				"	ldr		s1, [x3], #4			\n"
				"	ldr		s2, [x4], #4			\n"
				"	ldr		s3, [x5], #4			\n"

				"	subs 	x22, x22, #1			\n"

				"	st4		{v0.s, v1.s, v2.s, v3.s}[0], [x0], #16	\n"
				"	ldr		s4, [x6], #4			\n"
				"	ldr		s5, [x7], #4			\n"
				"	ldr		s6, [x8], #4			\n"
				"	ldr		s7, [x9], #4			\n"

				"	st4		{v4.s, v5.s, v6.s, v7.s}[0], [x0], #16	\n"

				"	bgt		K1_PACKA				\n"



				"PACKA_END:							\n"

				:
				:
				[temp] "m" (temp),
				[Ac] "m" (Ac),
				[K] "m" (K),
				[LK] "m" (LK)
				:"x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
				"x9", "x10", "x11", "x12", "x13","x14", "x15", "x16",
				"x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24","x25",
				"x26", "x27", "x28", "x29", "x30",
				"v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
				"v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
				"v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
				"v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"

		);

		Ac = Ac + K * 8;

	}

}


void Sin_PACK(float* A, float* Ac, long M, long K, long LK)
{
	long ii, jj, kk;
	long Kc;


	for( kk =0 ; kk < K; kk = kk + Kc)
	{
		
		Kc = GEMM_K;
		if(K - kk < GEMM_K)
			Kc= K - kk;

		for( ii = 0 ; ii < M; ii = ii + 8)
		{


			float *temp = A + ii * LK + kk;

			asm volatile(
					"	ldr		x0, %[Ac]							\n"
					"	ldr		x1, %[Kc]							\n"
					"	ldr		x2, %[temp]						\n"
					"	ldr		x30, %[LK]						\n"

					"	add		x3, x2, x30, lsl #2		\n"
					"	add		x4, x2, x30, lsl #3		\n"
					"	add		x5, x3, x30, lsl #3		\n"
					"	add		x6, x4, x30, lsl #3		\n"
					"	add		x7, x5, x30, lsl #3		\n"
					"	add		x8, x6, x30, lsl #3		\n"
					"	add		x9, x7, x30, lsl #3		\n"



					"	lsr		x21, x1, #3						\n"
					"	cmp		x21, #0								\n"
					"	beq		Sin_PACKA_END					\n"

					"Sin_PACKA:										\n"

					"	prfm	PLDL1KEEP, [x2, #128]	\n"
					"	prfm	PLDL1KEEP, [x3, #128]	\n"

					"	ldr		q0, [x2], #16					\n"
					"	ldr		q1, [x3], #16					\n"
					"	ldr		q2, [x4], #16					\n"
					"	ldr		q3, [x5], #16					\n"

					"	prfm	PLDL1KEEP, [x4, #128]	\n"
					"	prfm	PLDL1KEEP, [x5, #128]	\n"

					"	st4		{v0.s, v1.s, v2.s, v3.s}[0], [x0], #16	\n"

					"	ldr		q4, [x6], #16					\n"
					"	ldr		q5, [x7], #16					\n"
					"	ldr		q6, [x8], #16					\n"
					"	ldr		q7, [x9], #16					\n"

					"	st4		{v4.s, v5.s, v6.s, v7.s}[0], [x0], #16	\n"

					"	prfm	PLDL1KEEP, [x6, #128]	\n"
					"	prfm	PLDL1KEEP, [x7, #128]	\n"

					"	ldr		q8, [x2], #16					\n"
					"	st4		{v0.s, v1.s, v2.s, v3.s}[1], [x0], #16	\n"
					"	ldr		q9, [x3], #16					\n"
					"	st4		{v4.s, v5.s, v6.s, v7.s}[1], [x0], #16	\n"
					"	ldr		q10, [x4], #16				\n"
					"	st4		{v0.s, v1.s, v2.s, v3.s}[2], [x0], #16	\n"
					"	ldr		q11, [x5], #16				\n"
					"	st4		{v4.s, v5.s, v6.s, v7.s}[2], [x0], #16	\n"

					"	prfm	PLDL1KEEP, [x8, #128]	\n"
					"	prfm	PLDL1KEEP, [x9, #128]	\n"


					"	ldr		q12, [x6], #16				\n"
					"	st4		{v0.s, v1.s, v2.s, v3.s}[3], [x0], #16		\n"
					"	ldr		q13, [x7], #16				\n"
					"	st4		{v4.s, v5.s, v6.s, v7.s}[3], [x0], #16		\n"
					"	ldr		q14, [x8], #16				\n"
					"	st4		{v8.s, v9.s, v10.s, v11.s}[0], [x0], #16	\n"
					"	ldr		q15, [x9], #16				\n"
					"	st4		{v12.s, v13.s, v14.s, v15.s}[0], [x0], #16	\n"

					"	subs	x21, x21, #1					\n"

					"	st4		{v8.s, v9.s, v10.s, v11.s}[1], [x0], #16	\n"
					"	st4		{v12.s, v13.s, v14.s, v15.s}[1], [x0], #16	\n"
					"	st4		{v8.s, v9.s, v10.s, v11.s}[2], [x0], #16	\n"
					"	st4		{v12.s, v13.s, v14.s, v15.s}[2], [x0], #16	\n"
					"	st4		{v8.s, v9.s, v10.s, v11.s}[3], [x0], #16	\n"
					"	st4		{v12.s, v13.s, v14.s, v15.s}[3], [x0], #16	\n"

					"	bgt		Sin_PACKA 				\n"

					"	ands	x22, x1, #7				\n"
					"	beq		Sin_PACKA_END			\n"

					"	cmp		x22, #4						\n"
					"	blt 	Sin_K1_PACKA			\n"


					"Sin_K4_PACKA:						\n"

					"	ldr		q0, [x2], #16			\n"
					"	ldr		q1, [x3], #16			\n"
					"	ldr		q2, [x4], #16			\n"
					"	ldr		q3, [x5], #16			\n"

					"	st4		{v0.s, v1.s, v2.s, v3.s}[0], [x0], #16	\n"
					"	ldr		q4, [x6], #16			\n"
					"	st4		{v0.s, v1.s, v2.s, v3.s}[1], [x0], #16	\n"
					"	ldr		q5, [x7], #16			\n"
					"	st4		{v0.s, v1.s, v2.s, v3.s}[2], [x0], #16	\n"
					"	ldr		q6, [x8], #16			\n"
					"	st4		{v0.s, v1.s, v2.s, v3.s}[3], [x0], #16	\n"
					"	ldr		q7, [x9], #16			\n"

					"	st4		{v4.s, v5.s, v6.s, v7.s}[0], [x0], #16	\n"
					"	st4		{v4.s, v5.s, v6.s, v7.s}[1], [x0], #16	\n"
					"	st4		{v4.s, v5.s, v6.s, v7.s}[2], [x0], #16	\n"
					"	st4		{v4.s, v5.s, v6.s, v7.s}[3], [x0], #16	\n"

					"	subs	x22, x22, #4			\n"
					"	beq		Sin_PACKA_END			\n"

					"Sin_K1_PACKA:						\n"

					"	ldr		s0, [x2], #4			\n"
					"	ldr		s1, [x3], #4			\n"
					"	ldr		s2, [x4], #4			\n"
					"	ldr		s3, [x5], #4			\n"

					"	subs 	x22, x22, #1			\n"

					"	st4		{v0.s, v1.s, v2.s, v3.s}[0], [x0], #16	\n"
					"	ldr		s4, [x6], #4			\n"
					"	ldr		s5, [x7], #4			\n"
					"	ldr		s6, [x8], #4			\n"
					"	ldr		s7, [x9], #4			\n"

					"	st4		{v4.s, v5.s, v6.s, v7.s}[0], [x0], #16	\n"

					"	bgt		Sin_K1_PACKA			\n"



					"Sin_PACKA_END:						\n"

					:
	        :
	      		[temp] "m" (temp),
	      		[Ac] "m" (Ac),
	      		[Kc] "m" (Kc),
	      		[LK] "m" (LK)
	        :"x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
	        "x9", "x10", "x11", "x12", "x13","x14", "x15", "x16",
	        "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24","x25",
	        "x26", "x27", "x28", "x29", "x30",
	        "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
	        "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
	        "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
	        "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"

			);

			Ac = Ac + Kc * 8;

		}
	}
}
