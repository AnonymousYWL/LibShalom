#include <stdio.h>
#include "LibShalom.h"

void SGEMM_NN_L1(float *C, float *A, float *B, long M, long N, 
	long K)
{
	asm volatile(
		".macro KERNEL5x16_BEGIN_K             		 		\n"
		"										 	 												\n"

		"	ldr		q0, [x11], #16					 							\n"
		"	ldr		q1, [x12], #16					 							\n"
		"	ldp		q8, q9, [x17], #32			     					\n"

		" prfm  PLDL1KEEP, [x11, #64]       	 				\n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 					\n"
		" prfm  PLDL1KEEP, [x13, #64]       	 				\n"
		"	prfm	PLDL1KEEP, [x14, #64] 			 					\n"
		"	prfm	PLDL1KEEP, [x15, #64]			 						\n"

		"	fmul	v12.4s, v8.4s, v0.s[0]	         			\n"
		"	fmul	v13.4s, v9.4s, v0.s[0]	         			\n"

		"	ldr		q2, [x13], #16					 							\n"

		"	fmul	v14.4s, v8.4s, v1.s[0]	         			\n"
		"	fmul	v15.4s, v9.4s, v1.s[0]	         			\n"

		"	ldr		q3, [x14], #16					 							\n"

		"	fmul	v16.4s, v8.4s, v2.s[0]	         			\n"
		"	fmul	v17.4s, v9.4s, v2.s[0]	         			\n"

		"	prfm	PLDL1KEEP, [x18, #64]			 						\n"

		"	ldr		q4, [x15], #16				 	 							\n"

		"	fmul	v18.4s, v8.4s, v3.s[0]	         			\n"
		"	fmul	v19.4s, v9.4s, v3.s[0]	         			\n"

		"	ldp		q10, q11, [x17], #-32			 						\n"

		"	fmul	v20.4s, v8.4s, v4.s[0]			 					\n"
		"	ldr		q8, [x18]						 									\n"
		"	fmul	v21.4s, v9.4s, v4.s[0]			 					\n"
		"	ldr		q9, [x18, #16]					 							\n"

		"	fmul	v22.4s, v10.4s, v0.s[0]			 					\n"
		"	fmul	v23.4s, v11.4s, v0.s[0]			 					\n"

		"	add		x17, x17, x6, lsl #1 					 				\n"

		"	fmul	v24.4s, v10.4s, v1.s[0]	         			\n"
		"	fmul	v25.4s, v11.4s, v1.s[0]	         			\n"

		" prfm    PLDL1KEEP, [x17, #64]       	 			\n"

		"	fmul	v26.4s, v10.4s, v2.s[0]	         			\n"
		"	fmul	v27.4s, v11.4s, v2.s[0]	         			\n"

		"	ldr		q5, [x11], #16					 							\n"

		"	fmul	v28.4s, v10.4s, v3.s[0]	         			\n"
		"	fmul	v29.4s, v11.4s, v3.s[0]	         			\n"

		"	fmul	v30.4s, v10.4s, v4.s[0]	         			\n"
		"	ldr		q10, [x18, #32]					 							\n"
		"	fmul	v31.4s, v11.4s, v4.s[0]	         			\n"
		"	ldr		q11, [x18, #48]					 							\n"

		"	add		x18, x18, x6, lsl #1 				 					\n"

		".endm									 	 										\n"


		".macro KERNEL5x16_K0   	                 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"

		"	fmla	v12.4s, v8.4s, v0.s[0]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[0]			 \n"

		"   prfm    PLDL1KEEP, [x18, #64]       	 \n"

		"	fmla	v14.4s, v8.4s, v1.s[0]			 \n"
		"	fmla 	v15.4s, v9.4s, v1.s[0]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[0]			 \n"
		"	fmla 	v17.4s, v9.4s, v2.s[0]			 \n"

		"	fmla	v18.4s, v8.4s, v3.s[0]			 \n"
		"	fmla 	v19.4s, v9.4s, v3.s[0]			 \n"

		"	fmla	v20.4s, v8.4s, v4.s[0]			 \n"
		"	ldr		q8, [x18]						 \n"
		"	fmla	v21.4s, v9.4s, v4.s[0]			 \n"
		"	ldr		q9, [x18, #16]					 \n"

		"	fmla	v22.4s, v10.4s, v0.s[0]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[0]			 \n"

		"	fmla	v24.4s, v10.4s, v1.s[0]			 \n"
		"	fmla	v25.4s, v11.4s, v1.s[0]			 \n"

		"	fmla	v26.4s, v10.4s, v2.s[0]			 \n"
		"	fmla	v27.4s, v11.4s, v2.s[0]			 \n"

		"	ldr		q5, [x11], #16					 \n"  // 

		"	fmla	v28.4s, v10.4s, v3.s[0]			 \n"
		"	fmla	v29.4s, v11.4s, v3.s[0]			 \n"

		"	fmla	v30.4s, v10.4s, v4.s[0]			 \n"
		"	ldr		q10, [x18, #32]					 \n"
		"	fmla	v31.4s, v11.4s, v4.s[0]			 \n"
		"	ldr		q11, [x18, #48]					 \n"

		"	add 	x18, x18, x6, lsl #1 			 \n"

		".endm   									 \n"


		".macro KERNEL5x16_K1	                     \n"

		"   prfm    PLDL1KEEP, [x12, #64]       	 \n"
		"	fmla	v12.4s, v8.4s, v0.s[1]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[1]			 \n"

		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	fmla	v14.4s, v8.4s, v1.s[1]			 \n"
		"	fmla	v15.4s, v9.4s, v1.s[1]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[1]			 \n"
		"	fmla	v17.4s, v9.4s, v2.s[1]			 \n"

		"	fmla	v18.4s, v8.4s, v3.s[1]			 \n"
		"	fmla	v19.4s, v9.4s, v3.s[1]			 \n"

		"	fmla	v20.4s, v8.4s, v4.s[1]			 \n"
		"	ldr		q8, [x17]						 \n"
		"	fmla	v21.4s, v9.4s, v4.s[1]			 \n"
		"	ldr		q9, [x17, #16]					 \n"

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
		"	ldr		q10, [x17, #32]					 \n"
		"	fmla	v31.4s, v11.4s, v4.s[1]			 \n"
		"	ldr		q11, [x17, #48]					 \n"

		"	add		x17, x17, x6, lsl #1 			 \n"

		".endm									 	 \n"


		".macro KERNEL5x16_K2	                     \n"

		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"

		"	fmla	v12.4s, v8.4s, v0.s[2]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[2]			 \n"

		"   prfm    PLDL1KEEP, [x18, #64]       	 \n"

		"	fmla	v14.4s, v8.4s, v1.s[2]			 \n"
		"	fmla 	v15.4s, v9.4s, v1.s[2]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[2]			 \n"
		"	fmla 	v17.4s, v9.4s, v2.s[2]			 \n"

		"	fmla	v18.4s, v8.4s, v3.s[2]			 \n"
		"	fmla 	v19.4s, v9.4s, v3.s[2]			 \n"

		"	fmla	v20.4s, v8.4s, v4.s[2]			 \n"
		"	ldr		q8, [x18]						 \n"
		"	fmla	v21.4s, v9.4s, v4.s[2]			 \n"
		"	ldr		q9, [x18, #16]					 \n"

		"	fmla	v22.4s, v10.4s, v0.s[2]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[2]			 \n"

		"	prfm	PLDL1KEEP, [x14, #64]			 \n"

		"	fmla	v24.4s, v10.4s, v1.s[2]			 \n"
		"	fmla	v25.4s, v11.4s, v1.s[2]			 \n"

		"	fmla	v26.4s, v10.4s, v2.s[2]			 \n"
		"	fmla	v27.4s, v11.4s, v2.s[2]			 \n"

		"	ldr		q7, [x13], #16					 \n"  // 

		"	fmla	v28.4s, v10.4s, v3.s[2]			 \n"
		"	fmla	v29.4s, v11.4s, v3.s[2]			 \n"

		"	fmla	v30.4s, v10.4s, v4.s[2]			 \n"
		"	ldr		q10, [x18, #32]					 \n"
		"	fmla	v31.4s, v11.4s, v4.s[2]			 \n"
		"	ldr		q11, [x18, #48]					 \n"

		"	add 	x18, x18, x6, lsl #1 			 \n"

		".endm									 	 \n"


		".macro KERNEL5x16_K3       	             \n"

		"   prfm    PLDL1KEEP, [x15, #64]       	 \n"
		"	fmla	v12.4s, v8.4s, v0.s[3]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[3]			 \n"

		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	fmla	v14.4s, v8.4s, v1.s[3]			 \n"
		"	fmla	v15.4s, v9.4s, v1.s[3]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[3]			 \n"
		"	fmla	v17.4s, v9.4s, v2.s[3]			 \n"

		"	fmla	v18.4s, v8.4s, v3.s[3]			 \n"
		"	fmla	v19.4s, v9.4s, v3.s[3]			 \n"

		"	fmla	v20.4s, v8.4s, v4.s[3]			 \n"
		"	ldr		q8, [x17]						 \n"
		"	fmla	v21.4s, v9.4s, v4.s[3]			 \n"
		"	ldr		q9, [x17, #16]					 \n"

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
		"	ldr		q10, [x17, #32]				 	 \n"
		"	fmla	v31.4s, v11.4s, v4.s[3]			 \n"
		"	ldr		q11, [x17, #48]					 \n"

		"	add		x17, x17, x6, lsl #1 			 \n"

		".endm									 	 \n"


		".macro KERNEL5x16_K4	                     \n"

		"	prfm	PLDL1KEEP, [x13, #64]			 \n"

		"	fmla	v12.4s, v8.4s, v5.s[0]			 \n"
		"	fmla	v13.4s, v9.4s, v5.s[0]			 \n"

		"   prfm    PLDL1KEEP, [x18, #64]       	 \n"

		"	fmla	v14.4s, v8.4s, v6.s[0]			 \n"
		"	fmla 	v15.4s, v9.4s, v6.s[0]			 \n"

		"	fmla	v16.4s, v8.4s, v7.s[0]			 \n"
		"	fmla 	v17.4s, v9.4s, v7.s[0]			 \n"

		"	fmla	v18.4s, v8.4s, v0.s[0]			 \n"
		"	fmla 	v19.4s, v9.4s, v0.s[0]			 \n"

		"	fmla	v20.4s, v8.4s, v1.s[0]			 \n"
		"	ldr		q8, [x18]						 \n"
		"	fmla	v21.4s, v9.4s, v1.s[0]			 \n"
		"	ldr		q9, [x18, #16]					 \n"

		"	fmla	v22.4s, v10.4s, v5.s[0]			 \n"
		"	fmla	v23.4s, v11.4s, v5.s[0]			 \n"

		"	fmla	v24.4s, v10.4s, v6.s[0]			 \n"
		"	fmla	v25.4s, v11.4s, v6.s[0]			 \n"

		"	fmla	v26.4s, v10.4s, v7.s[0]			 \n"
		"	fmla	v27.4s, v11.4s, v7.s[0]			 \n"

		"	ldr		q2, [x13], #16					 \n"  // 

		"	fmla	v28.4s, v10.4s, v0.s[0]			 \n"
		"	fmla	v29.4s, v11.4s, v0.s[0]			 \n"

		"	fmla	v30.4s, v10.4s, v1.s[0]			 \n"
		"	ldr		q10, [x18, #32]					 \n"
		"	fmla	v31.4s, v11.4s, v1.s[0]			 \n"
		"	ldr		q11, [x18, #48]					 \n"

		"	add 	x18, x18, x6, lsl #1 			 \n"


		".endm 										 \n"


		".macro KERNEL5x16_K5	                     \n"

		"   prfm    PLDL1KEEP, [x14, #64]       	 \n"
		"	fmla	v12.4s, v8.4s, v5.s[1]			 \n"
		"	fmla	v13.4s, v9.4s, v5.s[1]			 \n"

		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	fmla	v14.4s, v8.4s, v6.s[1]			 \n"
		"	fmla	v15.4s, v9.4s, v6.s[1]			 \n"

		"	fmla	v16.4s, v8.4s, v7.s[1]			 \n"
		"	fmla	v17.4s, v9.4s, v7.s[1]			 \n"

		"	fmla	v18.4s, v8.4s, v0.s[1]			 \n"
		"	fmla	v19.4s, v9.4s, v0.s[1]			 \n"

		"	fmla	v20.4s, v8.4s, v1.s[1]			 \n"
		"	ldr		q8, [x17]					 	 \n"
		"	fmla	v21.4s, v9.4s, v1.s[1]			 \n"
		"	ldr		q9, [x17, #16]					 \n"

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
		"	ldr		q10, [x17, #32]					 \n"
		"	fmla	v31.4s, v11.4s, v1.s[1]			 \n"
		"	ldr		q11, [x17, #48]					 \n"

		"	add		x17, x17, x6, lsl #1 			 \n"

		".endm 										 						\n"


		".macro KERNEL5x16_K6     	         \n"

		"	prfm	PLDL1KEEP, [x15, #64]			 		\n"

		"	fmla	v12.4s, v8.4s, v5.s[2]			 \n"
		"	fmla	v13.4s, v9.4s, v5.s[2]			 \n"

		"   prfm    PLDL1KEEP, [x18, #64]    \n"

		"	fmla	v14.4s, v8.4s, v6.s[2]			 \n"
		"	fmla 	v15.4s, v9.4s, v6.s[2]			 \n"

		"	fmla	v16.4s, v8.4s, v7.s[2]			 \n"
		"	fmla 	v17.4s, v9.4s, v7.s[2]			 \n"

		"	fmla	v18.4s, v8.4s, v0.s[2]			 \n"
		"	fmla 	v19.4s, v9.4s, v0.s[2]			 \n"

		"	fmla	v20.4s, v8.4s, v1.s[2]			 \n"
		"	ldr		q8, [x18]						 					\n"
		"	fmla	v21.4s, v9.4s, v1.s[2]			 \n"
		"	ldr		q9, [x18, #16]					 		\n"

		"	fmla	v22.4s, v10.4s, v5.s[2]			 \n"
		"	fmla	v23.4s, v11.4s, v5.s[2]			 \n"

		"	fmla	v24.4s, v10.4s, v6.s[2]			 \n"
		"	fmla	v25.4s, v11.4s, v6.s[2]			 \n"

		"	prfm	PLDL1KEEP, [x11, #64]			 \n"

		"	fmla	v26.4s, v10.4s, v7.s[2]			 \n"
		"	fmla	v27.4s, v11.4s, v7.s[2]			 \n"

		"	ldr		q4, [x15], #16					 			\n"  // 

		"	fmla	v28.4s, v10.4s, v0.s[2]			 \n"
		"	fmla	v29.4s, v11.4s, v0.s[2]			 \n"

		"	fmla	v30.4s, v10.4s, v1.s[2]			 \n"
		"	ldr		q10, [x18, #32]					 			\n"
		"	fmla	v31.4s, v11.4s, v1.s[2]			 \n"
		"	ldr		q11, [x18, #48]					 			\n"

		"	add 	x18, x18, x6, lsl #1 			 		\n"

		".endm 										 					 \n"


		".macro KERNEL5x16_K7    	           \n"

		"   prfm    PLDL1KEEP, [x12, #64]    \n"
		"	fmla	v12.4s, v8.4s, v5.s[3]			 \n"
		"	fmla	v13.4s, v9.4s, v5.s[3]			 \n"

		"   prfm    PLDL1KEEP, [x17, #64]    \n"

		"	fmla	v14.4s, v8.4s, v6.s[3]			 \n"
		"	fmla	v15.4s, v9.4s, v6.s[3]			 \n"

		"	fmla	v16.4s, v8.4s, v7.s[3]			 \n"
		"	fmla	v17.4s, v9.4s, v7.s[3]			 \n"

		"	fmla	v18.4s, v8.4s, v0.s[3]			 \n"
		"	fmla	v19.4s, v9.4s, v0.s[3]			 \n"

		"	fmla	v20.4s, v8.4s, v1.s[3]			 \n"
		"	ldr		q8, [x17]			     		 			 \n"
		"	fmla	v21.4s, v9.4s, v1.s[3]			 \n"
		"	ldr		q9, [x17, #16]					 		 \n"

		"	fmla	v28.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v29.4s, v11.4s, v0.s[3]			 \n"

		"	ldr		q0, [x11], #16					 		 \n"

		"	fmla	v30.4s, v10.4s, v1.s[3]			 \n"
		"	fmla	v31.4s, v11.4s, v1.s[3]			 \n"

		"	ldr		q1, [x12], #16					 			\n"

		"	fmla	v22.4s, v10.4s, v5.s[3]			 \n"
		"	fmla	v23.4s, v11.4s, v5.s[3]			 \n"

		"	fmla	v24.4s, v10.4s, v6.s[3]			 \n"
		"	fmla	v25.4s, v11.4s, v6.s[3]			 \n"

		"	fmla	v26.4s, v10.4s, v7.s[3]			 \n"
		"	ldr		q10, [x17, #32]					 			\n"
		"	fmla	v27.4s, v11.4s, v7.s[3]			 \n"
		"	ldr		q11, [x17, #48]					 			\n"

		"	add		x17, x17, x6, lsl #1 			 	\n"

		".endm 											 				 \n"


		".macro KERNEL5x16_END_K 	           \n"

		"	fmla	v12.4s, v8.4s, v5.s[3]			 \n"
		"	fmla	v13.4s, v9.4s, v5.s[3]			 \n"

		"	sub  	x13, x13, #16 							 \n"

		"	fmla	v14.4s, v8.4s, v6.s[3]			 \n"
		"	fmla	v15.4s, v9.4s, v6.s[3]			 \n"

		"	sub  	x14, x14, #16 							 \n"

		"	fmla	v16.4s, v8.4s, v7.s[3]			 \n"
		"	fmla	v17.4s, v9.4s, v7.s[3]			 \n"

		"	sub  	x15, x15, #16 							 \n"

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

		".endm 										 					\n"


		".macro KERNEL5x16_EDGE_K 	         \n"

		"	ldp		q8, q9, [x17] 							 \n"

		"	ld1		{v5.s}[3], [x11], #4 			 	 \n"	
		"	fmla	v12.4s, v8.4s, v5.s[3]			 \n"
		"	ld1		{v6.s}[3], [x12], #4 			 	 \n"
		"	fmla	v13.4s, v9.4s, v5.s[3]			 \n"
		"	ld1		{v7.s}[3], [x13], #4 			 	 \n"
		"	fmla	v14.4s, v8.4s, v6.s[3]			 \n"
		"	ld1		{v0.s}[3], [x14], #4 			 	 \n"
		"	fmla	v15.4s, v9.4s, v6.s[3]			 \n"
		"	ld1		{v1.s}[3], [x15], #4 			 	 \n"
		"	fmla	v16.4s, v8.4s, v7.s[3]			 \n"
		"	fmla	v17.4s, v9.4s, v7.s[3]			 \n"

		"	ldp		q10, q11, [x17, #32] 				 \n"
		"	fmla	v18.4s, v8.4s, v0.s[3]			 \n"
		"	fmla	v19.4s, v9.4s, v0.s[3]			 \n"

		"	fmla	v20.4s, v8.4s, v1.s[3]			 \n"
		"	fmla	v21.4s, v9.4s, v1.s[3]			 \n"

		"	fmla	v28.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v29.4s, v11.4s, v0.s[3]			 \n"

		"	add		x17, x17, x6			 					 \n"

		"	fmla	v30.4s, v10.4s, v1.s[3]			 \n"
		"	fmla	v31.4s, v11.4s, v1.s[3]			 \n"

		"	prfm	PLDL1KEEP, [x17, #64] 			 \n"

		"	fmla	v22.4s, v10.4s, v5.s[3]			 \n"
		"	fmla	v23.4s, v11.4s, v5.s[3]			 \n"

		"	fmla	v24.4s, v10.4s, v6.s[3]			 \n"
		"	fmla	v25.4s, v11.4s, v6.s[3]			 \n"

		"	fmla	v26.4s, v10.4s, v7.s[3]			 \n"
		"	fmla	v27.4s, v11.4s, v7.s[3]			 \n"

		".endm 										 					\n"


		".macro SAVE5x16							 		 \n"

		"	subs	x21, x21, #1					 		 \n"

		"	stp		q12, q13, [x25]			 		 	 \n"
		"	stp		q14, q15, [x26]			 	     \n"
		"	stp		q16, q17, [x27]					 	 \n"
		"	stp		q18, q19, [x28]					 	 \n"
		"	stp		q20, q21, [x29]					 	 \n"

		"	add 	x10, x10, x8, lsl #2			 \n"
		"	add		x10, x10, x8 					 		 \n"

		"	stp		q22, q23, [x25, #32]			 \n"
		"	add		x25, x29, x6					 		 \n"
		"	stp		q24, q25, [x26, #32]			 \n"
		"	add		x26, x29, x6, lsl #1			 \n"
		"	stp		q26, q27, [x27, #32]			 \n"
		"	add		x27, x25, x6, lsl #1 			 \n"
		"	stp		q28, q29, [x28, #32]			 \n"
		"	add		x28, x26, x6, lsl #1 			 \n"
		"	stp		q30, q31, [x29, #32]			 \n"
		"	add		x29, x27, x6, lsl #1			 \n"

		".endm 									 	 				 \n"


		".macro	KERNEL4x16_BEGIN_K					 \n"

		"										 	 \n"
		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q1, [x12], #16					 \n"
		"	ldp		q8, q9, [x17], #32			     \n"

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

		"	prfm	PLDL1KEEP, [x18, #64]			 \n"

		"	ldp		q10, q11, [x17], #-32			 \n"

		"	fmul	v18.4s, v8.4s, v3.s[0]	         \n"
		"	fmul	v19.4s, v9.4s, v3.s[0]	         \n"

//		"	fmul	v20.4s, v8.4s, v4.s[0]			 \n"
		
//		"	fmul	v21.4s, v9.4s, v4.s[0]			 \n"
		
		"	fmul	v22.4s, v10.4s, v0.s[0]			 \n"
		"	ldr		q8, [x18]						 \n"
		"	fmul	v23.4s, v11.4s, v0.s[0]			 \n"
		"	ldr		q9, [x18, #16]					 \n"

		"	add		x17, x17, x6, lsl #1 			 \n"

		"	fmul	v24.4s, v10.4s, v1.s[0]	         \n"
		"	fmul	v25.4s, v11.4s, v1.s[0]	         \n"

		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	fmul	v26.4s, v10.4s, v2.s[0]	         \n"
		"	fmul	v27.4s, v11.4s, v2.s[0]	         \n"

//		"	ldr		q5, [x11], #16					 \n"

		"	fmul	v28.4s, v10.4s, v3.s[0]	         \n"
		"	ldr		q10, [x18, #32]					 \n"
		"	fmul	v29.4s, v11.4s, v3.s[0]	         \n"		
		"	ldr		q11, [x18, #48]					 \n"

		"	add		x18, x18, x6, lsl #1 			 \n"

		".endm 										 \n"


		".macro KERNEL4x16_K0                   	 \n"

		"   prfm    PLDL1KEEP, [x12, #64]       	 \n"
		"	fmla	v12.4s, v8.4s, v0.s[0]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[0]			 \n"

		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	fmla	v14.4s, v8.4s, v1.s[0]			 \n"
		"	fmla	v15.4s, v9.4s, v1.s[0]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[0]			 \n"
		"	fmla	v17.4s, v9.4s, v2.s[0]			 \n"

		"	fmla	v18.4s, v8.4s, v3.s[0]			 \n"
		"	ldr		q8, [x18]						 \n"
		"	fmla	v19.4s, v9.4s, v3.s[0]			 \n"
		"	ldr		q9, [x18, #16]					 \n"

		"	fmla	v22.4s, v10.4s, v0.s[0]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[0]			 \n"

		"	fmla	v24.4s, v10.4s, v1.s[0]			 \n"
		"	fmla	v25.4s, v11.4s, v1.s[0]			 \n"

		"	fmla	v26.4s, v10.4s, v2.s[0]			 \n"
		"	fmla	v27.4s, v11.4s, v2.s[0]			 \n"

//		"	ldr		q6, [x12], #16					 \n"

		"	fmla	v28.4s, v10.4s, v3.s[0]			 \n"
		"	ldr		q10, [x18, #32]					 \n"
		"	fmla	v29.4s, v11.4s, v3.s[0]			 \n"
		"	ldr		q11, [x18, #48]					 \n"

		"	add		x18, x18, x6, lsl #1 			 \n"


		".endm   									 \n"


		".macro KERNEL4x16_K1        	             \n"

		"   prfm    PLDL1KEEP, [x12, #64]       	 \n"
		"	fmla	v12.4s, v8.4s, v0.s[1]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[1]			 \n"

		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	fmla	v14.4s, v8.4s, v1.s[1]			 \n"
		"	fmla	v15.4s, v9.4s, v1.s[1]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[1]			 \n"
		"	fmla	v17.4s, v9.4s, v2.s[1]			 \n"

		"	fmla	v18.4s, v8.4s, v3.s[1]			 \n"
		"	ldr		q8, [x17]						 \n"
		"	fmla	v19.4s, v9.4s, v3.s[1]			 \n"
		"	ldr		q9, [x17, #16]					 \n"

		"	fmla	v22.4s, v10.4s, v0.s[1]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[1]			 \n"

		"	fmla	v24.4s, v10.4s, v1.s[1]			 \n"
		"	fmla	v25.4s, v11.4s, v1.s[1]			 \n"

		"	fmla	v26.4s, v10.4s, v2.s[1]			 \n"
		"	fmla	v27.4s, v11.4s, v2.s[1]			 \n"

//		"	ldr		q6, [x12], #16					 \n"

		"	fmla	v28.4s, v10.4s, v3.s[1]			 \n"
		"	ldr		q10, [x17, #32]					 \n"
		"	fmla	v29.4s, v11.4s, v3.s[1]			 \n"
		"	ldr		q11, [x17, #48]					 \n"

		"	add		x17, x17, x6, lsl #1 			 \n"

		".endm									 	 \n"


		".macro KERNEL4x16_K2          		         \n"

		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"

		"	fmla	v12.4s, v8.4s, v0.s[2]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[2]			 \n"

		"   prfm    PLDL1KEEP, [x18, #64]       	 \n"

		"	fmla	v14.4s, v8.4s, v1.s[2]			 \n"
		"	fmla 	v15.4s, v9.4s, v1.s[2]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[2]			 \n"
		"	fmla 	v17.4s, v9.4s, v2.s[2]			 \n"

		"	fmla	v18.4s, v8.4s, v3.s[2]			 \n"
		"	ldr		q8, [x18]						 \n"
		"	fmla 	v19.4s, v9.4s, v3.s[2]			 \n"
		"	ldr		q9, [x18, #16]					 \n"

		"	fmla	v22.4s, v10.4s, v0.s[2]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[2]			 \n"

		"	prfm	PLDL1KEEP, [x14, #64]			 \n"

		"	fmla	v24.4s, v10.4s, v1.s[2]			 \n"
		"	fmla	v25.4s, v11.4s, v1.s[2]			 \n"

		"	fmla	v26.4s, v10.4s, v2.s[2]			 \n"
		"	fmla	v27.4s, v11.4s, v2.s[2]			 \n"

//		"	ldr		q7, [x13], #16					 \n"  // 

		"	fmla	v28.4s, v10.4s, v3.s[2]			 \n"
		"	ldr		q10, [x18, #32]					 \n"
		"	fmla	v29.4s, v11.4s, v3.s[2]			 \n"
		"	ldr		q11, [x18, #48]					 \n"

		"	add 	x18, x18, x6, lsl #1 			 \n"

		".endm									 	 \n"


		".macro KERNEL4x16_K3   	                 \n"

		"	fmla	v12.4s, v8.4s, v0.s[3]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[3]			 \n"

		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	fmla	v14.4s, v8.4s, v1.s[3]			 	\n"
		"	fmla	v15.4s, v9.4s, v1.s[3]			 	\n"

		"	fmla	v16.4s, v8.4s, v2.s[3]			 	\n"
		"	fmla	v17.4s, v9.4s, v2.s[3]			 	\n"

		"	fmla	v18.4s, v8.4s, v3.s[3]			 	\n"
		"	ldr		q8, [x17]						 					\n"
		"	fmla	v19.4s, v9.4s, v3.s[3]			 	\n"
		"	ldr		q9, [x17, #16]					 			\n"

		"	fmla	v22.4s, v10.4s, v0.s[3]			 	\n"
		"	fmla	v23.4s, v11.4s, v0.s[3]			 	\n"

		"	ldr		q0, [x11], #16					 			\n"

		"	fmla	v24.4s, v10.4s, v1.s[3]			 	\n"
		"	fmla	v25.4s, v11.4s, v1.s[3]			 	\n"

		"	ldr		q1, [x12], #16 					 			\n"

		"	fmla	v26.4s, v10.4s, v2.s[3]			 	\n"
		"	fmla	v27.4s, v11.4s, v2.s[3]			 	\n"

		"	ldr		q2, [x13], #16					 			\n"

		"	fmla	v28.4s, v10.4s, v3.s[3]			 	\n"
		"	ldr		q10, [x17, #32]				 	 			\n"
		"	fmla	v29.4s, v11.4s, v3.s[3]			 	\n"
		"	ldr		q11, [x17, #48]						 		\n"

		"	ldr		q3, [x14], #16					 			\n"

		"	add		x17, x17, x6, lsl #1 			 		\n"

		".endm									 	 						\n"


		".macro KERNEL4x16_END_K   	         	\n"

		"	fmla	v12.4s, v8.4s, v0.s[3]			 	\n"
		"	fmla	v13.4s, v9.4s, v0.s[3]			 	\n"

		"	fmla	v14.4s, v8.4s, v1.s[3]			 	\n"
		"	fmla	v15.4s, v9.4s, v1.s[3]			 	\n"

		"	fmla	v16.4s, v8.4s, v2.s[3]			 	\n"
		"	fmla	v17.4s, v9.4s, v2.s[3]			 	\n"

		"	fmla	v18.4s, v8.4s, v3.s[3]			 	\n"
		"	fmla	v19.4s, v9.4s, v3.s[3]			 	\n"

		"	fmla	v22.4s, v10.4s, v0.s[3]			 	\n"
		"	fmla	v23.4s, v11.4s, v0.s[3]			 	\n"

		"	fmla	v24.4s, v10.4s, v1.s[3]			 	\n"
		"	fmla	v25.4s, v11.4s, v1.s[3]			 	\n"

		"	fmla	v26.4s, v10.4s, v2.s[3]			 	\n"
		"	fmla	v27.4s, v11.4s, v2.s[3]			 	\n"

		"	fmla	v28.4s, v10.4s, v3.s[3]			 	\n"
		"	fmla	v29.4s, v11.4s, v3.s[3]			 	\n"

		".endm									 	 						\n"



		".macro KERNEL4x16_EDGE_K   	        \n"


		"	ldp		q8, q9, [x17]	 								\n"
		"	ld1 	{v0.s}[3], [x11], #4 					\n"

		"	fmla	v12.4s, v8.4s, v0.s[3]			 	\n"
		"	ld1 	{v1.s}[3], [x12], #4 					\n"
		"	fmla	v13.4s, v9.4s, v0.s[3]			 	\n"

		"	ld1 	{v2.s}[3], [x13], #4 					\n"
		"	fmla	v14.4s, v8.4s, v1.s[3]			 	\n"
		"	ld1 	{v3.s}[3], [x14], #4 					\n"
		"	fmla	v15.4s, v9.4s, v1.s[3]			 	\n"

		"	ldp		q10, q11, [x17, #32]	 				\n"
		"	fmla	v16.4s, v8.4s, v2.s[3]			 	\n"
		"	fmla	v17.4s, v9.4s, v2.s[3]			 	\n"

		"	add 	x17, x17, x6 									\n"
		"	fmla	v18.4s, v8.4s, v3.s[3]			 	\n"
		"	fmla	v19.4s, v9.4s, v3.s[3]			 	\n"

		"	prfm	PLDL1KEEP, [x17, #64] 				\n"
		"	fmla	v22.4s, v10.4s, v0.s[3]			 	\n"
		"	fmla	v23.4s, v11.4s, v0.s[3]			 	\n"

		"	fmla	v24.4s, v10.4s, v1.s[3]			 	\n"
		"	fmla	v25.4s, v11.4s, v1.s[3]			 	\n"

		"	fmla	v26.4s, v10.4s, v2.s[3]			 	\n"
		"	fmla	v27.4s, v11.4s, v2.s[3]			 	\n"

		"	fmla	v28.4s, v10.4s, v3.s[3]			 	\n"
		"	fmla	v29.4s, v11.4s, v3.s[3]			 	\n"

		".endm									 	 						\n"


		".macro SAVE4x16							 				\n"

		"	stp		q12, q13, [x25]			 		 			\n"
		"	stp		q14, q15, [x26]			 	     		\n"
		"	stp		q16, q17, [x27]					 			\n"
		"	stp		q18, q19, [x28]					 			\n"

		"	stp		q22, q23, [x25, #32]			 \n"
		"	stp		q24, q25, [x26, #32]			 \n"
		"	stp		q26, q27, [x27, #32]			 \n"
		"	stp		q28, q29, [x28, #32]			 \n"

		".endm 									 	 \n"


		".macro	KERNEL3x16_BEGIN_K					 \n"

		"										 	 \n"
		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q1, [x12], #16					 \n"
		"	ldp		q8, q9, [x17], #32			     \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"

		"	fmul	v12.4s, v8.4s, v0.s[0]	         \n"
		"	fmul	v13.4s, v9.4s, v0.s[0]	         \n"

		"	ldr		q2, [x13], #16					 \n"

		"	fmul	v14.4s, v8.4s, v1.s[0]	         \n"
		"	fmul	v15.4s, v9.4s, v1.s[0]	         \n"

		"	ldp		q10, q11, [x17], #-32			 \n"

		"	fmul	v16.4s, v8.4s, v2.s[0]	         \n"
		"	ldr		q8, [x18]						 \n"
		"	fmul	v17.4s, v9.4s, v2.s[0]	         \n"
		"	ldr		q9, [x18, #16]					 \n"

		"	prfm	PLDL1KEEP, [x18, #64]			 \n"		
		

		"	add		x17, x17, x6, lsl #1 			 \n"

		"	fmul	v22.4s, v10.4s, v0.s[0]			 \n"
		"	fmul	v23.4s, v11.4s, v0.s[0]			 \n"

		"	fmul	v24.4s, v10.4s, v1.s[0]	         \n"
		"	fmul	v25.4s, v11.4s, v1.s[0]	         \n"

		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	fmul	v26.4s, v10.4s, v2.s[0]	         \n"
		"	ldr		q10, [x18, #32]					 \n"
		"	fmul	v27.4s, v11.4s, v2.s[0]	         \n"		
		"	ldr		q11, [x18, #48]					 \n"

		"	add		x18, x18, x6, lsl #1 			 \n"

		".endm 										 \n"


		".macro KERNEL3x16_K0                   	 \n"

		"   prfm    PLDL1KEEP, [x12, #64]       	 \n"
		"	fmla	v12.4s, v8.4s, v0.s[0]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[0]			 \n"

		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	fmla	v14.4s, v8.4s, v1.s[0]			 \n"
		"	fmla	v15.4s, v9.4s, v1.s[0]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[0]			 \n"
		"	ldr		q8, [x18]						 \n"
		"	fmla	v17.4s, v9.4s, v2.s[0]			 \n"
		"	ldr		q9, [x18, #16]					 \n"

		"	fmla	v22.4s, v10.4s, v0.s[0]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[0]			 \n"

		"	fmla	v24.4s, v10.4s, v1.s[0]			 \n"
		"	fmla	v25.4s, v11.4s, v1.s[0]			 \n"

		"	fmla	v26.4s, v10.4s, v2.s[0]			 \n"
		"	ldr		q10, [x18, #32]					 \n"
		"	fmla	v27.4s, v11.4s, v2.s[0]			 \n"
		"	ldr		q11, [x18, #48]					 \n"

		"	add		x18, x18, x6, lsl #1 			 \n"


		".endm   									 \n"


		".macro KERNEL3x16_K1        	             \n"

		"   prfm    PLDL1KEEP, [x12, #64]       	 \n"
		"	fmla	v12.4s, v8.4s, v0.s[1]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[1]			 \n"

		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	fmla	v14.4s, v8.4s, v1.s[1]			 \n"
		"	fmla	v15.4s, v9.4s, v1.s[1]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[1]			 \n"
		"	ldr		q8, [x17]						 \n"
		"	fmla	v17.4s, v9.4s, v2.s[1]			 \n"
		"	ldr		q9, [x17, #16]					 \n"

		"	fmla	v22.4s, v10.4s, v0.s[1]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[1]			 \n"

		"	fmla	v24.4s, v10.4s, v1.s[1]			 \n"
		"	fmla	v25.4s, v11.4s, v1.s[1]			 \n"

		"	fmla	v26.4s, v10.4s, v2.s[1]			 \n"
		"	ldr		q10, [x17, #32]					 \n"
		"	fmla	v27.4s, v11.4s, v2.s[1]			 \n"
		"	ldr		q11, [x17, #48]					 \n"

		"	add		x17, x17, x6, lsl #1 			 \n"

		".endm									 	 \n"


		".macro KERNEL3x16_K2          		         \n"

		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"

		"	fmla	v12.4s, v8.4s, v0.s[2]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[2]			 \n"

		"   prfm    PLDL1KEEP, [x18, #64]       	 \n"

		"	fmla	v14.4s, v8.4s, v1.s[2]			 \n"
		"	fmla 	v15.4s, v9.4s, v1.s[2]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[2]			 \n"
		"	ldr		q8, [x18]						 \n"
		"	fmla 	v17.4s, v9.4s, v2.s[2]			 \n"
		"	ldr		q9, [x18, #16]					 \n"

		"	fmla	v22.4s, v10.4s, v0.s[2]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[2]			 \n"

		"	prfm	PLDL1KEEP, [x14, #64]			 \n"

		"	fmla	v24.4s, v10.4s, v1.s[2]			 \n"
		"	fmla	v25.4s, v11.4s, v1.s[2]			 \n"

		"	fmla	v26.4s, v10.4s, v2.s[2]			 \n"
		"	ldr		q10, [x18, #32]					 \n"
		"	fmla	v27.4s, v11.4s, v2.s[2]			 \n"
		"	ldr		q11, [x18, #48]					 \n"

		"	add 	x18, x18, x6, lsl #1 			 \n"

		".endm									 	 \n"


		".macro KERNEL3x16_K3   	                 \n"

		"	fmla	v12.4s, v8.4s, v0.s[3]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[3]			 \n"

		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	fmla	v14.4s, v8.4s, v1.s[3]			 \n"
		"	fmla	v15.4s, v9.4s, v1.s[3]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[3]			 \n"
		"	ldr		q8, [x17]						 \n"
		"	fmla	v17.4s, v9.4s, v2.s[3]			 \n"
		"	ldr		q9, [x17, #16]					 \n"

		"	fmla	v22.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[3]			 \n"

		"	ldr		q0, [x11], #16					 \n"

		"	fmla	v24.4s, v10.4s, v1.s[3]			 \n"
		"	fmla	v25.4s, v11.4s, v1.s[3]			 \n"

		"	ldr		q1, [x12], #16 					 \n"

		"	fmla	v26.4s, v10.4s, v2.s[3]			 \n"
		"	ldr		q10, [x17, #32]				 	 \n"
		"	fmla	v27.4s, v11.4s, v2.s[3]			 \n"
		"	ldr		q11, [x17, #48]					 \n"

		"	ldr		q2, [x13], #16					 \n"
		"	add		x17, x17, x6, lsl #1 			 \n"

		".endm									 	 \n"


		".macro KERNEL3x16_END_K   	         \n"

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

		".endm									 	 					 \n"


		".macro KERNEL3x16_EDGE_K   	       \n"

		"	ldp		q8, q9, [x17] 							 \n"
		"	ld1		{v0.s}[3], [x11], #4 				 \n"
		"	fmla	v12.4s, v8.4s, v0.s[3]			 \n"
		"	ld1		{v1.s}[3], [x12], #4 				 \n"
		"	fmla	v13.4s, v9.4s, v0.s[3]			 \n"
		"	ld1		{v2.s}[3], [x13], #4 				 \n"
		"	fmla	v14.4s, v8.4s, v1.s[3]			 \n"
		"	ldp		q10, q11, [x17, #32] 				 \n"
		"	fmla	v15.4s, v9.4s, v1.s[3]			 \n"

		"	add 	x17, x17, x6 								 \n"
		"	fmla	v16.4s, v8.4s, v2.s[3]			 \n"
		"	fmla	v17.4s, v9.4s, v2.s[3]			 \n"

		"	prfm	PLDL1KEEP, [x17, #64] 			 \n"
		"	fmla	v22.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[3]			 \n"

		"	fmla	v24.4s, v10.4s, v1.s[3]			 \n"
		"	fmla	v25.4s, v11.4s, v1.s[3]			 \n"

		"	fmla	v26.4s, v10.4s, v2.s[3]			 \n"
		"	fmla	v27.4s, v11.4s, v2.s[3]			 \n"

		".endm									 	 					 \n"


		".macro SAVE3x16							 			 \n"

		"	stp		q12, q13, [x25]			 		 		 \n"
		"	stp		q14, q15, [x26]			 	     	 \n"
		"	stp		q16, q17, [x27]					     \n"

		"	stp		q22, q23, [x25, #32]			   \n"
		"	stp		q24, q25, [x26, #32]			   \n"
		"	stp		q26, q27, [x27, #32]			   \n"

		".endm 									 	           \n"


		".macro	KERNEL2x16_BEGIN_K					 \n"

		"										 	 \n"
		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q1, [x12], #16					 \n"
		"	ldp		q8, q9, [x17], #32			     \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"

		"	fmul	v12.4s, v8.4s, v0.s[0]	         \n"
		"	fmul	v13.4s, v9.4s, v0.s[0]	         \n"

		"	ldp		q10, q11, [x17], #-32			 \n"
		"	prfm	PLDL1KEEP, [x18, #64]			 \n"

		"	fmul	v14.4s, v8.4s, v1.s[0]	         \n"
		"	ldr		q8, [x18]						 \n"
		"	fmul	v15.4s, v9.4s, v1.s[0]	         \n"
		"	ldr		q9, [x18, #16]					 \n"		
		
		"	add		x17, x17, x6, lsl #1 			 \n"

		"	fmul	v22.4s, v10.4s, v0.s[0]			 \n"
		"	fmul	v23.4s, v11.4s, v0.s[0]			 \n"

		"	fmul	v24.4s, v10.4s, v1.s[0]	         \n"
		"	ldr		q10, [x18, #32]					 \n"
		"	fmul	v25.4s, v11.4s, v1.s[0]	         \n"
		"	ldr		q11, [x18, #48]					 \n"

		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"		

		"	add		x18, x18, x6, lsl #1 			 \n"

		".endm 										 \n"


		".macro KERNEL2x16_K0                   	 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	fmla	v12.4s, v8.4s, v0.s[0]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[0]			 \n"

		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	fmla	v14.4s, v8.4s, v1.s[0]			 \n"
		"	ldr		q8, [x18]						 \n"
		"	fmla	v15.4s, v9.4s, v1.s[0]			 \n"
		"	ldr		q9, [x18, #16]					 \n"

		"	fmla	v22.4s, v10.4s, v0.s[0]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[0]			 \n"

		"	fmla	v24.4s, v10.4s, v1.s[0]			 \n"
		"	ldr		q10, [x18, #32]					 \n"
		"	fmla	v25.4s, v11.4s, v1.s[0]			 \n"
		"	ldr		q11, [x18, #48]					 \n"

		"	add		x18, x18, x6, lsl #1 			 \n"


		".endm   									 \n"


		".macro KERNEL2x16_K1        	             \n"

		"   prfm    PLDL1KEEP, [x12, #64]       	 \n"
		"	fmla	v12.4s, v8.4s, v0.s[1]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[1]			 \n"

		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	fmla	v14.4s, v8.4s, v1.s[1]			 \n"
		"	ldr		q8, [x17]						 \n"
		"	fmla	v15.4s, v9.4s, v1.s[1]			 \n"
		"	ldr		q9, [x17, #16]					 \n"

		"	fmla	v22.4s, v10.4s, v0.s[1]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[1]			 \n"

		"	fmla	v24.4s, v10.4s, v1.s[1]			 \n"
		"	ldr		q10, [x17, #32]					 \n"
		"	fmla	v25.4s, v11.4s, v1.s[1]			 \n"
		"	ldr		q11, [x17, #48]					 \n"

		"	add		x17, x17, x6, lsl #1 			 \n"

		".endm									 	 \n"


		".macro KERNEL2x16_K2          		         \n"

		"	fmla	v12.4s, v8.4s, v0.s[2]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[2]			 \n"

		"   prfm    PLDL1KEEP, [x18, #64]       	 \n"

		"	fmla	v14.4s, v8.4s, v1.s[2]			 \n"
		"	ldr		q8, [x18]						 \n"
		"	fmla 	v15.4s, v9.4s, v1.s[2]			 \n"
		"	ldr		q9, [x18, #16]					 \n"

		"	fmla	v22.4s, v10.4s, v0.s[2]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[2]			 \n"

		"	fmla	v24.4s, v10.4s, v1.s[2]			 \n"
		"	ldr		q10, [x18, #32]					 \n"
		"	fmla	v25.4s, v11.4s, v1.s[2]			 \n"
		"	ldr		q11, [x18, #48]					 \n"

		"	add 	x18, x18, x6, lsl #1 			 \n"

		".endm									 	 \n"


		".macro KERNEL2x16_K3   	                 \n"

		"	fmla	v12.4s, v8.4s, v0.s[3]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[3]			 \n"

		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	fmla	v14.4s, v8.4s, v1.s[3]			 \n"
		"	ldr		q8, [x17]						 \n"
		"	fmla	v15.4s, v9.4s, v1.s[3]			 \n"
		"	ldr		q9, [x17, #16]					 \n"

		"	fmla	v22.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[3]			 \n"

		"	ldr		q0, [x11], #16					 \n"

		"	fmla	v24.4s, v10.4s, v1.s[3]			 \n"
		"	ldr		q10, [x17, #32]				 	 \n"
		"	fmla	v25.4s, v11.4s, v1.s[3]			 \n"
		"	ldr		q11, [x17, #48]					 \n"

		"	ldr		q1, [x12], #16 					 \n"

		"	add		x17, x17, x6, lsl #1 			 \n"

		".endm									 	 \n"


		".macro KERNEL2x16_END_K   	              \n"

		"	fmla	v12.4s, v8.4s, v0.s[3]			 			\n"
		"	fmla	v13.4s, v9.4s, v0.s[3]			 			\n"

		"	fmla	v14.4s, v8.4s, v1.s[3]			 			\n"
		"	fmla	v15.4s, v9.4s, v1.s[3]			 			\n"

		"	fmla	v22.4s, v10.4s, v0.s[3]			 			\n"
		"	fmla	v23.4s, v11.4s, v0.s[3]			 			\n"

		"	fmla	v24.4s, v10.4s, v1.s[3]			 			\n"
		"	fmla	v25.4s, v11.4s, v1.s[3]			 			\n"

		".endm									 	 								\n"


		".macro KERNEL2x16_EDGE_K   	             \n"


		"	ldp		q8, q9, [x17] 										\n"
		"	ld1		{v0.s}[3], [x11], #4 							\n"
		"	fmla	v12.4s, v8.4s, v0.s[3]			 			\n"
		"	ld1		{v1.s}[3], [x12], #4 							\n"
		"	fmla	v13.4s, v9.4s, v0.s[3]			 			\n"
		"	ldp		q10, q11, [x17, #32] 							\n"
		"	fmla	v14.4s, v8.4s, v1.s[3]			 			\n"
		"	add 	x17, x17, x6 											\n"
		"	fmla	v15.4s, v9.4s, v1.s[3]			 			\n"
		"	prfm 	PLDL1KEEP, [x17, #64] 						\n"

		"	fmla	v22.4s, v10.4s, v0.s[3]			 			\n"
		"	fmla	v23.4s, v11.4s, v0.s[3]			 			\n"

		"	fmla	v24.4s, v10.4s, v1.s[3]			 			\n"
		"	fmla	v25.4s, v11.4s, v1.s[3]			 			\n"

		".endm									 	 								\n"


		".macro SAVE2x16							 						\n"

		"	stp		q12, q13, [x25]			 		 					\n"
		"	stp		q14, q15, [x26]			 	     				\n"

		"	stp		q22, q23, [x25, #32]			 				\n"
		"	stp		q24, q25, [x26, #32]			 				\n"

		".endm 									 	 								\n"


		".macro	KERNEL1x16_BEGIN_K					 			\n"

		"										 	 										\n"
		"	ldr		q0, [x11], #16					 					\n"
		"	ldp		q8, q9, [x17], #32			     			\n"

		"   prfm    PLDL1KEEP, [x11, #64]       	\n"
		"	prfm	PLDL1KEEP, [x18, #64]			 				\n"

		"	ldp		q10, q11, [x17], #-32			 				\n"

		"	fmul	v12.4s, v8.4s, v0.s[0]	         \n"
		"	ldr		q8, [x18]						 \n"
		"	fmul	v13.4s, v9.4s, v0.s[0]	         \n"
		"	ldr		q9, [x18, #16]					 \n"
				
		
		"	add		x17, x17, x6, lsl #1 			 \n"

		"	fmul	v22.4s, v10.4s, v0.s[0]			 \n"
		"	ldr		q10, [x18, #32]					 \n"
		"	fmul	v23.4s, v11.4s, v0.s[0]			 \n"
		"	ldr		q11, [x18, #48]					 \n"

		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"		

		"	add		x18, x18, x6, lsl #1 			 \n"

		".endm 										 \n"


		".macro KERNEL1x16_K0                   	 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	fmla	v12.4s, v8.4s, v0.s[0]			 \n"
		"	ldr		q8, [x18]						 \n"
		"	fmla	v13.4s, v9.4s, v0.s[0]			 \n"
		"	ldr		q9, [x18, #16]					 \n"

		"	fmla	v22.4s, v10.4s, v0.s[0]			 \n"
		"	ldr		q10, [x18, #32]					 \n"
		"	fmla	v23.4s, v11.4s, v0.s[0]			 \n"
		"	ldr		q11, [x18, #48]					 \n"

		"	add		x18, x18, x6, lsl #1 			 \n"


		".endm   									 \n"


		".macro KERNEL1x16_K1        	             \n"

		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"
		"   prfm    PLDL1KEEP, [x12, #64]       	 \n"

		"	fmla	v12.4s, v8.4s, v0.s[1]			 \n"
		"	ldr		q8, [x17]						 \n"
		"	fmla	v13.4s, v9.4s, v0.s[1]			 \n"
		"	ldr		q9, [x17, #16]					 \n"

		"	fmla	v22.4s, v10.4s, v0.s[1]			 \n"
		"	ldr		q10, [x17, #32]					 \n"
		"	fmla	v23.4s, v11.4s, v0.s[1]			 \n"
		"	ldr		q11, [x17, #48]					 \n"

		"	add		x17, x17, x6, lsl #1 			 \n"

		".endm									 	 \n"


		".macro KERNEL1x16_K2          		         \n"

		"   prfm    PLDL1KEEP, [x18, #64]       	 \n"

		"	fmla	v12.4s, v8.4s, v0.s[2]			 \n"
		"	ldr		q8, [x18]						 \n"
		"	fmla	v13.4s, v9.4s, v0.s[2]			 \n"
		"	ldr		q9, [x18, #16]					 \n"

		"	fmla	v22.4s, v10.4s, v0.s[2]			 \n"
		"	ldr		q10, [x18, #32]					 \n"
		"	fmla	v23.4s, v11.4s, v0.s[2]			 \n"
		"	ldr		q11, [x18, #48]					 \n"

		"	add 	x18, x18, x6, lsl #1 			 \n"

		".endm									 	 \n"


		".macro KERNEL1x16_K3   	                 \n"

		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	fmla	v12.4s, v8.4s, v0.s[3]			 \n"
		"	ldr		q8, [x17]						 \n"
		"	fmla	v13.4s, v9.4s, v0.s[3]			 \n"		
		"	ldr		q9, [x17, #16]					 \n"

		"	fmla	v22.4s, v10.4s, v0.s[3]			 \n"
		"	ldr		q10, [x17, #32]				 	 \n"
		"	fmla	v23.4s, v11.4s, v0.s[3]			 \n"
		"	ldr		q11, [x17, #48]					 \n"

		"	ldr		q0, [x11], #16					 \n"

		"	add		x17, x17, x6, lsl #1 			 		\n"

		".endm									 	 						\n"


		".macro KERNEL1x16_END_K   	         \n"

		"	fmla	v12.4s, v8.4s, v0.s[3]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[3]			 \n"

		"	fmla	v22.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[3]			 \n"

		".endm									 	 					 \n"


		".macro KERNEL1x16_EDGE_K   	       \n"


		"	ldp		q8, q9, [x17] 							 \n"
		"	ld1		{v0.s}[3], [x11], #4 				 \n"
		"	fmla	v12.4s, v8.4s, v0.s[3]			 \n"
		"	ldp		q10, q11, [x17, #32] 				 \n"
		"	fmla	v13.4s, v9.4s, v0.s[3]			 \n"

		"	add 	x17, x17, x6 								 \n"
		"	fmla	v22.4s, v10.4s, v0.s[3]			 \n"
		"	prfm	PLDL1KEEP, [x17, #64] 			 \n"
		"	fmla	v23.4s, v11.4s, v0.s[3]			 \n"

		".endm									 	 					 \n"



		".macro SAVE1x16							 				\n"

		"	stp		q12, q13, [x25]			 		 			\n"
		"	stp		q22, q23, [x25, #32]			 		\n"

		".endm 									 	 						\n"



		".macro	N8_KERNEL5x8_BEGIN_K 			 	 \n"

		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q1, [x12], #16					 \n"
		"	ldp		q8, q9, [x17]				     \n"

		"	add		x17, x17, x6, lsl #1 			 \n"

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

		"	prfm	PLDL1KEEP, [x18, #32]			 \n"

		"	ldr		q4, [x15], #16				 	 \n"

		"	fmul	v18.4s, v8.4s, v3.s[0]	         \n"
		"	fmul	v19.4s, v9.4s, v3.s[0]	         \n"

		"	ldp		q10, q11, [x18]			 		 \n"

		"	fmul	v20.4s, v8.4s, v4.s[0]			 \n"
		"	fmul	v21.4s, v9.4s, v4.s[0]			 \n"

		"	add		x18, x18, x6, lsl #1 			 \n"

		".endm 										 \n"


		".macro N8_KERNEL5x8_K0                 	 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"

		"	fmla	v12.4s, v8.4s, v0.s[0]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[0]			 \n"

		"   prfm    PLDL1KEEP, [x18, #32]       	 \n"

		"	ldp		q10, q11, [x18]					 \n"

		"	fmla	v14.4s, v8.4s, v1.s[0]			 \n"
		"	fmla 	v15.4s, v9.4s, v1.s[0]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[0]			 \n"
		"	fmla 	v17.4s, v9.4s, v2.s[0]			 \n"

		"	fmla	v18.4s, v8.4s, v3.s[0]			 \n"
		"	fmla 	v19.4s, v9.4s, v3.s[0]			 \n"

		"	fmla	v20.4s, v8.4s, v4.s[0]			 \n"
		"	fmla	v21.4s, v9.4s, v4.s[0]			 \n"

		"	add 	x18, x18, x6, lsl #1 			 \n"

		".endm   									 \n"


		".macro N8_KERNEL5x8_K1                 	 \n"

		"   prfm    PLDL1KEEP, [x12, #64]       	 \n"
		"	fmla	v12.4s, v10.4s, v0.s[1]			 \n"
		"	fmla	v13.4s, v11.4s, v0.s[1]			 \n"

		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"
		"	ldp		q8, q9, [x17]					 \n"

		"	fmla	v14.4s, v10.4s, v1.s[1]			 \n"
		"	fmla	v15.4s, v11.4s, v1.s[1]			 \n"

		"	fmla	v16.4s, v10.4s, v2.s[1]			 \n"
		"	fmla	v17.4s, v11.4s, v2.s[1]			 \n"

		"	fmla	v18.4s, v10.4s, v3.s[1]			 \n"
		"	fmla	v19.4s, v11.4s, v3.s[1]			 \n"

		"	fmla	v20.4s, v10.4s, v4.s[1]			 \n"
		"	fmla	v21.4s, v11.4s, v4.s[1]			 \n"

		"	add		x17, x17, x6, lsl #1 			 \n"

		".endm									 	 \n"


		".macro N8_KERNEL5x8_K2            		     \n"

		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"

		"	fmla	v12.4s, v8.4s, v0.s[2]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[2]			 \n"

		"   prfm    PLDL1KEEP, [x18, #32]       	 \n"
		"	ldp		q10, q11, [x18] 				 \n"

		"	fmla	v14.4s, v8.4s, v1.s[2]			 \n"
		"	fmla 	v15.4s, v9.4s, v1.s[2]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[2]			 \n"
		"	fmla 	v17.4s, v9.4s, v2.s[2]			 \n"

		"	fmla	v18.4s, v8.4s, v3.s[2]			 \n"
		"	fmla 	v19.4s, v9.4s, v3.s[2]			 \n"

		"	fmla	v20.4s, v8.4s, v4.s[2]			 \n"
		"	fmla	v21.4s, v9.4s, v4.s[2]			 \n"

		"	add 	x18, x18, x6, lsl #1 			 \n"

		".endm									 	 \n"


		".macro N8_KERNEL5x8_K3                 	 \n"

		"   prfm    PLDL1KEEP, [x15, #64]       	 \n"
		"   prfm    PLDL1KEEP, [x17, #32]       	 \n"

		"	ldp		q8, q9, [x17]					 \n"

		"	fmla	v12.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v13.4s, v11.4s, v0.s[3]			 \n"

		"	ldr		q0, [x11], #16					 \n"

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

		"	add		x17, x17, x6, lsl #1 			 \n"

		".endm									 	 \n"



		".macro N8_KERNEL5x8_END_K           \n"

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

		".endm									 	 					 \n"


		".macro N8_KERNEL5x8_EDGE_K          \n"


		"	ldp		q10, q11, [x17] 						 \n"
		"	ld1		{v0.s}[3], [x11], #4 				 \n"
		"	fmla	v12.4s, v10.4s, v0.s[3]			 \n"
		"	ld1		{v1.s}[3], [x12], #4 				 \n"
		"	fmla	v13.4s, v11.4s, v0.s[3]			 \n"
		"	ld1		{v2.s}[3], [x13], #4 				 \n"
		"	fmla	v14.4s, v10.4s, v1.s[3]			 \n"
		"	ld1		{v3.s}[3], [x14], #4 				 \n"
		"	fmla	v15.4s, v11.4s, v1.s[3]			 \n"
		"	ld1		{v4.s}[3], [x15], #4 				 \n"

		"	fmla	v16.4s, v10.4s, v2.s[3]			 \n"
		"	add 	x17, x17, x6 								 \n"
		"	fmla	v17.4s, v11.4s, v2.s[3]			 \n"

		"	prfm	PLDL1KEEP, [x17, #32] 			 \n"
		"	fmla	v18.4s, v10.4s, v3.s[3]			 \n"
		"	fmla	v19.4s, v11.4s, v3.s[3]			 \n"

		"	fmla	v20.4s, v10.4s, v4.s[3]			 \n"
		"	fmla	v21.4s, v11.4s, v4.s[3]			 \n"

		".endm									 	 					 \n"



		".macro N8_SAVE5x8							 		 \n"

		"	subs	x21, x21, #1					 			 \n"

		"	add 	x10, x10, x8, lsl #2			 \n"
		"	add		x10, x10, x8 					 \n"

		"	stp		q12, q13, [x25]			 		 \n"
		"	add		x25, x29, x6					 \n"
		"	stp		q14, q15, [x26]			 	     \n"
		"	add		x26, x25, x6					 \n"
		"	stp		q16, q17, [x27]					 \n"
		"	add		x27, x26, x6					 \n"
		"	stp		q18, q19, [x28]					 \n"
		"	add		x28, x27, x6					 \n"
		"	stp		q20, q21, [x29]					 \n"
		"	add		x29, x28, x6					 \n"

		".endm 									 	 \n"


		".macro	N8_KERNEL4x8_BEGIN_K 			 	 \n"

		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q1, [x12], #16					 \n"
		"	ldp		q8, q9, [x17]				     \n"

		"	add		x17, x17, x6, lsl #1 			 \n"

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

		"	prfm	PLDL1KEEP, [x18, #32]			 \n"

		"	ldp		q10, q11, [x18]			 		 \n"

		"	fmul	v18.4s, v8.4s, v3.s[0]	         \n"
		"	fmul	v19.4s, v9.4s, v3.s[0]	         \n"

		"	add		x18, x18, x6, lsl #1 			 \n"

		".endm 										 \n"


		".macro N8_KERNEL4x8_K0                 	 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"

		"	fmla	v12.4s, v8.4s, v0.s[0]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[0]			 \n"

		"   prfm    PLDL1KEEP, [x18, #32]       	 \n"

		"	ldp		q10, q11, [x18]					 \n"

		"	fmla	v14.4s, v8.4s, v1.s[0]			 \n"
		"	fmla 	v15.4s, v9.4s, v1.s[0]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[0]			 \n"
		"	fmla 	v17.4s, v9.4s, v2.s[0]			 \n"

		"	fmla	v18.4s, v8.4s, v3.s[0]			 \n"
		"	fmla 	v19.4s, v9.4s, v3.s[0]			 \n"

		"	add 	x18, x18, x6, lsl #1 			 \n"

		".endm   									 \n"


		".macro N8_KERNEL4x8_K1                 	 \n"

		"   prfm    PLDL1KEEP, [x12, #64]       	 \n"
		"	fmla	v12.4s, v10.4s, v0.s[1]			 \n"
		"	fmla	v13.4s, v11.4s, v0.s[1]			 \n"

		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"
		"	ldp		q8, q9, [x17]					 \n"

		"	fmla	v14.4s, v10.4s, v1.s[1]			 \n"
		"	fmla	v15.4s, v11.4s, v1.s[1]			 \n"

		"	fmla	v16.4s, v10.4s, v2.s[1]			 \n"
		"	fmla	v17.4s, v11.4s, v2.s[1]			 \n"

		"	fmla	v18.4s, v10.4s, v3.s[1]			 \n"
		"	fmla	v19.4s, v11.4s, v3.s[1]			 \n"

		"	add		x17, x17, x6, lsl #1 			 \n"

		".endm									 	 \n"


		".macro N8_KERNEL4x8_K2            		     \n"

		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"

		"	fmla	v12.4s, v8.4s, v0.s[2]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[2]			 \n"

		"   prfm    PLDL1KEEP, [x18, #32]       	 \n"
		"	ldp		q10, q11, [x18] 				 \n"

		"	fmla	v14.4s, v8.4s, v1.s[2]			 \n"
		"	fmla 	v15.4s, v9.4s, v1.s[2]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[2]			 \n"
		"	fmla 	v17.4s, v9.4s, v2.s[2]			 \n"

		"	fmla	v18.4s, v8.4s, v3.s[2]			 \n"
		"	fmla 	v19.4s, v9.4s, v3.s[2]			 \n"

		"	add 	x18, x18, x6, lsl #1 			 \n"

		".endm									 	 \n"


		".macro N8_KERNEL4x8_K3                 	 \n"

		"   prfm    PLDL1KEEP, [x15, #64]       	 \n"
		"   prfm    PLDL1KEEP, [x17, #32]       	 \n"

		"	ldp		q8, q9, [x17]					 \n"

		"	fmla	v12.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v13.4s, v11.4s, v0.s[3]			 \n"

		"	ldr		q0, [x11], #16					 \n"

		"	fmla	v14.4s, v10.4s, v1.s[3]			 \n"
		"	fmla	v15.4s, v11.4s, v1.s[3]			 \n"

		"	ldr		q1, [x12], #16					 \n"

		"	fmla	v16.4s, v10.4s, v2.s[3]			 \n"
		"	fmla	v17.4s, v11.4s, v2.s[3]			 \n"

		"	ldr		q2, [x13], #16					 \n"

		"	fmla	v18.4s, v10.4s, v3.s[3]			 \n"
		"	fmla	v19.4s, v11.4s, v3.s[3]			 \n"

		"	ldr		q3, [x14], #16					 \n"


		"	add		x17, x17, x6, lsl #1 			 		\n"

		".endm									 	 						\n"



		".macro N8_KERNEL4x8_END_K           \n"

		"	fmla	v12.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v13.4s, v11.4s, v0.s[3]			 \n"

		"	fmla	v14.4s, v10.4s, v1.s[3]			 \n"
		"	fmla	v15.4s, v11.4s, v1.s[3]			 \n"

		"	fmla	v16.4s, v10.4s, v2.s[3]			 \n"
		"	fmla	v17.4s, v11.4s, v2.s[3]			 \n"

		"	fmla	v18.4s, v10.4s, v3.s[3]			 \n"
		"	fmla	v19.4s, v11.4s, v3.s[3]			 \n"

		".endm									 	 					 \n"


		".macro N8_KERNEL4x8_EDGE_K          \n"


		"	ldp		q10, q11, [x17] 						 \n"
		"	ld1		{v0.s}[3], [x11],	#4 				 \n"
		"	fmla	v12.4s, v10.4s, v0.s[3]			 \n"
		"	ld1		{v1.s}[3], [x12],	#4 				 \n"
		"	fmla	v13.4s, v11.4s, v0.s[3]			 \n"
		"	ld1		{v2.s}[3], [x13],	#4 				 \n"
		"	fmla	v14.4s, v10.4s, v1.s[3]			 \n"
		"	ld1		{v3.s}[3], [x14],	#4 				 \n"
		"	fmla	v15.4s, v11.4s, v1.s[3]			 \n"

		"	add 	x17, x17, x6 								 \n"
		"	fmla	v16.4s, v10.4s, v2.s[3]			 \n"
		"	prfm	PLDL1KEEP, [x17, #32] 			 \n"
		"	fmla	v17.4s, v11.4s, v2.s[3]			 \n"

		"	fmla	v18.4s, v10.4s, v3.s[3]			 \n"
		"	fmla	v19.4s, v11.4s, v3.s[3]			 \n"

		".endm									 	 					 \n"


		".macro N8_SAVE4x8 							 		\n"

		"	stp		q12, q13, [x25]			 		 		\n"
		"	stp		q14, q15, [x26]			 	     	\n"
		"	stp		q16, q17, [x27]					 		\n"
		"	stp		q18, q19, [x28]					 		\n"

		".endm 										 					\n"


		".macro	N8_KERNEL3x8_BEGIN_K 			 	 \n"

		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q1, [x12], #16					 \n"
		"	ldp		q8, q9, [x17]				     \n"

		"	add		x17, x17, x6, lsl #1 			 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"

		"	fmul	v12.4s, v8.4s, v0.s[0]	         \n"
		"	fmul	v13.4s, v9.4s, v0.s[0]	         \n"

		"	ldr		q2, [x13], #16					 \n"

		"	fmul	v14.4s, v8.4s, v1.s[0]	         \n"
		"	fmul	v15.4s, v9.4s, v1.s[0]	         \n"

		"	ldp		q10, q11, [x18]			 		 \n"

		"	fmul	v16.4s, v8.4s, v2.s[0]	         \n"
		"	fmul	v17.4s, v9.4s, v2.s[0]	         \n"

		"	prfm	PLDL1KEEP, [x18, #32]			 \n"

		"	add		x18, x18, x6, lsl #1 			 \n"

		".endm 										 \n"


		".macro N8_KERNEL3x8_K0                 	 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	ldp		q10, q11, [x18]					 \n"

		"	fmla	v12.4s, v8.4s, v0.s[0]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[0]			 \n"

		"   prfm    PLDL1KEEP, [x18, #32]       	 \n"

		"	fmla	v14.4s, v8.4s, v1.s[0]			 \n"
		"	fmla 	v15.4s, v9.4s, v1.s[0]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[0]			 \n"
		"	fmla 	v17.4s, v9.4s, v2.s[0]			 \n"

		"	add 	x18, x18, x6, lsl #1 			 \n"

		".endm   									 \n"


		".macro N8_KERNEL3x8_K1                 	 \n"

		"   prfm    PLDL1KEEP, [x12, #64]       	 \n"
		"	fmla	v12.4s, v10.4s, v0.s[1]			 \n"
		"	fmla	v13.4s, v11.4s, v0.s[1]			 \n"

		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"
		"	ldp		q8, q9, [x17]					 \n"

		"	fmla	v14.4s, v10.4s, v1.s[1]			 \n"
		"	fmla	v15.4s, v11.4s, v1.s[1]			 \n"

		"	fmla	v16.4s, v10.4s, v2.s[1]			 \n"
		"	fmla	v17.4s, v11.4s, v2.s[1]			 \n"

		"	add		x17, x17, x6, lsl #1 			 \n"

		".endm									 	 \n"


		".macro N8_KERNEL3x8_K2            		     \n"

		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"

		"	fmla	v12.4s, v8.4s, v0.s[2]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[2]			 \n"

		"   prfm    PLDL1KEEP, [x18, #32]       	 \n"
		"	ldp		q10, q11, [x18] 				 \n"

		"	fmla	v14.4s, v8.4s, v1.s[2]			 \n"
		"	fmla 	v15.4s, v9.4s, v1.s[2]			 \n"

		"	fmla	v16.4s, v8.4s, v2.s[2]			 \n"
		"	fmla 	v17.4s, v9.4s, v2.s[2]			 \n"

		"	add 	x18, x18, x6, lsl #1 			 \n"

		".endm									 	 \n"


		".macro N8_KERNEL3x8_K3                 	 \n"

		"   prfm    PLDL1KEEP, [x15, #64]       	 \n"
		"   prfm    PLDL1KEEP, [x17, #32]       	 \n"

		"	ldp		q8, q9, [x17]					 \n"

		"	fmla	v12.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v13.4s, v11.4s, v0.s[3]			 \n"

		"	ldr		q0, [x11], #16					 \n"

		"	fmla	v14.4s, v10.4s, v1.s[3]			 \n"
		"	fmla	v15.4s, v11.4s, v1.s[3]			 \n"

		"	ldr		q1, [x12], #16					 \n"

		"	fmla	v16.4s, v10.4s, v2.s[3]			 \n"
		"	fmla	v17.4s, v11.4s, v2.s[3]			 \n"

		"	ldr		q2, [x13], #16					 \n"

		"	add		x17, x17, x6, lsl #1 			 \n"

		".endm									 	 \n"



		".macro N8_KERNEL3x8_END_K           \n"

		"	fmla	v12.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v13.4s, v11.4s, v0.s[3]			 \n"

		"	fmla	v14.4s, v10.4s, v1.s[3]			 \n"
		"	fmla	v15.4s, v11.4s, v1.s[3]			 \n"

		"	fmla	v16.4s, v10.4s, v2.s[3]			 \n"
		"	fmla	v17.4s, v11.4s, v2.s[3]			 \n"

		".endm									 	 					 \n"



		".macro N8_KERNEL3x8_EDGE_K          \n"

		"	ldp		q10, q11, [x17] 						 \n"
		"	ld1		{v0.s}[3], [x11], #4 				 \n"
		"	fmla	v12.4s, v10.4s, v0.s[3]			 \n"
		"	ld1		{v1.s}[3], [x12], #4 				 \n"
		"	fmla	v13.4s, v11.4s, v0.s[3]			 \n"
		"	ld1		{v2.s}[3], [x13], #4 				 \n"
		"	fmla	v14.4s, v10.4s, v1.s[3]			 \n"
		"	add 	x17, x17, x6 								 \n"
		"	fmla	v15.4s, v11.4s, v1.s[3]			 \n"
		"	prfm	PLDL1KEEP, [x17, #32] 			 \n"

		"	fmla	v16.4s, v10.4s, v2.s[3]			 \n"
		"	fmla	v17.4s, v11.4s, v2.s[3]			 \n"

		".endm									 	 					 \n"


		".macro N8_SAVE3x8 							 \n"

		"	stp		q12, q13, [x25]			 		 \n"
		"	stp		q14, q15, [x26]			 	     \n"
		"	stp		q16, q17, [x27]					 \n"

		".endm 										 \n"



		".macro	N8_KERNEL2x8_BEGIN_K 			 	 \n"

		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q1, [x12], #16					 \n"
		"	ldp		q8, q9, [x17]				     \n"

		"	add		x17, x17, x6, lsl #1 			 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"

		"	fmul	v12.4s, v8.4s, v0.s[0]	         \n"
		"	fmul	v13.4s, v9.4s, v0.s[0]	         \n"

		"	ldp		q10, q11, [x18]			 		 \n"

		"	fmul	v14.4s, v8.4s, v1.s[0]	         \n"
		"	fmul	v15.4s, v9.4s, v1.s[0]	         \n"

		"	prfm	PLDL1KEEP, [x18, #32]			 \n"

		"	add		x18, x18, x6, lsl #1 			 \n"

		".endm 										 \n"


		".macro N8_KERNEL2x8_K0                 	 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	ldp		q10, q11, [x18]					 \n"

		"	fmla	v12.4s, v8.4s, v0.s[0]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[0]			 \n"

		"   prfm    PLDL1KEEP, [x18, #32]       	 \n"

		"	fmla	v14.4s, v8.4s, v1.s[0]			 \n"
		"	fmla 	v15.4s, v9.4s, v1.s[0]			 \n"

		"	add 	x18, x18, x6, lsl #1 			 \n"

		".endm   									 \n"


		".macro N8_KERNEL2x8_K1                 	 \n"

		"	ldp		q8, q9, [x17]					 \n"

		"   prfm    PLDL1KEEP, [x12, #64]       	 \n"
		"	fmla	v12.4s, v10.4s, v0.s[1]			 \n"
		"	fmla	v13.4s, v11.4s, v0.s[1]			 \n"

		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	fmla	v14.4s, v10.4s, v1.s[1]			 \n"
		"	fmla	v15.4s, v11.4s, v1.s[1]			 \n"

		"	add		x17, x17, x6, lsl #1 			 \n"

		".endm									 	 \n"


		".macro N8_KERNEL2x8_K2            		     \n"

		"	ldp		q10, q11, [x18] 				 \n"

		"	fmla	v12.4s, v8.4s, v0.s[2]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[2]			 \n"

		"   prfm    PLDL1KEEP, [x18, #32]       	 \n"
		"	fmla	v14.4s, v8.4s, v1.s[2]			 \n"
		"	fmla 	v15.4s, v9.4s, v1.s[2]			 \n"

		"	add 	x18, x18, x6, lsl #1 			 \n"

		".endm									 	 \n"


		".macro N8_KERNEL2x8_K3                 	 \n"

		"   prfm    PLDL1KEEP, [x15, #64]       	 \n"
		"   prfm    PLDL1KEEP, [x17, #32]       	 \n"

		"	ldp		q8, q9, [x17]					 \n"

		"	fmla	v12.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v13.4s, v11.4s, v0.s[3]			 \n"

		"	ldr		q0, [x11], #16					 \n"

		"	fmla	v14.4s, v10.4s, v1.s[3]			 \n"
		"	fmla	v15.4s, v11.4s, v1.s[3]			 \n"

		"	ldr		q1, [x12], #16					 \n"

		"	add		x17, x17, x6, lsl #1 			 \n"

		".endm									 	 \n"



		".macro N8_KERNEL2x8_END_K           \n"

		"	fmla	v12.4s, v10.4s, v0.s[3]			 \n"
		"	fmla	v13.4s, v11.4s, v0.s[3]			 \n"

		"	fmla	v14.4s, v10.4s, v1.s[3]			 \n"
		"	fmla	v15.4s, v11.4s, v1.s[3]			 \n"

		".endm									 	 					 \n"



		".macro N8_KERNEL2x8_EDGE_K          \n"

		"	ldp		q10, q11, [x17] 						 \n"
		"	ld1		{v0.s}[3], [x11], #4 				 \n"
		"	fmla	v12.4s, v10.4s, v0.s[3]			 \n"
		"	ld1		{v1.s}[3], [x12], #4 				 \n"
		"	fmla	v13.4s, v11.4s, v0.s[3]			 \n"

		"	add 	x17, x17, x6 								 \n"
		"	fmla	v14.4s, v10.4s, v1.s[3]			 \n"
		"	prfm	PLDL1KEEP, [x17, #32] 			 \n"
		"	fmla	v15.4s, v11.4s, v1.s[3]			 \n"

		".endm									 	 					 \n"


		".macro N8_SAVE2x8 							 \n"

		"	stp		q12, q13, [x25]			 		 \n"
		"	stp		q14, q15, [x26]			 	     \n"

		".endm 										 \n"


		".macro	N8_KERNEL1x8_BEGIN_K 			 	 \n"

		"	ldr		q0, [x11], #16					 \n"
		"	ldr		q1, [x12], #16					 \n"
		"	ldp		q8, q9, [x17]				     \n"

		"	add		x17, x17, x6, lsl #1 			 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"

		"	ldp		q10, q11, [x18]			 		 \n"

		"	fmul	v12.4s, v8.4s, v0.s[0]	         \n"
		"	fmul	v13.4s, v9.4s, v0.s[0]	         \n"

		"	prfm	PLDL1KEEP, [x18, #32]			 \n"

		"	add		x18, x18, x6, lsl #1 			 \n"

		".endm 										 \n"


		".macro N8_KERNEL1x8_K0                 	 \n"

		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	ldp		q10, q11, [x18]					 \n"

		"	fmla	v12.4s, v8.4s, v0.s[0]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[0]			 \n"

		"   prfm    PLDL1KEEP, [x18, #32]       	 \n"
		"	add 	x18, x18, x6, lsl #1 			 \n"

		".endm   									 \n"


		".macro N8_KERNEL1x8_K1                 	 \n"

		"	ldp		q8, q9, [x17]					 \n"
		"	fmla	v12.4s, v10.4s, v0.s[1]			 \n"
		"	fmla	v13.4s, v11.4s, v0.s[1]			 \n"

		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	add		x17, x17, x6, lsl #1 			 \n"

		".endm									 	 \n"


		".macro N8_KERNEL1x8_K2            		     \n"

		"	ldp		q10, q11, [x18] 				 \n"

		"	fmla	v12.4s, v8.4s, v0.s[2]			 \n"
		"	fmla	v13.4s, v9.4s, v0.s[2]			 \n"

		"   prfm    PLDL1KEEP, [x18, #32]       	 \n"
		"	add 	x18, x18, x6, lsl #1 			 				 \n"

		".endm									 	 								 \n"


		".macro N8_KERNEL1x8_K3                 	 \n"

		"   prfm    PLDL1KEEP, [x15, #64]       	 \n"
		"   prfm    PLDL1KEEP, [x17, #32]       	 \n"

		"	ldp		q8, q9, [x17]					 						 \n"

		"	fmla	v12.4s, v10.4s, v0.s[3]			 			 \n"
		"	fmla	v13.4s, v11.4s, v0.s[3]			 			 \n"

		"	ldr		q0, [x11], #16					 					 \n"

		"	add		x17, x17, x6, lsl #1 			 				 \n"

		".endm									 	 								 \n"



		".macro N8_KERNEL1x8_END_K              	 \n"

		"	fmla	v12.4s, v10.4s, v0.s[3]			 			 \n"
		"	fmla	v13.4s, v11.4s, v0.s[3]			 			 \n"

		".endm									 	 								 \n"


		".macro N8_KERNEL1x8_EDGE_K              	 \n"

		"	ldp		q10, q11, [x17] 									 \n"
		"	ld1		{v0.s}[3], [x11], #4 							 \n"

		"	fmla	v12.4s, v10.4s, v0.s[3]			 			 \n"
		"	add 	x17, x17, x6 											 \n"
		"	fmla	v13.4s, v11.4s, v0.s[3]			 			 \n"

		".endm									 	 								 \n"


		".macro N8_SAVE1x8 							 					\n"

		"	stp		q12, q13, [x25]			 		 					\n"

		".endm 										 								\n"




//------------------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++
// N4
//------------------------------------------------

		".macro	N4_KERNEL5x4_BEGIN_K 			 	 			\n"

		"	ldr		q0, [x11], #16					 					\n"
		"	ldr		q1, [x12], #16					 					\n"
		"	ldr		q10, [x17]				     						\n"

		"	add		x17, x17, x6, lsl #1 				 			\n"

		" prfm  PLDL1KEEP, [x11, #64]       			\n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 			\n"
		" prfm  PLDL1KEEP, [x13, #64]       			\n"
		"	prfm	PLDL1KEEP, [x14, #64] 			 			\n"
		"	prfm	PLDL1KEEP, [x15, #64]			 				\n"

		"	fmul	v12.4s, v10.4s, v0.s[0]	         	\n"
		"	ldr		q2, [x13], #16					 					\n"
		"	fmul	v13.4s, v10.4s, v1.s[0]	         	\n"
		"	ldr		q3, [x14], #16					 					\n"
		"	fmul	v14.4s, v10.4s, v2.s[0]	         	\n"

		"	prfm	PLDL1KEEP, [x18, #16]			 				\n"

		"	ldr		q4, [x15], #16				 	 					\n"
		"	fmul	v15.4s, v10.4s, v3.s[0]	         	\n"
		"	ldr		q11, [x18]			 		 							\n"
		"	fmul	v16.4s, v10.4s, v4.s[0]			 			\n"
		"	add		x18, x18, x6, lsl #1 			 				\n"

		".endm 										 								\n"


		".macro N4_KERNEL5x4_K0                 	\n"

		"	prfm    PLDL1KEEP, [x11, #64]       	 	\n"

		"	fmla	v12.4s, v10.4s, v0.s[0]			 			\n"
		"	fmla	v13.4s, v10.4s, v1.s[0]			 			\n"

		" prfm	PLDL1KEEP, [x18, #16]       	  	\n"

		"	ldr		q11, [x18]					 							\n"
		"	fmla	v14.4s, v10.4s, v2.s[0]			 			\n"
		"	fmla 	v15.4s, v10.4s, v3.s[0]			 			\n"

		"	fmla	v16.4s, v10.4s, v4.s[0]			 			\n"
		"	add 	x18, x18, x6, lsl #1 			 				\n"

		".endm   										 							\n"


		".macro N4_KERNEL5x4_K1                 	\n"

		" prfm  PLDL1KEEP, [x12, #64]       	 		\n"
		"	fmla	v12.4s, v11.4s, v0.s[1]			 			\n"
		"	fmla	v13.4s, v11.4s, v1.s[1]			 			\n"

		" prfm  PLDL1KEEP, [x17, #16]       			\n"
		"	ldr		q10, [x17]					 							\n"

		"	fmla	v14.4s, v11.4s, v2.s[1]			 			\n"
		"	fmla	v15.4s, v11.4s, v3.s[1]			 			\n"

		"	fmla	v16.4s, v11.4s, v4.s[1]			 			\n"
		"	add		x17, x17, x6, lsl #1 			 				\n"

		".endm									 	 								\n"


		".macro N4_KERNEL5x4_K2            		    \n"

		" prfm  PLDL1KEEP, [x13, #64]       			\n"
		"	fmla	v12.4s, v10.4s, v0.s[2]			 			\n"
		" prfm  PLDL1KEEP, [x14, #64]       			\n"
		"	fmla	v13.4s, v10.4s, v1.s[2]			 			\n"

		" prfm  PLDL1KEEP, [x18, #16]       			\n"
		"	ldr		q11, [x18] 				 								\n"
		"	fmla	v14.4s, v10.4s, v2.s[2]			 			\n"
		"	fmla 	v15.4s, v10.4s, v3.s[2]			 			\n"

		"	fmla	v16.4s, v10.4s, v4.s[2]			 			\n"
		"	add 	x18, x18, x6, lsl #1 			 				\n"

		".endm									 	 								\n"


		".macro N4_KERNEL5x4_K3                 	\n"

		"	prfm   PLDL1KEEP, [x15, #64]       			\n"
		"	prfm   PLDL1KEEP, [x17, #16]       			\n"

		"	ldr		q10, [x17]					 							\n"
		"	fmla	v12.4s, v11.4s, v0.s[3]			 			\n"
		"	ldr		q0, [x11], #16					 					\n"
		"	fmla	v13.4s, v11.4s, v1.s[3]			 			\n"
		"	ldr		q1, [x12], #16						 				\n"
		"	fmla	v14.4s, v11.4s, v2.s[3]			 			\n"
		"	ldr		q2, [x13], #16					 					\n"
		"	fmla	v15.4s, v11.4s, v3.s[3]			 			\n"
		"	ldr		q3, [x14], #16					 					\n"	
		"	fmla	v16.4s, v11.4s, v4.s[3]			 			\n"
		"	ldr		q4, [x15], #16					 					\n"
		"	add		x17, x17, x6, lsl #1 			 				\n"

		".endm									 	 								\n"


		".macro N4_KERNEL5x4_END_K              	\n"

		"	fmla	v12.4s, v11.4s, v0.s[3]			 			\n"
		"	fmla	v13.4s, v11.4s, v1.s[3]			 			\n"
		"	fmla	v14.4s, v11.4s, v2.s[3]			 			\n"
		"	fmla	v15.4s, v11.4s, v3.s[3]			 			\n"
		"	fmla	v16.4s, v11.4s, v4.s[3]			 			\n"

		".endm									 	 								\n"


		".macro N4_KERNEL5x4_EDGE_K              	\n"


		"	ldr		q11, [x17]	 											\n"
		"	ld1		{v0.s}[3], [x11], #4 							\n"
		"	fmla	v12.4s, v11.4s, v0.s[3]			 			\n"
		"	ld1		{v1.s}[3], [x12], #4 							\n"
		"	fmla	v13.4s, v11.4s, v1.s[3]			 			\n"
		"	ld1		{v2.s}[3], [x13], #4 							\n"
		"	fmla	v14.4s, v11.4s, v2.s[3]			 			\n"
		"	ld1		{v3.s}[3], [x14], #4 							\n"
		"	fmla	v15.4s, v11.4s, v3.s[3]			 			\n"
		"	ld1		{v4.s}[3], [x15], #4 							\n"
		"	fmla	v16.4s, v11.4s, v4.s[3]			 			\n"
		"	add 	x17, x17, x6 											\n"

		".endm									 	 								\n"


		".macro N4_SAVE5x4							 					\n"

		"	subs	x21, x21, #1					 						\n"

		"	add 	x10, x10, x8, lsl #2			 				\n"
		"	add		x10, x10, x8 					 						\n"

		"	str		q12, [x25]			 		 							\n"
		"	add		x25, x29, x6					 						\n"
		"	str		q13, [x26]			 	     						\n"
		"	add		x26, x25, x6					 						\n"
		"	str		q14, [x27]					 							\n"
		"	add		x27, x26, x6					 						\n"
		"	str		q15, [x28]					 							\n"
		"	add		x28, x27, x6					 						\n"
		"	str		q16, [x29]					 							\n"
		"	add		x29, x28, x6					 						\n"

		".endm 									 	 								\n"


		".macro	N4_KERNEL4x4_BEGIN_K 			 	 			\n"

		"	ldr		q0, [x11], #16					 					\n"
		"	ldr		q1, [x12], #16					 					\n"
		"	ldr		q10, [x17]				     						\n"

		"	add		x17, x17, x6, lsl #1 				 			\n"

		" prfm  PLDL1KEEP, [x11, #64]       			\n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 			\n"
		" prfm  PLDL1KEEP, [x13, #64]       			\n"
		"	prfm	PLDL1KEEP, [x14, #64] 			 			\n"

		"	fmul	v12.4s, v10.4s, v0.s[0]	         	\n"
		"	ldr		q2, [x13], #16					 					\n"
		"	fmul	v13.4s, v10.4s, v1.s[0]	         	\n"
		"	ldr		q3, [x14], #16					 					\n"
		"	fmul	v14.4s, v10.4s, v2.s[0]	         	\n"

		"	prfm	PLDL1KEEP, [x18, #16]			 				\n"

		"	fmul	v15.4s, v10.4s, v3.s[0]	         	\n"
		"	ldr		q11, [x18]			 		 							\n"
		"	add		x18, x18, x6, lsl #1 			 				\n"

		".endm 										 								\n"


		".macro N4_KERNEL4x4_K0                 	\n"

		"	prfm    PLDL1KEEP, [x11, #64]       	 	\n"

		"	fmla	v12.4s, v10.4s, v0.s[0]			 			\n"
		"	fmla	v13.4s, v10.4s, v1.s[0]			 			\n"

		" prfm	PLDL1KEEP, [x18, #16]       	  	\n"

		"	ldr		q11, [x18]					 							\n"
		"	fmla	v14.4s, v10.4s, v2.s[0]			 			\n"
		"	fmla 	v15.4s, v10.4s, v3.s[0]			 			\n"
		"	add 	x18, x18, x6, lsl #1 			 				\n"

		".endm   										 							\n"


		".macro N4_KERNEL4x4_K1                 	\n"

		" prfm  PLDL1KEEP, [x12, #64]       	 		\n"
		"	fmla	v12.4s, v11.4s, v0.s[1]			 			\n"
		"	fmla	v13.4s, v11.4s, v1.s[1]			 			\n"

		" prfm  PLDL1KEEP, [x17, #16]       			\n"
		"	ldr		q10, [x17]					 							\n"

		"	fmla	v14.4s, v11.4s, v2.s[1]			 			\n"
		"	fmla	v15.4s, v11.4s, v3.s[1]			 			\n"
		"	add		x17, x17, x6, lsl #1 			 				\n"

		".endm									 	 								\n"


		".macro N4_KERNEL4x4_K2            		    \n"

		" prfm  PLDL1KEEP, [x13, #64]       			\n"
		"	fmla	v12.4s, v10.4s, v0.s[2]			 			\n"
		"	prfm	PLDL1KEEP, [x14, #64] 			 			\n"
		"	fmla	v13.4s, v10.4s, v1.s[2]			 			\n"
		" prfm  PLDL1KEEP, [x18, #16]       			\n"
		"	ldr		q11, [x18] 				 								\n"

		"	fmla	v14.4s, v10.4s, v2.s[2]			 			\n"
		"	fmla 	v15.4s, v10.4s, v3.s[2]			 			\n"
		"	add 	x18, x18, x6, lsl #1 			 				\n"

		".endm									 	 								\n"


		".macro N4_KERNEL4x4_K3                 	\n"

		"	prfm   PLDL1KEEP, [x17, #16]       			\n"

		"	ldr		q10, [x17]					 							\n"
		"	fmla	v12.4s, v11.4s, v0.s[3]			 			\n"
		"	ldr		q0, [x11], #16					 					\n"
		"	fmla	v13.4s, v11.4s, v1.s[3]			 			\n"
		"	ldr		q1, [x12], #16						 				\n"
		"	fmla	v14.4s, v11.4s, v2.s[3]			 			\n"
		"	ldr		q2, [x13], #16					 					\n"
		"	fmla	v15.4s, v11.4s, v3.s[3]			 			\n"
		"	ldr		q3, [x14], #16					 					\n"	
		"	add		x17, x17, x6, lsl #1 			 				\n"

		".endm									 	 								\n"



		".macro N4_KERNEL4x4_END_K              	\n"

		"	fmla	v12.4s, v11.4s, v0.s[3]			 			\n"
		"	fmla	v13.4s, v11.4s, v1.s[3]			 			\n"
		"	fmla	v14.4s, v11.4s, v2.s[3]			 			\n"
		"	fmla	v15.4s, v11.4s, v3.s[3]			 			\n"

		".endm									 	 								\n"


		".macro N4_KERNEL4x4_EDGE_K              	\n"

		"	ldr 	q11, [x17] 												\n"
		"	ld1		{v0.s}[3], [x11], #4 							\n"
		"	fmla	v12.4s, v11.4s, v0.s[3]			 			\n"
		"	ld1		{v1.s}[3], [x12], #4 							\n"
		"	fmla	v13.4s, v11.4s, v1.s[3]			 			\n"
		"	ld1		{v2.s}[3], [x13], #4 							\n"
		"	fmla	v14.4s, v11.4s, v2.s[3]			 			\n"
		"	ld1		{v3.s}[3], [x14], #4 							\n"
		"	fmla	v15.4s, v11.4s, v3.s[3]			 			\n"
		"	add 	x17, x17, x6 											\n"

		".endm									 	 								\n"


		".macro N4_SAVE4x4							 					\n"

		"	str		q12, [x25]			 		 							\n"
		"	str		q13, [x26]			 	     						\n"
		"	str		q14, [x27]					 							\n"
		"	str		q15, [x28]					 							\n"


		".endm 									 	 								\n"



		".macro	N4_KERNEL3x4_BEGIN_K 			 	 			\n"

		"	ldr		q0, [x11], #16					 					\n"
		"	ldr		q1, [x12], #16					 					\n"
		"	ldr		q10, [x17]				     						\n"

		"	add		x17, x17, x6, lsl #1 				 			\n"

		" prfm  PLDL1KEEP, [x11, #64]       			\n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 			\n"
		" prfm  PLDL1KEEP, [x13, #64]       			\n"

		"	fmul	v12.4s, v10.4s, v0.s[0]	         	\n"
		"	ldr		q2, [x13], #16					 					\n"
		"	fmul	v13.4s, v10.4s, v1.s[0]	         	\n"
		"	fmul	v14.4s, v10.4s, v2.s[0]	         	\n"
		"	prfm	PLDL1KEEP, [x18, #16]			 				\n"
		"	ldr		q11, [x18]			 		 							\n"
		"	add		x18, x18, x6, lsl #1 			 				\n"

		".endm 										 								\n"


		".macro N4_KERNEL3x4_K0                 	\n"

		"	prfm    PLDL1KEEP, [x11, #64]       	 	\n"
		"	fmla	v12.4s, v10.4s, v0.s[0]			 			\n"
		"	fmla	v13.4s, v10.4s, v1.s[0]			 			\n"

		" prfm	PLDL1KEEP, [x18, #16]       	  	\n"
		"	ldr		q11, [x18]					 							\n"
		"	fmla	v14.4s, v10.4s, v2.s[0]			 			\n"
		"	add 	x18, x18, x6, lsl #1 			 				\n"

		".endm   										 							\n"


		".macro N4_KERNEL3x4_K1                 	\n"

		" prfm  PLDL1KEEP, [x12, #64]       	 		\n"
		"	fmla	v12.4s, v11.4s, v0.s[1]			 			\n"
		"	fmla	v13.4s, v11.4s, v1.s[1]			 			\n"
		" prfm  PLDL1KEEP, [x17, #16]       			\n"
		"	ldr		q10, [x17]					 							\n"
		"	fmla	v14.4s, v11.4s, v2.s[1]			 			\n"
		"	add		x17, x17, x6, lsl #1 			 				\n"

		".endm									 	 								\n"


		".macro N4_KERNEL3x4_K2            		    \n"

		" prfm  PLDL1KEEP, [x13, #64]       			\n"
		"	fmla	v12.4s, v10.4s, v0.s[2]			 			\n"
		"	fmla	v13.4s, v10.4s, v1.s[2]			 			\n"
		" prfm  PLDL1KEEP, [x18, #16]       			\n"
		"	ldr		q11, [x18] 				 								\n"
		"	fmla	v14.4s, v10.4s, v2.s[2]			 			\n"
		"	add 	x18, x18, x6, lsl #1 			 				\n"

		".endm									 	 								\n"


		".macro N4_KERNEL3x4_K3                 	\n"

		"	prfm   PLDL1KEEP, [x17, #16]       			\n"
		"	ldr		q10, [x17]					 							\n"
		"	fmla	v12.4s, v11.4s, v0.s[3]			 			\n"
		"	ldr		q0, [x11], #16					 					\n"
		"	fmla	v13.4s, v11.4s, v1.s[3]			 			\n"
		"	ldr		q1, [x12], #16						 				\n"
		"	fmla	v14.4s, v11.4s, v2.s[3]			 			\n"
		"	ldr		q2, [x13], #16					 					\n"
		"	add		x17, x17, x6, lsl #1 			 				\n"

		".endm									 	 								\n"



		".macro N4_KERNEL3x4_END_K              	\n"

		"	fmla	v12.4s, v11.4s, v0.s[3]			 			\n"
		"	fmla	v13.4s, v11.4s, v1.s[3]			 			\n"
		"	fmla	v14.4s, v11.4s, v2.s[3]			 			\n"

		".endm									 	 								\n"


		".macro N4_KERNEL3x4_EDGE_K              	\n"

		"	ldr 	q11, [x17] 												\n"
		"	ld1		{v0.s}[3], [x11], #4 							\n"
		"	fmla	v12.4s, v11.4s, v0.s[3]			 			\n"
		"	ld1		{v1.s}[3], [x12], #4 							\n"
		"	fmla	v13.4s, v11.4s, v1.s[3]			 			\n"
		"	ld1		{v2.s}[3], [x13], #4 							\n"
		"	fmla	v14.4s, v11.4s, v2.s[3]			 			\n"
		"	add 	x17, x17, x6 											\n"

		".endm									 	 								\n"


		".macro N4_SAVE3x4							 					\n"

		"	str		q12, [x25]			 		 							\n"
		"	str		q13, [x26]			 	     						\n"
		"	str		q14, [x27]					 							\n"

		".endm 									 	 								\n"


//

		".macro	N4_KERNEL2x4_BEGIN_K 			 	 			\n"

		"	ldr		q0, [x11], #16					 					\n"
		"	ldr		q1, [x12], #16					 					\n"
		"	ldr		q10, [x17]				     						\n"

		"	add		x17, x17, x6, lsl #1 				 			\n"

		" prfm  PLDL1KEEP, [x11, #64]       			\n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 			\n"

		"	fmul	v12.4s, v10.4s, v0.s[0]	         	\n"
		"	fmul	v13.4s, v10.4s, v1.s[0]	         	\n"
		"	prfm	PLDL1KEEP, [x18, #16]			 				\n"
		"	ldr		q11, [x18]			 		 							\n"
		"	add		x18, x18, x6, lsl #1 			 				\n"

		".endm 										 								\n"


		".macro N4_KERNEL2x4_K0                 	\n"

		"	prfm    PLDL1KEEP, [x11, #64]       	 	\n"
		"	fmla	v12.4s, v10.4s, v0.s[0]			 			\n"
		"	fmla	v13.4s, v10.4s, v1.s[0]			 			\n"

		" prfm	PLDL1KEEP, [x18, #16]       	  	\n"
		"	ldr		q11, [x18]					 							\n"
		"	add 	x18, x18, x6, lsl #1 			 				\n"

		".endm   										 							\n"


		".macro N4_KERNEL2x4_K1                 	\n"

		" prfm  PLDL1KEEP, [x12, #64]       	 		\n"
		"	fmla	v12.4s, v11.4s, v0.s[1]			 			\n"
		"	fmla	v13.4s, v11.4s, v1.s[1]			 			\n"
		" prfm  PLDL1KEEP, [x17, #16]       			\n"
		"	ldr		q10, [x17]					 							\n"
		"	add		x17, x17, x6, lsl #1 			 				\n"

		".endm									 	 								\n"


		".macro N4_KERNEL2x4_K2            		    \n"

		"	fmla	v12.4s, v10.4s, v0.s[2]			 			\n"
		"	fmla	v13.4s, v10.4s, v1.s[2]			 			\n"
		" prfm  PLDL1KEEP, [x18, #16]       			\n"
		"	ldr		q11, [x18] 				 								\n"
		"	add 	x18, x18, x6, lsl #1 			 				\n"

		".endm									 	 								\n"


		".macro N4_KERNEL2x4_K3                 	\n"

		"	prfm   PLDL1KEEP, [x17, #16]       			\n"
		"	ldr		q10, [x17]					 							\n"
		"	fmla	v12.4s, v11.4s, v0.s[3]			 			\n"
		"	ldr		q0, [x11], #16					 					\n"
		"	fmla	v13.4s, v11.4s, v1.s[3]			 			\n"
		"	ldr		q1, [x12], #16						 				\n"
		"	add		x17, x17, x6, lsl #1 			 				\n"

		".endm									 	 								\n"



		".macro N4_KERNEL2x4_END_K              	\n"

		"	fmla	v12.4s, v11.4s, v0.s[3]			 			\n"
		"	fmla	v13.4s, v11.4s, v1.s[3]			 			\n"

		".endm									 	 								\n"


		".macro N4_KERNEL2x4_EDGE_K              	\n"

		"	ldr		q11, [x17] 												\n"
		"	ld1 	{v0.s}[3], [x11], #4 							\n"
		"	fmla	v12.4s, v11.4s, v0.s[3]			 			\n"
		"	ld1 	{v1.s}[3], [x12], #4 							\n"
		"	fmla	v13.4s, v11.4s, v1.s[3]			 			\n"
		"	add 	x17, x17, x6 											\n"

		".endm									 	 								\n"


		".macro N4_SAVE2x4							 					\n"

		"	str		q12, [x25]			 		 							\n"
		"	str		q13, [x26]			 	     						\n"

		".endm 									 	 								\n"




//

		".macro	N4_KERNEL1x4_BEGIN_K 			 	 			\n"

		"	ldr		q0, [x11], #16					 					\n"
		"	ldr		q10, [x17]				     						\n"

		"	add		x17, x17, x6, lsl #1 				 			\n"
		" prfm  PLDL1KEEP, [x11, #64]       			\n"

		"	fmul	v12.4s, v10.4s, v0.s[0]	         	\n"
		"	prfm	PLDL1KEEP, [x18, #16]			 				\n"
		"	ldr		q11, [x18]			 		 							\n"
		"	add		x18, x18, x6, lsl #1 			 				\n"

		".endm 										 								\n"


		".macro N4_KERNEL1x4_K0                 	\n"

		"	prfm  PLDL1KEEP, [x11, #64]       	 		\n"
		"	fmla	v12.4s, v10.4s, v0.s[0]			 			\n"

		" prfm	PLDL1KEEP, [x18, #16]       	  	\n"
		"	ldr		q11, [x18]					 							\n"
		"	add 	x18, x18, x6, lsl #1 			 				\n"

		".endm   										 							\n"


		".macro N4_KERNEL1x4_K1                 	\n"

		"	fmla	v12.4s, v11.4s, v0.s[1]			 			\n"
		" prfm  PLDL1KEEP, [x17, #16]       			\n"
		"	ldr		q10, [x17]					 							\n"
		"	add		x17, x17, x6, lsl #1 			 				\n"

		".endm									 	 								\n"


		".macro N4_KERNEL1x4_K2            		    \n"

		"	fmla	v12.4s, v10.4s, v0.s[2]			 			\n"
		" prfm  PLDL1KEEP, [x18, #16]       			\n"
		"	ldr		q11, [x18] 				 								\n"
		"	add 	x18, x18, x6, lsl #1 			 				\n"

		".endm									 	 								\n"


		".macro N4_KERNEL1x4_K3                 	\n"

		"	prfm  PLDL1KEEP, [x17, #16]       			\n"
		"	ldr		q10, [x17]					 							\n"
		"	fmla	v12.4s, v11.4s, v0.s[3]			 			\n"
		"	ldr		q0, [x11], #16					 					\n"
		"	add		x17, x17, x6, lsl #1 			 				\n"

		".endm									 	 								\n"



		".macro N4_KERNEL1x4_END_K              	\n"

		"	fmla	v12.4s, v11.4s, v0.s[3]			 			\n"

		".endm									 	 								\n"


		".macro N4_KERNEL1x4_EDGE_K              	\n"

		"	ldr 	q11, [x17] 												\n"
		"	ld1		{v0.s}[3], [x11], #4 							\n"
		"	fmla	v12.4s, v11.4s, v0.s[3]			 			\n"
		"	add 	x17, x17, x6 											\n"

		".endm									 	 								\n"


		".macro N4_SAVE1x4							 					\n"

		"	str		q12, [x25]			 		 							\n"

		".endm 									 	 								\n"



//------------------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++
// N2
//------------------------------------------------

		".macro	N2_KERNEL5x2_BEGIN_K 			 	 			\n"

		"	ldr		q0, [x11], #16					 					\n"
		"	ldr		q1, [x12], #16					 					\n"
		"	ld1		{v10.2s}, [x17] 									\n"

		"	add		x17, x17, x6, lsl #1 				 			\n"

		" prfm  PLDL1KEEP, [x11, #64]       			\n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 			\n"
		" prfm  PLDL1KEEP, [x13, #64]       			\n"
		"	prfm	PLDL1KEEP, [x14, #64] 			 			\n"
		"	prfm	PLDL1KEEP, [x15, #64]			 				\n"

		"	fmul	v12.2s, v10.2s, v0.s[0]	         	\n"
		"	ldr		q2, [x13], #16					 					\n"
		"	fmul	v13.2s, v10.2s, v1.s[0]	         	\n"
		"	ldr		q3, [x14], #16					 					\n"
		"	fmul	v14.2s, v10.2s, v2.s[0]	         	\n"

		"	prfm	PLDL1KEEP, [x18, #8]			 				\n"

		"	ldr		q4, [x15], #16				 	 					\n"
		"	fmul	v15.2s, v10.2s, v3.s[0]	         	\n"
		"	ld1		{v11.2s}, [x18] 									\n"
		"	fmul	v16.2s, v10.2s, v4.s[0]			 			\n"
		"	add		x18, x18, x6, lsl #1 			 				\n"

		".endm 										 								\n"


		".macro N2_KERNEL5x2_K0                 	\n"

		"	prfm    PLDL1KEEP, [x11, #64]       	 	\n"

		"	fmla	v12.2s, v10.2s, v0.s[0]			 			\n"
		"	fmla	v13.2s, v10.2s, v1.s[0]			 			\n"

		" prfm	PLDL1KEEP, [x18, #8]       	  		\n"

		"	ld1		{v11.2s}, [x18]					 					\n"
		"	fmla	v14.2s, v10.2s, v2.s[0]			 			\n"
		"	fmla 	v15.2s, v10.2s, v3.s[0]			 			\n"

		"	fmla	v16.2s, v10.2s, v4.s[0]			 			\n"
		"	add 	x18, x18, x6, lsl #1 			 				\n"

		".endm   										 							\n"


		".macro N2_KERNEL5x2_K1                 	\n"

		" prfm  PLDL1KEEP, [x12, #64]       	 		\n"
		"	fmla	v12.2s, v11.2s, v0.s[1]			 			\n"
		"	fmla	v13.2s, v11.2s, v1.s[1]			 			\n"

		" prfm  PLDL1KEEP, [x17, #8]       				\n"
		"	ld1		{v10.2s}, [x17]					 					\n"

		"	fmla	v14.2s, v11.2s, v2.s[1]			 			\n"
		"	fmla	v15.2s, v11.2s, v3.s[1]			 			\n"

		"	fmla	v16.2s, v11.2s, v4.s[1]			 			\n"
		"	add		x17, x17, x6, lsl #1 			 				\n"

		".endm									 	 								\n"


		".macro N2_KERNEL5x2_K2            		    \n"

		" prfm  PLDL1KEEP, [x13, #64]       			\n"
		"	fmla	v12.2s, v10.2s, v0.s[2]			 			\n"
		" prfm  PLDL1KEEP, [x14, #64]       			\n"
		"	fmla	v13.2s, v10.2s, v1.s[2]			 			\n"

		" prfm  PLDL1KEEP, [x18, #8]       				\n"
		"	ld1		{v11.2s}, [x18] 				 					\n"
		"	fmla	v14.2s, v10.2s, v2.s[2]			 			\n"
		"	fmla 	v15.2s, v10.2s, v3.s[2]			 			\n"

		"	fmla	v16.2s, v10.2s, v4.s[2]			 			\n"
		"	add 	x18, x18, x6, lsl #1 			 				\n"

		".endm									 	 								\n"


		".macro N2_KERNEL5x2_K3                 	\n"

		"	prfm   PLDL1KEEP, [x15, #64]       			\n"
		"	prfm   PLDL1KEEP, [x17, #16]       			\n"

		"	ld1		{v10.2s}, [x17]					 					\n"
		"	fmla	v12.2s, v11.2s, v0.s[3]			 			\n"
		"	ldr		q0, [x11], #16					 					\n"
		"	fmla	v13.2s, v11.2s, v1.s[3]			 			\n"
		"	ldr		q1, [x12], #16						 				\n"
		"	fmla	v14.2s, v11.2s, v2.s[3]			 			\n"
		"	ldr		q2, [x13], #16					 					\n"
		"	fmla	v15.2s, v11.2s, v3.s[3]			 			\n"
		"	ldr		q3, [x14], #16					 					\n"	
		"	fmla	v16.2s, v11.2s, v4.s[3]			 			\n"
		"	ldr		q4, [x15], #16					 					\n"
		"	add		x17, x17, x6, lsl #1 			 				\n"

		".endm									 	 								\n"



		".macro N2_KERNEL5x2_END_K              	\n"


		"	fmla	v12.2s, v11.2s, v0.s[3]			 			\n"
		"	fmla	v13.2s, v11.2s, v1.s[3]			 			\n"
		"	fmla	v14.2s, v11.2s, v2.s[3]			 			\n"
		"	fmla	v15.2s, v11.2s, v3.s[3]			 			\n"
		"	fmla	v16.2s, v11.2s, v4.s[3]			 			\n"

		".endm									 	 								\n"


		".macro N2_KERNEL5x2_EDGE_K              	\n"

		"	ld1		{v11.2s}, [x17] 									\n"
		"	ld1		{v0.s}[3], [x11], #4 							\n"
		"	fmla	v12.2s, v11.2s, v0.s[3]			 			\n"
		"	ld1		{v1.s}[3], [x12], #4 							\n"
		"	fmla	v13.2s, v11.2s, v1.s[3]			 			\n"
		"	ld1		{v2.s}[3], [x13], #4 							\n"
		"	fmla	v14.2s, v11.2s, v2.s[3]			 			\n"
		"	ld1		{v3.s}[3], [x14], #4 							\n"
		"	fmla	v15.2s, v11.2s, v3.s[3]			 			\n"
		"	ld1		{v4.s}[3], [x15], #4 							\n"
		"	fmla	v16.2s, v11.2s, v4.s[3]			 			\n"
		"	add  	x17, x17, x6 											\n"

		".endm									 	 								\n"


		".macro N2_SAVE5x2							 					\n"

		"	subs	x21, x21, #1					 						\n"

		"	add 	x10, x10, x8, lsl #2			 				\n"
		"	add		x10, x10, x8 					 						\n"

		"	st1		{v12.2s}, [x25]			 		 					\n"
		"	add		x25, x29, x6					 						\n"
		"	st1		{v13.2s}, [x26]			 	     				\n"
		"	add		x26, x25, x6					 						\n"
		"	st1		{v14.2s}, [x27]					 					\n"
		"	add		x27, x26, x6					 						\n"
		"	st1		{v15.2s}, [x28]					 					\n"
		"	add		x28, x27, x6					 						\n"
		"	st1		{v16.2s}, [x29]					 					\n"
		"	add		x29, x28, x6					 						\n"

		".endm 									 	 								\n"



		".macro	N2_KERNEL4x2_BEGIN_K 			 	 			\n"

		"	ldr		q0, [x11], #16					 					\n"
		"	ldr		q1, [x12], #16					 					\n"
		"	ld1		{v10.2s}, [x17] 									\n"

		"	add		x17, x17, x6, lsl #1 				 			\n"

		" prfm  PLDL1KEEP, [x11, #64]       			\n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 			\n"
		" prfm  PLDL1KEEP, [x13, #64]       			\n"
		"	prfm	PLDL1KEEP, [x14, #64] 			 			\n"

		"	fmul	v12.2s, v10.2s, v0.s[0]	         	\n"
		"	ldr		q2, [x13], #16					 					\n"
		"	fmul	v13.2s, v10.2s, v1.s[0]	         	\n"
		"	ldr		q3, [x14], #16					 					\n"
		"	fmul	v14.2s, v10.2s, v2.s[0]	         	\n"

		"	prfm	PLDL1KEEP, [x18, #8]			 				\n"

		"	fmul	v15.2s, v10.2s, v3.s[0]	         	\n"
		"	ld1		{v11.2s}, [x18] 									\n"
		"	add		x18, x18, x6, lsl #1 			 				\n"

		".endm 										 								\n"


		".macro N2_KERNEL4x2_K0                 	\n"

		"	prfm    PLDL1KEEP, [x11, #64]       	 	\n"

		"	fmla	v12.2s, v10.2s, v0.s[0]			 			\n"
		"	fmla	v13.2s, v10.2s, v1.s[0]			 			\n"

		" prfm	PLDL1KEEP, [x18, #8]       	  		\n"

		"	ld1		{v11.2s}, [x18]					 					\n"
		"	fmla	v14.2s, v10.2s, v2.s[0]			 			\n"
		"	fmla 	v15.2s, v10.2s, v3.s[0]			 			\n"
		"	add 	x18, x18, x6, lsl #1 			 				\n"

		".endm   										 							\n"


		".macro N2_KERNEL4x2_K1                 	\n"

		" prfm  PLDL1KEEP, [x12, #64]       	 		\n"
		"	fmla	v12.2s, v11.2s, v0.s[1]			 			\n"
		"	fmla	v13.2s, v11.2s, v1.s[1]			 			\n"

		" prfm  PLDL1KEEP, [x17, #8]       				\n"
		"	ld1		{v10.2s}, [x17]					 					\n"

		"	fmla	v14.2s, v11.2s, v2.s[1]			 			\n"
		"	fmla	v15.2s, v11.2s, v3.s[1]			 			\n"
		"	add		x17, x17, x6, lsl #1 			 				\n"

		".endm									 	 								\n"


		".macro N2_KERNEL4x2_K2            		    \n"

		" prfm  PLDL1KEEP, [x13, #64]       			\n"
		"	fmla	v12.2s, v10.2s, v0.s[2]			 			\n"
		" prfm  PLDL1KEEP, [x14, #64]       			\n"
		"	fmla	v13.2s, v10.2s, v1.s[2]			 			\n"

		" prfm  PLDL1KEEP, [x18, #8]       				\n"
		"	ld1		{v11.2s}, [x18] 				 					\n"
		"	fmla	v14.2s, v10.2s, v2.s[2]			 			\n"
		"	fmla 	v15.2s, v10.2s, v3.s[2]			 			\n"
		"	add 	x18, x18, x6, lsl #1 			 				\n"

		".endm									 	 								\n"


		".macro N2_KERNEL4x2_K3                 	\n"

		"	prfm   PLDL1KEEP, [x17, #16]       			\n"

		"	ld1		{v10.2s}, [x17]					 					\n"
		"	fmla	v12.2s, v11.2s, v0.s[3]			 			\n"
		"	ldr		q0, [x11], #16					 					\n"
		"	fmla	v13.2s, v11.2s, v1.s[3]			 			\n"
		"	ldr		q1, [x12], #16						 				\n"
		"	fmla	v14.2s, v11.2s, v2.s[3]			 			\n"
		"	ldr		q2, [x13], #16					 					\n"
		"	fmla	v15.2s, v11.2s, v3.s[3]			 			\n"
		"	ldr		q3, [x14], #16					 					\n"	
		"	add		x17, x17, x6, lsl #1 			 				\n"

		".endm									 	 								\n"



		".macro N2_KERNEL4x2_END_K              	\n"

		"	fmla	v12.2s, v11.2s, v0.s[3]			 			\n"
		"	fmla	v13.2s, v11.2s, v1.s[3]			 			\n"
		"	fmla	v14.2s, v11.2s, v2.s[3]			 			\n"
		"	fmla	v15.2s, v11.2s, v3.s[3]			 			\n"

		".endm									 	 								\n"


		".macro N2_KERNEL4x2_EDGE_K              	\n"

		"	ld1		{v11.2s}, [x17] 									\n"
		"	ld1		{v0.s}[3], [x11], #4 							\n"
		"	fmla	v12.2s, v11.2s, v0.s[3]			 			\n"
		"	ld1		{v1.s}[3], [x12], #4 							\n"
		"	fmla	v13.2s, v11.2s, v1.s[3]			 			\n"
		"	ld1		{v2.s}[3], [x13], #4 							\n"
		"	fmla	v14.2s, v11.2s, v2.s[3]			 			\n"
		"	ld1		{v3.s}[3], [x14], #4 							\n"
		"	fmla	v15.2s, v11.2s, v3.s[3]			 			\n"
		"	add  	x17, x17, x6 											\n"

		".endm									 	 								\n"


		".macro N2_SAVE4x2							 					\n"

		"	st1		{v12.2s}, [x25]			 		 					\n"
		"	st1		{v13.2s}, [x26]			 	     				\n"
		"	st1		{v14.2s}, [x27]					 					\n"
		"	st1		{v15.2s}, [x28]					 					\n"

		".endm 									 	 								\n"



		".macro	N2_KERNEL3x2_BEGIN_K 			 	 			\n"

		"	ldr		q0, [x11], #16					 					\n"
		"	ldr		q1, [x12], #16					 					\n"
		"	ld1		{v10.2s}, [x17] 									\n"

		"	add		x17, x17, x6, lsl #1 				 			\n"

		" prfm  PLDL1KEEP, [x11, #64]       			\n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 			\n"
		" prfm  PLDL1KEEP, [x13, #64]       			\n"

		"	fmul	v12.2s, v10.2s, v0.s[0]	         	\n"
		"	ldr		q2, [x13], #16					 					\n"
		"	fmul	v13.2s, v10.2s, v1.s[0]	         	\n"
		"	fmul	v14.2s, v10.2s, v2.s[0]	         	\n"

		"	prfm	PLDL1KEEP, [x18, #8]			 				\n"
		"	ld1		{v11.2s}, [x18] 									\n"
		"	add		x18, x18, x6, lsl #1 			 				\n"

		".endm 										 								\n"


		".macro N2_KERNEL3x2_K0                 	\n"

		"	prfm    PLDL1KEEP, [x11, #64]       	 	\n"

		"	fmla	v12.2s, v10.2s, v0.s[0]			 			\n"
		"	fmla	v13.2s, v10.2s, v1.s[0]			 			\n"

		" prfm	PLDL1KEEP, [x18, #8]       	  		\n"

		"	ld1		{v11.2s}, [x18]					 					\n"
		"	fmla	v14.2s, v10.2s, v2.s[0]			 			\n"
		"	add 	x18, x18, x6, lsl #1 			 				\n"

		".endm   										 							\n"


		".macro N2_KERNEL3x2_K1                 	\n"

		" prfm  PLDL1KEEP, [x12, #64]       	 		\n"
		"	fmla	v12.2s, v11.2s, v0.s[1]			 			\n"
		"	fmla	v13.2s, v11.2s, v1.s[1]			 			\n"

		" prfm  PLDL1KEEP, [x17, #8]       				\n"
		"	ld1		{v10.2s}, [x17]					 					\n"

		"	fmla	v14.2s, v11.2s, v2.s[1]			 			\n"
		"	add		x17, x17, x6, lsl #1 			 				\n"

		".endm									 	 								\n"


		".macro N2_KERNEL3x2_K2            		    \n"

		" prfm  PLDL1KEEP, [x13, #64]       			\n"
		"	fmla	v12.2s, v10.2s, v0.s[2]			 			\n"
		"	fmla	v13.2s, v10.2s, v1.s[2]			 			\n"

		" prfm  PLDL1KEEP, [x18, #8]       				\n"
		"	ld1		{v11.2s}, [x18] 				 					\n"
		"	fmla	v14.2s, v10.2s, v2.s[2]			 			\n"
		"	add 	x18, x18, x6, lsl #1 			 				\n"

		".endm									 	 								\n"


		".macro N2_KERNEL3x2_K3                 	\n"

		"	prfm   PLDL1KEEP, [x17, #16]       			\n"

		"	ld1		{v10.2s}, [x17]					 					\n"
		"	fmla	v12.2s, v11.2s, v0.s[3]			 			\n"
		"	ldr		q0, [x11], #16					 					\n"
		"	fmla	v13.2s, v11.2s, v1.s[3]			 			\n"
		"	ldr		q1, [x12], #16						 				\n"
		"	fmla	v14.2s, v11.2s, v2.s[3]			 			\n"
		"	ldr		q2, [x13], #16					 					\n"
		"	add		x17, x17, x6, lsl #1 			 				\n"

		".endm									 	 								\n"



		".macro N2_KERNEL3x2_END_K              	\n"

		"	fmla	v12.2s, v11.2s, v0.s[3]			 			\n"
		"	fmla	v13.2s, v11.2s, v1.s[3]			 			\n"
		"	fmla	v14.2s, v11.2s, v2.s[3]			 			\n"

		".endm									 	 								\n"


		".macro N2_KERNEL3x2_EDGE_K              	\n"

		"	ld1		{v11.2s}, [x17] 									\n"
		"	ld1 	{v0.s}[3], [x11], #4 							\n"
		"	fmla	v12.2s, v11.2s, v0.s[3]			 			\n"
		"	ld1 	{v1.s}[3], [x12], #4 							\n"
		"	fmla	v13.2s, v11.2s, v1.s[3]			 			\n"
		"	ld1 	{v2.s}[3], [x13], #4 							\n"
		"	fmla	v14.2s, v11.2s, v2.s[3]			 			\n"
		"	add 	x17, x17, x6 											\n"

		".endm									 	 								\n"


		".macro N2_SAVE3x2							 					\n"

		"	st1		{v12.2s}, [x25]			 		 					\n"
		"	st1		{v13.2s}, [x26]			 	     				\n"
		"	st1		{v14.2s}, [x27]					 					\n"

		".endm 									 	 								\n"



		".macro	N2_KERNEL2x2_BEGIN_K 			 	 			\n"

		"	ldr		q0, [x11], #16					 					\n"
		"	ldr		q1, [x12], #16					 					\n"
		"	ld1		{v10.2s}, [x17] 									\n"

		"	add		x17, x17, x6, lsl #1 				 			\n"

		" prfm  PLDL1KEEP, [x11, #64]       			\n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 			\n"

		"	fmul	v12.2s, v10.2s, v0.s[0]	         	\n"
		"	fmul	v13.2s, v10.2s, v1.s[0]	         	\n"

		"	prfm	PLDL1KEEP, [x18, #8]			 				\n"
		"	ld1		{v11.2s}, [x18] 									\n"
		"	add		x18, x18, x6, lsl #1 			 				\n"

		".endm 										 								\n"


		".macro N2_KERNEL2x2_K0                 	\n"

		"	prfm    PLDL1KEEP, [x11, #64]       	 	\n"
		"	fmla	v12.2s, v10.2s, v0.s[0]			 			\n"
		"	fmla	v13.2s, v10.2s, v1.s[0]			 			\n"
		" prfm	PLDL1KEEP, [x18, #8]       	  		\n"

		"	ld1		{v11.2s}, [x18]					 					\n"
		"	add 	x18, x18, x6, lsl #1 			 				\n"

		".endm   										 							\n"


		".macro N2_KERNEL2x2_K1                 	\n"

		" prfm  PLDL1KEEP, [x12, #64]       	 		\n"
		"	fmla	v12.2s, v11.2s, v0.s[1]			 			\n"
		"	fmla	v13.2s, v11.2s, v1.s[1]			 			\n"

		" prfm  PLDL1KEEP, [x17, #8]       				\n"
		"	ld1		{v10.2s}, [x17]					 					\n"
		"	add		x17, x17, x6, lsl #1 			 				\n"

		".endm									 	 								\n"


		".macro N2_KERNEL2x2_K2            		    \n"

		"	fmla	v12.2s, v10.2s, v0.s[2]			 			\n"
		"	fmla	v13.2s, v10.2s, v1.s[2]			 			\n"

		" prfm  PLDL1KEEP, [x18, #8]       				\n"
		"	ld1		{v11.2s}, [x18] 				 					\n"
		"	add 	x18, x18, x6, lsl #1 			 				\n"

		".endm									 	 								\n"


		".macro N2_KERNEL2x2_K3                 	\n"

		"	prfm   PLDL1KEEP, [x17, #16]       			\n"

		"	ld1		{v10.2s}, [x17]					 					\n"
		"	fmla	v12.2s, v11.2s, v0.s[3]			 			\n"
		"	ldr		q0, [x11], #16					 					\n"
		"	fmla	v13.2s, v11.2s, v1.s[3]			 			\n"
		"	ldr		q1, [x12], #16						 				\n"
		"	add		x17, x17, x6, lsl #1 			 				\n"

		".endm									 	 								\n"



		".macro N2_KERNEL2x2_END_K              	\n"

		"	fmla	v12.2s, v11.2s, v0.s[3]			 			\n"
		"	fmla	v13.2s, v11.2s, v1.s[3]			 			\n"

		".endm									 	 								\n"


		".macro N2_KERNEL2x2_EDGE_K              	\n"

		"	ld1		{v11.2s}, [x17] 									\n"
		"	ld1		{v0.s}[3], [x11], #4 							\n"
		"	fmla	v12.2s, v11.2s, v0.s[3]			 			\n"
		"	ld1 	{v1.s}[3], [x12], #4 							\n"
		"	fmla	v13.2s, v11.2s, v1.s[3]			 			\n"
		"	add 	x17, x17, x6 										 	\n"

		".endm									 	 								\n"

		".macro N2_SAVE2x2							 					\n"

		"	st1		{v12.2s}, [x25]			 		 					\n"
		"	st1		{v13.2s}, [x26]			 	     				\n"

		".endm 									 	 								\n"




		".macro	N2_KERNEL1x2_BEGIN_K 			 	 			\n"

		"	ldr		q0, [x11], #16					 					\n"
		"	ld1		{v10.2s}, [x17] 									\n"

		"	add		x17, x17, x6, lsl #1 				 			\n"

		" prfm  PLDL1KEEP, [x11, #64]       			\n"
		"	fmul	v12.2s, v10.2s, v0.s[0]	         	\n"

		"	prfm	PLDL1KEEP, [x18, #8]			 				\n"
		"	ld1		{v11.2s}, [x18] 									\n"
		"	add		x18, x18, x6, lsl #1 			 				\n"

		".endm 										 								\n"


		".macro N2_KERNEL1x2_K0                 	\n"

		"	prfm    PLDL1KEEP, [x11, #64]       	 	\n"
		"	fmla	v12.2s, v10.2s, v0.s[0]			 			\n"
		" prfm	PLDL1KEEP, [x18, #8]       	  		\n"

		"	ld1		{v11.2s}, [x18]					 					\n"
		"	add 	x18, x18, x6, lsl #1 			 				\n"

		".endm   										 							\n"


		".macro N2_KERNEL1x2_K1                 	\n"

		"	fmla	v12.2s, v11.2s, v0.s[1]			 			\n"
		" prfm  PLDL1KEEP, [x17, #8]       				\n"
		"	ld1		{v10.2s}, [x17]					 					\n"
		"	add		x17, x17, x6, lsl #1 			 				\n"

		".endm									 	 								\n"


		".macro N2_KERNEL1x2_K2            		    \n"

		"	fmla	v12.2s, v10.2s, v0.s[2]			 			\n"

		" prfm  PLDL1KEEP, [x18, #8]       				\n"
		"	ld1		{v11.2s}, [x18] 				 					\n"
		"	add 	x18, x18, x6, lsl #1 			 				\n"

		".endm									 	 								\n"


		".macro N2_KERNEL1x2_K3                 	\n"

		"	prfm   PLDL1KEEP, [x17, #16]       			\n"

		"	ld1		{v10.2s}, [x17]					 					\n"
		"	fmla	v12.2s, v11.2s, v0.s[3]			 			\n"
		"	ldr		q0, [x11], #16					 					\n"
		"	add		x17, x17, x6, lsl #1 			 				\n"

		".endm									 	 								\n"



		".macro N2_KERNEL1x2_END_K              	\n"

		"	fmla	v12.2s, v11.2s, v0.s[3]			 			\n"

		".endm									 	 								\n"


		".macro N2_KERNEL1x2_EDGE_K              	\n"

		"	ld1		{v11.2s}, [x17] 									\n"
		"	ld1		{v0.s}[3], [x11], #4 							\n"
		"	fmla	v12.2s, v11.2s, v0.s[3]			 			\n"
		"	add 	x17, x17, x6 											\n"

		".endm									 	 								\n"



		".macro N2_SAVE1x2							 					\n"

		"	st1		{v12.2s}, [x25]			 		 					\n"

		".endm 									 	 								\n"



		".macro N1_SAVE5x1							 					\n"

		"	subs	x21, x21, #1					 						\n"

		"	add 	x10, x10, x8, lsl #2			 				\n"
		"	add		x10, x10, x8 					 						\n"

		"	faddp	v12.4s, v12.4s, v13.4s 						\n"
		"	faddp	v14.4s, v14.4s, v15.4s 						\n"
		"	faddp	v16.4s, v16.4s, v17.4s  					\n"
		"	faddp	v12.4s, v12.4s, v14.4s 						\n"
		"	faddp s13, v16.2s 											\n"

		"	st1		{v12.s}[0], [x25]			 		 				\n"
		"	add		x25, x29, x6					 						\n"
		"	st1		{v12.s}[1], [x26]			 	     			\n"
		"	add		x26, x25, x6					 						\n"
		"	st1		{v12.s}[2], [x27]					 				\n"
		"	add		x27, x26, x6					 						\n"
		"	st1		{v12.s}[3], [x28]					 				\n"
		"	add		x28, x27, x6					 						\n"
		"	str		s13, [x29]					 							\n"
		"	add		x29, x28, x6					 						\n"

		".endm 									 	 								\n"



		".macro N1_SAVE4x1							 					\n"

		"	faddp	v12.4s, v12.4s, v13.4s 						\n"
		"	faddp	v14.4s, v14.4s, v15.4s 						\n"
		"	faddp	v12.4s, v12.4s, v14.4s 						\n"

		"	st1		{v12.s}[0], [x25]			 		 				\n"
		"	st1		{v12.s}[1], [x26]			 	     			\n"
		"	st1		{v12.s}[2], [x27]					 				\n"
		"	st1		{v12.s}[3], [x28]					 				\n"

		".endm 									 	 								\n"


		".macro N1_SAVE3x1							 					\n"

		"	faddp	v12.4s, v12.4s, v13.4s 						\n"
		"	faddp	v14.4s, v14.4s, v15.4s 						\n"
		"	faddp	v12.4s, v12.4s, v14.4s 						\n"

		"	st1		{v12.s}[0], [x25]			 		 				\n"
		"	st1		{v12.s}[1], [x26]			 	     			\n"
		"	st1		{v12.s}[2], [x27]					 				\n"

		".endm 									 	 								\n"


		".macro N1_SAVE2x1							 					\n"

		"	faddp	v12.4s, v12.4s, v13.4s 						\n"
		"	faddp	v12.4s, v12.4s, v13.4s 						\n"

		"	st1		{v12.s}[0], [x25]			 		 				\n"
		"	st1		{v12.s}[1], [x26]			 	     			\n"

		".endm 									 	 								\n"


		".macro N1_SAVE1x1							 					\n"

		"	faddp	v12.4s, v12.4s, v13.4s 						\n"
		"	faddp	s12, v12.2s 											\n"

		"	str 	s12, [x25]			 		 							\n"

		".endm 									 	 								\n"


//  GEMM begin!!!
		"SMM:									 	 									\n"

		"	ldr		x0, %[C]						 							\n"
		"	ldr		x1, %[A]						 							\n"
		"	ldr		x2, %[B]						 							\n"

		"	ldr		x3, %[M]						 							\n"
		"	ldr		x4, %[N]						 							\n"
		"	ldr		x5, %[K]						 							\n"

		"	lsl		x6, x4, #2						 						\n"  //sizeof(N)

		"	mov		x23, #5						 	 							\n"
		"	lsl		x8, x5, #2						 						\n"  //sizeof(K)

		"   prfm    PLDL1KEEP, [x1]             	\n"
		"   prfm    PLDL1KEEP, [x2]             	\n"

		"	mov		x16, x2 						 							\n"  	//B11
		"	udiv	x30, x3, x23				 	 						\n"	// M / 5
		"	lsr		x20, x4, #4					 	 						\n"	// N / 16

		"	msub	x23, x30, x23, x3 				 				\n"

		"	cmp		x20, #0							 							\n"
		"	beq 	N_EDGE_NN 					 	 						\n"


		"BEGIN_N16:								 	 			 \n"

		"	mov 	x25, x0   						 		 \n"   //C1*
		"	add		x26, x25, x6 					 		 \n"   //C2*
		"	add 	x27, x25, x6, lsl #1 			 \n"   //C3*
		"	add 	x28, x26, x6, lsl #1 			 \n"   //C4*
		"	add		x29, x27, x6, lsl #1 			 \n"   //C5*

		"	mov		x10, x1   						     \n"
		"	cmp		x30, #0 									 \n"
		"	beq 	BEGIN_M4 									 \n"
		"	mov		x21, x30  						     \n"   // M / 5

		//---------------------------------------------------

		"BEGIN_M5:									 			\n"

		"	mov		x17, x16						 			\n"   //B0*
		"	add		x18, x17, x6 					 		\n"

		"	mov		x11, x10						 			\n"
		"	add		x12, x11, x8						 	\n"
		"	add		x13, x12, x8					 		\n"
		"	add		x14, x13, x8					 		\n"
		"	add		x15, x14, x8 					 		\n"

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
		
		"	subs	x22, x22, #1			 	 \n"
		"	b 		Main_K			 				 \n"	

		"Edge_K:									 	 \n"


	//	"	prfm	PLDL1KEEP, [x25, #64]			 \n"			//note
	//	"	prfm	PLDL1KEEP, [x26, #64]			 \n"
	//	"	prfm	PLDL1KEEP, [x27, #64]			 \n"
	//	"	prfm	PLDL1KEEP, [x28, #64]			 \n"
	//	"	prfm	PLDL1KEEP, [x29, #64]			 \n"

		
		"	KERNEL5x16_END_K						 		\n"	


//-----------------------------------------EDGE
		"	ands	x22, x5, #7 							\n"
		"	beq 	BEGIN_SAVE5x16 						\n"
		
		"Edge_K_END: 											\n"
		"	subs	x22, x22, #1 							\n"
		"	KERNEL5x16_EDGE_K								\n"
		"	bgt		Edge_K_END 								\n"

		"BEGIN_SAVE5x16:									\n"

		"	SAVE5x16								 				\n"
		"	bgt   	BEGIN_M5								\n"

		"END_M5:									 				\n"


	//--------------------------------------------------------
		"BEGIN_M4:						 			 			\n"

		"	mov		x17, x16						 			\n"   //B0*
		"	add		x18, x17, x6 					 		\n"

		"	cmp		x23, #4							 			\n"
		"	bne 	BEGIN_M3 						 			\n"

		"	mov		x11, x10 						 				 \n"
		"	add		x12, x11, x8 					 			 \n"
		"	add		x13, x12, x8 					 			 \n"
		"	add		x14, x13, x8 				 	 			 \n"

		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"
		"	prfm    PLDL1KEEP, [x13, #64]			 \n"
		"	prfm    PLDL1KEEP, [x14, #64]			 \n"


		"M4_Body_K:								 	 				\n"

		"	lsr		x22, x5, #2 					 			\n"     // K / 8
		"	KERNEL4x16_BEGIN_K						 		\n"
		"	subs	x22, x22, #1		     		 		\n"	
		"	b 		M4_K1_3							 				\n"

		"M4_Main_K:									 				\n"
		
		"	KERNEL4x16_K0						 	 				\n"
		"M4_K1_3:									 					\n"
		
		"	KERNEL4x16_K1							 				\n"
		"	KERNEL4x16_K2							 				\n"

		"	beq		M4_Edge_K							 			\n"

		"	KERNEL4x16_K3							 				\n"
		
		"	subs	x22, x22, #1			 		 			\n"
		"	b 		M4_Main_K			 			 				\n"	

		"M4_Edge_K:									 				\n"


		"	prfm	PLDL1KEEP, [x25, #64]			 	\n"
		"	prfm	PLDL1KEEP, [x26, #64]			 	\n"
		"	prfm	PLDL1KEEP, [x27, #64]			 	\n"
		"	prfm	PLDL1KEEP, [x28, #64]			 	\n"

		"	KERNEL4x16_END_K						 			\n"

		"	ands	x22, x5, #3 								\n"
		"	beq		BEGIN_SAVE4x16 							\n"


		"M4_Edge_K_END: 										\n"
		"	subs	x22, x22, #1 								\n"
		"	KERNEL4x16_EDGE_K									\n"
		"	bgt		M4_Edge_K_END 							\n"


		"BEGIN_SAVE4x16: 										\n"
		"	SAVE4x16								 					\n"

		"	b 		END_M 							 				\n"


		//--------------------------------------------------------

		"BEGIN_M3:			 						 				\n"
		"	cmp		x23, #3							 				\n"
		"	bne 	BEGIN_M2 						 				\n"

		"	mov		x11, x10 						 				\n"
		"	add		x12, x11, x8 					 			\n"
		"	add		x13, x12, x8 					 			\n"

		"	prfm    PLDL1KEEP, [x11, #64]			\n"
		"	prfm    PLDL1KEEP, [x12, #64]			\n"
		"	prfm    PLDL1KEEP, [x13, #64]			\n"

		"M3_Body_K:								 	 				\n"

		"	lsr		x22, x5, #2 					 			\n"     // K / 8
		"	KERNEL3x16_BEGIN_K						 		\n"
		"	subs	x22, x22, #1		     		 		\n"	
		"	b 		M3_K1_3							 				\n"

		"M3_Main_K:									 				\n"
		
		"	KERNEL3x16_K0						 	 				\n"
		"M3_K1_3:									 					\n"
		
		"	KERNEL3x16_K1							 				\n"
		"	KERNEL3x16_K2							 				\n"

		"	beq		M3_Edge_K							 			\n"

		"	KERNEL3x16_K3							 				\n"
		
		"	subs	x22, x22, #1			 		 			\n"
		"	b 		M3_Main_K			 			 				\n"	

		"M3_Edge_K:									 				\n"

		"	prfm	PLDL1KEEP, [x25, #64]			 	\n"
		"	prfm	PLDL1KEEP, [x26, #64]			 	\n"
		"	prfm	PLDL1KEEP, [x27, #64]			 	\n"
		"	KERNEL3x16_END_K						 			\n"

		"	ands	x22, x5, #3 								\n"
		"	beq		BEGIN_SAVE3x16 							\n"

		"M3_Edge_K_END: 										\n"
		"	subs	x22, x22, #1 								\n"
		"	KERNEL3x16_EDGE_K									\n"
		"	bgt		M3_Edge_K_END 							\n"


		"BEGIN_SAVE3x16: 										\n"

		"	SAVE3x16								 					\n"
		"	b 		END_M 							 				\n"




//----------------------------------------------------------
		"BEGIN_M2:			 						 \n"

		"	cmp		x23, #2							 \n"
		"	bne 	BEGIN_M1 						 \n"

		"	mov		x11, x10 						 \n"
		"	add		x12, x11, x8 					 \n"

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
		"	KERNEL2x16_K2							 			\n"

		"	beq		M2_Edge_K						 			\n"

		"	KERNEL2x16_K3							 			\n"
		
		"	subs	x22, x22, #1			 		 		\n"
		"	b 		M2_Main_K			 			 			\n"	

		"M2_Edge_K:									 			\n"

		"	prfm	PLDL1KEEP, [x25, #64]			\n"
		"	prfm	PLDL1KEEP, [x26, #64]			\n"

		"	KERNEL2x16_END_K						 		\n"


		"	ands	x22, x5, #3 								\n"
		"	beq		BEGIN_SAVE2x16 							\n"

		"M2_Edge_K_END: 										\n"
		"	subs	x22, x22, #1 								\n"
		"	KERNEL2x16_EDGE_K									\n"
		"	bgt		M2_Edge_K_END 							\n"


		"BEGIN_SAVE2x16: 										\n"

		"	SAVE2x16								 				\n"
		"	b 		END_M 							 			\n"




//---------------------------------------------------------		

		"BEGIN_M1:									 \n"

		"	cmp		x23, #1							 \n"
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

		"	KERNEL1x16_K3							 			\n"
		
		"	subs	x22, x22, #1			 		 		\n"
		"	b 		M1_Main_K			 			 			\n"	

		"M1_Edge_K:									 			\n"

		"	prfm	PLDL1KEEP, [x25, #64]			\n"

		"	KERNEL1x16_END_K						 		\n"


		"	ands	x22, x5, #3 								\n"
		"	beq		BEGIN_SAVE1x16 							\n"

		"M1_Edge_K_END: 										\n"
		"	subs	x22, x22, #1 								\n"
		"	KERNEL1x16_EDGE_K									\n"
		"	bgt		M1_Edge_K_END 							\n"

		"BEGIN_SAVE1x16: 										\n"


		"	SAVE1x16											 	\n"

		"END_M:										 				\n"


		"	subs	x20, x20, #1					 		\n"

		"	add		x16, x16, #64					 		\n"
		"	add		x0, x0, #64						 		\n"

		"	bgt		BEGIN_N16						 			\n"

		"END_N16:									 				\n"






//
//++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++

//

		"N_EDGE_NN: 													\n"

		"	ands	x20, x4, #15					 				\n"
		"	beq		END_N 							 					\n"

		"	cmp		x20, #8 											\n"
		"	blt		BEGIN_N4 											\n"

//----------------------------------------------------------N8
		"BEGIN_N8:									 					\n"

		"	mov 	x25, x0   						 				\n"   //C1*
		"	add		x26, x25, x6 					 				\n"   //C2*
		"	add 	x27, x25, x6, lsl #1 					\n"   //C3*
		" add 	x28, x26, x6, lsl #1 					\n"   //C4*
		"	add		x29, x27, x6, lsl #1 					\n"   //C5*

		"	mov		x10, x1   						 				\n"   //A0*
		"	cmp		x30, #0 											\n"
		"	beq		N8_BEGIN_M4 									\n"
		"	mov		x21, x30  						 				\n"   // M / 5

//--------------------------------------------------N8M5
		"N8_BEGIN_M5:								 					\n"

		"	mov		x17, x16						 					\n"   //B0*
		"	add		x18, x17, x6 					 				\n"   //B1*

		"	mov		x11, x10						 					\n"
		"	add		x12, x11, x8					 				\n"
		"	add		x13, x12, x8					 				\n"
		"	add		x14, x13, x8					 				\n"
		"	add		x15, x14, x8 					 				\n"

		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"
		"	prfm    PLDL1KEEP, [x13, #64]			 \n"
		"	prfm    PLDL1KEEP, [x14, #64]			 \n"
		"	prfm    PLDL1KEEP, [x15, #64]			 \n"


		"N8_Body_K:								 	 	\n"

		"	lsr		x22, x5, #2 					\n"     // K / 8
		"	N8_KERNEL5x8_BEGIN_K				\n"
		"	subs	x22, x22, #1		     	\n"	
		"	b 		N8_K1_3							 	\n"

		"N8_Main_K:									 	\n"
		

		"	N8_KERNEL5x8_K0						 	 \n"
		"N8_K1_3:									 		\n"
		
		"	N8_KERNEL5x8_K1							 \n"
		"	N8_KERNEL5x8_K2							 \n"

		"	beq		N8_Edge_K						 \n"

		"	N8_KERNEL5x8_K3							 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		N8_Main_K			 			 \n"	

		"N8_Edge_K:									 \n"


		"	prfm	PLDL1KEEP, [x25, #64]			 		\n"
		"	prfm	PLDL1KEEP, [x26, #64]			 		\n"
		"	prfm	PLDL1KEEP, [x27, #64]			 		\n"
		"	prfm	PLDL1KEEP, [x28, #64]			 		\n"
		"	prfm	PLDL1KEEP, [x29, #64]			 		\n"

		"	N8_KERNEL5x8_END_K						 			\n"


		"	ands	x22, x5, #3 									\n"
		"	beq		BEGIN_SAVE5x8 								\n"

		"N8M5_Edge_K_END: 										\n"
		"	subs	x22, x22, #1 									\n"
		"	N8_KERNEL5x8_EDGE_K									\n"
		"	bgt		N8M5_Edge_K_END 							\n"

		"BEGIN_SAVE5x8: 											\n"		

		"	N8_SAVE5x8								 					\n"
		"	bgt   	N8_BEGIN_M5					 	 			\n"





//---------------------------------------------------------

		"N8_END_M5:									 					\n"



	//--------------------------------------------------------
		"N8_BEGIN_M4:						 			 				\n"

		"	mov		x17, x16						 					\n"   //B0*
		"	add		x18, x17, x6 					 				\n"   //B1*

		"	cmp		x23, #4							 					\n"
		"	bne 	N8_BEGIN_M3 					 				\n"

		"	mov		x11, x10 						 					\n"
		"	add		x12, x11, x8 					 				\n"
		"	add		x13, x12, x8 					 				\n"
		"	add		x14, x13, x8 				 	 				\n"

		"	prfm    PLDL1KEEP, [x11, #64]			 	\n"
		"	prfm    PLDL1KEEP, [x12, #64]			 	\n"
		"	prfm    PLDL1KEEP, [x13, #64]			 	\n"
		"	prfm    PLDL1KEEP, [x14, #64]			 	\n"


		"N8_M4_Body_K:								 				\n"

		"	lsr		x22, x5, #2 					 				\n"     // K / 8
		"	N8_KERNEL4x8_BEGIN_K					 			\n"
		"	subs	x22, x22, #1		     		 			\n"	
		"	b 		N8_M4_K1_3						 				\n"

		"N8_M4_Main_K:								 				\n"
		
		"	N8_KERNEL4x8_K0						 	 				\n"
		"N8_M4_K1_3:									 				\n"
		
		"	N8_KERNEL4x8_K1						 	 				\n"
		"	N8_KERNEL4x8_K2						 	 				\n"

		"	beq		N8_M4_Edge_K					 				\n"

		"	N8_KERNEL4x8_K3						 	 				\n"
		
		"	subs	x22, x22, #1			 		 				\n"
		"	b 		N8_M4_Main_K			 		 				\n"	

		"N8_M4_Edge_K:								 				\n"


		"	prfm	PLDL1KEEP, [x25, #32]			 		\n"
		"	prfm	PLDL1KEEP, [x26, #32]			 		\n"
		"	prfm	PLDL1KEEP, [x27, #32]			 		\n"
		"	prfm	PLDL1KEEP, [x28, #32]			 		\n"

		"	N8_KERNEL4x8_END_K						 			\n"

		"	ands	x22, x5, #3 									\n"
		"	beq		BEGIN_SAVE4x8 								\n"

		"N8M4_Edge_K_END: 										\n"
		"	subs	x22, x22, #1 									\n"
		"	N8_KERNEL4x8_EDGE_K									\n"
		"	bgt		N8M4_Edge_K_END 							\n"

		"BEGIN_SAVE4x8: 											\n"		

		"	N8_SAVE4x8								 					\n"

		"	b 		N8_END_M 						 					\n"


//------------------------------------------------------------------

		"N8_BEGIN_M3: 								 \n"

		"	cmp		x23, #3							 \n"
		"	bne 	N8_BEGIN_M2 					 \n"

		"	mov		x11, x10 						 \n"
		"	add		x12, x11, x8 					 \n"
		"	add		x13, x12, x8 					 \n"

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
		"N8_M3_K1_3:								 				\n"
		
		"	N8_KERNEL3x8_K1						 	 			\n"
		"	N8_KERNEL3x8_K2						 	 			\n"

		"	beq		N8_M3_Edge_K					 			\n"

		"	N8_KERNEL3x8_K3						 	 			\n"
		
		"	subs	x22, x22, #1			 		 			\n"
		"	b 		N8_M3_Main_K			 		 			\n"	

		"N8_M3_Edge_K:								 			\n"


		"	prfm	PLDL1KEEP, [x25, #32]			 		\n"
		"	prfm	PLDL1KEEP, [x26, #32]			 		\n"
		"	prfm	PLDL1KEEP, [x27, #32]			 		\n"

		"	N8_KERNEL3x8_END_K						 	 		\n"

		"	ands	x22, x5, #3 									\n"
		"	beq		BEGIN_SAVE3x8 								\n"

		"N8M3_Edge_K_END: 										\n"
		"	subs	x22, x22, #1 									\n"
		"	N8_KERNEL3x8_EDGE_K									\n"
		"	bgt		N8M3_Edge_K_END 							\n"

		"BEGIN_SAVE3x8: 											\n"		


		"	N8_SAVE3x8								 			 \n"

		"	b 		N8_END_M 						 			 \n"





//--------------------------------------------------------

		"N8_BEGIN_M2:								 \n"


		"	cmp		x23, #2							 \n"
		"	bne 	N8_BEGIN_M1 					 \n"

		"	mov		x11, x10 						 \n"
		"	add		x12, x11, x8 					 \n"

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

		"	beq		N8_M2_Edge_K					 			\n"

		"	N8_KERNEL2x8_K3						 	 			\n"
		
		"	subs	x22, x22, #1			 		 			\n"
		"	b 		N8_M2_Main_K			 		 			\n"	

		"N8_M2_Edge_K:								 			\n"


		"	prfm	PLDL1KEEP, [x25, #32]			 	\n"
		"	prfm	PLDL1KEEP, [x26, #32]			 	\n"

		"	N8_KERNEL2x8_END_K						 		\n"

		"	ands	x22, x5, #3 									\n"
		"	beq		BEGIN_SAVE2x8 								\n"

		"N8M2_Edge_K_END: 										\n"
		"	subs	x22, x22, #1 									\n"
		"	N8_KERNEL2x8_EDGE_K									\n"
		"	bgt		N8M2_Edge_K_END 							\n"

		"BEGIN_SAVE2x8: 											\n"		


		"	N8_SAVE2x8								 				\n"

		"	b 		N8_END_M 						 				\n"





//-----------------------------------------------------
		"N8_BEGIN_M1:								 				\n"


		"	cmp		x23, #1							 				\n"
		"	bne 	N8_END_M 					 	 				\n"

		"	mov		x11, x10 						 				\n"

		"	prfm    PLDL1KEEP, [x11, #64]			\n"

		"N8_M1_Body_K:								 			\n"

		"	lsr		x22, x5, #2 					 			\n"     // K / 8
		"	N8_KERNEL1x8_BEGIN_K					 		\n"
		"	subs	x22, x22, #1		     		 		\n"	
		"	b 		N8_M1_K1_3						 			\n"

		"N8_M1_Main_K:								 			\n"
		
		"	N8_KERNEL1x8_K0						 	 			\n"
		"N8_M1_K1_3:								 		 		\n"
		
		"	N8_KERNEL1x8_K1						 	 			\n"
		"	N8_KERNEL1x8_K2						 	 			\n"

		"	beq		N8_M1_Edge_K					 			\n"

		"	N8_KERNEL1x8_K3						 	 			\n"
		
		"	subs	x22, x22, #1			 		 			\n"
		"	b 		N8_M1_Main_K			 		 				\n"	

		"N8_M1_Edge_K:								 				\n"

		"	prfm	PLDL1KEEP, [x25, #32]			  	\n"

		"	N8_KERNEL1x8_END_K						 			\n"	


		"	ands	x22, x5, #3 									\n"
		"	beq		BEGIN_SAVE1x8 								\n"

		"N8M1_Edge_K_END: 										\n"
		"	subs	x22, x22, #1 									\n"
		"	N8_KERNEL1x8_EDGE_K									\n"
		"	bgt		N8M1_Edge_K_END 							\n"

		"BEGIN_SAVE1x8: 											\n"		

		"	N8_SAVE1x8								 					\n"

		"N8_END_M: 									 					\n"

		"	sub 	x20, x20, #8 									\n"
		"	add 	x16, x16, #32 								\n"
		"	add		x0, x0, #32						 				\n"



//
//++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++
// N4
//

//----------------------------------------------------------N4
		"BEGIN_N4:									 				\n"

		"	cmp 	x20, #4 										\n"
		"	blt		BEGIN_N2 										\n"

		"	mov 	x25, x0   						 			\n"   //C1*
		"	add		x26, x25, x6 					 			\n"   //C2*
		"	add 	x27, x25, x6, lsl #1 				\n"   //C3*
		" add 	x28, x26, x6, lsl #1 				\n"   //C4*
		"	add		x29, x27, x6, lsl #1 				\n"   //C5*

		"	mov		x10, x1   						 			\n"   //A0*
		"	cmp		x30, #0 										\n"
		"	beq		N4_BEGIN_M4 								\n"
		"	mov		x21, x30  						 			\n"   // M / 5
//--------------------------------------------------N8M5
		"N4_BEGIN_M5:								 				\n"

		"	mov		x17, x16						 				\n"   //B0*
		"	add		x18, x17, x6 					 			\n"   //B1*

		"	mov		x11, x10						 				\n"
		"	add		x12, x11, x8					 			\n"
		"	add		x13, x12, x8					 			\n"
		"	add		x14, x13, x8					 			\n"
		"	add		x15, x14, x8 					 			\n"

		"	prfm    PLDL1KEEP, [x11, #64]			\n"
		"	prfm    PLDL1KEEP, [x12, #64]			\n"
		"	prfm    PLDL1KEEP, [x13, #64]			\n"
		"	prfm    PLDL1KEEP, [x14, #64]			\n"
		"	prfm    PLDL1KEEP, [x15, #64]			\n"


		"N4_Body_K:								 	 				\n"

		"	lsr		x22, x5, #2 					 			\n"     // K / 4
		"	N4_KERNEL5x4_BEGIN_K					 		\n"
		"	subs	x22, x22, #1		     		 		\n"	
		"	b 		N4_K1_3							 				\n"

		"N4_Main_K:									 				\n"
		

		"	N4_KERNEL5x4_K0						 	 			\n"
		"N4_K1_3:									 					\n"
		
		"	N4_KERNEL5x4_K1							 			\n"
		"	N4_KERNEL5x4_K2							 			\n"

		"	beq		N4_Edge_K						 				\n"

		"	N4_KERNEL5x4_K3							 			\n"
		
		"	subs	x22, x22, #1			 		 			\n"
		"	b 		N4_Main_K			 			 				\n"	

		"N4_Edge_K:									 				\n"

		"	N4_KERNEL5x4_END_K						 	 	\n"	

		"	ands	x22, x5, #3 									\n"
		"	beq		BEGIN_SAVE5x4 								\n"

		"N4M5_Edge_K_END: 										\n"
		"	subs	x22, x22, #1 									\n"
		"	N4_KERNEL5x4_EDGE_K									\n"
		"	bgt		N4M5_Edge_K_END 							\n"

		"BEGIN_SAVE5x4: 											\n"		


		"	N4_SAVE5x4								 			 	\n"
		"	bgt   	N4_BEGIN_M5					 	 	 	\n"





//---------------------------------------------------------

		"N4_END_M5:									 					\n"

	//--------------------------------------------------------
		"N4_BEGIN_M4:						 			 				\n"

		"	mov		x17, x16						 					\n"   //B0*
		"	add		x18, x17, x6 					 				\n"   //B1*

		"	cmp		x23, #4							 					\n"
		"	bne 	N4_BEGIN_M3 					 				\n"

		"	mov		x11, x10 						 					\n"
		"	add		x12, x11, x8 					 				\n"
		"	add		x13, x12, x8 					 				\n"
		"	add		x14, x13, x8 				 	 				\n"

		"	prfm  PLDL1KEEP, [x11, #64]			 		\n"
		"	prfm  PLDL1KEEP, [x12, #64]			 		\n"
		"	prfm  PLDL1KEEP, [x13, #64]			 		\n"
		"	prfm  PLDL1KEEP, [x14, #64]			 		\n"


		"N4_M4_Body_K:								 				\n"

		"	lsr		x22, x5, #2 					 				\n"     // K / 8
		"	N4_KERNEL4x4_BEGIN_K					 			\n"
		"	subs	x22, x22, #1		     		 			\n"	
		"	b 		N4_M4_K1_3						 				\n"

		"N4_M4_Main_K:								 				\n"
		
		"	N4_KERNEL4x4_K0						 	 				\n"
		"N4_M4_K1_3:									 				\n"
		
		"	N4_KERNEL4x4_K1						 	 				\n"
		"	N4_KERNEL4x4_K2						 	 				\n"

		"	beq		N4_M4_Edge_K					 				\n"

		"	N4_KERNEL4x4_K3						 	 				\n"
		
		"	subs	x22, x22, #1			 		 				\n"
		"	b 		N4_M4_Main_K			 		 				\n"	

		"N4_M4_Edge_K:								 				\n"

		"	N4_KERNEL4x4_END_K						 			\n"	

		"	ands	x22, x5, #3 									\n"
		"	beq		BEGIN_SAVE4x4 								\n"

		"N4M4_Edge_K_END: 										\n"
		"	subs	x22, x22, #1 									\n"
		"	N4_KERNEL4x4_EDGE_K									\n"
		"	bgt		N4M4_Edge_K_END 							\n"

		"BEGIN_SAVE4x4: 											\n"		


		"	N4_SAVE4x4								 					\n"

		"	b 		N4_END_M 						 					\n"


//------------------------------------------------------------------


		"N4_BEGIN_M3: 								 				\n"

		"	cmp		x23, #3							 					\n"
		"	bne 	N4_BEGIN_M2 					 				\n"

		"	mov		x11, x10 						 					\n"
		"	add		x12, x11, x8 					 				\n"
		"	add		x13, x12, x8 					 				\n"

		"	prfm    PLDL1KEEP, [x11, #64]			 	\n"
		"	prfm    PLDL1KEEP, [x12, #64]			 	\n"
		"	prfm    PLDL1KEEP, [x13, #64]			 	\n"


		"N4_M3_Body_K:										 		\n"

		"	lsr		x22, x5, #2 					 				\n"     // K / 8
		"	N4_KERNEL3x4_BEGIN_K					 			\n"
		"	subs	x22, x22, #1		     		 			\n"	
		"	b 		N4_M3_K1_3						 				\n"

		"N4_M3_Main_K:								 				\n"
		
		"	N4_KERNEL3x4_K0						 	 				\n"
		"N4_M3_K1_3:								 					\n"
		
		"	N4_KERNEL3x4_K1						 	 				\n"
		"	N4_KERNEL3x4_K2						 	 				\n"

		"	beq		N4_M3_Edge_K					 				\n"

		"	N4_KERNEL3x4_K3						 	 				\n"
		
		"	subs	x22, x22, #1			 		 				\n"
		"	b 		N4_M3_Main_K			 		 				\n"	

		"N4_M3_Edge_K:								 				\n"

		"	N4_KERNEL3x4_END_K						 			\n"	


		"	ands	x22, x5, #3 									\n"
		"	beq		BEGIN_SAVE3x4 								\n"

		"N4M3_Edge_K_END: 										\n"
		"	subs	x22, x22, #1 									\n"
		"	N4_KERNEL3x4_EDGE_K									\n"
		"	bgt		N4M3_Edge_K_END 							\n"

		"BEGIN_SAVE3x4: 											\n"		

		"	N4_SAVE3x4								 					\n"

		"	b 		N4_END_M 						 					\n"





//--------------------------------------------------------

		"N4_BEGIN_M2:								 					\n"


		"	cmp		x23, #2							 					\n"
		"	bne 	N4_BEGIN_M1 					 				\n"

		"	mov		x11, x10 						 					\n"
		"	add		x12, x11, x8 					 				\n"

		"	prfm    PLDL1KEEP, [x11, #64]			 	\n"
		"	prfm    PLDL1KEEP, [x12, #64]			 	\n"


		"N4_M2_Body_K:								 				\n"

		"	lsr		x22, x5, #2 					 				\n"     // K / 4
		"	N4_KERNEL2x4_BEGIN_K					 			\n"
		"	subs	x22, x22, #1		     		 			\n"	
		"	b 		N4_M2_K1_3						 				\n"

		"N4_M2_Main_K:								 				\n"
		
		"	N4_KERNEL2x4_K0						 	 				\n"
		"N4_M2_K1_3:								 					\n"
		
		"	N4_KERNEL2x4_K1						 	 				\n"
		"	N4_KERNEL2x4_K2						 	 				\n"

		"	beq		N4_M2_Edge_K					 				\n"

		"	N4_KERNEL2x4_K3						 	 				\n"
		
		"	subs	x22, x22, #1			 		 				\n"
		"	b 		N4_M2_Main_K			 		 				\n"	

		"N4_M2_Edge_K:								 				\n"

		"	N4_KERNEL2x4_END_K								 	\n"	

		"	ands	x22, x5, #3 									\n"
		"	beq		BEGIN_SAVE2x4 								\n"

		"N4M2_Edge_K_END: 										\n"
		"	subs	x22, x22, #1 									\n"
		"	N4_KERNEL2x4_EDGE_K									\n"
		"	bgt		N4M2_Edge_K_END 							\n"

		"BEGIN_SAVE2x4: 											\n"		

		"	N4_SAVE2x4								 					\n"

		"	b 		N4_END_M 						 					\n"





//-----------------------------------------------------
		"N4_BEGIN_M1:								 				\n"


		"	cmp		x23, #1							 				\n"
		"	bne 	N4_END_M 					 	 				\n"

		"	mov		x11, x10 						 				\n"
		"	prfm  PLDL1KEEP, [x11, #64]				\n"

		"N4_M1_Body_K:								 			\n"

		"	lsr		x22, x5, #2 					 			\n"     // K / 8
		"	N4_KERNEL1x4_BEGIN_K					 		\n"
		"	subs	x22, x22, #1		     		 		\n"	
		"	b 		N4_M1_K1_3						 			\n"

		"N4_M1_Main_K:								 			\n"
		
		"	N4_KERNEL1x4_K0						 	 			\n"
		"N4_M1_K1_3:								 		 		\n"
		
		"	N4_KERNEL1x4_K1						 	 			\n"
		"	N4_KERNEL1x4_K2						 	 			\n"

		"	beq		N4_M1_Edge_K					 			\n"

		"	N4_KERNEL1x4_K3						 	 			\n"
		
		"	subs	x22, x22, #1			 		 			\n"
		"	b 		N4_M1_Main_K			 		 			\n"	

		"N4_M1_Edge_K:								 			\n"

		"	N4_KERNEL1x4_END_K						 		\n"		


		"	ands	x22, x5, #3 									\n"
		"	beq		BEGIN_SAVE1x4 								\n"

		"N4M1_Edge_K_END: 										\n"
		"	subs	x22, x22, #1 									\n"
		"	N4_KERNEL1x4_EDGE_K									\n"
		"	bgt		N4M1_Edge_K_END 							\n"

		"BEGIN_SAVE1x4: 											\n"		

		"	N4_SAVE1x4								 				\n"

		"N4_END_M: 									 				\n"

		"	sub 	x20, x20, #4 								\n"
		"	add 	x16, x16, #16 							\n"
		"	add		x0, x0, #16						 			\n"


//-----------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++
// N2
//++++++++++++++++++++++++++++++++++++++++++++		
//-----------------------------------------------------------


		"BEGIN_N2:									 				\n"

		"	cmp 	x20, #2 										\n"
		"	blt		BEGIN_N1 										\n"

		"	mov 	x25, x0   						 			\n"   //C1*
		"	add		x26, x25, x6 					 			\n"   //C2*
		"	add 	x27, x25, x6, lsl #1 				\n"   //C3*
		" add 	x28, x26, x6, lsl #1 				\n"   //C4*
		"	add		x29, x27, x6, lsl #1 				\n"   //C5*

		"	mov		x10, x1   						 			\n"   //A0*
		"	cmp		x30, #0 										\n"
		"	beq		N2_BEGIN_M4 								\n"
		"	mov		x21, x30  						 			\n"   // M / 5
//--------------------------------------------------N8M5
		"N2_BEGIN_M5:								 				\n"

		"	mov		x17, x16						 				\n"   //B0*
		"	add		x18, x17, x6 					 			\n"   //B1*

		"	mov		x11, x10						 				\n"
		"	add		x12, x11, x8					 			\n"
		"	add		x13, x12, x8					 			\n"
		"	add		x14, x13, x8					 			\n"
		"	add		x15, x14, x8 					 			\n"

		"	prfm   PLDL1KEEP, [x11, #64]			\n"
		"	prfm   PLDL1KEEP, [x12, #64]			\n"
		"	prfm   PLDL1KEEP, [x13, #64]			\n"
		"	prfm   PLDL1KEEP, [x14, #64]			\n"
		"	prfm   PLDL1KEEP, [x15, #64]			\n"


		"N2_Body_K:								 	 				\n"

		"	lsr		x22, x5, #2 					 			\n"     // K / 4
		"	N2_KERNEL5x2_BEGIN_K					 		\n"
		"	subs	x22, x22, #1		     		 		\n"	
		"	b 		N2_K1_3							 				\n"

		"N2_Main_K:									 				\n"
		
		"	N2_KERNEL5x2_K0						 	 			\n"
		"N2_K1_3:									 					\n"
		
		"	N2_KERNEL5x2_K1							 			\n"
		"	N2_KERNEL5x2_K2							 			\n"

		"	beq		N2_Edge_K						 				\n"

		"	N2_KERNEL5x2_K3							 			\n"
		
		"	subs	x22, x22, #1			 		 			\n"
		"	b 		N2_Main_K			 			 				\n"	

		"N2_Edge_K:									 				\n"

		"	N2_KERNEL5x2_END_K						 	 	\n"


		"	ands	x22, x5, #3 								\n"
		"	beq		BEGIN_SAVE5x2 							\n"

		"N2M5_Edge_K_END: 									\n"
		"	subs	x22, x22, #1 								\n"
		"	N2_KERNEL5x2_EDGE_K								\n"
		"	bgt		N2M5_Edge_K_END 						\n"

		"BEGIN_SAVE5x2: 										\n"		

		"	N2_SAVE5x2								 			 	\n"
		"	bgt   	N2_BEGIN_M5					 	 	 	\n"





//---------------------------------------------------------

		"N2_END_M5:									 					\n"

	//--------------------------------------------------------
		"N2_BEGIN_M4:						 			 				\n"

		"	mov		x17, x16						 					\n"   //B0*
		"	add		x18, x17, x6 					 				\n"   //B1*

		"	cmp		x23, #4							 					\n"
		"	blt 	N2_BEGIN_M3 					 				\n"

		"	mov		x11, x10 						 					\n"
		"	add		x12, x11, x8 					 				\n"
		"	add		x13, x12, x8 					 				\n"
		"	add		x14, x13, x8 				 	 				\n"

		"	prfm  PLDL1KEEP, [x11, #64]			 		\n"
		"	prfm  PLDL1KEEP, [x12, #64]			 		\n"
		"	prfm  PLDL1KEEP, [x13, #64]			 		\n"
		"	prfm  PLDL1KEEP, [x14, #64]			 		\n"


		"N2_M4_Body_K:								 				\n"

		"	lsr		x22, x5, #2 					 				\n"     // K / 8
		"	N2_KERNEL4x2_BEGIN_K					 			\n"
		"	subs	x22, x22, #1		     		 			\n"	
		"	b 		N2_M4_K1_3						 				\n"

		"N2_M4_Main_K:								 				\n"
		
		"	N2_KERNEL4x2_K0						 	 				\n"
		"N2_M4_K1_3:									 				\n"
		
		"	N2_KERNEL4x2_K1						 	 				\n"
		"	N2_KERNEL4x2_K2						 	 				\n"

		"	beq		N2_M4_Edge_K					 				\n"

		"	N2_KERNEL4x2_K3						 	 				\n"
		
		"	subs	x22, x22, #1			 		 				\n"
		"	b 		N2_M4_Main_K			 		 				\n"	

		"N2_M4_Edge_K:								 				\n"

		"	N2_KERNEL4x2_END_K						 			\n"		

		"	ands	x22, x5, #3 								\n"
		"	beq		BEGIN_SAVE4x2 							\n"

		"N2M4_Edge_K_END: 									\n"
		"	subs	x22, x22, #1 								\n"
		"	N2_KERNEL4x2_EDGE_K								\n"
		"	bgt		N2M4_Edge_K_END 						\n"

		"BEGIN_SAVE4x2: 										\n"		

		"	N2_SAVE4x2								 					\n"

		"	b 		N2_END_M 						 					\n"


//------------------------------------------------------------------


		"N2_BEGIN_M3: 								 				\n"

		"	cmp		x23, #3							 					\n"
		"	blt 	N2_BEGIN_M2 					 				\n"

		"	mov		x11, x10 						 					\n"
		"	add		x12, x11, x8 					 				\n"
		"	add		x13, x12, x8 					 				\n"

		"	prfm    PLDL1KEEP, [x11, #64]			 	\n"
		"	prfm    PLDL1KEEP, [x12, #64]			 	\n"
		"	prfm    PLDL1KEEP, [x13, #64]			 	\n"


		"N2_M3_Body_K:										 		\n"

		"	lsr		x22, x5, #2 					 				\n"     // K / 8
		"	N2_KERNEL3x2_BEGIN_K					 			\n"
		"	subs	x22, x22, #1		     		 			\n"	
		"	b 		N2_M3_K1_3						 				\n"

		"N2_M3_Main_K:								 				\n"
		
		"	N2_KERNEL3x2_K0						 	 				\n"
		"N2_M3_K1_3:								 					\n"
		
		"	N2_KERNEL3x2_K1						 	 				\n"
		"	N2_KERNEL3x2_K2						 	 				\n"

		"	beq		N2_M3_Edge_K					 				\n"

		"	N2_KERNEL3x2_K3						 	 				\n"
		
		"	subs	x22, x22, #1			 		 				\n"
		"	b 		N2_M3_Main_K			 		 				\n"	

		"N2_M3_Edge_K:								 				\n"

		"	N2_KERNEL3x2_END_K						 			\n"	

		"	ands	x22, x5, #3 									\n"
		"	beq		BEGIN_SAVE3x2 								\n"

		"N2M3_Edge_K_END: 										\n"
		"	subs	x22, x22, #1 									\n"
		"	N2_KERNEL3x2_EDGE_K									\n"
		"	bgt		N2M3_Edge_K_END 							\n"

		"BEGIN_SAVE3x2: 											\n"		

		"	N2_SAVE3x2								 					\n"

		"	b 		N2_END_M 						 					\n"





//--------------------------------------------------------

		"N2_BEGIN_M2:								 					\n"


		"	cmp		x23, #2							 					\n"
		"	bne 	N2_BEGIN_M1 					 				\n"

		"	mov		x11, x10 						 					\n"
		"	add		x12, x11, x8 					 				\n"

		"	prfm    PLDL1KEEP, [x11, #64]			 	\n"
		"	prfm    PLDL1KEEP, [x12, #64]			 	\n"


		"N2_M2_Body_K:								 				\n"

		"	lsr		x22, x5, #2 					 				\n"     // K / 4
		"	N2_KERNEL2x2_BEGIN_K					 			\n"
		"	subs	x22, x22, #1		     		 			\n"	
		"	b 		N2_M2_K1_3						 				\n"

		"N2_M2_Main_K:								 				\n"
		
		"	N2_KERNEL2x2_K0						 	 				\n"
		"N2_M2_K1_3:								 					\n"
		
		"	N2_KERNEL2x2_K1						 	 				\n"
		"	N2_KERNEL2x2_K2						 	 				\n"

		"	beq		N2_M2_Edge_K					 				\n"

		"	N2_KERNEL2x2_K3						 	 				\n"
		
		"	subs	x22, x22, #1			 		 				\n"
		"	b 		N2_M2_Main_K			 		 				\n"	

		"N2_M2_Edge_K:								 				\n"

		"	N2_KERNEL2x2_END_K								 	\n"		

		"	ands	x22, x5, #3 									\n"
		"	beq		BEGIN_SAVE2x2 								\n"

		"N2M2_Edge_K_END: 										\n"
		"	subs	x22, x22, #1 									\n"
		"	N2_KERNEL2x2_EDGE_K									\n"
		"	bgt		N2M2_Edge_K_END 							\n"

		"BEGIN_SAVE2x2: 											\n"		


		"	N2_SAVE2x2								 					\n"

		"	b 		N2_END_M 						 					\n"





//-----------------------------------------------------
		"N2_BEGIN_M1:								 				\n"


		"	cmp		x23, #1							 				\n"
		"	bne 	N2_END_M 					 	 				\n"

		"	mov		x11, x10 						 				\n"
		"	prfm  PLDL1KEEP, [x11, #64]				\n"

		"N2_M1_Body_K:								 			\n"

		"	lsr		x22, x5, #2 					 			\n"     // K / 8
		"	N2_KERNEL1x2_BEGIN_K					 		\n"
		"	subs	x22, x22, #1		     		 		\n"	
		"	b 		N2_M1_K1_3						 			\n"

		"N2_M1_Main_K:								 			\n"
		
		"	N2_KERNEL1x2_K0						 	 			\n"
		"N2_M1_K1_3:								 		 		\n"
		
		"	N2_KERNEL1x2_K1						 	 			\n"
		"	N2_KERNEL1x2_K2						 	 			\n"

		"	beq		N2_M1_Edge_K					 			\n"

		"	N2_KERNEL1x2_K3						 	 			\n"
		
		"	subs	x22, x22, #1			 		 			\n"
		"	b 		N2_M1_Main_K			 		 			\n"	

		"N2_M1_Edge_K:								 			\n"

		"	N2_KERNEL1x2_END_K						 		\n"	

		"	ands	x22, x5, #3 									\n"
		"	beq		BEGIN_SAVE1x2 								\n"

		"N2M1_Edge_K_END: 										\n"
		"	subs	x22, x22, #1 									\n"
		"	N2_KERNEL1x2_EDGE_K									\n"
		"	bgt		N2M1_Edge_K_END 							\n"

		"BEGIN_SAVE1x2: 											\n"		
		"	N2_SAVE1x2								 				\n"

		"N2_END_M: 									 				\n"

		"	sub 		x20, x20, #2 							\n"
		"	add 		x16, x16, #8 							\n"
		"	add 		x0, x0, #8 								\n"


//---------------------------------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//N1		
//---------------------------------------------------------------
		"BEGIN_N1: 													\n"

		"	cmp 	x20, #1 										\n"
		"	blt		END_N 											\n"

		"	mov 	x25, x0   						 			\n"   //C1*
		"	add		x26, x25, x6 					 			\n"   //C2*
		"	add 	x27, x25, x6, lsl #1 				\n"   //C3*
		" add 	x28, x26, x6, lsl #1 				\n"   //C4*
		"	add		x29, x27, x6, lsl #1 				\n"   //C5*

		"	mov		x10, x1   						 			\n"   //A0*
		"	cmp		x30, #0 										\n"
		"	beq		N1_BEGIN_M4 								\n"
		"	mov		x21, x30  						 			\n"   // M / 5
//--------------------------------------------------N8M5
		"N1_BEGIN_M5:								 				\n"

		"	mov		x17, x16						 				\n"   //B0*
		"	add		x18, x17, x6 					 			\n"   //B1*

		"	mov		x11, x10						 				\n"
		"	add		x12, x11, x8					 			\n"
		"	add		x13, x12, x8					 			\n"
		"	add		x14, x13, x8					 			\n"
		"	add		x15, x14, x8 					 			\n"

		"	prfm   PLDL1KEEP, [x11, #64]			\n"
		"	prfm   PLDL1KEEP, [x12, #64]			\n"
		"	prfm   PLDL1KEEP, [x13, #64]			\n"
		"	prfm   PLDL1KEEP, [x14, #64]			\n"
		"	prfm   PLDL1KEEP, [x15, #64]			\n"


		"N1_Body_K:								 	 				\n"

		"	lsr		x22, x5, #2 					 			\n"     // K / 4

		"	dup		v12.4s, wzr 								\n"
		"	dup		v13.4s, wzr 								\n"
		"	dup		v14.4s, wzr 								\n"
		"	dup		v15.4s, wzr 								\n"
		"	dup		v16.4s, wzr 								\n"

		"N1_Main_K: 												\n"

		"	subs	x22, x22, #1    						\n"

		"	ld1 	{v5.s}[0], [x17] 						\n"
		"	add 	x17, x17, x6, lsl #1 				\n"
		"	ld1 	{v5.s}[1], [x18] 						\n"
		"	add 	x18, x18, x6, lsl #1 				\n"
		"	ld1 	{v5.s}[2], [x17] 						\n"
		"	add 	x17, x17, x6, lsl #1 				\n"
		"	ld1 	{v5.s}[3], [x18] 						\n"
		"	add 	x18, x18, x6, lsl #1 				\n"

		"	ldr		q0, [x11], #16 							\n"
		"	prfm   PLDL1KEEP, [x11, #64]			\n"
		"	ldr		q1, [x12], #16 							\n"
		"	fmla	v12.4s, v5.4s, v0.4s 				\n"
		"	fmla	v13.4s, v5.4s, v1.4s 				\n"
		"	prfm   PLDL1KEEP, [x12, #64]			\n"
		"	ldr		q2, [x13], #16 							\n"
		"	prfm   PLDL1KEEP, [x13, #64]			\n"
		"	ldr		q3, [x14], #16 							\n"
		"	prfm   PLDL1KEEP, [x14, #64]			\n"
		"	fmla	v14.4s, v5.4s, v2.4s 				\n"
		"	fmla	v15.4s, v5.4s, v3.4s 				\n"
		"	ldr		q4, [x15], #16 							\n"
		"	prfm   PLDL1KEEP, [x15, #64]			\n"
		"	fmla	v16.4s, v5.4s, v4.4s 				\n"

		"	bgt		N1_Main_K 									\n"

		"	ands	x22, x5, #3 								\n"
		"	beq		BEGIN_SAVE5x1 							\n"


		"	ins 	v17.s[0], v12.s[0] 					\n"
		"	ins 	v17.s[1], v13.s[0] 					\n"
		"	ins 	v17.s[2], v14.s[0] 					\n"
		"	ins 	v17.s[3], v15.s[0] 					\n"
		"	ins 	v18.s[0], v16.s[0] 					\n"

		"N1M5_Edge_K_END: 									\n"
		"	subs	x22, x22, #1 								\n"

		"	ld1		{v5.s}[0], [x17] 						\n"
		"	add 	x17, x17, x6								\n"

		"	ld1		{v0.s}[0], [x11], #4 				\n"
		"	ld1		{v0.s}[1], [x12], #4 				\n"
		"	ld1		{v0.s}[2], [x13], #4 				\n"
		"	ld1		{v0.s}[3], [x14], #4 				\n"
		"	fmla	v17.4s, v0.4s, v5.s[0]			\n"
		"	ld1		{v1.s}[0], [x15], #4 				\n"
		"	fmadd	s18, s5, s1, s18 						\n"

		"	bgt		N1M5_Edge_K_END 						\n"

		"	ins 	v12.s[0], v17.s[0] 					\n"
		"	ins 	v13.s[0], v17.s[1] 					\n"
		"	ins 	v14.s[0], v17.s[2] 					\n"
		"	ins 	v15.s[0], v17.s[3] 					\n"
		"	ins 	v16.s[0], v18.s[0] 					\n"

		"BEGIN_SAVE5x1: 										\n"		

		"	N1_SAVE5x1								 			 	\n"
		"	bgt   	N1_BEGIN_M5					 	 	 	\n"



//---------------------------------------------------------

		"N1_END_M5:									 					\n"

	//--------------------------------------------------------
		"N1_BEGIN_M4:						 			 				\n"

		"	mov		x17, x16						 					\n"   //B0*
		"	add		x18, x17, x6 					 				\n"   //B1*

		"	cmp		x23, #4							 					\n"
		"	blt 	N1_BEGIN_M3 					 				\n"

		"	mov		x11, x10 						 					\n"
		"	add		x12, x11, x8 					 				\n"
		"	add		x13, x12, x8 					 				\n"
		"	add		x14, x13, x8 				 	 				\n"

		"	prfm  PLDL1KEEP, [x11, #64]			 		\n"
		"	prfm  PLDL1KEEP, [x12, #64]			 		\n"
		"	prfm  PLDL1KEEP, [x13, #64]			 		\n"
		"	prfm  PLDL1KEEP, [x14, #64]			 		\n"


		"N1_M4_Body_K:								 				\n"

		"	lsr		x22, x5, #2 					 				\n"     // K / 4

		"	dup		v12.4s, wzr 									\n"
		"	dup		v13.4s, wzr 									\n"
		"	dup		v14.4s, wzr 									\n"
		"	dup		v15.4s, wzr 									\n"

		"N1_M4_Main_K:								 				\n"

		"	subs	x22, x22, #1 									\n"

		"	ld1 	{v5.s}[0], [x17] 							\n"
		"	add 	x17, x17, x6, lsl #1 					\n"
		"	ld1 	{v5.s}[1], [x18] 							\n"
		"	add 	x18, x18, x6, lsl #1 					\n"
		"	ld1 	{v5.s}[2], [x17] 							\n"
		"	add 	x17, x17, x6, lsl #1 					\n"
		"	ld1 	{v5.s}[3], [x18] 							\n"
		"	add 	x18, x18, x6, lsl #1 					\n"

		"	ldr		q0, [x11], #16 								\n"
		"	prfm   PLDL1KEEP, [x11, #64]				\n"
		"	ldr		q1, [x12], #16 								\n"
		"	fmla	v12.4s, v5.4s, v0.4s 					\n"
		"	fmla	v13.4s, v5.4s, v1.4s 					\n"
		"	prfm   PLDL1KEEP, [x12, #64]				\n"
		"	ldr		q2, [x13], #16 								\n"
		"	prfm   PLDL1KEEP, [x13, #64]				\n"
		"	ldr		q3, [x14], #16 								\n"
		"	prfm   PLDL1KEEP, [x14, #64]				\n"
		"	fmla	v14.4s, v5.4s, v2.4s 					\n"
		"	fmla	v15.4s, v5.4s, v3.4s 					\n"
		

		"	bgt 		N1_M4_Main_K			 		 			\n"	



		"	ands	x22, x5, #3 								\n"
		"	beq		BEGIN_SAVE4x1 							\n"


		"	ins 	v17.s[0], v12.s[0] 					\n"
		"	ins 	v17.s[1], v13.s[0] 					\n"
		"	ins 	v17.s[2], v14.s[0] 					\n"
		"	ins 	v17.s[3], v15.s[0] 					\n"

		"N1M4_Edge_K_END: 									\n"
		"	subs	x22, x22, #1 								\n"

		"	ld1		{v5.s}[0], [x17] 						\n"
		"	add 	x17, x17, x6								\n"

		"	ld1		{v0.s}[0], [x11], #4 				\n"
		"	ld1		{v0.s}[1], [x12], #4 				\n"
		"	ld1		{v0.s}[2], [x13], #4 				\n"
		"	ld1		{v0.s}[3], [x14], #4 				\n"
		"	fmla	v17.4s, v0.4s, v5.s[0]			\n"

		"	bgt		N1M4_Edge_K_END 						\n"

		"	ins 	v12.s[0], v17.s[0] 					\n"
		"	ins 	v13.s[0], v17.s[1] 					\n"
		"	ins 	v14.s[0], v17.s[2] 					\n"
		"	ins 	v15.s[0], v17.s[3] 					\n"

		"BEGIN_SAVE4x1: 										\n"		

		"	N1_SAVE4x1								 				\n"

		"	b 		END_N 						 					\n"


//------------------------------------------------------------------


		"N1_BEGIN_M3: 								 				\n"

		"	cmp		x23, #3							 					\n"
		"	blt 	N1_BEGIN_M2 					 				\n"

		"	mov		x11, x10 						 					\n"
		"	add		x12, x11, x8 					 				\n"
		"	add		x13, x12, x8 					 				\n"

		"	prfm    PLDL1KEEP, [x11, #64]			 	\n"
		"	prfm    PLDL1KEEP, [x12, #64]			 	\n"
		"	prfm    PLDL1KEEP, [x13, #64]			 	\n"


		"N1_M3_Body_K:										 		\n"

		"	lsr		x22, x5, #2 					 				\n"     // K / 4
		"	dup		v12.4s, wzr 									\n"
		"	dup		v13.4s, wzr 									\n"
		"	dup		v14.4s, wzr 									\n"

		"N1_M3_Main_K:								 				\n"


		"	subs	x22, x22, #1 									\n"

		"	ld1 	{v5.s}[0], [x17] 							\n"
		"	add 	x17, x17, x6, lsl #1 					\n"
		"	ld1 	{v5.s}[1], [x18] 							\n"
		"	add 	x18, x18, x6, lsl #1 					\n"
		"	ld1 	{v5.s}[2], [x17] 							\n"
		"	add 	x17, x17, x6, lsl #1 					\n"
		"	ld1 	{v5.s}[3], [x18] 							\n"
		"	add 	x18, x18, x6, lsl #1 					\n"

		"	ldr		q0, [x11], #16 								\n"
		"	prfm   PLDL1KEEP, [x11, #64]				\n"
		"	ldr		q1, [x12], #16 								\n"
		"	fmla	v12.4s, v5.4s, v0.4s 					\n"
		"	fmla	v13.4s, v5.4s, v1.4s 					\n"
		"	prfm   PLDL1KEEP, [x12, #64]				\n"
		"	ldr		q2, [x13], #16 								\n"
		"	prfm   PLDL1KEEP, [x13, #64]				\n"
		"	fmla	v14.4s, v5.4s, v2.4s 					\n"


		"	bgt	N1_M3_Main_K										\n"


		"	ands	x22, x5, #3 								\n"
		"	beq		BEGIN_SAVE3x1 							\n"


		"	ins 	v17.s[0], v12.s[0] 					\n"
		"	ins 	v17.s[1], v13.s[0] 					\n"
		"	ins 	v17.s[2], v14.s[0] 					\n"

		"N1M3_Edge_K_END: 									\n"
		"	subs	x22, x22, #1 								\n"

		"	ld1		{v5.s}[0], [x17] 						\n"
		"	add 	x17, x17, x6								\n"

		"	ld1		{v0.s}[0], [x11], #4 				\n"
		"	ld1		{v0.s}[1], [x12], #4 				\n"
		"	ld1		{v0.s}[2], [x13], #4 				\n"
		"	fmla	v17.4s, v0.4s, v5.s[0]			\n"

		"	bgt		N1M3_Edge_K_END 						\n"

		"	ins 	v12.s[0], v17.s[0] 					\n"
		"	ins 	v13.s[0], v17.s[1] 					\n"
		"	ins 	v14.s[0], v17.s[2] 					\n"

		"BEGIN_SAVE3x1: 										\n"		


		"	N1_SAVE3x1								 					\n"

		"	b 		END_N 						 						\n"





//--------------------------------------------------------

		"N1_BEGIN_M2:								 					\n"


		"	cmp		x23, #2							 					\n"
		"	bne 	N1_BEGIN_M1 					 				\n"

		"	mov		x11, x10 						 					\n"
		"	add		x12, x11, x8 					 				\n"

		"	prfm    PLDL1KEEP, [x11, #64]			 	\n"
		"	prfm    PLDL1KEEP, [x12, #64]			 	\n"


		"N1_M2_Body_K:								 				\n"

		"	lsr		x22, x5, #2 					 				\n"     // K / 4
		"	dup		v12.4s, wzr 									\n"
		"	dup		v13.4s, wzr 									\n"

		"N1_M2_Main_K:								 				\n"

		"	subs	x22, x22, #1 									\n"

		"	ld1 	{v5.s}[0], [x17] 							\n"
		"	add 	x17, x17, x6, lsl #1 					\n"
		"	ld1 	{v5.s}[1], [x18] 							\n"
		"	add 	x18, x18, x6, lsl #1 					\n"
		"	ld1 	{v5.s}[2], [x17] 							\n"
		"	add 	x17, x17, x6, lsl #1 					\n"
		"	ld1 	{v5.s}[3], [x18] 							\n"
		"	add 	x18, x18, x6, lsl #1 					\n"

		"	ldr		q0, [x11], #16 								\n"
		"	prfm   PLDL1KEEP, [x11, #64]				\n"
		"	ldr		q1, [x12], #16 								\n"
		"	fmla	v12.4s, v5.4s, v0.4s 					\n"
		"	fmla	v13.4s, v5.4s, v1.4s 					\n"
		"	prfm   PLDL1KEEP, [x12, #64]				\n"

		"	bgt		N1_M2_Main_K 									\n"


		"	ands	x22, x5, #3 								\n"
		"	beq		BEGIN_SAVE2x1 							\n"

		"	ins 	v17.s[0], v12.s[0] 					\n"
		"	ins 	v17.s[1], v13.s[0] 					\n"

		"N1M2_Edge_K_END: 									\n"
		"	subs	x22, x22, #1 								\n"

		"	ld1		{v5.s}[0], [x17] 						\n"
		"	add 	x17, x17, x6								\n"

		"	ld1		{v0.s}[0], [x11], #4 				\n"
		"	ld1		{v0.s}[1], [x12], #4 				\n"
		"	fmla	v17.4s, v0.4s, v5.s[0]			\n"

		"	bgt		N1M2_Edge_K_END 						\n"

		"	ins 	v12.s[0], v17.s[0] 					\n"
		"	ins 	v13.s[0], v17.s[1] 					\n"

		"BEGIN_SAVE2x1: 										\n"		



		"	N1_SAVE2x1								 					\n"

		"	b 		END_N 						 						\n"



//-----------------------------------------------------
		"N1_BEGIN_M1:								 					\n"


		"	cmp		x23, #1							 					\n"
		"	bne 	END_N 					 	 						\n"

		"	mov		x11, x10 						 					\n"
		"	prfm  PLDL1KEEP, [x11, #64]					\n"

		"N1_M1_Body_K:								 				\n"

		"	lsr		x22, x5, #2 					 				\n"     // K / 4
		"	dup		v12.4s, wzr 									\n"

		"N1_M1_Main_K:								 				\n"	

		"	subs	x22, x22, #1 									\n"

		"	ld1 	{v5.s}[0], [x17] 							\n"
		"	add 	x17, x17, x6, lsl #1 					\n"
		"	ld1 	{v5.s}[1], [x18] 							\n"
		"	add 	x18, x18, x6, lsl #1 					\n"
		"	ld1 	{v5.s}[2], [x17] 							\n"
		"	add 	x17, x17, x6, lsl #1 					\n"
		"	ld1 	{v5.s}[3], [x18] 							\n"
		"	add 	x18, x18, x6, lsl #1 					\n"

		"	ldr		q0, [x11], #16 								\n"
		"	prfm   PLDL1KEEP, [x11, #64]				\n"
		"	fmla	v12.4s, v5.4s, v0.4s 					\n"

		"	bgt		N1_M1_Main_K 									\n"


		"	ands	x22, x5, #3 								\n"
		"	beq		BEGIN_SAVE1x1 							\n"

		"	ins 	v17.s[0], v12.s[0] 					\n"

		"N1M1_Edge_K_END: 									\n"
		"	subs	x22, x22, #1 								\n"

		"	ld1		{v5.s}[0], [x17] 						\n"
		"	add 	x17, x17, x6								\n"

		"	ld1		{v0.s}[0], [x11], #4 				\n"
		"	fmadd s17, s5, s0, s17						\n"
		"	bgt		N1M1_Edge_K_END 						\n"

		"	ins 	v12.s[0], v17.s[0] 					\n"

		"BEGIN_SAVE1x1: 										\n"		
	
		"	N1_SAVE1x1								 					\n"

		"END_N:									 	 						\n"

		
		:
		:	
			[C] "m" (C),
			[A] "m" (A),
      [B] "m" (B), 
      [M] "m" (M),
      [N] "m" (N),
      [K] "m" (K)
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
}
