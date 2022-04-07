#include "LibShalom.h"


void NPACK(float* A, float* Ac, long M, long K, long LK)
{
	long ii, jj, kk;
	for( ii = 0 ; ii < M; ii = ii + 8)
	{
		
		float *temp = A + ii * LK;

		asm volatile(
			"	ldr		x0, %[Ac]							\n"
			"	ldr		x1, %[K]							\n"
			"	ldr		x2, %[temp]							\n"
			"	ldr		x30, %[LK]							\n"

			"	add		x3, x2, x30, lsl #2					\n"
			"	add		x4, x2, x30, lsl #3					\n"
			"	add		x5, x3, x30, lsl #3					\n"
			"	add		x6, x4, x30, lsl #3					\n"
			"	add		x7, x5, x30, lsl #3					\n"
			"	add		x8, x6, x30, lsl #3					\n"
			"	add		x9, x7, x30, lsl #3					\n"

			"	lsr		x21, x1, #3							\n"
			"	cmp		x21, #0								\n"
			"	beq		NPACK_END							\n"

			"NPACK:											\n"

			"	prfm	PLDL1KEEP, [x2, #128]				\n"
			"	prfm	PLDL1KEEP, [x3, #128]				\n"

			"	ldr		q0, [x2], #16						\n"
			"	ldr		q1, [x3], #16						\n"
			"	ldr		q2, [x4], #16						\n"
			"	ldr		q3, [x5], #16						\n"

			"	prfm	PLDL1KEEP, [x4, #128]				\n"
			"	prfm	PLDL1KEEP, [x5, #128]				\n"

			"	st4		{v0.s, v1.s, v2.s, v3.s}[0], [x0], #16	\n"

			"	ldr		q4, [x6], #16						\n"
			"	ldr		q5, [x7], #16						\n"
			"	ldr		q6, [x8], #16						\n"
			"	ldr		q7, [x9], #16						\n"

			"	st4		{v4.s, v5.s, v6.s, v7.s}[0], [x0], #16	\n"

			"	prfm	PLDL1KEEP, [x6, #128]				\n"
			"	prfm	PLDL1KEEP, [x7, #128]				\n"

			"	ldr		q8, [x2], #16						\n"
			"	st4		{v0.s, v1.s, v2.s, v3.s}[1], [x0], #16	\n"
			"	ldr		q9, [x3], #16						\n"
			"	st4		{v4.s, v5.s, v6.s, v7.s}[1], [x0], #16	\n"
			"	ldr		q10, [x4], #16						\n"
			"	st4		{v0.s, v1.s, v2.s, v3.s}[2], [x0], #16	\n"
			"	ldr		q11, [x5], #16						\n"
			"	st4		{v4.s, v5.s, v6.s, v7.s}[2], [x0], #16	\n"

			"	prfm	PLDL1KEEP, [x8, #128]				\n"
			"	prfm	PLDL1KEEP, [x9, #128]				\n"


			"	ldr		q12, [x6], #16						\n"
			"	st4		{v0.s, v1.s, v2.s, v3.s}[3], [x0], #16		\n"
			"	ldr		q13, [x7], #16						\n"
			"	st4		{v4.s, v5.s, v6.s, v7.s}[3], [x0], #16		\n"
			"	ldr		q14, [x8], #16						\n"
			"	st4		{v8.s, v9.s, v10.s, v11.s}[0], [x0], #16	\n"
			"	ldr		q15, [x9], #16						\n"
			"	st4		{v12.s, v13.s, v14.s, v15.s}[0], [x0], #16	\n"

			"	subs	x21, x21, #1						\n"

			"	st4		{v8.s, v9.s, v10.s, v11.s}[1], [x0], #16	\n"
			"	st4		{v12.s, v13.s, v14.s, v15.s}[1], [x0], #16	\n"
			"	st4		{v8.s, v9.s, v10.s, v11.s}[2], [x0], #16	\n"
			"	st4		{v12.s, v13.s, v14.s, v15.s}[2], [x0], #16	\n"
			"	st4		{v8.s, v9.s, v10.s, v11.s}[3], [x0], #16	\n"
			"	st4		{v12.s, v13.s, v14.s, v15.s}[3], [x0], #16	\n"

			"	bgt		NPACK 								\n"

			"	ands	x22, x1, #7							\n"
			"	beq		NPACK_END							\n"

			"	cmp		x22, #4								\n"
			"	blt 	K1_NPACK							\n"


			"K4_NPACK:										\n"

			"	ldr		q0, [x2], #16						\n"
			"	ldr		q1, [x3], #16						\n"
			"	ldr		q2, [x4], #16						\n"
			"	ldr		q3, [x5], #16						\n"

			"	st4		{v0.s, v1.s, v2.s, v3.s}[0], [x0], #16	\n"
			"	ldr		q4, [x6], #16						\n"
			"	st4		{v0.s, v1.s, v2.s, v3.s}[1], [x0], #16	\n"
			"	ldr		q5, [x7], #16						\n"
			"	st4		{v0.s, v1.s, v2.s, v3.s}[2], [x0], #16	\n"
			"	ldr		q6, [x8], #16						\n"
			"	st4		{v0.s, v1.s, v2.s, v3.s}[3], [x0], #16	\n"
			"	ldr		q7, [x9], #16						\n"

			"	st4		{v4.s, v5.s, v6.s, v7.s}[0], [x0], #16	\n"
			"	st4		{v4.s, v5.s, v6.s, v7.s}[1], [x0], #16	\n"
			"	st4		{v4.s, v5.s, v6.s, v7.s}[2], [x0], #16	\n"
			"	st4		{v4.s, v5.s, v6.s, v7.s}[3], [x0], #16	\n"

			"	subs	x22, x22, #4						\n"
			"	beq		NPACK_END							\n"

			"K1_NPACK:										\n"

			"	ldr		s0, [x2], #4						\n"
			"	ldr		s1, [x3], #4						\n"
			"	ldr		s2, [x4], #4						\n"
			"	ldr		s3, [x5], #4						\n"

			"	subs 	x22, x22, #1						\n"

			"	st4		{v0.s, v1.s, v2.s, v3.s}[0], [x0], #16	\n"
			"	ldr		s4, [x6], #4						\n"
			"	ldr		s5, [x7], #4						\n"
			"	ldr		s6, [x8], #4						\n"
			"	ldr		s7, [x9], #4						\n"

			"	st4		{v4.s, v5.s, v6.s, v7.s}[0], [x0], #16	\n"

			"	bgt		K1_NPACK							\n"



			"NPACK_END:										\n"

			:
			:
				[temp] "m" (temp),
				[Ac] "m" (Ac),
				[K] "m" (K),
				[LK] "m" (LK)
			:
				"x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
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


void TPACK(float* B, float* Bc, float* addr, long K, long N, long LK,long LN)
{
	long jj, kk;
	for( kk = 0 ; kk < K; )
	{
	
		float *temp = B + kk * LN;
		long flag = K - kk;

		if( flag >= 8 )
		{

			asm volatile(

					"	ldr		x0, %[Bc]				\n"
					"	ldr		x1, %[K]				\n"
					"	ldr		x2, %[temp]				\n"
					"	ldr 	x3, %[N] 				\n"
					"	ldr		x29, %[LK]				\n"
					"	ldr		x30, %[LN]				\n"

					"	ldr 	x28, %[kk]				\n"
					"	ldr 	x27, %[addr]			\n"


					"	lsr 	x20, x3, #3 			\n"

					"	add 	x4, x2, x30, lsl #2 	\n"
					"	add 	x5, x4, x30, lsl #2 	\n"
					"	add 	x6, x5, x30, lsl #2 	\n"
					"	add 	x7, x6, x30, lsl #2 	\n"
					"	prfm	PLDL1KEEP, [x2, #128]	\n"
					"	prfm	PLDL1KEEP, [x4, #128]	\n"
					"	add 	x8, x7, x30, lsl #2 	\n"
					"	add 	x9, x8, x30, lsl #2 	\n"
					"	add 	x10, x9, x30, lsl #2 	\n"
					"	prfm	PLDL1KEEP, [x5, #128]	\n"
					"	prfm	PLDL1KEEP, [x6, #128]	\n"

					"TPACK8:							\n"

					"	mov		x11, x0 				\n"

					"	prfm	PLDL2KEEP, [x2, #128]	\n"
					"	prfm	PLDL2KEEP, [x4, #128]	\n"

					"	ldp		q0, q1, [x2], #32		\n"
					"	ldp		q2, q3, [x4], #32		\n"
					"	ldp		q4, q5, [x5], #32		\n"
					"	ldp		q6, q7, [x6], #32		\n"

					"	prfm	PLDL2KEEP, [x5, #128]	\n"
					"	prfm	PLDL2KEEP, [x6, #128]	\n"

					"	stp 	q0, q1, [x11]			\n"
					"	ldp		q8, q9, [x7], #32 		\n"
					"	stp 	q2, q3, [x11, #32]		\n"
					"	ldp		q10, q11, [x8], #32 	\n"
					"	stp 	q4, q5, [x11, #64]		\n"
					"	ldp		q12, q13, [x9], #32 	\n"
					"	stp 	q6, q7, [x11, #96]		\n"
					"	ldp		q14, q15, [x10], #32 	\n"

					"	add  	x11, x11, #128 			\n"
					"	prfm	PLDL2KEEP, [x7, #128]	\n"
					"	prfm	PLDL2KEEP, [x8, #128]	\n"

					"	subs	x20, x20, #1 			\n"
					"	stp 	q8, q9, [x11]			\n"
					"	stp 	q10, q11, [x11, #32]	\n"
					"	stp 	q12, q13, [x11, #64]	\n"
					"	stp 	q14, q15, [x11, #96]	\n"
					"	prfm	PLDL2KEEP, [x9, #128]	\n"
					"	prfm	PLDL2KEEP, [x10, #128]	\n"

					"	add  	x0, x0, x29, lsl #5 	\n"
					"	bgt		TPACK8 					\n"


//---------------------------------------------------------------
					"	ands	x20, x3, #7 			\n"
					"	beq		TPACK_N_END 			\n"

					"	cmp		x20, #4 				\n"
					"	blt		TPACK_N2				\n"

					"	mov 	x11, x27 				\n"
					"	add 	x11, x11, x28, lsl #4 	\n"
					"	ldr 	q0, [x2], #16 			\n"
					"	ldr 	q1, [x4], #16 			\n"
					"	ldr 	q2, [x5], #16 			\n"
					"	ldr 	q3, [x6], #16 			\n"

					"	ldr 	q4, [x7], #16 			\n"
					"	stp 	q0, q1, [x11]			\n"
					"	ldr 	q5, [x8], #16 			\n"
					"	stp 	q2, q3, [x11, #32]		\n"
					"	ldr 	q6, [x9], #16 			\n"
					"	stp 	q4, q5, [x11, #64]		\n"
					"	ldr 	q7, [x10], #16 			\n"
					"	stp 	q6, q7, [x11, #96]		\n"

					"	add  	x27, x27, x29, lsl #4 	\n"
					"	sub 	x20, x20, #4 			\n"

					"TPACK_N2:							\n"

					"	cmp		x20, #2 				\n"
					"	blt		TPACK_N1 				\n"

					"	mov 	x11, x27 				\n"
					"	add    	x11, x11, x28, lsl #3	\n"
					"	ld1 	{v0.2s}, [x2], #8 		\n"
					"	ld1 	{v1.2s}, [x4], #8 		\n"
					"	ld1 	{v2.2s}, [x5], #8 		\n"
					"	ld1 	{v3.2s}, [x6], #8 		\n"

					"	ld1 	{v4.2s}, [x7], #8 	    \n"
					"	st2 	{v0.2s, v1.2s}, [x11], #16			\n"
					"	ld1 	{v5.2s}, [x8], #8 		\n"
					"	st2 	{v2.2s, v3.2s}, [x11], #16			\n"
					"	ld1 	{v6.2s}, [x9], #8 		\n"
					"	st2 	{v4.2s, v5.2s}, [x11], #16			\n"
					"	ld1 	{v7.2s}, [x10], #8 		\n"
					"	st2 	{v6.2s, v7.2s}, [x11], #16			\n"

					"	add  	x27, x27, x29, lsl #3 	\n"
					"	sub 	x20, x20, #2 			\n"



					"TPACK_N1: 							\n"
					"	cmp		x20, #1 				\n"
					"	blt		TPACK_N_END 			\n"

					"	mov 	x11, x27 				\n"
					"	add    	x11, x11, x28, lsl #2	\n"
					"	ld1 	{v0.s}[0], [x2] 		\n"
					"	ld1 	{v0.s}[1], [x4] 		\n"
					"	ld1 	{v0.s}[2], [x5] 		\n"
					"	ld1 	{v0.s}[3], [x6] 		\n"

					"	ld1 	{v1.s}[0], [x7] 	 	\n"
					"	ld1 	{v1.s}[1], [x8] 		\n"
					"	ld1 	{v1.s}[2], [x9] 		\n"		
					"	ld1 	{v1.s}[3], [x10] 		\n"

					"	stp 	q0, q1, [x11] 			\n"
			


					"TPACK_N_END: 						\n"



					:
	          		:
	          		[temp] "m" (temp),
	          		[Bc] "m" (Bc),
	          		[K] "m" (K),
	          		[LK] "m" (LK),
	          		[LN] "m" (LN),
	          		[N] "m" (N),
	          		[kk] "m" (kk),
	          		[addr] "m" (addr)
	                :"x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
	                "x9", "x10", "x11", "x12", "x13","x14", "x15", "x16",
	                "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24","x25",
	                "x26", "x27", "x28", "x29", "x30",
	                "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
	                "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
	                "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
	                "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"
				);

			Bc = Bc + 8 * 8;
			kk = kk + 8;

		}

		else
		{
			asm volatile(

				"	ldr		x0, %[Bc]				\n"
				"	ldr		x1, %[K]				\n"
				"	ldr		x2, %[temp]				\n"
				"	ldr 	x3, %[N] 				\n"
				"	ldr		x30, %[LN]				\n"
				"	ldr		x29, %[LK]				\n"

				"	ldr 	x28, %[kk]				\n"
				"	ldr 	x27, %[addr]			\n"

				"	prfm	PLDL1KEEP, [x2, #128]	\n"

				"	lsr 	x20, x3, #3 			\n"


				"EDGE_TPACK:						\n"

				"	subs	x20, x20, #1 			\n"
				"	prfm	PLDL2KEEP, [x2, #128]	\n"
				"	ldp		q0, q1, [x2], #32		\n"
				"	stp 	q0, q1, [x0]			\n"

				"	add  	x0, x0, x29, lsl #5 	\n"
				"	bgt		EDGE_TPACK 				\n"
//---------------------------------------------------------



				"	ands 	x20, x3, #7 			\n"
				"	beq		EDGE_N_END				\n"

				"	cmp		x20, #4 				\n"
				"	blt		EDGE_TPACK_N2			\n"

				"	mov 	x11, x27 				\n"
				"	add 	x11, x11, x28, lsl #4 	\n"

				"	ldr		q0, [x2], #16			\n"
				"	str 	q0, [x11]				\n"
				"	add  	x27, x27, x29, lsl #4 	\n"
				"	sub 	x20, x20, #4 			\n"

				"EDGE_TPACK_N2:						\n"
				"	cmp 	x20, #2 				\n"
				"	blt 	EDGE_TPACK_N1 			\n"

				"	mov 	x11, x27 				\n"
				"	add 	x11, x11, x28, lsl #3 	\n"

				"	ld1		{v0.2s}, [x2], #8		\n"
				"	st1 	{v0.2s}, [x11]			\n"
				"	add  	x27, x27, x29, lsl #3 	\n"
				"	sub 	x20, x20, #2 			\n"

				"EDGE_TPACK_N1: 					\n"

				"	cmp 	x20, #1 				\n"
				"	blt 	EDGE_N_END 				\n"

				"	mov 	x11, x27 				\n"
				"	add 	x11, x11, x28, lsl #2 	\n"
				"	ld1		{v0.s}[0], [x2]			\n"
				"	st1 	{v0.s}[0], [x11]	    \n"

				"EDGE_N_END: 						\n"

				:
          		:
          		[temp] "m" (temp),
          		[Bc] "m" (Bc),
          		[K] "m" (K),
          		[LK] "m" (LK),
          		[LN] "m" (LN),
          		[N] "m" (N),
          		[kk] "m" (kk),
          		[addr] "m" (addr)
                :"x0", "x1", "x2", "x3","x20", "x29", "x30", "v0", "v1"
			);

			Bc = Bc + 8;
			kk = kk + 1;
		}
	}

}

void Sin_NPACK(float* A, float* Ac, long M, long K, long LK)
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
					"	ldr		x0, %[Ac]						\n"
					"	ldr		x1, %[Kc]						\n"
					"	ldr		x2, %[temp]						\n"
					"	ldr		x30, %[LK]						\n"

					"	add		x3, x2, x30, lsl #2				\n"
					"	add		x4, x2, x30, lsl #3				\n"
					"	add		x5, x3, x30, lsl #3				\n"
					"	add		x6, x4, x30, lsl #3				\n"
					"	add		x7, x5, x30, lsl #3				\n"
					"	add		x8, x6, x30, lsl #3				\n"
					"	add		x9, x7, x30, lsl #3				\n"



					"	lsr		x21, x1, #3						\n"
					"	cmp		x21, #0							\n"
					"	beq		Sin_NPACK_END					\n"

					"Sin_NPACK:									\n"

					"	prfm	PLDL1KEEP, [x2, #128]			\n"
					"	prfm	PLDL1KEEP, [x3, #128]			\n"

					"	ldr		q0, [x2], #16					\n"
					"	ldr		q1, [x3], #16					\n"
					"	ldr		q2, [x4], #16					\n"
					"	ldr		q3, [x5], #16					\n"

					"	prfm	PLDL1KEEP, [x4, #128]			\n"
					"	prfm	PLDL1KEEP, [x5, #128]			\n"

					"	st4		{v0.s, v1.s, v2.s, v3.s}[0], [x0], #16	\n"

					"	ldr		q4, [x6], #16					\n"
					"	ldr		q5, [x7], #16					\n"
					"	ldr		q6, [x8], #16					\n"
					"	ldr		q7, [x9], #16					\n"

					"	st4		{v4.s, v5.s, v6.s, v7.s}[0], [x0], #16	\n"

					"	prfm	PLDL1KEEP, [x6, #128]			\n"
					"	prfm	PLDL1KEEP, [x7, #128]			\n"

					"	ldr		q8, [x2], #16					\n"
					"	st4		{v0.s, v1.s, v2.s, v3.s}[1], [x0], #16	\n"
					"	ldr		q9, [x3], #16					\n"
					"	st4		{v4.s, v5.s, v6.s, v7.s}[1], [x0], #16	\n"
					"	ldr		q10, [x4], #16					\n"
					"	st4		{v0.s, v1.s, v2.s, v3.s}[2], [x0], #16	\n"
					"	ldr		q11, [x5], #16					\n"
					"	st4		{v4.s, v5.s, v6.s, v7.s}[2], [x0], #16	\n"

					"	prfm	PLDL1KEEP, [x8, #128]			\n"
					"	prfm	PLDL1KEEP, [x9, #128]			\n"


					"	ldr		q12, [x6], #16					\n"
					"	st4		{v0.s, v1.s, v2.s, v3.s}[3], [x0], #16		\n"
					"	ldr		q13, [x7], #16					\n"
					"	st4		{v4.s, v5.s, v6.s, v7.s}[3], [x0], #16		\n"
					"	ldr		q14, [x8], #16					\n"
					"	st4		{v8.s, v9.s, v10.s, v11.s}[0], [x0], #16	\n"
					"	ldr		q15, [x9], #16					\n"
					"	st4		{v12.s, v13.s, v14.s, v15.s}[0], [x0], #16	\n"

					"	subs	x21, x21, #1					\n"

					"	st4		{v8.s, v9.s, v10.s, v11.s}[1], [x0], #16	\n"
					"	st4		{v12.s, v13.s, v14.s, v15.s}[1], [x0], #16	\n"
					"	st4		{v8.s, v9.s, v10.s, v11.s}[2], [x0], #16	\n"
					"	st4		{v12.s, v13.s, v14.s, v15.s}[2], [x0], #16	\n"
					"	st4		{v8.s, v9.s, v10.s, v11.s}[3], [x0], #16	\n"
					"	st4		{v12.s, v13.s, v14.s, v15.s}[3], [x0], #16	\n"

					"	bgt		Sin_NPACK 						\n"

					"	ands	x22, x1, #7						\n"
					"	beq		Sin_NPACK_END					\n"

					"	cmp		x22, #4							\n"
					"	blt 	Sin_K1_NPACK					\n"


					"Sin_K4_NPACK:								\n"

					"	ldr		q0, [x2], #16					\n"
					"	ldr		q1, [x3], #16					\n"
					"	ldr		q2, [x4], #16					\n"
					"	ldr		q3, [x5], #16					\n"

					"	st4		{v0.s, v1.s, v2.s, v3.s}[0], [x0], #16	\n"
					"	ldr		q4, [x6], #16					\n"
					"	st4		{v0.s, v1.s, v2.s, v3.s}[1], [x0], #16	\n"
					"	ldr		q5, [x7], #16					\n"
					"	st4		{v0.s, v1.s, v2.s, v3.s}[2], [x0], #16	\n"
					"	ldr		q6, [x8], #16					\n"
					"	st4		{v0.s, v1.s, v2.s, v3.s}[3], [x0], #16	\n"
					"	ldr		q7, [x9], #16					\n"

					"	st4		{v4.s, v5.s, v6.s, v7.s}[0], [x0], #16	\n"
					"	st4		{v4.s, v5.s, v6.s, v7.s}[1], [x0], #16	\n"
					"	st4		{v4.s, v5.s, v6.s, v7.s}[2], [x0], #16	\n"
					"	st4		{v4.s, v5.s, v6.s, v7.s}[3], [x0], #16	\n"

					"	subs	x22, x22, #4					\n"
					"	beq		Sin_NPACK_END					\n"

					"Sin_K1_NPACK:								\n"

					"	ldr		s0, [x2], #4					\n"
					"	ldr		s1, [x3], #4					\n"
					"	ldr		s2, [x4], #4					\n"
					"	ldr		s3, [x5], #4					\n"

					"	subs 	x22, x22, #1					\n"

					"	st4		{v0.s, v1.s, v2.s, v3.s}[0], [x0], #16	\n"
					"	ldr		s4, [x6], #4					\n"
					"	ldr		s5, [x7], #4					\n"
					"	ldr		s6, [x8], #4					\n"
					"	ldr		s7, [x9], #4					\n"

					"	st4		{v4.s, v5.s, v6.s, v7.s}[0], [x0], #16	\n"

					"	bgt		Sin_K1_NPACK					\n"



					"Sin_NPACK_END:								\n"

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
