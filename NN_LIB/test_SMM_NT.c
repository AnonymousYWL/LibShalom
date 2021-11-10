#include <stdio.h>
#include "LibShalom.h"


/* kernels of NT small SGEMM*/
void SGEMM_NT(float *C, float *A, float *B, long M, long N, long K)
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

		"	prfm 	PLDL1KEEP, [x17, #64]			 \n"

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

		"	prfm 	PLDL1KEEP, [x16, #64]			 \n"

		"	ldr		q6, [x17], #16					 \n"

		"	fmla	v12.4s, v0.4s, v5.4s			 \n"
		"	fmla	v13.4s, v1.4s, v5.4s			 \n"

		"	prfm 	PLDL1KEEP, [x17, #64]			 \n"

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

		"	prfm 	PLDL1KEEP, [x18, #64]			 \n"

		"	ldr		q6, [x17], #16					 \n"
		"	fmla	v12.4s, v9.4s, v5.4s 			 \n"
		"	fmla	v13.4s, v10.4s, v5.4s		  	 \n"

		"	prfm  	PLDL1KEEP, [x19, #64]			 \n"
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

		"	ldr		q6, [x17], #16					 \n"
		"	fmla	v12.4s, v9.4s, v5.4s 			 \n"
		"	fmla	v13.4s, v10.4s, v5.4s		  	 \n"

		"	ldr		q7, [x18], #16					 \n"
		"	fmla	v14.4s, v11.4s, v5.4s		 	 \n"
		"	fmla	v15.4s, v0.4s, v5.4s			 \n"
		"	fmla	v16.4s, v1.4s, v5.4s			 \n"

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


		".macro SAVE5x4							 	 \n"

		"	subs	x7, x7, #1					 	 \n"

		"	str		q12, [x25], #16					 \n"
		"	str		q13, [x26], #16					 \n"
		"	str		q14, [x27], #16		 			 \n"
		"	str		q15, [x28], #16		 			 \n"
		"	str		q16, [x29], #16					 \n"

		".endm 									 	 \n"



		".macro SAVE5x16							 \n"


		"	subs	x21, x21, #1					 \n"

		"	stp		q12, q13, [x25]			 		 \n"
		"	stp		q14, q15, [x26]			 	     \n"
		"	stp		q16, q17, [x27]					 \n"
		"	stp		q18, q19, [x28]					 \n"
		"	stp		q20, q21, [x29]					 \n"

//		"	add 	x10, x10, x9					 \n"

		"	add 	x10, x10, x5, lsl #4			 \n"
		"	add		x10, x10, x5, lsl #2 			 \n"

		"	stp		q22, q23, [x25, #32]			 \n"
		"	add		x25, x29, x4, lsl #2			 \n"
		"	stp		q24, q25, [x26, #32]			 \n"
		"	add		x26, x29, x4, lsl #3			 \n"
		"	stp		q26, q27, [x27, #32]			 \n"
		"	add		x27, x25, x4, lsl #3 			 \n"
		"	stp		q28, q29, [x28, #32]			 \n"
		"	add		x28, x26, x4, lsl #3 			 \n"
		"	stp		q30, q31, [x29, #32]			 \n"
		"	add		x29, x27, x4, lsl #3			 \n"

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

		"	fmla	v12.4s, v8.4s, v5.s[3]			 \n"
		"	fmla	v13.4s, v9.4s, v5.s[3]			 \n"

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

//		"	ldr		q5, [x11], #16					 \n"

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

//		"	ldr		q5, [x11], #16					 \n"  //预取

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

//		"	ldr		q6, [x12], #16					 \n"

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

//		"	ldr		q7, [x13], #16					 \n"  //预取

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


		".macro N8_SAVE5x8							 \n"

		"	subs	x21, x21, #1					 \n"

		"	add 	x10, x10, x5, lsl #4			 \n"
		"	add		x10, x10, x5, lsl #2 			 \n"

		"	stp		q12, q13, [x25]			 		 \n"
		"	add		x25, x29, x4, lsl #2			 \n"
		"	stp		q14, q15, [x26]			 	     \n"
		"	add		x26, x25, x4, lsl #2			 \n"
		"	stp		q16, q17, [x27]					 \n"
		"	add		x27, x26, x4, lsl #2			 \n"
		"	stp		q18, q19, [x28]					 \n"
		"	add		x28, x27, x4, lsl #2			 \n"
		"	stp		q20, q21, [x29]					 \n"
		"	add		x29, x28, x4, lsl #2			 \n"

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


		".macro N8_SAVE1x8 							 \n"

		"	stp		q12, q13, [x25]			 		 \n"

		".endm 										 \n"




//----------------------------------------------------

		"SMM:									 	 \n"

		"	ldr		x0, %[C]						 \n"
		"	ldr		x1, %[A]						 \n"
		"	ldr		x2, %[B]						 \n"

		"	ldr		x3, %[M]						 \n"
		"	ldr		x4, %[N]						 \n"
		"	ldr		x5, %[K]						 \n"

		"	mov		x23, #5						 	 \n"

		"   prfm    PLDL1KEEP, [x1]             	 \n"
		"   prfm    PLDL1KEEP, [x2]             	 \n"

		"	udiv	x30, x3, x23				 	 \n"	// M / 5
		"	lsr		x20, x4, #4					 	 \n"	// N / 16

		"	lsl		x6, x5, #6 						 \n"
		"	sub 	sp, sp, x6   				 	 \n"

		"	msub	x23, x30, x23, x3 				 \n"	// M % 5
		"	mov		x6, x23 						 \n"
//		"	mov 	x23, x19 						 \n"    //SB
		"	mov 	x23, sp 						 \n"

		"	cmp		x20, #0							 \n"
		"	beq		BEGIN_N8						 \n"



//-----------------------------------------------N16
		"BEGIN_N16:								 	 \n"

		"	mov 	x25, x0   						 \n"   //C1*
		"	add		x26, x25, x4, lsl #2 			 \n"   //C2*
		"	add 	x27, x25, x4, lsl #3 			 \n"   //C3*
		" 	add 	x28, x26, x4, lsl #3 			 \n"   //C4*
		"	add		x29, x27, x4, lsl #3 			 \n"   //C5*

		"	mov		x10, x1   						 \n"
		"	mov		x21, x30  						 \n"
		"	mov		x24, x23						 \n"   //还原SB的地址

		"	mov		x7, #4							 \n"   //4次B的循环
		"	mov		x8, #16							 \n"

		"BEGIN_PACKB:								 \n"

		"	mov		x16, x2						 	 \n"   //B0*
		"	add		x17, x16, x5, lsl #2 			 \n"
		"	add		x18, x17, x5, lsl #2 			 \n"
		"	add		x19, x18, x5, lsl #2 			 \n"

		"	mov		x11, x10						 \n"    //A0*
		"	add		x12, x11, x5, lsl #2			 \n"    //A1*
		"	add		x13, x12, x5, lsl #2			 \n"	//A2*
		"	add		x14, x13, x5, lsl #2			 \n"	//A3*
		"	add		x15, x14, x5, lsl #2 			 \n"	//A4*

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


		"	prfm	PLDL1KEEP, [x25, #64]			 \n"
		"	prfm	PLDL1KEEP, [x26, #64]			 \n"
		"	prfm	PLDL1KEEP, [x27, #64]			 \n"
		"	prfm	PLDL1KEEP, [x28, #64]			 \n"
		"	prfm	PLDL1KEEP, [x29, #64]			 \n"

		"	PACK_KERNEL5x4_END_K			         \n"		

		"	SAVE5x4								 	 \n"

//		"	beq		END_PACKB						 \n"

		"	add		x23, x23, #16					 \n"
		"	mov		x24, x23 						 \n"
		"	add		x2, x2, x5, lsl #4				 \n"

		"	bgt  	BEGIN_PACKB					 	 \n"

		"END_PACKB:									 \n"

		"	sub		x23, x23, x8, lsl #2			 \n"    //还原SB的索引

		"	sub 	x29, x29, x8, lsl #2			 \n"    //C矩阵的偏移量还原
		"	add		x25, x29, x4, lsl #2			 \n"
		"	add		x26, x25, x4, lsl #2			 \n"
		"	add		x27, x26, x4, lsl #2			 \n"
		"	add		x28, x27, x4, lsl #2			 \n"
		"	add		x29, x28, x4, lsl #2 			 \n"

		"	add		x10, x10, x5, lsl #4			 \n"  
		"	add 	x10, x10, x5, lsl #2 			 \n"	// A + 5行

		"	cmp		x8, #16							 \n"
		"	bne 	N8_END_PACKB					 \n"

		"	subs	x21, x21, #1					 \n"
		"	beq		END_M5							 \n"


		//---------------------------------------------------

		"BEGIN_M5:									 \n"

		"	mov		x24, x23	     				 \n"

		"	mov		x11, x10						 \n"
		"	add		x12, x11, x5, lsl #2			 \n"
		"	add		x13, x12, x5, lsl #2			 \n"
		"	add		x14, x13, x5, lsl #2			 \n"
		"	add		x15, x14, x5, lsl #2 			 \n"

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


		"	prfm	PLDL1KEEP, [x25, #64]			 \n"
		"	prfm	PLDL1KEEP, [x26, #64]			 \n"
		"	prfm	PLDL1KEEP, [x27, #64]			 \n"
		"	prfm	PLDL1KEEP, [x28, #64]			 \n"
		"	prfm	PLDL1KEEP, [x29, #64]			 \n"

		"	KERNEL5x16_END_K						 \n"		

		"	SAVE5x16								 \n"
		"	bgt   	BEGIN_M5						 \n"

		"END_M5:									 \n"


		"	mov		x24, x23	     				 \n"  //B0*

		"	cmp		x6, #4							 \n"
		"	bne 	BEGIN_M3 						 \n"



	//--------------------------------------------------------
		"BEGIN_M4:						 			 \n"

		"	mov		x11, x10 						 \n"
		"	add		x12, x11, x5, lsl #2 		     \n"
		"	add		x13, x12, x5, lsl #2 			 \n"
		"	add		x14, x13, x5, lsl #2 			 \n"

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
		"	SAVE4x16								 \n"

		"	b 		END_M 							 \n"


		//--------------------------------------------------------

		"BEGIN_M3:			 						 \n"
		"	cmp		x6, #3							 \n"
		"	bne 	BEGIN_M2 						 \n"

		"	mov		x11, x10 						 \n"
		"	add		x12, x11, x5, lsl #2 			 \n"
		"	add		x13, x12, x5, lsl #2 			 \n"

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
		"	SAVE3x16								 \n"
		"	b 		END_M 							 \n"




//----------------------------------------------------------
		"BEGIN_M2:			 						 \n"

		"	cmp		x6, #2							 \n"
		"	bne 	BEGIN_M1 						 \n"

		"	mov		x11, x10 						 \n"
		"	add		x12, x11, x5, lsl #2 			 \n"

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
		"	SAVE2x16								 \n"
		"	b 		END_M 							 \n"




//---------------------------------------------------------		

		"BEGIN_M1:									 \n"

		"	cmp		x6, #1							 \n"
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
		"	SAVE1x16								 \n"

		"END_M:										 \n"


		"	subs	x20, x20, #1					 \n"

		"	add		x16, x16, #64					 \n"
		"	add		x0, x0, #64						 \n"

		"	bgt		BEGIN_N16						 \n"

		"END_N16:									 \n"

		"	ands	x20, x4, #15					 \n"
		"	beq		END_N 							 \n"


		

		"BEGIN_N8:									\n"


//----------------------------------------------------------N8
		"N8_BEGIN_PACKB:							 \n"

		"	mov 	x25, x0   						 \n"   //C1*
		"	add		x26, x25, x4, lsl #2 			 \n"   //C2*
		"	add 	x27, x25, x4, lsl #3 			 \n"   //C3*
		" 	add 	x28, x26, x4, lsl #3 			 \n"   //C4*
		"	add		x29, x27, x4, lsl #3 			 \n"   //C5*

		"	mov		x10, x1   						 \n"   //A0*
		"	mov		x21, x30  						 \n"   // M / 5
		"	mov		x7, #2							 \n"
		"	mov		x24, x23						 \n"

		"	mov		x8, #8 							 \n"

		"	b 		BEGIN_PACKB 					 \n"

		"N8_END_PACKB:								 \n"

		"	subs	x21, x21, #1					 \n"
		"	beq		N8_END_M5						 \n"

//		"	add		x10, x10, x5, lsl #4			 \n"  
//		"	add 	x10, x10, x5, lsl #2 			 \n"	// A + 5行

//		"	sub		x23, x23, #32					 \n"    //还原SB的索引

//		"	sub 	x29, x29, #32					 \n"    //C矩阵的偏移量还原
//		"	add		x25, x29, x6					 \n"
//		"	add		x26, x25, x6					 \n"
//		"	add		x27, x26, x6					 \n"
//		"	add		x28, x27, x6					 \n"
//		"	add		x29, x28, x6 					 \n"


//--------------------------------------------------N8M5
		"N8_BEGIN_M5:								 \n"

		"	mov		x24, x23	     				 \n"

		"	mov		x11, x10						 \n"
		"	add		x12, x11, x5, lsl #2			 \n"
		"	add		x13, x12, x5, lsl #2			 \n"
		"	add		x14, x13, x5, lsl #2			 \n"
		"	add		x15, x14, x5, lsl #2 			 \n"

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

		"	N8_SAVE5x8								 \n"
		"	bgt   	N8_BEGIN_M5					 	 \n"





//---------------------------------------------------------

		"N8_END_M5:									 \n"

		"	mov		x24, x23	     				 \n"  //B0*

		"	cmp		x6, #4							 \n"
		"	bne 	N8_BEGIN_M3 					 \n"

	//--------------------------------------------------------
		"N8_BEGIN_M4:						 		 \n"

		"	mov		x11, x10 						 \n"
		"	add		x12, x11, x5, lsl #2 			 \n"
		"	add		x13, x12, x5, lsl #2 			 \n"
		"	add		x14, x13, x5, lsl #2 			 \n"

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
		"	N8_SAVE4x8								 \n"

		"	b 		N8_END_M 						 \n"


//------------------------------------------------------------------

		"N8_BEGIN_M3: 								 \n"

		"	cmp		x6, #3							 \n"
		"	bne 	N8_BEGIN_M2 					 \n"

		"	mov		x11, x10 						 \n"
		"	add		x12, x11, x5, lsl #2 			 \n"
		"	add		x13, x12, x5, lsl #2 			 \n"

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
		"	N8_SAVE3x8								 \n"

		"	b 		N8_END_M 						 \n"





//--------------------------------------------------------

		"N8_BEGIN_M2:								 \n"


		"	cmp		x6, #2							 \n"
		"	bne 	N8_BEGIN_M1 					 \n"

		"	mov		x11, x10 						 \n"
		"	add		x12, x11, x5, lsl #2 			 \n"

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
		"	N8_SAVE2x8								 \n"

		"	b 		N8_END_M 						 \n"





//-----------------------------------------------------
		"N8_BEGIN_M1:								 \n"


		"	cmp		x6, #1							 \n"
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
		"	N8_SAVE1x8								 \n"

		"N8_END_M: 									 \n"

		"END_N:									 	 \n"

		"	lsl		x6, x5, #6 						 \n"
		"	add 	sp, sp, x6   				 	 \n"
		
		:
		:	
			[C] "m" (C),
			[A] "m" (A),
            [B] "m" (B), 
            [M] "m" (M),
            [N] "m" (N),
            [K] "m" (K)
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

