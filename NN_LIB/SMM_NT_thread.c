#include<stdio.h>
#include <stdlib.h>
#include <omp.h>
#include<math.h>
#include "LibShalom.h"

int Tm, Tn, T;

void SGEMM_NT_KERNEL_MP(float *C, float *A, float *B, long	M, long N, long K, 
			long LN, long LK, float *SB, long k_tag)
{
	asm volatile(
		".macro PACK_KERNEL5x4_BEGIN_K             	 \n"
		"										 	 \n"

		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q1, [x12], #16				     \n"
		"	ldr		q5, [x16], #16					 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x14, #64] 			 \n"
		"	prfm	PLDL1KEEP, [x15, #64]			 \n"

		"	prfm 	PLDL1KEEP, [x17, #128]			 \n"

		"	fmul	v12.4s, v0.4s, v5.4s			 \n"
		"	ldr		q2, [x13], #16					 \n"
		"	fmul	v13.4s, v1.4s, v5.4s			 \n"

		"	ldr		q3, [x14], #16					 \n"

		"	fmul	v14.4s, v2.4s, v5.4s		 	 \n"
		"	ldr		q4, [x15], #16					 \n"
		"	fmul	v15.4s, v3.4s, v5.4s			 \n"

		"	ldr		q6, [x17], #16					 \n"
		"	fmul	v16.4s, v4.4s, v5.4s			 \n"
		"	ldr		q7, [x18], #16					 \n"
		"	fmul	v17.4s, v0.4s, v6.4s			 \n"

		"	ldr		q8, [x19], #16					 \n"
		"	ldr		q9, [x11], #16					 \n"
		"	st4		{v5.s, v6.s, v7.s, v8.s}[0], [x24]		\n"
		"	add		x24, x24, x8, lsl #2			 \n"

		"	fmul	v18.4s, v1.4s, v6.4s			 \n"
		"	fmul	v19.4s, v2.4s, v6.4s			 \n"
		"	fmul	v20.4s, v3.4s, v6.4s			 \n"
		"	fmul	v21.4s, v4.4s, v6.4s			 \n"

		"	ldr		q10, [x12], #16					 \n"
		"	st4		{v5.s, v6.s, v7.s, v8.s}[1], [x24]		\n"
		"	add		x24, x24, x8, lsl #2			 \n"

		"	fmul	v22.4s, v0.4s, v7.4s			 \n"
		"	fmul	v23.4s, v1.4s, v7.4s			 \n"
		"	fmul	v24.4s, v2.4s, v7.4s			 \n"
		"	fmul	v25.4s, v3.4s, v7.4s			 \n"
		"	fmul	v26.4s, v4.4s, v7.4s			 \n"

		"	ldr		q11, [x13], #16					 \n"
		"	st4		{v5.s, v6.s, v7.s, v8.s}[2], [x24]		\n"
		"	add		x24, x24, x8, lsl #2			 \n"

		"	fmul	v27.4s, v0.4s, v8.4s			 \n"
		"	ldr		q0, [x14], #16					 \n"
		"	fmul	v28.4s, v1.4s, v8.4s			 \n"
		"	ldr		q1, [x15], #16					 \n"

		"	st4		{v5.s, v6.s, v7.s, v8.s}[3], [x24]		\n"
		"	add		x24, x24, x8, lsl #2			 \n"
		"	ldr		q5, [x16], #16					 \n"

		"	fmul	v29.4s, v2.4s, v8.4s			 \n"
		"	fmul	v30.4s, v3.4s, v8.4s			 \n"
		"	fmul	v31.4s, v4.4s, v8.4s			 \n"


		".endm									 	 \n"



		".macro PACK_KERNEL5x4_K0                    \n"

		"	prfm 	PLDL1KEEP, [x16, #128]			 \n"

		"	ldr		q6, [x17], #16					 \n"

		"	fmla	v12.4s, v0.4s, v5.4s			 \n"
		"	fmla	v13.4s, v1.4s, v5.4s			 \n"

		"	prfm 	PLDL1KEEP, [x17, #128]			 \n"

		"	ldr		q7, [x18], #16					 \n"

		"	prfm 	PLDL1KEEP, [x11, #64]			 \n"

		"	fmla	v14.4s, v2.4s, v5.4s			 \n"
		"	fmla	v15.4s, v3.4s, v5.4s			 \n"
		"	fmla	v16.4s, v4.4s, v5.4s			 \n"

		"	prfm 	PLDL1KEEP, [x12, #64]			 \n"

		"	ldr		q8, [x19], #16					 \n"
		"	st4		{v5.s, v6.s, v7.s, v8.s}[0], [x24]		\n"
		"	add		x24, x24, x8, lsl #2			 \n"

		"	fmla	v17.4s, v0.4s, v6.4s			 \n"
		"	fmla	v18.4s, v1.4s, v6.4s		 	 \n"
		"	fmla	v19.4s, v2.4s, v6.4s			 \n"
		"	fmla	v20.4s, v3.4s, v6.4s			 \n"
		"	fmla	v21.4s, v4.4s, v6.4s			 \n"

		"	ldr		q9, [x11], #16					 \n"
		"	st4		{v5.s, v6.s, v7.s, v8.s}[1], [x24]		\n"
		"	add		x24, x24, x8, lsl #2			 \n"

		"	fmla	v22.4s, v0.4s, v7.4s			 \n"
		"	fmla	v23.4s, v1.4s, v7.4s		 	 \n"

		"	ldr		q10, [x12], #16					 \n"
		"	st4		{v5.s, v6.s, v7.s, v8.s}[2], [x24]		\n"
		"	add		x24, x24, x8, lsl #2			 \n"

		"	fmla	v24.4s, v2.4s, v7.4s			 \n"
		"	fmla	v25.4s, v3.4s, v7.4s			 \n"
		"	fmla	v26.4s, v4.4s, v7.4s			 \n"

		"	ldr		q11, [x13], #16					 \n"
		"	st4		{v5.s, v6.s, v7.s, v8.s}[3], [x24]		\n"
		"	add		x24, x24, x8, lsl #2			 \n"

		"	fmla	v27.4s, v0.4s, v8.4s			 \n"
		"	ldr		q5, [x16], #16					 \n"
		"	ldr		q0, [x14], #16					 \n"
		"	fmla	v28.4s, v1.4s, v8.4s		 	 \n"
		"	ldr		q1, [x15], #16					 \n"

		"	fmla	v29.4s, v2.4s, v8.4s			 \n"
		"	fmla	v30.4s, v3.4s, v8.4s			 \n"
		"	fmla	v31.4s, v4.4s, v8.4s			 \n"

		".endm   									 \n"


		".macro PACK_KERNEL5x4_K1                    \n"

		"	prfm 	PLDL1KEEP, [x18, #128]			 \n"

		"	ldr		q6, [x17], #16					 \n"
		"	fmla	v12.4s, v9.4s, v5.4s 			 \n"
		"	fmla	v13.4s, v10.4s, v5.4s		  	 \n"

		"	prfm  	PLDL1KEEP, [x19, #128]			 \n"
		"	ldr		q7, [x18], #16					 \n"
		"	fmla	v14.4s, v11.4s, v5.4s		 	 \n"
		"	fmla	v15.4s, v0.4s, v5.4s			 \n"
		"	fmla	v16.4s, v1.4s, v5.4s			 \n"

		"	prfm 	PLDL1KEEP, [x13, #64]			 \n"
		"	ldr		q8, [x19], #16					 \n"
		"	st4		{v5.s, v6.s, v7.s, v8.s}[0], [x24]		\n"
		"	add		x24, x24, x8, lsl #2			 \n"

		"	fmla	v17.4s, v9.4s, v6.4s 			 \n"
		"	fmla	v18.4s, v10.4s, v6.4s		  	 \n"

		"	ldr		q2, [x13], #16					 \n"
		"	st4		{v5.s, v6.s, v7.s, v8.s}[1], [x24]		\n"
		"	add		x24, x24, x8, lsl #2			 \n"

		"	prfm  	PLDL1KEEP, [x14, #64]			 \n"
		"	fmla	v19.4s, v11.4s, v6.4s		 	 \n"
		"	fmla	v20.4s, v0.4s, v6.4s			 \n"
		"	fmla	v21.4s, v1.4s, v6.4s			 \n"

		"	prfm  	PLDL1KEEP, [x15, #64]			 \n"
		"	ldr		q3, [x14], #16					 \n"

		"	fmla	v25.4s, v0.4s, v7.4s			 \n"
		"	fmla	v26.4s, v1.4s, v7.4s			 \n"

		"	st4		{v5.s, v6.s, v7.s, v8.s}[2], [x24]		\n"
		"	add		x24, x24, x8, lsl #2			 \n"

		"	fmla	v30.4s, v0.4s, v8.4s			 \n"
		"	ldr		q0, [x11], #16					 \n"
		"	fmla	v31.4s, v1.4s, v8.4s			 \n"
		"	ldr		q1, [x12], #16					 \n"

		"	fmla	v22.4s, v9.4s, v7.4s		  	 \n"

		"	st4		{v5.s, v6.s, v7.s, v8.s}[3], [x24]		\n"
		"	add		x24, x24, x8, lsl #2			 \n"
		"	ldr		q5, [x16], #16					 \n"

		"	fmla	v23.4s, v10.4s, v7.4s			 \n"
		"	fmla	v24.4s, v11.4s, v7.4s			 \n"

		"	ldr		q4, [x15], #16					 \n"

		"	fmla	v27.4s, v9.4s, v8.4s			 \n"
		"	fmla	v28.4s, v10.4s, v8.4s			 \n"
		"	fmla	v29.4s, v11.4s, v8.4s			 \n"

		".endm									 	 \n"


		".macro PACK_KERNEL5x4_END_K                 \n"

		"	prfm	PLDL1KEEP, [x25, #64]			 \n"
		"	prfm	PLDL1KEEP, [x26, #64]			 \n"
		"	prfm	PLDL1KEEP, [x27, #64]			 \n"

		"	ldr		q6, [x17], #16					 \n"
		"	fmla	v12.4s, v9.4s, v5.4s 			 \n"
		"	fmla	v13.4s, v10.4s, v5.4s		  	 \n"

		"	ldr		q7, [x18], #16					 \n"
		"	fmla	v14.4s, v11.4s, v5.4s		 	 \n"
		"	fmla	v15.4s, v0.4s, v5.4s			 \n"
		"	fmla	v16.4s, v1.4s, v5.4s			 \n"

		"	prfm	PLDL1KEEP, [x28, #64]			 \n"
		"	prfm	PLDL1KEEP, [x29, #64]			 \n"

		"	ldr		q8, [x19], #16					 \n"
		"	st4		{v5.s, v6.s, v7.s, v8.s}[0], [x24]		\n"
		"	add		x24, x24, x8, lsl #2			 \n"

		"	fmla	v17.4s, v9.4s, v6.4s 			 \n"
		"	faddp	v12.4s, v12.4s, v17.4s			 \n"
		"	fmla	v18.4s, v10.4s, v6.4s		  	 \n"
		"	faddp	v13.4s, v13.4s, v18.4s			 \n"

		"	st4		{v5.s, v6.s, v7.s, v8.s}[1], [x24]		\n"
		"	add		x24, x24, x8, lsl #2			 \n"

		"	fmla	v19.4s, v11.4s, v6.4s		 	 \n"
		"	faddp	v14.4s, v14.4s, v19.4s			 \n"
		"	fmla	v20.4s, v0.4s, v6.4s			 \n"
		"	faddp	v15.4s, v15.4s, v20.4s			 \n"
		"	fmla	v21.4s, v1.4s, v6.4s			 \n"
		"	faddp	v16.4s, v16.4s, v21.4s			 \n"

		"	fmla	v25.4s, v0.4s, v7.4s			 \n"
		"	fmla	v26.4s, v1.4s, v7.4s			 \n"

		"	st4		{v5.s, v6.s, v7.s, v8.s}[2], [x24]		\n"
		"	add		x24, x24, x8, lsl #2			 \n"

		"	fmla	v30.4s, v0.4s, v8.4s			 \n"
		"	faddp	v25.4s, v25.4s, v30.4s			 \n"
		"	fmla	v31.4s, v1.4s, v8.4s			 \n"
		"	faddp	v26.4s, v26.4s, v31.4s			 \n"

		"	fmla	v22.4s, v9.4s, v7.4s		  	 \n"
		"	st4		{v5.s, v6.s, v7.s, v8.s}[3], [x24]		\n"
		"	add		x24, x24, x8, lsl #2			 \n"
		"	fmla	v23.4s, v10.4s, v7.4s			 \n"
		"	fmla	v24.4s, v11.4s, v7.4s			 \n"

		"	faddp	v15.4s, v15.4s, v25.4s			 \n"
		"	faddp 	v16.4s, v16.4s, v26.4s			 \n"

		"	fmla	v27.4s, v9.4s, v8.4s			 \n"
		"	faddp 	v22.4s, v22.4s, v27.4s			 \n"
		"	fmla	v28.4s, v10.4s, v8.4s			 \n"
		"	faddp	v23.4s, v23.4s, v28.4s			 \n"
		"	fmla	v29.4s, v11.4s, v8.4s			 \n"
		"	faddp	v24.4s, v24.4s, v29.4s			 \n"

		"	faddp	v12.4s, v12.4s, v22.4s			 \n"
		"	faddp	v13.4s, v13.4s, v23.4s		 	 \n"
		"	faddp	v14.4s, v14.4s, v24.4s			 \n"

		".endm 										 \n"


		".macro	PACKB_ADD_C 						 \n"

		"	ldr		q0, [x25]						 \n"
		"	ldr		q1, [x26]						 \n"

		"	fadd 	v12.4s, v12.4s, v0.4s			 \n"
		"	ldr		q2, [x27]						 \n"
		"	fadd 	v13.4s, v13.4s, v1.4s			 \n"
		"	ldr		q3, [x28]						 \n"
		"	fadd 	v14.4s, v14.4s, v2.4s			 \n"
		"	ldr		q4, [x29]						 \n"
		"	fadd 	v15.4s, v15.4s, v3.4s			 \n"
		"	fadd 	v16.4s, v16.4s, v4.4s			 \n"


		".endm 									 	 \n"


		".macro SAVE5x4							 	 \n"

		"	subs	x7, x7, #1					 	 \n"

		"	str		q12, [x25], #16					 \n"
		"	prfm	PLDL2KEEP, [x25, #64]			 \n"
		"	str		q13, [x26], #16					 \n"
		"	prfm	PLDL2KEEP, [x26, #64]			 \n"
		"	str		q14, [x27], #16		 			 \n"
		"	prfm	PLDL2KEEP, [x27, #64]			 \n"
		"	str		q15, [x28], #16		 			 \n"
		"	prfm	PLDL2KEEP, [x28, #64]			 \n"
		"	str		q16, [x29], #16					 \n"
		"	prfm	PLDL2KEEP, [x29, #64]			 \n"

		".endm 									 	 \n"


		".macro ADD_C								 \n"

		"	prfm	PLDL1KEEP, [x25, #64]			 \n"
		"	prfm	PLDL1KEEP, [x26, #64]			 \n"
		"	prfm	PLDL1KEEP, [x27, #64]			 \n"
		"	prfm	PLDL1KEEP, [x28, #64]			 \n"
		"	prfm	PLDL1KEEP, [x29, #64]			 \n"

		"	ldp		q0, q1, [x25]					 \n"

		"	fadd	v12.4s, v12.4s, v0.4s			 \n"
		"	ldp		q2, q3, [x25, #32]				 \n"
		"	fadd	v13.4s, v13.4s, v1.4s			 \n"

		"	fadd	v22.4s, v2.4s, v22.4s			 \n"
		"	ldp		q4, q5, [x26]					 \n"
		"	fadd	v23.4s, v3.4s, v23.4s		 	 \n"

		"	fadd	v14.4s, v4.4s, v14.4s			 \n"
		"	ldp		q6, q7, [x26, #32]				 \n"
		"	fadd	v15.4s, v5.4s, v15.4s			 \n"

		"	prfm	PLDL1KEEP, [x27, #64]			 \n"
		"	prfm	PLDL1KEEP, [x28, #64]			 \n"
		"	prfm	PLDL1KEEP, [x29, #64]			 \n"

		"	fadd	v24.4s, v6.4s, v24.4s			 \n"
		"	ldp		q8, q9, [x27]					 \n"
		"	fadd 	v25.4s, v7.4s, v25.4s			 \n"

		"	fadd	v16.4s, v8.4s, v16.4s			 \n"
		"	ldp		q10, q11, [x27, #32]			 \n"
		"	fadd	v17.4s, v9.4s, v17.4s			 \n"

		"	fadd	v26.4s, v10.4s, v26.4s			 \n"
		"	ldp		q0, q1, [x28]				  	 \n"
		"	fadd	v27.4s, v11.4s, v27.4s			 \n"


		"	fadd	v18.4s, v0.4s, v18.4s			 \n"
		"	ldp		q2, q3, [x28, #32]				 \n"
		"	fadd	v19.4s, v1.4s, v19.4s			 \n"

		"	fadd	v28.4s, v2.4s, v28.4s			 \n"
		"	ldp		q4, q5, [x29]					 \n"
		"	fadd	v29.4s, v3.4s, v29.4s			 \n"

		"	fadd	v20.4s, v4.4s, v20.4s			 \n"
		"	ldp		q6, q7, [x29, #32]				 \n"
		"	fadd	v21.4s, v5.4s, v21.4s			 \n"

		"	fadd	v30.4s, v30.4s, v6.4s			 \n"
		"	fadd	v31.4s, v31.4s, v7.4s		     \n"

		".endm 										 \n"


		".macro SAVE5x16							 \n"


		"	subs	x21, x21, #1					 \n"

		"	stp		q12, q13, [x25]			 		 \n"
		"	stp		q14, q15, [x26]			 	     \n"
		"	stp		q16, q17, [x27]					 \n"
		"	stp		q18, q19, [x28]					 \n"
		"	stp		q20, q21, [x29]					 \n"

		"	add 	x10, x10, x6, lsl #4			 \n"
		"	add		x10, x10, x6, lsl #2 			 \n"

		"	stp		q22, q23, [x25, #32]			 \n"
		"	add		x25, x29, x9, lsl #2			 \n"
		"	prfm	PLDL2KEEP, [x25, #64]			 \n"
		"	stp		q24, q25, [x26, #32]			 \n"
		"	add		x26, x29, x9, lsl #3			 \n"
		"	prfm	PLDL2KEEP, [x26, #64]			 \n"
		"	stp		q26, q27, [x27, #32]			 \n"
		"	add		x27, x25, x9, lsl #3 			 \n"
		"	prfm	PLDL2KEEP, [x27, #64]			 \n"
		"	stp		q28, q29, [x28, #32]			 \n"
		"	add		x28, x26, x9, lsl #3 			 \n"
		"	prfm	PLDL2KEEP, [x28, #64]			 \n"
		"	stp		q30, q31, [x29, #32]			 \n"
		"	add		x29, x27, x9, lsl #3			 \n"
		"	prfm	PLDL2KEEP, [x29, #64]			 \n"

		".endm 									 	 \n"


		".macro KERNEL5x16_BEGIN_K             	 	 \n"
		"										 	 \n"

		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q1, [x12], #16					 \n"
		"	ldp		q8, q9, [x24], #32			     \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x14, #64] 			 \n"
		"	prfm	PLDL1KEEP, [x15, #64]			 \n"

		"	fmul	v12.4s, v8.4s, v0.s[0]	         \n"
		"	fmul	v13.4s, v9.4s, v0.s[0]	         \n"

		"	ldr		q2, [x13], #16					 \n"

		"	fmul	v14.4s, v8.4s, v1.s[0]	         \n"
		"	fmul	v15.4s, v9.4s, v1.s[0]	         \n"

		"	ldr		q3, [x14], #16					 \n"

		"	fmul	v16.4s, v8.4s, v2.s[0]	         \n"
		"	fmul	v17.4s, v9.4s, v2.s[0]	         \n"

		"	prfm	PLDL1KEEP, [x24, #128]			 \n"

		"	ldr		q4, [x15], #16				 	 \n"

		"	fmul	v18.4s, v8.4s, v3.s[0]	         \n"
		"	fmul	v19.4s, v9.4s, v3.s[0]	         \n"

		"	ldp		q10, q11, [x24], #32			 \n"

		"	fmul	v20.4s, v8.4s, v4.s[0]			 \n"
		"	ldr		q8, [x24], #16					 \n"
		"	fmul	v21.4s, v9.4s, v4.s[0]			 \n"
		"	ldr		q9, [x24], #16					 \n"

		"	fmul	v22.4s, v10.4s, v0.s[0]			 \n"
		"	fmul	v23.4s, v11.4s, v0.s[0]			 \n"

		"	fmul	v24.4s, v10.4s, v1.s[0]	         \n"
		"	fmul	v25.4s, v11.4s, v1.s[0]	         \n"

		"	fmul	v26.4s, v10.4s, v2.s[0]	         \n"
		"	fmul	v27.4s, v11.4s, v2.s[0]	         \n"

		"	ldr		q5, [x11], #16					 \n"

		"	fmul	v28.4s, v10.4s, v3.s[0]	         \n"
		"	fmul	v29.4s, v11.4s, v3.s[0]	         \n"

		"	fmul	v30.4s, v10.4s, v4.s[0]	         \n"
		"	ldr		q10, [x24], #16					 \n"
		"	fmul	v31.4s, v11.4s, v4.s[0]	         \n"
		"	ldr		q11, [x24], #16					 \n"

		".endm									 	 \n"


		".macro KERNEL5x16_K0                   	 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"

		"	fmla	v12.4s, v8.4s, v0.s[0]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[0]			 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"

		"	fmla	v14.4s, v8.4s, v1.s[0]			 \n"
		"	fmla 	v15.4s, v9.4s, v1.s[0]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[0]			 \n"
		"	fmla 	v17.4s, v9.4s, v2.s[0]			 \n"

		"	fmla	v18.4s, v8.4s, v3.s[0]			 \n"
		"	fmla 	v19.4s, v9.4s, v3.s[0]			 \n"

		"	fmla	v20.4s, v8.4s, v4.s[0]			 \n"
		"	ldr		q8, [x24], #16					 \n"
		"	fmla	v21.4s, v9.4s, v4.s[0]			 \n"
		"	ldr		q9, [x24], #16					 \n"

		"	fmla	v22.4s, v10.4s, v0.s[0]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[0]			 \n"

		"	fmla	v24.4s, v10.4s, v1.s[0]			 \n"
		"	fmla	v25.4s, v11.4s, v1.s[0]			 \n"

		"	fmla	v26.4s, v10.4s, v2.s[0]			 \n"
		"	fmla	v27.4s, v11.4s, v2.s[0]			 \n"

		"	ldr		q5, [x11], #16					 \n"  //预取

		"	fmla	v28.4s, v10.4s, v3.s[0]			 \n"
		"	fmla	v29.4s, v11.4s, v3.s[0]			 \n"

		"	fmla	v30.4s, v10.4s, v4.s[0]			 \n"
		"	ldr		q10, [x24], #16					 \n"
		"	fmla	v31.4s, v11.4s, v4.s[0]			 \n"
		"	ldr		q11, [x24], #16					 \n"

		".endm   									 \n"


		".macro KERNEL5x16_K1        	             \n"

		"   prfm    PLDL1KEEP, [x12, #64]       	 \n"
		"	fmla	v12.4s, v8.4s, v0.s[1]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[1]			 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"

		"	fmla	v14.4s, v8.4s, v1.s[1]			 \n"
		"	fmla	v15.4s, v9.4s, v1.s[1]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[1]			 \n"
		"	fmla	v17.4s, v9.4s, v2.s[1]			 \n"

		"	fmla	v18.4s, v8.4s, v3.s[1]			 \n"
		"	fmla	v19.4s, v9.4s, v3.s[1]			 \n"

		"	fmla	v20.4s, v8.4s, v4.s[1]			 \n"
		"	ldr		q8, [x24], #16					 \n"
		"	fmla	v21.4s, v9.4s, v4.s[1]			 \n"
		"	ldr		q9, [x24], #16					 \n"

		"	fmla	v22.4s, v10.4s, v0.s[1]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[1]			 \n"

		"	fmla	v24.4s, v10.4s, v1.s[1]			 \n"
		"	fmla	v25.4s, v11.4s, v1.s[1]			 \n"

		"	fmla	v26.4s, v10.4s, v2.s[1]			 \n"
		"	fmla	v27.4s, v11.4s, v2.s[1]			 \n"

		"	ldr		q6, [x12], #16					 \n"

		"	fmla	v28.4s, v10.4s, v3.s[1]			 \n"
		"	fmla	v29.4s, v11.4s, v3.s[1]			 \n"

		"	fmla	v30.4s, v10.4s, v4.s[1]			 \n"
		"	ldr		q10, [x24], #16					 \n"
		"	fmla	v31.4s, v11.4s, v4.s[1]			 \n"
		"	ldr		q11, [x24], #16					 \n"

		".endm									 	 \n"


		".macro KERNEL5x16_K2          		         \n"

		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"

		"	fmla	v12.4s, v8.4s, v0.s[2]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[2]			 \n"

		"	fmla	v14.4s, v8.4s, v1.s[2]			 \n"
		"	fmla 	v15.4s, v9.4s, v1.s[2]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[2]			 \n"
		"	fmla 	v17.4s, v9.4s, v2.s[2]			 \n"

		"	fmla	v18.4s, v8.4s, v3.s[2]			 \n"
		"	fmla 	v19.4s, v9.4s, v3.s[2]			 \n"

		"	fmla	v20.4s, v8.4s, v4.s[2]			 \n"
		"	ldr		q8, [x24], #16					 \n"
		"	fmla	v21.4s, v9.4s, v4.s[2]			 \n"
		"	ldr		q9, [x24], #16					 \n"

		"	fmla	v22.4s, v10.4s, v0.s[2]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[2]			 \n"

		"	prfm	PLDL1KEEP, [x14, #64]			 \n"

		"	fmla	v24.4s, v10.4s, v1.s[2]			 \n"
		"	fmla	v25.4s, v11.4s, v1.s[2]			 \n"

		"	fmla	v26.4s, v10.4s, v2.s[2]			 \n"
		"	fmla	v27.4s, v11.4s, v2.s[2]			 \n"

		"	ldr		q7, [x13], #16					 \n"  //预取

		"	fmla	v28.4s, v10.4s, v3.s[2]			 \n"
		"	fmla	v29.4s, v11.4s, v3.s[2]			 \n"

		"	fmla	v30.4s, v10.4s, v4.s[2]			 \n"
		"	ldr		q10, [x24], #16					 \n"
		"	fmla	v31.4s, v11.4s, v4.s[2]			 \n"
		"	ldr		q11, [x24], #16					 \n"

		".endm									 	 \n"


		".macro KERNEL5x16_K3   	                 \n"

		"   prfm    PLDL1KEEP, [x15, #64]       	 \n"
		"	fmla	v12.4s, v8.4s, v0.s[3]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[3]			 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"

		"	fmla	v14.4s, v8.4s, v1.s[3]			 \n"
		"	fmla	v15.4s, v9.4s, v1.s[3]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[3]			 \n"
		"	fmla	v17.4s, v9.4s, v2.s[3]			 \n"

		"	fmla	v18.4s, v8.4s, v3.s[3]			 \n"
		"	fmla	v19.4s, v9.4s, v3.s[3]			 \n"

		"	fmla	v20.4s, v8.4s, v4.s[3]			 \n"
		"	ldr		q8, [x24], #16					 \n"
		"	fmla	v21.4s, v9.4s, v4.s[3]			 \n"
		"	ldr		q9, [x24], #16					 \n"

		"	fmla	v22.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[3]			 \n"

		"	ldr		q0, [x14], #16					 \n"

		"	fmla	v24.4s, v10.4s, v1.s[3]			 \n"
		"	fmla	v25.4s, v11.4s, v1.s[3]			 \n"

		"	ldr		q1, [x15], #16					 \n"

		"	fmla	v26.4s, v10.4s, v2.s[3]			 \n"
		"	fmla	v27.4s, v11.4s, v2.s[3]			 \n"

		"	fmla	v28.4s, v10.4s, v3.s[3]			 \n"
		"	fmla	v29.4s, v11.4s, v3.s[3]			 \n"

		"	fmla	v30.4s, v10.4s, v4.s[3]			 \n"
		"	ldr		q10, [x24], #16				 	 \n"
		"	fmla	v31.4s, v11.4s, v4.s[3]			 \n"
		"	ldr		q11, [x24], #16					 \n"

		".endm									 	 \n"


		".macro KERNEL5x16_K4    	                 \n"

		"	prfm	PLDL1KEEP, [x13, #64]			 \n"

		"	fmla	v12.4s, v8.4s, v5.s[0]			 \n"
		"	fmla	v13.4s, v9.4s, v5.s[0]			 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"

		"	fmla	v14.4s, v8.4s, v6.s[0]			 \n"
		"	fmla 	v15.4s, v9.4s, v6.s[0]			 \n"

		"	fmla	v16.4s, v8.4s, v7.s[0]			 \n"
		"	fmla 	v17.4s, v9.4s, v7.s[0]			 \n"

		"	fmla	v18.4s, v8.4s, v0.s[0]			 \n"
		"	fmla 	v19.4s, v9.4s, v0.s[0]			 \n"

		"	fmla	v20.4s, v8.4s, v1.s[0]			 \n"
		"	ldr		q8, [x24], #16					 \n"
		"	fmla	v21.4s, v9.4s, v1.s[0]			 \n"
		"	ldr		q9, [x24], #16					 \n"

		"	fmla	v22.4s, v10.4s, v5.s[0]			 \n"
		"	fmla	v23.4s, v11.4s, v5.s[0]			 \n"

		"	fmla	v24.4s, v10.4s, v6.s[0]			 \n"
		"	fmla	v25.4s, v11.4s, v6.s[0]			 \n"

		"	fmla	v26.4s, v10.4s, v7.s[0]			 \n"
		"	fmla	v27.4s, v11.4s, v7.s[0]			 \n"

		"	ldr		q2, [x13], #16					 \n"  //预取

		"	fmla	v28.4s, v10.4s, v0.s[0]			 \n"
		"	fmla	v29.4s, v11.4s, v0.s[0]			 \n"

		"	fmla	v30.4s, v10.4s, v1.s[0]			 \n"
		"	ldr		q10, [x24], #16					 \n"
		"	fmla	v31.4s, v11.4s, v1.s[0]			 \n"
		"	ldr		q11, [x24], #16					 \n"


		".endm 										 \n"


		".macro KERNEL5x16_K5         	             \n"

		"   prfm    PLDL1KEEP, [x14, #64]       	 \n"
		"	fmla	v12.4s, v8.4s, v5.s[1]			 \n"
		"	fmla	v13.4s, v9.4s, v5.s[1]			 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"

		"	fmla	v14.4s, v8.4s, v6.s[1]			 \n"
		"	fmla	v15.4s, v9.4s, v6.s[1]			 \n"

		"	fmla	v16.4s, v8.4s, v7.s[1]			 \n"
		"	fmla	v17.4s, v9.4s, v7.s[1]			 \n"

		"	fmla	v18.4s, v8.4s, v0.s[1]			 \n"
		"	fmla	v19.4s, v9.4s, v0.s[1]			 \n"

		"	fmla	v20.4s, v8.4s, v1.s[1]			 \n"
		"	ldr		q8, [x24], #16					 \n"
		"	fmla	v21.4s, v9.4s, v1.s[1]			 \n"
		"	ldr		q9, [x24], #16					 \n"

		"	fmla	v22.4s, v10.4s, v5.s[1]			 \n"
		"	fmla	v23.4s, v11.4s, v5.s[1]			 \n"

		"	fmla	v24.4s, v10.4s, v6.s[1]			 \n"
		"	fmla	v25.4s, v11.4s, v6.s[1]			 \n"

		"	fmla	v26.4s, v10.4s, v7.s[1]			 \n"
		"	fmla	v27.4s, v11.4s, v7.s[1]			 \n"

		"	ldr		q3, [x14], #16					 \n"

		"	fmla	v28.4s, v10.4s, v0.s[1]			 \n"
		"	fmla	v29.4s, v11.4s, v0.s[1]			 \n"

		"	fmla	v30.4s, v10.4s, v1.s[1]			 \n"
		"	ldr		q10, [x24], #16					 \n"
		"	fmla	v31.4s, v11.4s, v1.s[1]			 \n"
		"	ldr		q11, [x24], #16					 \n"

		".endm 										 \n"


		".macro KERNEL5x16_K6 	                     \n"

		"	prfm	PLDL1KEEP, [x15, #64]			 \n"

		"	fmla	v12.4s, v8.4s, v5.s[2]			 \n"
		"	fmla	v13.4s, v9.4s, v5.s[2]			 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"

		"	fmla	v14.4s, v8.4s, v6.s[2]			 \n"
		"	fmla 	v15.4s, v9.4s, v6.s[2]			 \n"

		"	fmla	v16.4s, v8.4s, v7.s[2]			 \n"
		"	fmla 	v17.4s, v9.4s, v7.s[2]			 \n"

		"	fmla	v18.4s, v8.4s, v0.s[2]			 \n"
		"	fmla 	v19.4s, v9.4s, v0.s[2]			 \n"

		"	fmla	v20.4s, v8.4s, v1.s[2]			 \n"
		"	ldr		q8, [x24], #16					 \n"
		"	fmla	v21.4s, v9.4s, v1.s[2]			 \n"
		"	ldr		q9, [x24], #16					 \n"

		"	fmla	v22.4s, v10.4s, v5.s[2]			 \n"
		"	fmla	v23.4s, v11.4s, v5.s[2]			 \n"

		"	fmla	v24.4s, v10.4s, v6.s[2]			 \n"
		"	fmla	v25.4s, v11.4s, v6.s[2]			 \n"

		"	prfm	PLDL1KEEP, [x11, #64]			 \n"

		"	fmla	v26.4s, v10.4s, v7.s[2]			 \n"
		"	fmla	v27.4s, v11.4s, v7.s[2]			 \n"

		"	ldr		q4, [x15], #16					 \n"  //预取

		"	fmla	v28.4s, v10.4s, v0.s[2]			 \n"
		"	fmla	v29.4s, v11.4s, v0.s[2]			 \n"

		"	fmla	v30.4s, v10.4s, v1.s[2]			 \n"
		"	ldr		q10, [x24], #16					 \n"
		"	fmla	v31.4s, v11.4s, v1.s[2]			 \n"
		"	ldr		q11, [x24], #16					 \n"

		".endm 										 \n"


		".macro KERNEL5x16_K7      		             \n"

		"   prfm    PLDL1KEEP, [x12, #64]       	 \n"
		"	fmla	v12.4s, v8.4s, v5.s[3]			 \n"
		"	fmla	v13.4s, v9.4s, v5.s[3]			 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"

		"	fmla	v14.4s, v8.4s, v6.s[3]			 \n"
		"	fmla	v15.4s, v9.4s, v6.s[3]			 \n"

		"	fmla	v16.4s, v8.4s, v7.s[3]			 \n"
		"	fmla	v17.4s, v9.4s, v7.s[3]			 \n"

		"	fmla	v18.4s, v8.4s, v0.s[3]			 \n"
		"	fmla	v19.4s, v9.4s, v0.s[3]			 \n"

		"	fmla	v20.4s, v8.4s, v1.s[3]			 \n"
		"	ldr		q8, [x24], #16			     	 \n"
		"	fmla	v21.4s, v9.4s, v1.s[3]			 \n"
		"	ldr		q9, [x24], #16					 \n"

		"	fmla	v28.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v29.4s, v11.4s, v0.s[3]			 \n"

		"	ldr		q0, [x11], #16					 \n"

		"	fmla	v30.4s, v10.4s, v1.s[3]			 \n"
		"	fmla	v31.4s, v11.4s, v1.s[3]			 \n"

		"	ldr		q1, [x12], #16					 \n"

		"	fmla	v22.4s, v10.4s, v5.s[3]			 \n"
		"	fmla	v23.4s, v11.4s, v5.s[3]			 \n"

		"	fmla	v24.4s, v10.4s, v6.s[3]			 \n"
		"	fmla	v25.4s, v11.4s, v6.s[3]			 \n"

		"	fmla	v26.4s, v10.4s, v7.s[3]			 \n"
		"	ldr		q10, [x24], #16					 \n"
		"	fmla	v27.4s, v11.4s, v7.s[3]			 \n"
		"	ldr		q11, [x24], #16					 \n"

		".endm 										 \n"


		".macro KERNEL5x16_END_K                	 \n"

		"	prfm	PLDL1KEEP, [x25, #64]			 \n"
		"	prfm	PLDL1KEEP, [x26, #64]			 \n"
		"	prfm	PLDL1KEEP, [x27, #64]			 \n"

		"	fmla	v12.4s, v8.4s, v5.s[3]			 \n"
		"	fmla	v13.4s, v9.4s, v5.s[3]			 \n"

		"	prfm	PLDL1KEEP, [x28, #64]			 \n"
		"	prfm	PLDL1KEEP, [x29, #64]			 \n"

		"	fmla	v14.4s, v8.4s, v6.s[3]			 \n"
		"	fmla	v15.4s, v9.4s, v6.s[3]			 \n"

		"	fmla	v16.4s, v8.4s, v7.s[3]			 \n"
		"	fmla	v17.4s, v9.4s, v7.s[3]			 \n"

		"	fmla	v18.4s, v8.4s, v0.s[3]			 \n"
		"	fmla	v19.4s, v9.4s, v0.s[3]			 \n"

		"	fmla	v20.4s, v8.4s, v1.s[3]			 \n"
		"	fmla	v21.4s, v9.4s, v1.s[3]			 \n"

		"	fmla	v28.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v29.4s, v11.4s, v0.s[3]			 \n"

		"	fmla	v30.4s, v10.4s, v1.s[3]			 \n"
		"	fmla	v31.4s, v11.4s, v1.s[3]			 \n"

		"	fmla	v22.4s, v10.4s, v5.s[3]			 \n"
		"	fmla	v23.4s, v11.4s, v5.s[3]			 \n"

		"	fmla	v24.4s, v10.4s, v6.s[3]			 \n"
		"	fmla	v25.4s, v11.4s, v6.s[3]			 \n"

		"	fmla	v26.4s, v10.4s, v7.s[3]			 \n"
		"	fmla	v27.4s, v11.4s, v7.s[3]			 \n"

		".endm 										 \n"



		".macro	KERNEL4x16_BEGIN_K					 \n"

		"										 	 \n"
		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q1, [x12], #16					 \n"
		"	ldp		q8, q9, [x24], #32			     \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x14, #64] 			 \n"

		"	fmul	v12.4s, v8.4s, v0.s[0]	         \n"
		"	fmul	v13.4s, v9.4s, v0.s[0]	         \n"

		"	ldr		q2, [x13], #16					 \n"

		"	fmul	v14.4s, v8.4s, v1.s[0]	         \n"
		"	fmul	v15.4s, v9.4s, v1.s[0]	         \n"

		"	ldr		q3, [x14], #16					 \n"

		"	fmul	v16.4s, v8.4s, v2.s[0]	         \n"
		"	fmul	v17.4s, v9.4s, v2.s[0]	         \n"

		"	prfm	PLDL1KEEP, [x24, #128]			 \n"
		"	ldp		q10, q11, [x24], #32			 \n"

		"	fmul	v18.4s, v8.4s, v3.s[0]	         \n"
		"	ldr		q8, [x24], #16					 \n"
		"	fmul	v19.4s, v9.4s, v3.s[0]	         \n"
		"	ldr		q9, [x24], #16					 \n"

		"	fmul	v22.4s, v10.4s, v0.s[0]			 \n"
		"	fmul	v23.4s, v11.4s, v0.s[0]			 \n"

		"	fmul	v24.4s, v10.4s, v1.s[0]	         \n"
		"	fmul	v25.4s, v11.4s, v1.s[0]	         \n"

		"	fmul	v26.4s, v10.4s, v2.s[0]	         \n"
		"	fmul	v27.4s, v11.4s, v2.s[0]	         \n"

		"	fmul	v28.4s, v10.4s, v3.s[0]	         \n"
		"	ldr		q10, [x24], #16					 \n"
		"	fmul	v29.4s, v11.4s, v3.s[0]	         \n"
		"	ldr		q11, [x24], #16					 \n"

		".endm 										 \n"


		".macro KERNEL4x16_K0                   	 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"

		"	fmla	v12.4s, v8.4s, v0.s[0]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[0]			 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"

		"	fmla	v14.4s, v8.4s, v1.s[0]			 \n"
		"	fmla 	v15.4s, v9.4s, v1.s[0]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[0]			 \n"
		"	fmla 	v17.4s, v9.4s, v2.s[0]			 \n"

		"	fmla	v18.4s, v8.4s, v3.s[0]			 \n"
		"	ldr		q8, [x24], #16					 \n"
		"	fmla 	v19.4s, v9.4s, v3.s[0]			 \n"
		"	ldr		q9, [x24], #16					 \n"

		"	fmla	v22.4s, v10.4s, v0.s[0]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[0]			 \n"

		"	fmla	v24.4s, v10.4s, v1.s[0]			 \n"
		"	fmla	v25.4s, v11.4s, v1.s[0]			 \n"

		"	fmla	v26.4s, v10.4s, v2.s[0]			 \n"
		"	fmla	v27.4s, v11.4s, v2.s[0]			 \n"

		"	fmla	v28.4s, v10.4s, v3.s[0]			 \n"
		"	ldr		q10, [x24], #16					 \n"
		"	fmla	v29.4s, v11.4s, v3.s[0]			 \n"
		"	ldr		q11, [x24], #16					 \n"

		".endm   									 \n"


		".macro KERNEL4x16_K1        	             \n"

		"   prfm    PLDL1KEEP, [x12, #64]       	 \n"
		"	fmla	v12.4s, v8.4s, v0.s[1]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[1]			 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"

		"	fmla	v14.4s, v8.4s, v1.s[1]			 \n"
		"	fmla	v15.4s, v9.4s, v1.s[1]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[1]			 \n"
		"	fmla	v17.4s, v9.4s, v2.s[1]			 \n"

		"	fmla	v18.4s, v8.4s, v3.s[1]			 \n"
		"	ldr		q8, [x24], #16					 \n"
		"	fmla	v19.4s, v9.4s, v3.s[1]			 \n"
		"	ldr		q9, [x24], #16					 \n"

		"	fmla	v22.4s, v10.4s, v0.s[1]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[1]			 \n"

		"	fmla	v24.4s, v10.4s, v1.s[1]			 \n"
		"	fmla	v25.4s, v11.4s, v1.s[1]			 \n"

		"	fmla	v26.4s, v10.4s, v2.s[1]			 \n"
		"	fmla	v27.4s, v11.4s, v2.s[1]			 \n"

		"	fmla	v28.4s, v10.4s, v3.s[1]			 \n"
		"	ldr		q10, [x24], #16					 \n"
		"	fmla	v29.4s, v11.4s, v3.s[1]			 \n"
		"	ldr		q11, [x24], #16					 \n"


		".endm									 	 \n"


		".macro KERNEL4x16_K2          		         \n"

		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"

		"	fmla	v12.4s, v8.4s, v0.s[2]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[2]			 \n"

		"	fmla	v14.4s, v8.4s, v1.s[2]			 \n"
		"	fmla 	v15.4s, v9.4s, v1.s[2]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[2]			 \n"
		"	fmla 	v17.4s, v9.4s, v2.s[2]			 \n"

		"	fmla	v18.4s, v8.4s, v3.s[2]			 \n"
		"	ldr		q8, [x24], #16					 \n"
		"	fmla 	v19.4s, v9.4s, v3.s[2]			 \n"
		"	ldr		q9, [x24], #16					 \n"

		"	fmla	v22.4s, v10.4s, v0.s[2]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[2]			 \n"

		"	prfm	PLDL1KEEP, [x14, #64]			 \n"

		"	fmla	v24.4s, v10.4s, v1.s[2]			 \n"
		"	fmla	v25.4s, v11.4s, v1.s[2]			 \n"

		"	fmla	v26.4s, v10.4s, v2.s[2]			 \n"
		"	fmla	v27.4s, v11.4s, v2.s[2]			 \n"

		"	fmla	v28.4s, v10.4s, v3.s[2]			 \n"
		"	ldr		q10, [x24], #16					 \n"
		"	fmla	v29.4s, v11.4s, v3.s[2]			 \n"
		"	ldr		q11, [x24], #16					 \n"

		".endm									 	 \n"


		".macro KERNEL4x16_K3   	                 \n"

		"   prfm    PLDL1KEEP, [x14, #64]       	 \n"
		"	fmla	v12.4s, v8.4s, v0.s[3]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[3]			 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"

		"	fmla	v14.4s, v8.4s, v1.s[3]			 \n"
		"	fmla	v15.4s, v9.4s, v1.s[3]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[3]			 \n"
		"	fmla	v17.4s, v9.4s, v2.s[3]			 \n"

		"	fmla	v18.4s, v8.4s, v3.s[3]			 \n"
		"	ldr		q8, [x24], #16					 \n"
		"	fmla	v19.4s, v9.4s, v3.s[3]			 \n"
		"	ldr		q9, [x24], #16					 \n"


		"	fmla	v22.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[3]			 \n"

		"	ldr		q0, [x11], #16					 \n"

		"	fmla	v24.4s, v10.4s, v1.s[3]			 \n"
		"	fmla	v25.4s, v11.4s, v1.s[3]			 \n"

		"	ldr		q1, [x12], #16					 \n"

		"	fmla	v26.4s, v10.4s, v2.s[3]			 \n"
		"	fmla	v27.4s, v11.4s, v2.s[3]			 \n"

		"	ldr		q2, [x13], #16					 \n"

		"	fmla	v28.4s, v10.4s, v3.s[3]			 \n"
		"	ldr		q10, [x24], #16				 	 \n"
		"	fmla	v29.4s, v11.4s, v3.s[3]			 \n"
		"	ldr		q11, [x24], #16					 \n"

		"	ldr		q3, [x14], #16					 \n"

		".endm									 	 \n"


		".macro KERNEL4x16_END_K   	                 \n"

		"	fmla	v12.4s, v8.4s, v0.s[3]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[3]			 \n"

		"	fmla	v14.4s, v8.4s, v1.s[3]			 \n"
		"	fmla	v15.4s, v9.4s, v1.s[3]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[3]			 \n"
		"	fmla	v17.4s, v9.4s, v2.s[3]			 \n"

		"	fmla	v18.4s, v8.4s, v3.s[3]			 \n"
		"	fmla	v19.4s, v9.4s, v3.s[3]			 \n"

		"	fmla	v22.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[3]			 \n"

		"	fmla	v24.4s, v10.4s, v1.s[3]			 \n"
		"	fmla	v25.4s, v11.4s, v1.s[3]			 \n"

		"	fmla	v26.4s, v10.4s, v2.s[3]			 \n"
		"	fmla	v27.4s, v11.4s, v2.s[3]			 \n"

		"	fmla	v28.4s, v10.4s, v3.s[3]			 \n"
		"	fmla	v29.4s, v11.4s, v3.s[3]			 \n"

		".endm									 	 \n"


		".macro	M4_ADD_C 							 \n"

		"	prfm	PLDL1KEEP, [x25, #64]			 \n"
		"	prfm	PLDL1KEEP, [x26, #64]			 \n"
		"	prfm	PLDL1KEEP, [x27, #64]			 \n"
		"	prfm	PLDL1KEEP, [x28, #64]			 \n"

		"	ldp		q0, q1, [x25]					 \n"

		"	fadd	v12.4s, v12.4s, v0.4s			 \n"
		"	ldp		q2, q3, [x25, #32]				 \n"
		"	fadd	v13.4s, v13.4s, v1.4s			 \n"

		"	fadd	v22.4s, v2.4s, v22.4s			 \n"
		"	ldp		q4, q5, [x26]					 \n"
		"	fadd	v23.4s, v3.4s, v23.4s		 	 \n"

		"	fadd	v14.4s, v4.4s, v14.4s			 \n"
		"	ldp		q6, q7, [x26, #32]				 \n"
		"	fadd	v15.4s, v5.4s, v15.4s			 \n"

		"	prfm	PLDL1KEEP, [x27, #64]			 \n"
		"	prfm	PLDL1KEEP, [x28, #64]			 \n"

		"	fadd	v24.4s, v6.4s, v24.4s			 \n"
		"	ldp		q8, q9, [x27]					 \n"
		"	fadd 	v25.4s, v7.4s, v25.4s			 \n"

		"	fadd	v16.4s, v8.4s, v16.4s			 \n"
		"	ldp		q10, q11, [x27, #32]			 \n"
		"	fadd	v17.4s, v9.4s, v17.4s			 \n"

		"	fadd	v26.4s, v10.4s, v26.4s			 \n"
		"	ldp		q0, q1, [x28]				  	 \n"
		"	fadd	v27.4s, v11.4s, v27.4s			 \n"

		"	fadd	v18.4s, v0.4s, v18.4s			 \n"
		"	ldp		q2, q3, [x28, #32]				 \n"
		"	fadd	v19.4s, v1.4s, v19.4s			 \n"

		"	fadd	v28.4s, v2.4s, v28.4s			 \n"
		"	fadd	v29.4s, v3.4s, v29.4s			 \n"

		".endm 										 \n"


		".macro SAVE4x16							 \n"

		"	stp		q12, q13, [x25]			 		 \n"
		"	stp		q14, q15, [x26]			 	     \n"
		"	stp		q16, q17, [x27]					 \n"
		"	stp		q18, q19, [x28]					 \n"

		"	stp		q22, q23, [x25, #32]			 \n"
		"	stp		q24, q25, [x26, #32]			 \n"
		"	stp		q26, q27, [x27, #32]			 \n"
		"	stp		q28, q29, [x28, #32]			 \n"

		".endm 									 	 \n"


		".macro	KERNEL3x16_BEGIN_K					 \n"

		"										 	 \n"
		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q1, [x12], #16					 \n"
		"	ldp		q8, q9, [x24], #32			     \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"

		"	fmul	v12.4s, v8.4s, v0.s[0]	         \n"
		"	fmul	v13.4s, v9.4s, v0.s[0]	         \n"

		"	ldr		q2, [x13], #16					 \n"

		"	fmul	v14.4s, v8.4s, v1.s[0]	         \n"
		"	fmul	v15.4s, v9.4s, v1.s[0]	         \n"

		"	prfm	PLDL1KEEP, [x24, #128]			 \n"
		"	ldp		q10, q11, [x24], #32			 \n"

		"	fmul	v16.4s, v8.4s, v2.s[0]	         \n"
		"	ldr		q8, [x24], #16					 \n"
		"	fmul	v17.4s, v9.4s, v2.s[0]	         \n"
		"	ldr		q9, [x24], #16					 \n"

		"	fmul	v22.4s, v10.4s, v0.s[0]			 \n"
		"	fmul	v23.4s, v11.4s, v0.s[0]			 \n"

		"	fmul	v24.4s, v10.4s, v1.s[0]	         \n"
		"	fmul	v25.4s, v11.4s, v1.s[0]	         \n"

		"	fmul	v26.4s, v10.4s, v2.s[0]	         \n"
		"	ldr		q10, [x24], #16					 \n"
		"	fmul	v27.4s, v11.4s, v2.s[0]	         \n"
		"	ldr		q11, [x24], #16					 \n"


		".endm 										 \n"


		".macro KERNEL3x16_K0                   	 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"

		"	fmla	v12.4s, v8.4s, v0.s[0]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[0]			 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"

		"	fmla	v14.4s, v8.4s, v1.s[0]			 \n"
		"	fmla 	v15.4s, v9.4s, v1.s[0]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[0]			 \n"
		"	ldr		q8, [x24], #16					 \n"
		"	fmla 	v17.4s, v9.4s, v2.s[0]			 \n"
		"	ldr		q9, [x24], #16					 \n"

		"	fmla	v22.4s, v10.4s, v0.s[0]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[0]			 \n"

		"	fmla	v24.4s, v10.4s, v1.s[0]			 \n"
		"	fmla	v25.4s, v11.4s, v1.s[0]			 \n"

		"	fmla	v26.4s, v10.4s, v2.s[0]			 \n"
		"	ldr		q10, [x24], #16					 \n"
		"	fmla	v27.4s, v11.4s, v2.s[0]			 \n"
		"	ldr		q11, [x24], #16					 \n"


		".endm   									 \n"


		".macro KERNEL3x16_K1        	             \n"

		"   prfm    PLDL1KEEP, [x12, #64]       	 \n"
		"	fmla	v12.4s, v8.4s, v0.s[1]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[1]			 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"

		"	fmla	v14.4s, v8.4s, v1.s[1]			 \n"
		"	fmla	v15.4s, v9.4s, v1.s[1]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[1]			 \n"
		"	ldr		q8, [x24], #16					 \n"
		"	fmla	v17.4s, v9.4s, v2.s[1]			 \n"
		"	ldr		q9, [x24], #16					 \n"

		"	fmla	v22.4s, v10.4s, v0.s[1]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[1]			 \n"

		"	fmla	v24.4s, v10.4s, v1.s[1]			 \n"
		"	fmla	v25.4s, v11.4s, v1.s[1]			 \n"

		"	fmla	v26.4s, v10.4s, v2.s[1]			 \n"
		"	ldr		q10, [x24], #16					 \n"
		"	fmla	v27.4s, v11.4s, v2.s[1]			 \n"
		"	ldr		q11, [x24], #16					 \n"


		".endm									 	 \n"


		".macro KERNEL3x16_K2          		         \n"

		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"

		"	fmla	v12.4s, v8.4s, v0.s[2]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[2]			 \n"

		"	fmla	v14.4s, v8.4s, v1.s[2]			 \n"
		"	fmla 	v15.4s, v9.4s, v1.s[2]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[2]			 \n"
		"	ldr		q8, [x24], #16					 \n"
		"	fmla 	v17.4s, v9.4s, v2.s[2]			 \n"
		"	ldr		q9, [x24], #16					 \n"

		"	fmla	v22.4s, v10.4s, v0.s[2]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[2]			 \n"

		"	fmla	v24.4s, v10.4s, v1.s[2]			 \n"
		"	fmla	v25.4s, v11.4s, v1.s[2]			 \n"

		"	fmla	v26.4s, v10.4s, v2.s[2]			 \n"
		"	ldr		q10, [x24], #16					 \n"
		"	fmla	v27.4s, v11.4s, v2.s[2]			 \n"
		"	ldr		q11, [x24], #16					 \n"

		".endm									 	 \n"


		".macro KERNEL3x16_K3   	                 \n"

		"	fmla	v12.4s, v8.4s, v0.s[3]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[3]			 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"

		"	fmla	v14.4s, v8.4s, v1.s[3]			 \n"
		"	fmla	v15.4s, v9.4s, v1.s[3]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[3]			 \n"
		"	ldr		q8, [x24], #16					 \n"
		"	fmla	v17.4s, v9.4s, v2.s[3]			 \n"
		"	ldr		q9, [x24], #16					 \n"


		"	fmla	v22.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[3]			 \n"

		"	ldr		q0, [x11], #16					 \n"

		"	fmla	v24.4s, v10.4s, v1.s[3]			 \n"
		"	fmla	v25.4s, v11.4s, v1.s[3]			 \n"

		"	ldr		q1, [x12], #16					 \n"

		"	fmla	v26.4s, v10.4s, v2.s[3]			 \n"
		"	ldr		q10, [x24], #16				 	 \n"
		"	fmla	v27.4s, v11.4s, v2.s[3]			 \n"
		"	ldr		q11, [x24], #16					 \n"

		"	ldr		q2, [x13], #16					 \n"

		".endm									 	 \n"


		".macro KERNEL3x16_END_K   	                 \n"

		"	fmla	v12.4s, v8.4s, v0.s[3]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[3]			 \n"

		"	fmla	v14.4s, v8.4s, v1.s[3]			 \n"
		"	fmla	v15.4s, v9.4s, v1.s[3]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[3]			 \n"
		"	fmla	v17.4s, v9.4s, v2.s[3]			 \n"

		"	fmla	v22.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[3]			 \n"

		"	fmla	v24.4s, v10.4s, v1.s[3]			 \n"
		"	fmla	v25.4s, v11.4s, v1.s[3]			 \n"

		"	fmla	v26.4s, v10.4s, v2.s[3]			 \n"
		"	fmla	v27.4s, v11.4s, v2.s[3]			 \n"

		".endm									 	 \n"


		".macro	M3_ADD_C 							 \n"

		"	prfm	PLDL1KEEP, [x25, #64]			 \n"
		"	prfm	PLDL1KEEP, [x26, #64]			 \n"
		"	prfm	PLDL1KEEP, [x27, #64]			 \n"

		"	ldp		q0, q1, [x25]					 \n"

		"	fadd	v12.4s, v12.4s, v0.4s			 \n"
		"	ldp		q2, q3, [x25, #32]				 \n"
		"	fadd	v13.4s, v13.4s, v1.4s			 \n"

		"	fadd	v22.4s, v2.4s, v22.4s			 \n"
		"	ldp		q4, q5, [x26]					 \n"
		"	fadd	v23.4s, v3.4s, v23.4s		 	 \n"

		"	fadd	v14.4s, v4.4s, v14.4s			 \n"
		"	ldp		q6, q7, [x26, #32]				 \n"
		"	fadd	v15.4s, v5.4s, v15.4s			 \n"

		"	prfm	PLDL1KEEP, [x27, #64]			 \n"

		"	fadd	v24.4s, v6.4s, v24.4s			 \n"
		"	ldp		q8, q9, [x27]					 \n"
		"	fadd 	v25.4s, v7.4s, v25.4s			 \n"

		"	fadd	v16.4s, v8.4s, v16.4s			 \n"
		"	ldp		q10, q11, [x27, #32]			 \n"
		"	fadd	v17.4s, v9.4s, v17.4s			 \n"

		"	fadd	v26.4s, v10.4s, v26.4s			 \n"
		"	fadd	v27.4s, v11.4s, v27.4s			 \n"

		".endm 										 \n"


		".macro SAVE3x16							 \n"

		"	stp		q12, q13, [x25]			 		 \n"
		"	stp		q14, q15, [x26]			 	     \n"
		"	stp		q16, q17, [x27]					 \n"

		"	stp		q22, q23, [x25, #32]			 \n"
		"	stp		q24, q25, [x26, #32]			 \n"
		"	stp		q26, q27, [x27, #32]			 \n"

		".endm 									 	 \n"


		".macro	KERNEL2x16_BEGIN_K					 \n"

		"										 	 \n"
		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q1, [x12], #16					 \n"
		"	ldp		q8, q9, [x24], #32			     \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"

		"	fmul	v12.4s, v8.4s, v0.s[0]	         \n"
		"	fmul	v13.4s, v9.4s, v0.s[0]	         \n"

		"	prfm	PLDL1KEEP, [x24, #128]			 \n"
		"	ldp		q10, q11, [x24], #32			 \n"

		"	fmul	v14.4s, v8.4s, v1.s[0]	         \n"
		"	ldr		q8, [x24], #16					 \n"
		"	fmul	v15.4s, v9.4s, v1.s[0]	         \n"
		"	ldr		q9, [x24], #16					 \n"

		"	fmul	v22.4s, v10.4s, v0.s[0]			 \n"
		"	fmul	v23.4s, v11.4s, v0.s[0]			 \n"

		"	fmul	v24.4s, v10.4s, v1.s[0]	         \n"
		"	ldr		q10, [x24], #16					 \n"
		"	fmul	v25.4s, v11.4s, v1.s[0]	         \n"
		"	ldr		q11, [x24], #16					 \n"

		".endm 										 \n"


		".macro KERNEL2x16_K0                   	 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"

		"	fmla	v12.4s, v8.4s, v0.s[0]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[0]			 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"

		"	fmla	v14.4s, v8.4s, v1.s[0]			 \n"
		"	ldr		q8, [x24], #16					 \n"
		"	fmla 	v15.4s, v9.4s, v1.s[0]			 \n"
		"	ldr		q9, [x24], #16					 \n"

		"	fmla	v22.4s, v10.4s, v0.s[0]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[0]			 \n"

		"	fmla	v24.4s, v10.4s, v1.s[0]			 \n"
		"	ldr		q10, [x24], #16					 \n"
		"	fmla	v25.4s, v11.4s, v1.s[0]			 \n"
		"	ldr		q11, [x24], #16					 \n"


		".endm   									 \n"


		".macro KERNEL2x16_K1        	             \n"

		"   prfm    PLDL1KEEP, [x12, #64]       	 \n"
		"	fmla	v12.4s, v8.4s, v0.s[1]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[1]			 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"

		"	fmla	v14.4s, v8.4s, v1.s[1]			 \n"
		"	ldr		q8, [x24], #16					 \n"
		"	fmla	v15.4s, v9.4s, v1.s[1]			 \n"
		"	ldr		q9, [x24], #16					 \n"

		"	fmla	v22.4s, v10.4s, v0.s[1]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[1]			 \n"

		"	fmla	v24.4s, v10.4s, v1.s[1]			 \n"
		"	ldr		q10, [x24], #16					 \n"
		"	fmla	v25.4s, v11.4s, v1.s[1]			 \n"
		"	ldr		q11, [x24], #16					 \n"


		".endm									 	 \n"


		".macro KERNEL2x16_K2          		         \n"

		"	fmla	v12.4s, v8.4s, v0.s[2]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[2]			 \n"

		"	fmla	v14.4s, v8.4s, v1.s[2]			 \n"
		"	ldr		q8, [x24], #16					 \n"
		"	fmla 	v15.4s, v9.4s, v1.s[2]			 \n"
		"	ldr		q9, [x24], #16					 \n"

		"	fmla	v22.4s, v10.4s, v0.s[2]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[2]			 \n"

		"	fmla	v24.4s, v10.4s, v1.s[2]			 \n"
		"	ldr		q10, [x24], #16					 \n"
		"	fmla	v25.4s, v11.4s, v1.s[2]			 \n"
		"	ldr		q11, [x24], #16					 \n"

		".endm									 	 \n"


		".macro KERNEL2x16_K3   	                 \n"

		"	fmla	v12.4s, v8.4s, v0.s[3]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[3]			 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"

		"	fmla	v14.4s, v8.4s, v1.s[3]			 \n"
		"	ldr		q8, [x24], #16					 \n"
		"	fmla	v15.4s, v9.4s, v1.s[3]			 \n"
		"	ldr		q9, [x24], #16					 \n"

		"	fmla	v22.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[3]			 \n"

		"	ldr		q0, [x11], #16					 \n"

		"	fmla	v24.4s, v10.4s, v1.s[3]			 \n"
		"	ldr		q10, [x24], #16				 	 \n"
		"	fmla	v25.4s, v11.4s, v1.s[3]			 \n"
		"	ldr		q11, [x24], #16					 \n"

		"	ldr		q1, [x12], #16					 \n"

		".endm									 	 \n"


		".macro KERNEL2x16_END_K   	                 \n"

		"	fmla	v12.4s, v8.4s, v0.s[3]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[3]			 \n"

		"	fmla	v14.4s, v8.4s, v1.s[3]			 \n"
		"	fmla	v15.4s, v9.4s, v1.s[3]			 \n"

		"	fmla	v22.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[3]			 \n"

		"	fmla	v24.4s, v10.4s, v1.s[3]			 \n"
		"	fmla	v25.4s, v11.4s, v1.s[3]			 \n"

		".endm									 	 \n"


		".macro	M2_ADD_C 							 \n"

		"	prfm	PLDL1KEEP, [x25, #64]			 \n"
		"	prfm	PLDL1KEEP, [x26, #64]			 \n"

		"	ldp		q0, q1, [x25]					 \n"

		"	fadd	v12.4s, v12.4s, v0.4s			 \n"
		"	ldp		q2, q3, [x25, #32]				 \n"
		"	fadd	v13.4s, v13.4s, v1.4s			 \n"

		"	fadd	v22.4s, v2.4s, v22.4s			 \n"
		"	ldp		q4, q5, [x26]					 \n"
		"	fadd	v23.4s, v3.4s, v23.4s		 	 \n"

		"	fadd	v14.4s, v4.4s, v14.4s			 \n"
		"	ldp		q6, q7, [x26, #32]				 \n"
		"	fadd	v15.4s, v5.4s, v15.4s			 \n"


		"	fadd	v24.4s, v6.4s, v24.4s			 \n"
		"	fadd 	v25.4s, v7.4s, v25.4s			 \n"

		".endm 										 \n"


		".macro SAVE2x16							 \n"

		"	stp		q12, q13, [x25]			 		 \n"
		"	stp		q14, q15, [x26]			 	     \n"

		"	stp		q22, q23, [x25, #32]			 \n"
		"	stp		q24, q25, [x26, #32]			 \n"

		".endm 									 	 \n"



		".macro	KERNEL1x16_BEGIN_K					 \n"

		"										 	 \n"
		"	ldr		q0, [x11], #16					 \n"
		"	ldp		q8, q9, [x24], #32			     \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x24, #128]			 \n"
		"	ldp		q10, q11, [x24], #32			 \n"

		"	fmul	v12.4s, v8.4s, v0.s[0]	         \n"
		"	ldr		q8, [x24], #16					 \n"
		"	fmul	v13.4s, v9.4s, v0.s[0]	         \n"
		"	ldr		q9, [x24], #16					 \n"

		"	fmul	v22.4s, v10.4s, v0.s[0]			 \n"
		"	ldr		q10, [x24], #16					 \n"
		"	fmul	v23.4s, v11.4s, v0.s[0]			 \n"
		"	ldr		q11, [x24], #16					 \n"

		".endm 										 \n"


		".macro KERNEL1x16_K0                   	 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"

		"	fmla	v12.4s, v8.4s, v0.s[0]			 \n"
		"	ldr		q8, [x24], #16					 \n"
		"	fmla	v13.4s, v9.4s, v0.s[0]			 \n"
		"	ldr		q9, [x24], #16					 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"

		"	fmla	v22.4s, v10.4s, v0.s[0]			 \n"
		"	ldr		q10, [x24], #16					 \n"
		"	fmla	v23.4s, v11.4s, v0.s[0]			 \n"
		"	ldr		q11, [x24], #16					 \n"

		".endm   									 \n"


		".macro KERNEL1x16_K1        	             \n"

		"	fmla	v12.4s, v8.4s, v0.s[1]			 \n"
		"	ldr		q8, [x24], #16					 \n"
		"	fmla	v13.4s, v9.4s, v0.s[1]			 \n"
		"	ldr		q9, [x24], #16					 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"

		"	fmla	v22.4s, v10.4s, v0.s[1]			 \n"
		"	ldr		q10, [x24], #16					 \n"
		"	fmla	v23.4s, v11.4s, v0.s[1]			 \n"
		"	ldr		q11, [x24], #16					 \n"

		".endm									 	 \n"


		".macro KERNEL1x16_K2          		         \n"

		"	fmla	v12.4s, v8.4s, v0.s[2]			 \n"
		"	ldr		q8, [x24], #16					 \n"
		"	fmla	v13.4s, v9.4s, v0.s[2]			 \n"
		"	ldr		q9, [x24], #16					 \n"

		"	fmla	v22.4s, v10.4s, v0.s[2]			 \n"
		"	ldr		q10, [x24], #16					 \n"
		"	fmla	v23.4s, v11.4s, v0.s[2]			 \n"
		"	ldr		q11, [x24], #16					 \n"

		".endm									 	 \n"


		".macro KERNEL1x16_K3   	                 \n"

		"	fmla	v12.4s, v8.4s, v0.s[3]			 \n"
		"	ldr		q8, [x24], #16					 \n"
		"	fmla	v13.4s, v9.4s, v0.s[3]			 \n"
		"	ldr		q9, [x24], #16					 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"

		"	fmla	v22.4s, v10.4s, v0.s[3]			 \n"
		"	ldr		q10, [x24], #16				 	 \n"
		"	fmla	v23.4s, v11.4s, v0.s[3]			 \n"
		"	ldr		q11, [x24], #16					 \n"

		"	ldr		q0, [x11], #16					 \n"

		".endm									 	 \n"


		".macro	M1_ADD_C 							 \n"


		"	prfm	PLDL1KEEP, [x25, #64]			 \n"

		"	ldp		q0, q1, [x25]					 \n"

		"	fadd	v12.4s, v12.4s, v0.4s			 \n"
		"	ldp		q2, q3, [x25, #32]				 \n"
		"	fadd	v13.4s, v13.4s, v1.4s			 \n"

		"	fadd	v22.4s, v2.4s, v22.4s			 \n"
		"	fadd	v23.4s, v3.4s, v23.4s		 	 \n"

		".endm 										 \n"


		".macro KERNEL1x16_END_K   	                 \n"

		"	fmla	v12.4s, v8.4s, v0.s[3]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[3]			 \n"

		"	fmla	v22.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[3]			 \n"

		".endm									 	 \n"


		".macro SAVE1x16							 \n"

		"	stp		q12, q13, [x25]			 		 \n"
		"	stp		q22, q23, [x25, #32]			 \n"

		".endm 									 	 \n"




		".macro	N8M5_ADD_C 							 \n"

		"	prfm	PLDL1KEEP, [x25, #64]			 \n"
		"	prfm	PLDL1KEEP, [x26, #64]			 \n"
		"	prfm	PLDL1KEEP, [x27, #64]			 \n"
		"	prfm	PLDL1KEEP, [x28, #64]			 \n"
		"	prfm	PLDL1KEEP, [x29, #64]			 \n"

		"	ldp		q0, q1, [x25]					 \n"

		"	fadd	v12.4s, v12.4s, v0.4s			 \n"
		"	ldp		q4, q5, [x26]					 \n"
		"	fadd	v13.4s, v13.4s, v1.4s			 \n"

		"	fadd	v14.4s, v4.4s, v14.4s			 \n"
		"	ldp		q8, q9, [x27]					 \n"
		"	fadd	v15.4s, v5.4s, v15.4s			 \n"

		"	prfm	PLDL1KEEP, [x28, #64]			 \n"
		"	prfm	PLDL1KEEP, [x29, #64]			 \n"


		"	fadd	v16.4s, v8.4s, v16.4s			 \n"
		"	ldp		q0, q1, [x28]				  	 \n"
		"	fadd	v17.4s, v9.4s, v17.4s			 \n"

		"	fadd	v18.4s, v0.4s, v18.4s			 \n"
		"	ldp		q4, q5, [x29]					 \n"
		"	fadd	v19.4s, v1.4s, v19.4s			 \n"

		"	fadd	v20.4s, v4.4s, v20.4s			 \n"
		"	fadd	v21.4s, v5.4s, v21.4s			 \n"


		".endm 										 \n"



		".macro N8_SAVE5x8							 \n"

		"	subs	x21, x21, #1					 \n"

		"	add 	x10, x10, x6, lsl #4			 \n"
		"	add		x10, x10, x6, lsl #2 			 \n"

		"	stp		q12, q13, [x25]			 		 \n"
		"	add		x25, x29, x9, lsl #2			 \n"
		"	stp		q14, q15, [x26]			 	     \n"
		"	add		x26, x25, x9, lsl #2			 \n"
		"	stp		q16, q17, [x27]					 \n"
		"	add		x27, x26, x9, lsl #2			 \n"
		"	stp		q18, q19, [x28]					 \n"
		"	add		x28, x27, x9, lsl #2			 \n"
		"	stp		q20, q21, [x29]					 \n"
		"	add		x29, x28, x9, lsl #2			 \n"

		".endm 									 	 \n"


		".macro	N8_KERNEL5x8_BEGIN_K  				 \n"

		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q1, [x12], #16					 \n"
		"	ldp		q8, q9, [x24], #32			     \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x14, #64] 			 \n"
		"	prfm	PLDL1KEEP, [x15, #64]			 \n"

		"	fmul	v12.4s, v8.4s, v0.s[0]	         \n"
		"	fmul	v13.4s, v9.4s, v0.s[0]	         \n"

		"	ldr		q2, [x13], #16					 \n"

		"	fmul	v14.4s, v8.4s, v1.s[0]	         \n"
		"	fmul	v15.4s, v9.4s, v1.s[0]	         \n"

		"	ldr		q3, [x14], #16					 \n"

		"	fmul	v16.4s, v8.4s, v2.s[0]	         \n"
		"	fmul	v17.4s, v9.4s, v2.s[0]	         \n"

		"	prfm	PLDL1KEEP, [x24, #128]			 \n"

		"	ldr		q4, [x15], #16				 	 \n"

		"	fmul	v18.4s, v8.4s, v3.s[0]	         \n"
		"	fmul	v19.4s, v9.4s, v3.s[0]	         \n"

		"	ldp		q10, q11, [x24], #32			 \n"

		"	fmul	v20.4s, v8.4s, v4.s[0]			 \n"
		"	fmul	v21.4s, v9.4s, v4.s[0]			 \n"

		".endm 										 \n"


		".macro N8_KERNEL5x8_K0                   	 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"

		"	fmla	v12.4s, v8.4s, v0.s[0]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[0]			 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"
		"	ldp		q10, q11, [x24], #32			 \n"

		"	fmla	v14.4s, v8.4s, v1.s[0]			 \n"
		"	fmla 	v15.4s, v9.4s, v1.s[0]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[0]			 \n"
		"	fmla 	v17.4s, v9.4s, v2.s[0]			 \n"

		"	fmla	v18.4s, v8.4s, v3.s[0]			 \n"
		"	fmla 	v19.4s, v9.4s, v3.s[0]			 \n"

		"	fmla	v20.4s, v8.4s, v4.s[0]			 \n"
		"	fmla	v21.4s, v9.4s, v4.s[0]			 \n"

		".endm   									 \n"


		".macro N8_KERNEL5x8_K1        	             \n"

		"   prfm    PLDL1KEEP, [x12, #64]       	 \n"
		"	fmla	v12.4s, v10.4s, v0.s[1]			 \n"
		"	fmla	v13.4s, v11.4s, v0.s[1]			 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"
		"	ldp		q8, q9, [x24], #32				 \n"

		"	fmla	v14.4s, v10.4s, v1.s[1]			 \n"
		"	fmla	v15.4s, v11.4s, v1.s[1]			 \n"

		"	fmla	v16.4s, v10.4s, v2.s[1]			 \n"
		"	fmla	v17.4s, v11.4s, v2.s[1]			 \n"

		"	fmla	v18.4s, v10.4s, v3.s[1]			 \n"
		"	fmla	v19.4s, v11.4s, v3.s[1]			 \n"

		"	fmla	v20.4s, v10.4s, v4.s[1]			 \n"
		"	fmla	v21.4s, v11.4s, v4.s[1]			 \n"

		".endm									 	 \n"


		".macro N8_KERNEL5x8_K2          		     \n"

		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"

		"	fmla	v12.4s, v8.4s, v0.s[2]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[2]			 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"
		"	ldp		q10, q11, [x24], #32			 \n"

		"	fmla	v14.4s, v8.4s, v1.s[2]			 \n"
		"	fmla 	v15.4s, v9.4s, v1.s[2]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[2]			 \n"
		"	fmla 	v17.4s, v9.4s, v2.s[2]			 \n"

		"	fmla	v18.4s, v8.4s, v3.s[2]			 \n"
		"	fmla 	v19.4s, v9.4s, v3.s[2]			 \n"

		"	fmla	v20.4s, v8.4s, v4.s[2]			 \n"
		"	fmla	v21.4s, v9.4s, v4.s[2]			 \n"

		".endm									 	 \n"


		".macro N8_KERNEL5x8_K3   	                 \n"

		"   prfm    PLDL1KEEP, [x15, #64]       	 \n"
		"	fmla	v12.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v13.4s, v11.4s, v0.s[3]			 \n"

		"	ldr		q0, [x11], #16					 \n"
		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"
		"	ldp		q8, q9, [x24], #32				 \n"

		"	fmla	v14.4s, v10.4s, v1.s[3]			 \n"
		"	fmla	v15.4s, v11.4s, v1.s[3]			 \n"

		"	ldr		q1, [x12], #16					 \n"

		"	fmla	v16.4s, v10.4s, v2.s[3]			 \n"
		"	fmla	v17.4s, v11.4s, v2.s[3]			 \n"

		"	ldr		q2, [x13], #16					 \n"

		"	fmla	v18.4s, v10.4s, v3.s[3]			 \n"
		"	fmla	v19.4s, v11.4s, v3.s[3]			 \n"

		"	ldr		q3, [x14], #16					 \n"

		"	fmla	v20.4s, v10.4s, v4.s[3]			 \n"
		"	fmla	v21.4s, v11.4s, v4.s[3]			 \n"

		"	ldr		q4, [x15], #16					 \n"

		".endm									 	 \n"


		".macro N8_KERNEL5x8_END_K   	             \n"

		"	fmla	v12.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v13.4s, v11.4s, v0.s[3]			 \n"

		"	fmla	v14.4s, v10.4s, v1.s[3]			 \n"
		"	fmla	v15.4s, v11.4s, v1.s[3]			 \n"

		"	fmla	v16.4s, v10.4s, v2.s[3]			 \n"
		"	fmla	v17.4s, v11.4s, v2.s[3]			 \n"

		"	fmla	v18.4s, v10.4s, v3.s[3]			 \n"
		"	fmla	v19.4s, v11.4s, v3.s[3]			 \n"

		"	fmla	v20.4s, v10.4s, v4.s[3]			 \n"
		"	fmla	v21.4s, v11.4s, v4.s[3]			 \n"

		".endm									 	 \n"


		".macro 	N8_KERNEL4x8_BEGIN_K			 \n"

		"										 	 \n"
		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q1, [x12], #16					 \n"
		"	ldp		q8, q9, [x24], #32			     \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x14, #64] 			 \n"

		"	fmul	v12.4s, v8.4s, v0.s[0]	         \n"
		"	fmul	v13.4s, v9.4s, v0.s[0]	         \n"

		"	ldr		q2, [x13], #16					 \n"

		"	fmul	v14.4s, v8.4s, v1.s[0]	         \n"
		"	fmul	v15.4s, v9.4s, v1.s[0]	         \n"

		"	ldr		q3, [x14], #16					 \n"

		"	fmul	v16.4s, v8.4s, v2.s[0]	         \n"
		"	fmul	v17.4s, v9.4s, v2.s[0]	         \n"

		"	prfm	PLDL1KEEP, [x24, #128]			 \n"
		"	ldp		q10, q11, [x24], #32			 \n"

		"	fmul	v18.4s, v8.4s, v3.s[0]	         \n"
		"	fmul	v19.4s, v9.4s, v3.s[0]	         \n"

		".endm 										 \n"


		".macro N8_KERNEL4x8_K0                   	 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"

		"	fmla	v12.4s, v8.4s, v0.s[0]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[0]			 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"
		"	ldp		q10, q11, [x24], #32			 \n"

		"	fmla	v14.4s, v8.4s, v1.s[0]			 \n"
		"	fmla 	v15.4s, v9.4s, v1.s[0]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[0]			 \n"
		"	fmla 	v17.4s, v9.4s, v2.s[0]			 \n"

		"	fmla	v18.4s, v8.4s, v3.s[0]			 \n"
		"	fmla 	v19.4s, v9.4s, v3.s[0]			 \n"


		".endm   									 \n"


		".macro N8_KERNEL4x8_K1        	             \n"

		"   prfm    PLDL1KEEP, [x12, #64]       	 \n"
		"	fmla	v12.4s, v10.4s, v0.s[1]			 \n"
		"	fmla	v13.4s, v11.4s, v0.s[1]			 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"
		"	ldp		q8, q9, [x24], #32				 \n"

		"	fmla	v14.4s, v10.4s, v1.s[1]			 \n"
		"	fmla	v15.4s, v11.4s, v1.s[1]			 \n"

		"	fmla	v16.4s, v10.4s, v2.s[1]			 \n"
		"	fmla	v17.4s, v11.4s, v2.s[1]			 \n"

		"	fmla	v18.4s, v10.4s, v3.s[1]			 \n"
		"	fmla	v19.4s, v11.4s, v3.s[1]			 \n"


		".endm									 	 \n"


		".macro N8_KERNEL4x8_K2          		     \n"

		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"

		"	fmla	v12.4s, v8.4s, v0.s[2]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[2]			 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"
		"	ldp		q10, q11, [x24], #32			 \n"

		"	fmla	v14.4s, v8.4s, v1.s[2]			 \n"
		"	fmla 	v15.4s, v9.4s, v1.s[2]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[2]			 \n"
		"	fmla 	v17.4s, v9.4s, v2.s[2]			 \n"

		"	fmla	v18.4s, v8.4s, v3.s[2]			 \n"
		"	fmla 	v19.4s, v9.4s, v3.s[2]			 \n"

		".endm									 	 \n"


		".macro N8_KERNEL4x8_K3   	                 \n"

		"   prfm    PLDL1KEEP, [x15, #64]       	 \n"
		"	fmla	v12.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v13.4s, v11.4s, v0.s[3]			 \n"

		"	ldr		q0, [x11], #16					 \n"
		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"
		"	ldp		q8, q9, [x24], #32				 \n"

		"	fmla	v14.4s, v10.4s, v1.s[3]			 \n"
		"	fmla	v15.4s, v11.4s, v1.s[3]			 \n"

		"	ldr		q1, [x12], #16					 \n"

		"	fmla	v16.4s, v10.4s, v2.s[3]			 \n"
		"	fmla	v17.4s, v11.4s, v2.s[3]			 \n"

		"	ldr		q2, [x13], #16					 \n"

		"	fmla	v18.4s, v10.4s, v3.s[3]			 \n"
		"	fmla	v19.4s, v11.4s, v3.s[3]			 \n"

		"	ldr		q3, [x14], #16					 \n"


		".endm									 	 \n"


		".macro N8_KERNEL4x8_END_K   	             \n"

		"	fmla	v12.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v13.4s, v11.4s, v0.s[3]			 \n"

		"	fmla	v14.4s, v10.4s, v1.s[3]			 \n"
		"	fmla	v15.4s, v11.4s, v1.s[3]			 \n"

		"	fmla	v16.4s, v10.4s, v2.s[3]			 \n"
		"	fmla	v17.4s, v11.4s, v2.s[3]			 \n"

		"	fmla	v18.4s, v10.4s, v3.s[3]			 \n"
		"	fmla	v19.4s, v11.4s, v3.s[3]			 \n"

		".endm									 	 \n"


		".macro	N8M4_ADD_C							 \n"


		"	prfm	PLDL1KEEP, [x25, #64]			 \n"
		"	prfm	PLDL1KEEP, [x26, #64]			 \n"
		"	prfm	PLDL1KEEP, [x27, #64]			 \n"
		"	prfm	PLDL1KEEP, [x28, #64]			 \n"

		"	ldp		q0, q1, [x25]					 \n"

		"	fadd	v12.4s, v12.4s, v0.4s			 \n"
		"	ldp		q4, q5, [x26]					 \n"
		"	fadd	v13.4s, v13.4s, v1.4s			 \n"

		"	fadd	v14.4s, v4.4s, v14.4s			 \n"
		"	ldp		q8, q9, [x27]					 \n"
		"	fadd	v15.4s, v5.4s, v15.4s			 \n"

		"	prfm	PLDL1KEEP, [x28, #64]			 \n"

		"	fadd	v16.4s, v8.4s, v16.4s			 \n"
		"	ldp		q0, q1, [x28]				  	 \n"
		"	fadd	v17.4s, v9.4s, v17.4s			 \n"

		"	fadd	v18.4s, v0.4s, v18.4s			 \n"
		"	fadd	v19.4s, v1.4s, v19.4s			 \n"

		".endm 										 \n"


		".macro N8_SAVE4x8 							 \n"

		"	stp		q12, q13, [x25]			 		 \n"
		"	stp		q14, q15, [x26]			 	     \n"
		"	stp		q16, q17, [x27]					 \n"
		"	stp		q18, q19, [x28]					 \n"

		".endm 										 \n"


		".macro 	N8_KERNEL3x8_BEGIN_K			 \n"

		"										 	 \n"
		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q1, [x12], #16					 \n"
		"	ldp		q8, q9, [x24], #32			     \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x14, #64] 			 \n"

		"	fmul	v12.4s, v8.4s, v0.s[0]	         \n"
		"	fmul	v13.4s, v9.4s, v0.s[0]	         \n"

		"	ldr		q2, [x13], #16					 \n"

		"	fmul	v14.4s, v8.4s, v1.s[0]	         \n"
		"	fmul	v15.4s, v9.4s, v1.s[0]	         \n"

		"	prfm	PLDL1KEEP, [x24, #128]			 \n"
		"	ldp		q10, q11, [x24], #32			 \n"

		"	fmul	v16.4s, v8.4s, v2.s[0]	         \n"
		"	fmul	v17.4s, v9.4s, v2.s[0]	         \n"

		".endm 										 \n"


		".macro N8_KERNEL3x8_K0                   	 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"

		"	fmla	v12.4s, v8.4s, v0.s[0]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[0]			 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"
		"	ldp		q10, q11, [x24], #32			 \n"

		"	fmla	v14.4s, v8.4s, v1.s[0]			 \n"
		"	fmla 	v15.4s, v9.4s, v1.s[0]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[0]			 \n"
		"	fmla 	v17.4s, v9.4s, v2.s[0]			 \n"


		".endm   									 \n"


		".macro N8_KERNEL3x8_K1        	             \n"

		"   prfm    PLDL1KEEP, [x12, #64]       	 \n"
		"	fmla	v12.4s, v10.4s, v0.s[1]			 \n"
		"	fmla	v13.4s, v11.4s, v0.s[1]			 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"
		"	ldp		q8, q9, [x24], #32				 \n"

		"	fmla	v14.4s, v10.4s, v1.s[1]			 \n"
		"	fmla	v15.4s, v11.4s, v1.s[1]			 \n"

		"	fmla	v16.4s, v10.4s, v2.s[1]			 \n"
		"	fmla	v17.4s, v11.4s, v2.s[1]			 \n"


		".endm									 	 \n"


		".macro N8_KERNEL3x8_K2          		     \n"

		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"

		"	fmla	v12.4s, v8.4s, v0.s[2]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[2]			 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"
		"	ldp		q10, q11, [x24], #32			 \n"

		"	fmla	v14.4s, v8.4s, v1.s[2]			 \n"
		"	fmla 	v15.4s, v9.4s, v1.s[2]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[2]			 \n"
		"	fmla 	v17.4s, v9.4s, v2.s[2]			 \n"

		".endm									 	 \n"


		".macro N8_KERNEL3x8_K3   	                 \n"

		"   prfm    PLDL1KEEP, [x15, #64]       	 \n"
		"	fmla	v12.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v13.4s, v11.4s, v0.s[3]			 \n"

		"	ldr		q0, [x11], #16					 \n"
		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"
		"	ldp		q8, q9, [x24], #32				 \n"

		"	fmla	v14.4s, v10.4s, v1.s[3]			 \n"
		"	fmla	v15.4s, v11.4s, v1.s[3]			 \n"

		"	ldr		q1, [x12], #16					 \n"

		"	fmla	v16.4s, v10.4s, v2.s[3]			 \n"
		"	fmla	v17.4s, v11.4s, v2.s[3]			 \n"

		"	ldr		q2, [x13], #16					 \n"


		".endm									 	 \n"


		".macro N8_KERNEL3x8_END_K   	             \n"

		"	fmla	v12.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v13.4s, v11.4s, v0.s[3]			 \n"

		"	fmla	v14.4s, v10.4s, v1.s[3]			 \n"
		"	fmla	v15.4s, v11.4s, v1.s[3]			 \n"

		"	fmla	v16.4s, v10.4s, v2.s[3]			 \n"
		"	fmla	v17.4s, v11.4s, v2.s[3]			 \n"

		".endm									 	 \n"



		".macro N8M3_ADD_C 							 \n"


		"	prfm	PLDL1KEEP, [x25, #64]			 \n"
		"	prfm	PLDL1KEEP, [x26, #64]			 \n"
		"	prfm	PLDL1KEEP, [x27, #64]			 \n"

		"	ldp		q0, q1, [x25]					 \n"

		"	fadd	v12.4s, v12.4s, v0.4s			 \n"
		"	ldp		q4, q5, [x26]					 \n"
		"	fadd	v13.4s, v13.4s, v1.4s			 \n"

		"	fadd	v14.4s, v4.4s, v14.4s			 \n"
		"	ldp		q8, q9, [x27]					 \n"
		"	fadd	v15.4s, v5.4s, v15.4s			 \n"

		"	fadd	v16.4s, v8.4s, v16.4s			 \n"
		"	fadd	v17.4s, v9.4s, v17.4s			 \n"


		".endm 										 \n"


		".macro N8_SAVE3x8 							 \n"

		"	stp		q12, q13, [x25]			 		 \n"
		"	stp		q14, q15, [x26]			 	     \n"
		"	stp		q16, q17, [x27]					 \n"

		".endm 										 \n"



		".macro  N8_KERNEL2x8_BEGIN_K			     \n"

		"										 	 \n"
		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q1, [x12], #16					 \n"
		"	ldp		q8, q9, [x24], #32			     \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"

		"	fmul	v12.4s, v8.4s, v0.s[0]	         \n"
		"	fmul	v13.4s, v9.4s, v0.s[0]	         \n"

		"	prfm	PLDL1KEEP, [x24, #128]			 \n"
		"	ldp		q10, q11, [x24], #32			 \n"

		"	fmul	v14.4s, v8.4s, v1.s[0]	         \n"
		"	fmul	v15.4s, v9.4s, v1.s[0]	         \n"

		".endm 										 \n"


		".macro N8_KERNEL2x8_K0                   	 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"
		"	ldp		q10, q11, [x24], #32			 \n"

		"	fmla	v12.4s, v8.4s, v0.s[0]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[0]			 \n"

		"	fmla	v14.4s, v8.4s, v1.s[0]			 \n"
		"	fmla 	v15.4s, v9.4s, v1.s[0]			 \n"


		".endm   									 \n"


		".macro N8_KERNEL2x8_K1        	             \n"

		"   prfm    PLDL1KEEP, [x12, #64]       	 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"
		"	ldp		q8, q9, [x24], #32				 \n"

		"	fmla	v12.4s, v10.4s, v0.s[1]			 \n"
		"	fmla	v13.4s, v11.4s, v0.s[1]			 \n"

		"	fmla	v14.4s, v10.4s, v1.s[1]			 \n"
		"	fmla	v15.4s, v11.4s, v1.s[1]			 \n"

		".endm									 	 \n"


		".macro N8_KERNEL2x8_K2          		     \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"
		"	ldp		q10, q11, [x24], #32			 \n"

		"	fmla	v12.4s, v8.4s, v0.s[2]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[2]			 \n"

		"	fmla	v14.4s, v8.4s, v1.s[2]			 \n"
		"	fmla 	v15.4s, v9.4s, v1.s[2]			 \n"

		".endm									 	 \n"


		".macro N8_KERNEL2x8_K3   	                 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"
		"	ldp		q8, q9, [x24], #32				 \n"

		"	fmla	v12.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v13.4s, v11.4s, v0.s[3]			 \n"

		"	ldr		q0, [x11], #16					 \n"

		"	fmla	v14.4s, v10.4s, v1.s[3]			 \n"
		"	fmla	v15.4s, v11.4s, v1.s[3]			 \n"

		"	ldr		q1, [x12], #16					 \n"

		".endm									 	 \n"


		".macro N8_KERNEL2x8_END_K   	             \n"

		"	fmla	v12.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v13.4s, v11.4s, v0.s[3]			 \n"

		"	fmla	v14.4s, v10.4s, v1.s[3]			 \n"
		"	fmla	v15.4s, v11.4s, v1.s[3]			 \n"

		".endm									 	 \n"



		".macro N8M2_ADD_C 							 \n"


		"	prfm	PLDL1KEEP, [x25, #64]			 \n"
		"	prfm	PLDL1KEEP, [x26, #64]			 \n"

		"	ldp		q0, q1, [x25]					 \n"

		"	fadd	v12.4s, v12.4s, v0.4s			 \n"
		"	ldp		q4, q5, [x26]					 \n"
		"	fadd	v13.4s, v13.4s, v1.4s			 \n"

		"	fadd	v14.4s, v4.4s, v14.4s			 \n"
		"	fadd	v15.4s, v5.4s, v15.4s			 \n"

		".endm 										 \n"


		".macro N8_SAVE2x8 							 \n"

		"	stp		q12, q13, [x25]			 		 \n"
		"	stp		q14, q15, [x26]			 	     \n"

		".endm 										 \n"


		".macro  N8_KERNEL1x8_BEGIN_K			     \n"

		"										 	 \n"
		"	ldr		q0, [x11], #16					 \n"
		"	ldp		q8, q9, [x24], #32			     \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"

		"	prfm	PLDL1KEEP, [x24, #128]			 \n"
		"	ldp		q10, q11, [x24], #32			 \n"

		"	fmul	v12.4s, v8.4s, v0.s[0]	         \n"
		"	fmul	v13.4s, v9.4s, v0.s[0]	         \n"

		".endm 										 \n"


		".macro N8_KERNEL1x8_K0                   	 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"
		"	ldp		q10, q11, [x24], #32			 \n"

		"	fmla	v12.4s, v8.4s, v0.s[0]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[0]			 \n"
		".endm   									 \n"


		".macro N8_KERNEL1x8_K1        	             \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"
		"	ldp		q8, q9, [x24], #32				 \n"

		"	fmla	v12.4s, v10.4s, v0.s[1]			 \n"
		"	fmla	v13.4s, v11.4s, v0.s[1]			 \n"

		".endm									 	 \n"


		".macro N8_KERNEL1x8_K2          		     \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"
		"	ldp		q10, q11, [x24], #32			 \n"

		"	fmla	v12.4s, v8.4s, v0.s[2]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[2]			 \n"

		".endm									 	 \n"


		".macro N8_KERNEL1x8_K3   	                 \n"

		"   prfm    PLDL1KEEP, [x24, #128]       	 \n"
		"	ldp		q8, q9, [x24], #32				 \n"

		"	fmla	v12.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v13.4s, v11.4s, v0.s[3]			 \n"

		"	ldr		q0, [x11], #16					 \n"

		".endm									 	 \n"


		".macro N8_KERNEL1x8_END_K   	             \n"

		"	fmla	v12.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v13.4s, v11.4s, v0.s[3]			 \n"

		".endm									 	 \n"


		".macro N8M1_ADD_C 							 \n"


		"	prfm	PLDL1KEEP, [x25, #64]			 \n"


		"	ldp		q0, q1, [x25]					 \n"

		"	fadd	v12.4s, v12.4s, v0.4s			 \n"
		"	fadd	v13.4s, v13.4s, v1.4s			 \n"

		".endm 										 \n"


		".macro N8_SAVE1x8 							 \n"

		"	stp		q12, q13, [x25]			 		 \n"

		".endm 										 \n"




		".macro	N4M5_KERNEL5x4_BEGIN_K				\n"


		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q1, [x12], #16				     \n"
		"	ldr		q5, [x16], #16					 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x14, #64] 			 \n"
		"	prfm	PLDL1KEEP, [x15, #64]			 \n"

		"	prfm 	PLDL1KEEP, [x17, #128]			 \n"

		"	fmul	v12.4s, v0.4s, v5.4s			 \n"
		"	ldr		q2, [x13], #16					 \n"
		"	fmul	v13.4s, v1.4s, v5.4s			 \n"

		"	ldr		q3, [x14], #16					 \n"

		"	fmul	v14.4s, v2.4s, v5.4s		 	 \n"
		"	ldr		q4, [x15], #16					 \n"
		"	fmul	v15.4s, v3.4s, v5.4s			 \n"

		"	ldr		q6, [x17], #16					 \n"
		"	fmul	v16.4s, v4.4s, v5.4s			 \n"
		"	ldr		q7, [x18], #16					 \n"
		"	fmul	v17.4s, v0.4s, v6.4s			 \n"

		"	ldr		q8, [x19], #16					 \n"
		"	ldr		q9, [x11], #16					 \n"

		"	fmul	v18.4s, v1.4s, v6.4s			 \n"
		"	fmul	v19.4s, v2.4s, v6.4s			 \n"
		"	fmul	v20.4s, v3.4s, v6.4s			 \n"
		"	fmul	v21.4s, v4.4s, v6.4s			 \n"

		"	ldr		q10, [x12], #16					 \n"

		"	fmul	v22.4s, v0.4s, v7.4s			 \n"
		"	fmul	v23.4s, v1.4s, v7.4s			 \n"
		"	fmul	v24.4s, v2.4s, v7.4s			 \n"
		"	fmul	v25.4s, v3.4s, v7.4s			 \n"
		"	fmul	v26.4s, v4.4s, v7.4s			 \n"

		"	ldr		q11, [x13], #16					 \n"

		"	fmul	v27.4s, v0.4s, v8.4s			 \n"
		"	ldr		q0, [x14], #16					 \n"
		"	fmul	v28.4s, v1.4s, v8.4s			 \n"
		"	ldr		q1, [x15], #16					 \n"

		"	ldr		q5, [x16], #16					 \n"

		"	fmul	v29.4s, v2.4s, v8.4s			 \n"
		"	fmul	v30.4s, v3.4s, v8.4s			 \n"
		"	fmul	v31.4s, v4.4s, v8.4s			 \n"


		".endm 										 \n"




		".macro N4M5_KERNEL5x4_K0                    \n"

		"	prfm 	PLDL1KEEP, [x16, #128]			 \n"

		"	ldr		q6, [x17], #16					 \n"

		"	fmla	v12.4s, v0.4s, v5.4s			 \n"
		"	fmla	v13.4s, v1.4s, v5.4s			 \n"

		"	prfm 	PLDL1KEEP, [x17, #128]			 \n"

		"	ldr		q7, [x18], #16					 \n"

		"	prfm 	PLDL1KEEP, [x11, #64]			 \n"

		"	fmla	v14.4s, v2.4s, v5.4s			 \n"
		"	fmla	v15.4s, v3.4s, v5.4s			 \n"
		"	fmla	v16.4s, v4.4s, v5.4s			 \n"

		"	prfm 	PLDL1KEEP, [x12, #64]			 \n"

		"	ldr		q8, [x19], #16					 \n"

		"	fmla	v17.4s, v0.4s, v6.4s			 \n"
		"	fmla	v18.4s, v1.4s, v6.4s		 	 \n"
		"	fmla	v19.4s, v2.4s, v6.4s			 \n"
		"	fmla	v20.4s, v3.4s, v6.4s			 \n"
		"	fmla	v21.4s, v4.4s, v6.4s			 \n"

		"	ldr		q9, [x11], #16					 \n"

		"	fmla	v22.4s, v0.4s, v7.4s			 \n"
		"	fmla	v23.4s, v1.4s, v7.4s		 	 \n"

		"	ldr		q10, [x12], #16					 \n"

		"	fmla	v24.4s, v2.4s, v7.4s			 \n"
		"	fmla	v25.4s, v3.4s, v7.4s			 \n"
		"	fmla	v26.4s, v4.4s, v7.4s			 \n"

		"	ldr		q11, [x13], #16					 \n"

		"	fmla	v27.4s, v0.4s, v8.4s			 \n"
		"	ldr		q5, [x16], #16					 \n"
		"	ldr		q0, [x14], #16					 \n"
		"	fmla	v28.4s, v1.4s, v8.4s		 	 \n"
		"	ldr		q1, [x15], #16					 \n"

		"	fmla	v29.4s, v2.4s, v8.4s			 \n"
		"	fmla	v30.4s, v3.4s, v8.4s			 \n"
		"	fmla	v31.4s, v4.4s, v8.4s			 \n"

		".endm   									 \n"


		".macro N4M5_KERNEL5x4_K1                    \n"

		"	prfm 	PLDL1KEEP, [x18, #128]			 \n"

		"	ldr		q6, [x17], #16					 \n"
		"	fmla	v12.4s, v9.4s, v5.4s 			 \n"
		"	fmla	v13.4s, v10.4s, v5.4s		  	 \n"

		"	prfm  	PLDL1KEEP, [x19, #128]			 \n"
		"	ldr		q7, [x18], #16					 \n"
		"	fmla	v14.4s, v11.4s, v5.4s		 	 \n"
		"	fmla	v15.4s, v0.4s, v5.4s			 \n"
		"	fmla	v16.4s, v1.4s, v5.4s			 \n"

		"	prfm 	PLDL1KEEP, [x13, #64]			 \n"
		"	ldr		q8, [x19], #16					 \n"

		"	fmla	v17.4s, v9.4s, v6.4s 			 \n"
		"	fmla	v18.4s, v10.4s, v6.4s		  	 \n"

		"	ldr		q2, [x13], #16					 \n"

		"	prfm  	PLDL1KEEP, [x14, #64]			 \n"
		"	fmla	v19.4s, v11.4s, v6.4s		 	 \n"
		"	fmla	v20.4s, v0.4s, v6.4s			 \n"
		"	fmla	v21.4s, v1.4s, v6.4s			 \n"

		"	prfm  	PLDL1KEEP, [x15, #64]			 \n"
		"	ldr		q3, [x14], #16					 \n"

		"	fmla	v25.4s, v0.4s, v7.4s			 \n"
		"	fmla	v26.4s, v1.4s, v7.4s			 \n"

		"	fmla	v30.4s, v0.4s, v8.4s			 \n"
		"	ldr		q0, [x11], #16					 \n"
		"	fmla	v31.4s, v1.4s, v8.4s			 \n"
		"	ldr		q1, [x12], #16					 \n"

		"	fmla	v22.4s, v9.4s, v7.4s		  	 \n"

		"	ldr		q5, [x16], #16					 \n"

		"	fmla	v23.4s, v10.4s, v7.4s			 \n"
		"	fmla	v24.4s, v11.4s, v7.4s			 \n"

		"	ldr		q4, [x15], #16					 \n"

		"	fmla	v27.4s, v9.4s, v8.4s			 \n"
		"	fmla	v28.4s, v10.4s, v8.4s			 \n"
		"	fmla	v29.4s, v11.4s, v8.4s			 \n"

		".endm									 	 \n"


		".macro N4M5_KERNEL5x4_END_K                 \n"

		"	ldr		q6, [x17], #16					 \n"
		"	fmla	v12.4s, v9.4s, v5.4s 			 \n"
		"	fmla	v13.4s, v10.4s, v5.4s		  	 \n"

		"	ldr		q7, [x18], #16					 \n"
		"	fmla	v14.4s, v11.4s, v5.4s		 	 \n"
		"	fmla	v15.4s, v0.4s, v5.4s			 \n"
		"	fmla	v16.4s, v1.4s, v5.4s			 \n"

		"	ldr		q8, [x19], #16					 \n"

		"	fmla	v17.4s, v9.4s, v6.4s 			 \n"
		"	faddp	v12.4s, v12.4s, v17.4s			 \n"
		"	fmla	v18.4s, v10.4s, v6.4s		  	 \n"
		"	faddp	v13.4s, v13.4s, v18.4s			 \n"

		"	fmla	v19.4s, v11.4s, v6.4s		 	 \n"
		"	faddp	v14.4s, v14.4s, v19.4s			 \n"
		"	fmla	v20.4s, v0.4s, v6.4s			 \n"
		"	faddp	v15.4s, v15.4s, v20.4s			 \n"
		"	fmla	v21.4s, v1.4s, v6.4s			 \n"
		"	faddp	v16.4s, v16.4s, v21.4s			 \n"

		"	fmla	v25.4s, v0.4s, v7.4s			 \n"
		"	fmla	v26.4s, v1.4s, v7.4s			 \n"

		"	fmla	v30.4s, v0.4s, v8.4s			 \n"
		"	faddp	v25.4s, v25.4s, v30.4s			 \n"
		"	fmla	v31.4s, v1.4s, v8.4s			 \n"
		"	faddp	v26.4s, v26.4s, v31.4s			 \n"

		"	fmla	v22.4s, v9.4s, v7.4s		  	 \n"
		"	fmla	v23.4s, v10.4s, v7.4s			 \n"
		"	fmla	v24.4s, v11.4s, v7.4s			 \n"

		"	faddp	v15.4s, v15.4s, v25.4s			 \n"
		"	faddp 	v16.4s, v16.4s, v26.4s			 \n"

		"	fmla	v27.4s, v9.4s, v8.4s			 \n"
		"	faddp 	v22.4s, v22.4s, v27.4s			 \n"
		"	fmla	v28.4s, v10.4s, v8.4s			 \n"
		"	faddp	v23.4s, v23.4s, v28.4s			 \n"
		"	fmla	v29.4s, v11.4s, v8.4s			 \n"
		"	faddp	v24.4s, v24.4s, v29.4s			 \n"

		"	faddp	v12.4s, v12.4s, v22.4s			 \n"
		"	faddp	v13.4s, v13.4s, v23.4s		 	 \n"
		"	faddp	v14.4s, v14.4s, v24.4s			 \n"

		".endm 										 \n"




		".macro	N4M5_SAVE5x4 						 \n"

		"	subs	x21, x21, #1					 \n"

		"	str		q12, [x25]					 	 \n"
		"	add		x25, x29, x9, lsl #2			 \n"
		"	str		q13, [x26]					 	 \n"
		"	add		x26, x29, x9, lsl #3			 \n"

		"	add 	x10, x10, x6, lsl #4			 \n"
		"	add		x10, x10, x6, lsl #2 			 \n"

		"	str		q14, [x27]		 			 	 \n"
		"	add		x27, x25, x9, lsl #3 			 \n"
		"	str		q15, [x28]		 			 	 \n"
		"	add		x28, x26, x9, lsl #3 			 \n"
		"	str		q16, [x29]					 	 \n"
		"	add		x29, x27, x9, lsl #3			 \n"

		".endm 										 \n"



		".macro	N4_KERNEL4x4_BEGIN_K				 \n"


		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q1, [x12], #16				     \n"
		"	ldr		q5, [x16], #16					 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x14, #64] 			 \n"

		"	prfm 	PLDL1KEEP, [x17, #128]			 \n"

		"	fmul	v12.4s, v0.4s, v5.4s			 \n"
		"	ldr		q2, [x13], #16					 \n"
		"	fmul	v13.4s, v1.4s, v5.4s			 \n"

		"	ldr		q3, [x14], #16					 \n"

		"	fmul	v14.4s, v2.4s, v5.4s		 	 \n"
		"	fmul	v15.4s, v3.4s, v5.4s			 \n"

		"	ldr		q6, [x17], #16					 \n"
		"	ldr		q7, [x18], #16					 \n"
		"	fmul	v17.4s, v0.4s, v6.4s			 \n"

		"	ldr		q8, [x19], #16					 \n"
		"	ldr		q9, [x11], #16					 \n"

		"	fmul	v18.4s, v1.4s, v6.4s			 \n"
		"	fmul	v19.4s, v2.4s, v6.4s			 \n"
		"	fmul	v20.4s, v3.4s, v6.4s			 \n"

		"	ldr		q10, [x12], #16					 \n"

		"	fmul	v22.4s, v0.4s, v7.4s			 \n"
		"	fmul	v23.4s, v1.4s, v7.4s			 \n"
		"	fmul	v24.4s, v2.4s, v7.4s			 \n"
		"	fmul	v25.4s, v3.4s, v7.4s			 \n"

		"	ldr		q11, [x13], #16					 \n"

		"	fmul	v27.4s, v0.4s, v8.4s			 \n"
		"	ldr		q16, [x14], #16					 \n"
		"	fmul	v28.4s, v1.4s, v8.4s			 \n"

		"	ldr		q5, [x16], #16					 \n"

		"	fmul	v29.4s, v2.4s, v8.4s			 \n"
		"	fmul	v30.4s, v3.4s, v8.4s			 \n"


		".endm 										 \n"



		".macro	N4_KERNEL4x4_K0						 \n"


		"	prfm 	PLDL1KEEP, [x16, #128]			 \n"

		"	ldr		q6, [x17], #16					 \n"

		"	fmla	v12.4s, v0.4s, v5.4s			 \n"
		"	fmla	v13.4s, v1.4s, v5.4s			 \n"

		"	prfm 	PLDL1KEEP, [x17, #128]			 \n"

		"	ldr		q7, [x18], #16					 \n"

		"	prfm 	PLDL1KEEP, [x11, #64]			 \n"

		"	fmla	v14.4s, v2.4s, v5.4s			 \n"
		"	fmla	v15.4s, v3.4s, v5.4s			 \n"

		"	prfm 	PLDL1KEEP, [x12, #64]			 \n"

		"	ldr		q8, [x19], #16					 \n"

		"	fmla	v17.4s, v0.4s, v6.4s			 \n"
		"	fmla	v18.4s, v1.4s, v6.4s		 	 \n"
		"	fmla	v19.4s, v2.4s, v6.4s			 \n"
		"	fmla	v20.4s, v3.4s, v6.4s			 \n"

		"	ldr		q9, [x11], #16					 \n"

		"	fmla	v22.4s, v0.4s, v7.4s			 \n"
		"	fmla	v23.4s, v1.4s, v7.4s		 	 \n"

		"	ldr		q10, [x12], #16					 \n"

		"	fmla	v24.4s, v2.4s, v7.4s			 \n"
		"	fmla	v25.4s, v3.4s, v7.4s			 \n"

		"	ldr		q11, [x13], #16					 \n"

		"	fmla	v27.4s, v0.4s, v8.4s			 \n"
		"	ldr		q5, [x16], #16					 \n"
		"	ldr		q16, [x14], #16					 \n"
		"	fmla	v28.4s, v1.4s, v8.4s		 	 \n"

		"	fmla	v29.4s, v2.4s, v8.4s			 \n"
		"	fmla	v30.4s, v3.4s, v8.4s			 \n"


		".endm 										 \n"




		".macro	N4_KERNEL4x4_K1						 \n"

		"	prfm 	PLDL1KEEP, [x18, #128]			 \n"

		"	ldr		q6, [x17], #16					 \n"
		"	fmla	v12.4s, v9.4s, v5.4s 			 \n"
		"	fmla	v13.4s, v10.4s, v5.4s		  	 \n"

		"	prfm  	PLDL1KEEP, [x19, #128]			 \n"
		"	ldr		q7, [x18], #16					 \n"
		"	fmla	v14.4s, v11.4s, v5.4s		 	 \n"
		"	fmla	v15.4s, v16.4s, v5.4s			 \n"

		"	prfm 	PLDL1KEEP, [x13, #64]			 \n"
		"	ldr		q8, [x19], #16					 \n"

		"	fmla	v17.4s, v9.4s, v6.4s 			 \n"
		"	fmla	v18.4s, v10.4s, v6.4s		  	 \n"

		"	ldr		q2, [x13], #16					 \n"

		"	prfm  	PLDL1KEEP, [x14, #64]			 \n"
		"	fmla	v19.4s, v11.4s, v6.4s		 	 \n"
		"	fmla	v20.4s, v16.4s, v6.4s			 \n"

		"	ldr		q3, [x14], #16					 \n"

		"	fmla	v25.4s, v16.4s, v7.4s			 \n"

		"	fmla	v30.4s, v16.4s, v8.4s			 \n"
		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q1, [x12], #16					 \n"

		"	fmla	v22.4s, v9.4s, v7.4s		  	 \n"

		"	ldr		q5, [x16], #16					 \n"

		"	fmla	v23.4s, v10.4s, v7.4s			 \n"
		"	fmla	v24.4s, v11.4s, v7.4s			 \n"

		"	fmla	v27.4s, v9.4s, v8.4s			 \n"
		"	fmla	v28.4s, v10.4s, v8.4s			 \n"
		"	fmla	v29.4s, v11.4s, v8.4s			 \n"

		".endm									 	 \n"


		".macro	N4_KERNEL4x4_END_K                 	 \n"

		"	ldr		q6, [x17], #16					 \n"
		"	fmla	v12.4s, v9.4s, v5.4s 			 \n"
		"	fmla	v13.4s, v10.4s, v5.4s		  	 \n"

		"	ldr		q7, [x18], #16					 \n"
		"	fmla	v14.4s, v11.4s, v5.4s		 	 \n"
		"	fmla	v15.4s, v16.4s, v5.4s			 \n"

		"	ldr		q8, [x19], #16					 \n"

		"	fmla	v17.4s, v9.4s, v6.4s 			 \n"
		"	faddp	v12.4s, v12.4s, v17.4s			 \n"
		"	fmla	v18.4s, v10.4s, v6.4s		  	 \n"
		"	faddp	v13.4s, v13.4s, v18.4s			 \n"

		"	fmla	v19.4s, v11.4s, v6.4s		 	 \n"
		"	faddp	v14.4s, v14.4s, v19.4s			 \n"
		"	fmla	v20.4s, v16.4s, v6.4s			 \n"
		"	faddp	v15.4s, v15.4s, v20.4s			 \n"

		"	fmla	v25.4s, v16.4s, v7.4s			 \n"

		"	fmla	v30.4s, v16.4s, v8.4s			 \n"
		"	faddp	v25.4s, v25.4s, v30.4s			 \n"

		"	fmla	v22.4s, v9.4s, v7.4s		  	 \n"
		"	fmla	v23.4s, v10.4s, v7.4s			 \n"
		"	fmla	v24.4s, v11.4s, v7.4s			 \n"

		"	faddp	v15.4s, v15.4s, v25.4s			 \n"

		"	fmla	v27.4s, v9.4s, v8.4s			 \n"
		"	faddp 	v22.4s, v22.4s, v27.4s			 \n"
		"	fmla	v28.4s, v10.4s, v8.4s			 \n"
		"	faddp	v23.4s, v23.4s, v28.4s			 \n"
		"	fmla	v29.4s, v11.4s, v8.4s			 \n"
		"	faddp	v24.4s, v24.4s, v29.4s			 \n"

		"	faddp	v12.4s, v12.4s, v22.4s			 \n"
		"	faddp	v13.4s, v13.4s, v23.4s		 	 \n"
		"	faddp	v14.4s, v14.4s, v24.4s			 \n"

		".endm 										 \n"


		".macro	N4M4_ADD_C							 \n"

		"	ldr		q0, [x25]						 \n"
		"	ldr		q1, [x26]						 \n"

		"	fadd 	v12.4s, v12.4s, v0.4s			 \n"
		"	ldr		q2, [x27]						 \n"
		"	fadd 	v13.4s, v13.4s, v1.4s			 \n"
		"	ldr		q3, [x28]						 \n"
		"	fadd 	v14.4s, v14.4s, v2.4s			 \n"
		"	fadd 	v15.4s, v15.4s, v3.4s			 \n"

		".endm 										 \n"




		".macro	N4M4_SAVE4x4 						 \n"

		"	str		q12, [x25]					 	 \n"
		"	str		q13, [x26]					 	 \n"

		"	str		q14, [x27]		 			 	 \n"
		"	str		q15, [x28]		 			 	 \n"

		".endm 										 \n"



		".macro	N4_KERNEL3x4_BEGIN_K				 \n"


		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q1, [x12], #16				     \n"
		"	ldr		q5, [x16], #16					 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"

		"	prfm 	PLDL1KEEP, [x17, #128]			 \n"

		"	fmul	v12.4s, v0.4s, v5.4s			 \n"
		"	ldr		q2, [x13], #16					 \n"
		"	fmul	v13.4s, v1.4s, v5.4s			 \n"

		"	fmul	v14.4s, v2.4s, v5.4s		 	 \n"

		"	ldr		q6, [x17], #16					 \n"
		"	ldr		q7, [x18], #16					 \n"
		"	fmul	v17.4s, v0.4s, v6.4s			 \n"

		"	ldr		q8, [x19], #16					 \n"
		"	ldr		q9, [x11], #16					 \n"

		"	fmul	v18.4s, v1.4s, v6.4s			 \n"
		"	fmul	v19.4s, v2.4s, v6.4s			 \n"

		"	ldr		q10, [x12], #16					 \n"

		"	fmul	v22.4s, v0.4s, v7.4s			 \n"
		"	fmul	v23.4s, v1.4s, v7.4s			 \n"
		"	fmul	v24.4s, v2.4s, v7.4s			 \n"

		"	ldr		q11, [x13], #16					 \n"

		"	fmul	v27.4s, v0.4s, v8.4s			 \n"
		"	fmul	v28.4s, v1.4s, v8.4s			 \n"

		"	ldr		q5, [x16], #16					 \n"

		"	fmul	v29.4s, v2.4s, v8.4s			 \n"

		".endm 										 \n"



		".macro	N4_KERNEL3x4_K0						 \n"


		"	prfm 	PLDL1KEEP, [x16, #128]			 \n"

		"	ldr		q6, [x17], #16					 \n"

		"	fmla	v12.4s, v0.4s, v5.4s			 \n"
		"	fmla	v13.4s, v1.4s, v5.4s			 \n"

		"	prfm 	PLDL1KEEP, [x17, #128]			 \n"

		"	ldr		q7, [x18], #16					 \n"

		"	prfm 	PLDL1KEEP, [x11, #64]			 \n"

		"	fmla	v14.4s, v2.4s, v5.4s			 \n"

		"	prfm 	PLDL1KEEP, [x12, #64]			 \n"

		"	ldr		q8, [x19], #16					 \n"

		"	fmla	v17.4s, v0.4s, v6.4s			 \n"
		"	fmla	v18.4s, v1.4s, v6.4s		 	 \n"
		"	fmla	v19.4s, v2.4s, v6.4s			 \n"

		"	ldr		q9, [x11], #16					 \n"

		"	fmla	v22.4s, v0.4s, v7.4s			 \n"
		"	fmla	v23.4s, v1.4s, v7.4s		 	 \n"

		"	ldr		q10, [x12], #16					 \n"

		"	fmla	v24.4s, v2.4s, v7.4s			 \n"

		"	ldr		q11, [x13], #16					 \n"

		"	fmla	v27.4s, v0.4s, v8.4s			 \n"
		"	ldr		q5, [x16], #16					 \n"
		"	fmla	v28.4s, v1.4s, v8.4s		 	 \n"

		"	fmla	v29.4s, v2.4s, v8.4s			 \n"

		".endm 										 \n"




		".macro	N4_KERNEL3x4_K1						 \n"

		"	prfm 	PLDL1KEEP, [x18, #128]			 \n"

		"	ldr		q6, [x17], #16					 \n"
		"	fmla	v12.4s, v9.4s, v5.4s 			 \n"
		"	fmla	v13.4s, v10.4s, v5.4s		  	 \n"

		"	prfm  	PLDL1KEEP, [x19, #128]			 \n"
		"	ldr		q7, [x18], #16					 \n"
		"	fmla	v14.4s, v11.4s, v5.4s		 	 \n"

		"	prfm 	PLDL1KEEP, [x13, #64]			 \n"
		"	ldr		q8, [x19], #16					 \n"

		"	fmla	v17.4s, v9.4s, v6.4s 			 \n"
		"	fmla	v18.4s, v10.4s, v6.4s		  	 \n"

		"	ldr		q2, [x13], #16					 \n"

		"	fmla	v19.4s, v11.4s, v6.4s		 	 \n"

		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q1, [x12], #16					 \n"

		"	fmla	v22.4s, v9.4s, v7.4s		  	 \n"

		"	ldr		q5, [x16], #16					 \n"

		"	fmla	v23.4s, v10.4s, v7.4s			 \n"
		"	fmla	v24.4s, v11.4s, v7.4s			 \n"

		"	fmla	v27.4s, v9.4s, v8.4s			 \n"
		"	fmla	v28.4s, v10.4s, v8.4s			 \n"
		"	fmla	v29.4s, v11.4s, v8.4s			 \n"

		".endm									 	 \n"


		".macro	N4_KERNEL3x4_END_K                 	 \n"

		"	ldr		q6, [x17], #16					 \n"
		"	fmla	v12.4s, v9.4s, v5.4s 			 \n"
		"	fmla	v13.4s, v10.4s, v5.4s		  	 \n"

		"	ldr		q7, [x18], #16					 \n"
		"	fmla	v14.4s, v11.4s, v5.4s		 	 \n"

		"	ldr		q8, [x19], #16					 \n"

		"	fmla	v17.4s, v9.4s, v6.4s 			 \n"
		"	faddp	v12.4s, v12.4s, v17.4s			 \n"
		"	fmla	v18.4s, v10.4s, v6.4s		  	 \n"
		"	faddp	v13.4s, v13.4s, v18.4s			 \n"

		"	fmla	v19.4s, v11.4s, v6.4s		 	 \n"
		"	faddp	v14.4s, v14.4s, v19.4s			 \n"

		"	fmla	v22.4s, v9.4s, v7.4s		  	 \n"
		"	fmla	v23.4s, v10.4s, v7.4s			 \n"
		"	fmla	v24.4s, v11.4s, v7.4s			 \n"

		"	fmla	v27.4s, v9.4s, v8.4s			 \n"
		"	faddp 	v22.4s, v22.4s, v27.4s			 \n"
		"	fmla	v28.4s, v10.4s, v8.4s			 \n"
		"	faddp	v23.4s, v23.4s, v28.4s			 \n"
		"	fmla	v29.4s, v11.4s, v8.4s			 \n"
		"	faddp	v24.4s, v24.4s, v29.4s			 \n"

		"	faddp	v12.4s, v12.4s, v22.4s			 \n"
		"	faddp	v13.4s, v13.4s, v23.4s		 	 \n"
		"	faddp	v14.4s, v14.4s, v24.4s			 \n"

		".endm 										 \n"


		".macro	N4M3_ADD_C							 \n"

		"	ldr		q0, [x25]						 \n"
		"	ldr		q1, [x26]						 \n"

		"	fadd 	v12.4s, v12.4s, v0.4s			 \n"
		"	ldr		q2, [x27]						 \n"
		"	fadd 	v13.4s, v13.4s, v1.4s			 \n"
		"	fadd 	v14.4s, v14.4s, v2.4s			 \n"

		".endm 										 \n"


		".macro	N4M3_SAVE3x4 						 \n"

		"	str		q12, [x25]					 	 \n"
		"	str		q13, [x26]					 	 \n"

		"	str		q14, [x27]		 			 	 \n"

		".endm 										 \n"




		".macro	N4_KERNEL2x4_BEGIN_K				 \n"


		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q1, [x12], #16				     \n"
		"	ldr		q5, [x16], #16					 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"

		"	prfm 	PLDL1KEEP, [x17, #128]			 \n"

		"	fmul	v12.4s, v0.4s, v5.4s			 \n"
		"	fmul	v13.4s, v1.4s, v5.4s			 \n"

		"	ldr		q6, [x17], #16					 \n"
		"	ldr		q7, [x18], #16					 \n"
		"	fmul	v17.4s, v0.4s, v6.4s			 \n"

		"	ldr		q8, [x19], #16					 \n"
		"	ldr		q9, [x11], #16					 \n"

		"	fmul	v18.4s, v1.4s, v6.4s			 \n"

		"	ldr		q10, [x12], #16					 \n"

		"	fmul	v22.4s, v0.4s, v7.4s			 \n"
		"	fmul	v23.4s, v1.4s, v7.4s			 \n"

		"	ldr		q5, [x16], #16					 \n"

		"	fmul	v27.4s, v0.4s, v8.4s			 \n"
		"	fmul	v28.4s, v1.4s, v8.4s			 \n"


		".endm 										 \n"



		".macro	N4_KERNEL2x4_K0						 \n"


		"	prfm 	PLDL1KEEP, [x16, #128]			 \n"

		"	ldr		q6, [x17], #16					 \n"

		"	fmla	v12.4s, v0.4s, v5.4s			 \n"
		"	fmla	v13.4s, v1.4s, v5.4s			 \n"

		"	prfm 	PLDL1KEEP, [x17, #128]			 \n"

		"	ldr		q7, [x18], #16					 \n"

		"	prfm 	PLDL1KEEP, [x11, #64]			 \n"

		"	prfm 	PLDL1KEEP, [x12, #64]			 \n"

		"	ldr		q8, [x19], #16					 \n"

		"	fmla	v17.4s, v0.4s, v6.4s			 \n"
		"	fmla	v18.4s, v1.4s, v6.4s		 	 \n"

		"	ldr		q9, [x11], #16					 \n"

		"	fmla	v22.4s, v0.4s, v7.4s			 \n"
		"	fmla	v23.4s, v1.4s, v7.4s		 	 \n"

		"	ldr		q10, [x12], #16					 \n"

		"	fmla	v27.4s, v0.4s, v8.4s			 \n"
		"	ldr		q5, [x16], #16					 \n"
		"	fmla	v28.4s, v1.4s, v8.4s		 	 \n"


		".endm 										 \n"




		".macro	N4_KERNEL2x4_K1						 \n"

		"	prfm 	PLDL1KEEP, [x18, #128]			 \n"

		"	ldr		q6, [x17], #16					 \n"
		"	fmla	v12.4s, v9.4s, v5.4s 			 \n"
		"	fmla	v13.4s, v10.4s, v5.4s		  	 \n"

		"	prfm  	PLDL1KEEP, [x19, #128]			 \n"
		"	ldr		q7, [x18], #16					 \n"

		"	ldr		q8, [x19], #16					 \n"

		"	fmla	v17.4s, v9.4s, v6.4s 			 \n"
		"	fmla	v18.4s, v10.4s, v6.4s		  	 \n"

		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q1, [x12], #16					 \n"

		"	fmla	v22.4s, v9.4s, v7.4s		  	 \n"

		"	ldr		q5, [x16], #16					 \n"

		"	fmla	v23.4s, v10.4s, v7.4s			 \n"

		"	fmla	v27.4s, v9.4s, v8.4s			 \n"
		"	fmla	v28.4s, v10.4s, v8.4s			 \n"

		".endm									 	 \n"


		".macro	N4_KERNEL2x4_END_K                 	 \n"

		"	ldr		q6, [x17], #16					 \n"
		"	fmla	v12.4s, v9.4s, v5.4s 			 \n"
		"	fmla	v13.4s, v10.4s, v5.4s		  	 \n"

		"	ldr		q7, [x18], #16					 \n"

		"	ldr		q8, [x19], #16					 \n"

		"	fmla	v17.4s, v9.4s, v6.4s 			 \n"
		"	faddp	v12.4s, v12.4s, v17.4s			 \n"
		"	fmla	v18.4s, v10.4s, v6.4s		  	 \n"
		"	faddp	v13.4s, v13.4s, v18.4s			 \n"

		"	fmla	v22.4s, v9.4s, v7.4s		  	 \n"
		"	fmla	v23.4s, v10.4s, v7.4s			 \n"

		"	fmla	v27.4s, v9.4s, v8.4s			 \n"
		"	faddp 	v22.4s, v22.4s, v27.4s			 \n"
		"	fmla	v28.4s, v10.4s, v8.4s			 \n"
		"	faddp	v23.4s, v23.4s, v28.4s			 \n"

		"	faddp	v12.4s, v12.4s, v22.4s			 \n"
		"	faddp	v13.4s, v13.4s, v23.4s		 	 \n"

		".endm 										 \n"


		".macro	N4M2_ADD_C							 \n"

		"	ldr		q0, [x25]						 \n"
		"	ldr		q1, [x26]						 \n"

		"	fadd 	v12.4s, v12.4s, v0.4s			 \n"
		"	fadd 	v13.4s, v13.4s, v1.4s			 \n"

		".endm 										 \n"


		".macro	N4M2_SAVE2x4 						 \n"

		"	str		q12, [x25]					 	 \n"
		"	str		q13, [x26]					 	 \n"

		".endm 										 \n"



		".macro	N4_KERNEL1x4_BEGIN_K				 \n"


		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q5, [x16], #16					 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"

		"	prfm 	PLDL1KEEP, [x17, #128]			 \n"

		"	fmul	v12.4s, v0.4s, v5.4s			 \n"
		"	ldr		q2, [x13], #16					 \n"

		"	ldr		q6, [x17], #16					 \n"
		"	ldr		q7, [x18], #16					 \n"
		"	fmul	v17.4s, v0.4s, v6.4s			 \n"

		"	ldr		q8, [x19], #16					 \n"
		"	ldr		q9, [x11], #16					 \n"

		"	fmul	v22.4s, v0.4s, v7.4s			 \n"

		"	ldr		q5, [x16], #16					 \n"

		"	fmul	v27.4s, v0.4s, v8.4s			 \n"


		".endm 										 \n"



		".macro	N4_KERNEL1x4_K0						 \n"


		"	prfm 	PLDL1KEEP, [x16, #128]			 \n"

		"	ldr		q6, [x17], #16					 \n"

		"	fmla	v12.4s, v0.4s, v5.4s			 \n"

		"	prfm 	PLDL1KEEP, [x17, #128]			 \n"

		"	ldr		q7, [x18], #16					 \n"

		"	prfm 	PLDL1KEEP, [x11, #64]			 \n"


		"	ldr		q8, [x19], #16					 \n"

		"	fmla	v17.4s, v0.4s, v6.4s			 \n"

		"	ldr		q9, [x11], #16					 \n"

		"	fmla	v22.4s, v0.4s, v7.4s			 \n"

		"	fmla	v27.4s, v0.4s, v8.4s			 \n"
		"	ldr		q5, [x16], #16					 \n"


		".endm 										 \n"




		".macro	N4_KERNEL1x4_K1						 \n"

		"	prfm 	PLDL1KEEP, [x18, #128]			 \n"

		"	ldr		q6, [x17], #16					 \n"
		"	fmla	v12.4s, v9.4s, v5.4s 			 \n"

		"	prfm  	PLDL1KEEP, [x19, #128]			 \n"
		"	ldr		q7, [x18], #16					 \n"

		"	prfm 	PLDL1KEEP, [x13, #64]			 \n"
		"	ldr		q8, [x19], #16					 \n"

		"	fmla	v17.4s, v9.4s, v6.4s 			 \n"

		"	ldr		q0, [x11], #16					 \n"

		"	fmla	v22.4s, v9.4s, v7.4s		  	 \n"

		"	ldr		q5, [x16], #16					 \n"

		"	fmla	v27.4s, v9.4s, v8.4s			 \n"

		".endm									 	 \n"


		".macro	N4_KERNEL1x4_END_K                 	 \n"

		"	ldr		q6, [x17], #16					 \n"
		"	fmla	v12.4s, v9.4s, v5.4s 			 \n"

		"	ldr		q7, [x18], #16					 \n"

		"	ldr		q8, [x19], #16					 \n"

		"	fmla	v17.4s, v9.4s, v6.4s 			 \n"
		"	faddp	v12.4s, v12.4s, v17.4s			 \n"

		"	fmla	v22.4s, v9.4s, v7.4s		  	 \n"

		"	fmla	v27.4s, v9.4s, v8.4s			 \n"
		"	faddp 	v22.4s, v22.4s, v27.4s			 \n"

		"	faddp	v12.4s, v12.4s, v22.4s			 \n"

		".endm 										 \n"


		".macro	N4M1_ADD_C							 \n"

		"	ldr		q0, [x25]						 \n"
		"	fadd 	v12.4s, v12.4s, v0.4s			 \n"

		".endm 										 \n"


		".macro	N4M1_SAVE1x4 						 \n"

		"	str		q12, [x25]					 	 \n"

		".endm 										 \n"



		".macro	N2M5_KERNEL5x2_BEGIN_K				 \n"


		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q1, [x12], #16				     \n"
		"	ldr		q5, [x16], #16					 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x14, #64] 			 \n"
		"	prfm	PLDL1KEEP, [x15, #64]			 \n"

		"	fmul	v12.4s, v0.4s, v5.4s			 \n"
		"	ldr		q2, [x13], #16					 \n"
		"	fmul	v13.4s, v1.4s, v5.4s			 \n"

		"	ldr		q3, [x14], #16					 \n"

		"	fmul	v14.4s, v2.4s, v5.4s		 	 \n"
		"	ldr		q4, [x15], #16					 \n"
		"	fmul	v15.4s, v3.4s, v5.4s			 \n"

		"	ldr		q6, [x17], #16					 \n"

		"	fmul	v16.4s, v4.4s, v5.4s			 \n"

		"	ldr		q26, [x16], #16					 \n"

		"	fmul	v17.4s, v0.4s, v6.4s 			 \n"
		"	fmul	v18.4s, v1.4s, v6.4s 			 \n"

		"	ldr		q7, [x11], #16					 \n"

		"	fmul	v19.4s, v2.4s, v6.4s 			 \n"
		"	fmul	v20.4s, v3.4s, v6.4s 			 \n"

		"	ldr		q8, [x12], #16					 \n"

		"	fmul 	v21.4s, v4.4s, v6.4s 			 \n"

		".endm 										 \n"


		".macro	N2M5_KERNEL5x2_K0				 	\n"

		"	ldr		q2, [x13], #16					\n"
		"	fmla	v12.4s, v0.4s, v5.4s			\n"
		"	ldr		q3, [x14], #16					\n"
		"	fmla	v13.4s, v1.4s, v5.4s			\n"
		"	ldr		q4, [x15], #16					\n"
		"	fmla	v14.4s, v2.4s, v5.4s			\n"
		"	ldr 	q6, [x17], #16					\n"
		"	fmla	v15.4s, v3.4s, v5.4s			\n"
		"	fmla	v16.4s, v4.4s, v5.4s			\n"

		"	prfm	PLDL1KEEP, [x16, #64]			\n"

		"	ldr		q7, [x11], #16					\n"
		"	fmla	v17.4s, v0.4s, v6.4s			\n"
		"	ldr		q8, [x12], #16					\n"
		"	fmla	v18.4s, v1.4s, v6.4s			\n"

		"	prfm	PLDL1KEEP, [x17, #64]			\n"
		"	ldr		q26, [x16], #16					\n"

		"	fmla	v19.4s, v2.4s, v6.4s			\n"
		"	fmla	v20.4s, v3.4s, v6.4s			\n"
		"	fmla	v21.4s, v4.4s, v6.4s			\n"

		".endm 										\n"


		".macro	N2M5_KERNEL5x2_K1				 	\n"


		"	ldr		q9, [x13], #16					\n"
		"	fmla	v12.4s, v7.4s, v26.4s			\n"
		"	ldr		q10, [x14], #16					\n"
		"	fmla	v13.4s, v8.4s, v26.4s 			\n"

		"	prfm	PLDL1KEEP, [x11, #64]			\n"

		"	ldr		q11, [x15], #16					\n"
		"	fmla	v14.4s, v9.4s, v26.4s			\n"
		"	ldr		q27, [x17], #16					\n"
		"	fmla	v15.4s, v10.4s, v26.4s 			\n"
		"	fmla	v16.4s, v11.4s, v26.4s 			\n"

		"	prfm	PLDL1KEEP, [x12, #64]			\n"

		"	fmla	v17.4s, v7.4s, v27.4s			\n"

		"	ldr		q0, [x11], #16					\n"
		"	fmla	v18.4s, v8.4s, v27.4s			\n"
		"	ldr		q5, [x16], #16					\n"
		"	fmla	v19.4s, v9.4s, v27.4s			\n"
		"	ldr		q1, [x12], #16					\n"
		"	fmla	v20.4s, v10.4s, v27.4s			\n"
		"	prfm	PLDL1KEEP, [x13, #64]			\n"
		"	fmla	v21.4s, v11.4s, v27.4s			\n"

		".endm 										\n"


		".macro	N2M5_KERNEL5x2_END_K				\n"

		"	ldr		q9, [x13], #16					\n"
		"	fmla	v12.4s, v7.4s, v26.4s			\n"
		"	ldr		q10, [x14], #16					\n"
		"	fmla	v13.4s, v8.4s, v26.4s 			\n"

		"	ldr		q11, [x15], #16					\n"
		"	fmla	v14.4s, v9.4s, v26.4s			\n"
		"	ldr		q27, [x17], #16					\n"
		"	fmla	v15.4s, v10.4s, v26.4s 			\n"
		"	fmla	v16.4s, v11.4s, v26.4s 			\n"

		"	fmla	v17.4s, v7.4s, v27.4s			\n"
		"	fmla	v18.4s, v8.4s, v27.4s			\n"
		"	fmla	v19.4s, v9.4s, v27.4s			\n"
		"	fmla	v20.4s, v10.4s, v27.4s			\n"
		"	fmla	v21.4s, v11.4s, v27.4s			\n"


		"	faddp 	v12.4s, v12.4s, v17.4s			\n"
		"	faddp 	v13.4s, v13.4s, v18.4s			\n"
		"	faddp 	v14.4s, v14.4s, v19.4s			\n"
		"	faddp 	v15.4s, v15.4s, v20.4s			\n"
		"	faddp 	v16.4s, v16.4s, v21.4s			\n"

		"	faddp 	v12.4s, v12.4s, v0.4s			\n"
		"	faddp	v13.4s, v13.4s, v0.4s			\n"
		"	faddp 	v14.4s, v14.4s, v0.4s			\n"
		"	faddp	v15.4s, v15.4s, v0.4s			\n"
		"	faddp	v16.4s, v16.4s, v0.4s			\n"


		".endm 										 \n"


		".macro	N2M5_ADD_C							 \n"

		"	ld1		{v0.2s}, [x25]					 \n"
		"	ld1		{v1.2s}, [x26]					 \n"

		"	fadd 	v12.2s, v12.2s, v0.2s			 \n"
		"	ld1		{v2.2s}, [x27]					 \n"
		"	fadd 	v13.2s, v13.2s, v1.2s			 \n"
		"	ld1		{v3.2s}, [x28]					 \n"
		"	fadd 	v14.2s, v14.2s, v2.2s			 \n"
		"	ld1		{v4.2s}, [x29]					 \n"
		"	fadd 	v15.2s, v15.2s, v3.2s			 \n"
		"	fadd 	v16.2s, v16.2s, v4.2s			 \n"

		".endm 										 \n"



		".macro	N2M5_SAVE5x2 						 \n"

		"	subs	x21, x21, #1					 \n"

		"	st1		{v12.2s}, [x25]				 	 \n"
		"	add		x25, x29, x9, lsl #2			 \n"
		"	st1		{v13.2s}, [x26]				 	 \n"
		"	add		x26, x29, x9, lsl #3			 \n"

		"	add 	x10, x10, x6, lsl #4			 \n"
		"	add		x10, x10, x6, lsl #2 			 \n"

		"	st1		{v14.2s}, [x27]		 		 	 \n"
		"	add		x27, x25, x9, lsl #3 			 \n"
		"	st1		{v15.2s}, [x28]		 		 	 \n"
		"	add		x28, x26, x9, lsl #3 			 \n"
		"	st1		{v16.2s}, [x29]		 		 	 \n"
		"	add		x29, x27, x9, lsl #3			 \n"

		".endm 										 \n"





		".macro	N2_KERNEL3x2_BEGIN_K				 \n"


		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q1, [x12], #16				     \n"
		"	ldr		q5, [x16], #16					 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"

		"	fmul	v12.4s, v0.4s, v5.4s			 \n"
		"	ldr		q2, [x13], #16					 \n"
		"	fmul	v13.4s, v1.4s, v5.4s			 \n"
		"	ldr		q6, [x17], #16					 \n"
		"	fmul	v14.4s, v2.4s, v5.4s		 	 \n"


		
		"	ldr		q26, [x16], #16					 \n"
		"	fmul	v17.4s, v0.4s, v6.4s 			 \n"
		"	fmul	v18.4s, v1.4s, v6.4s 			 \n"

		"	ldr		q7, [x11], #16					 \n"
		"	fmul	v19.4s, v2.4s, v6.4s 			 \n"
		"	ldr		q8, [x12], #16					 \n"


		".endm 										 \n"


		".macro	N2_KERNEL3x2_K0				 		\n"

		"	ldr		q2, [x13], #16					\n"
		"	fmla	v12.4s, v0.4s, v5.4s			\n"
		"	ldr		q6, [x17], #16					\n"
		"	fmla	v13.4s, v1.4s, v5.4s			\n"
		"	ldr		q7, [x11], #16					\n"
		"	fmla	v14.4s, v2.4s, v5.4s			\n"

		"	prfm	PLDL1KEEP, [x16, #64]			\n"

		"	fmla	v17.4s, v0.4s, v6.4s			\n"
		"	ldr		q8, [x12], #16					\n"
		"	fmla	v18.4s, v1.4s, v6.4s			\n"

		"	prfm	PLDL1KEEP, [x17, #64]			\n"
		"	ldr		q26, [x16], #16					\n"

		"	fmla	v19.4s, v2.4s, v6.4s			\n"

		".endm 										\n"


		".macro	N2_KERNEL3x2_K1				 		\n"


		"	ldr		q9, [x13], #16					\n"
		"	fmla	v12.4s, v7.4s, v26.4s			\n"
		"	ldr		q27, [x17], #16					\n"
		"	fmla	v13.4s, v8.4s, v26.4s 			\n"

		"	prfm	PLDL1KEEP, [x11, #64]			\n"
		"	fmla	v14.4s, v9.4s, v26.4s			\n"

		"	prfm	PLDL1KEEP, [x12, #64]			\n"

		"	fmla	v17.4s, v7.4s, v27.4s			\n"

		"	ldr		q0, [x11], #16					\n"
		"	fmla	v18.4s, v8.4s, v27.4s			\n"
		"	ldr		q5, [x16], #16					\n"
		"	fmla	v19.4s, v9.4s, v27.4s			\n"
		"	ldr		q1, [x12], #16					\n"
		"	prfm	PLDL1KEEP, [x13, #64]			\n"

		".endm 										\n"


		".macro	N2_KERNEL3x2_END_K					\n"

		"	ldr		q9, [x13], #16					\n"
		"	fmla	v12.4s, v7.4s, v26.4s			\n"
		"	fmla	v13.4s, v8.4s, v26.4s 			\n"
		"	ldr		q27, [x17], #16					\n"
		"	fmla	v14.4s, v9.4s, v26.4s			\n"
		

		"	fmla	v17.4s, v7.4s, v27.4s			\n"
		"	fmla	v18.4s, v8.4s, v27.4s			\n"
		"	fmla	v19.4s, v9.4s, v27.4s			\n"


		"	faddp 	v12.4s, v12.4s, v17.4s			\n"
		"	faddp 	v13.4s, v13.4s, v18.4s			\n"
		"	faddp 	v14.4s, v14.4s, v19.4s			\n"

		"	faddp 	v12.4s, v12.4s, v0.4s			\n"
		"	faddp	v13.4s, v13.4s, v0.4s			\n"
		"	faddp 	v14.4s, v14.4s, v0.4s			\n"


		".endm 										 \n"


		".macro	N2M3_ADD_C							 \n"

		"	ld1		{v0.2s}, [x25]					 \n"
		"	ld1		{v1.2s}, [x26]					 \n"

		"	fadd 	v12.2s, v12.2s, v0.2s			 \n"
		"	ld1		{v2.2s}, [x27]					 \n"
		"	fadd 	v13.2s, v13.2s, v1.2s			 \n"
		"	fadd 	v14.2s, v14.2s, v2.2s			 \n"

		".endm 										 \n"



		".macro	N2M3_SAVE3x2 						 \n"


		"	st1		{v12.2s}, [x25]				 	 \n"
		"	st1		{v13.2s}, [x26]				 	 \n"
		"	st1		{v14.2s}, [x27]		 		 	 \n"

		".endm 										 \n"



		".macro	N1M5_KERNEL5x1_BEGIN_K				 \n"


		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q1, [x12], #16				     \n"
		"	ldr		q5, [x16], #16					 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x14, #64] 			 \n"
		"	prfm	PLDL1KEEP, [x15, #64]			 \n"

		"	fmul	v12.4s, v0.4s, v5.4s			 \n"
		"	ldr		q2, [x13], #16					 \n"
		"	fmul	v13.4s, v1.4s, v5.4s			 \n"

		"	ldr		q3, [x14], #16					 \n"

		"	fmul	v14.4s, v2.4s, v5.4s		 	 \n"
		"	ldr		q4, [x15], #16					 \n"
		"	fmul	v15.4s, v3.4s, v5.4s			 \n"

		"	ldr		q6, [x11], #16					 \n"
		"	ldr		q11, [x16], #16					 \n"

		"	fmul	v16.4s, v4.4s, v5.4s			 \n"

		".endm 										 \n"


		".macro	N1M5_KERNEL5x1_K0				 	\n"

		"	ldr		q1, [x12], #16					\n"
		"	ldr		q2, [x13], #16					\n"
		"	fmla	v12.4s, v0.4s, v5.4s			\n"
		"	ldr		q3, [x14], #16					\n"
		"	fmla	v13.4s,	v1.4s, v5.4s			\n"
		"	ldr		q4, [x15], #16					\n"

		"	prfm	PLDL1KEEP, [x14, #64]			\n"

		"	fmla	v14.4s,	v2.4s, v5.4s			\n"

		"	ldr		q6, [x11], #16					\n"

		"	prfm	PLDL1KEEP, [x15, #64]			\n"

		"	fmla	v15.4s,	v3.4s, v5.4s			\n"

		"	prfm	PLDL1KEEP, [x16, #64]			\n"

		"	ldr		q11, [x16], #16					\n"
		"	fmla	v16.4s,	v4.4s, v5.4s			\n"

		".endm 										\n"


		".macro	N1M5_KERNEL5x1_K1				 	\n"

		"	ldr		q7, [x12], #16					\n"
		"	ldr		q8, [x13], #16					\n"
		"	fmla	v12.4s, v6.4s, v11.4s			\n"
		"	ldr		q9, [x14], #16					\n"
		"	fmla	v13.4s,	v7.4s, v11.4s			\n"
		"	ldr		q10, [x15], #16					\n"

		"	prfm	PLDL1KEEP, [x11, #64]			\n"

		"	fmla	v14.4s,	v8.4s, v11.4s			\n"

		"	prfm	PLDL1KEEP, [x12, #64]			\n"

		"	ldr		q0, [x11], #16					\n"

		"	prfm	PLDL1KEEP, [x13, #64]			\n"

		"	fmla	v15.4s,	v9.4s, v11.4s			\n"
		"	ldr		q5, [x16], #16					\n"
		"	fmla	v16.4s,	v10.4s, v11.4s			\n"

		".endm 										\n"


		".macro	N1M5_KERNEL5x1_END_K				\n"

		"	ldr		q7, [x12], #16					\n"
		"	ldr		q8, [x13], #16					\n"
		"	fmla	v12.4s, v6.4s, v11.4s			\n"
		"	ldr		q9, [x14], #16					\n"
		"	fmla	v13.4s,	v7.4s, v11.4s			\n"
		"	ldr		q10, [x15], #16					\n"

		"	faddp	v12.4s, v12.4s, v13.4s 			\n"

		"	fmla	v14.4s,	v8.4s, v11.4s			\n"
		"	fmla	v15.4s,	v9.4s, v11.4s			\n"

		"	faddp	v14.4s, v14.4s, v15.4s			\n"
		"	fmla	v16.4s,	v10.4s, v11.4s			\n"
		"	faddp	v12.4s, v12.4s, v14.4s			\n"

		"	faddp	s13, v16.2s						\n"

		"	ins		v17.s[0], v16.s[2]				\n"
		"	ins		v17.s[1], v16.s[3]				\n"

		"	faddp	s14, v17.2s						\n"

		"	fadd 	s15, s13, s14 					\n"


		".endm 										 \n"


		".macro	N1M5_ADD_C							 \n"


		"	ld1		{v0.s}[0], [x25]				 \n"
		"	ld1		{v0.s}[1], [x26]				 \n"
		"	ld1		{v0.s}[2], [x27]				 \n"
		"	ld1		{v0.s}[3], [x28]				 \n"

		"	ldr		s16, [x29]						 \n"

		"	fadd 	v12.4s, v12.4s, v0.4s			 \n"
		"	fadd 	s15, s15, s16 					 \n"

		".endm 										 \n"



		".macro	N1M5_SAVE5x1 						 \n"

		"	subs	x21, x21, #1					 \n"

		"	st1		{v12.s}[0], [x25]				 \n"
		"	add		x25, x29, x9, lsl #2			 \n"
		"	st1		{v12.s}[1], [x26]				 \n"
		"	add		x26, x29, x9, lsl #3			 \n"

		"	add 	x10, x10, x6, lsl #4			 \n"
		"	add		x10, x10, x6, lsl #2 			 \n"

		"	st1		{v12.s}[2], [x27]		 		 \n"
		"	add		x27, x25, x9, lsl #3 			 \n"
		"	st1		{v12.s}[3], [x28]		 		 \n"
		"	add		x28, x26, x9, lsl #3 			 \n"
		"	str		s15, [x29]					 	 \n"
		"	add		x29, x27, x9, lsl #3			 \n"

		".endm 										 \n"



		".macro	N1M4_KERNEL4x1_BEGIN_K				 \n"


		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q1, [x12], #16				     \n"
		"	ldr		q5, [x16], #16					 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x14, #64] 			 \n"

		"	fmul	v12.4s, v0.4s, v5.4s			 \n"
		"	ldr		q2, [x13], #16					 \n"
		"	fmul	v13.4s, v1.4s, v5.4s			 \n"

		"	ldr		q3, [x14], #16					 \n"

		"	fmul	v14.4s, v2.4s, v5.4s		 	 \n"
		"	fmul	v15.4s, v3.4s, v5.4s			 \n"

		"	ldr		q6, [x11], #16					 \n"
		"	ldr		q11, [x16], #16					 \n"

		".endm 										 \n"


		".macro	N1M4_KERNEL4x1_K0				 	\n"

		"	ldr		q1, [x12], #16					\n"
		"	ldr		q2, [x13], #16					\n"
		"	fmla	v12.4s, v0.4s, v5.4s			\n"
		"	ldr		q3, [x14], #16					\n"
		"	fmla	v13.4s,	v1.4s, v5.4s			\n"

		"	prfm	PLDL1KEEP, [x14, #64]			\n"

		"	fmla	v14.4s,	v2.4s, v5.4s			\n"

		"	ldr		q6, [x11], #16					\n"

		"	fmla	v15.4s,	v3.4s, v5.4s			\n"

		"	prfm	PLDL1KEEP, [x16, #64]			\n"

		"	ldr		q11, [x16], #16					\n"

		".endm 										\n"


		".macro	N1M4_KERNEL4x1_K1				 	\n"

		"	ldr		q7, [x12], #16					\n"
		"	ldr		q8, [x13], #16					\n"
		"	fmla	v12.4s, v6.4s, v11.4s			\n"
		"	ldr		q9, [x14], #16					\n"
		"	fmla	v13.4s,	v7.4s, v11.4s			\n"

		"	prfm	PLDL1KEEP, [x11, #64]			\n"

		"	fmla	v14.4s,	v8.4s, v11.4s			\n"

		"	prfm	PLDL1KEEP, [x12, #64]			\n"

		"	ldr		q0, [x11], #16					\n"

		"	prfm	PLDL1KEEP, [x13, #64]			\n"

		"	fmla	v15.4s,	v9.4s, v11.4s			\n"
		"	ldr		q5, [x16], #16					\n"

		".endm 										\n"


		".macro	N1M4_KERNEL4x1_END_K				\n"

		"	ldr		q7, [x12], #16					\n"
		"	ldr		q8, [x13], #16					\n"
		"	fmla	v12.4s, v6.4s, v11.4s			\n"
		"	ldr		q9, [x14], #16					\n"
		"	fmla	v13.4s,	v7.4s, v11.4s			\n"

		"	faddp	v12.4s, v12.4s, v13.4s 			\n"

		"	fmla	v14.4s,	v8.4s, v11.4s			\n"
		"	fmla	v15.4s,	v9.4s, v11.4s			\n"

		"	faddp	v14.4s, v14.4s, v15.4s			\n"
		"	faddp	v12.4s, v12.4s, v14.4s			\n"

		".endm 										\n"


		".macro	N1M4_ADD_C							 \n"


		"	ld1		{v0.s}[0], [x25]				 \n"
		"	ld1		{v0.s}[1], [x26]				 \n"
		"	ld1		{v0.s}[2], [x27]				 \n"
		"	ld1		{v0.s}[3], [x28]				 \n"

		"	fadd 	v12.4s, v12.4s, v0.4s			 \n"

		".endm 										 \n"



		".macro	N1M4_SAVE4x1 						 \n"

		"	st1		{v12.s}[0], [x25]				 \n"
		"	st1		{v12.s}[1], [x26]				 \n"
		"	st1		{v12.s}[2], [x27]		 		 \n"
		"	st1		{v12.s}[3], [x28]		 		 \n"

		".endm 										 \n"




		".macro	N1M3_KERNEL3x1_BEGIN_K				 \n"


		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q1, [x12], #16				     \n"
		"	ldr		q5, [x16], #16					 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"

		"	fmul	v12.4s, v0.4s, v5.4s			 \n"
		"	ldr		q2, [x13], #16					 \n"
		"	fmul	v13.4s, v1.4s, v5.4s			 \n"

		"	fmul	v14.4s, v2.4s, v5.4s		 	 \n"

		"	ldr		q6, [x11], #16					 \n"
		"	ldr		q11, [x16], #16					 \n"

		".endm 										 \n"


		".macro	N1M3_KERNEL3x1_K0				 	\n"

		"	ldr		q1, [x12], #16					\n"
		"	ldr		q2, [x13], #16					\n"
		"	fmla	v12.4s, v0.4s, v5.4s			\n"
		"	fmla	v13.4s,	v1.4s, v5.4s			\n"

		"	fmla	v14.4s,	v2.4s, v5.4s			\n"

		"	ldr		q6, [x11], #16					\n"

		"	prfm	PLDL1KEEP, [x16, #64]			\n"

		"	ldr		q11, [x16], #16					\n"

		".endm 										\n"


		".macro	N1M3_KERNEL3x1_K1				 	\n"

		"	ldr		q7, [x12], #16					\n"
		"	ldr		q8, [x13], #16					\n"
		"	fmla	v12.4s, v6.4s, v11.4s			\n"
		"	fmla	v13.4s,	v7.4s, v11.4s			\n"

		"	prfm	PLDL1KEEP, [x11, #64]			\n"

		"	fmla	v14.4s,	v8.4s, v11.4s			\n"

		"	prfm	PLDL1KEEP, [x12, #64]			\n"

		"	ldr		q0, [x11], #16					\n"

		"	prfm	PLDL1KEEP, [x13, #64]			\n"
		"	ldr		q5, [x16], #16					\n"

		".endm 										\n"


		".macro	N1M3_KERNEL3x1_END_K				\n"

		"	ldr		q7, [x12], #16					\n"
		"	ldr		q8, [x13], #16					\n"
		"	fmla	v12.4s, v6.4s, v11.4s			\n"
		"	fmla	v13.4s,	v7.4s, v11.4s			\n"

		"	faddp	v12.4s, v12.4s, v13.4s 			\n"

		"	fmla	v14.4s,	v8.4s, v11.4s			\n"

		"	faddp	v12.4s, v12.4s, v14.4s			\n"

		"	ins		v16.s[0], v12.s[2]				\n"
		"	ins		v16.s[1], v12.s[3]				\n"

		"	faddp 	s17, v16.2s						\n"

		"	ins 	v12.s[2], v17.s[0]				\n"

		".endm 										\n"


		".macro	N1M3_ADD_C							 \n"


		"	ld1		{v0.s}[0], [x25]				 \n"
		"	ld1		{v0.s}[1], [x26]				 \n"
		"	ld1		{v0.s}[2], [x27]				 \n"

		"	fadd 	v12.4s, v12.4s, v0.4s			 \n"

		".endm 										 \n"



		".macro	N1M3_SAVE3x1 						 \n"

		"	st1		{v12.s}[0], [x25]				 \n"
		"	st1		{v12.s}[1], [x26]				 \n"
		"	st1		{v12.s}[2], [x27]		 		 \n"

		".endm 										 \n"





		".macro	N1M2_KERNEL2x1_BEGIN_K				 \n"


		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q1, [x12], #16				     \n"
		"	ldr		q5, [x16], #16					 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"

		"	fmul	v12.4s, v0.4s, v5.4s			 \n"

		"	ldr		q6, [x11], #16					 \n"
		"	ldr		q11, [x16], #16					 \n"

		"	fmul	v13.4s, v1.4s, v5.4s			 \n"

		".endm 										 \n"


		".macro	N1M2_KERNEL2x1_K0				 	\n"

		"	ldr		q1, [x12], #16					\n"
		"	fmla	v12.4s, v0.4s, v5.4s			\n"

		"	ldr		q6, [x11], #16					\n"
		"	ldr		q11, [x16], #16					\n"

		"	fmla	v13.4s,	v1.4s, v5.4s			\n"

		"	prfm	PLDL1KEEP, [x16, #64]			\n"

		".endm 										\n"


		".macro	N1M2_KERNEL2x1_K1				 	\n"

		"	ldr		q7, [x12], #16					\n"
		"	fmla	v12.4s, v6.4s, v11.4s			\n"

		"	ldr		q0, [x11], #16					\n"
		"	ldr		q5, [x16], #16					\n"

		"	fmla	v13.4s,	v7.4s, v11.4s			\n"
		"	prfm	PLDL1KEEP, [x11, #64]			\n"
		"	prfm	PLDL1KEEP, [x12, #64]			\n"

		".endm 										\n"


		".macro	N1M2_KERNEL2x1_END_K				\n"

		"	ldr		q7, [x12], #16					\n"
		"	fmla	v12.4s, v6.4s, v11.4s			\n"
		"	fmla	v13.4s,	v7.4s, v11.4s			\n"

		"	faddp	v20.4s, v12.4s, v13.4s 			\n"
		"	faddp	s12, v20.2s						\n"

		"	ins		v16.s[0], v20.s[2]				\n"
		"	ins		v16.s[1], v20.s[3]				\n"

		"	faddp 	s17, v16.2s 					\n"
		"	ins 	v12.s[1], v17.s[0]				\n"

		".endm 										\n"


		".macro	N1M2_ADD_C							 \n"


		"	ld1		{v0.s}[0], [x25]				 \n"
		"	ld1		{v0.s}[1], [x26]				 \n"

		"	fadd 	v12.2s, v12.2s, v0.2s			 \n"

		".endm 										 \n"



		".macro	N1M2_SAVE2x1 						 \n"

		"	st1		{v12.s}[0], [x25]				 \n"
		"	st1		{v12.s}[1], [x26]				 \n"

		".endm 										 \n"



		".macro	N1M1_KERNEL1x1_BEGIN_K				 \n"


		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q5, [x16], #16					 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"

		"	fmul	v12.4s, v0.4s, v5.4s			 \n"

		"	ldr		q6, [x11], #16					 \n"
		"	ldr		q11, [x16], #16					 \n"

		".endm 										 \n"


		".macro	N1M1_KERNEL1x1_K0				 	\n"

		"	ldr		q6, [x11], #16					\n"
		"	ldr		q11, [x16], #16					\n"

		"	fmla	v12.4s, v0.4s, v5.4s			\n"

		"	prfm	PLDL1KEEP, [x16, #64]			\n"

		".endm 										\n"


		".macro	N1M1_KERNEL1x1_K1				 	\n"


		"	ldr		q0, [x11], #16					\n"
		"	ldr		q5, [x16], #16					\n"

		"	fmla	v12.4s, v6.4s, v11.4s			\n"
		"	prfm	PLDL1KEEP, [x11, #64]			\n"

		".endm 										\n"


		".macro	N1M1_KERNEL1x1_END_K				\n"

		"	fmla	v12.4s, v6.4s, v11.4s			\n"

		"	faddp	s20, v12.2s 					\n"

		"	ins		v16.s[0], v12.s[2]				\n"
		"	ins		v16.s[1], v12.s[3]				\n"

		"	faddp 	s16, v16.2s 					\n"
		"	ins		v20.s[1], v16.s[0]				\n"
		"	faddp 	s12, v20.2s 					\n"

		".endm 										\n"


		".macro	N1M1_ADD_C							 \n"


		"	ld1		{v0.s}[0], [x25]				 \n"

		"	fadd 	s12, s12, s0			 		 \n"

		".endm 										 \n"



		".macro	N1M1_SAVE1x1 						 \n"

		"	st1		{v12.s}[0], [x25]				 \n"

		".endm 										 \n"













//----------------------------------------------------

		"SMM:									 	 \n"

		"	ldr		x0, %[C]						 \n"
		"	ldr		x1, %[A]						 \n"
		"	ldr		x2, %[B]						 \n"

		"	ldr		x3, %[M]						 \n"
		"	ldr		x4, %[N]						 \n"
		"	ldr		x5, %[K]						 \n"
		"	ldr		x9, %[LN] 						 \n"
		"	ldr		x6, %[LK]						 \n"

		"	ldr		x19, %[SB]						 \n"
		"	ldr		x8, %[k_tag] 					 \n"

		"	add		sp, sp, #-32					 \n"

		"	mov		x23, #5						 	 \n"

		"   prfm    PLDL1KEEP, [x1]             	 \n"
		"   prfm    PLDL1KEEP, [x2]             	 \n"

		"	udiv	x30, x3, x23				 	 \n"	// M / 5
		"	lsr		x20, x4, #4					 	 \n"	// N / 16

		"	msub	x23, x30, x23, x3 				 \n"	// M % 5
		"	str 	x23, [sp] 						 \n"
		"	mov 	x23, x19 						 \n"    //SB
		"	str  	x8, [sp, #16]					 \n"

//		"	cmp		x20, #0							 \n"
//		"	beq		BEGIN_N8						 \n"



//-----------------------------------------------N16
		"BEGIN_N16:								 	 \n"

		"	mov 	x25, x0   						 \n"   //C1*
		"	add		x26, x25, x9, lsl #2 			 \n"   //C2*
		"	add 	x27, x25, x9, lsl #3 			 \n"   //C3*
		" 	add 	x28, x26, x9, lsl #3 			 \n"   //C4*
		"	add		x29, x27, x9, lsl #3 			 \n"   //C5*

		"	mov		x10, x1   						 \n"
		"	mov		x21, x30  						 \n"
		"	mov		x24, x23						 \n"   //还原SB的地址

		"	mov		x7, #4							 \n"   //4次B的循环
		"	mov		x8, #16							 \n"

		"BEGIN_PACKB:								 \n"

		"	mov		x16, x2						 	 \n"   //B0*
		"	add		x17, x16, x6, lsl #2 			 \n"
		"	add		x18, x17, x6, lsl #2 			 \n"
		"	add		x19, x18, x6, lsl #2 			 \n"

		"	mov		x11, x10						 \n"    //A0*
		"	add		x12, x11, x6, lsl #2			 \n"    //A1*
		"	add		x13, x12, x6, lsl #2			 \n"	//A2*
		"	add		x14, x13, x6, lsl #2			 \n"	//A3*
		"	add		x15, x14, x6, lsl #2 			 \n"	//A4*

		"	prfm    PLDL1KEEP, [x16, #64]			 \n"
		"	prfm    PLDL1KEEP, [x17, #64]			 \n"
		"	prfm    PLDL1KEEP, [x18, #64]			 \n"
		"	prfm    PLDL1KEEP, [x19, #64]			 \n"

		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"
		"	prfm    PLDL1KEEP, [x13, #64]			 \n"
		"	prfm    PLDL1KEEP, [x14, #64]			 \n"
		"	prfm    PLDL1KEEP, [x15, #64]			 \n"

		"PACK_Body_K:								 \n"

		"	lsr		x22, x5, #3 					 \n"     // K / 8

		"	PACK_KERNEL5x4_BEGIN_K				 	 \n"

		"	subs	x22, x22, #1		     		 \n"		
		"	b 		PACK_K1_7						 \n"

		"PACK_K:									 \n"
		
		"	PACK_KERNEL5x4_K0						 \n"
		"PACK_K1_7:									 \n"

		"	beq		PACK_Edge_K						 \n"

		"	PACK_KERNEL5x4_K1						 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		PACK_K			 				 \n"	

		"PACK_Edge_K:							     \n"

		"	PACK_KERNEL5x4_END_K			         \n"		

		"	ldr		x16, [sp, #16]					 \n"	
		"	cmp		x16, #0							 \n"
		"	beq		PACKB_SAVE1 					 \n"
		"	PACKB_ADD_C 							 \n"
		"PACKB_SAVE1: 								 \n"
		"	SAVE5x4								 	 \n"
//		"	mov		x8, #16							 \n"

//		"	beq		END_PACKB						 \n"

		"	add		x23, x23, #16					 \n"
		"	mov		x24, x23 						 \n"
		"	add		x2, x2, x6, lsl #4				 \n"

		"	bgt  	BEGIN_PACKB					 	 \n"

		"END_PACKB:									 \n"

		"	sub		x23, x23, x8, lsl #2			 \n"    //还原SB的索引

		"	sub 	x29, x29, x8, lsl #2			 \n"    //C矩阵的偏移量还原
		"	add		x25, x29, x9, lsl #2			 \n"
		"	add		x26, x25, x9, lsl #2			 \n"
		"	add		x27, x26, x9, lsl #2			 \n"
		"	add		x28, x27, x9, lsl #2			 \n"
		"	add		x29, x28, x9, lsl #2 			 \n"

		"	add		x10, x10, x6, lsl #4			 \n"  
		"	add 	x10, x10, x6, lsl #2 			 \n"	// A + 5行

		"	cmp		x8, #16							 \n"
		"	bne 	N8_END_PACKB					 \n"

		"	subs	x21, x21, #1					 \n"
		"	beq		END_M5							 \n"


		//---------------------------------------------------

		"BEGIN_M5:									 \n"

		"	mov		x24, x23	     				 \n"

		"	mov		x11, x10						 \n"
		"	add		x12, x11, x6, lsl #2			 \n"
		"	add		x13, x12, x6, lsl #2			 \n"
		"	add		x14, x13, x6, lsl #2			 \n"
		"	add		x15, x14, x6, lsl #2 			 \n"

		"	prfm	PLDL1KEEP, [x24, #512]			 \n"

		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"
		"	prfm    PLDL1KEEP, [x13, #64]			 \n"
		"	prfm    PLDL1KEEP, [x14, #64]			 \n"
		"	prfm    PLDL1KEEP, [x15, #64]			 \n"


		"Body_K:								 	 \n"

		"	lsr		x22, x5, #3 					 \n"     // K / 8
		"	KERNEL5x16_BEGIN_K						 \n"
		"	subs	x22, x22, #1		     		 \n"	
		"	b K1_7									 \n"

		"Main_K:									 \n"
		

		"	KERNEL5x16_K0						 	 \n"
		"K1_7:										 \n"
		
		"	KERNEL5x16_K1							 \n"
		"	KERNEL5x16_K2							 \n"
		"	KERNEL5x16_K3							 \n"
		"	KERNEL5x16_K4							 \n"
		"	KERNEL5x16_K5							 \n"
		"	KERNEL5x16_K6							 \n"

		"	beq		Edge_K							 \n"

		"	KERNEL5x16_K7							 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		Main_K			 				 \n"	

		"Edge_K:									 \n"

		"	KERNEL5x16_END_K						 \n"

		"	ldr		x8, [sp, #16]					 \n"	
		"	cmp		x8, #0							 \n"
		"	beq		M5_SAVE1 						 \n"
		"	ADD_C 								 	 \n"
		"M5_SAVE1: 									 \n"
		"	SAVE5x16								 \n"	

		"	bgt   	BEGIN_M5						 \n"

		"END_M5:									 \n"


		"	mov		x24, x23	     				 \n"  //B0*

		"	ldr		x21, [sp]					 	 \n"
		"	cmp		x21, #4							 \n"
		"	bne 	BEGIN_M3 						 \n"



	//--------------------------------------------------------
		"BEGIN_M4:						 			 \n"

		"	mov		x11, x10 						 \n"
		"	add		x12, x11, x6, lsl #2 		     \n"
		"	add		x13, x12, x6, lsl #2 			 \n"
		"	add		x14, x13, x6, lsl #2 			 \n"

		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"
		"	prfm    PLDL1KEEP, [x13, #64]			 \n"
		"	prfm    PLDL1KEEP, [x14, #64]			 \n"


		"M4_Body_K:								 	 \n"

		"	lsr		x22, x5, #2 					 \n"     // K / 8
		"	KERNEL4x16_BEGIN_K						 \n"
		"	subs	x22, x22, #1		     		 \n"	
		"	b 		M4_K1_3							 \n"

		"M4_Main_K:									 \n"
		
		"	KERNEL4x16_K0						 	 \n"
		"M4_K1_3:									 \n"
		
		"	KERNEL4x16_K1							 \n"
		"	KERNEL4x16_K2							 \n"

		"	beq		M4_Edge_K						 \n"

		"	KERNEL4x16_K3							 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		M4_Main_K			 			 \n"	

		"M4_Edge_K:									 \n"


		"	prfm	PLDL1KEEP, [x25, #64]			 \n"
		"	prfm	PLDL1KEEP, [x26, #64]			 \n"
		"	prfm	PLDL1KEEP, [x27, #64]			 \n"
		"	prfm	PLDL1KEEP, [x28, #64]			 \n"

		"	KERNEL4x16_END_K						 \n"

		"	ldr		x8, [sp, #16]					 \n"	
		"	cmp		x8, #0							 \n"
		"	beq		M4_SAVE1 						 \n"
		"	M4_ADD_C 								 \n"
		"M4_SAVE1: 									 \n"
		"	SAVE4x16								 \n"

		"	b 		END_M 							 \n"


		//--------------------------------------------------------

		"BEGIN_M3:			 						 \n"
		"	cmp		x21, #3							 \n"
		"	bne 	BEGIN_M2 						 \n"

		"	mov		x11, x10 						 \n"
		"	add		x12, x11, x6, lsl #2 			 \n"
		"	add		x13, x12, x6, lsl #2 			 \n"

		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"
		"	prfm    PLDL1KEEP, [x13, #64]			 \n"

		"M3_Body_K:								 	 \n"

		"	lsr		x22, x5, #2 					 \n"     // K / 8
		"	KERNEL3x16_BEGIN_K						 \n"
		"	subs	x22, x22, #1		     		 \n"	
		"	b 		M3_K1_3							 \n"

		"M3_Main_K:									 \n"
		
		"	KERNEL3x16_K0						 	 \n"
		"M3_K1_3:									 \n"
		
		"	KERNEL3x16_K1							 \n"
		"	KERNEL3x16_K2							 \n"

		"	beq		M3_Edge_K						 \n"

		"	KERNEL3x16_K3							 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		M3_Main_K			 			 \n"	

		"M3_Edge_K:									 \n"

		"	prfm	PLDL1KEEP, [x25, #64]			 \n"
		"	prfm	PLDL1KEEP, [x26, #64]			 \n"
		"	prfm	PLDL1KEEP, [x27, #64]			 \n"

		"	KERNEL3x16_END_K						 \n"	

		"	ldr		x8, [sp, #16]					 \n"	
		"	cmp		x8, #0							 \n"
		"	beq		M3_SAVE1 						 \n"
		"	M3_ADD_C 								 \n"
		"M3_SAVE1: 									 \n"
		"	SAVE3x16								 \n"

		"	b 		END_M 							 \n"




//----------------------------------------------------------
		"BEGIN_M2:			 						 \n"

		"	cmp		x21, #2							 \n"
		"	bne 	BEGIN_M1 						 \n"

		"	mov		x11, x10 						 \n"
		"	add		x12, x11, x6, lsl #2 			 \n"

		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"

		"M2_Body_K:								 	 \n"

		"	lsr		x22, x5, #2 					 \n"     // K / 8
		"	KERNEL2x16_BEGIN_K						 \n"
		"	subs	x22, x22, #1		     		 \n"	
		"	b 		M2_K1_3							 \n"

		"M2_Main_K:									 \n"
		
		"	KERNEL2x16_K0						 	 \n"
		"M2_K1_3:									 \n"
		
		"	KERNEL2x16_K1							 \n"
		"	KERNEL2x16_K2							 \n"

		"	beq		M2_Edge_K						 \n"

		"	KERNEL2x16_K3							 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		M2_Main_K			 			 \n"	

		"M2_Edge_K:									 \n"

		"	prfm	PLDL1KEEP, [x25, #64]			 \n"
		"	prfm	PLDL1KEEP, [x26, #64]			 \n"

		"	KERNEL2x16_END_K						 \n"

		"	ldr		x8, [sp, #16]					 \n"	
		"	cmp		x8, #0							 \n"
		"	beq		M2_SAVE1 						 \n"
		"	M2_ADD_C 								 \n"
		"M2_SAVE1: 									 \n"
		"	SAVE2x16								 \n"

		"	b 		END_M 							 \n"




//---------------------------------------------------------		

		"BEGIN_M1:									 \n"

		"	cmp		x21, #1							 \n"
		"	bne 	END_M 						 	 \n"

		"	mov		x11, x10 						 \n"
		"	prfm    PLDL1KEEP, [x11, #64]			 \n"

		"M1_Body_K:								 	 \n"

		"	lsr		x22, x5, #2 					 \n"     // K / 8
		"	KERNEL1x16_BEGIN_K						 \n"
		"	subs	x22, x22, #1		     		 \n"	
		"	b 		M1_K1_3							 \n"

		"M1_Main_K:									 \n"
		
		"	KERNEL1x16_K0						 	 \n"
		"M1_K1_3:									 \n"
		
		"	KERNEL1x16_K1							 \n"
		"	KERNEL1x16_K2							 \n"

		"	beq		M1_Edge_K						 \n"

		"	KERNEL1x16_K3							 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		M1_Main_K			 			 \n"	

		"M1_Edge_K:									 \n"

		"	prfm	PLDL1KEEP, [x25, #64]			 \n"

		"	KERNEL1x16_END_K						 \n"

		"	ldr		x8, [sp, #16]					 \n"	
		"	cmp		x8, #0							 \n"
		"	beq		M1_SAVE1 						 \n"
		"	M1_ADD_C 								 \n"
		"M1_SAVE1: 									 \n"

		"	SAVE1x16								 \n"

		"END_M:										 \n"


		"	subs	x20, x20, #1					 \n"

//		"	add		x16, x16, #64					 \n"
		"	add		x0, x0, #64						 \n"

		"	bgt		BEGIN_N16						 \n"

		"END_N16:									 \n"





		"	ands	x20, x4, #15					 \n"
		"	beq		END_N 							 \n"

		"	cmp		x20, #8 						 \n"
		"	blt		BEGIN_N4 						 \n"
		"	sub 	x20, x20, #8 					 \n"



//	    "N8_END_PACKB:	    						 \n"
		


/*
----------------------------------------
----------------------------------------
----------------------------------------
----------------------------------------
----------------------------------------
*/



//------------------------------------------------------------M5N8
		"BEGIN_N8:									\n"


//----------------------------------------------------------N8
		"N8_BEGIN_PACKB:							 \n"

		"	mov 	x25, x0   						 \n"   //C1*
		"	add		x26, x25, x9, lsl #2 			 \n"   //C2*
		"	add 	x27, x25, x9, lsl #3 			 \n"   //C3*
		" 	add 	x28, x26, x9, lsl #3 			 \n"   //C4*
		"	add		x29, x27, x9, lsl #3 			 \n"   //C5*

		"	mov		x10, x1   						 \n"   //A0*
		"	mov		x21, x30  						 \n"   // M / 5
		"	mov		x7, #2							 \n"
		"	mov		x24, x23						 \n"

		"	mov		x8, #8 							 \n"

		"	b 		BEGIN_PACKB 					 \n"

		"N8_END_PACKB:								 \n"

		"	subs	x21, x21, #1					 \n"
		"	beq		N8_END_M5						 \n"


//--------------------------------------------------N8M5
		"N8_BEGIN_M5:								 \n"

		"	mov		x24, x23	     				 \n"

		"	mov		x11, x10						 \n"
		"	add		x12, x11, x6, lsl #2			 \n"
		"	add		x13, x12, x6, lsl #2			 \n"
		"	add		x14, x13, x6, lsl #2			 \n"
		"	add		x15, x14, x6, lsl #2 			 \n"

		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"
		"	prfm    PLDL1KEEP, [x13, #64]			 \n"
		"	prfm    PLDL1KEEP, [x14, #64]			 \n"
		"	prfm    PLDL1KEEP, [x15, #64]			 \n"


		"N8_Body_K:								 	 \n"

		"	lsr		x22, x5, #2 					 \n"     // K / 8
		"	N8_KERNEL5x8_BEGIN_K					 \n"
		"	subs	x22, x22, #1		     		 \n"	
		"	b 		N8_K1_3							 \n"

		"N8_Main_K:									 \n"
		

		"	N8_KERNEL5x8_K0						 	 \n"
		"N8_K1_3:									 \n"
		
		"	N8_KERNEL5x8_K1							 \n"
		"	N8_KERNEL5x8_K2							 \n"

		"	beq		N8_Edge_K						 \n"

		"	N8_KERNEL5x8_K3							 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		N8_Main_K			 			 \n"	

		"N8_Edge_K:									 \n"


		"	prfm	PLDL1KEEP, [x25, #64]			 \n"
		"	prfm	PLDL1KEEP, [x26, #64]			 \n"
		"	prfm	PLDL1KEEP, [x27, #64]			 \n"
		"	prfm	PLDL1KEEP, [x28, #64]			 \n"
		"	prfm	PLDL1KEEP, [x29, #64]			 \n"

		"	N8_KERNEL5x8_END_K						 \n"

		"	ldr		x8, [sp, #16]					 \n"	
		"	cmp		x8, #0							 \n"
		"	beq		N8M5_SAVE 						 \n"
		"	N8M5_ADD_C 								 \n"
		"N8M5_SAVE: 								 \n"		

		"	N8_SAVE5x8								 \n"
		"	bgt   	N8_BEGIN_M5					 	 \n"





//---------------------------------------------------------

		"N8_END_M5:									 \n"

		"	mov		x24, x23	     				 \n"  //B0*

		"	ldr		x21, [sp]						 \n"
		"	cmp		x21, #4							 \n"
		"	bne 	N8_BEGIN_M3 					 \n"

	//--------------------------------------------------------
		"N8_BEGIN_M4:						 		 \n"

		"	mov		x11, x10 						 \n"
		"	add		x12, x11, x6, lsl #2 			 \n"
		"	add		x13, x12, x6, lsl #2 			 \n"
		"	add		x14, x13, x6, lsl #2 			 \n"

		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"
		"	prfm    PLDL1KEEP, [x13, #64]			 \n"
		"	prfm    PLDL1KEEP, [x14, #64]			 \n"


		"N8_M4_Body_K:								 \n"

		"	lsr		x22, x5, #2 					 \n"     // K / 8
		"	N8_KERNEL4x8_BEGIN_K					 \n"
		"	subs	x22, x22, #1		     		 \n"	
		"	b 		N8_M4_K1_3						 \n"

		"N8_M4_Main_K:								 \n"
		
		"	N8_KERNEL4x8_K0						 	 \n"
		"N8_M4_K1_3:								 \n"
		
		"	N8_KERNEL4x8_K1						 	 \n"
		"	N8_KERNEL4x8_K2						 	 \n"

		"	beq		N8_M4_Edge_K					 \n"

		"	N8_KERNEL4x8_K3						 	 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		N8_M4_Main_K			 		 \n"	

		"N8_M4_Edge_K:								 \n"


		"	prfm	PLDL1KEEP, [x25, #32]			 \n"
		"	prfm	PLDL1KEEP, [x26, #32]			 \n"
		"	prfm	PLDL1KEEP, [x27, #32]			 \n"
		"	prfm	PLDL1KEEP, [x28, #32]			 \n"

		"	N8_KERNEL4x8_END_K						 \n"


		"	ldr		x8, [sp, #16]					 \n"	
		"	cmp		x8, #0							 \n"
		"	beq		N8M4_SAVE 						 \n"
		"	N8M4_ADD_C 								 \n"
		"N8M4_SAVE: 								 \n"		
	
		"	N8_SAVE4x8								 \n"

		"	b 		N8_END_M 						 \n"


//------------------------------------------------------------------

		"N8_BEGIN_M3: 								 \n"

		"	cmp		x21, #3							 \n"
		"	bne 	N8_BEGIN_M2 					 \n"

		"	mov		x11, x10 						 \n"
		"	add		x12, x11, x6, lsl #2 			 \n"
		"	add		x13, x12, x6, lsl #2 			 \n"

		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"
		"	prfm    PLDL1KEEP, [x13, #64]			 \n"


		"N8_M3_Body_K:								 \n"

		"	lsr		x22, x5, #2 					 \n"     // K / 8
		"	N8_KERNEL3x8_BEGIN_K					 \n"
		"	subs	x22, x22, #1		     		 \n"	
		"	b 		N8_M3_K1_3						 \n"

		"N8_M3_Main_K:								 \n"
		
		"	N8_KERNEL3x8_K0						 	 \n"
		"N8_M3_K1_3:								 \n"
		
		"	N8_KERNEL3x8_K1						 	 \n"
		"	N8_KERNEL3x8_K2						 	 \n"

		"	beq		N8_M3_Edge_K					 \n"

		"	N8_KERNEL3x8_K3						 	 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		N8_M3_Main_K			 		 \n"	

		"N8_M3_Edge_K:								 \n"


		"	prfm	PLDL1KEEP, [x25, #32]			 \n"
		"	prfm	PLDL1KEEP, [x26, #32]			 \n"
		"	prfm	PLDL1KEEP, [x27, #32]			 \n"

		"	N8_KERNEL3x8_END_K						 \n"	


		"	ldr		x8, [sp, #16]					 \n"	
		"	cmp		x8, #0							 \n"
		"	beq		N8M3_SAVE 						 \n"
		"	N8M3_ADD_C 								 \n"
		"N8M3_SAVE: 								 \n"		
	
		"	N8_SAVE3x8								 \n"

		"	b 		N8_END_M 						 \n"





//--------------------------------------------------------

		"N8_BEGIN_M2:								 \n"


		"	cmp		x21, #2							 \n"
		"	bne 	N8_BEGIN_M1 					 \n"

		"	mov		x11, x10 						 \n"
		"	add		x12, x11, x6, lsl #2 			 \n"

		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"


		"N8_M2_Body_K:								 \n"

		"	lsr		x22, x5, #2 					 \n"     // K / 8
		"	N8_KERNEL2x8_BEGIN_K					 \n"
		"	subs	x22, x22, #1		     		 \n"	
		"	b 		N8_M2_K1_3						 \n"

		"N8_M2_Main_K:								 \n"
		
		"	N8_KERNEL2x8_K0						 	 \n"
		"N8_M2_K1_3:								 \n"
		
		"	N8_KERNEL2x8_K1						 	 \n"
		"	N8_KERNEL2x8_K2						 	 \n"

		"	beq		N8_M2_Edge_K					 \n"

		"	N8_KERNEL2x8_K3						 	 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		N8_M2_Main_K			 		 \n"	

		"N8_M2_Edge_K:								 \n"


		"	prfm	PLDL1KEEP, [x25, #32]			 \n"
		"	prfm	PLDL1KEEP, [x26, #32]			 \n"

		"	N8_KERNEL2x8_END_K						 \n"


		"	ldr		x8, [sp, #16]					 \n"	
		"	cmp		x8, #0							 \n"
		"	beq		N8M2_SAVE 						 \n"
		"	N8M2_ADD_C 								 \n"
		"N8M2_SAVE: 								 \n"			
		"	N8_SAVE2x8								 \n"

		"	b 		N8_END_M 						 \n"





//-----------------------------------------------------
		"N8_BEGIN_M1:								 \n"


		"	cmp		x21, #1							 \n"
		"	bne 	N8_END_M 					 	 \n"

		"	mov		x11, x10 						 \n"

		"	prfm    PLDL1KEEP, [x11, #64]			 \n"

		"N8_M1_Body_K:								 \n"

		"	lsr		x22, x5, #2 					 \n"     // K / 8
		"	N8_KERNEL1x8_BEGIN_K					 \n"
		"	subs	x22, x22, #1		     		 \n"	
		"	b 		N8_M1_K1_3						 \n"

		"N8_M1_Main_K:								 \n"
		
		"	N8_KERNEL1x8_K0						 	 \n"
		"N8_M1_K1_3:								 \n"
		
		"	N8_KERNEL1x8_K1						 	 \n"
		"	N8_KERNEL1x8_K2						 	 \n"

		"	beq		N8_M1_Edge_K					 \n"

		"	N8_KERNEL1x8_K3						 	 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		N8_M1_Main_K			 		 \n"	

		"N8_M1_Edge_K:								 \n"

		"	prfm	PLDL1KEEP, [x25, #32]			 \n"

		"	N8_KERNEL1x8_END_K						 \n"

		"	ldr		x8, [sp, #16]					 \n"	
		"	cmp		x8, #0							 \n"
		"	beq		N8M1_SAVE 						 \n"
		"	N8M1_ADD_C 								 \n"
		"N8M1_SAVE: 								 \n"			
		"	N8_SAVE1x8								 \n"

		"N8_END_M: 									 \n"

		"	add 	x0, x0, #32 					 \n"

//--------------------------------------------------------





		"BEGIN_N4: 									 \n"

		"	cmp		x20, #4 						 \n"
		"	blt		BEGIN_N2 						 \n"
		"	sub 	x20, x20, #4 					 \n"


		"N4M5_BEGIN:								 \n"

		"	mov 	x25, x0   						 \n"   //C1*
		"	add		x26, x25, x9, lsl #2 			 \n"   //C2*
		"	add 	x27, x25, x9, lsl #3 			 \n"   //C3*
		" 	add 	x28, x26, x9, lsl #3 			 \n"   //C4*
		"	add		x29, x27, x9, lsl #3 			 \n"   //C5*

		"	mov		x10, x1   						 \n"   //address_A
		"	mov		x21, x30  						 \n"


		"N4M5_MAIN:									 \n"

		"	mov		x11, x10						 \n"    //A0*
		"	add		x12, x11, x6, lsl #2			 \n"    //A1*
		"	add		x13, x12, x6, lsl #2			 \n"	//A2*
		"	add		x14, x13, x6, lsl #2			 \n"	//A3*
		"	add		x15, x14, x6, lsl #2 			 \n"	//A4*

		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"
		"	prfm    PLDL1KEEP, [x13, #64]			 \n"
		"	prfm    PLDL1KEEP, [x14, #64]			 \n"
		"	prfm    PLDL1KEEP, [x15, #64]			 \n"

		"	mov		x16, x2						 	 \n"   //B0*
		"	add		x17, x16, x6, lsl #2 			 \n"
		"	add		x18, x17, x6, lsl #2 			 \n"
		"	add		x19, x18, x6, lsl #2 			 \n"

		"	prfm    PLDL1KEEP, [x16, #64]			 \n"
		"	prfm    PLDL1KEEP, [x17, #64]			 \n"
		"	prfm    PLDL1KEEP, [x18, #64]			 \n"
		"	prfm    PLDL1KEEP, [x19, #64]			 \n"


		"N4M5_Body_K:								 \n"

		"	lsr		x22, x5, #3 					 \n"     // K / 8

		"	N4M5_KERNEL5x4_BEGIN_K				 	 \n"

		"	subs	x22, x22, #1		     		 \n"
		"	b 		N4M5_K1_7						 \n"


		"N4M5_K:									 \n"

		"	N4M5_KERNEL5x4_K0						 \n"
		"N4M5_K1_7:									 \n"

		"	beq		N4M5_Edge_K						 \n"

		"	N4M5_KERNEL5x4_K1						 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		N4M5_K			 				 \n"	

		"N4M5_Edge_K:							     \n"


		"	prfm	PLDL1KEEP, [x25, #32]			 \n"
		"	prfm	PLDL1KEEP, [x26, #32]			 \n"
		"	prfm	PLDL1KEEP, [x27, #32]			 \n"
		"	prfm	PLDL1KEEP, [x28, #32]			 \n"
		"	prfm	PLDL1KEEP, [x29, #32]			 \n"

		"	N4M5_KERNEL5x4_END_K			         \n"		

		"	ldr		x16, [sp, #16]					 \n"	
		"	cmp		x16, #0							 \n"
		"	beq		N4M5_SAVE	 					 \n"
		"	PACKB_ADD_C 							 \n"
		"N4M5_SAVE: 								 \n"
		"	N4M5_SAVE5x4							 \n"

		"	bgt		N4M5_MAIN						 \n"



//---------------------------------------------------------

		"N4_END_M5:									 \n"

		"	mov		x16, x2						 	 \n"   //B0*
		"	add		x17, x16, x6, lsl #2 			 \n"
		"	add		x18, x17, x6, lsl #2 			 \n"
		"	add		x19, x18, x6, lsl #2 			 \n"

		"	prfm    PLDL1KEEP, [x16, #64]			 \n"
		"	prfm    PLDL1KEEP, [x17, #64]			 \n"
		"	prfm    PLDL1KEEP, [x18, #64]			 \n"
		"	prfm    PLDL1KEEP, [x19, #64]			 \n"


		"	ldr		x21, [sp]						 \n"
		"	cmp		x21, #4							 \n"
		"	bne 	N4_BEGIN_M3 					 \n"


//--------------------------------------------------------
		"N4_BEGIN_M4:						 		 \n"

		"	mov		x11, x10 						 \n"   //A0*
		"	add		x12, x11, x6, lsl #2 			 \n"
		"	add		x13, x12, x6, lsl #2 			 \n"
		"	add		x14, x13, x6, lsl #2 			 \n"

		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"
		"	prfm    PLDL1KEEP, [x13, #64]			 \n"
		"	prfm    PLDL1KEEP, [x14, #64]			 \n"

		"N4_M4_Body_K:								 \n"

		"	lsr		x22, x5, #3 					 \n"     // K / 8
		"	N4_KERNEL4x4_BEGIN_K					 \n"
		"	subs	x22, x22, #1		     		 \n"	
		"	b 		N4_M4_K1_7						 \n"

		"N4_M4_Main_K:								 \n"
		
		"	N4_KERNEL4x4_K0						 	 \n"
		"N4_M4_K1_7:								 \n"

		"	beq		N4_M4_Edge_K					 \n"

		"	N4_KERNEL4x4_K1						 	 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		N4_M4_Main_K			 		 \n"	

		"N4_M4_Edge_K:								 \n"


		"	prfm	PLDL1KEEP, [x25, #16]			 \n"
		"	prfm	PLDL1KEEP, [x26, #16]			 \n"
		"	prfm	PLDL1KEEP, [x27, #16]			 \n"
		"	prfm	PLDL1KEEP, [x28, #16]			 \n"

		"	N4_KERNEL4x4_END_K						 \n"


		"	ldr		x8, [sp, #16]					 \n"	
		"	cmp		x8, #0							 \n"
		"	beq		N4M4_SAVE 						 \n"
		"	N4M4_ADD_C 								 \n"
		"N4M4_SAVE: 								 \n"		
	
		"	N4M4_SAVE4x4							 \n"

		"	b 		N4_END_M 						 \n"



//--------------------------------------------------------
		"N4_BEGIN_M3:						 		 \n"


		"	ldr		x21, [sp]						 \n"
		"	cmp		x21, #3							 \n"
		"	bne 	N4_BEGIN_M2 					 \n"

		"	mov		x11, x10 						 \n"   //A0*
		"	add		x12, x11, x6, lsl #2 			 \n"
		"	add		x13, x12, x6, lsl #2 			 \n"

		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"
		"	prfm    PLDL1KEEP, [x13, #64]			 \n"

		"N4_M3_Body_K:								 \n"

		"	lsr		x22, x5, #3 					 \n"     // K / 8
		"	N4_KERNEL3x4_BEGIN_K					 \n"
		"	subs	x22, x22, #1		     		 \n"	
		"	b 		N4_M3_K1_7						 \n"

		"N4_M3_Main_K:								 \n"
		
		"	N4_KERNEL3x4_K0						 	 \n"
		"N4_M3_K1_7:								 \n"

		"	beq		N4_M3_Edge_K					 \n"

		"	N4_KERNEL3x4_K1						 	 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		N4_M3_Main_K			 		 \n"	

		"N4_M3_Edge_K:								 \n"


		"	prfm	PLDL1KEEP, [x25, #16]			 \n"
		"	prfm	PLDL1KEEP, [x26, #16]			 \n"
		"	prfm	PLDL1KEEP, [x27, #16]			 \n"

		"	N4_KERNEL3x4_END_K						 \n"


		"	ldr		x8, [sp, #16]					 \n"	
		"	cmp		x8, #0							 \n"
		"	beq		N4M3_SAVE 						 \n"
		"	N4M3_ADD_C 								 \n"
		"N4M3_SAVE: 								 \n"		
	
		"	N4M3_SAVE3x4							 \n"

		"	b 		N4_END_M 						 \n"






//------------------------------------------------------M2N4
		"N4_BEGIN_M2:								 \n"

		"	ldr		x21, [sp]						 \n"
		"	cmp		x21, #2							 \n"
		"	bne 	N4_BEGIN_M1 					 \n"

		"	mov		x11, x10 						 \n"   //A0*
		"	add		x12, x11, x6, lsl #2 			 \n"

		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"

		"N4_M2_Body_K:								 \n"

		"	lsr		x22, x5, #3 					 \n"     // K / 8
		"	N4_KERNEL2x4_BEGIN_K					 \n"
		"	subs	x22, x22, #1		     		 \n"	
		"	b 		N4_M2_K1_7						 \n"

		"N4_M2_Main_K:								 \n"
		
		"	N4_KERNEL2x4_K0						 	 \n"
		"N4_M2_K1_7:								 \n"

		"	beq		N4_M2_Edge_K					 \n"

		"	N4_KERNEL2x4_K1						 	 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		N4_M2_Main_K			 		 \n"	

		"N4_M2_Edge_K:								 \n"


		"	prfm	PLDL1KEEP, [x25, #16]			 \n"
		"	prfm	PLDL1KEEP, [x26, #16]			 \n"

		"	N4_KERNEL2x4_END_K						 \n"


		"	ldr		x8, [sp, #16]					 \n"	
		"	cmp		x8, #0							 \n"
		"	beq		N4M2_SAVE 						 \n"
		"	N4M2_ADD_C 								 \n"
		"N4M2_SAVE: 								 \n"		
	
		"	N4M2_SAVE2x4							 \n"

		"	b 		N4_END_M 						 \n"


		"N4_BEGIN_M1:								 \n"

		"	ldr		x21, [sp]						 \n"
		"	cmp		x21, #1							 \n"
		"	bne 	N4_END_M 					 	 \n"

		"	mov		x11, x10 						 \n"   //A0*

		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"


		"N4_M1_Body_K:								 \n"

		"	lsr		x22, x5, #3 					 \n"     // K / 8
		"	N4_KERNEL1x4_BEGIN_K					 \n"
		"	subs	x22, x22, #1		     		 \n"	
		"	b 		N4_M1_K1_7						 \n"

		"N4_M1_Main_K:								 \n"
		
		"	N4_KERNEL1x4_K0						 	 \n"
		"N4_M1_K1_7:								 \n"

		"	beq		N4_M1_Edge_K					 \n"

		"	N4_KERNEL1x4_K1						 	 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		N4_M1_Main_K			 		 \n"	

		"N4_M1_Edge_K:								 \n"


		"	prfm	PLDL1KEEP, [x25, #16]			 \n"

		"	N4_KERNEL1x4_END_K						 \n"


		"	ldr		x8, [sp, #16]					 \n"	
		"	cmp		x8, #0							 \n"
		"	beq		N4M1_SAVE 						 \n"
		"	N4M1_ADD_C 								 \n"
		"N4M1_SAVE: 								 \n"		
	
		"	N4M1_SAVE1x4							 \n"


		"N4_END_M:									 \n"

		"	add 	x0, x0, #16						 \n"
		"	add 	x2, x2, x6, lsl #4				 \n"



//-------------------------------------------------------N2

		"BEGIN_N2: 									 \n"


		"	cmp		x20, #2 						 \n"
		"	blt		BEGIN_N1 						 \n"

		"	sub 	x20, x20, #2					 \n"

		"	mov 	x25, x0   						 \n"   //C1*
		"	add		x26, x25, x9, lsl #2 			 \n"   //C2*
		"	add 	x27, x25, x9, lsl #3 			 \n"   //C3*
		" 	add 	x28, x26, x9, lsl #3 			 \n"   //C4*
		"	add		x29, x27, x9, lsl #3 			 \n"   //C5*

		"	mov		x21, x30 						 \n"
		"	mov		x10, x1   						 \n"   //address_A

		"N2M5_MAIN:									 \n"

		"	mov		x11, x10						 \n"
		"	add		x12, x11, x6, lsl #2			 \n"
		"	add		x13, x12, x6, lsl #2			 \n"
		"	add		x14, x13, x6, lsl #2			 \n"
		"	add		x15, x14, x6, lsl #2 			 \n"

		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"
		"	prfm    PLDL1KEEP, [x13, #64]			 \n"
		"	prfm    PLDL1KEEP, [x14, #64]			 \n"
		"	prfm    PLDL1KEEP, [x15, #64]			 \n"

		"	mov		x16, x2 						 \n"
		"	prfm	PLDL1KEEP, [x16, #64]			 \n"
		"	add		x17, x16, x6, lsl #2 			 \n"
		"	prfm	PLDL1KEEP, [x17, #64]			 \n"


		"N2M5_Body_K:								 \n"

		"	lsr		x22, x5, #3 					 \n"     // K / 8

		"	N2M5_KERNEL5x2_BEGIN_K				 	 \n"

		"	subs	x22, x22, #1		     		 \n"
		"	b 		N2M5_K1_7						 \n"


		"N2M5_K:									 \n"

		"	N2M5_KERNEL5x2_K0						 \n"
		"N2M5_K1_7:									 \n"

		"	beq		N2M5_Edge_K						 \n"

		"	N2M5_KERNEL5x2_K1						 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		N2M5_K			 				 \n"	

		"N2M5_Edge_K:							     \n"


		"	prfm	PLDL1KEEP, [x25, #32]			 \n"
		"	prfm	PLDL1KEEP, [x26, #32]			 \n"
		"	prfm	PLDL1KEEP, [x27, #32]			 \n"
		"	prfm	PLDL1KEEP, [x28, #32]			 \n"
		"	prfm	PLDL1KEEP, [x29, #32]			 \n"

		"	N2M5_KERNEL5x2_END_K			         \n"		

		"	ldr		x16, [sp, #16]					 \n"	
		"	cmp		x16, #0							 \n"
		"	beq		N2M5_SAVE	 					 \n"
		"	N2M5_ADD_C 							 	 \n"
		"N2M5_SAVE: 								 \n"
		"	N2M5_SAVE5x2							 \n"

		"	bgt		N2M5_MAIN						 \n"




		"	ldr		x21, [sp]						 \n"
		"	cmp		x21, #4							 \n"
		"	bne 	N2_BEGIN_M3 					 \n"

		"N2_BEGIN_M4:						 		 \n"


/*......................................................*/

//--------------------------------------------------------
		"N2_BEGIN_M3:						 		 \n"

		"	ldr		x21, [sp]						 \n"
		"	cmp		x21, #3							 \n"
		"	bne 	N2_BEGIN_M2 					 \n"

		"	mov		x11, x10 						 \n"   //A0*
		"	add		x12, x11, x6, lsl #2 			 \n"
		"	add		x13, x12, x6, lsl #2 			 \n"

		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"
		"	prfm    PLDL1KEEP, [x13, #64]			 \n"

		"	mov		x16, x2 						 \n"
		"	prfm	PLDL1KEEP, [x16, #64]			 \n"
		"	add		x17, x16, x6, lsl #2 			 \n"
		"	prfm	PLDL1KEEP, [x17, #64]			 \n"


		"N2_M3_Body_K:								 \n"

		"	lsr		x22, x5, #3 					 \n"     // K / 8
		"	N2_KERNEL3x2_BEGIN_K					 \n"
		"	subs	x22, x22, #1		     		 \n"	
		"	b 		N2_M3_K1_7						 \n"

		"N2_M3_Main_K:								 \n"
		
		"	N2_KERNEL3x2_K0						 	 \n"
		"N2_M3_K1_7:								 \n"

		"	beq		N2_M3_Edge_K					 \n"

		"	N2_KERNEL3x2_K1						 	 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		N2_M3_Main_K			 		 \n"	

		"N2_M3_Edge_K:								 \n"


		"	prfm	PLDL1KEEP, [x25, #16]			 \n"
		"	prfm	PLDL1KEEP, [x26, #16]			 \n"
		"	prfm	PLDL1KEEP, [x27, #16]			 \n"

		"	N2_KERNEL3x2_END_K						 \n"


		"	ldr		x8, [sp, #16]					 \n"	
		"	cmp		x8, #0							 \n"
		"	beq		N2M3_SAVE 						 \n"
		"	N2M3_ADD_C 								 \n"
		"N2M3_SAVE: 								 \n"		
	
		"	N2M3_SAVE3x2							 \n"

		"	b 		N2_END_M 						 \n"


		"N2_BEGIN_M2:						 		 \n"

		"	ldr		x21, [sp]						 \n"
		"	cmp		x21, #2							 \n"
		"	bne 	N2_BEGIN_M1 					 \n"

/*......................................................*/
		"N2_BEGIN_M1:						 		 \n"

		"	ldr		x21, [sp]						 \n"
		"	cmp		x21, #1							 \n"
		"	bne 	N2_END_M 						 \n"


/*......................................................*/
		"N2_END_M:									 \n"

		"	add 	x0, x0, #8						 \n"







//------------------------------------------------------------
		"BEGIN_N1: 									 \n"

		"	cmp		x20, #1 						 \n"
		"	blt		END_N 						 	 \n"

		"BEGIN_N1M5:								 \n"

		"	mov 	x25, x0   						 \n"   //C1*
		"	add		x26, x25, x9, lsl #2 			 \n"   //C2*
		"	add 	x27, x25, x9, lsl #3 			 \n"   //C3*
		" 	add 	x28, x26, x9, lsl #3 			 \n"   //C4*
		"	add		x29, x27, x9, lsl #3 			 \n"   //C5*

		"	mov		x21, x30 						 \n"

		"	mov		x10, x1   						 \n"   //address_A

		"N1M5_MAIN:									 \n"

		"	mov		x11, x10						 \n"
		"	add		x12, x11, x6, lsl #2			 \n"
		"	add		x13, x12, x6, lsl #2			 \n"
		"	add		x14, x13, x6, lsl #2			 \n"
		"	add		x15, x14, x6, lsl #2 			 \n"

		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"
		"	prfm    PLDL1KEEP, [x13, #64]			 \n"
		"	prfm    PLDL1KEEP, [x14, #64]			 \n"
		"	prfm    PLDL1KEEP, [x15, #64]			 \n"

		"	mov		x16, x2 						 \n"
		"	prfm	PLDL1KEEP, [x16, #64]			 \n"

		"N1M5_Body_K:								 \n"

		"	lsr		x22, x5, #3 					 \n"     // K / 8

		"	N1M5_KERNEL5x1_BEGIN_K				 	 \n"

		"	subs	x22, x22, #1		     		 \n"
		"	b 		N1M5_K1_7						 \n"


		"N1M5_K:									 \n"

		"	N1M5_KERNEL5x1_K0						 \n"
		"N1M5_K1_7:									 \n"

		"	beq		N1M5_Edge_K						 \n"

		"	N1M5_KERNEL5x1_K1						 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		N1M5_K			 				 \n"	

		"N1M5_Edge_K:							     \n"


		"	prfm	PLDL1KEEP, [x25, #32]			 \n"
		"	prfm	PLDL1KEEP, [x26, #32]			 \n"
		"	prfm	PLDL1KEEP, [x27, #32]			 \n"
		"	prfm	PLDL1KEEP, [x28, #32]			 \n"
		"	prfm	PLDL1KEEP, [x29, #32]			 \n"

		"	N1M5_KERNEL5x1_END_K			         \n"		

		"	ldr		x16, [sp, #16]					 \n"	
		"	cmp		x16, #0							 \n"
		"	beq		N1M5_SAVE	 					 \n"
		"	N1M5_ADD_C 							 	 \n"
		"N1M5_SAVE: 								 \n"
		"	N1M5_SAVE5x1							 \n"

		"	bgt		N1M5_MAIN						 \n"



//---------------------------------------------------N1M4
		"	ldr		x21, [sp]					 	 \n"
		"	cmp		x21, #4							 \n"
		"	bne 	N1_BEGIN_M3 					 \n"


//--------------------------------------------------------
		"N1_BEGIN_M4:						 		 \n"

		"	mov		x11, x10 						 \n"
		"	add		x12, x11, x6, lsl #2 		     \n"
		"	add		x13, x12, x6, lsl #2 			 \n"
		"	add		x14, x13, x6, lsl #2 			 \n"

		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"
		"	prfm    PLDL1KEEP, [x13, #64]			 \n"
		"	prfm    PLDL1KEEP, [x14, #64]			 \n"

		"	mov		x16, x2 						 \n"
		"	prfm    PLDL1KEEP, [x16, #64]			 \n"

		"N1_M4_Body_K:								 \n"

		"	lsr		x22, x5, #3 					 \n"     // K / 8
		"	N1M4_KERNEL4x1_BEGIN_K					 \n"
		"	subs	x22, x22, #1		     		 \n"	
		"	b 		N1M4_K1_7						 \n"

		"N1M4_Main_K:								 \n"
		
		"	N1M4_KERNEL4x1_K0						 \n"
		"N1M4_K1_7:									 \n"

		"	beq		N1M4_Edge_K					 	 \n"

		"	N1M4_KERNEL4x1_K1						 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		N1M4_Main_K			 			 \n"	

		"N1M4_Edge_K:								 \n"

		"	N1M4_KERNEL4x1_END_K					 \n"

		"	ldr		x8, [sp, #16]					 \n"	
		"	cmp		x8, #0							 \n"
		"	beq		N1M4_SAVE 						 \n"
		"	N1M4_ADD_C 								 \n"
		"N1M4_SAVE: 								 \n"
		"	N1M4_SAVE4x1							 \n"

		"	b 		END_N1M 						 \n"



//--------------------------------------------------M3N1
		"N1_BEGIN_M3:								 \n"

		"	cmp		x21, #3							 \n"
		"	bne 	N1_BEGIN_M2 					 \n"


//-------------------------------------------------------

		"	mov		x11, x10 						 \n"
		"	add		x12, x11, x6, lsl #2 		     \n"
		"	add		x13, x12, x6, lsl #2 			 \n"

		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"
		"	prfm    PLDL1KEEP, [x13, #64]			 \n"

		"	mov		x16, x2 						 \n"
		"	prfm    PLDL1KEEP, [x16, #64]			 \n"

		"N1_M3_Body_K:								 \n"

		"	lsr		x22, x5, #3 					 \n"     // K / 8
		"	N1M3_KERNEL3x1_BEGIN_K					 \n"
		"	subs	x22, x22, #1		     		 \n"	
		"	b 		N1M3_K1_7						 \n"

		"N1M3_Main_K:								 \n"
		
		"	N1M3_KERNEL3x1_K0						 \n"
		"N1M3_K1_7:									 \n"

		"	beq		N1M3_Edge_K					 	 \n"

		"	N1M3_KERNEL3x1_K1						 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		N1M3_Main_K			 			 \n"	

		"N1M3_Edge_K:								 \n"

		"	N1M3_KERNEL3x1_END_K					 \n"

		"	ldr		x8, [sp, #16]					 \n"	
		"	cmp		x8, #0							 \n"
		"	beq		N1M3_SAVE 						 \n"
		"	N1M3_ADD_C 								 \n"
		"N1M3_SAVE: 								 \n"
		"	N1M3_SAVE3x1							 \n"

		"	b 		END_N1M 						 \n"




//---------------------------------------------------M2N1
		"N1_BEGIN_M2:								 \n"


		"	cmp		x21, #2							 \n"
		"	bne 	N1_BEGIN_M1 					 \n"


//-------------------------------------------------------

		"	mov		x11, x10 						 \n"
		"	add		x12, x11, x6, lsl #2 		     \n"

		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"

		"	mov		x16, x2 						 \n"
		"	prfm    PLDL1KEEP, [x16, #64]			 \n"

		"N1_M2_Body_K:								 \n"

		"	lsr		x22, x5, #3 					 \n"     // K / 8
		"	N1M2_KERNEL2x1_BEGIN_K					 \n"
		"	subs	x22, x22, #1		     		 \n"	
		"	b 		N1M2_K1_7						 \n"

		"N1M2_Main_K:								 \n"
		
		"	N1M2_KERNEL2x1_K0						 \n"
		"N1M2_K1_7:									 \n"

		"	beq		N1M2_Edge_K					 	 \n"

		"	N1M2_KERNEL2x1_K1						 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		N1M2_Main_K			 			 \n"	

		"N1M2_Edge_K:								 \n"

		"	N1M2_KERNEL2x1_END_K					 \n"

		"	ldr		x8, [sp, #16]					 \n"	
		"	cmp		x8, #0							 \n"
		"	beq		N1M2_SAVE 						 \n"
		"	N1M2_ADD_C 								 \n"
		"N1M2_SAVE: 								 \n"
		"	N1M2_SAVE2x1							 \n"

		"	b 		END_N1M 						 \n"


//---------------------------------------------------------M1N1
		"N1_BEGIN_M1:								 \n"


		"	cmp		x21, #1							 \n"
		"	bne 	END_N1M 					 	 \n"

//-------------------------------------------------------

		"	mov		x11, x10 						 \n"

		"	prfm    PLDL1KEEP, [x11, #64]			 \n"

		"	mov		x16, x2 						 \n"
		"	prfm    PLDL1KEEP, [x16, #64]			 \n"

		"N1_M1_Body_K:								 \n"

		"	lsr		x22, x5, #3 					 \n"     // K / 8
		"	N1M1_KERNEL1x1_BEGIN_K					 \n"
		"	subs	x22, x22, #1		     		 \n"	
		"	b 		N1M1_K1_7						 \n"

		"N1M1_Main_K:								 \n"
		
		"	N1M1_KERNEL1x1_K0						 \n"
		"N1M1_K1_7:									 \n"

		"	beq		N1M1_Edge_K					 	 \n"

		"	N1M1_KERNEL1x1_K1						 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		N1M1_Main_K			 			 \n"	

		"N1M1_Edge_K:								 \n"

		"	N1M1_KERNEL1x1_END_K					 \n"

		"	ldr		x8, [sp, #16]					 \n"	
		"	cmp		x8, #0							 \n"
		"	beq		N1M1_SAVE 						 \n"
		"	N1M1_ADD_C 								 \n"
		"N1M1_SAVE: 								 \n"
		"	N1M1_SAVE1x1							 \n"



		"END_N1M:									 \n"



		"END_N:									 	 \n"

		"	add		sp, sp, #32						 \n"
		
		:
		:	
			[C] "m" (C),
			[A] "m" (A),
            [B] "m" (B), 
            [M] "m" (M),
            [N] "m" (N),
            [K] "m" (K),
            [LN] "m" (LN),
            [LK] "m" (LK),
            [SB] "m" (SB),
            [k_tag] "m" (k_tag)
        :   "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
        	"x9", "x10", "x11", "x12", "x13","x14", "x15", "x16",
        	"x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24","x25",
        	"x26", "x27", "x28", "x29", "x30",
            "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
            "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
            "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
            "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"

		);


}

void SGEMM_NT_KERNEL_MP_EDGEN(float *C, float *A, float *B, long M, long N, long K, 
			long LN, long LK, float *SB, long k_tag)
{

	asm volatile(


		".macro INNER_KERNEL4x4_BEGIN_K              \n"
		"										 	 \n"

		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q1, [x12], #16				     \n"
		"	ldr		q4, [x16], #16					 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x14, #64] 			 \n"

		"	fmul	v16.4s, v0.4s, v4.4s			 \n"
		"	ldr		q2, [x13], #16					 \n"
		"	fmul	v17.4s, v1.4s, v4.4s			 \n"
		"	ldr		q3, [x14], #16					 \n"
		"	fmul	v18.4s, v2.4s, v4.4s			 \n"
		"	ldr		q5, [x17], #16					 \n"
		"	fmul	v19.4s, v3.4s, v4.4s			 \n"

		"   prfm    PLDL1KEEP, [x15, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x17, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x18, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x19, #64] 			 \n"

		"	ldr		q6, [x18], #16					 \n"

		"	fmul	v20.4s, v0.4s, v5.4s			 \n"
		"	fmul	v21.4s, v1.4s, v5.4s			 \n"

		"	ldr		q7, [x19], #16					 \n"

		"	fmul	v22.4s, v2.4s, v5.4s			 \n"
		"	fmul	v23.4s, v3.4s, v5.4s			 \n"

		"	ldr		q8, [x11], #16					 \n"

		"	fmul	v24.4s, v0.4s, v6.4s			 \n"
		"	fmul	v25.4s, v1.4s, v6.4s			 \n"
		"	ldr		q12, [x16], #16					 \n"
		"	fmul	v26.4s, v2.4s, v6.4s			 \n"
		"	ldr		q9, [x12], #16					 \n"
		"	fmul	v27.4s, v3.4s, v6.4s			 \n"
		"	ldr		q10, [x13], #16					 \n"
		"	fmul	v28.4s, v0.4s, v7.4s			 \n"
		"	ldr		q11, [x14], #16					 \n"
		"	fmul	v29.4s, v1.4s, v7.4s			 \n"
		"	ldr		q13, [x17], #16					 \n"
		"	fmul	v30.4s, v2.4s, v7.4s			 \n"
		"	ldr		q14, [x18], #16					 \n"
		"	fmul	v31.4s, v3.4s, v7.4s			 \n"
		"	ldr		q15, [x19], #16					 \n"

		".endm									 	 \n"



		".macro INNER_KERNEL4x4_K0              	 \n"
		"										 	 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x14, #64] 			 \n"


		"	ldr		q8, [x11], #16					 \n"
		"	fmla	v16.4s, v0.4s, v4.4s			 \n"
		"	fmla	v17.4s, v1.4s, v4.4s			 \n"
		"	ldr		q12, [x16], #16					 \n"
		"	fmla	v18.4s, v2.4s, v4.4s			 \n"
		"	fmla	v19.4s, v3.4s, v4.4s			 \n"

		"   prfm    PLDL1KEEP, [x15, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x17, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x18, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x19, #64] 			 \n"

		"	ldr		q9, [x12], #16					 \n"
		"	fmla	v20.4s, v0.4s, v5.4s			 \n"
		"	fmla	v21.4s, v1.4s, v5.4s			 \n"
		"	ldr		q13, [x17], #16					 \n"
		"	fmla	v22.4s, v2.4s, v5.4s			 \n"
		"	fmla	v23.4s, v3.4s, v5.4s			 \n"
		"	ldr		q10, [x13], #16					 \n"

		"	fmla	v24.4s, v0.4s, v6.4s			 \n"
		"	fmla	v25.4s, v1.4s, v6.4s			 \n"
		"	ldr		q11, [x14], #16					 \n"
		"	fmla	v26.4s, v2.4s, v6.4s			 \n"
		"	fmla	v27.4s, v3.4s, v6.4s			 \n"
		"	ldr		q14, [x18], #16					 \n"
		
		"	fmla	v28.4s, v0.4s, v7.4s			 \n"
		"	fmla	v29.4s, v1.4s, v7.4s			 \n"
		"	ldr		q15, [x19], #16					 \n"
		"	fmla	v30.4s, v2.4s, v7.4s			 \n"
		"	fmla	v31.4s, v3.4s, v7.4s			 \n"

		".endm									 	 \n"


		".macro INNER_KERNEL4x4_K1              	 \n"
		"										 	 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x14, #64] 			 \n"

		"	ldr		q0, [x11], #16					 \n"

		"	fmla	v16.4s, v8.4s, v12.4s			 \n"
		"	fmla	v17.4s, v9.4s, v12.4s			 \n"
		"	ldr		q4, [x16], #16					 \n"
		"	fmla	v18.4s, v10.4s, v12.4s			 \n"
		"	fmla	v19.4s, v11.4s, v12.4s			 \n"

		"	ldr		q1, [x12], #16					 \n"

		"   prfm    PLDL1KEEP, [x15, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x17, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x18, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x19, #64] 			 \n"

		"	fmla	v20.4s, v8.4s, v13.4s			 \n"
		"	fmla	v21.4s, v9.4s, v13.4s			 \n"
		"	ldr		q2, [x13], #16					 \n"
		"	fmla	v22.4s, v10.4s, v13.4s			 \n"
		"	fmla	v23.4s, v11.4s, v13.4s			 \n"
		"	ldr		q3, [x14], #16					 \n"	

		"	fmla	v24.4s, v8.4s, v14.4s			 \n"
		"	fmla	v25.4s, v9.4s, v14.4s			 \n"
		"	ldr		q5, [x17], #16					 \n"
		"	fmla	v26.4s, v10.4s, v14.4s			 \n"
		"	fmla	v27.4s, v11.4s, v14.4s			 \n"

		"	ldr		q6, [x18], #16					 \n"
		
		"	fmla	v28.4s, v8.4s, v15.4s			 \n"
		"	fmla	v29.4s, v9.4s, v15.4s			 \n"
		"	ldr		q7, [x19], #16					 \n"
		"	fmla	v30.4s, v10.4s, v15.4s			 \n"
		"	fmla	v31.4s, v11.4s, v15.4s			 \n"

		".endm									 	 \n"


		".macro INNER_KERNEL4x4_END_K              	 \n"
		"										 	 \n"

		"	fmla	v16.4s, v8.4s, v12.4s			 \n"
		"	fmla	v17.4s, v9.4s, v12.4s			 \n"
		"	fmla	v18.4s, v10.4s, v12.4s			 \n"
		"	fmla	v19.4s, v11.4s, v12.4s			 \n"

		"	fmla	v20.4s, v8.4s, v13.4s			 \n"
		"	fmla	v21.4s, v9.4s, v13.4s			 \n"
		"	fmla	v22.4s, v10.4s, v13.4s			 \n"
		"	fmla	v23.4s, v11.4s, v13.4s			 \n"

		"	faddp	v16.4s, v16.4s, v20.4s			 \n"
		"	faddp	v17.4s, v17.4s, v21.4s			 \n"
		"	faddp	v18.4s, v18.4s, v22.4s			 \n"
		"	faddp	v19.4s, v19.4s, v23.4s			 \n"

		"	fmla	v24.4s, v8.4s, v14.4s			 \n"
		"	fmla	v25.4s, v9.4s, v14.4s			 \n"
		"	fmla	v26.4s, v10.4s, v14.4s			 \n"
		"	fmla	v27.4s, v11.4s, v14.4s			 \n"
		
		"	fmla	v28.4s, v8.4s, v15.4s			 \n"
		"	fmla	v29.4s, v9.4s, v15.4s			 \n"
		"	fmla	v30.4s, v10.4s, v15.4s			 \n"
		"	fmla	v31.4s, v11.4s, v15.4s			 \n"

		"	faddp	v24.4s, v24.4s, v28.4s			 \n"
		"	faddp	v25.4s, v25.4s, v29.4s			 \n"
		"	faddp	v26.4s, v26.4s, v30.4s			 \n"
		"	faddp	v27.4s, v27.4s, v31.4s			 \n"

		"	faddp	v16.4s, v16.4s, v24.4s			 \n"
		"	faddp	v17.4s, v17.4s, v25.4s			 \n"
		"	faddp	v18.4s, v18.4s, v26.4s			 \n"
		"	faddp	v19.4s, v19.4s, v27.4s			 \n"

		".endm									 	 \n"



		".macro	INNER_ADD_C 						 \n"

		"	ldr		q0, [x25]						 \n"
		"	ldr		q1, [x26]						 \n"

		"	fadd 	v16.4s, v16.4s, v0.4s			 \n"
		"	ldr		q2, [x27]						 \n"
		"	fadd 	v17.4s, v17.4s, v1.4s			 \n"
		"	ldr		q3, [x28]						 \n"
		"	fadd 	v18.4s, v18.4s, v2.4s			 \n"
		"	fadd 	v19.4s, v19.4s, v3.4s			 \n"


		".endm 									 	 \n"


		".macro INNER_SAVE4x4						 \n"

		"	subs	x21, x21, #1					 \n"

		"	add 	x10, x10, x6, lsl #4			 \n"

		"	str		q16, [x25]					 	 \n"
		"	add 	x25, x25, x9, lsl #4			 \n"
		"	prfm	PLDL2KEEP, [x25, #16]			 \n"
		"	str		q17, [x26]					 	 \n"
		"	add 	x26, x26, x9, lsl #4			 \n"
		"	prfm	PLDL2KEEP, [x26, #16]			 \n"
		"	str		q18, [x27]		 			 	 \n"
		"	add 	x27, x27, x9, lsl #4			 \n"
		"	prfm	PLDL2KEEP, [x27, #16]			 \n"
		"	str		q19, [x28]		 			 	 \n"
		"	add 	x28, x28, x9, lsl #4			 \n"
		"	prfm	PLDL2KEEP, [x28, #16]			 \n"

		".endm 									 	 \n"


		//----------------------------------------------------

		"SMM_EDGEN:									 \n"

		"	ldr		x0, %[C]						 \n"
		"	ldr		x1, %[A]						 \n"
		"	ldr		x2, %[B]						 \n"

		"	ldr		x3, %[M]						 \n"
		"	ldr		x4, %[N]						 \n"
		"	ldr		x5, %[K]						 \n"
		"	ldr		x9, %[LN] 						 \n"
		"	ldr		x6, %[LK]						 \n"

		"	ldr		x23, %[SB]						 \n"
		"	ldr		x8, %[k_tag] 					 \n"


		"	lsr		x20, x4, #2						 \n"
		//-----------------------------------------------N4
		"BEGIN_INNER_N4:							 \n"

		"	mov 	x25, x0   						 \n"   //C1*
		"	add		x26, x25, x9, lsl #2 			 \n"   //C2*
		"	add 	x27, x25, x9, lsl #3 			 \n"   //C3*
		" 	add 	x28, x26, x9, lsl #3 			 \n"   //C4*

		"	mov		x10, x1   						 \n"
		"	lsr		x21, x3, #2					 	 \n"  // M / 4

		"BEGIN_INNER_M4:							 \n"

		"	mov		x16, x2						 	 \n"   //B0*
		"	add		x17, x16, x6, lsl #2 			 \n"
		"	add		x18, x17, x6, lsl #2 			 \n"
		"	add		x19, x18, x6, lsl #2 			 \n"

		"	prfm    PLDL1KEEP, [x16, #64]			 \n"
		"	prfm    PLDL1KEEP, [x17, #64]			 \n"
		"	prfm    PLDL1KEEP, [x18, #64]			 \n"
		"	prfm    PLDL1KEEP, [x19, #64]			 \n"

		"	mov		x11, x10						 \n"    //A0*
		"	add		x12, x11, x6, lsl #2			 \n"    //A1*
		"	add		x13, x12, x6, lsl #2			 \n"	//A2*
		"	add		x14, x13, x6, lsl #2			 \n"	//A3*

		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"
		"	prfm    PLDL1KEEP, [x13, #64]			 \n"
		"	prfm    PLDL1KEEP, [x14, #64]			 \n"

		"INNER_Body_K:								 \n"

		"	lsr		x22, x5, #3 					 \n"     // K / 8

		"	INNER_KERNEL4x4_BEGIN_K				 	 \n"

		"	subs	x22, x22, #1		     		 \n"		
		"	b 		INNER_K1_7						 \n"

		"INNER_K:									 \n"
		
		"	INNER_KERNEL4x4_K0						 \n"
		"INNER_K1_7:								 \n"

		"	beq		INNER_Edge_K					 \n"

		"	INNER_KERNEL4x4_K1						 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		INNER_K			 				 \n"	

		"INNER_Edge_K:							     \n"

		"	INNER_KERNEL4x4_END_K			         \n"		

		"	cmp		x8, #0							 \n"
		"	beq		INNER_SAVE1 					 \n"
		"	INNER_ADD_C 							 \n"
		"INNER_SAVE1: 								 \n"
		"	INNER_SAVE4x4							 \n"

		"	bgt		BEGIN_INNER_M4					 \n"


		"END_INNERP_M4:								 \n"
		"	subs	x20, x20, #1					 \n"
		"	add		x0, x0, #16						 \n"

		"	bgt		BEGIN_INNER_N4					 \n"

		:
		:	
			[C] "m" (C),
			[A] "m" (A),
            [B] "m" (B), 
            [M] "m" (M),
            [N] "m" (N),
            [K] "m" (K),
            [LN] "m" (LN),
            [LK] "m" (LK),
            [SB] "m" (SB),
            [k_tag] "m" (k_tag)
        :   "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
        	"x9", "x10", "x11", "x12", "x13","x14", "x15", "x16",
        	"x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24","x25",
        	"x26", "x27", "x28", "x29", "x30",
            "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
            "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
            "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
            "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"
		);

}

void SGEMM_NT_mp(float *C, float *A, float *B, long M, long N, 
					long K)
{

//	omp_set_num_threads(T);
    long GEMM_K = 320;
    long GEMM_M = 256;
    long GEMM_N = 4096;
    void *ptr;
    int i;

    posix_memalign(&ptr, 64, T * GEMM_K * 16  *sizeof( float ));
    float *SSB = (float *)ptr;

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

    if(Tn > T)
    	Tn = T;

// Determines the number of threads to parallelize the M-dimension
    Tm = T / Tn;

//    Tn =T;
//   Tm=1;

//    #ifdef XXX
//    	printf("KP920\n");
//    #endif
   
    #pragma omp parallel num_threads(T) 
    {
    	long ii, jj, kk;
    	long mb = M / Tm, mc, me = M % Tm;
    	long nb = N / Tn, nc, ne = N % Tn;
    	long kc;

    	int id = omp_get_thread_num();
    	long jjs = (id % Tn) * nb;
    	long iis = (id / Tn) * mb;
    	long kks = 0;
    	long m_to = iis + mb;
    	long n_to = jjs + nb;
    	long k_to = K;
    	
    	// Parallelizing M and N dimensions
		for( jj = jjs; jj < n_to; jj= jj + nc)
		{
			nc = GEMM_N;
    		if(n_to - jj < GEMM_N)
    		{
    			nc = n_to - jj;
    		}

    		for( ii = iis ; ii < m_to; ii = ii + mc)
			{
				mc = GEMM_M;
				if(m_to - ii < GEMM_M)
				{
					mc = m_to - ii;
				}
    			float *CC = C + ii * N + jj;

				/*the kcxnc panel of B reside in L3 cache or memory*/
    			/*the mcxkc block of A reside in L1 cache*/
	    		for(kk= kks; kk < k_to; kk = kk + kc)
	    		{
	    			kc = GEMM_K;
	    			if(k_to - kk < GEMM_K)
						kc = k_to - kk;

					float *AA = A + ii * K + kk;
					float *BB = B + jj * K + kk ;

					// the kernels of irregular-shaped GEMM
				 	SGEMM_NT_KERNEL_MP(CC, AA, BB, mc, nc, 
				 		kc, N, K, &SSB[id * GEMM_K * 16], kk);
	    		}
    		}
		}


// handling edge cases
		if(ne != 0)
		{

			jjs = N - ne;
			n_to = N;

			mb = M / T;
			iis = id * mb;
			m_to = iis + mb;

			for( jj = jjs; jj < n_to; jj= jj + nc)
			{
				nc = GEMM_N;
				if(n_to - jj < GEMM_N)
				{
					nc = n_to - jj;
				}

				for( ii = iis ; ii < m_to; ii = ii + mc)
				{
					mc = GEMM_M;
					if(m_to - ii < GEMM_M)
					{
						mc = m_to - ii;
					}
					float *CC = C + ii * N + jj;

					/*the kcxnc panel of B reside in L3 cache or memory*/
					/*the mcxkc block of A reside in L1 cache*/
		    		for(kk= kks; kk < k_to; kk = kk + kc)
		    		{
		    			kc = GEMM_K;
		    			if(k_to - kk < GEMM_K)
							kc = k_to - kk;

						float *AA = A + ii * K + kk;
						float *BB = B + jj * K + kk ;

						// the edge cases of irregular-shaped GEMM
					 	SGEMM_NT_KERNEL_MP_EDGEN(CC, AA, BB, mc, nc, 
					 		kc, N, K, &SSB[id * GEMM_K * 16], kk);
		    		}
				}
			}


		}

	
	}

    free(SSB);

}
