#include<stdio.h>
#include <stdlib.h>
#include "LibShalom.h"

void DGEMM_NN(double *C, double *A, double *B, long M, long N, 
				long K)
{
	asm volatile(

		".macro	KERNEL7x6_BEGIN_K					 \n"

		"	ldr		q0, [x9], #16					 \n"
		"	ldr		q1, [x10], #16					 \n"

		"	ldp		q8, q9, [x17]				     \n"

		"   prfm    PLDL1KEEP, [x9, #64]       	 	 \n"
		"	prfm	PLDL1KEEP, [x10, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x14, #64] 			 \n"
		"	prfm	PLDL1KEEP, [x15, #64]			 \n"

		"	fmul	v11.2d, v8.2d, v0.d[0]			 \n"
		"	ldr		q2, [x11], #16				 	 \n"
		"	fmul	v12.2d, v8.2d, v1.d[0]			 \n"
		"	ldr		q3, [x12], #16					 \n"
		"	fmul	v13.2d, v8.2d, v2.d[0]			 \n"

		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	ldr		q4, [x13], #16					 \n"
		"	fmul	v14.2d, v8.2d, v3.d[0]			 \n"

		"	ldr		q5, [x14], #16					 \n"
		"	fmul	v15.2d, v8.2d, v4.d[0]			 \n"

		"	ldr		q6, [x15], #16					 \n"
		"	fmul	v16.2d, v8.2d, v5.d[0]			 \n"

		"	ldr		q10, [x17, #32]					 \n"
		"	fmul	v17.2d, v8.2d, v6.d[0]			 \n"

		"	add		x17, x17, x4, lsl #4			 \n"

		"	ldr		q8, [x18]						 \n"

		"	fmul	v18.2d, v9.2d, v0.d[0]			 \n"
		"	fmul	v19.2d, v9.2d, v1.d[0]			 \n"
		"	fmul	v20.2d, v9.2d, v2.d[0]			 \n"
		"	fmul	v21.2d, v9.2d, v3.d[0]			 \n"
		"	fmul	v22.2d, v9.2d, v4.d[0]			 \n"
		"	fmul	v23.2d, v9.2d, v5.d[0]			 \n"
		"	fmul	v24.2d, v9.2d, v6.d[0]			 \n"

		"	ldr		q9, [x18, #16]   				 \n"
		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	fmul	v25.2d, v10.2d, v0.d[0]			 \n"
		"	fmul	v26.2d, v10.2d, v1.d[0]			 \n"
		"	fmul	v27.2d, v10.2d, v2.d[0]			 \n"
		"	fmul	v28.2d, v10.2d, v3.d[0]			 \n"
		"	fmul	v29.2d, v10.2d, v4.d[0]			 \n"
		"	fmul	v30.2d, v10.2d, v5.d[0]			 \n"
		"	fmul	v31.2d, v10.2d, v6.d[0]			 \n"

		"	ldr		q10, [x18, #32]   				 \n"

		"	add		x18, x18, x4, lsl #4			 \n"

		".endm 										 \n"



		".macro	KERNEL7x6_K0     					 \n"


		"   prfm    PLDL1KEEP, [x18, #64]       	 \n"

		"	fmla	v11.2d, v8.2d, v0.d[0]			 \n"
		"	fmla	v12.2d, v8.2d, v1.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x9, #64]			 \n"
		"	fmla	v13.2d, v8.2d, v2.d[0]			 \n"
		"	fmla	v14.2d, v8.2d, v3.d[0]			 \n"
		"	fmla	v15.2d, v8.2d, v4.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x10, #64]			 \n"
		"	fmla	v16.2d, v8.2d, v5.d[0]			 \n"
		"	fmla	v17.2d, v8.2d, v6.d[0]			 \n"

		"	prfm	PLDL1KEEP, [x9, #64]			 \n"
		"	ldr		q8, [x18]						 \n"

		"	fmla	v18.2d, v9.2d, v0.d[0]			 \n"
		"	fmla	v19.2d, v9.2d, v1.d[0]			 \n"
		"	fmla	v20.2d, v9.2d, v2.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x11, #64]			 \n"
		"	fmla	v21.2d, v9.2d, v3.d[0]			 \n"
		"	fmla	v22.2d, v9.2d, v4.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x12, #64]			 \n"
		"	fmla	v23.2d, v9.2d, v5.d[0]			 \n"
		"	fmla	v24.2d, v9.2d, v6.d[0]			 \n"

		"	prfm	PLDL1KEEP, [x11, #64]			 \n"
		"	ldr		q9, [x18, #16]   				 \n"

		"	fmla	v25.2d, v10.2d, v0.d[0]			 \n"
		"	fmla	v26.2d, v10.2d, v1.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x13, #64]			 \n"
		"	fmla	v27.2d, v10.2d, v2.d[0]			 \n"
		"	fmla	v28.2d, v10.2d, v3.d[0]			 \n"
		"	fmla	v29.2d, v10.2d, v4.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x14, #64]			 \n"
		"	fmla	v30.2d, v10.2d, v5.d[0]			 \n"
		"	fmla	v31.2d, v10.2d, v6.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x15, #64]			 \n"

		"	ldr		q10, [x18, #32]   				 \n"
		"	add 	x18, x18, x4, lsl #4			 \n"

		".endm 										 \n"


		".macro	KERNEL7x6_K1     					 \n"


		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	fmla	v11.2d, v8.2d, v0.d[1]			 \n"
		"	fmla	v18.2d, v9.2d, v0.d[1]			 \n"
		"	fmla	v25.2d, v10.2d, v0.d[1]			 \n"
		"	ldr		q0, [x9], #16					 \n"

		"	fmla	v12.2d, v8.2d, v1.d[1]			 \n"
		"	fmla	v19.2d, v9.2d, v1.d[1]			 \n"
		"	fmla	v26.2d, v10.2d, v1.d[1]			 \n"
		"	ldr		q1, [x10], #16					 \n"

		"	fmla	v13.2d, v8.2d, v2.d[1]			 \n"
		"	fmla	v20.2d, v9.2d, v2.d[1]			 \n"
		"	fmla	v27.2d, v10.2d, v2.d[1]			 \n"
		"	ldr		q2, [x11], #16					 \n"

		"	fmla	v14.2d, v8.2d, v3.d[1]			 \n"
		"	fmla	v21.2d, v9.2d, v3.d[1]			 \n"
		"	fmla	v28.2d, v10.2d, v3.d[1]			 \n"
		"	ldr		q3, [x12], #16					 \n"


		"	fmla	v15.2d, v8.2d, v4.d[1]			 \n"
		"	fmla	v16.2d, v8.2d, v5.d[1]			 \n"
		"	fmla	v17.2d, v8.2d, v6.d[1]			 \n"
		"	ldr		q8, [x17]						 \n"

	
		"	fmla	v22.2d, v9.2d, v4.d[1]			 \n"
		"	fmla	v23.2d, v9.2d, v5.d[1]			 \n"
		"	fmla	v24.2d, v9.2d, v6.d[1]			 \n"
		"	ldr		q9, [x17, #16]   				 \n"

		"	fmla	v29.2d, v10.2d, v4.d[1]			 \n"
		"	ldr		q4, [x13], #16					 \n"
		"	fmla	v30.2d, v10.2d, v5.d[1]			 \n"
		"	ldr		q5, [x14], #16					 \n"
		"	fmla	v31.2d, v10.2d, v6.d[1]			 \n"
		"	ldr		q6, [x15], #16					 \n"

		"	ldr		q10, [x17, #32]   				 \n"
		"	add 	x17, x17, x4, lsl #4			 \n"
		

		".endm 										 \n"


		".macro	KERNEL7x6_END_K     				 \n"

		"	fmla	v11.2d, v8.2d, v0.d[1]			 \n"
		"	fmla	v18.2d, v9.2d, v0.d[1]			 \n"
		"	fmla	v25.2d, v10.2d, v0.d[1]			 \n"

		"	fmla	v12.2d, v8.2d, v1.d[1]			 \n"
		"	fmla	v19.2d, v9.2d, v1.d[1]			 \n"
		"	fmla	v26.2d, v10.2d, v1.d[1]			 \n"

		"	fmla	v13.2d, v8.2d, v2.d[1]			 \n"
		"	fmla	v20.2d, v9.2d, v2.d[1]			 \n"
		"	fmla	v27.2d, v10.2d, v2.d[1]			 \n"

		"	fmla	v14.2d, v8.2d, v3.d[1]			 \n"
		"	fmla	v21.2d, v9.2d, v3.d[1]			 \n"
		"	fmla	v28.2d, v10.2d, v3.d[1]			 \n"


		"	fmla	v15.2d, v8.2d, v4.d[1]			 \n"
		"	fmla	v16.2d, v8.2d, v5.d[1]			 \n"
		"	fmla	v17.2d, v8.2d, v6.d[1]			 \n"

	
		"	fmla	v22.2d, v9.2d, v4.d[1]			 \n"
		"	fmla	v23.2d, v9.2d, v5.d[1]			 \n"
		"	fmla	v24.2d, v9.2d, v6.d[1]			 \n"

		"	fmla	v29.2d, v10.2d, v4.d[1]			 \n"
		"	fmla	v30.2d, v10.2d, v5.d[1]			 \n"
		"	fmla	v31.2d, v10.2d, v6.d[1]			 \n"

		".endm 										 \n"



		".macro SAVE7x6							 	 \n"

		"	subs	x21, x21, #1					 \n"

		"	stp		q11, q18, [x25]					 \n"
		"	stp		q12, q19, [x26]					 \n"
		"	stp		q13, q20, [x27]					 \n"
		"	stp		q14, q21, [x28]					 \n"
		"	stp		q15, q22, [x29]					 \n"
		"	str 	q25, [x25, #32]					 \n"
		"	str 	q26, [x26, #32] 				 \n"
		"	str 	q27, [x27, #32]  				 \n"
		"	str  	q28, [x28, #32] 				 \n"
		"	str 	q29, [x29, #32]					 \n"

		"	add		x8, x8, x5, lsl #5				 \n"
		"	add	  	x8, x8, x5, lsl #4				 \n"
		"	add		x8, x8, x5, lsl #3				 \n"

		"	add		x28, x28, x4, lsl #4 			 \n"
		"	add 	x29, x29, x4, lsl #4 			 \n"


		"	stp		q16, q23, [x28] 				 \n"
		"	add 	x25, x29, x4, lsl #3			 \n"
		"	stp 	q17, q24, [x29]					 \n"
		"	add 	x26, x25, x4, lsl #3			 \n"
		"	str 	q30, [x28, #32]					 \n"
		"	add 	x27, x26, x4, lsl #3			 \n"
		"	str 	q31, [x29, #32]					 \n"
		"	add 	x28, x27, x4, lsl #3			 \n"
		"	add 	x29, x28, x4, lsl #3			 \n"


		".endm 									 	 \n"



		".macro		EDGE_K_KERNEL7x6     		 	 \n"


		"	ldr		d0, [x9], #8 					 \n"
		"	ldr		d1, [x10], #8					 \n"
		"	ldp		q8, q9, [x17]					 \n"

		"	fmla	v11.2d, v8.2d, v0.d[0]			 \n"
		"	ldr 	d2, [x11], #8					 \n"
		"	fmla	v12.2d, v8.2d, v1.d[0]			 \n"

		"	ldr 	d3, [x12], #8					 \n"
		"	fmla	v13.2d, v8.2d, v2.d[0]			 \n"
		"	ldr		d4, [x13], #8 					 \n"
		"	fmla	v14.2d, v8.2d, v3.d[0]			 \n"

		"	ldr 	d5, [x14], #8 					 \n"
		"	fmla	v15.2d, v8.2d, v4.d[0]			 \n"
		"	ldr 	d6, [x15], #8 					 \n"
		"	fmla	v16.2d, v8.2d, v5.d[0]			 \n"
		"	fmla	v17.2d, v8.2d, v6.d[0]			 \n"

		"	ldr		q10, [x17, #32]					 \n"

		"	fmla	v18.2d, v9.2d, v0.d[0]			 \n"
		"	fmla	v19.2d, v9.2d, v1.d[0]			 \n"
		"	fmla	v20.2d, v9.2d, v2.d[0]			 \n"
		"	fmla	v21.2d, v9.2d, v3.d[0]			 \n"
		"	fmla	v22.2d, v9.2d, v4.d[0]			 \n"
		"	fmla	v23.2d, v9.2d, v5.d[0]			 \n"
		"	fmla	v24.2d, v9.2d, v6.d[0]			 \n"

		"	add		x17, x17, x4, lsl #3			 \n"
		"	prfm    PLDL1KEEP, [x17, #32]			 \n"

		"	fmla	v25.2d, v10.2d, v0.d[0]			 \n"
		"	fmla	v26.2d, v10.2d, v1.d[0]			 \n"
		"	fmla	v27.2d, v10.2d, v2.d[0]			 \n"
		"	fmla	v28.2d, v10.2d, v3.d[0]			 \n"
		"	fmla	v29.2d, v10.2d, v4.d[0]			 \n"
		"	fmla	v30.2d, v10.2d, v5.d[0]			 \n"
		"	fmla	v31.2d, v10.2d, v6.d[0]			 \n"

		".endm 										 \n"



		".macro	KERNEL6x6_BEGIN_K					 \n"

		"	ldr		q0, [x9], #16					 \n"
		"	ldr		q1, [x10], #16					 \n"

		"	ldp		q8, q9, [x17]				     \n"

		"   prfm    PLDL1KEEP, [x9, #64]       	 	 \n"
		"	prfm	PLDL1KEEP, [x10, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x14, #64] 			 \n"

		"	fmul	v11.2d, v8.2d, v0.d[0]			 \n"
		"	ldr		q2, [x11], #16				 	 \n"
		"	fmul	v12.2d, v8.2d, v1.d[0]			 \n"
		"	ldr		q3, [x12], #16					 \n"
		"	fmul	v13.2d, v8.2d, v2.d[0]			 \n"

		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	ldr		q4, [x13], #16					 \n"
		"	fmul	v14.2d, v8.2d, v3.d[0]			 \n"

		"	ldr		q5, [x14], #16					 \n"
		"	fmul	v15.2d, v8.2d, v4.d[0]			 \n"
		"	fmul	v16.2d, v8.2d, v5.d[0]			 \n"
		"	ldr		q10, [x17, #32]					 \n"

		"	add		x17, x17, x4, lsl #4			 \n"
		"	ldr		q8, [x18]						 \n"

		"	fmul	v18.2d, v9.2d, v0.d[0]			 \n"
		"	fmul	v19.2d, v9.2d, v1.d[0]			 \n"
		"	fmul	v20.2d, v9.2d, v2.d[0]			 \n"
		"	fmul	v21.2d, v9.2d, v3.d[0]			 \n"
		"	fmul	v22.2d, v9.2d, v4.d[0]			 \n"
		"	fmul	v23.2d, v9.2d, v5.d[0]			 \n"

		"	ldr		q9, [x18, #16]   				 \n"
		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	fmul	v25.2d, v10.2d, v0.d[0]			 \n"
		"	fmul	v26.2d, v10.2d, v1.d[0]			 \n"
		"	fmul	v27.2d, v10.2d, v2.d[0]			 \n"
		"	fmul	v28.2d, v10.2d, v3.d[0]			 \n"
		"	fmul	v29.2d, v10.2d, v4.d[0]			 \n"
		"	fmul	v30.2d, v10.2d, v5.d[0]			 \n"

		"	ldr		q10, [x18, #32]   				 \n"
		"	add		x18, x18, x4, lsl #4			 \n"

		".endm 										 \n"



		".macro	KERNEL6x6_K0     					 \n"


		"   prfm    PLDL1KEEP, [x18, #64]       	 \n"

		"	fmla	v11.2d, v8.2d, v0.d[0]			 \n"
		"	fmla	v12.2d, v8.2d, v1.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x9, #64]			 \n"
		"	fmla	v13.2d, v8.2d, v2.d[0]			 \n"
		"	fmla	v14.2d, v8.2d, v3.d[0]			 \n"
		"	fmla	v15.2d, v8.2d, v4.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x10, #64]			 \n"
		"	fmla	v16.2d, v8.2d, v5.d[0]			 \n"

		"	ldr		q8, [x18]						 \n"

		"	fmla	v18.2d, v9.2d, v0.d[0]			 \n"
		"	fmla	v19.2d, v9.2d, v1.d[0]			 \n"
		"	fmla	v20.2d, v9.2d, v2.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x11, #64]			 \n"
		"	fmla	v21.2d, v9.2d, v3.d[0]			 \n"
		"	fmla	v22.2d, v9.2d, v4.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x12, #64]			 \n"
		"	fmla	v23.2d, v9.2d, v5.d[0]			 \n"

		"	prfm	PLDL1KEEP, [x11, #64]			 \n"
		"	ldr		q9, [x18, #16]   				 \n"

		"	fmla	v25.2d, v10.2d, v0.d[0]			 \n"
		"	fmla	v26.2d, v10.2d, v1.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x13, #64]			 \n"
		"	fmla	v27.2d, v10.2d, v2.d[0]			 \n"
		"	fmla	v28.2d, v10.2d, v3.d[0]			 \n"
		"	fmla	v29.2d, v10.2d, v4.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x14, #64]			 \n"
		"	fmla	v30.2d, v10.2d, v5.d[0]			 \n"

		"	ldr		q10, [x18, #32]   				 \n"
		"	add 	x18, x18, x4, lsl #4			 \n"

		".endm 										 \n"


		".macro	KERNEL6x6_K1     					 \n"


		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	fmla	v11.2d, v8.2d, v0.d[1]			 \n"
		"	fmla	v18.2d, v9.2d, v0.d[1]			 \n"
		"	fmla	v25.2d, v10.2d, v0.d[1]			 \n"
		"	ldr		q0, [x9], #16					 \n"

		"	fmla	v12.2d, v8.2d, v1.d[1]			 \n"
		"	fmla	v19.2d, v9.2d, v1.d[1]			 \n"
		"	fmla	v26.2d, v10.2d, v1.d[1]			 \n"
		"	ldr		q1, [x10], #16					 \n"

		"	fmla	v13.2d, v8.2d, v2.d[1]			 \n"
		"	fmla	v20.2d, v9.2d, v2.d[1]			 \n"
		"	fmla	v27.2d, v10.2d, v2.d[1]			 \n"
		"	ldr		q2, [x11], #16					 \n"

		"	fmla	v14.2d, v8.2d, v3.d[1]			 \n"
		"	fmla	v21.2d, v9.2d, v3.d[1]			 \n"
		"	fmla	v28.2d, v10.2d, v3.d[1]			 \n"
		"	ldr		q3, [x12], #16					 \n"

		"	fmla	v15.2d, v8.2d, v4.d[1]			 \n"
		"	fmla	v16.2d, v8.2d, v5.d[1]			 \n"
		"	ldr		q8, [x17]						 \n"
	
		"	fmla	v22.2d, v9.2d, v4.d[1]			 \n"
		"	fmla	v23.2d, v9.2d, v5.d[1]			 \n"
		"	ldr		q9, [x17, #16]   				 \n"

		"	fmla	v29.2d, v10.2d, v4.d[1]			 \n"
		"	ldr		q4, [x13], #16					 \n"
		"	fmla	v30.2d, v10.2d, v5.d[1]			 \n"
		"	ldr		q5, [x14], #16					 \n"

		"	ldr		q10, [x17, #32]   				 \n"
		"	add 	x17, x17, x4, lsl #4			 \n"
		

		".endm 										 \n"


		".macro	KERNEL6x6_END_K     				 \n"

		"	fmla	v11.2d, v8.2d, v0.d[1]			 \n"
		"	fmla	v18.2d, v9.2d, v0.d[1]			 \n"
		"	fmla	v25.2d, v10.2d, v0.d[1]			 \n"

		"	fmla	v12.2d, v8.2d, v1.d[1]			 \n"
		"	fmla	v19.2d, v9.2d, v1.d[1]			 \n"
		"	fmla	v26.2d, v10.2d, v1.d[1]			 \n"

		"	fmla	v13.2d, v8.2d, v2.d[1]			 \n"
		"	fmla	v20.2d, v9.2d, v2.d[1]			 \n"
		"	fmla	v27.2d, v10.2d, v2.d[1]			 \n"

		"	fmla	v14.2d, v8.2d, v3.d[1]			 \n"
		"	fmla	v21.2d, v9.2d, v3.d[1]			 \n"
		"	fmla	v28.2d, v10.2d, v3.d[1]			 \n"


		"	fmla	v15.2d, v8.2d, v4.d[1]			 \n"
		"	fmla	v16.2d, v8.2d, v5.d[1]			 \n"

		"	fmla	v22.2d, v9.2d, v4.d[1]			 \n"
		"	fmla	v23.2d, v9.2d, v5.d[1]			 \n"

		"	fmla	v29.2d, v10.2d, v4.d[1]			 \n"
		"	fmla	v30.2d, v10.2d, v5.d[1]			 \n"

		".endm 										 \n"



		".macro SAVE6x6							 	 \n"

		"	stp		q11, q18, [x25]					 \n"
		"	stp		q12, q19, [x26]					 \n"
		"	stp		q13, q20, [x27]					 \n"
		"	stp		q14, q21, [x28]					 \n"
		"	stp		q15, q22, [x29]					 \n"
		"	str 	q25, [x25, #32]					 \n"
		"	str 	q26, [x26, #32] 				 \n"
		"	str 	q27, [x27, #32]  				 \n"
		"	str  	q28, [x28, #32] 				 \n"
		"	str 	q29, [x29, #32]					 \n"

		"	add		x28, x28, x4, lsl #4 			 \n"

		"	stp		q16, q23, [x28] 				 \n"
		"	str 	q30, [x28, #32]					 \n"



		".endm 									 	 \n"


		".macro	EDGE_K_KERNEL6x6					 \n"



		"	ldr		d0, [x9], #8 					 \n"
		"	ldr		d1, [x10], #8					 \n"
		"	ldp		q8, q9, [x17]					 \n"

		"	fmla	v11.2d, v8.2d, v0.d[0]			 \n"
		"	ldr 	d2, [x11], #8					 \n"
		"	fmla	v12.2d, v8.2d, v1.d[0]			 \n"

		"	ldr 	d3, [x12], #8					 \n"
		"	fmla	v13.2d, v8.2d, v2.d[0]			 \n"
		"	ldr		d4, [x13], #8 					 \n"
		"	fmla	v14.2d, v8.2d, v3.d[0]			 \n"

		"	ldr 	d5, [x14], #8 					 \n"
		"	fmla	v15.2d, v8.2d, v4.d[0]			 \n"
		"	fmla	v16.2d, v8.2d, v5.d[0]			 \n"

		"	ldr		q10, [x17, #32]					 \n"

		"	fmla	v18.2d, v9.2d, v0.d[0]			 \n"
		"	fmla	v19.2d, v9.2d, v1.d[0]			 \n"
		"	fmla	v20.2d, v9.2d, v2.d[0]			 \n"
		"	fmla	v21.2d, v9.2d, v3.d[0]			 \n"
		"	fmla	v22.2d, v9.2d, v4.d[0]			 \n"
		"	fmla	v23.2d, v9.2d, v5.d[0]			 \n"

		"	add		x17, x17, x4, lsl #3			 \n"
		"	prfm    PLDL1KEEP, [x17, #32]			 \n"

		"	fmla	v25.2d, v10.2d, v0.d[0]			 \n"
		"	fmla	v26.2d, v10.2d, v1.d[0]			 \n"
		"	fmla	v27.2d, v10.2d, v2.d[0]			 \n"
		"	fmla	v28.2d, v10.2d, v3.d[0]			 \n"
		"	fmla	v29.2d, v10.2d, v4.d[0]			 \n"
		"	fmla	v30.2d, v10.2d, v5.d[0]			 \n"


		".endm 										 \n"



		".macro	KERNEL5x6_BEGIN_K					 \n"

		"	ldr		q0, [x9], #16					 \n"
		"	ldr		q1, [x10], #16					 \n"

		"	ldp		q8, q9, [x17]				     \n"

		"   prfm    PLDL1KEEP, [x9, #64]       	 	 \n"
		"	prfm	PLDL1KEEP, [x10, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"

		"	fmul	v11.2d, v8.2d, v0.d[0]			 \n"
		"	ldr		q2, [x11], #16				 	 \n"
		"	fmul	v12.2d, v8.2d, v1.d[0]			 \n"
		"	ldr		q3, [x12], #16					 \n"
		"	fmul	v13.2d, v8.2d, v2.d[0]			 \n"

		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	ldr		q4, [x13], #16					 \n"
		"	fmul	v14.2d, v8.2d, v3.d[0]			 \n"

		"	fmul	v15.2d, v8.2d, v4.d[0]			 \n"
		"	ldr		q10, [x17, #32]					 \n"

		"	add		x17, x17, x4, lsl #4			 \n"
		"	ldr		q8, [x18]						 \n"

		"	fmul	v18.2d, v9.2d, v0.d[0]			 \n"
		"	fmul	v19.2d, v9.2d, v1.d[0]			 \n"
		"	fmul	v20.2d, v9.2d, v2.d[0]			 \n"
		"	fmul	v21.2d, v9.2d, v3.d[0]			 \n"
		"	fmul	v22.2d, v9.2d, v4.d[0]			 \n"

		"	ldr		q9, [x18, #16]   				 \n"
		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	fmul	v25.2d, v10.2d, v0.d[0]			 \n"
		"	fmul	v26.2d, v10.2d, v1.d[0]			 \n"
		"	fmul	v27.2d, v10.2d, v2.d[0]			 \n"
		"	fmul	v28.2d, v10.2d, v3.d[0]			 \n"
		"	fmul	v29.2d, v10.2d, v4.d[0]			 \n"

		"	ldr		q10, [x18, #32]   				 \n"
		"	add		x18, x18, x4, lsl #4			 \n"

		".endm 										 \n"



		".macro	KERNEL5x6_K0     					 \n"


		"   prfm    PLDL1KEEP, [x18, #64]       	 \n"

		"	fmla	v11.2d, v8.2d, v0.d[0]			 \n"
		"	fmla	v12.2d, v8.2d, v1.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x9, #64]			 \n"
		"	fmla	v13.2d, v8.2d, v2.d[0]			 \n"
		"	fmla	v14.2d, v8.2d, v3.d[0]			 \n"
		"	fmla	v15.2d, v8.2d, v4.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x10, #64]			 \n"
		"	ldr		q8, [x18]						 \n"

		"	fmla	v18.2d, v9.2d, v0.d[0]			 \n"
		"	fmla	v19.2d, v9.2d, v1.d[0]			 \n"
		"	fmla	v20.2d, v9.2d, v2.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x11, #64]			 \n"
		"	fmla	v21.2d, v9.2d, v3.d[0]			 \n"
		"	fmla	v22.2d, v9.2d, v4.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x12, #64]			 \n"

		"	prfm	PLDL1KEEP, [x11, #64]			 \n"
		"	ldr		q9, [x18, #16]   				 \n"

		"	fmla	v25.2d, v10.2d, v0.d[0]			 \n"
		"	fmla	v26.2d, v10.2d, v1.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x13, #64]			 \n"
		"	fmla	v27.2d, v10.2d, v2.d[0]			 \n"
		"	fmla	v28.2d, v10.2d, v3.d[0]			 \n"
		"	fmla	v29.2d, v10.2d, v4.d[0]			 \n"

		"	ldr		q10, [x18, #32]   				 \n"
		"	add 	x18, x18, x4, lsl #4			 \n"

		".endm 										 \n"


		".macro	KERNEL5x6_K1     					 \n"


		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	fmla	v11.2d, v8.2d, v0.d[1]			 \n"
		"	fmla	v18.2d, v9.2d, v0.d[1]			 \n"
		"	fmla	v25.2d, v10.2d, v0.d[1]			 \n"
		"	ldr		q0, [x9], #16					 \n"

		"	fmla	v12.2d, v8.2d, v1.d[1]			 \n"
		"	fmla	v19.2d, v9.2d, v1.d[1]			 \n"
		"	fmla	v26.2d, v10.2d, v1.d[1]			 \n"
		"	ldr		q1, [x10], #16					 \n"

		"	fmla	v13.2d, v8.2d, v2.d[1]			 \n"
		"	fmla	v20.2d, v9.2d, v2.d[1]			 \n"
		"	fmla	v27.2d, v10.2d, v2.d[1]			 \n"
		"	ldr		q2, [x11], #16					 \n"

		"	fmla	v14.2d, v8.2d, v3.d[1]			 \n"
		"	fmla	v21.2d, v9.2d, v3.d[1]			 \n"
		"	fmla	v28.2d, v10.2d, v3.d[1]			 \n"
		"	ldr		q3, [x12], #16					 \n"

		"	fmla	v15.2d, v8.2d, v4.d[1]			 \n"
		"	ldr		q8, [x17]						 \n"
	
		"	fmla	v22.2d, v9.2d, v4.d[1]			 \n"
		"	ldr		q9, [x17, #16]   				 \n"

		"	fmla	v29.2d, v10.2d, v4.d[1]			 \n"
		"	ldr		q4, [x13], #16					 \n"

		"	ldr		q10, [x17, #32]   				 \n"
		"	add 	x17, x17, x4, lsl #4			 \n"
		

		".endm 										 \n"


		".macro	KERNEL5x6_END_K     				 \n"

		"	fmla	v11.2d, v8.2d, v0.d[1]			 \n"
		"	fmla	v18.2d, v9.2d, v0.d[1]			 \n"
		"	fmla	v25.2d, v10.2d, v0.d[1]			 \n"

		"	fmla	v12.2d, v8.2d, v1.d[1]			 \n"
		"	fmla	v19.2d, v9.2d, v1.d[1]			 \n"
		"	fmla	v26.2d, v10.2d, v1.d[1]			 \n"

		"	fmla	v13.2d, v8.2d, v2.d[1]			 \n"
		"	fmla	v20.2d, v9.2d, v2.d[1]			 \n"
		"	fmla	v27.2d, v10.2d, v2.d[1]			 \n"

		"	fmla	v14.2d, v8.2d, v3.d[1]			 \n"
		"	fmla	v21.2d, v9.2d, v3.d[1]			 \n"
		"	fmla	v28.2d, v10.2d, v3.d[1]			 \n"

		"	fmla	v15.2d, v8.2d, v4.d[1]			 \n"
		"	fmla	v22.2d, v9.2d, v4.d[1]			 \n"
		"	fmla	v29.2d, v10.2d, v4.d[1]			 \n"

		".endm 										 \n"



		".macro SAVE5x6							 	 \n"

		"	stp		q11, q18, [x25]					 \n"
		"	stp		q12, q19, [x26]					 \n"
		"	stp		q13, q20, [x27]					 \n"
		"	stp		q14, q21, [x28]					 \n"
		"	stp		q15, q22, [x29]					 \n"
		"	str 	q25, [x25, #32]					 \n"
		"	str 	q26, [x26, #32] 				 \n"
		"	str 	q27, [x27, #32]  				 \n"
		"	str  	q28, [x28, #32] 				 \n"
		"	str 	q29, [x29, #32]					 \n"

		".endm 									 	 \n"


		".macro	EDGE_K_KERNEL5x6					 \n"


		"	ldr		d0, [x9], #8 					 \n"
		"	ldr		d1, [x10], #8					 \n"
		"	ldp		q8, q9, [x17]					 \n"

		"	fmla	v11.2d, v8.2d, v0.d[0]			 \n"
		"	ldr 	d2, [x11], #8					 \n"
		"	fmla	v12.2d, v8.2d, v1.d[0]			 \n"

		"	ldr 	d3, [x12], #8					 \n"
		"	fmla	v13.2d, v8.2d, v2.d[0]			 \n"
		"	ldr		d4, [x13], #8 					 \n"
		"	fmla	v14.2d, v8.2d, v3.d[0]			 \n"
		"	fmla	v15.2d, v8.2d, v4.d[0]			 \n"

		"	ldr		q10, [x17, #32]					 \n"

		"	fmla	v18.2d, v9.2d, v0.d[0]			 \n"
		"	fmla	v19.2d, v9.2d, v1.d[0]			 \n"
		"	fmla	v20.2d, v9.2d, v2.d[0]			 \n"
		"	fmla	v21.2d, v9.2d, v3.d[0]			 \n"
		"	fmla	v22.2d, v9.2d, v4.d[0]			 \n"

		"	add		x17, x17, x4, lsl #3			 \n"
		"	prfm    PLDL1KEEP, [x17, #32]			 \n"

		"	fmla	v25.2d, v10.2d, v0.d[0]			 \n"
		"	fmla	v26.2d, v10.2d, v1.d[0]			 \n"
		"	fmla	v27.2d, v10.2d, v2.d[0]			 \n"
		"	fmla	v28.2d, v10.2d, v3.d[0]			 \n"
		"	fmla	v29.2d, v10.2d, v4.d[0]			 \n"


		".endm 										 \n"


		".macro	KERNEL2x6_BEGIN_K					 \n"

		"	ldr		q0, [x9], #16					 \n"
		"	ldr		q1, [x10], #16					 \n"

		"	ldp		q8, q9, [x17]				     \n"

		"   prfm    PLDL1KEEP, [x9, #64]       	 	 \n"
		"	prfm	PLDL1KEEP, [x10, #64] 			 \n"

		"	fmul	v11.2d, v8.2d, v0.d[0]			 \n"
		"	fmul	v12.2d, v8.2d, v1.d[0]			 \n"

		"	ldr		q10, [x17, #32]					 \n"

		"	add		x17, x17, x4, lsl #4			 \n"
		"	ldr		q8, [x18]						 \n"

		"	fmul	v18.2d, v9.2d, v0.d[0]			 \n"
		"	fmul	v19.2d, v9.2d, v1.d[0]			 \n"

		"	ldr		q9, [x18, #16]   				 \n"
		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	fmul	v25.2d, v10.2d, v0.d[0]			 \n"
		"	fmul	v26.2d, v10.2d, v1.d[0]			 \n"

		"	ldr		q10, [x18, #32]   				 \n"
		"	add		x18, x18, x4, lsl #4			 \n"

		".endm 										 \n"



		".macro	KERNEL2x6_K0     					 \n"


		"   prfm    PLDL1KEEP, [x18, #64]       	 \n"

		"	fmla	v11.2d, v8.2d, v0.d[0]			 \n"
		"	fmla	v12.2d, v8.2d, v1.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x9, #64]			 \n"
		"	prfm	PLDL1KEEP, [x10, #64]			 \n"

		"	ldr		q8, [x18]						 \n"

		"	fmla	v18.2d, v9.2d, v0.d[0]			 \n"
		"	fmla	v19.2d, v9.2d, v1.d[0]			 \n"

		"	prfm	PLDL1KEEP, [x11, #64]			 \n"
		"	ldr		q9, [x18, #16]   				 \n"

		"	fmla	v25.2d, v10.2d, v0.d[0]			 \n"
		"	fmla	v26.2d, v10.2d, v1.d[0]			 \n"

		"	ldr		q10, [x18, #32]   				 \n"
		"	add 	x18, x18, x4, lsl #4			 \n"

		".endm 										 \n"


		".macro	KERNEL2x6_K1     					 \n"


		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	fmla	v11.2d, v8.2d, v0.d[1]			 \n"
		"	fmla	v18.2d, v9.2d, v0.d[1]			 \n"
		"	fmla	v25.2d, v10.2d, v0.d[1]			 \n"
		"	ldr		q0, [x9], #16					 \n"

		"	fmla	v12.2d, v8.2d, v1.d[1]			 \n"
		"	ldr		q8, [x17]						 \n"
		"	fmla	v19.2d, v9.2d, v1.d[1]			 \n"
		"	fmla	v26.2d, v10.2d, v1.d[1]			 \n"
		"	ldr		q1, [x10], #16					 \n"

		"	ldr		q9, [x17, #16]   				 \n"
		"	ldr		q10, [x17, #32]   				 \n"

		"	add 	x17, x17, x4, lsl #4			 \n"		

		".endm 										 \n"


		".macro	KERNEL2x6_END_K     				 \n"

		"	fmla	v11.2d, v8.2d, v0.d[1]			 \n"
		"	fmla	v18.2d, v9.2d, v0.d[1]			 \n"
		"	fmla	v25.2d, v10.2d, v0.d[1]			 \n"

		"	fmla	v12.2d, v8.2d, v1.d[1]			 \n"
		"	fmla	v19.2d, v9.2d, v1.d[1]			 \n"
		"	fmla	v26.2d, v10.2d, v1.d[1]			 \n"

		".endm 										 \n"



		".macro SAVE2x6							 	 \n"

		"	stp		q11, q18, [x25]					 \n"
		"	stp		q12, q19, [x26]					 \n"
		"	str 	q25, [x25, #32]					 \n"
		"	str 	q26, [x26, #32] 				 \n"

		".endm 									 	 \n"


		".macro	EDGE_K_KERNEL2x6					 \n"


		"	ldr		d0, [x9], #8 					 \n"
		"	ldr		d1, [x10], #8					 \n"
		"	ldp		q8, q9, [x17]					 \n"

		"	fmla	v11.2d, v8.2d, v0.d[0]			 \n"
		"	fmla	v12.2d, v8.2d, v1.d[0]			 \n"

		"	ldr		q10, [x17, #32]					 \n"

		"	fmla	v18.2d, v9.2d, v0.d[0]			 \n"
		"	fmla	v19.2d, v9.2d, v1.d[0]			 \n"

		"	add		x17, x17, x4, lsl #3			 \n"
		"	prfm    PLDL1KEEP, [x17, #32]			 \n"

		"	fmla	v25.2d, v10.2d, v0.d[0]			 \n"
		"	fmla	v26.2d, v10.2d, v1.d[0]			 \n"


		".endm 										 \n"



		".macro	KERNEL7x4_BEGIN_K					 \n"

		"	ldr		q0, [x9], #16					 \n"
		"	ldr		q1, [x10], #16					 \n"

		"	ldp		q8, q9, [x17]				     \n"

		"   prfm    PLDL1KEEP, [x9, #64]       	 	 \n"
		"	prfm	PLDL1KEEP, [x10, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x14, #64] 			 \n"
		"	prfm	PLDL1KEEP, [x15, #64]			 \n"

		"	fmul	v11.2d, v8.2d, v0.d[0]			 \n"
		"	ldr		q2, [x11], #16				 	 \n"
		"	fmul	v12.2d, v8.2d, v1.d[0]			 \n"
		"	ldr		q3, [x12], #16					 \n"
		"	fmul	v13.2d, v8.2d, v2.d[0]			 \n"

		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	ldr		q4, [x13], #16					 \n"
		"	fmul	v14.2d, v8.2d, v3.d[0]			 \n"

		"	ldr		q5, [x14], #16					 \n"
		"	fmul	v15.2d, v8.2d, v4.d[0]			 \n"

		"	ldr		q6, [x15], #16					 \n"
		"	fmul	v16.2d, v8.2d, v5.d[0]			 \n"
		"	fmul	v17.2d, v8.2d, v6.d[0]			 \n"

		"	add		x17, x17, x4, lsl #4			 \n"

		"	ldr		q8, [x18]						 \n"

		"	fmul	v18.2d, v9.2d, v0.d[0]			 \n"
		"	fmul	v19.2d, v9.2d, v1.d[0]			 \n"
		"	fmul	v20.2d, v9.2d, v2.d[0]			 \n"
		"	fmul	v21.2d, v9.2d, v3.d[0]			 \n"
		"	fmul	v22.2d, v9.2d, v4.d[0]			 \n"
		"	fmul	v23.2d, v9.2d, v5.d[0]			 \n"
		"	fmul	v24.2d, v9.2d, v6.d[0]			 \n"

		"	ldr		q9, [x18, #16]   				 \n"
		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	add		x18, x18, x4, lsl #4			 \n"

		".endm 										 \n"



		".macro	KERNEL7x4_K0     					 \n"


		"   prfm    PLDL1KEEP, [x18, #64]       	 \n"

		"	fmla	v11.2d, v8.2d, v0.d[0]			 \n"
		"	fmla	v12.2d, v8.2d, v1.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x9, #64]			 \n"
		"	fmla	v13.2d, v8.2d, v2.d[0]			 \n"
		"	fmla	v14.2d, v8.2d, v3.d[0]			 \n"
		"	fmla	v15.2d, v8.2d, v4.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x10, #64]			 \n"
		"	fmla	v16.2d, v8.2d, v5.d[0]			 \n"
		"	fmla	v17.2d, v8.2d, v6.d[0]			 \n"

		"	prfm	PLDL1KEEP, [x9, #64]			 \n"
		"	ldr		q8, [x18]						 \n"

		"	fmla	v18.2d, v9.2d, v0.d[0]			 \n"
		"	fmla	v19.2d, v9.2d, v1.d[0]			 \n"
		"	fmla	v20.2d, v9.2d, v2.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x11, #64]			 \n"
		"	fmla	v21.2d, v9.2d, v3.d[0]			 \n"
		"	fmla	v22.2d, v9.2d, v4.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x12, #64]			 \n"
		"	fmla	v23.2d, v9.2d, v5.d[0]			 \n"
		"	fmla	v24.2d, v9.2d, v6.d[0]			 \n"

		"	prfm	PLDL1KEEP, [x11, #64]			 \n"
		"	ldr		q9, [x18, #16]   				 \n"

		"	prfm	PLDL1KEEP, [x13, #64]			 \n"
		"	prfm	PLDL1KEEP, [x14, #64]			 \n"
		"	prfm	PLDL1KEEP, [x15, #64]			 \n"

		"	add 	x18, x18, x4, lsl #4			 \n"

		".endm 										 \n"


		".macro	KERNEL7x4_K1     					 \n"


		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	fmla	v11.2d, v8.2d, v0.d[1]			 \n"
		"	fmla	v18.2d, v9.2d, v0.d[1]			 \n"
		"	ldr		q0, [x9], #16					 \n"

		"	fmla	v12.2d, v8.2d, v1.d[1]			 \n"
		"	fmla	v19.2d, v9.2d, v1.d[1]			 \n"
		"	ldr		q1, [x10], #16					 \n"

		"	fmla	v13.2d, v8.2d, v2.d[1]			 \n"
		"	fmla	v20.2d, v9.2d, v2.d[1]			 \n"
		"	ldr		q2, [x11], #16					 \n"

		"	fmla	v14.2d, v8.2d, v3.d[1]			 \n"
		"	fmla	v21.2d, v9.2d, v3.d[1]			 \n"
		"	ldr		q3, [x12], #16					 \n"


		"	fmla	v15.2d, v8.2d, v4.d[1]			 \n"
		"	fmla	v16.2d, v8.2d, v5.d[1]			 \n"
		"	fmla	v17.2d, v8.2d, v6.d[1]			 \n"
		"	ldr		q8, [x17]						 \n"

	
		"	fmla	v22.2d, v9.2d, v4.d[1]			 \n"
		"	ldr		q4, [x13], #16					 \n"
		"	fmla	v23.2d, v9.2d, v5.d[1]			 \n"
		"	ldr		q5, [x14], #16					 \n"
		"	fmla	v24.2d, v9.2d, v6.d[1]			 \n"
		"	ldr		q9, [x17, #16]   				 \n"
		"	ldr		q6, [x15], #16					 \n"

		"	add 	x17, x17, x4, lsl #4			 \n"
		

		".endm 										 \n"


		".macro	KERNEL7x4_END_K     				 \n"

		"	fmla	v11.2d, v8.2d, v0.d[1]			 \n"
		"	fmla	v18.2d, v9.2d, v0.d[1]			 \n"

		"	fmla	v12.2d, v8.2d, v1.d[1]			 \n"
		"	fmla	v19.2d, v9.2d, v1.d[1]			 \n"

		"	fmla	v13.2d, v8.2d, v2.d[1]			 \n"
		"	fmla	v20.2d, v9.2d, v2.d[1]			 \n"

		"	fmla	v14.2d, v8.2d, v3.d[1]			 \n"
		"	fmla	v21.2d, v9.2d, v3.d[1]			 \n"

		"	fmla	v15.2d, v8.2d, v4.d[1]			 \n"
		"	fmla	v16.2d, v8.2d, v5.d[1]			 \n"
		"	fmla	v17.2d, v8.2d, v6.d[1]			 \n"
	
		"	fmla	v22.2d, v9.2d, v4.d[1]			 \n"
		"	fmla	v23.2d, v9.2d, v5.d[1]			 \n"
		"	fmla	v24.2d, v9.2d, v6.d[1]			 \n"

		".endm 										 \n"



		".macro SAVE7x4							 	 \n"

		"	subs	x21, x21, #1					 \n"

		"	stp		q11, q18, [x25]					 \n"
		"	stp		q12, q19, [x26]					 \n"
		"	stp		q13, q20, [x27]					 \n"
		"	stp		q14, q21, [x28]					 \n"
		"	stp		q15, q22, [x29]					 \n"

		"	add		x8, x8, x5, lsl #5				 \n"
		"	add	  	x8, x8, x5, lsl #4				 \n"
		"	add		x8, x8, x5, lsl #3				 \n"

		"	add		x28, x28, x4, lsl #4 			 \n"
		"	add 	x29, x29, x4, lsl #4 			 \n"


		"	stp		q16, q23, [x28] 				 \n"
		"	add 	x25, x29, x4, lsl #3			 \n"
		"	stp 	q17, q24, [x29]					 \n"
		"	add 	x26, x25, x4, lsl #3			 \n"
		"	add 	x27, x26, x4, lsl #3			 \n"
		"	add 	x28, x27, x4, lsl #3			 \n"
		"	add 	x29, x28, x4, lsl #3			 \n"


		".endm 									 	 \n"



		".macro		EDGE_K_KERNEL7x4     		 	 \n"


		"	ldr		d0, [x9], #8 					 \n"
		"	ldr		d1, [x10], #8					 \n"
		"	ldp		q8, q9, [x17]					 \n"

		"	fmla	v11.2d, v8.2d, v0.d[0]			 \n"
		"	ldr 	d2, [x11], #8					 \n"
		"	fmla	v12.2d, v8.2d, v1.d[0]			 \n"

		"	ldr 	d3, [x12], #8					 \n"
		"	fmla	v13.2d, v8.2d, v2.d[0]			 \n"
		"	ldr		d4, [x13], #8 					 \n"
		"	fmla	v14.2d, v8.2d, v3.d[0]			 \n"

		"	ldr 	d5, [x14], #8 					 \n"
		"	fmla	v15.2d, v8.2d, v4.d[0]			 \n"
		"	ldr 	d6, [x15], #8 					 \n"
		"	fmla	v16.2d, v8.2d, v5.d[0]			 \n"
		"	fmla	v17.2d, v8.2d, v6.d[0]			 \n"

		"	fmla	v18.2d, v9.2d, v0.d[0]			 \n"
		"	fmla	v19.2d, v9.2d, v1.d[0]			 \n"
		"	fmla	v20.2d, v9.2d, v2.d[0]			 \n"
		"	fmla	v21.2d, v9.2d, v3.d[0]			 \n"
		"	fmla	v22.2d, v9.2d, v4.d[0]			 \n"
		"	fmla	v23.2d, v9.2d, v5.d[0]			 \n"
		"	fmla	v24.2d, v9.2d, v6.d[0]			 \n"

		"	add		x17, x17, x4, lsl #3			 \n"
		"	prfm    PLDL1KEEP, [x17, #32]			 \n"


		".endm 										 \n"


		".macro	KERNEL6x4_BEGIN_K					 \n"

		"	ldr		q0, [x9], #16					 \n"
		"	ldr		q1, [x10], #16					 \n"

		"	ldp		q8, q9, [x17]				     \n"

		"   prfm    PLDL1KEEP, [x9, #64]       	 	 \n"
		"	prfm	PLDL1KEEP, [x10, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x14, #64] 			 \n"

		"	fmul	v11.2d, v8.2d, v0.d[0]			 \n"
		"	ldr		q2, [x11], #16				 	 \n"
		"	fmul	v12.2d, v8.2d, v1.d[0]			 \n"
		"	ldr		q3, [x12], #16					 \n"
		"	fmul	v13.2d, v8.2d, v2.d[0]			 \n"

		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	ldr		q4, [x13], #16					 \n"
		"	fmul	v14.2d, v8.2d, v3.d[0]			 \n"

		"	ldr		q5, [x14], #16					 \n"
		"	fmul	v15.2d, v8.2d, v4.d[0]			 \n"
		"	fmul	v16.2d, v8.2d, v5.d[0]			 \n"

		"	add		x17, x17, x4, lsl #4			 \n"
		"	ldr		q8, [x18]						 \n"

		"	fmul	v18.2d, v9.2d, v0.d[0]			 \n"
		"	fmul	v19.2d, v9.2d, v1.d[0]			 \n"
		"	fmul	v20.2d, v9.2d, v2.d[0]			 \n"
		"	fmul	v21.2d, v9.2d, v3.d[0]			 \n"
		"	fmul	v22.2d, v9.2d, v4.d[0]			 \n"
		"	fmul	v23.2d, v9.2d, v5.d[0]			 \n"

		"	ldr		q9, [x18, #16]   				 \n"
		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"
		"	add		x18, x18, x4, lsl #4			 \n"

		".endm 										 \n"



		".macro	KERNEL6x4_K0     					 \n"


		"   prfm    PLDL1KEEP, [x18, #64]       	 \n"

		"	fmla	v11.2d, v8.2d, v0.d[0]			 \n"
		"	fmla	v12.2d, v8.2d, v1.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x9, #64]			 \n"
		"	fmla	v13.2d, v8.2d, v2.d[0]			 \n"
		"	fmla	v14.2d, v8.2d, v3.d[0]			 \n"
		"	fmla	v15.2d, v8.2d, v4.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x10, #64]			 \n"
		"	fmla	v16.2d, v8.2d, v5.d[0]			 \n"

		"	ldr		q8, [x18]						 \n"

		"	fmla	v18.2d, v9.2d, v0.d[0]			 \n"
		"	fmla	v19.2d, v9.2d, v1.d[0]			 \n"
		"	fmla	v20.2d, v9.2d, v2.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x11, #64]			 \n"
		"	fmla	v21.2d, v9.2d, v3.d[0]			 \n"
		"	fmla	v22.2d, v9.2d, v4.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x12, #64]			 \n"
		"	fmla	v23.2d, v9.2d, v5.d[0]			 \n"

		"	prfm	PLDL1KEEP, [x11, #64]			 \n"
		"	ldr		q9, [x18, #16]   				 \n"

		"	prfm	PLDL1KEEP, [x13, #64]			 \n"
		"	prfm	PLDL1KEEP, [x14, #64]			 \n"

		"	add 	x18, x18, x4, lsl #4			 \n"

		".endm 										 \n"


		".macro	KERNEL6x4_K1     					 \n"


		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	fmla	v11.2d, v8.2d, v0.d[1]			 \n"
		"	fmla	v18.2d, v9.2d, v0.d[1]			 \n"
		"	ldr		q0, [x9], #16					 \n"

		"	fmla	v12.2d, v8.2d, v1.d[1]			 \n"
		"	fmla	v19.2d, v9.2d, v1.d[1]			 \n"
		"	ldr		q1, [x10], #16					 \n"

		"	fmla	v13.2d, v8.2d, v2.d[1]			 \n"
		"	fmla	v20.2d, v9.2d, v2.d[1]			 \n"
		"	ldr		q2, [x11], #16					 \n"

		"	fmla	v14.2d, v8.2d, v3.d[1]			 \n"
		"	fmla	v21.2d, v9.2d, v3.d[1]			 \n"
		"	ldr		q3, [x12], #16					 \n"

		"	fmla	v15.2d, v8.2d, v4.d[1]			 \n"
		"	fmla	v16.2d, v8.2d, v5.d[1]			 \n"
		"	ldr		q8, [x17]						 \n"
	
		"	fmla	v22.2d, v9.2d, v4.d[1]			 \n"
		"	ldr		q4, [x13], #16					 \n"
		"	fmla	v23.2d, v9.2d, v5.d[1]			 \n"
		"	ldr		q9, [x17, #16]   				 \n"

		"	ldr		q5, [x14], #16					 \n"
		"	add 	x17, x17, x4, lsl #4			 \n"
		

		".endm 										 \n"


		".macro	KERNEL6x4_END_K     				 \n"

		"	fmla	v11.2d, v8.2d, v0.d[1]			 \n"
		"	fmla	v18.2d, v9.2d, v0.d[1]			 \n"

		"	fmla	v12.2d, v8.2d, v1.d[1]			 \n"
		"	fmla	v19.2d, v9.2d, v1.d[1]			 \n"

		"	fmla	v13.2d, v8.2d, v2.d[1]			 \n"
		"	fmla	v20.2d, v9.2d, v2.d[1]			 \n"

		"	fmla	v14.2d, v8.2d, v3.d[1]			 \n"
		"	fmla	v21.2d, v9.2d, v3.d[1]			 \n"

		"	fmla	v15.2d, v8.2d, v4.d[1]			 \n"
		"	fmla	v16.2d, v8.2d, v5.d[1]			 \n"

		"	fmla	v22.2d, v9.2d, v4.d[1]			 \n"
		"	fmla	v23.2d, v9.2d, v5.d[1]			 \n"


		".endm 										 \n"



		".macro SAVE6x4							 	 \n"

		"	stp		q11, q18, [x25]					 \n"
		"	stp		q12, q19, [x26]					 \n"
		"	stp		q13, q20, [x27]					 \n"
		"	stp		q14, q21, [x28]					 \n"
		"	stp		q15, q22, [x29]					 \n"


		"	add		x28, x28, x4, lsl #4 			 \n"

		"	stp		q16, q23, [x28] 				 \n"

		".endm 									 	 \n"


		".macro	EDGE_K_KERNEL6x4					 \n"



		"	ldr		d0, [x9], #8 					 \n"
		"	ldr		d1, [x10], #8					 \n"
		"	ldp		q8, q9, [x17]					 \n"

		"	fmla	v11.2d, v8.2d, v0.d[0]			 \n"
		"	ldr 	d2, [x11], #8					 \n"
		"	fmla	v12.2d, v8.2d, v1.d[0]			 \n"

		"	ldr 	d3, [x12], #8					 \n"
		"	fmla	v13.2d, v8.2d, v2.d[0]			 \n"
		"	ldr		d4, [x13], #8 					 \n"
		"	fmla	v14.2d, v8.2d, v3.d[0]			 \n"

		"	ldr 	d5, [x14], #8 					 \n"
		"	fmla	v15.2d, v8.2d, v4.d[0]			 \n"
		"	fmla	v16.2d, v8.2d, v5.d[0]			 \n"

		"	fmla	v18.2d, v9.2d, v0.d[0]			 \n"
		"	fmla	v19.2d, v9.2d, v1.d[0]			 \n"
		"	fmla	v20.2d, v9.2d, v2.d[0]			 \n"
		"	fmla	v21.2d, v9.2d, v3.d[0]			 \n"
		"	fmla	v22.2d, v9.2d, v4.d[0]			 \n"
		"	fmla	v23.2d, v9.2d, v5.d[0]			 \n"

		"	add		x17, x17, x4, lsl #3			 \n"
		"	prfm    PLDL1KEEP, [x17, #32]			 \n"


		".endm 										 \n"



		".macro	KERNEL5x4_BEGIN_K					 \n"

		"	ldr		q0, [x9], #16					 \n"
		"	ldr		q1, [x10], #16					 \n"

		"	ldp		q8, q9, [x17]				     \n"

		"   prfm    PLDL1KEEP, [x9, #64]       	 	 \n"
		"	prfm	PLDL1KEEP, [x10, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"

		"	fmul	v11.2d, v8.2d, v0.d[0]			 \n"
		"	ldr		q2, [x11], #16				 	 \n"
		"	fmul	v12.2d, v8.2d, v1.d[0]			 \n"
		"	ldr		q3, [x12], #16					 \n"
		"	fmul	v13.2d, v8.2d, v2.d[0]			 \n"

		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	ldr		q4, [x13], #16					 \n"
		"	fmul	v14.2d, v8.2d, v3.d[0]			 \n"

		"	fmul	v15.2d, v8.2d, v4.d[0]			 \n"
		"	add		x17, x17, x4, lsl #4			 \n"
		"	ldr		q8, [x18]						 \n"

		"	fmul	v18.2d, v9.2d, v0.d[0]			 \n"
		"	fmul	v19.2d, v9.2d, v1.d[0]			 \n"
		"	fmul	v20.2d, v9.2d, v2.d[0]			 \n"
		"	fmul	v21.2d, v9.2d, v3.d[0]			 \n"
		"	fmul	v22.2d, v9.2d, v4.d[0]			 \n"

		"	ldr		q9, [x18, #16]   				 \n"
		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	add		x18, x18, x4, lsl #4			 \n"

		".endm 										 \n"



		".macro	KERNEL5x4_K0     					 \n"


		"   prfm    PLDL1KEEP, [x18, #64]       	 \n"

		"	fmla	v11.2d, v8.2d, v0.d[0]			 \n"
		"	fmla	v12.2d, v8.2d, v1.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x9, #64]			 \n"
		"	fmla	v13.2d, v8.2d, v2.d[0]			 \n"
		"	fmla	v14.2d, v8.2d, v3.d[0]			 \n"
		"	fmla	v15.2d, v8.2d, v4.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x10, #64]			 \n"
		"	ldr		q8, [x18]						 \n"

		"	fmla	v18.2d, v9.2d, v0.d[0]			 \n"
		"	fmla	v19.2d, v9.2d, v1.d[0]			 \n"
		"	fmla	v20.2d, v9.2d, v2.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x11, #64]			 \n"
		"	fmla	v21.2d, v9.2d, v3.d[0]			 \n"
		"	fmla	v22.2d, v9.2d, v4.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x12, #64]			 \n"

		"	prfm	PLDL1KEEP, [x11, #64]			 \n"
		"	ldr		q9, [x18, #16]   				 \n"

		"	prfm	PLDL1KEEP, [x13, #64]			 \n"
		"	add 	x18, x18, x4, lsl #4			 \n"

		".endm 										 \n"


		".macro	KERNEL5x4_K1     					 \n"


		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	fmla	v11.2d, v8.2d, v0.d[1]			 \n"
		"	fmla	v18.2d, v9.2d, v0.d[1]			 \n"
		"	ldr		q0, [x9], #16					 \n"

		"	fmla	v12.2d, v8.2d, v1.d[1]			 \n"
		"	fmla	v19.2d, v9.2d, v1.d[1]			 \n"
		"	ldr		q1, [x10], #16					 \n"

		"	fmla	v13.2d, v8.2d, v2.d[1]			 \n"
		"	fmla	v20.2d, v9.2d, v2.d[1]			 \n"
		"	ldr		q2, [x11], #16					 \n"

		"	fmla	v14.2d, v8.2d, v3.d[1]			 \n"
		"	fmla	v21.2d, v9.2d, v3.d[1]			 \n"
		"	ldr		q3, [x12], #16					 \n"

		"	fmla	v15.2d, v8.2d, v4.d[1]			 \n"
		"	ldr		q8, [x17]						 \n"
	
		"	fmla	v22.2d, v9.2d, v4.d[1]			 \n"
		"	ldr		q9, [x17, #16]   				 \n"

		"	ldr		q4, [x13], #16					 \n"
		"	add 	x17, x17, x4, lsl #4			 \n"
		

		".endm 										 \n"


		".macro	KERNEL5x4_END_K     				 \n"

		"	fmla	v11.2d, v8.2d, v0.d[1]			 \n"
		"	fmla	v18.2d, v9.2d, v0.d[1]			 \n"

		"	fmla	v12.2d, v8.2d, v1.d[1]			 \n"
		"	fmla	v19.2d, v9.2d, v1.d[1]			 \n"

		"	fmla	v13.2d, v8.2d, v2.d[1]			 \n"
		"	fmla	v20.2d, v9.2d, v2.d[1]			 \n"

		"	fmla	v14.2d, v8.2d, v3.d[1]			 \n"
		"	fmla	v21.2d, v9.2d, v3.d[1]			 \n"

		"	fmla	v15.2d, v8.2d, v4.d[1]			 \n"
		"	fmla	v22.2d, v9.2d, v4.d[1]			 \n"

		".endm 										 \n"



		".macro SAVE5x4							 	 \n"

		"	stp		q11, q18, [x25]					 \n"
		"	stp		q12, q19, [x26]					 \n"
		"	stp		q13, q20, [x27]					 \n"
		"	stp		q14, q21, [x28]					 \n"
		"	stp		q15, q22, [x29]					 \n"

		".endm 									 	 \n"


		".macro	EDGE_K_KERNEL5x4					 \n"


		"	ldr		d0, [x9], #8 					 \n"
		"	ldr		d1, [x10], #8					 \n"
		"	ldp		q8, q9, [x17]					 \n"

		"	fmla	v11.2d, v8.2d, v0.d[0]			 \n"
		"	ldr 	d2, [x11], #8					 \n"
		"	fmla	v12.2d, v8.2d, v1.d[0]			 \n"

		"	ldr 	d3, [x12], #8					 \n"
		"	fmla	v13.2d, v8.2d, v2.d[0]			 \n"
		"	ldr		d4, [x13], #8 					 \n"
		"	fmla	v14.2d, v8.2d, v3.d[0]			 \n"
		"	fmla	v15.2d, v8.2d, v4.d[0]			 \n"

		"	fmla	v18.2d, v9.2d, v0.d[0]			 \n"
		"	fmla	v19.2d, v9.2d, v1.d[0]			 \n"
		"	fmla	v20.2d, v9.2d, v2.d[0]			 \n"
		"	fmla	v21.2d, v9.2d, v3.d[0]			 \n"
		"	fmla	v22.2d, v9.2d, v4.d[0]			 \n"

		"	add		x17, x17, x4, lsl #3			 \n"
		"	prfm    PLDL1KEEP, [x17, #32]			 \n"


		".endm 										 \n"


		".macro	KERNEL2x4_BEGIN_K					 \n"

		"	ldr		q0, [x9], #16					 \n"
		"	ldr		q1, [x10], #16					 \n"

		"	ldp		q8, q9, [x17]				     \n"

		"   prfm    PLDL1KEEP, [x9, #64]       	 	 \n"
		"	prfm	PLDL1KEEP, [x10, #64] 			 \n"

		"	fmul	v11.2d, v8.2d, v0.d[0]			 \n"
		"	fmul	v12.2d, v8.2d, v1.d[0]			 \n"


		"	add		x17, x17, x4, lsl #4			 \n"
		"	ldr		q8, [x18]						 \n"

		"	fmul	v18.2d, v9.2d, v0.d[0]			 \n"
		"	fmul	v19.2d, v9.2d, v1.d[0]			 \n"

		"	ldr		q9, [x18, #16]   				 \n"
		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	add		x18, x18, x4, lsl #4			 \n"

		".endm 										 \n"



		".macro	KERNEL2x4_K0     					 \n"


		"   prfm    PLDL1KEEP, [x18, #64]       	 \n"

		"	fmla	v11.2d, v8.2d, v0.d[0]			 \n"
		"	fmla	v12.2d, v8.2d, v1.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x9, #64]			 \n"
		"	prfm	PLDL1KEEP, [x10, #64]			 \n"

		"	ldr		q8, [x18]						 \n"

		"	fmla	v18.2d, v9.2d, v0.d[0]			 \n"
		"	fmla	v19.2d, v9.2d, v1.d[0]			 \n"

		"	prfm	PLDL1KEEP, [x11, #64]			 \n"
		"	ldr		q9, [x18, #16]   				 \n"
		"	add 	x18, x18, x4, lsl #4			 \n"

		".endm 										 \n"


		".macro	KERNEL2x4_K1     					 \n"


		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	fmla	v11.2d, v8.2d, v0.d[1]			 \n"
		"	fmla	v18.2d, v9.2d, v0.d[1]			 \n"
		"	ldr		q0, [x9], #16					 \n"

		"	fmla	v12.2d, v8.2d, v1.d[1]			 \n"
		"	ldr		q8, [x17]						 \n"
		"	fmla	v19.2d, v9.2d, v1.d[1]			 \n"
		"	ldr		q9, [x17, #16]   				 \n"

		"	ldr		q1, [x10], #16					 \n"
		"	add 	x17, x17, x4, lsl #4			 \n"		

		".endm 										 \n"


		".macro	KERNEL2x4_END_K     				 \n"

		"	fmla	v11.2d, v8.2d, v0.d[1]			 \n"
		"	fmla	v18.2d, v9.2d, v0.d[1]			 \n"

		"	fmla	v12.2d, v8.2d, v1.d[1]			 \n"
		"	fmla	v19.2d, v9.2d, v1.d[1]			 \n"


		".endm 										 \n"



		".macro SAVE2x4							 	 \n"

		"	stp		q11, q18, [x25]					 \n"
		"	stp		q12, q19, [x26]					 \n"

		".endm 									 	 \n"


		".macro	EDGE_K_KERNEL2x4					 \n"


		"	ldr		d0, [x9], #8 					 \n"
		"	ldr		d1, [x10], #8					 \n"
		"	ldp		q8, q9, [x17]					 \n"

		"	fmla	v11.2d, v8.2d, v0.d[0]			 \n"
		"	fmla	v12.2d, v8.2d, v1.d[0]			 \n"

		"	fmla	v18.2d, v9.2d, v0.d[0]			 \n"
		"	fmla	v19.2d, v9.2d, v1.d[0]			 \n"

		"	add		x17, x17, x4, lsl #3			 \n"
		"	prfm    PLDL1KEEP, [x17, #32]			 \n"


		".endm 										 \n"





		".macro	KERNEL7x2_BEGIN_K					 \n"

		"	ldr		q0, [x9], #16					 \n"
		"	ldr		q1, [x10], #16					 \n"

		"	ldr		q8, [x17]				     	 \n"

		"   prfm    PLDL1KEEP, [x9, #64]       	 	 \n"
		"	prfm	PLDL1KEEP, [x10, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x14, #64] 			 \n"
		"	prfm	PLDL1KEEP, [x15, #64]			 \n"

		"	fmul	v11.2d, v8.2d, v0.d[0]			 \n"
		"	ldr		q2, [x11], #16				 	 \n"
		"	fmul	v12.2d, v8.2d, v1.d[0]			 \n"
		"	ldr		q3, [x12], #16					 \n"
		"	fmul	v13.2d, v8.2d, v2.d[0]			 \n"

		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	ldr		q4, [x13], #16					 \n"
		"	fmul	v14.2d, v8.2d, v3.d[0]			 \n"

		"	ldr		q5, [x14], #16					 \n"
		"	fmul	v15.2d, v8.2d, v4.d[0]			 \n"

		"	ldr		q6, [x15], #16					 \n"
		"	fmul	v16.2d, v8.2d, v5.d[0]			 \n"
		"	fmul	v17.2d, v8.2d, v6.d[0]			 \n"

		"	add		x17, x17, x4, lsl #4			 \n"

		"	ldr		q8, [x18]						 \n"
		"   prfm    PLDL1KEEP, [x17, #8]       	 	 \n"

		"	add		x18, x18, x4, lsl #4			 \n"

		".endm 										 \n"



		".macro	KERNEL7x2_K0     					 \n"


		"   prfm    PLDL1KEEP, [x18, #8]       	 	 \n"

		"	fmla	v11.2d, v8.2d, v0.d[0]			 \n"
		"	fmla	v12.2d, v8.2d, v1.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x9, #64]			 \n"
		"	fmla	v13.2d, v8.2d, v2.d[0]			 \n"
		"	fmla	v14.2d, v8.2d, v3.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x13, #64]			 \n"
		"	prfm	PLDL1KEEP, [x14, #64]			 \n"
		"	prfm	PLDL1KEEP, [x15, #64]			 \n"
		"	fmla	v15.2d, v8.2d, v4.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x10, #64]			 \n"
		"	fmla	v16.2d, v8.2d, v5.d[0]			 \n"
		"	fmla	v17.2d, v8.2d, v6.d[0]			 \n"

		"	prfm	PLDL1KEEP, [x9, #64]			 \n"
		"	ldr		q8, [x18]						 \n"

		"	prfm	PLDL1KEEP, [x11, #64]			 \n"
		"	prfm	PLDL1KEEP, [x12, #64]			 \n"

		"	add 	x18, x18, x4, lsl #4			 \n"

		".endm 										 \n"


		".macro	KERNEL7x2_K1     					 \n"


		"   prfm    PLDL1KEEP, [x17, #8]       	 	 \n"

		"	fmla	v11.2d, v8.2d, v0.d[1]			 \n"
		"	ldr		q0, [x9], #16					 \n"

		"	fmla	v12.2d, v8.2d, v1.d[1]			 \n"
		"	ldr		q1, [x10], #16					 \n"

		"	fmla	v13.2d, v8.2d, v2.d[1]			 \n"
		"	ldr		q2, [x11], #16					 \n"

		"	fmla	v14.2d, v8.2d, v3.d[1]			 \n"
		"	ldr		q3, [x12], #16					 \n"

		"	fmla	v15.2d, v8.2d, v4.d[1]			 \n"
		"	fmla	v16.2d, v8.2d, v5.d[1]			 \n"
		"	fmla	v17.2d, v8.2d, v6.d[1]			 \n"
		"	ldr		q8, [x17]						 \n"

		"	ldr		q4, [x13], #16					 \n"
		"	ldr		q5, [x14], #16					 \n"
		"	ldr		q6, [x15], #16					 \n"

		"	add 	x17, x17, x4, lsl #4			 \n"
		

		".endm 										 \n"


		".macro	KERNEL7x2_END_K     				 \n"

		"	fmla	v11.2d, v8.2d, v0.d[1]			 \n"
		"	fmla	v12.2d, v8.2d, v1.d[1]			 \n"
		"	fmla	v13.2d, v8.2d, v2.d[1]			 \n"
		"	fmla	v14.2d, v8.2d, v3.d[1]			 \n"
		"	fmla	v15.2d, v8.2d, v4.d[1]			 \n"
		"	fmla	v16.2d, v8.2d, v5.d[1]			 \n"
		"	fmla	v17.2d, v8.2d, v6.d[1]			 \n"

		".endm 										 \n"



		".macro SAVE7x2							 	 \n"

		"	subs	x21, x21, #1					 \n"

		"	str		q11, [x25]					 	 \n"
		"	str		q12, [x26]					 	 \n"
		"	str		q13, [x27]					 	 \n"
		"	str		q14, [x28]					 	 \n"
		"	str		q15, [x29]					 	 \n"

		"	add		x8, x8, x5, lsl #5				 \n"
		"	add	  	x8, x8, x5, lsl #4				 \n"
		"	add		x8, x8, x5, lsl #3				 \n"

		"	add		x28, x28, x4, lsl #4 			 \n"
		"	add 	x29, x29, x4, lsl #4 			 \n"


		"	str		q16, [x28] 				 		 \n"
		"	add 	x25, x29, x4, lsl #3			 \n"
		"	str 	q17, [x29]					 	 \n"
		"	add 	x26, x25, x4, lsl #3			 \n"
		"	add 	x27, x26, x4, lsl #3			 \n"
		"	add 	x28, x27, x4, lsl #3			 \n"
		"	add 	x29, x28, x4, lsl #3			 \n"


		".endm 									 	 \n"



		".macro		EDGE_K_KERNEL7x2     		 	 \n"


		"	ldr		d0, [x9], #8 					 \n"
		"	ldr		d1, [x10], #8					 \n"
		"	ldr		q8, [x17]					     \n"

		"	fmla	v11.2d, v8.2d, v0.d[0]			 \n"
		"	ldr 	d2, [x11], #8					 \n"
		"	fmla	v12.2d, v8.2d, v1.d[0]			 \n"

		"	ldr 	d3, [x12], #8					 \n"
		"	fmla	v13.2d, v8.2d, v2.d[0]			 \n"
		"	ldr		d4, [x13], #8 					 \n"
		"	fmla	v14.2d, v8.2d, v3.d[0]			 \n"

		"	ldr 	d5, [x14], #8 					 \n"
		"	fmla	v15.2d, v8.2d, v4.d[0]			 \n"
		"	ldr 	d6, [x15], #8 					 \n"
		"	fmla	v16.2d, v8.2d, v5.d[0]			 \n"
		"	fmla	v17.2d, v8.2d, v6.d[0]			 \n"

		"	add		x17, x17, x4, lsl #3			 \n"
		"	prfm    PLDL1KEEP, [x17, #8]			 \n"


		".endm 										 \n"


		".macro	KERNEL6x2_BEGIN_K					 \n"

		"	ldr		q0, [x9], #16					 \n"
		"	ldr		q1, [x10], #16					 \n"

		"	ldr		q8, [x17]				     	 \n"

		"   prfm    PLDL1KEEP, [x9, #64]       	 	 \n"
		"	prfm	PLDL1KEEP, [x10, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x14, #64] 			 \n"

		"	fmul	v11.2d, v8.2d, v0.d[0]			 \n"
		"	ldr		q2, [x11], #16				 	 \n"
		"	fmul	v12.2d, v8.2d, v1.d[0]			 \n"
		"	ldr		q3, [x12], #16					 \n"
		"	fmul	v13.2d, v8.2d, v2.d[0]			 \n"

		"   prfm    PLDL1KEEP, [x17, #64]       	 \n"

		"	ldr		q4, [x13], #16					 \n"
		"	fmul	v14.2d, v8.2d, v3.d[0]			 \n"

		"	ldr		q5, [x14], #16					 \n"
		"	fmul	v15.2d, v8.2d, v4.d[0]			 \n"
		"	fmul	v16.2d, v8.2d, v5.d[0]			 \n"

		"	add		x17, x17, x4, lsl #4			 \n"
		"	ldr		q8, [x18]						 \n"
		"   prfm    PLDL1KEEP, [x17, #8]       	 	 \n"
		"	add		x18, x18, x4, lsl #4			 \n"

		".endm 										 \n"



		".macro	KERNEL6x2_K0     					 \n"


		"   prfm    PLDL1KEEP, [x18, #8]       	 	 \n"

		"	fmla	v11.2d, v8.2d, v0.d[0]			 \n"
		"	fmla	v12.2d, v8.2d, v1.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x9, #64]			 \n"
		"	fmla	v13.2d, v8.2d, v2.d[0]			 \n"
		"	fmla	v14.2d, v8.2d, v3.d[0]			 \n"
		"	fmla	v15.2d, v8.2d, v4.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x10, #64]			 \n"
		"	fmla	v16.2d, v8.2d, v5.d[0]			 \n"

		"	ldr		q8, [x18]						 \n"

		"	prfm	PLDL1KEEP, [x11, #64]			 \n"
		"	prfm	PLDL1KEEP, [x12, #64]			 \n"
		"	prfm	PLDL1KEEP, [x13, #64]			 \n"
		"	prfm	PLDL1KEEP, [x14, #64]			 \n"

		"	add 	x18, x18, x4, lsl #4			 \n"

		".endm 										 \n"


		".macro	KERNEL6x2_K1     					 \n"


		"   prfm    PLDL1KEEP, [x17, #8]       		 \n"

		"	fmla	v11.2d, v8.2d, v0.d[1]			 \n"
		"	ldr		q0, [x9], #16					 \n"

		"	fmla	v12.2d, v8.2d, v1.d[1]			 \n"
		"	ldr		q1, [x10], #16					 \n"

		"	fmla	v13.2d, v8.2d, v2.d[1]			 \n"
		"	ldr		q2, [x11], #16					 \n"

		"	fmla	v14.2d, v8.2d, v3.d[1]			 \n"
		"	ldr		q3, [x12], #16					 \n"

		"	fmla	v15.2d, v8.2d, v4.d[1]			 \n"
		"	fmla	v16.2d, v8.2d, v5.d[1]			 \n"
		"	ldr		q8, [x17]						 \n"
	
		"	ldr		q4, [x13], #16					 \n"
		"	ldr		q5, [x14], #16					 \n"
		"	add 	x17, x17, x4, lsl #4			 \n"
		

		".endm 										 \n"



		".macro	KERNEL6x2_END_K     				 \n"

		"	fmla	v11.2d, v8.2d, v0.d[1]			 \n"
		"	fmla	v12.2d, v8.2d, v1.d[1]			 \n"
		"	fmla	v13.2d, v8.2d, v2.d[1]			 \n"
		"	fmla	v14.2d, v8.2d, v3.d[1]			 \n"
		"	fmla	v15.2d, v8.2d, v4.d[1]			 \n"
		"	fmla	v16.2d, v8.2d, v5.d[1]			 \n"


		".endm 										 \n"



		".macro SAVE6x2							 	 \n"

		"	str		q11, [x25]					 	 \n"
		"	str		q12, [x26]					 	 \n"
		"	str		q13, [x27]					 	 \n"
		"	str		q14, [x28]					 	 \n"
		"	str		q15, [x29]					 	 \n"


		"	add		x28, x28, x4, lsl #4 			 \n"

		"	str		q16, [x28] 				 		 \n"

		".endm 									 	 \n"


		".macro	EDGE_K_KERNEL6x2					 \n"



		"	ldr		d0, [x9], #8 					 \n"
		"	ldr		d1, [x10], #8					 \n"
		"	ldr		q8, [x17]						 \n"

		"	fmla	v11.2d, v8.2d, v0.d[0]			 \n"
		"	ldr 	d2, [x11], #8					 \n"
		"	fmla	v12.2d, v8.2d, v1.d[0]			 \n"

		"	ldr 	d3, [x12], #8					 \n"
		"	fmla	v13.2d, v8.2d, v2.d[0]			 \n"
		"	ldr		d4, [x13], #8 					 \n"
		"	fmla	v14.2d, v8.2d, v3.d[0]			 \n"

		"	ldr 	d5, [x14], #8 					 \n"
		"	fmla	v15.2d, v8.2d, v4.d[0]			 \n"
		"	fmla	v16.2d, v8.2d, v5.d[0]			 \n"

		"	add		x17, x17, x4, lsl #3			 \n"
		"	prfm    PLDL1KEEP, [x17, #8]			 \n"


		".endm 										 \n"



		".macro	KERNEL5x2_BEGIN_K					 \n"

		"	ldr		q0, [x9], #16					 \n"
		"	ldr		q1, [x10], #16					 \n"

		"	ldr		q8, [x17]				     	 \n"

		"   prfm    PLDL1KEEP, [x9, #64]       	 	 \n"
		"	prfm	PLDL1KEEP, [x10, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x11, #64]       	 \n"
		"	prfm	PLDL1KEEP, [x12, #64] 			 \n"
		"   prfm    PLDL1KEEP, [x13, #64]       	 \n"

		"	fmul	v11.2d, v8.2d, v0.d[0]			 \n"
		"	ldr		q2, [x11], #16				 	 \n"
		"	fmul	v12.2d, v8.2d, v1.d[0]			 \n"
		"	ldr		q3, [x12], #16					 \n"
		"	fmul	v13.2d, v8.2d, v2.d[0]			 \n"

		"   prfm    PLDL1KEEP, [x17, #8]       	 	 \n"

		"	ldr		q4, [x13], #16					 \n"
		"	fmul	v14.2d, v8.2d, v3.d[0]			 \n"

		"	fmul	v15.2d, v8.2d, v4.d[0]			 \n"
		"	add		x17, x17, x4, lsl #4			 \n"
		"	ldr		q8, [x18]						 \n"
		"   prfm    PLDL1KEEP, [x17, #8]       	 	 \n"

		"	add		x18, x18, x4, lsl #4			 \n"

		".endm 										 \n"



		".macro	KERNEL5x2_K0     					 \n"


		"   prfm    PLDL1KEEP, [x18, #64]       	 \n"

		"	fmla	v11.2d, v8.2d, v0.d[0]			 \n"
		"	fmla	v12.2d, v8.2d, v1.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x9, #64]			 \n"
		"	fmla	v13.2d, v8.2d, v2.d[0]			 \n"
		"	fmla	v14.2d, v8.2d, v3.d[0]			 \n"
		"	fmla	v15.2d, v8.2d, v4.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x10, #64]			 \n"
		"	ldr		q8, [x18]						 \n"


		"	prfm	PLDL1KEEP, [x11, #64]			 \n"
		"	prfm	PLDL1KEEP, [x12, #64]			 \n"
		"	prfm	PLDL1KEEP, [x13, #64]			 \n"
		"	add 	x18, x18, x4, lsl #4			 \n"

		".endm 										 \n"


		".macro	KERNEL5x2_K1     					 \n"


		"   prfm    PLDL1KEEP, [x17, #8]       	 	 \n"

		"	fmla	v11.2d, v8.2d, v0.d[1]			 \n"
		"	ldr		q0, [x9], #16					 \n"

		"	fmla	v12.2d, v8.2d, v1.d[1]			 \n"
		"	ldr		q1, [x10], #16					 \n"

		"	fmla	v13.2d, v8.2d, v2.d[1]			 \n"
		"	ldr		q2, [x11], #16					 \n"

		"	fmla	v14.2d, v8.2d, v3.d[1]			 \n"
		"	ldr		q3, [x12], #16					 \n"

		"	fmla	v15.2d, v8.2d, v4.d[1]			 \n"
		"	ldr		q8, [x17]						 \n"

		"	ldr		q4, [x13], #16					 \n"
		"	add 	x17, x17, x4, lsl #4			 \n"
		

		".endm 										 \n"


		".macro	KERNEL5x2_END_K     				 \n"

		"	fmla	v11.2d, v8.2d, v0.d[1]			 \n"
		"	fmla	v12.2d, v8.2d, v1.d[1]			 \n"
		"	fmla	v13.2d, v8.2d, v2.d[1]			 \n"
		"	fmla	v14.2d, v8.2d, v3.d[1]			 \n"
		"	fmla	v15.2d, v8.2d, v4.d[1]			 \n"

		".endm 										 \n"



		".macro SAVE5x2							 	 \n"

		"	str		q11, [x25]					 	 \n"
		"	str		q12, [x26]					 	 \n"
		"	str		q13, [x27]					 	 \n"
		"	str		q14, [x28]					 	 \n"
		"	str		q15, [x29]					 	 \n"

		".endm 									 	 \n"


		".macro	EDGE_K_KERNEL5x2					 \n"


		"	ldr		d0, [x9], #8 					 \n"
		"	ldr		d1, [x10], #8					 \n"
		"	ldr		q8, [x17]					 	 \n"

		"	fmla	v11.2d, v8.2d, v0.d[0]			 \n"
		"	ldr 	d2, [x11], #8					 \n"
		"	fmla	v12.2d, v8.2d, v1.d[0]			 \n"

		"	ldr 	d3, [x12], #8					 \n"
		"	fmla	v13.2d, v8.2d, v2.d[0]			 \n"
		"	ldr		d4, [x13], #8 					 \n"
		"	fmla	v14.2d, v8.2d, v3.d[0]			 \n"
		"	fmla	v15.2d, v8.2d, v4.d[0]			 \n"

		"	add		x17, x17, x4, lsl #3			 \n"
		"	prfm    PLDL1KEEP, [x17, #8]			 \n"


		".endm 										 \n"


		".macro	KERNEL2x2_BEGIN_K					 \n"

		"	ldr		q0, [x9], #16					 \n"
		"	ldr		q1, [x10], #16					 \n"

		"	ldr		q8, [x17]				     	 \n"

		"   prfm    PLDL1KEEP, [x9, #64]       	 	 \n"
		"	prfm	PLDL1KEEP, [x10, #64] 			 \n"

		"	fmul	v11.2d, v8.2d, v0.d[0]			 \n"
		"	fmul	v12.2d, v8.2d, v1.d[0]			 \n"

		"	add		x17, x17, x4, lsl #4			 \n"
		"	ldr		q8, [x18]						 \n"

		"   prfm    PLDL1KEEP, [x17, #8]       	 	 \n"

		"	add		x18, x18, x4, lsl #4			 \n"

		".endm 										 \n"



		".macro	KERNEL2x2_K0     					 \n"


		"   prfm    PLDL1KEEP, [x18, #8]       	 \n"

		"	fmla	v11.2d, v8.2d, v0.d[0]			 \n"
		"	fmla	v12.2d, v8.2d, v1.d[0]			 \n"
		"	prfm	PLDL1KEEP, [x9, #64]			 \n"
		"	prfm	PLDL1KEEP, [x10, #64]			 \n"

		"	ldr		q8, [x18]						 \n"
		"	prfm	PLDL1KEEP, [x11, #64]			 \n"
		"	add 	x18, x18, x4, lsl #4			 \n"

		".endm 										 \n"


		".macro	KERNEL2x2_K1     					 \n"


		"   prfm    PLDL1KEEP, [x17, #8]       	 	 \n"

		"	fmla	v11.2d, v8.2d, v0.d[1]			 \n"
		"	ldr		q0, [x9], #16					 \n"

		"	fmla	v12.2d, v8.2d, v1.d[1]			 \n"
		"	ldr		q8, [x17]						 \n"

		"	ldr		q1, [x10], #16					 \n"
		"	add 	x17, x17, x4, lsl #4			 \n"		

		".endm 										 \n"


		".macro	KERNEL2x2_END_K     				 \n"

		"	fmla	v11.2d, v8.2d, v0.d[1]			 \n"
		"	fmla	v12.2d, v8.2d, v1.d[1]			 \n"


		".endm 										 \n"



		".macro SAVE2x2							 	 \n"

		"	str		q11, [x25]					 	 \n"
		"	str		q12, [x26]					 	 \n"

		".endm 									 	 \n"


		".macro	EDGE_K_KERNEL2x2					 \n"


		"	ldr		d0, [x9], #8 					 \n"
		"	ldr		d1, [x10], #8					 \n"
		"	ldr		q8, [x17]					 	 \n"

		"	fmla	v11.2d, v8.2d, v0.d[0]			 \n"
		"	fmla	v18.2d, v9.2d, v0.d[0]			 \n"

		"	add		x17, x17, x4, lsl #3			 \n"
		"	prfm    PLDL1KEEP, [x17, #16]			 \n"


		".endm 										 \n"


		".macro	BEGIN_KERNEL4x1_K  				    \n"


		"	ldr		d4, [x17] 					 	\n"
		"	ldr 	d5, [x18]						\n"

		"	ins		v4.d[1], v5.d[0]				\n"

		"	prfm	PLDL1KEEP, [x9, #64]			\n"

		"	ldr		q0, [x9], #16				 	\n"
		"	add		x17, x17, x4, lsl #4			\n"
		"	ldr		q1, [x10], #16				  	\n"
		"	add		x18, x18, x4, lsl #4			\n"

		"	prfm	PLDL1KEEP, [x10, #64]			\n"

		"	fmul	v11.2d, v0.2d, v4.2d			\n"
		"	ldr		q2, [x11], #16					\n"
		"	prfm	PLDL1KEEP, [x11, #64]			\n"
		"	fmul	v12.2d, v1.2d, v4.2d		    \n"
		"	ldr		q3, [x12], #16					\n"
		"	prfm	PLDL1KEEP, [x12, #64]			\n"
		"	fmul	v13.2d, v2.2d, v4.2d			\n"
		"	fmul	v14.2d, v3.2d, v4.2d			\n"

		".endm 									 	\n"



		".macro	KERNEL4x1_K  						\n"


		"	ldr		d4, [x17] 					 	\n"
		"	ldr 	d5, [x18]						\n"

		"	ins		v4.d[1], v5.d[0]				\n"

		"	prfm	PLDL1KEEP, [x9, #64]			\n"

		"	ldr		q0, [x9], #16				 	\n"
		"	add		x17, x17, x4, lsl #4			\n"
		"	prfm	PLDL1KEEP, [x17, #8]			\n"
		"	ldr		q1, [x10], #16				  	\n"
		"	add		x18, x18, x4, lsl #4			\n"
		"	prfm	PLDL1KEEP, [x18, #8]			\n"

		"	prfm	PLDL1KEEP, [x10, #64]			\n"

		"	fmla	v11.2d, v0.2d, v4.2d			\n"
		"	ldr		q2, [x11], #16					\n"
		"	prfm	PLDL1KEEP, [x11, #64]			\n"
		"	fmla	v12.2d, v1.2d, v4.2d		    \n"
		"	ldr		q3, [x12], #16					\n"
		"	prfm	PLDL1KEEP, [x12, #64]			\n"
		"	fmla	v13.2d, v2.2d, v4.2d			\n"
		"	fmla	v14.2d, v3.2d, v4.2d			\n"

		".endm 									 	\n"


		".macro	EDGE_K_KERNEL4x1  				    \n"


		"	ldr		d4, [x17]						\n"

		"	ldr		d0, [x9]					 	\n"
		"	ldr		d1, [x10]						\n"
		"	ldr		d2, [x11]					 	\n"
		"	ldr		d3, [x12]						\n"

		"	fmadd	d21, d0, d4, d11 			    \n"
		"	ins 	v11.d[0], v21.d[0]				\n"
		"	fmadd	d22, d1, d4, d12 				\n"
		"	ins 	v12.d[0], v22.d[0]				\n"
		"	fmadd	d23, d2, d4, d13 				\n"
		"	ins 	v13.d[0], v23.d[0]				\n"
		"	fmadd 	d24, d3, d4, d14 			    \n"
		"	ins 	v14.d[0], v24.d[0]				\n"

		".endm 										\n"


		".macro SAVE4x1								\n"

		"	subs 	x21, x21, #1					\n"

		"	faddp	d15, v11.2d			 			\n"
		"	faddp	d16, v12.2d 					\n"
		"	faddp 	d17, v13.2d 					\n"
		"	faddp 	d18, v14.2d 					\n"

		"	add		x8, x8, x5, lsl #5			    \n"

		"	str 	d15, [x25]					    \n"
		"	add		x25, x25, x4, lsl #5	        \n"
		"	str 	d16, [x26]						\n"
		"	add		x26, x26, x4, lsl #5	        \n"
		"	str 	d17, [x27]						\n"
		"	add		x27, x27, x4, lsl #5	        \n"
		"	str 	d18, [x28]						\n"
		"	add		x28, x28, x4, lsl #5	        \n"


		".endm 										\n"



		".macro KERNEL1x1_BEGIN_K					\n"

		"	ldr		d4, [x17] 					 	\n"
		"	ldr 	d5, [x18]						\n"

		"	ins		v4.d[1], v5.d[0]				\n"

		"	prfm	PLDL1KEEP, [x9, #64]			\n"

		"	ldr		q0, [x9], #16				 	\n"
		"	add		x17, x17, x4, lsl #4			\n"
		"	prfm	PLDL1KEEP, [x17, #8]			\n"
		"	add		x18, x18, x4, lsl #4			\n"
		"	prfm	PLDL1KEEP, [x18, #8]			\n"

		"	fmul	v11.2d, v0.2d, v4.2d			\n"

		".endm 										\n"


		".macro	KERNEL1x1_K  						\n"


		"	ldr		d4, [x17] 					 	\n"
		"	ldr 	d5, [x18]						\n"

		"	ins		v4.d[1], v5.d[0]				\n"

		"	prfm	PLDL1KEEP, [x9, #64]			\n"

		"	ldr		q0, [x9], #16				 	\n"
		"	add		x17, x17, x4, lsl #4			\n"
		"	prfm	PLDL1KEEP, [x17, #8]			\n"
		"	add		x18, x18, x4, lsl #4			\n"
		"	prfm	PLDL1KEEP, [x18, #8]			\n"

		"	fmla	v11.2d, v0.2d, v4.2d			\n"

		".endm 									 	\n"


		".macro	EDGE_K_KERNEL1x1  				    \n"


		"	ldr		d4, [x17]						\n"

		"	ldr		d0, [x9]					 	\n"

		"	fmadd	d21, d0, d4, d11 			    \n"
		"	ins 	v11.d[0], v21.d[0]				\n"

		".endm 										\n"


		".macro SAVE1x1								\n"

		"	faddp	d15, v11.2d			 			\n"
		"	subs 	x21, x21, #1					\n"

		"	add		x8, x8, x5, lsl #3			    \n"
		"	str 	d15, [x25]					    \n"
		"	add		x25, x25, x4, lsl #3	        \n"

		".endm 										\n"





		"SMM:									 	 \n"

		"	ldr		x0, %[C]						 \n"
		"	ldr		x1, %[A]						 \n"
		"	ldr		x2, %[B]						 \n"

		"	ldr		x3, %[M]						 \n"
		"	ldr		x4, %[N]						 \n"
		"	ldr		x5, %[K]						 \n"
//		"	ldr		x6, %[temp]						 \n"

//		"	ldr		x19, %[SB]						 \n"

		"	mov		x23, #7						 	 \n"

		"   prfm    PLDL1KEEP, [x1]             	 \n"
		"   prfm    PLDL1KEEP, [x2]             	 \n"

		"	mov		x16, x2 						 \n"  	//B11
		"	udiv	x30, x3, x23				 	 \n"	// M / 7
		"	msub	x23, x30, x23, x3 				 \n"    // M % 7

		"	mov		x7, #6							 \n"
		"	udiv	x20, x4, x7						 \n"	// N / 6

		"	cmp		x20, #0							 \n"
		"	beq 	END_N6	 					 	 \n"

		"BEGIN_N6:								 	 \n"

		"	mov 	x25, x0   						 \n"   //C1*
		"	add		x26, x25, x4, lsl #3 			 \n"   //C2*
		"	add 	x27, x26, x4, lsl #3 			 \n"   //C3*
		" 	add 	x28, x27, x4, lsl #3 			 \n"   //C4*
		"	add		x29, x28, x4, lsl #3 			 \n"   //C5*

		"	cmp		x30, #1							 \n"
		"	blt		END_M7 						 	 \n"

		"	mov		x8, x1   						 \n"
		"	mov		x21, x30  						 \n"   // M / 7

		"BEGIN_M7:									 \n"

		"	mov		x17, x16						 \n"   //B0*
		"	add		x18, x17, x4, lsl #3 					 \n"


		"	mov		x9, x8						 	 \n"   //A0*
		"	add		x10, x9, x5, lsl #3				 \n"   //A1*
		"	add		x11, x10, x5, lsl #3			 \n"   //A2*
		"	add		x12, x11, x5, lsl #3			 \n"   //A3*
		"	add		x13, x12, x5, lsl #3 			 \n"   //A4*
		"	add		x14, x13, x5, lsl #3			 \n"   //A5*
		"	add	  	x15, x14, x5, lsl #3			 \n"   //A6*

		"	prfm    PLDL1KEEP, [x9, #64]			 \n"
		"	prfm    PLDL1KEEP, [x10, #64]			 \n"		
		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"
		"	prfm    PLDL1KEEP, [x13, #64]			 \n"
		"	prfm    PLDL1KEEP, [x14, #64]			 \n"
		"	prfm    PLDL1KEEP, [x15, #64]			 \n"



		"Body_K:								 	 \n"

		"	lsr		x22, x5, #2 					 \n"     // K / 4
		"	KERNEL7x6_BEGIN_K						 \n"
		"	subs	x22, x22, #1		     		 \n"	
		"	b K1_3									 \n"

		"Main_K:									 \n"
		

		"	KERNEL7x6_K0						 	 \n"
		"K1_3:										 \n"
		
		"	KERNEL7x6_K1							 \n"
		"	KERNEL7x6_K0							 \n"

		"	beq		Edge_K							 \n"

		"	KERNEL7x6_K1							 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		Main_K			 				 \n"	

		"Edge_K:									 \n"

		"	prfm	PLDL1KEEP, [x25, #64]			 \n"
		"	prfm	PLDL1KEEP, [x26, #64]			 \n"
		"	prfm	PLDL1KEEP, [x27, #64]			 \n"
		"	prfm	PLDL1KEEP, [x28, #64]			 \n"
		"	prfm	PLDL1KEEP, [x29, #64]			 \n"

		"	KERNEL7x6_END_K						 	 \n"

		"	ands	x22, x5, #3					 	 \n"
		"	beq		N6_SAVE 						 \n"

		"N6_EDGE_K:								 	 \n"
		"	EDGE_K_KERNEL7x6 						 \n"
		"	subs 	x22, x22, #1					 \n"
		"	bgt 	N6_EDGE_K  						 \n"

		"N6_SAVE:									 \n"

		"	SAVE7x6									 \n"

		"	bgt   	BEGIN_M7						 \n"

		"END_M7: 								 	 \n"


//------------------------------------------------------M6
		"	mov		x17, x16						 \n"   //B0*
		"	add		x18, x17, x4, lsl #3 			 \n"

		"	cmp		x23, #6							 \n"
		"	bne 	BEGIN_M5 						 \n"


		"BEGIN_M6:						 			 \n"

		"	mov		x9, x8 						  	 \n"
		"	add		x10, x9, x5, lsl #3 			 \n"
		"	add		x11, x10, x5, lsl #3 			 \n"
		"	add		x12, x11, x5, lsl #3 			 \n"
		"	add 	x13, x12, x5, lsl #3			 \n"
		"	add 	x14, x13, x5, lsl #3			 \n"

		"	prfm    PLDL1KEEP, [x9, #64]			 \n"
		"	prfm    PLDL1KEEP, [x10, #64]			 \n"
		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"
		"	prfm    PLDL1KEEP, [x13, #64]			 \n"
		"	prfm    PLDL1KEEP, [x14, #64]			 \n"


		"M6_Body_K:								 	 \n"

		"	lsr		x22, x5, #2 					 \n"     // K / 8
		"	KERNEL6x6_BEGIN_K						 \n"
		"	subs	x22, x22, #1		     		 \n"	
		"	b 		M6_K1_3							 \n"

		"M6_Main_K:									 \n"
		
		"	KERNEL6x6_K0						 	 \n"
		"M6_K1_3:									 \n"
		
		"	KERNEL6x6_K1							 \n"
		"	KERNEL6x6_K0							 \n"

		"	beq		M6_Edge_K						 \n"

		"	KERNEL6x6_K1							 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		M6_Main_K			 			 \n"	

		"M6_Edge_K:									 \n"


		"	prfm	PLDL1KEEP, [x25, #64]			 \n"
		"	prfm	PLDL1KEEP, [x26, #64]			 \n"
		"	prfm	PLDL1KEEP, [x27, #64]			 \n"
		"	prfm	PLDL1KEEP, [x28, #64]			 \n"
		"	prfm	PLDL1KEEP, [x29, #64]			 \n"

		"	KERNEL6x6_END_K						 	 \n"	


		"	ands	x22, x5, #3					 	 \n"
		"	beq		N6M6_SAVE 						 \n"

		"N6M6_EDGE_K:								 \n"
		"	EDGE_K_KERNEL6x6 						 \n"
		"	subs 	x22, x22, #1					 \n"
		"	bgt 	N6M6_EDGE_K  					 \n"

		"N6M6_SAVE:									 \n"

		"	SAVE6x6									 \n"

		"	b 		END_M 							 \n"



//-------------------------------------------------------


		"BEGIN_M5:									\n"


		"	cmp		x23, #5							 \n"
		"	bne 	BEGIN_M2 						 \n"

		"	mov		x9, x8 						  	 \n"
		"	add		x10, x9, x5, lsl #3 			 \n"
		"	add		x11, x10, x5, lsl #3 			 \n"
		"	add		x12, x11, x5, lsl #3 			 \n"
		"	add 	x13, x12, x5, lsl #3			 \n"

		"	prfm    PLDL1KEEP, [x9, #64]			 \n"
		"	prfm    PLDL1KEEP, [x10, #64]			 \n"
		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"
		"	prfm    PLDL1KEEP, [x13, #64]			 \n"


		"M5_Body_K:								 	 \n"

		"	lsr		x22, x5, #2 					 \n"     // K / 8
		"	KERNEL5x6_BEGIN_K						 \n"
		"	subs	x22, x22, #1		     		 \n"	
		"	b 		M5_K1_3							 \n"

		"M5_Main_K:									 \n"
		
		"	KERNEL5x6_K0						 	 \n"
		"M5_K1_3:									 \n"
		
		"	KERNEL5x6_K1							 \n"
		"	KERNEL5x6_K0							 \n"

		"	beq		M5_Edge_K						 \n"

		"	KERNEL5x6_K1							 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		M5_Main_K			 			 \n"	

		"M5_Edge_K:									 \n"


		"	prfm	PLDL1KEEP, [x25, #64]			 \n"
		"	prfm	PLDL1KEEP, [x26, #64]			 \n"
		"	prfm	PLDL1KEEP, [x27, #64]			 \n"
		"	prfm	PLDL1KEEP, [x28, #64]			 \n"
		"	prfm	PLDL1KEEP, [x29, #64]			 \n"

		"	KERNEL5x6_END_K						 	 \n"	


		"	ands	x22, x5, #3					 	 \n"
		"	beq		N6M5_SAVE 						 \n"

		"N6M5_EDGE_K:								 \n"
		"	EDGE_K_KERNEL5x6 						 \n"
		"	subs 	x22, x22, #1					 \n"
		"	bgt 	N6M5_EDGE_K  					 \n"

		"N6M5_SAVE:									 \n"

		"	SAVE5x6									 \n"

		"	b 		END_M 							 \n"


//------------------------------------------------------M2
		"BEGIN_M2:									 \n"


		"	cmp		x23, #2							 \n"
		"	bne 	END_M 						 	 \n"

		"	mov		x9, x8 						  	 \n"
		"	add		x10, x9, x5, lsl #3 			 \n"

		"	prfm    PLDL1KEEP, [x9, #64]			 \n"
		"	prfm    PLDL1KEEP, [x10, #64]			 \n"


		"M2_Body_K:								 	 \n"

		"	lsr		x22, x5, #2 					 \n"     // K / 8
		"	KERNEL2x6_BEGIN_K						 \n"
		"	subs	x22, x22, #1		     		 \n"	
		"	b 		M2_K1_3							 \n"

		"M2_Main_K:									 \n"
		
		"	KERNEL2x6_K0						 	 \n"
		"M2_K1_3:									 \n"
		
		"	KERNEL2x6_K1							 \n"
		"	KERNEL2x6_K0							 \n"

		"	beq		M2_Edge_K						 \n"

		"	KERNEL2x6_K1							 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		M2_Main_K			 			 \n"	

		"M2_Edge_K:									 \n"

		"	prfm	PLDL1KEEP, [x25, #64]			 \n"
		"	prfm	PLDL1KEEP, [x26, #64]			 \n"

		"	KERNEL2x6_END_K						 	 \n"	


		"	ands	x22, x5, #3					 	 \n"
		"	beq		N6M2_SAVE 						 \n"

		"N6M2_EDGE_K:								 \n"
		"	EDGE_K_KERNEL2x6 						 \n"
		"	subs 	x22, x22, #1					 \n"
		"	bgt 	N6M2_EDGE_K  					 \n"

		"N6M2_SAVE:									 \n"

		"	SAVE2x6									 \n"


		"END_M:										 \n"


		"	subs	x20, x20, #1					 \n"

		"	add		x16, x16, #48					 \n"
		"	add		x0, x0, #48						 \n"

		"	bgt		BEGIN_N6						 \n"


//---------------------------------------------------------

		"END_N6:									 \n"

		"	udiv	x20, x4, x7 				 	 \n"
		"	msub	x20, x20, x7, x4 				 \n"

		"	cmp 	x20, #0 						 \n"
		"	beq 	END_N 							 \n"











//----------------------------------------------------------N4M7
	  	"BEGIN_N4: 								     \n"

	  	"	cmp		x20, #4							 \n"
	  	"	blt 	BEGIN_N2 						 \n"

		"	mov 	x25, x0   						 \n"   //C1*
		"	add		x26, x25, x4, lsl #3 			 \n"   //C2*
		"	add 	x27, x26, x4, lsl #3 			 \n"   //C3*
		" 	add 	x28, x27, x4, lsl #3 			 \n"   //C4*
		"	add		x29, x28, x4, lsl #3 			 \n"   //C5*

		"	mov		x8, x1   						 \n"
		"	cmp		x30, #1							 \n"
		"	blt		END_N4M7 						 \n"

		"	mov		x21, x30  						 \n"   // M / 7

		//--------------------------------------------------N8M5
		"N4_BEGIN_M7:								 \n"


		"	mov		x17, x16						 \n"   //B0*
		"	add		x18, x17, x4, lsl #3 			 \n"


		"	mov		x9, x8						 	 \n"   //A0*
		"	add		x10, x9, x5, lsl #3				 \n"   //A1*
		"	add		x11, x10, x5, lsl #3			 \n"   //A2*
		"	add		x12, x11, x5, lsl #3			 \n"   //A3*
		"	add		x13, x12, x5, lsl #3 			 \n"   //A4*
		"	add		x14, x13, x5, lsl #3			 \n"   //A5*
		"	add	  	x15, x14, x5, lsl #3			 \n"   //A6*

		"	prfm    PLDL1KEEP, [x9, #64]			 \n"
		"	prfm    PLDL1KEEP, [x10, #64]			 \n"		
		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"
		"	prfm    PLDL1KEEP, [x13, #64]			 \n"
		"	prfm    PLDL1KEEP, [x14, #64]			 \n"
		"	prfm    PLDL1KEEP, [x15, #64]			 \n"



		"N4_Body_K:								 	 \n"

		"	lsr		x22, x5, #2 					 \n"     // K / 4
		"	KERNEL7x4_BEGIN_K						 \n"
		"	subs	x22, x22, #1		     		 \n"	
		"	b 		N4_K1_3							 \n"

		"N4_Main_K:									 \n"
		

		"	KERNEL7x4_K0						 	 \n"
		"N4_K1_3:									 \n"
		
		"	KERNEL7x4_K1							 \n"
		"	KERNEL7x4_K0							 \n"

		"	beq		N4_Edge_K						 \n"

		"	KERNEL7x4_K1							 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		N4_Main_K			 			 \n"	

		"N4_Edge_K:									 \n"

		"	prfm	PLDL1KEEP, [x25, #64]			 \n"
		"	prfm	PLDL1KEEP, [x26, #64]			 \n"
		"	prfm	PLDL1KEEP, [x27, #64]			 \n"
		"	prfm	PLDL1KEEP, [x28, #64]			 \n"
		"	prfm	PLDL1KEEP, [x29, #64]			 \n"

		"	KERNEL7x4_END_K						 	 \n"

		"	ands	x22, x5, #3					 	 \n"
		"	beq		N4_SAVE 						 \n"

		"N4_EDGE_K:								 	 \n"
		"	EDGE_K_KERNEL7x4 						 \n"
		"	subs 	x22, x22, #1					 \n"
		"	bgt 	N4_EDGE_K  						 \n"

		"N4_SAVE:									 \n"

		"	SAVE7x4									 \n"

		"	bgt   	N4_BEGIN_M7						 \n"

		"END_N4M7: 									 \n"


		//------------------------------------------------------N4M6
		"	mov		x17, x16						 \n"   //B0*
		"	add		x18, x17, x4, lsl #3 			 \n"

		"	cmp		x23, #6							 \n"
		"	bne 	N4_BEGIN_M5 					 \n"


		"N4_BEGIN_M6:						 	     \n"

		"	mov		x9, x8 						  	 \n"
		"	add		x10, x9, x5, lsl #3 			 \n"
		"	add		x11, x10, x5, lsl #3 			 \n"
		"	add		x12, x11, x5, lsl #3 			 \n"
		"	add 	x13, x12, x5, lsl #3			 \n"
		"	add 	x14, x13, x5, lsl #3			 \n"

		"	prfm    PLDL1KEEP, [x9, #64]			 \n"
		"	prfm    PLDL1KEEP, [x10, #64]			 \n"
		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"
		"	prfm    PLDL1KEEP, [x13, #64]			 \n"
		"	prfm    PLDL1KEEP, [x14, #64]			 \n"


		"N4_M6_Body_K:								 \n"

		"	lsr		x22, x5, #2 					 \n"     // K / 8
		"	KERNEL6x4_BEGIN_K						 \n"
		"	subs	x22, x22, #1		     		 \n"	
		"	b 		N4_M6_K1_3						 \n"

		"N4_M6_Main_K:								 \n"
		
		"	KERNEL6x4_K0						 	 \n"
		"N4_M6_K1_3:								 \n"
		
		"	KERNEL6x4_K1							 \n"
		"	KERNEL6x4_K0							 \n"

		"	beq		N4_M6_Edge_K					 \n"

		"	KERNEL6x4_K1							 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		N4_M6_Main_K			 		 \n"	

		"N4_M6_Edge_K:								 \n"


		"	prfm	PLDL1KEEP, [x25, #64]			 \n"
		"	prfm	PLDL1KEEP, [x26, #64]			 \n"
		"	prfm	PLDL1KEEP, [x27, #64]			 \n"
		"	prfm	PLDL1KEEP, [x28, #64]			 \n"
		"	prfm	PLDL1KEEP, [x29, #64]			 \n"

		"	KERNEL6x4_END_K						 	 \n"	


		"	ands	x22, x5, #3					 	 \n"
		"	beq		N4M6_SAVE 						 \n"

		"N4M6_EDGE_K:								 \n"
		"	EDGE_K_KERNEL6x4 						 \n"
		"	subs 	x22, x22, #1					 \n"
		"	bgt 	N4M6_EDGE_K  					 \n"

		"N4M6_SAVE:									 \n"

		"	SAVE6x4									 \n"

		"	b 		N4_END_M 						 \n"



//-------------------------------------------------------


		"N4_BEGIN_M5:							    \n"


		"	cmp		x23, #5							 \n"
		"	bne 	N4_BEGIN_M2 					 \n"

		"	mov		x9, x8 						  	 \n"
		"	add		x10, x9, x5, lsl #3 			 \n"
		"	add		x11, x10, x5, lsl #3 			 \n"
		"	add		x12, x11, x5, lsl #3 			 \n"
		"	add 	x13, x12, x5, lsl #3			 \n"

		"	prfm    PLDL1KEEP, [x9, #64]			 \n"
		"	prfm    PLDL1KEEP, [x10, #64]			 \n"
		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"
		"	prfm    PLDL1KEEP, [x13, #64]			 \n"


		"N4_M5_Body_K:								 \n"

		"	lsr		x22, x5, #2 					 \n"     // K / 8
		"	KERNEL5x4_BEGIN_K						 \n"
		"	subs	x22, x22, #1		     		 \n"	
		"	b 		N4_M5_K1_3						 \n"

		"N4_M5_Main_K:								 \n"
		
		"	KERNEL5x4_K0						 	 \n"
		"N4_M5_K1_3:								 \n"
		
		"	KERNEL5x4_K1							 \n"
		"	KERNEL5x4_K0							 \n"

		"	beq		N4_M5_Edge_K					 \n"

		"	KERNEL5x4_K1							 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		N4_M5_Main_K			 		 \n"	

		"N4_M5_Edge_K:								 \n"


		"	prfm	PLDL1KEEP, [x25, #64]			 \n"
		"	prfm	PLDL1KEEP, [x26, #64]			 \n"
		"	prfm	PLDL1KEEP, [x27, #64]			 \n"
		"	prfm	PLDL1KEEP, [x28, #64]			 \n"
		"	prfm	PLDL1KEEP, [x29, #64]			 \n"

		"	KERNEL5x4_END_K						 	 \n"	


		"	ands	x22, x5, #3					 	 \n"
		"	beq		N4M5_SAVE 						 \n"

		"N4M5_EDGE_K:								 \n"
		"	EDGE_K_KERNEL5x4 						 \n"
		"	subs 	x22, x22, #1					 \n"
		"	bgt 	N4M5_EDGE_K  					 \n"

		"N4M5_SAVE:									 \n"

		"	SAVE5x4									 \n"

		"	b 		N4_END_M 						 \n"


//------------------------------------------------------M2
		"N4_BEGIN_M2:									 \n"

		"	cmp		x23, #2							 \n"
		"	bne 	N4_END_M 						 \n"

		"	mov		x9, x8 						  	 \n"
		"	add		x10, x9, x5, lsl #3 			 \n"

		"	prfm    PLDL1KEEP, [x9, #64]			 \n"
		"	prfm    PLDL1KEEP, [x10, #64]			 \n"


		"N4_M2_Body_K:								 \n"

		"	lsr		x22, x5, #2 					 \n"     // K / 8
		"	KERNEL2x4_BEGIN_K						 \n"
		"	subs	x22, x22, #1		     		 \n"	
		"	b 		N4_M2_K1_3						 \n"

		"N4_M2_Main_K:								 \n"
		
		"	KERNEL2x4_K0						 	 \n"
		"N4_M2_K1_3:								 \n"
		
		"	KERNEL2x4_K1							 \n"
		"	KERNEL2x4_K0							 \n"

		"	beq		N4_M2_Edge_K					 \n"

		"	KERNEL2x4_K1							 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		N4_M2_Main_K			 		 \n"	

		"N4_M2_Edge_K:								 \n"

		"	prfm	PLDL1KEEP, [x25, #64]			 \n"
		"	prfm	PLDL1KEEP, [x26, #64]			 \n"

		"	KERNEL2x4_END_K						 	 \n"	


		"	ands	x22, x5, #3					 	 \n"
		"	beq		N4M2_SAVE 						 \n"

		"N4M2_EDGE_K:								 \n"
		"	EDGE_K_KERNEL2x4 						 \n"
		"	subs 	x22, x22, #1					 \n"
		"	bgt 	N4M2_EDGE_K  					 \n"

		"N4M2_SAVE:									 \n"

		"	SAVE2x4									 \n"

		"N4_END_M:									 \n"

		"	add		x0, x0, #32						 \n"
		"	add		x16, x16, #32					 \n"

		"	subs	x20, x20, #4					 \n"
		"	beq		END_N 							 \n"











//----------------------------------------------------------N2M7
	  	"BEGIN_N2: 								     \n"


	  	"	cmp		x20, #2							 \n"
	  	"	blt     BEGIN_N1 						 \n"

		"	mov 	x25, x0   						 \n"   //C1*
		"	add		x26, x25, x4, lsl #3 			 \n"   //C2*
		"	add 	x27, x26, x4, lsl #3 			 \n"   //C3*
		" 	add 	x28, x27, x4, lsl #3 			 \n"   //C4*
		"	add		x29, x28, x4, lsl #3 			 \n"   //C5*

		"	mov		x8, x1   						 \n"
		"	cmp		x30, #1							 \n"
	  	"	blt 	END_N2M7 						 \n"

		"	mov		x21, x30  						 \n"   // M / 7


		//--------------------------------------------------N8M5
		"N2_BEGIN_M7:								 \n"


		"	mov		x17, x16						 \n"   //B0*
		"	add		x18, x17, x4, lsl #3 			 \n"


		"	mov		x9, x8						 	 \n"   //A0*
		"	add		x10, x9, x5, lsl #3				 \n"   //A1*
		"	add		x11, x10, x5, lsl #3			 \n"   //A2*
		"	add		x12, x11, x5, lsl #3			 \n"   //A3*
		"	add		x13, x12, x5, lsl #3 			 \n"   //A4*
		"	add		x14, x13, x5, lsl #3			 \n"   //A5*
		"	add	  	x15, x14, x5, lsl #3			 \n"   //A6*

		"	prfm    PLDL1KEEP, [x9, #64]			 \n"
		"	prfm    PLDL1KEEP, [x10, #64]			 \n"		
		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"
		"	prfm    PLDL1KEEP, [x13, #64]			 \n"
		"	prfm    PLDL1KEEP, [x14, #64]			 \n"
		"	prfm    PLDL1KEEP, [x15, #64]			 \n"



		"N2_Body_K:								 	 \n"

		"	lsr		x22, x5, #2 					 \n"     // K / 4
		"	KERNEL7x2_BEGIN_K						 \n"
		"	subs	x22, x22, #1		     		 \n"	
		"	b 		N2_K1_3							 \n"

		"N2_Main_K:									 \n"
		

		"	KERNEL7x2_K0						 	 \n"
		"N2_K1_3:									 \n"
		
		"	KERNEL7x2_K1							 \n"
		"	KERNEL7x2_K0							 \n"

		"	beq		N2_Edge_K						 \n"

		"	KERNEL7x2_K1							 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		N2_Main_K			 			 \n"	

		"N2_Edge_K:									 \n"

		"	prfm	PLDL1KEEP, [x25, #64]			 \n"
		"	prfm	PLDL1KEEP, [x26, #64]			 \n"
		"	prfm	PLDL1KEEP, [x27, #64]			 \n"
		"	prfm	PLDL1KEEP, [x28, #64]			 \n"
		"	prfm	PLDL1KEEP, [x29, #64]			 \n"

		"	KERNEL7x2_END_K						 	 \n"

		"	ands	x22, x5, #3					 	 \n"
		"	beq		N2_SAVE 						 \n"

		"N2_EDGE_K:								 	 \n"
		"	EDGE_K_KERNEL7x2 						 \n"
		"	subs 	x22, x22, #1					 \n"
		"	bgt 	N2_EDGE_K  						 \n"

		"N2_SAVE:									 \n"

		"	SAVE7x2									 \n"

		"	bgt   	N2_BEGIN_M7						 \n"

		"END_N2M7: 									 \n"


		//------------------------------------------------------N2M6
		"	mov		x17, x16						 \n"   //B0*
		"	add		x18, x17, x4, lsl #3 			 \n"

		"	cmp		x23, #6							 \n"
		"	bne 	N2_BEGIN_M5 					 \n"


		"N2_BEGIN_M6:						 	     \n"

		"	mov		x9, x8 						  	 \n"
		"	add		x10, x9, x5, lsl #3 			 \n"
		"	add		x11, x10, x5, lsl #3 			 \n"
		"	add		x12, x11, x5, lsl #3 			 \n"
		"	add 	x13, x12, x5, lsl #3			 \n"
		"	add 	x14, x13, x5, lsl #3			 \n"

		"	prfm    PLDL1KEEP, [x9, #64]			 \n"
		"	prfm    PLDL1KEEP, [x10, #64]			 \n"
		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"
		"	prfm    PLDL1KEEP, [x13, #64]			 \n"
		"	prfm    PLDL1KEEP, [x14, #64]			 \n"


		"N2_M6_Body_K:								 \n"

		"	lsr		x22, x5, #2 					 \n"     // K / 8
		"	KERNEL6x2_BEGIN_K						 \n"
		"	subs	x22, x22, #1		     		 \n"	
		"	b 		N2_M6_K1_3						 \n"

		"N2_M6_Main_K:								 \n"
		
		"	KERNEL6x2_K0						 	 \n"
		"N2_M6_K1_3:								 \n"
		
		"	KERNEL6x2_K1							 \n"
		"	KERNEL6x2_K0							 \n"

		"	beq		N2_M6_Edge_K					 \n"

		"	KERNEL6x2_K1							 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		N2_M6_Main_K			 		 \n"	

		"N2_M6_Edge_K:								 \n"


		"	prfm	PLDL1KEEP, [x25, #64]			 \n"
		"	prfm	PLDL1KEEP, [x26, #64]			 \n"
		"	prfm	PLDL1KEEP, [x27, #64]			 \n"
		"	prfm	PLDL1KEEP, [x28, #64]			 \n"
		"	prfm	PLDL1KEEP, [x29, #64]			 \n"

		"	KERNEL6x2_END_K						 	 \n"	


		"	ands	x22, x5, #3					 	 \n"
		"	beq		N2M6_SAVE 						 \n"

		"N2M6_EDGE_K:								 \n"
		"	EDGE_K_KERNEL6x2 						 \n"
		"	subs 	x22, x22, #1					 \n"
		"	bgt 	N2M6_EDGE_K  					 \n"

		"N2M6_SAVE:									 \n"

		"	SAVE6x2									 \n"

		"	b 		N2_END_M 						 \n"



//-------------------------------------------------------


		"N2_BEGIN_M5:							    \n"


		"	cmp		x23, #5							 \n"
		"	bne 	N2_BEGIN_M2 					 \n"

		"	mov		x9, x8 						  	 \n"
		"	add		x10, x9, x5, lsl #3 			 \n"
		"	add		x11, x10, x5, lsl #3 			 \n"
		"	add		x12, x11, x5, lsl #3 			 \n"
		"	add 	x13, x12, x5, lsl #3			 \n"

		"	prfm    PLDL1KEEP, [x9, #64]			 \n"
		"	prfm    PLDL1KEEP, [x10, #64]			 \n"
		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"
		"	prfm    PLDL1KEEP, [x13, #64]			 \n"


		"N2_M5_Body_K:								 \n"

		"	lsr		x22, x5, #2 					 \n"     // K / 8
		"	KERNEL5x2_BEGIN_K						 \n"
		"	subs	x22, x22, #1		     		 \n"	
		"	b 		N2_M5_K1_3						 \n"

		"N2_M5_Main_K:								 \n"
		
		"	KERNEL5x2_K0						 	 \n"
		"N2_M5_K1_3:								 \n"
		
		"	KERNEL5x2_K1							 \n"
		"	KERNEL5x2_K0							 \n"

		"	beq		N2_M5_Edge_K					 \n"

		"	KERNEL5x2_K1							 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		N2_M5_Main_K			 		 \n"	

		"N2_M5_Edge_K:								 \n"


		"	prfm	PLDL1KEEP, [x25, #64]			 \n"
		"	prfm	PLDL1KEEP, [x26, #64]			 \n"
		"	prfm	PLDL1KEEP, [x27, #64]			 \n"
		"	prfm	PLDL1KEEP, [x28, #64]			 \n"
		"	prfm	PLDL1KEEP, [x29, #64]			 \n"

		"	KERNEL5x2_END_K						 	 \n"	


		"	ands	x22, x5, #3					 	 \n"
		"	beq		N2M5_SAVE 						 \n"

		"N2M5_EDGE_K:								 \n"
		"	EDGE_K_KERNEL5x4 						 \n"
		"	subs 	x22, x22, #1					 \n"
		"	bgt 	N2M5_EDGE_K  					 \n"

		"N2M5_SAVE:									 \n"

		"	SAVE5x2									 \n"

		"	b 		N2_END_M 						 \n"


//------------------------------------------------------M2
		"N2_BEGIN_M2:									 \n"

		"	cmp		x23, #2							 \n"
		"	bne 	N2_END_M 						 \n"

		"	mov		x9, x8 						  	 \n"
		"	add		x10, x9, x5, lsl #3 			 \n"

		"	prfm    PLDL1KEEP, [x9, #64]			 \n"
		"	prfm    PLDL1KEEP, [x10, #64]			 \n"


		"N2_M2_Body_K:								 \n"

		"	lsr		x22, x5, #2 					 \n"     // K / 8
		"	KERNEL2x2_BEGIN_K						 \n"
		"	subs	x22, x22, #1		     		 \n"	
		"	b 		N2_M2_K1_3						 \n"

		"N2_M2_Main_K:								 \n"
		
		"	KERNEL2x2_K0						 	 \n"
		"N2_M2_K1_3:								 \n"
		
		"	KERNEL2x2_K1							 \n"
		"	KERNEL2x2_K0							 \n"

		"	beq		N2_M2_Edge_K					 \n"

		"	KERNEL2x2_K1							 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		N2_M2_Main_K			 		 \n"	

		"N2_M2_Edge_K:								 \n"

		"	prfm	PLDL1KEEP, [x25, #64]			 \n"
		"	prfm	PLDL1KEEP, [x26, #64]			 \n"

		"	KERNEL2x2_END_K						 	 \n"	


		"	ands	x22, x5, #3					 	 \n"
		"	beq		N2M2_SAVE 						 \n"

		"N2M2_EDGE_K:								 \n"
		"	EDGE_K_KERNEL2x2 						 \n"
		"	subs 	x22, x22, #1					 \n"
		"	bgt 	N2M2_EDGE_K  					 \n"

		"N2M2_SAVE:									 \n"

		"	SAVE2x2									 \n"


		"N2_END_M:									 \n"

		"	add		x0, x0, #16						 \n"
		"	add		x16, x16, #16					 \n"

		"	subs	x20, x20, #2					 \n"
		"	beq		END_N 							 \n"
















//----------------------------------------------------------N1M4

		"BEGIN_N1:									 \n"

		"	lsr		x21, x3, #2						 \n"   // M /4

	  	"	cmp		x20, #1							 \n"
	  	"	blt     END_N 						 	 \n"

		"	mov 	x25, x0   						 \n"   //C1*
		"	add		x26, x25, x4, lsl #3 			 \n"   //C2*
		"	add 	x27, x26, x4, lsl #3 			 \n"   //C3*
		" 	add 	x28, x27, x4, lsl #3 			 \n"   //C4*

		"	mov		x8, x1   						 \n"
		"	cmp		x21, #1							 \n"
	  	"	blt		END_N1M4 						 \n"

		"N1_BEGIN_M4:								 \n"

		"	mov		x17, x16						 \n"   //B0*
		"	add		x18, x17, x4, lsl #3 			 \n"


		"	mov		x9, x8						 	 \n"   //A0*
		"	add		x10, x9, x5, lsl #3				 \n"   //A1*
		"	add		x11, x10, x5, lsl #3			 \n"   //A2*
		"	add		x12, x11, x5, lsl #3			 \n"   //A3*

		"	prfm    PLDL1KEEP, [x9, #64]			 \n"
		"	prfm    PLDL1KEEP, [x10, #64]			 \n"
		"	prfm    PLDL1KEEP, [x11, #64]			 \n"
		"	prfm    PLDL1KEEP, [x12, #64]			 \n"


		"N1_M4_Body_K:								 \n"

		"	lsr		x22, x5, #1						 \n"
		"	BEGIN_KERNEL4x1_K 						 \n"
		"	sub 	x22, x22, #1		     		 \n"

		"N1_Main_K:									 \n"

		"	KERNEL4x1_K 							 \n"
		"	subs 	x22, x22, #1					 \n"
		"	bgt 	N1_Main_K						 \n"

		"	ands	x22, x5, #1					 	 \n"
		"	beq		N1_SAVE 						 \n"

		"N1_EDGE_K:								 	 \n"
		"	EDGE_K_KERNEL4x1 						 \n"

		"N1_SAVE:									 \n"

		"	SAVE4x1									 \n"

		"	bgt   	N1_BEGIN_M4						 \n"

		"END_N1M4: 									 \n"

		"	ands 	x21, x3, #3						 \n"
		"	beq		END_N 							 \n"



//-----------------------------------------------------N1M1


		"BEGIN_N1M1:								 \n"

		"	mov		x17, x16						 \n"
		"	add		x18, x17, x4, lsl #3			 \n"


		"	mov		x9, x8 						  	 \n"
		"	prfm    PLDL1KEEP, [x9, #64]			 \n"

		"N1_M1_Body_K:								 \n"

		"	lsr		x22, x5, #1 					 \n"
		"	KERNEL1x1_BEGIN_K						 \n"
		"	sub 	x22, x22, #1		     		 \n"

		"N1M1_Main_K:								 \n"

		"	subs 	x22, x22, #1					 \n"
		"	KERNEL1x1_K 							 \n"
		"	bgt 	N1M1_Main_K						 \n"

		"	ands	x22, x5, #1					 	 \n"
		"	beq		N1M1_SAVE 						 \n"

		"N1M1_EDGE_K:								 \n"
		"	EDGE_K_KERNEL1x1 						 \n"

		"N1M1_SAVE:									 \n"

		"	SAVE1x1									 \n"

		"	bgt		BEGIN_N1M1	 					 \n"


	  	"END_N:										 \n"




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
