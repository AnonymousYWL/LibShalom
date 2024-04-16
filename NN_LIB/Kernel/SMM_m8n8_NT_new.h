#pragma once


void SMM_NT_KERNELMx4(float *C, float *A, float *B, long	M, long N, long K,
         long LN, long LK, float *SB, long k_tag)
{
	asm volatile(

	    ".macro	PACK_KERNEL8x4_BEGIN_K_Mx4							\n"


	    "	ldr		q0,	[x11], #16									\n"

	    "	ldr		q4, [x12], #16									\n"
	    "	ldr		q5, [x13], #16									\n"
	    "	fmul	v24.4s, v0.4s, v4.s[0]			 				\n"
	    "	ldr		q6, [x14], #16					 				\n"
	    "	fmul	v25.4s, v0.4s, v5.s[0]			 				\n"

	    "	ldr		q7, [x15], #16					 				\n"
	    "	fmul	v26.4s, v0.4s, v6.s[0]			 				\n"
	    "	ldr		q1, [x11], #16									\n"
	    "	fmul	v27.4s, v0.4s, v7.s[0]			 				\n"

	    "	ldp		q2, q3, [x11], #32				    			\n"
	    "	fmul	v28.4s, v1.4s, v4.s[0]							\n"
	    "	fmul	v29.4s, v1.4s, v5.s[0]							\n"
	    "	fmul	v30.4s, v1.4s, v6.s[0]							\n"
	    "	prfm	PLDL1KEEP, [x11, #256]							\n"
	    "	fmul	v31.4s, v1.4s, v7.s[0]							\n"

	    "	st4		{v4.s, v5.s, v6.s, v7.s}[0], [x24], #16			\n"
	    "	ldr		q8, [x12], #16									\n"

	    ".endm 										 				\n"


	    ".macro	PACK_KERNEL8x4_K0_Mx4								\n"

	    "	prfm	PLDL1KEEP, [x12, #128]							\n"
	    "	ldp		q2, q3, [x11], #32 								\n"
	    "	fmla	v24.4s, v0.4s, v4.s[0]			 				\n"
	    "	fmla	v25.4s, v0.4s, v5.s[0]			 				\n"
	    "	fmla	v26.4s, v0.4s, v6.s[0]			 				\n"
	    "	fmla	v27.4s, v0.4s, v7.s[0]			 				\n"

	    "	ldr		q8, [x12], #16									\n"

	    "	fmla	v28.4s, v1.4s, v4.s[0]							\n"
	    "	fmla	v29.4s, v1.4s, v5.s[0]							\n"
	    "	fmla	v30.4s, v1.4s, v6.s[0]							\n"
	    "	fmla	v31.4s, v1.4s, v7.s[0]							\n"
	    "	st4		{v4.s, v5.s, v6.s, v7.s}[0], [x24], #16			\n"

	    ".endm 														\n"


	    ".macro	PACK_KERNEL8x4_K1_Mx4								\n"

	    "	prfm	PLDL1KEEP, [x13, #128]							\n"
	    "	ldp		q0, q1, [x11], #32 								\n"
	    "	fmla	v24.4s, v2.4s, v4.s[1]			 				\n"
	    "	fmla	v25.4s, v2.4s, v5.s[1]			 				\n"
	    "	fmla	v26.4s, v2.4s, v6.s[1]			 				\n"
	    "	fmla	v27.4s, v2.4s, v7.s[1]			 				\n"

	    "	ldr		q9, [x13], #16									\n"

	    "	fmla	v28.4s, v3.4s, v4.s[1]							\n"
	    "	fmla	v29.4s, v3.4s, v5.s[1]							\n"
	    "	fmla	v30.4s, v3.4s, v6.s[1]							\n"
	    "	fmla	v31.4s, v3.4s, v7.s[1]							\n"
	    "	st4		{v4.s, v5.s, v6.s, v7.s}[1], [x24], #16			\n"

	    ".endm 														\n"

	    ".macro	PACK_KERNEL8x4_K2_Mx4								\n"

	    "	prfm	PLDL1KEEP, [x14, #128]							\n"
	    "	ldp		q2, q3, [x11], #32 								\n"
	    "	fmla	v24.4s, v0.4s, v4.s[2]			 				\n"
	    "	fmla	v25.4s, v0.4s, v5.s[2]			 				\n"
	    "	fmla	v26.4s, v0.4s, v6.s[2]			 				\n"
	    "	fmla	v27.4s, v0.4s, v7.s[2]			 				\n"

	    "	ldr		q10, [x14], #16									\n"

	    "	fmla	v28.4s, v1.4s, v4.s[2]							\n"
	    "	fmla	v29.4s, v1.4s, v5.s[2]							\n"
	    "	fmla	v30.4s, v1.4s, v6.s[2]							\n"
	    "	fmla	v31.4s, v1.4s, v7.s[2]							\n"
	    "	st4		{v4.s, v5.s, v6.s, v7.s}[2], [x24], #16			\n"

	    ".endm 														\n"


	    ".macro	PACK_KERNEL8x4_K3_Mx4								\n"

	    "	prfm	PLDL1KEEP, [x15, #128]							\n"
	    "	ldp		q0, q1, [x11], #32 								\n"
	    "	fmla	v24.4s, v2.4s, v4.s[3]			 				\n"
	    "	fmla	v25.4s, v2.4s, v5.s[3]			 				\n"
	    "	fmla	v26.4s, v2.4s, v6.s[3]			 				\n"
	    "	fmla	v27.4s, v2.4s, v7.s[3]			 				\n"

	    "	ldr		q11, [x15], #16									\n"

	    "	fmla	v28.4s, v3.4s, v4.s[3]							\n"
	    "	fmla	v29.4s, v3.4s, v5.s[3]							\n"
	    "	fmla	v30.4s, v3.4s, v6.s[3]							\n"
	    "	fmla	v31.4s, v3.4s, v7.s[3]							\n"
	    "	st4		{v4.s, v5.s, v6.s, v7.s}[3], [x24], #16			\n"

	    ".endm 														\n"



	    ".macro	PACK_KERNEL8x4_K4_Mx4								\n"

	    "	prfm	PLDL1KEEP, [x12, #128]							\n"
	    "	ldp		q2, q3, [x11], #32 								\n"
	    "	fmla	v24.4s, v0.4s, v8.s[0]			 				\n"
	    "	fmla	v25.4s, v0.4s, v9.s[0]			 				\n"
	    "	fmla	v26.4s, v0.4s, v10.s[0]			 				\n"
	    "	fmla	v27.4s, v0.4s, v11.s[0]			 				\n"

	    "	ldr		q4, [x12], #16									\n"

	    "	fmla	v28.4s, v1.4s, v8.s[0]							\n"
	    "	fmla	v29.4s, v1.4s, v9.s[0]							\n"
	    "	fmla	v30.4s, v1.4s, v10.s[0]							\n"
	    "	fmla	v31.4s, v1.4s, v11.s[0]							\n"
	    "	st4		{v8.s, v9.s, v10.s, v11.s}[0], [x24], #16		\n"

	    ".endm 														\n"



	    ".macro	PACK_KERNEL8x4_K5_Mx4								\n"

	    "	prfm	PLDL1KEEP, [x13, #128]							\n"
	    "	ldp		q0, q1, [x11], #32 								\n"
	    "	fmla	v24.4s, v2.4s, v8.s[1]			 				\n"
	    "	fmla	v25.4s, v2.4s, v9.s[1]			 				\n"
	    "	fmla	v26.4s, v2.4s, v10.s[1]			 				\n"
	    "	fmla	v27.4s, v2.4s, v11.s[1]			 				\n"

	    "	ldr		q5, [x13], #16									\n"

	    "	fmla	v28.4s, v3.4s, v8.s[1]							\n"
	    "	fmla	v29.4s, v3.4s, v9.s[1]							\n"
	    "	fmla	v30.4s, v3.4s, v10.s[1]							\n"
	    "	fmla	v31.4s, v3.4s, v11.s[1]							\n"

	    "	st4		{v8.s, v9.s, v10.s, v11.s}[1], [x24], #16		\n"

	    ".endm 														\n"





	    ".macro	PACK_KERNEL8x4_K6_Mx4								\n"

	    "	prfm	PLDL1KEEP, [x14, #128]							\n"
	    "	ldp		q2, q3, [x11], #32 								\n"
	    "	fmla	v24.4s, v0.4s, v8.s[2]			 				\n"
	    "	fmla	v25.4s, v0.4s, v9.s[2]			 				\n"
	    "	fmla	v26.4s, v0.4s, v10.s[2]			 				\n"
	    "	fmla	v27.4s, v0.4s, v11.s[2]			 				\n"

	    "	ldr		q6, [x14], #16									\n"

	    "	fmla	v28.4s, v1.4s, v8.s[2]							\n"
	    "	fmla	v29.4s, v1.4s, v9.s[2]							\n"
	    "	fmla	v30.4s, v1.4s, v10.s[2]							\n"
	    "	fmla	v31.4s, v1.4s, v11.s[2]							\n"
	    "	st4		{v8.s, v9.s, v10.s, v11.s}[2], [x24], #16		\n"

	    ".endm 														\n"



	    ".macro	PACK_KERNEL8x4_K7_Mx4								\n"

	    "	prfm	PLDL1KEEP, [x15, #128]							\n"
	    "	ldp		q0, q1, [x11], #32 								\n"
	    "	fmla	v24.4s, v2.4s, v8.s[3]			 				\n"
	    "	fmla	v25.4s, v2.4s, v9.s[3]			 				\n"
	    "	fmla	v26.4s, v2.4s, v10.s[3]			 				\n"
	    "	fmla	v27.4s, v2.4s, v11.s[3]			 				\n"

	    "	ldr		q7, [x15], #16									\n"

	    "	fmla	v28.4s, v3.4s, v8.s[3]							\n"
	    "	fmla	v29.4s, v3.4s, v9.s[3]							\n"
	    "	fmla	v30.4s, v3.4s, v10.s[3]							\n"
	    "	fmla	v31.4s, v3.4s, v11.s[3]							\n"
	   	"	st4		{v8.s, v9.s, v10.s, v11.s}[3], [x24], #16		\n"

	    ".endm 														\n"



	    ".macro	PACK_KERNEL8x4_END_K_Mx4							\n"

	    "	fmla	v24.4s, v2.4s, v8.s[3]			 				\n"
	    "	fmla	v25.4s, v2.4s, v9.s[3]			 				\n"
	    "	fmla	v26.4s, v2.4s, v10.s[3]			 				\n"
	    "	fmla	v27.4s, v2.4s, v11.s[3]			 				\n"

	    "	fmla	v28.4s, v3.4s, v8.s[3]							\n"
	    "	fmla	v29.4s, v3.4s, v9.s[3]							\n"
	    "	fmla	v30.4s, v3.4s, v10.s[3]							\n"
	    "	fmla	v31.4s, v3.4s, v11.s[3]							\n"
	   	"	st4		{v8.s, v9.s, v10.s, v11.s}[3], [x24], #16		\n"


	    ".endm 														\n"



	    ".macro	PACK_SAVE0_Mx4										\n"

	    " 	subs  x21, x21, #1                						\n"

	    "	st4		{v24.s, v25.s, v26.s, v27.s}[0], [x25]	    	\n"
	    "	add		x25, x25, x9, lsl #5							\n"
	    "	st4		{v24.s, v25.s, v26.s, v27.s}[1], [x26]	    	\n"
	    "	add		x26, x26, x9, lsl #5							\n"
	    "	st4		{v24.s, v25.s, v26.s, v27.s}[2], [x27]	    	\n"
	    "	add		x27, x27, x9, lsl #5							\n"
	    "	st4		{v24.s, v25.s, v26.s, v27.s}[3], [x28]	    	\n"
	    "	add		x28, x28, x9, lsl #5							\n"

	    "	st4		{v28.s, v29.s, v30.s, v31.s}[0], [x16]		    \n"
	    "	add		x16, x16, x9, lsl #5							\n"
	    "	st4		{v28.s, v29.s, v30.s, v31.s}[1], [x17]		    \n"
	    "	add		x17, x17, x9, lsl #5							\n"
	    "	st4		{v28.s, v29.s, v30.s, v31.s}[2], [x18]		    \n"
	    "	add		x18, x18, x9, lsl #5							\n"
	    "	st4		{v28.s, v29.s, v30.s, v31.s}[3], [x19]		    \n"
	    "	add		x19, x19, x9, lsl #5							\n"


	    ".endm	 													\n"



	    ".macro	PACK_SAVE_Mx4										\n"

	    "	sub 	sp, sp ,#128									\n"
	    "	mov 	x22, sp											\n"

	    "	prfm	PLDL1KEEP, [x25, #16]							\n"
	    "	prfm	PLDL1KEEP, [x26, #16]							\n"

	    "	st4		{v24.s, v25.s, v26.s, v27.s}[0], [sp], #16	  	\n"
	    "	st4		{v24.s, v25.s, v26.s, v27.s}[1], [sp], #16		\n"
	    "	st4		{v24.s, v25.s, v26.s, v27.s}[2], [sp], #16		\n"
	    "	st4		{v24.s, v25.s, v26.s, v27.s}[3], [sp], #16		\n"
	    "	st4		{v28.s, v29.s, v30.s, v31.s}[0], [sp], #16		\n"
	    "	st4		{v28.s, v29.s, v30.s, v31.s}[1], [sp], #16		\n"
	    "	st4		{v28.s, v29.s, v30.s, v31.s}[2], [sp], #16		\n"
	    "	st4		{v28.s, v29.s, v30.s, v31.s}[3], [sp], #16		\n"

	    " 	subs  x21, x21, #1                						\n"

	    "	ldp		q0, q1, [x22], #32								\n"
	    "	ldp		q2, q3, [x22], #32								\n"
	    "	ldp		q4, q5, [x22], #32								\n"
	    "	ldp		q6, q7, [x22], #32								\n"

	    "	prfm	PLDL1KEEP, [x27, #16]							\n"
	    "	prfm	PLDL1KEEP, [x28, #16]							\n"

	    "	ldr 	q16, [x25] 										\n"
	    "	fadd	v0.4s, v0.4s, v16.4s 							\n"
	    "	ldr		q17, [x26] 										\n"
	    "	fadd	v1.4s, v1.4s, v17.4s 							\n"
	    "	str 	q0, [x25] 										\n"
	    "	add 	x25, x25, x9, lsl #5 							\n"
	    "	ldr 	q18, [x27] 										\n"
	    "	fadd	v2.4s, v2.4s, v18.4s 							\n"
	    "	str 	q1, [x26] 										\n"
	    "	add 	x26, x26, x9, lsl #5 							\n"
	    "	prfm	PLDL2KEEP, [x25, #16]							\n"
	    "	ldr		q19, [x28] 										\n"
	    "	fadd	v3.4s, v3.4s, v19.4s 							\n"
	    "	str 	q2, [x27] 										\n"
	    "	add 	x27, x27, x9, lsl #5 							\n"
	    "	prfm	PLDL2KEEP, [x26, #16]							\n"

	    "	ldr 	q20, [x16] 										\n"
	    "	fadd	v4.4s, v4.4s, v20.4s 							\n"
	    "	str 	q3, [x28] 										\n"
	    "	add 	x28, x28, x9, lsl #5 							\n"
	    "	prfm	PLDL2KEEP, [x27, #16]							\n"
	    "	ldr		q21, [x17] 										\n"
	    "	fadd	v5.4s, v5.4s, v21.4s 							\n"
	    "	prfm	PLDL2KEEP, [x28, #16]							\n"
	    "	str 	q4, [x16] 										\n"
	    "	add 	x16, x16, x9, lsl #5 							\n"
	    "	ldr 	q22, [x18] 										\n"
	    "	fadd	v6.4s, v6.4s, v22.4s 							\n"
	    "	prfm	PLDL2KEEP, [x16, #16]							\n"
	    "	str 	q5, [x17] 										\n"
	    "	add 	x17, x17, x9, lsl #5 							\n"
	    "	ldr		q23, [x19] 										\n"
	    "	fadd	v7.4s, v7.4s, v23.4s 							\n"
	    "	prfm	PLDL2KEEP, [x17, #16]							\n"
	    "	str 	q6, [x18] 										\n"
	    "	add 	x18, x18, x9, lsl #5 							\n"
	    "	prfm	PLDL2KEEP, [x18, #16]							\n"
	    "	str 	q7, [x19] 										\n"
	    "	add 	x19, x19, x9, lsl #5 							\n"
	    "	prfm	PLDL2KEEP, [x19, #16]							\n"

	    ".endm	 													\n"



	    ".macro	KERNEL8x4_BEGIN_K_Mx4								\n"

	    "	prfm	PLDL1KEEP, [x11, #1024]							\n"
	    "	ldr		q0,	[x11], #16									\n"
	    "	ldr		q4, [x24], #16									\n"
	    "	fmul	v24.4s, v4.4s, v0.s[0]			 				\n"
	    "	fmul	v25.4s, v4.4s, v0.s[1]			 				\n"

	    "	ldr		q1,	[x11], #16									\n"
	    "	fmul	v26.4s, v4.4s, v0.s[2]			 				\n"
	    "	fmul	v27.4s, v4.4s, v0.s[3]			 				\n"

	    "	ldp		q2,	q3, [x11], #32								\n"
	    "	fmul	v28.4s, v4.4s, v1.s[0]			 				\n"
	    "	fmul	v29.4s, v4.4s, v1.s[1]			 				\n"

	    "	ldr		q5,	[x24], #16									\n"
	    "	fmul	v30.4s, v4.4s, v1.s[2]			 				\n"
	    "	fmul	v31.4s, v4.4s, v1.s[3]			 				\n"


	    ".endm 										 				\n"



	    ".macro	KERNEL8x4_K0_Mx4									\n"


	    "	prfm	PLDL1KEEP, [x11, #256] 							\n"
	  	"	ldp		q2,	q3, [x11], #32								\n"
	    "	fmla	v24.4s, v4.4s, v0.s[0]			 				\n"
	    "	fmla	v25.4s, v4.4s, v0.s[1]			 				\n"
	    
	    "	ldr		q5,	[x24], #16									\n"
	    "	fmla	v26.4s, v4.4s, v0.s[2]			 				\n"
	    "	fmla	v27.4s, v4.4s, v0.s[3]			 				\n"

	    "	fmla	v28.4s, v4.4s, v1.s[0]			 				\n"
	    "	fmla	v29.4s, v4.4s, v1.s[1]			 				\n"

	    "	fmla	v30.4s, v4.4s, v1.s[2]			 				\n"
	    "	fmla	v31.4s, v4.4s, v1.s[3]			 				\n"


	    ".endm 										 				\n"


	    ".macro	KERNEL8x4_K1_Mx4									\n"

	  	"	ldp		q0,	q1, [x11], #32								\n"
	    "	fmla	v24.4s, v5.4s, v2.s[0]			 				\n"
	    "	fmla	v25.4s, v5.4s, v2.s[1]			 				\n"
	    
	    "	ldr		q4,	[x24], #16									\n"
	    "	fmla	v26.4s, v5.4s, v2.s[2]			 				\n"
	    "	fmla	v27.4s, v5.4s, v2.s[3]			 				\n"

	    "	prfm	PLDL1KEEP, [x24, #512] 							\n"
	    "	fmla	v28.4s, v5.4s, v3.s[0]			 				\n"
	    "	fmla	v29.4s, v5.4s, v3.s[1]			 				\n"

	    "	fmla	v30.4s, v5.4s, v3.s[2]			 				\n"
	    "	fmla	v31.4s, v5.4s, v3.s[3]			 				\n"

	    ".endm 										 				\n"



	    ".macro	KERNEL8x4_END_K_Mx4									\n"


	    "	fmla	v24.4s, v5.4s, v2.s[0]			 				\n"
	    "	fmla	v25.4s, v5.4s, v2.s[1]			 				\n"  
	    "	fmla	v26.4s, v5.4s, v2.s[2]			 				\n"
	    "	fmla	v27.4s, v5.4s, v2.s[3]			 				\n"
	    "	fmla	v28.4s, v5.4s, v3.s[0]			 				\n"
	    "	fmla	v29.4s, v5.4s, v3.s[1]			 				\n"
	    "	fmla	v30.4s, v5.4s, v3.s[2]			 				\n"
	    "	fmla	v31.4s, v5.4s, v3.s[3]			 				\n"

	    ".endm 										 				\n"


	    ".macro	M8N4_ADD_C_Mx4										\n"

	    "	ldr		q0, [x25]										\n"
	    "	fadd	v24.4s, v24.4s, v0.4s							\n"
	    "	ldr		q1, [x26]										\n"
	    "	fadd	v25.4s, v25.4s, v1.4s							\n"
	    "	ldr		q2, [x27]										\n"
	    "	fadd	v26.4s, v26.4s, v2.4s							\n"
	    "	ldr		q3, [x28]										\n"
	    "	fadd	v27.4s, v27.4s, v3.4s							\n"

	    "	ldr		q4, [x16]										\n"
	    "	ldr		q5, [x17]										\n"
	    "	fadd	v28.4s, v28.4s, v4.4s							\n"
	   	"	ldr		q6, [x18]										\n"
	    "	ldr		q7, [x19]										\n"
	    "	fadd	v29.4s, v29.4s, v5.4s							\n"
	    "	fadd	v30.4s, v30.4s, v6.4s							\n"
	    "	fadd	v31.4s, v31.4s, v7.4s							\n"

	    ".endm														\n"


	    ".macro	SAVE8x4_Mx4											\n"

	    "	subs	x21, x21, #1					 				\n"

	    "	str		q24, [x25]			 		 					\n"
	    "	add 	x25, x25, x9, lsl #5							\n"
	    "	prfm	PLDL2KEEP, [x25, #16]							\n"
	    "	str		q25, [x26]			 	     					\n"
	    "	add 	x26, x26, x9, lsl #5							\n"
	    "	prfm	PLDL2KEEP, [x26, #16]							\n"
	    "	str		q26, [x27]					 					\n"
	    "	add 	x27, x27, x9, lsl #5							\n"
	    "	prfm	PLDL2KEEP, [x27, #16]							\n"
	    "	str		q27, [x28]					 					\n"
	    "	add 	x28, x28, x9, lsl #5							\n"
	    "	prfm	PLDL2KEEP, [x28, #16]							\n"

	    "	str		q28, [x16]			 		 					\n"
	    "	add 	x16, x16, x9, lsl #5							\n"
	    "	prfm	PLDL2KEEP, [x16, #16]							\n"
	    "	str		q29, [x17]			 	     					\n"
	    "	add 	x17, x17, x9, lsl #5							\n"
	    "	prfm	PLDL2KEEP, [x17, #16]							\n"
	    "	str		q30, [x18]					 					\n"
	    "	add 	x18, x18, x9, lsl #5							\n"
	    "	prfm	PLDL2KEEP, [x18, #16]							\n"
	    "	str		q31, [x19]					 					\n"
	    "	add 	x19, x19, x9, lsl #5							\n"
	    "	prfm	PLDL2KEEP, [x19, #16]							\n"

	    ".endm										 				\n"



	    //----------------------------------------------------

	    "SMM_NT_KERNELMx4:									 	 	\n"

	    "	ldr		x0, %[C]						 				\n"
	    "	ldr		x1, %[A]						 				\n"
	    "	ldr		x2, %[B]						 				\n"

	    "	ldr		x3, %[M]						 				\n"
	    "	ldr		x4, %[N]						 				\n"
	    "	ldr		x5, %[K]										\n"
	    "	ldr		x9, %[LN] 						 				\n"
	    "	ldr		x6, %[LK]						 				\n"

	    "	prfm    PLDL1KEEP, [x1, #512]           				\n"
	    "	prfm    PLDL1KEEP, [x2, #128]           				\n"

	    "	ldr		x10, %[SB]						 				\n"
	    "	ldr		x8, %[k_tag] 					 				\n"


	    "	mov 	x25, x0   						 				\n"   //C1*
	    "	add		x26, x25, x9, lsl #2 			 				\n"   //C2*
	    "	add 	x27, x25, x9, lsl #3 			 				\n"   //C3*
	    " 	add 	x28, x26, x9, lsl #3 			 				\n"   //C4*

	    "	add 	x16, x27, x9, lsl #3 							\n"	 //C5*
	  	"	add 	x17, x28, x9, lsl #3 							\n"	 //C6*
	  	"	add 	x18, x16, x9, lsl #3 							\n"	 //C7*
	  	"	add 	x19, x17, x9, lsl #3 							\n"	 //C8*

	    "	mov		x11, x1   						 				\n"   // address_A
	    "	lsr		x21, x3, #3						 				\n"

	    "BEGIN_PACKB_Mx4:								 			\n"

	    "	mov		x24, x10						 				\n"   //还原SB的地址

	    "	mov		x12, x2						 	 				\n"   //B0*
	    "	add		x13, x12, x6, lsl #2 			 				\n"
	    "	add		x14, x12, x6, lsl #3 			 				\n"
	    "	add		x15, x13, x6, lsl #3 			 				\n"

	    "	prfm    PLDL1KEEP, [x12, #64]			 				\n"
	    "	prfm    PLDL1KEEP, [x13, #64]			 				\n"
	    "	prfm    PLDL1KEEP, [x14, #64]			 				\n"
	    "	prfm    PLDL1KEEP, [x15, #64]			 				\n"

	    "	prfm    PLDL1KEEP, [x11, #256]			 				\n"

	    "PACK_Body_K_Mx4:								 			\n"

	    "	lsr		x22, x5, #3 					 				\n"     // K / 8

	    "	PACK_KERNEL8x4_BEGIN_K_Mx4				 	 			\n"

	    "	subs	x22, x22, #1		     		 				\n"
	    "	b 		PACK_K1_7_Mx4						 			\n"

	    "PACK_K_Mx4:									 			\n"

	    "	PACK_KERNEL8x4_K0_Mx4						 			\n"
	    "PACK_K1_7_Mx4:									 			\n"

	    "	PACK_KERNEL8x4_K1_Mx4						 			\n"
	    "	PACK_KERNEL8x4_K2_Mx4						 			\n"
	    "	PACK_KERNEL8x4_K3_Mx4						 			\n"
	    "	PACK_KERNEL8x4_K4_Mx4						 			\n"
	    "	PACK_KERNEL8x4_K5_Mx4						 			\n"
	    "	PACK_KERNEL8x4_K6_Mx4						 			\n"

	    "	beq		PACK_Edge_K_Mx4						 			\n"

	    "	PACK_KERNEL8x4_K7_Mx4						 			\n"

	    "	subs	x22, x22, #1			 		 				\n"
	    "	b 		PACK_K_Mx4			 				 			\n"

	    "PACK_Edge_K_Mx4:							     			\n"


	    "	PACK_KERNEL8x4_END_K_Mx4			         			\n"


	    "	cmp		x8, #0							 				\n"
	    "	beq		M8N4_PACK_SAVE_Mx4 					 			\n"
	    "	PACK_SAVE_Mx4 											\n"
	    "	b 	END_PACKB_Mx4						 	 			\n"
	    "M8N4_PACK_SAVE_Mx4: 							 			\n"
	    "	PACK_SAVE0_Mx4								 			\n"

	    "END_PACKB_Mx4:									 			\n"

	    "	beq		M8_END_Mx4							 			\n"


//---------------------------------------------------------------------------
	    "BEGIN_M8_Mx4:                          					\n"

	    " mov     x24, x10                    						\n"
	    " prfm    PLDL1KEEP, [x24, #256]    						\n"
	    " prfm    PLDL1KEEP, [x11, #256]    						\n"

	    "Body_K_Mx4:                            					\n"

	    " lsr   x22, x5, #2                 						\n"     // K / 4
	    " KERNEL8x4_BEGIN_K_Mx4                		 				\n"
	    " subs  x22, x22, #1                						\n"
	    " b 	K1_3_Mx4                            				\n"

	    "Main_K_Mx4:                            					\n"

	    " KERNEL8x4_K0_Mx4                     		 				\n"
	    "K1_3_Mx4:                              					\n"

	    " KERNEL8x4_K1_Mx4                     		 				\n"
	    " KERNEL8x4_K0_Mx4                     		 				\n"

	    " beq   Edge_K_Mx4                      					\n"

	    " KERNEL8x4_K1_Mx4                     		 				\n"

	    " subs  x22, x22, #1                						\n"
	    " b     Main_K_Mx4                      					\n"

	    "Edge_K_Mx4:                            					\n"

	    " KERNEL8x4_END_K_Mx4                 						\n"

	    "	cmp		x8, #0							 				\n"
	    "	beq		M8N4_SAVE_Mx4 					 	 			\n"
	    "	M8N4_ADD_C_Mx4 							 	 			\n"
	    "M8N4_SAVE_Mx4: 							     			\n"
	    "	SAVE8x4_Mx4								 	 			\n"

	    " bgt     BEGIN_M8_Mx4                						\n"
	    "M8_END_Mx4:									 			\n"

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
	    : "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
	    "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16",
	    "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24", "x25",
	    "x26", "x27", "x28", "x29", "x30",
	    "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
	    "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
	    "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
	    "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"
	);

}




//------------------------------------------------------------------------------------
void SMM_NT_KERNELMx8(float *C, float *A, float *B, long	M, long N, long K,
         long LN, long LK, float *SB, long k_tag)
{
	asm volatile(

	    ".macro	PACK_KERNEL8x8_BEGIN_K_Mx8							\n"


	    "	ldr		q0,	[x11], #16									\n"

	    "	ldr		q4, [x12], #16									\n"
	    "	ldr		q5, [x13], #16									\n"

	    "	fmul	v16.4s, v0.4s, v4.s[0]			 				\n"
	    "	ldr		q6, [x14], #16					 				\n"
	    "	fmul	v17.4s, v0.4s, v5.s[0]			 				\n"

	    "	ldr		q7, [x15], #16					 				\n"
	    "	fmul	v18.4s, v0.4s, v6.s[0]			 				\n"
	    "	ldr		q8, [x16], #16					 				\n"
	    "	fmul	v19.4s, v0.4s, v7.s[0]			 				\n"

	    "	ldr		q9, [x17], #16									\n"
	    "	fmul	v20.4s, v0.4s, v8.s[0]							\n"
	    "	ldr		q10, [x18], #16									\n"
	    "	fmul	v21.4s, v0.4s, v9.s[0]							\n"
	    "	ldr		q11, [x19], #16									\n"
	    "	fmul	v22.4s, v0.4s, v10.s[0]							\n"

	    "	ldr		q1, [x11], #16									\n"
	    "	prfm	PLDL1KEEP, [x11, #256]							\n"
	    "	fmul	v23.4s, v0.4s, v11.s[0]							\n"

	    "	ldp		q2, q3, [x11], #32				    			\n"

	    "	fmul	v24.4s, v1.4s, v4.s[0]			 				\n"
	    "	fmul	v25.4s, v1.4s, v5.s[0]			 				\n"
	    "	fmul	v26.4s, v1.4s, v6.s[0]			 				\n"
	    "	fmul	v27.4s, v1.4s, v7.s[0]			 				\n"

	    "	st4		{v4.s, v5.s, v6.s, v7.s}[0], [x24], #16			\n"

	    "	fmul	v28.4s, v1.4s, v8.s[0]							\n"
	    "	fmul	v29.4s, v1.4s, v9.s[0]							\n"
	    "	fmul	v30.4s, v1.4s, v10.s[0]							\n"
	    "	fmul	v31.4s, v1.4s, v11.s[0]							\n"

	    "	st4		{v8.s, v9.s, v10.s, v11.s}[0], [x24], #16		\n"

	    "	ldr		q12, [x12], #16									\n"


	    ".endm 										 				\n"



	    ".macro	PACK_KERNEL8x8_K0_Mx8								\n"

	    "	prfm	PLDL1KEEP, [x12, #64]							\n"

	    "	ldr		q2, [x11], #16									\n"
	    "	fmla	v16.4s, v0.4s, v4.s[0]			 				\n"
	    "	fmla	v17.4s, v0.4s, v5.s[0]			 				\n"
	    "	fmla	v18.4s, v0.4s, v6.s[0]			 				\n"
	    "	fmla	v19.4s, v0.4s, v7.s[0]			 				\n"

	    "	st4		{v4.s, v5.s, v6.s, v7.s}[0], [x24], #16			\n"

	    "	fmla	v20.4s, v0.4s, v8.s[0]							\n"
	    "	fmla	v21.4s, v0.4s, v9.s[0]							\n"
	    "	fmla	v22.4s, v0.4s, v10.s[0]							\n"
	    "	fmla	v23.4s, v0.4s, v11.s[0]							\n"

	    "	prfm	PLDL1KEEP, [x13, #64]							\n"


	    "	ldr		q3, [x11], #16									\n"
	    "	fmla	v24.4s, v1.4s, v4.s[0]			 				\n"
	    "	fmla	v25.4s, v1.4s, v5.s[0]			 				\n"
	    "	fmla	v26.4s, v1.4s, v6.s[0]			 				\n"
	    "	fmla	v27.4s, v1.4s, v7.s[0]			 				\n"

	    "	st4		{v8.s, v9.s, v10.s, v11.s}[0], [x24], #16		\n"

	    "	fmla	v28.4s, v1.4s, v8.s[0]							\n"
	    "	fmla	v29.4s, v1.4s, v9.s[0]							\n"
	    "	fmla	v30.4s, v1.4s, v10.s[0]							\n"
	    "	fmla	v31.4s, v1.4s, v11.s[0]							\n"

	    "	prfm	PLDL1KEEP, [x11, #512]							\n"

	    "	ldr		q12, [x12], #16									\n"


	    ".endm 														\n"



	    ".macro	PACK_KERNEL8x8_K1_Mx8								\n"


	    "	prfm	PLDL1KEEP, [x14, #128]							\n"

	    "	ldr		q0, [x11], #16									\n"
	    "	fmla	v16.4s, v2.4s, v4.s[1]			 				\n"
	    "	fmla	v17.4s, v2.4s, v5.s[1]			 				\n"
	    "	fmla	v18.4s, v2.4s, v6.s[1]			 				\n"
	    "	fmla	v19.4s, v2.4s, v7.s[1]			 				\n"

	    "	st4		{v4.s, v5.s, v6.s, v7.s}[1], [x24], #16			\n"

	    "	fmla	v20.4s, v2.4s, v8.s[1]							\n"
	    "	fmla	v21.4s, v2.4s, v9.s[1]							\n"
	    "	fmla	v22.4s, v2.4s, v10.s[1]							\n"
	    "	fmla	v23.4s, v2.4s, v11.s[1]							\n"

	    "	prfm	PLDL1KEEP, [x15, #64]							\n"


	    "	ldr		q1, [x11], #16									\n"
	    "	fmla	v24.4s, v3.4s, v4.s[1]			 				\n"
	    "	fmla	v25.4s, v3.4s, v5.s[1]			 				\n"
	    "	fmla	v26.4s, v3.4s, v6.s[1]			 				\n"
	    "	fmla	v27.4s, v3.4s, v7.s[1]			 				\n"

	    "	st4		{v8.s, v9.s, v10.s, v11.s}[1], [x24], #16		\n"

	    "	fmla	v28.4s, v3.4s, v8.s[1]							\n"
	    "	fmla	v29.4s, v3.4s, v9.s[1]							\n"
	    "	fmla	v30.4s, v3.4s, v10.s[1]							\n"
	    "	fmla	v31.4s, v3.4s, v11.s[1]							\n"

	    "	ldr		q13, [x13], #16									\n"

	    ".endm 														\n"





	    ".macro	PACK_KERNEL8x8_K2_Mx8								\n"

	    "	prfm	PLDL1KEEP, [x16, #128]							\n"

	    "	ldr		q2, [x11], #16									\n"
	    "	fmla	v16.4s, v0.4s, v4.s[2]			 				\n"
	    "	fmla	v17.4s, v0.4s, v5.s[2]			 				\n"
	    "	fmla	v18.4s, v0.4s, v6.s[2]			 				\n"
	    "	fmla	v19.4s, v0.4s, v7.s[2]			 				\n"

	    "	st4		{v4.s, v5.s, v6.s, v7.s}[2], [x24], #16			\n"

	    "	fmla	v20.4s, v0.4s, v8.s[2]							\n"
	    "	fmla	v21.4s, v0.4s, v9.s[2]							\n"

	    "	ldr		q14, [x14], #16									\n"

	    "	fmla	v22.4s, v0.4s, v10.s[2]							\n"
	    "	fmla	v23.4s, v0.4s, v11.s[2]							\n"


	    "	prfm	PLDL1KEEP, [x17, #64]							\n"


	    "	ldr		q3, [x11], #16									\n"
	    "	fmla	v24.4s, v1.4s, v4.s[2]			 				\n"
	    "	fmla	v25.4s, v1.4s, v5.s[2]			 				\n"

	    "	ldr		q15, [x15], #16									\n"

	    "	fmla	v26.4s, v1.4s, v6.s[2]			 				\n"
	    "	fmla	v27.4s, v1.4s, v7.s[2]			 				\n"

	    "	st4		{v8.s, v9.s, v10.s, v11.s}[2], [x24], #16		\n"

	    "	fmla	v28.4s, v1.4s, v8.s[2]							\n"
	    "	fmla	v29.4s, v1.4s, v9.s[2]							\n"
	    "	fmla	v30.4s, v1.4s, v10.s[2]							\n"
	    "	fmla	v31.4s, v1.4s, v11.s[2]							\n"


	    ".endm 														\n"



	    ".macro	PACK_KERNEL8x8_K3_Mx8								\n"


	    "	prfm	PLDL1KEEP, [x18, #128]							\n"

	    "	st4		{v4.s, v5.s, v6.s, v7.s}[3], [x24], #16			\n"

	    "	ldr		q0, [x11], #16									\n"
	    "	fmla	v16.4s, v2.4s, v4.s[3]			 				\n"
	    "	fmla	v24.4s, v3.4s, v4.s[3]			 				\n"
	    "	ldr		q4, [x16], #16									\n"

	    "	prfm	PLDL1KEEP, [x19, #64]							\n"

	    "	fmla	v17.4s, v2.4s, v5.s[3]			 				\n"
	    "	fmla	v25.4s, v3.4s, v5.s[3]			 				\n"
	    "	ldr		q5, [x17], #16									\n"

	    "	fmla	v18.4s, v2.4s, v6.s[3]			 				\n"
	    "	fmla	v26.4s, v3.4s, v6.s[3]			 				\n"
	    "	ldr		q6, [x18], #16									\n"

	    "	fmla	v19.4s, v2.4s, v7.s[3]			 				\n"
	    "	fmla	v27.4s, v3.4s, v7.s[3]			 				\n"
	    "	ldr		q7, [x19], #16									\n"

	    "	fmla	v20.4s, v2.4s, v8.s[3]							\n"
	    "	fmla	v21.4s, v2.4s, v9.s[3]							\n"
	    "	fmla	v22.4s, v2.4s, v10.s[3]							\n"
	    "	fmla	v23.4s, v2.4s, v11.s[3]							\n"

	    "	ldr		q1, [x11], #16									\n"

	    "	fmla	v28.4s, v3.4s, v8.s[3]							\n"
	    "	fmla	v29.4s, v3.4s, v9.s[3]							\n"
	    "	fmla	v30.4s, v3.4s, v10.s[3]							\n"
	    "	fmla	v31.4s, v3.4s, v11.s[3]							\n"

	    "	st4		{v8.s, v9.s, v10.s, v11.s}[3], [x24], #16		\n"

	    ".endm 														\n"



	    ".macro	PACK_KERNEL8x8_K4_Mx8								\n"


	    "	prfm	PLDL1KEEP, [x12, #128]							\n"

	    "	ldr		q2, [x11], #16									\n"
	    "	fmla	v16.4s, v0.4s, v12.s[0]							\n"
	    "	fmla	v17.4s, v0.4s, v13.s[0]							\n"

	    "	st4		{v12.s, v13.s, v14.s, v15.s}[0], [x24], #16		\n"

	    "	fmla	v18.4s, v0.4s, v14.s[0]							\n"
	    "	fmla	v19.4s, v0.4s, v15.s[0]							\n"

	    "	ldr		q8, [x16], #16									\n"

	    "	fmla	v20.4s, v0.4s, v4.s[0]							\n"
	    "	fmla	v21.4s, v0.4s, v5.s[0]							\n"
	    "	fmla	v22.4s, v0.4s, v6.s[0]							\n"
	    "	fmla	v23.4s, v0.4s, v7.s[0]							\n"

	    "	ldr		q3, [x11], #16									\n"
	    "	prfm	PLDL1KEEP, [x13, #128]							\n"

	    "	fmla	v24.4s, v1.4s, v12.s[0]							\n"
	    "	fmla	v25.4s, v1.4s, v13.s[0]							\n"
	    "	fmla	v26.4s, v1.4s, v14.s[0]							\n"
	    "	fmla	v27.4s, v1.4s, v15.s[0]							\n"

	    "	ldr		q9, [x17], #16									\n"

	    "	fmla	v28.4s, v1.4s, v4.s[0]							\n"
	    "	fmla	v29.4s, v1.4s, v5.s[0]							\n"
	    "	fmla	v30.4s, v1.4s, v6.s[0]							\n"
	    "	fmla	v31.4s, v1.4s, v7.s[0]							\n"


	    "	st4		{v4.s, v5.s, v6.s, v7.s}[0], [x24], #16			\n"
	    "	prfm	PLDL1KEEP, [x11, #512]							\n"


	    ".endm 														\n"




	    ".macro	PACK_KERNEL8x8_K5_Mx8								\n"


	    "	prfm	PLDL1KEEP, [x14, #128]							\n"

	    "	ldr		q0, [x11], #16									\n"
	    "	fmla	v16.4s, v2.4s, v12.s[1]							\n"
	    "	fmla	v17.4s, v2.4s, v13.s[1]							\n"

	    "	st4		{v12.s, v13.s, v14.s, v15.s}[1], [x24], #16		\n"

	    "	fmla	v18.4s, v2.4s, v14.s[1]							\n"
	    "	fmla	v19.4s, v2.4s, v15.s[1]							\n"

	    "	ldr		q10, [x18], #16									\n"

	    "	fmla	v20.4s, v2.4s, v4.s[1]							\n"
	    "	fmla	v21.4s, v2.4s, v5.s[1]							\n"
	    "	fmla	v22.4s, v2.4s, v6.s[1]							\n"
	    "	fmla	v23.4s, v2.4s, v7.s[1]							\n"

	    "	ldr		q1, [x11], #16									\n"
	    "	prfm	PLDL1KEEP, [x15, #128]							\n"

	    "	fmla	v24.4s, v3.4s, v12.s[1]							\n"
	    "	fmla	v25.4s, v3.4s, v13.s[1]							\n"
	    "	fmla	v26.4s, v3.4s, v14.s[1]							\n"
	    "	fmla	v27.4s, v3.4s, v15.s[1]							\n"

	    "	fmla	v28.4s, v3.4s, v4.s[1]							\n"
	    "	fmla	v29.4s, v3.4s, v5.s[1]							\n"
	    "	fmla	v30.4s, v3.4s, v6.s[1]							\n"
	    "	fmla	v31.4s, v3.4s, v7.s[1]							\n"

	    "	st4		{v4.s, v5.s, v6.s, v7.s}[1], [x24], #16			\n"


	    ".endm 														\n"




	    ".macro	PACK_KERNEL8x8_K6_Mx8								\n"


	    "	prfm	PLDL1KEEP, [x16, #128]							\n"

	    "	ldr		q2, [x11], #16									\n"
	    "	fmla	v16.4s, v0.4s, v12.s[2]							\n"
	    "	fmla	v17.4s, v0.4s, v13.s[2]							\n"

	    "	st4		{v12.s, v13.s, v14.s, v15.s}[2], [x24], #16		\n"

	    "	fmla	v18.4s, v0.4s, v14.s[2]							\n"
	    "	fmla	v19.4s, v0.4s, v15.s[2]							\n"

	    "	ldr		q11, [x19], #16									\n"

	    "	fmla	v20.4s, v0.4s, v4.s[2]							\n"
	    "	fmla	v21.4s, v0.4s, v5.s[2]							\n"
	    "	fmla	v22.4s, v0.4s, v6.s[2]							\n"
	    "	fmla	v23.4s, v0.4s, v7.s[2]							\n"

	    "	ldr		q3, [x11], #16									\n"
	    "	prfm	PLDL1KEEP, [x17, #128]							\n"

	    "	st4		{v4.s, v5.s, v6.s, v7.s}[2], [x24], #16			\n"

	    "	fmla	v24.4s, v1.4s, v12.s[2]							\n"
	    "	fmla	v25.4s, v1.4s, v13.s[2]							\n"
	    "	fmla	v26.4s, v1.4s, v14.s[2]							\n"
	    "	fmla	v27.4s, v1.4s, v15.s[2]							\n"

	    "	fmla	v28.4s, v1.4s, v4.s[2]							\n"
	    "	fmla	v29.4s, v1.4s, v5.s[2]							\n"
	    "	fmla	v30.4s, v1.4s, v6.s[2]							\n"
	    "	fmla	v31.4s, v1.4s, v7.s[2]							\n"


	    ".endm 														\n"



	    ".macro	PACK_KERNEL8x8_K7_Mx8								\n"


	    "	prfm	PLDL1KEEP, [x18, #64]							\n"

	    "	add 	x24, x24, #16									\n"
	    "	ldr		q0, [x11], #16									\n"
	    "	fmla	v20.4s, v2.4s, v4.s[3]							\n"
	    "	fmla	v28.4s, v3.4s, v4.s[3]							\n"
	    "	st4		{v4.s, v5.s, v6.s, v7.s}[3], [x24]				\n"
	    "	ldr		q4, [x12], #16									\n"

	    "	fmla	v29.4s, v3.4s, v5.s[3]							\n"
	    "	fmla	v21.4s, v2.4s, v5.s[3]							\n"
	    "	ldr		q5, [x13], #16									\n"

	    "	fmla	v22.4s, v2.4s, v6.s[3]							\n"
	    "	fmla	v30.4s, v3.4s, v6.s[3]							\n"
	    "	ldr		q6, [x14], #16									\n"

	    "	fmla	v23.4s, v2.4s, v7.s[3]							\n"
	    "	fmla	v31.4s, v3.4s, v7.s[3]							\n"
	    "	ldr		q7, [x15], #16									\n"

	    "	sub 	x24, x24, #16									\n"

	    "	ldr		q1, [x11], #16									\n"
	    "	prfm	PLDL1KEEP, [x19, #64]							\n"

	    "	fmla	v16.4s, v2.4s, v12.s[3]							\n"
	    "	fmla	v17.4s, v2.4s, v13.s[3]							\n"
	    "	fmla	v18.4s, v2.4s, v14.s[3]							\n"
	    "	fmla	v19.4s, v2.4s, v15.s[3]							\n"

	    "	st4		{v12.s, v13.s, v14.s, v15.s}[3], [x24]			\n"

	    "	fmla	v24.4s, v3.4s, v12.s[3]							\n"
	    "	fmla	v25.4s, v3.4s, v13.s[3]							\n"
	    "	fmla	v26.4s, v3.4s, v14.s[3]							\n"
	    "	fmla	v27.4s, v3.4s, v15.s[3]							\n"

	    "	add 	x24, x24, #32									\n"

	    ".endm 														\n"



	    ".macro	PACK_KERNEL8x8_END_K_Mx8							\n"


	    "	prfm	PLDL1KEEP, [x25, #32]			 				\n"

	    "	fmla	v20.4s, v2.4s, v4.s[3]							\n"
	    "	fmla	v28.4s, v3.4s, v4.s[3]							\n"
	    "	fmla	v21.4s, v2.4s, v5.s[3]							\n"
	    "	fmla	v29.4s, v3.4s, v5.s[3]							\n"

	    "	st4		{v12.s, v13.s, v14.s, v15.s}[3], [x24], #16		\n"

	    "	prfm	PLDL1KEEP, [x26, #32]			 				\n"

	    "	fmla	v22.4s, v2.4s, v6.s[3]							\n"
	    "	fmla	v30.4s, v3.4s, v6.s[3]							\n"
	    "	fmla	v23.4s, v2.4s, v7.s[3]							\n"
	    "	fmla	v31.4s, v3.4s, v7.s[3]							\n"

	    "	fmla	v16.4s, v2.4s, v12.s[3]							\n"
	    "	fmla	v17.4s, v2.4s, v13.s[3]							\n"
	    "	fmla	v18.4s, v2.4s, v14.s[3]							\n"
	    "	fmla	v19.4s, v2.4s, v15.s[3]							\n"

	    "	st4		{v4.s, v5.s, v6.s, v7.s}[3], [x24], #16			\n"
	    "	prfm	PLDL1KEEP, [x27, #32]			 				\n"
	    "	fmla	v24.4s, v3.4s, v12.s[3]							\n"
	    "	fmla	v25.4s, v3.4s, v13.s[3]							\n"
	    "	fmla	v26.4s, v3.4s, v14.s[3]							\n"
	    "	fmla	v27.4s, v3.4s, v15.s[3]							\n"


	    "	prfm	PLDL1KEEP, [x28, #32]			    			\n"



	    ".endm 														\n"



	    ".macro	PACK_SAVE0_Mx8										\n"

	    " 	subs  x21, x21, #1                						\n"

	    "	st4		{v16.s, v17.s, v18.s, v19.s}[0], [x25], #16	    \n"
	    "	st4		{v20.s, v21.s, v22.s, v23.s}[0], [x25]			\n"

	    "	add 	x2, x2, x6, lsl #5								\n"

	    "	st4		{v16.s, v17.s, v18.s, v19.s}[1], [x26], #16		\n"
	    "	st4		{v20.s, v21.s, v22.s, v23.s}[1], [x26]		    \n"
	    "	sub 	x25, x25, #16									\n"
	    "	st4		{v16.s, v17.s, v18.s, v19.s}[2], [x27], #16		\n"
	    "	st4		{v20.s, v21.s, v22.s, v23.s}[2], [x27]		    \n"
	    "	sub 	x26, x26, #16									\n"
	    "	st4		{v16.s, v17.s, v18.s, v19.s}[3], [x28], #16	  	\n"
	    "	st4		{v20.s, v21.s, v22.s, v23.s}[3], [x28]		    \n"
	    "	sub 	x27, x27, #16									\n"
	    "	sub 	x28, x28, #16									\n"

	    "	add		x25, x25, x9, lsl #4							\n"
	    "	add		x26, x26, x9, lsl #4							\n"
	    "	add		x27, x27, x9, lsl #4							\n"
	    "	add		x28, x28, x9, lsl #4							\n"

	    "	st4		{v24.s, v25.s, v26.s, v27.s}[0], [x25], #16		\n"
	    "	st4		{v28.s, v29.s, v30.s, v31.s}[0], [x25]		    \n"
	    "	st4		{v24.s, v25.s, v26.s, v27.s}[1], [x26], #16		\n"
	    "	st4		{v28.s, v29.s, v30.s, v31.s}[1], [x26]	      	\n"
	    "	sub 	x25, x25, #16									\n"
	    "	st4		{v24.s, v25.s, v26.s, v27.s}[2], [x27], #16		\n"
	    "	st4		{v28.s, v29.s, v30.s, v31.s}[2], [x27]		    \n"
	    "	sub 	x26, x26, #16									\n"
	    "	st4		{v24.s, v25.s, v26.s, v27.s}[3], [x28], #16		\n"
	    "	st4		{v28.s, v29.s, v30.s, v31.s}[3], [x28]			\n"
	    "	sub 	x27, x27, #16									\n"
	    "	sub 	x28, x28, #16									\n"

	    "	add		x25, x25, x9, lsl #4							\n"
	    "	add		x26, x26, x9, lsl #4							\n"
	    "	add		x27, x27, x9, lsl #4							\n"
	    "	add		x28, x28, x9, lsl #4							\n"


	    ".endm	 													\n"



	    ".macro	PACK_SAVE_Mx8										\n"

	    "	sub 	sp, sp ,#256									\n"
	    "	mov 	x22, sp											\n"

	    "	prfm	PLDL1KEEP, [x25, #64]							\n"
	    "	prfm	PLDL1KEEP, [x26, #64]							\n"

	    "	st4		{v16.s, v17.s, v18.s, v19.s}[0], [sp], #16	  	\n"
	    "	st4		{v20.s, v21.s, v22.s, v23.s}[0], [sp], #16		\n"
	    "	st4		{v16.s, v17.s, v18.s, v19.s}[1], [sp], #16		\n"
	    "	st4		{v20.s, v21.s, v22.s, v23.s}[1], [sp], #16		\n"
	    "	st4		{v16.s, v17.s, v18.s, v19.s}[2], [sp], #16		\n"
	    "	st4		{v20.s, v21.s, v22.s, v23.s}[2], [sp], #16		\n"
	    "	st4		{v16.s, v17.s, v18.s, v19.s}[3], [sp], #16		\n"
	    "	st4		{v20.s, v21.s, v22.s, v23.s}[3], [sp], #16		\n"

	    "	add 	x2, x2, x6, lsl #5								\n"

	    "	ldp		q0, q1, [x22], #32								\n"
	    "	ldp		q2, q3, [x22], #32								\n"
	    "	ldp		q4, q5, [x22], #32								\n"
	    "	ldp		q6, q7, [x22], #32								\n"


	    "	prfm	PLDL1KEEP, [x27, #64]							\n"
	    "	prfm	PLDL1KEEP, [x28, #64]							\n"


	    "	st4		{v24.s, v25.s, v26.s, v27.s}[0], [sp], #16		\n"
	    "	st4		{v28.s, v29.s, v30.s, v31.s}[0], [sp], #16		\n"
	    "	st4		{v24.s, v25.s, v26.s, v27.s}[1], [sp], #16		\n"
	    "	st4		{v28.s, v29.s, v30.s, v31.s}[1], [sp], #16		\n"

	    "	ldp		q16, q17, [x25]									\n"
	    "	ldp		q18, q19, [x26]									\n"

	    "	st4		{v24.s, v25.s, v26.s, v27.s}[2], [sp], #16		\n"
	    "	st4		{v28.s, v29.s, v30.s, v31.s}[2], [sp], #16		\n"
	    "	st4		{v24.s, v25.s, v26.s, v27.s}[3], [sp], #16		\n"
	    "	st4		{v28.s, v29.s, v30.s, v31.s}[3], [sp], #16		\n"

	    "	ldp		q20, q21, [x27]									\n"
	    "	ldp		q22, q23, [x28]									\n"


	    "	fadd	v0.4s, v0.4s, v16.4s 							\n"
	    "	ldp		q8, q9, [x22], #32								\n"
	    "	fadd	v1.4s, v1.4s, v17.4s 							\n"
	    "	stp		q0, q1, [x25]									\n"

	    "	ldp		q10, q11, [x22], #32							\n"
	    "	fadd	v2.4s, v2.4s, v18.4s 							\n"
	    "	fadd	v3.4s, v3.4s, v19.4s 							\n"
	    "	stp		q2, q3, [x26]									\n"
	    "	add 	x25, x25, x9, lsl #4							\n"
	    "	prfm	PLDL1KEEP, [x25, #64]							\n"

	    "	ldp		q12, q13, [x22], #32							\n"
	    "	fadd	v4.4s, v4.4s, v20.4s 							\n"
	    "	ldp		q14, q15, [x22], #32							\n"
	    "	fadd	v5.4s, v5.4s, v21.4s 							\n"
	    "	stp		q4, q5, [x27]									\n"
	    "	add 	x26, x26, x9, lsl #4							\n"
	    "	prfm	PLDL1KEEP, [x26, #64]							\n"

	    "	ldp		q24, q25, [x25]									\n"

	    "	fadd	v6.4s, v6.4s, v22.4s 							\n"
	    "	fadd	v7.4s, v7.4s, v23.4s 							\n"
	    "	stp		q6, q7, [x28]									\n"
	    "	add 	x27, x27, x9, lsl #4							\n"
	    "	prfm	PLDL1KEEP, [x27, #64]							\n"

	    "	ldp		q26, q27, [x26]									\n"

	    "	add 	x28, x28, x9, lsl #4							\n"
	    "	prfm	PLDL1KEEP, [x28, #64]							\n"


	    "	fadd	v8.4s, v8.4s, v24.4s 							\n"
	    "	fadd	v9.4s, v9.4s, v25.4s 							\n"
	    "	ldp		q28, q29, [x27]									\n"

	    "	fadd	v10.4s, v10.4s, v26.4s 							\n"
	    "	fadd	v11.4s, v11.4s, v27.4s 							\n"
	    "	ldp		q30, q31, [x28]									\n"
	    "	stp		q8, q9, [x25] 									\n"

	    "	fadd	v12.4s, v12.4s, v28.4s 							\n"
	    "	fadd	v13.4s, v13.4s, v29.4s 							\n"
	    "	stp		q10, q11, [x26] 								\n"

	    " 	subs  x21, x21, #1                						\n"

	    "	add		x25, x25, x9, lsl #4							\n"
	    "	prfm	PLDL2KEEP, [x25, #64]							\n"
	    "	add		x26, x26, x9, lsl #4							\n"
	    "	prfm	PLDL2KEEP, [x25, #64]							\n"

	    "	fadd	v14.4s, v14.4s, v30.4s 							\n"
	    "	stp		q12, q13, [x27] 								\n"
	    "	add		x27, x27, x9, lsl #4							\n"
	    "	prfm	PLDL2KEEP, [x27, #64]							\n"
	    "	fadd	v15.4s, v15.4s, v31.4s 							\n"
	    "	stp		q14, q15, [x28] 								\n"
	    "	add		x28, x28, x9, lsl #4							\n"
	    "	prfm	PLDL2KEEP, [x28, #64]							\n"

	    ".endm	 													\n"



	    ".macro	KERNEL8x8_BEGIN_K_Mx8								\n"


	    "	prfm	PLDL1KEEP, [x11, #1024]							\n"
	    "	ldr		q0,	[x11], #16									\n"

	    "	ldr		q4, [x24], #16									\n"
	    "	ldr		q5, [x24], #16									\n"

	    "	fmul	v16.4s, v4.4s, v0.s[0]			 				\n"
	    "	fmul	v17.4s, v5.4s, v0.s[0]			 				\n"

	    "	ldr		q1,	[x11], #16									\n"

	    "	fmul	v18.4s, v4.4s, v0.s[1]			 				\n"
	    "	fmul	v19.4s, v5.4s, v0.s[1]			 				\n"

	    "	ldr		q2,	[x11], #16									\n"

	    "	fmul	v20.4s, v4.4s, v0.s[2]			 				\n"
	    "	fmul	v21.4s, v5.4s, v0.s[2]			 				\n"

	    "	ldr		q6,	[x24], #16									\n"

	    "	fmul	v22.4s, v4.4s, v0.s[3]			 				\n"
	    "	fmul	v23.4s, v5.4s, v0.s[3]			 				\n"


	    "	ldr		q3,	[x11], #16									\n"
	    "	fmul	v24.4s, v4.4s, v1.s[0]			 				\n"
	    "	fmul	v25.4s, v5.4s, v1.s[0]			 				\n"

	    "	ldr		q7,	[x24], #16									\n"

	    "	fmul	v26.4s, v4.4s, v1.s[1]			 				\n"
	    "	fmul	v27.4s, v5.4s, v1.s[1]			 				\n"

	    "	prfm	PLDL1KEEP, [x24, #256]							\n"

	    "	fmul	v28.4s, v4.4s, v1.s[2]			 				\n"
	    "	fmul	v29.4s, v5.4s, v1.s[2]			 				\n"

	    "	fmul	v30.4s, v4.4s, v1.s[3]			 				\n"
	    "	fmul	v31.4s, v5.4s, v1.s[3]			 				\n"


	    ".endm 										 				\n"



	    ".macro	KERNEL8x8_K0_Mx8									\n"


	    "	prfm	PLDL1KEEP, [x11, #1024]							\n"

	    "	ldr		q2, [x11], #16									\n"

	    "	fmla	v16.4s, v4.4s, v0.s[0]			 				\n"
	    "	fmla	v17.4s, v5.4s, v0.s[0]			 				\n"
	    "	ldr		q6, [x24], #16									\n"

	    "	fmla	v18.4s, v4.4s, v0.s[1]			 				\n"
	    "	fmla	v19.4s, v5.4s, v0.s[1]			 				\n"
	    "	ldr		q3, [x11], #16									\n"

	    "	fmla	v20.4s, v4.4s, v0.s[2]			 				\n"
	    "	fmla	v21.4s, v5.4s, v0.s[2]			 				\n"

	    "	ldr		q7, [x24], #16									\n"

	    "	fmla	v22.4s, v4.4s, v0.s[3]			 				\n"
	    "	fmla	v23.4s, v5.4s, v0.s[3]			 				\n"

	    "	prfm	PLDL1KEEP, [x11, #1024]							\n"

	    "	fmla	v24.4s, v4.4s, v1.s[0]			 				\n"
	    "	fmla	v25.4s, v5.4s, v1.s[0]			 				\n"
	    "	fmla	v26.4s, v4.4s, v1.s[1]			 				\n"
	    "	fmla	v27.4s, v5.4s, v1.s[1]			 				\n"
	    "	fmla	v28.4s, v4.4s, v1.s[2]			 				\n"
	    "	fmla	v29.4s, v5.4s, v1.s[2]			 				\n"
	    "	fmla	v30.4s, v4.4s, v1.s[3]			 				\n"
	    "	fmla	v31.4s, v5.4s, v1.s[3]			 				\n"


	    ".endm 										 				\n"


	    ".macro	KERNEL8x8_K1_Mx8									\n"


	    "	ldr		q0, [x11], #16									\n"
	    "	fmla	v16.4s, v6.4s, v2.s[0]			 				\n"
	    "	fmla	v17.4s, v7.4s, v2.s[0]			 				\n"
	    "	ldr		q4, [x24], #16									\n"

	    "	fmla	v18.4s, v6.4s, v2.s[1]			 				\n"
	    "	fmla	v19.4s, v7.4s, v2.s[1]			 				\n"
	    "	ldr		q1, [x11], #16									\n"

	    "	fmla	v20.4s, v6.4s, v2.s[2]			 				\n"
	    "	fmla	v21.4s, v7.4s, v2.s[2]			 				\n"
	    "	ldr		q5, [x24], #16									\n"

	    "	fmla	v22.4s, v6.4s, v2.s[3]			 				\n"
	    "	fmla	v23.4s, v7.4s, v2.s[3]			 				\n"

	    "	prfm	PLDL1KEEP, [x24, #256]							\n"

	    "	fmla	v24.4s, v6.4s, v3.s[0]			 				\n"
	    "	fmla	v25.4s, v7.4s, v3.s[0]			 				\n"
	    "	fmla	v26.4s, v6.4s, v3.s[1]			 				\n"
	    "	fmla	v27.4s, v7.4s, v3.s[1]			 				\n"
	    "	fmla	v28.4s, v6.4s, v3.s[2]			 				\n"
	    "	fmla	v29.4s, v7.4s, v3.s[2]			 				\n"
	    "	fmla	v30.4s, v6.4s, v3.s[3]			 				\n"
	    "	fmla	v31.4s, v7.4s, v3.s[3]			 				\n"


	    ".endm 										 				\n"



	    ".macro	KERNEL8x8_END_K_Mx8									\n"


	    " 	prfm  PLDL1KEEP, [x25, #32]      						\n"
	    " 	prfm  PLDL1KEEP, [x26, #32]      						\n"
	    " 	prfm  PLDL1KEEP, [x27, #32]      						\n"
	    " 	prfm  PLDL1KEEP, [x28, #32]      						\n"

	    "	fmla	v16.4s, v6.4s, v2.s[0]			 				\n"
	    "	fmla	v17.4s, v7.4s, v2.s[0]			 				\n"
	    "	fmla	v18.4s, v6.4s, v2.s[1]			 				\n"
	    "	fmla	v19.4s, v7.4s, v2.s[1]			 				\n"
	    "	fmla	v20.4s, v6.4s, v2.s[2]			 				\n"
	    "	prfm  	PLDL1KEEP, [x15, #32]      						\n"
	    " 	prfm  	PLDL1KEEP, [x16, #32]      						\n"
	    "	fmla	v21.4s, v7.4s, v2.s[2]			 				\n"
	    "	fmla	v22.4s, v6.4s, v2.s[3]			 				\n"
	    "	prfm  	PLDL1KEEP, [x17, #32]      						\n"
	    "	prfm  	PLDL1KEEP, [x18, #32]      						\n"
	    "	fmla	v23.4s, v7.4s, v2.s[3]			 				\n"

	    "	fmla	v24.4s, v6.4s, v3.s[0]			 				\n"
	    "	fmla	v25.4s, v7.4s, v3.s[0]			 				\n"
	    "	fmla	v26.4s, v6.4s, v3.s[1]			 				\n"
	    "	fmla	v27.4s, v7.4s, v3.s[1]			 				\n"
	    "	fmla	v28.4s, v6.4s, v3.s[2]			 				\n"
	    "	fmla	v29.4s, v7.4s, v3.s[2]			 				\n"
	    "	fmla	v30.4s, v6.4s, v3.s[3]			 				\n"
	    "	fmla	v31.4s, v7.4s, v3.s[3]			 				\n"

	    ".endm 										 				\n"


	    ".macro	M8N8_ADD_C_Mx8										\n"

	    "	ldp		q0, q1, [x25]									\n"
	    "	fadd	v16.4s, v16.4s, v0.4s							\n"
	    "	ldp		q2, q3, [x26]									\n"
	    "	fadd	v17.4s, v17.4s, v1.4s							\n"
	    "	ldp		q4, q5, [x27]									\n"
	    "	fadd	v18.4s, v18.4s, v2.4s							\n"
	    "	ldp		q6, q7, [x28]									\n"
	    "	fadd	v19.4s, v19.4s, v3.4s							\n"

	    "	ldp		q8, q9, [x15]									\n"
	    "	fadd	v20.4s, v20.4s, v4.4s							\n"
	    "	ldp		q10, q11, [x16]									\n"
	    "	fadd	v21.4s, v21.4s, v5.4s							\n"
	    "	ldp		q12, q13, [x17]									\n"
	    "	fadd	v22.4s, v22.4s, v6.4s							\n"
	    "	ldp		q14, q15, [x18]									\n"
	    "	fadd	v23.4s, v23.4s, v7.4s							\n"


	    "	fadd	v24.4s, v24.4s, v8.4s							\n"
	    "	fadd	v25.4s, v25.4s, v9.4s							\n"
	    "	fadd	v26.4s, v26.4s, v10.4s							\n"
	    "	fadd	v27.4s, v27.4s, v11.4s							\n"
	    "	fadd	v28.4s, v28.4s, v12.4s							\n"
	    "	fadd	v29.4s, v29.4s, v13.4s							\n"
	    "	fadd	v30.4s, v30.4s, v14.4s							\n"
	    "	fadd	v31.4s, v31.4s, v15.4s							\n"


	    ".endm														\n"


	    ".macro	SAVE8x8_Mx8											\n"

	    "	subs	x21, x21, #1					 				\n"

	    "	stp		q16, q17, [x25]			 		 				\n"
	    "	add 	x25, x25, x9, lsl #5							\n"
	    "	prfm	PLDL2KEEP, [x25, #32]							\n"
	    "	stp		q18, q19, [x26]			 	     				\n"
	    "	add 	x26, x26, x9, lsl #5							\n"
	    "	prfm	PLDL2KEEP, [x26, #32]							\n"
	    "	stp		q20, q21, [x27]					 				\n"
	    "	add 	x27, x27, x9, lsl #5							\n"
	    "	prfm	PLDL2KEEP, [x27, #32]							\n"
	    "	stp		q22, q23, [x28]					 				\n"
	    "	add 	x28, x28, x9, lsl #5							\n"
	    "	prfm	PLDL2KEEP, [x28, #32]							\n"

	    "	stp		q24, q25, [x15]			 		 				\n"
	    "	add 	x15, x15, x9, lsl #5							\n"
	    "	prfm	PLDL2KEEP, [x15, #32]							\n"
	    "	stp		q26, q27, [x16]			 	     				\n"
	    "	add 	x16, x16, x9, lsl #5							\n"
	    "	prfm	PLDL2KEEP, [x16, #32]							\n"
	    "	stp		q28, q29, [x17]					 				\n"
	    "	add 	x17, x17, x9, lsl #5							\n"
	    "	prfm	PLDL2KEEP, [x17, #32]							\n"
	    "	stp		q30, q31, [x18]					 				\n"
	    "	add 	x18, x18, x9, lsl #5							\n"
	    "	prfm	PLDL2KEEP, [x18, #32]							\n"

	    ".endm										 				\n"



	    //----------------------------------------------------

	    "SMM_NT_KERNELMx8:									 	 	\n"

	    "	ldr		x0, %[C]						 				\n"
	    "	ldr		x1, %[A]						 				\n"
	    "	ldr		x2, %[B]						 				\n"

	    "	ldr		x3, %[M]						 				\n"
	    "	ldr		x4, %[N]						 				\n"
	    "	ldr		x5, %[K]										\n"
	    "	ldr		x9, %[LN] 						 				\n"
	    "	ldr		x6, %[LK]						 				\n"

	    "	prfm    PLDL1KEEP, [x1, #512]           				\n"
	    "	prfm    PLDL1KEEP, [x2, #128]           				\n"

	    "	ldr		x10, %[SB]						 				\n"
	    "	ldr		x8, %[k_tag] 					 				\n"

	    "	lsr		x20, x4, #3						 				\n"

	    "BEGIN_N8_Mx8:								 	 			\n"

	    "	mov 	x25, x0   						 				\n"   //C1*
	    "	add		x26, x25, x9, lsl #2 			 				\n"   //C2*
	    "	add 	x27, x25, x9, lsl #3 			 				\n"   //C3*
	    " 	add 	x28, x26, x9, lsl #3 			 				\n"   //C4*

	    "	mov		x11, x1   						 				\n"
	    "	lsr		x21, x3, #3						 				\n"

	    "BEGIN_PACKB_Mx8:								 			\n"

	    "	mov		x24, x10						 				\n"   //还原SB的地址

	    "	mov		x12, x2						 	 				\n"   //B0*
	    "	add		x13, x12, x6, lsl #2 			 				\n"
	    "	add		x14, x12, x6, lsl #3 			 				\n"
	    "	add		x15, x13, x6, lsl #3 			 				\n"

	    "	prfm    PLDL1KEEP, [x12, #64]			 				\n"
	    "	prfm    PLDL1KEEP, [x13, #64]			 				\n"
	    "	prfm    PLDL1KEEP, [x14, #64]			 				\n"
	    "	prfm    PLDL1KEEP, [x15, #64]			 				\n"

	    "	add		x16, x14, x6, lsl #3 			 				\n"
	    "	add		x17, x15, x6, lsl #3 			 				\n"
	    "	add		x18, x16, x6, lsl #3 			 				\n"
	    "	add		x19, x17, x6, lsl #3 			 				\n"

	    "	prfm    PLDL1KEEP, [x16, #64]			 				\n"
	    "	prfm    PLDL1KEEP, [x17, #64]			 				\n"
	    "	prfm    PLDL1KEEP, [x18, #64]			 				\n"
	    "	prfm    PLDL1KEEP, [x19, #64]			 				\n"

	    "	prfm    PLDL1KEEP, [x11, #256]			 				\n"


	    "PACK_Body_K_Mx8:								 			\n"

	    "	lsr		x22, x5, #3 					 				\n"     // K / 8

	    "	PACK_KERNEL8x8_BEGIN_K_Mx8				 	 			\n"

	    "	subs	x22, x22, #1		     		 				\n"
	    "	b 		PACK_K1_7_Mx8						 			\n"

	    "PACK_K_Mx8:									 			\n"

	    "	PACK_KERNEL8x8_K0_Mx8						 			\n"
	    "PACK_K1_7_Mx8:									 			\n"

	    "	PACK_KERNEL8x8_K1_Mx8						 			\n"
	    "	PACK_KERNEL8x8_K2_Mx8						 			\n"
	    "	PACK_KERNEL8x8_K3_Mx8						 			\n"
	    "	PACK_KERNEL8x8_K4_Mx8						 			\n"
	    "	PACK_KERNEL8x8_K5_Mx8						 			\n"
	    "	PACK_KERNEL8x8_K6_Mx8						 			\n"

	    "	beq		PACK_Edge_K_Mx8						 			\n"

	    "	PACK_KERNEL8x8_K7_Mx8						 			\n"

	    "	subs	x22, x22, #1			 		 				\n"
	    "	b 		PACK_K_Mx8			 				 			\n"

	    "PACK_Edge_K_Mx8:							     			\n"


	    "	PACK_KERNEL8x8_END_K_Mx8			         			\n"


	    "	cmp		x8, #0							 				\n"
	    "	beq		M8N8_PACK_SAVE_Mx8 					 			\n"
	    "	PACK_SAVE_Mx8 											\n"
	    "	b 	END_PACKB_Mx8						 	 			\n"
	    "M8N8_PACK_SAVE_Mx8: 							 			\n"
	    "	PACK_SAVE0_Mx8								 			\n"

	    "END_PACKB_Mx8:									 			\n"

	    "	beq		M8_END_Mx8							 			\n"

	    "	add		x15, x25, x9, lsl #4			 				\n"
	    "	add		x16, x26, x9, lsl #4			 				\n"
	    "	add		x17, x27, x9, lsl #4			 				\n"
	    "	add		x18, x28, x9, lsl #4			 				\n"




	    //----------------------------------------------------------
	    "BEGIN_M8_Mx8:                          					\n"

	    " mov     x24, x10                    						\n"
	    " prfm    PLDL1KEEP, [x24, #256]    						\n"
	    " prfm    PLDL1KEEP, [x11, #256]    						\n"

	    "Body_K_Mx8:                            					\n"

	    " lsr   x22, x5, #2                 						\n"     // K / 4
	    " KERNEL8x8_BEGIN_K_Mx8                		 				\n"
	    " subs  x22, x22, #1                						\n"
	    " b 	K1_7_Mx8                            				\n"

	    "Main_K_Mx8:                            					\n"

	    " KERNEL8x8_K0_Mx8                     		 				\n"
	    "K1_7_Mx8:                              					\n"

	    " KERNEL8x8_K1_Mx8                     		 				\n"
	    " KERNEL8x8_K0_Mx8                     		 				\n"

	    " beq   Edge_K_Mx8                      					\n"

	    " KERNEL8x8_K1_Mx8                     		 				\n"

	    " subs  x22, x22, #1                						\n"
	    " b     Main_K_Mx8                      					\n"

	    "Edge_K_Mx8:                            					\n"

	    " KERNEL8x8_END_K_Mx8                 						\n"

	    "	cmp		x8, #0							 				\n"
	    "	beq		M8N8_SAVE_Mx8 					 	 			\n"
	    "	M8N8_ADD_C_Mx8 							 	 			\n"
	    "M8N8_SAVE_Mx8: 							     			\n"
	    "	SAVE8x8_Mx8								 	 			\n"

	    " bgt     BEGIN_M8_Mx8                						\n"
	    "M8_END_Mx8:									 			\n"



	    "	subs	x20, x20, #1					 				\n"
	    "	add		x0, x0, #32						 				\n"
	    "	bgt		BEGIN_N8_Mx8						 			\n"

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
	    : "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
	    "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16",
	    "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24", "x25",
	    "x26", "x27", "x28", "x29", "x30",
	    "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
	    "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
	    "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
	    "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"
	);

}
