#pragma once


void SMM_NN_KERNEL1xN(float *C, float *A, float *B, long	M, long N, long K,
                      long LN, long LK, float *SA, long k_tag)
{

		asm volatile(


		".macro		KERNEL1x8_K_1xN	 								\n"

		"	ldp		q2, q3, [x11], #32 								\n"
		"	ldr		q0, [x12], #16									\n"

		"	fmla	v24.4s, v2.4s, v0.s[0]							\n"
		"	ldp		q4, q5, [x11], #32 								\n"
		"	fmla	v25.4s, v3.4s, v0.s[0]							\n"

		"	prfm	PLDL1KEEP, [x11, #64]							\n"

		"	ldp		q6, q7, [x11], #32 								\n"
		"	fmla	v24.4s, v4.4s, v0.s[1]							\n"
		"	ldp		q8, q9, [x11], #32 								\n"
		"	fmla	v25.4s, v5.4s, v0.s[1]							\n"

		"	fmla	v24.4s, v6.4s, v0.s[2]							\n"
		"	fmla	v25.4s, v7.4s, v0.s[2]							\n"

		"	prfm	PLDL1KEEP, [x12, #64]							\n"

		"	fmla	v24.4s, v8.4s, v0.s[3]							\n"
		"	fmla	v25.4s, v9.4s, v0.s[3]							\n"

		".endm 														\n"


		".macro	SAVE_ADD_C_1xN  									\n"

		"	ldp 	q0, q1, [x25]									\n"
		"	fadd 	v24.4s, v24.4s, v0.4s 							\n"
		"	fadd 	v25.4s, v25.4s, v1.4s 							\n"

		".endm 														\n"


		".macro SAVE1x8_1xN 										\n"

		"	subs	x21, x21, #1 									\n"
		"	stp		q24, q25, [x25], #32 							\n"

		".endm 														\n"


		".macro	KERNEL1x1_K_N1_1xN	 								\n"

		"	ldr		q0, [x12], #16 									\n"
		"	ldr		q1, [x11], #16 									\n"
		"	fmla	v24.4s, v0.4s, v1.4s 							\n"
		"	prfm	PLDL1KEEP, [x11, #64]							\n"

		".endm 														\n"


	    ".macro	SAVE_ADD_C_N1_1xN									\n"

	    "	ldr		s0, [x25]										\n"
	    "	fadd	s24, s24, s0									\n"

	    ".endm														\n"


	    ".macro	SAVE1x1_1xN											\n"

	    "	str 	s24, [x25] 										\n"

	    ".endm										 				\n"



		"SMM_NN_KERNEL1xN:											\n"

	    "	ldr		x0, %[C]						 				\n"
	    "	ldr		x1, %[A]						 				\n"
	    "	ldr		x2, %[B]						 				\n"

	    "	ldr		x3, %[M]						 				\n"
	    "	ldr		x4, %[N]						 				\n"
	    "	ldr		x5, %[K]						 				\n"
	    "	ldr		x9, %[LN] 						 				\n"
	    "	ldr		x6, %[LK]						 				\n"

	    "	prfm	PLDL1KEEP, [x1, #512]           				\n"
	    "	prfm	PLDL1KEEP, [x2, #64]          					\n"


	    "	ldr		x10, %[SA]						 				\n"
	    "	ldr		x8, %[k_tag] 					 				\n"
//---------------------------------------------------------------------------------

	    "	mov		x11, x2 										\n"		//address_B
	    "	lsr		x21, x4, #3						 				\n"    // N / 8

	    "	mov		x25, x0		     		 						\n"
	    "	prfm	PLDL2KEEP, [x25, #64] 							\n"
	    //--------------------------------------------------------------PACK

	    "BEGIN_NN_1xN:								 				\n"

	    "	prfm	PLDL1KEEP, [x11, #256]			 				\n"
	    "	mov		x12, x1						 	 				\n"   //A0*
	    "	prfm	PLDL1KEEP, [x12, #64]			 				\n"

	    "	dup		v24.4s, wzr 									\n"
	    "	dup		v25.4s, wzr 									\n"
	    "	lsr		x22, x5, #2 					 				\n"   // K / 4
		"Body_K_1xN:								 				\n"
		"	subs	x22, x22, #1		     		 				\n"
		"	KERNEL1x8_K_1xN											\n"
	   	"	bgt		Body_K_1xN										\n"

	    "	cmp		x8, #0							 				\n"
	    "	beq		M1N8_SAVE_1xN 					 				\n"
	    "	SAVE_ADD_C_1xN 								 			\n"
	    "M1N8_SAVE_1xN: 							 				\n"
	    "	SAVE1x8_1xN								 				\n"

	    "	bgt		BEGIN_NN_1xN									\n"

	    "	ands	x21, x4, #7 									\n"
	    "	beq		N_END_1xN 										\n"
	    "	cmp		x21, #1 										\n"
	    "	blt		N_END_1xN 										\n"


	    "	mov		x12, x1						 	 				\n"   //A0*
	    "	prfm	PLDL1KEEP, [x12, #64]			 				\n"

	    "	dup		v24.4s, wzr 									\n"
	    "	lsr		x22, x5, #2 					 				\n"   // K / 8
		"Body_K_N1_1xN:								 				\n"
		"	subs	x22, x22, #1		     		 				\n"
		"	KERNEL1x1_K_N1_1xN										\n"
	   	"	bgt		Body_K_N1_1xN									\n"

	   	"	faddp 	v24.4s, v24.4s, v24.4s							\n"
	   	"	faddp 	s24, v24.2s 								   	\n"

	    "	cmp		x8, #0							 				\n"
	    "	beq		M1N1_SAVE_1xN 					 				\n"
	    "	SAVE_ADD_C_N1_1xN 								 		\n"
	    "M1N1_SAVE_1xN: 							 				\n"
	    "	SAVE1x1_1xN								 				\n"

//--------------------------------------------------------------
//--------------------------------------------------------------

	    "N8_END_1xN:									 			\n"
	    "N_END_1xN: 												\n"

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
	    [SA] "m" (SA),
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


void SMM_NN_KERNEL2xN(float *C, float *A, float *B, long	M, long N, long K,
                      long LN, long LK, float *SA, long k_tag)
{

	asm volatile(


		".macro	PACK_KERNEL2x8_BEGIN_K_2xN	 						\n"


		"	ldp 	q2, q3, [x11], #32 								\n"
	    "	ldr 	q0, [x12], #16 									\n"

	    "	fmul	v24.4s, v2.4s, v0.s[0] 				 			\n"
	    "	ldr 	q1, [x13], #16 									\n"
	    "	fmul	v25.4s, v3.4s, v0.s[0] 				 			\n"
	    "	prfm	PLDL1KEEP, [x12, #128]							\n"

	    "	fmul	v26.4s, v2.4s, v1.s[0] 				 			\n"
	    "	fmul	v27.4s, v3.4s, v1.s[0] 				  			\n"
	    "	prfm	PLDL1KEEP, [x13, #128]							\n"

	    "	ldp		q6, q7, [x11], #32 								\n"
	    "	st2		{v0.s, v1.s}[0], [x24], #8						\n"	    
	    "	ldr		q4, [x12], #16 									\n"

		".endm 														\n"

//----------------------------------------------------------------------
		".macro	PACK_KERNEL2x8_K0_2xN								\n"

		"	prfm	PLDL1KEEP, [x12, #128]							\n"
		"	ldr  	q6, [x11], #16 									\n"
		"	fmla	v24.4s, v2.4s, v0.s[0]							\n"
		"	fmla	v25.4s, v3.4s, v0.s[0] 							\n"

		"	ldr  	q7, [x11], #16 									\n"
		"	fmla	v26.4s, v2.4s, v1.s[0]							\n"
		"	fmla	v27.4s, v3.4s, v1.s[0] 							\n"

		"	ldr 	q4, [x12], #16 									\n"
		"	st2		{v0.s, v1.s}[0], [x24], #8						\n"

	    ".endm 												  		\n"



	    ".macro	PACK_KERNEL2x8_K1_2xN								\n"

		"	prfm	PLDL1KEEP, [x13, #128]							\n"
		"	ldr  	q2, [x11], #16 									\n"
		"	fmla	v24.4s, v6.4s, v0.s[1]							\n"
		"	fmla	v25.4s, v7.4s, v0.s[1] 							\n"

		"	ldr  	q3, [x11], #16 									\n"
		"	fmla	v26.4s, v6.4s, v1.s[1]							\n"
		"	fmla	v27.4s, v7.4s, v1.s[1] 							\n"

		"	ldr 	q5, [x13], #16 									\n"
		"	st2		{v0.s, v1.s}[1], [x24], #8						\n"

	    ".endm 												  		\n"


	    ".macro	PACK_KERNEL2x8_K2_2xN								\n"

		"	ldr  	q6, [x11], #16 									\n"
		"	fmla	v24.4s, v2.4s, v0.s[2]							\n"
		"	fmla	v25.4s, v3.4s, v0.s[2] 							\n"

		"	ldr  	q7, [x11], #16 									\n"
		"	fmla	v26.4s, v2.4s, v1.s[2]							\n"
		"	fmla	v27.4s, v3.4s, v1.s[2] 							\n"
		"	st2		{v0.s, v1.s}[2], [x24], #8						\n"


	    ".endm 												  		\n"


	    ".macro	PACK_KERNEL2x8_K3_2xN								\n"

		"	prfm	PLDL1KEEP, [x11, #512]							\n"
		"	ldr  	q2, [x11], #16 									\n"
		"	fmla	v24.4s, v6.4s, v0.s[3]							\n"
		"	fmla	v25.4s, v7.4s, v0.s[3] 							\n"

		"	ldr  	q3, [x11], #16 									\n"
		"	fmla	v26.4s, v6.4s, v1.s[3]							\n"
		"	fmla	v27.4s, v7.4s, v1.s[3] 							\n"

		"	st2		{v0.s, v1.s}[3], [x24], #8						\n"

	    ".endm 												  		\n"

//-------------------------------------------------------------------------------


		".macro	PACK_KERNEL2x8_K4_2xN								\n"

		"	prfm	PLDL1KEEP, [x12, #128]							\n"
		"	ldr  	q6, [x11], #16 									\n"
		"	fmla	v24.4s, v2.4s, v4.s[0]							\n"
		"	fmla	v25.4s, v3.4s, v4.s[0] 							\n"

		"	ldr  	q7, [x11], #16 									\n"
		"	fmla	v26.4s, v2.4s, v5.s[0]							\n"
		"	fmla	v27.4s, v3.4s, v5.s[0] 							\n"

		"	ldr 	q0, [x12], #16 									\n"
		"	st2		{v4.s, v5.s}[0], [x24], #8						\n"

	    ".endm 												  		\n"



	    ".macro	PACK_KERNEL2x8_K5_2xN								\n"

		"	prfm	PLDL1KEEP, [x13, #128]							\n"
		"	ldr  	q2, [x11], #16 									\n"
		"	fmla	v24.4s, v6.4s, v4.s[1]							\n"
		"	fmla	v25.4s, v7.4s, v4.s[1] 							\n"

		"	ldr  	q3, [x11], #16 									\n"
		"	fmla	v26.4s, v6.4s, v5.s[1]							\n"
		"	fmla	v27.4s, v7.4s, v5.s[1] 							\n"

		"	ldr 	q1, [x13], #16 									\n"
		"	st2		{v4.s, v5.s}[1], [x24], #8						\n"

	    ".endm 												  		\n"


	    ".macro	PACK_KERNEL2x8_K6_2xN								\n"

		"	ldr  	q6, [x11], #16 									\n"
		"	fmla	v24.4s, v2.4s, v4.s[2]							\n"
		"	fmla	v25.4s, v3.4s, v4.s[2] 							\n"

		"	ldr  	q7, [x11], #16 									\n"
		"	fmla	v26.4s, v2.4s, v5.s[2]							\n"
		"	fmla	v27.4s, v3.4s, v5.s[2] 							\n"
		"	st2		{v4.s, v5.s}[2], [x24], #8						\n"

	    ".endm 												  		\n"



	    ".macro	PACK_KERNEL2x8_K7_2xN								\n"

		"	prfm	PLDL1KEEP, [x11, #512]							\n"
		"	ldr  	q2, [x11], #16 									\n"
		"	fmla	v24.4s, v6.4s, v4.s[3]							\n"
		"	fmla	v25.4s, v7.4s, v4.s[3] 							\n"

		"	ldr  	q3, [x11], #16 									\n"
		"	fmla	v26.4s, v6.4s, v5.s[3]							\n"
		"	fmla	v27.4s, v7.4s, v5.s[3] 							\n"

		"	st2		{v4.s, v5.s}[3], [x24], #8						\n"

	    ".endm 												  		\n"


	    ".macro	PACK_KERNEL2x8_END_K_2xN							\n"

	    "	mov 	x25, x0   						 				\n"   //C1*
	    "	prfm	PLDL2KEEP, [x25, #64]							\n"
	    "	add		x26, x25, x9, lsl #2 			 				\n"   //C2*
	    "	prfm	PLDL2KEEP, [x26, #64]							\n"

		"	fmla	v24.4s, v6.4s, v4.s[3]							\n"
		"	fmla	v25.4s, v7.4s, v4.s[3] 							\n"
		"	fmla	v26.4s, v6.4s, v5.s[3]							\n"
		"	fmla	v27.4s, v7.4s, v5.s[3] 							\n"

		"	st2		{v4.s, v5.s}[3], [x24], #8						\n"

	    ".endm 												  		\n"


	    ".macro	SAVE_ADD_C_2xN										\n"

	    "	ldp		q0, q1, [x25]									\n"
	    "	fadd	v24.4s, v24.4s, v0.4s							\n"
	    "	ldp		q2, q3, [x26]									\n"
	    "	fadd	v25.4s, v25.4s, v1.4s							\n"
	    "	fadd	v26.4s, v26.4s, v2.4s							\n"
	    "	fadd	v27.4s, v27.4s, v3.4s							\n"

	    ".endm														\n"


	    ".macro	SAVE2x8_2xN											\n"

	    "	subs	x21, x21, #1					 				\n"
	    "	stp		q24, q25, [x25], #32			 		 		\n"
	    "	prfm	PLDL2KEEP, [x25, #32]							\n"
	    "	stp		q26, q27, [x26], #32			 	    		\n"
	    "	prfm	PLDL2KEEP, [x26, #32]							\n"

	    ".endm										 				\n"



	   	".macro	KERNEL2x8_BEGIN_K_2xN	 							\n"


		"	ldp 	q2, q3, [x11], #32 								\n"
	    "	ld1 	{v0.2s}, [x24], #8 								\n"

	    "	fmul	v24.4s, v2.4s, v0.s[0] 				 			\n"
	    "	fmul	v25.4s, v3.4s, v0.s[0] 				 			\n"

	    "	fmul	v26.4s, v2.4s, v0.s[1] 				 			\n"
	    "	fmul	v27.4s, v3.4s, v0.s[1] 				  			\n"
	    "	prfm	PLDL1KEEP, [x24, #128]							\n"

	    "	ldp		q6, q7, [x11], #32 								\n"   
	    "	ld1		{v4.2s}, [x24], #8 								\n"

		".endm 														\n"


//----------------------------------------------------------------------
		".macro	KERNEL2x8_K0_2xN									\n"

		"	prfm	PLDL1KEEP, [x24, #128]							\n"
		"	ldr  	q6, [x11], #16 									\n"
		"	fmla	v24.4s, v2.4s, v0.s[0]							\n"
		"	ldr  	q7, [x11], #16 									\n"
		"	fmla	v25.4s, v3.4s, v0.s[0] 							\n"

		"	ld1		{v4.2s}, [x24], #8 								\n"
		"	fmla	v26.4s, v2.4s, v0.s[1]							\n"
		"	fmla	v27.4s, v3.4s, v0.s[1] 							\n"

	    ".endm 												  		\n"



	    ".macro	KERNEL2x8_K1_2xN									\n"

		"	prfm	PLDL1KEEP, [x11, #256]							\n"
		"	ldr  	q2, [x11], #16 									\n"
		"	fmla	v24.4s, v6.4s, v4.s[0]							\n"
		"	ldr  	q3, [x11], #16 									\n"
		"	fmla	v25.4s, v7.4s, v4.s[0] 							\n"

		"	ld1 	{v0.2s}, [x24], #8 								\n"
		"	fmla	v26.4s, v6.4s, v4.s[1]							\n"
		"	fmla	v27.4s, v7.4s, v4.s[1] 							\n"

	    ".endm 												  		\n"


	    ".macro	KERNEL2x8_END_K_2xN									\n"

		"	fmla	v24.4s, v6.4s, v4.s[0]							\n"	
		"	fmla	v25.4s, v7.4s, v4.s[0] 							\n"

		"	fmla	v26.4s, v6.4s, v4.s[1]							\n"
		"	fmla	v27.4s, v7.4s, v4.s[1] 							\n"

	    ".endm 												  		\n"


	    ".macro	SAVE_ADD_C_N2_2xN 									\n"

	    "	ldr 	s0, [x25] 										\n"
	    "	ldr 	s1, [x26] 										\n"

	    "	fadd 	s24, s24, s0 									\n"
	    "	fadd 	s25, s25, s1 									\n"

	    ".endm 														\n"


	    ".macro	SAVE2x1_2xN 										\n"

	    "	str		s24, [x25] 										\n"
	    "	str 	s25, [x26] 										\n"

	    ".endm 													    \n"




		"SMM_NN_KERNEL2xN:											\n"

	    "	ldr		x0, %[C]						 				\n"
	    "	ldr		x1, %[A]						 				\n"
	    "	ldr		x2, %[B]						 				\n"

	    "	ldr		x3, %[M]						 				\n"
	    "	ldr		x4, %[N]						 				\n"
	    "	ldr		x5, %[K]						 				\n"
	    "	ldr		x9, %[LN] 						 				\n"
	    "	ldr		x6, %[LK]						 				\n"

	    "	prfm	PLDL1KEEP, [x1, #512]           				\n"
	    "	prfm	PLDL1KEEP, [x2, #64]          					\n"


	    "	ldr		x10, %[SA]						 				\n"
	    "	ldr		x8, %[k_tag] 					 				\n"
//---------------------------------------------------------------------------------

	    "	mov		x11, x2 										\n"		//address_B
	    "	lsr		x21, x4, #3						 				\n"    // N / 8

	    //-----------------------------------------------------------------PACK

	    "BEGIN_PACKA_NN_2xN:								 		\n"

	    "	mov		x24, x10						 				\n"   //还原SA的地址
	    "	prfm	PLDL1KEEP, [x11, #256]			 				\n"

	    "	mov		x12, x1						 	 				\n"   //A0*
	    "	add		x13, x12, x6, lsl #2 			 				\n"

	    "	prfm	PLDL1KEEP, [x12, #64]			 				\n"
	    "	prfm	PLDL1KEEP, [x13, #64]			 				\n"

	    "PACK_Body_K_2xN:								 			\n"

	    "	lsr		x22, x5, #3 					 				\n"   // K / 8
	    "	PACK_KERNEL2x8_BEGIN_K_2xN				 	 			\n" 

	    "	subs	x22, x22, #1		     		 				\n"
	    "	b 		PACK_K1_7_2xN						 			\n"

	    "PACK_K_2xN:									 			\n"

	    "	PACK_KERNEL2x8_K0_2xN						 			\n"
	    "PACK_K1_7_2xN:									 			\n"

	    "	PACK_KERNEL2x8_K1_2xN						 			\n"
	    "	PACK_KERNEL2x8_K2_2xN						 			\n"
	    "	PACK_KERNEL2x8_K3_2xN						 			\n"
	    "	PACK_KERNEL2x8_K4_2xN								 	\n"
	    "	PACK_KERNEL2x8_K5_2xN						 			\n"
	    "	PACK_KERNEL2x8_K6_2xN						 			\n"

	    "	beq		PACK_Edge_K_2xN						 			\n"

	    "	PACK_KERNEL2x8_K7_2xN						 		 	\n"

	    "	subs	x22, x22, #1			 		 				\n"
	    "	b 		PACK_K_2xN			 				 			\n"

	    "PACK_Edge_K_2xN:							     			\n"
	    "	PACK_KERNEL2x8_END_K_2xN			         			\n"

	    "	cmp		x8, #0							 				\n"
	    "	beq		M2N8_PACK_SAVE_2xN 					 			\n"
	    "	SAVE_ADD_C_2xN 								 			\n"
	    "M2N8_PACK_SAVE_2xN: 							 			\n"
	    "	SAVE2x8_2xN								 				\n"

	    "END_PACKA_2xN:									 			\n"
	    "	beq		N8_END_2xN							 			\n"


//--------------------------------------------------------------
//--------------------------------------------------------------


	    "BEGIN_N8_2xN:                          		 			\n"

	    " mov 	x24, x10                    						\n"
	    " prfm	PLDL1KEEP, [x24, #256]    		 					\n"
	    " prfm	PLDL1KEEP, [x11, #256]    		 					\n"

	    "Body_K_2xN:                            		 			\n"

	    " lsr 	x22, x5, #2                 		 				\n"     // K / 4
	    " KERNEL2x8_BEGIN_K_2xN                		 	 			\n"
	    " subs  x22, x22, #1                		 				\n"
	    " b 	K1_3_2xN                            	 			\n"

	    "Main_K_2xN:                            					\n"

	    " KERNEL2x8_K0_2xN                     		 				\n"
	    "K1_3_2xN:                              					\n"

	    " KERNEL2x8_K1_2xN                     		 				\n"
	    " KERNEL2x8_K0_2xN                     		 				\n"

	    " beq   Edge_K_2xN                      					\n"

	    " KERNEL2x8_K1_2xN                     		 				\n"
	    " subs  x22, x22, #1                						\n"
	    " b     Main_K_2xN                      					\n"

	    "Edge_K_2xN:                            					\n"

	    " KERNEL2x8_END_K_2xN                 						\n"

	    "	cmp		x8, #0							 				\n"
	    "	beq		M2N8_SAVE_2xN 					 	 			\n"
	    "	SAVE_ADD_C_2xN 							 	 			\n"
	    "M2N8_SAVE_2xN: 							     			\n"
	    "	SAVE2x8_2xN								 	 			\n"

	    " bgt		BEGIN_N8_2xN                					\n"


	    "	ands	x21, x4, #7 									\n"
	    "	beq		N_END_2xN 										\n"
	    "	cmp		x21, #1 										\n"
	    "	blt		N_END_2xN 										\n"

	    " 	mov		x24, x10                    					\n"
	    " 	prfm	PLDL1KEEP, [x24, #128]    		 				\n"

	    "	dup		v24.4s, wzr 									\n"
	    "	dup		v25.4s, wzr 									\n"
	   	"	mov 	x22, x5 										\n"
		"Body_K_N1_2xN:								 				\n"
		"	subs	x22, x22, #1		     		 				\n"
		"	ld2		{v0.s, v1.s}[0], [x24], #8						\n"
	    "	ldr 	s2, [x11], #4 									\n"

	    "	fmadd 	s24, s2, s0, s24								\n"
	    "	fmadd 	s25, s2, s1, s25								\n"

	   	"	bgt		Body_K_N1_2xN									\n"

	    "	cmp		x8, #0							 				\n"
	    "	beq		M2N1_SAVE_2xN 					 				\n"
	    "	SAVE_ADD_C_N2_2xN 								 		\n"
	    "M2N1_SAVE_2xN: 							 				\n"
	    "	SAVE2x1_2xN								 				\n"

	    "N8_END_2xN:									 			\n"
	    "N_END_2xN: 												\n"

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
	    [SA] "m" (SA),
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

void SMM_NN_KERNEL4xN(float *C, float *A, float *B, long	M, long N, long K,
                      long LN, long LK, float *SA, long k_tag)
{
	asm volatile(

		".macro	PACK_KERNEL4x8_BEGIN_K_4xN	 						\n"


		"	ldp 	q8, q9, [x11], #32 								\n"
	    "	ldr 	q0, [x12], #16 									\n"

	    "	fmul	v24.4s, v8.4s, v0.s[0] 				 			\n"
	    "	ldr 	q1, [x13], #16 									\n"
	    "	fmul	v25.4s, v9.4s, v0.s[0] 				 			\n"

	    "	prfm	PLDL1KEEP, [x12, #128]							\n"

	    "	ldr 	q2, [x14], #16 									\n"
	    "	fmul	v26.4s, v8.4s, v1.s[0] 				 			\n"
	    "	ldr 	q3, [x15], #16 									\n"
	    "	fmul	v27.4s, v9.4s, v1.s[0] 				  			\n"

	    "	prfm	PLDL1KEEP, [x13, #128]							\n"
	    "	fmul	v28.4s, v8.4s, v2.s[0] 				 			\n"
	    "	fmul	v29.4s, v9.4s, v2.s[0] 				 			\n"

	    "	ldr		q10, [x11], #16 								\n"
	    "	prfm	PLDL1KEEP, [x14, #128]							\n"
	    "	fmul	v30.4s, v8.4s, v3.s[0] 				 			\n"
	    "	fmul	v31.4s, v9.4s, v3.s[0] 				 			\n"

	    "	ldr		q11, [x11], #16 								\n"
	    "	prfm	PLDL1KEEP, [x15, #128]							\n"
	    "	st4		{v0.s, v1.s, v2.s, v3.s}[0], [x24], #16			\n"	    
	    "	prfm	PLDL1KEEP, [x17, #128]							\n"
	    "	ldr		q4, [x12], #16 									\n"

		".endm 														\n"


//----------------------------------------------------------------------
		".macro	PACK_KERNEL4x8_K0_4xN								\n"

		"	prfm	PLDL1KEEP, [x12, #128]							\n"
		"	ldr  	q10, [x11], #16 								\n"
		"	fmla	v24.4s, v8.4s, v0.s[0]							\n"
		"	fmla	v25.4s, v9.4s, v0.s[0] 							\n"

		"	ldr  	q11, [x11], #16 								\n"
		"	fmla	v26.4s, v8.4s, v1.s[0]							\n"
		"	fmla	v27.4s, v9.4s, v1.s[0] 							\n"

		"	ldr 	q4, [x12], #16 									\n"
		"	fmla	v28.4s, v8.4s, v2.s[0]							\n"
		"	fmla	v29.4s, v9.4s, v2.s[0] 							\n"

		"	st4		{v0.s, v1.s, v2.s, v3.s}[0], [x24], #16			\n"

		"	fmla	v30.4s, v8.4s, v3.s[0]							\n"
		"	fmla	v31.4s, v9.4s, v3.s[0] 							\n"

	    ".endm 												  		\n"



	    ".macro	PACK_KERNEL4x8_K1_4xN								\n"

		"	prfm	PLDL1KEEP, [x13, #128]							\n"
		"	ldr  	q8, [x11], #16 									\n"
		"	fmla	v24.4s, v10.4s, v0.s[1]							\n"
		"	fmla	v25.4s, v11.4s, v0.s[1] 						\n"

		"	ldr  	q9, [x11], #16 									\n"
		"	fmla	v26.4s, v10.4s, v1.s[1]							\n"
		"	fmla	v27.4s, v11.4s, v1.s[1] 						\n"

		"	ldr 	q5, [x13], #16 									\n"
		"	fmla	v28.4s, v10.4s, v2.s[1]							\n"
		"	fmla	v29.4s, v11.4s, v2.s[1] 						\n"

		"	st4		{v0.s, v1.s, v2.s, v3.s}[1], [x24], #16			\n"

		"	fmla	v30.4s, v10.4s, v3.s[1]							\n"
		"	fmla	v31.4s, v11.4s, v3.s[1] 						\n"

	    ".endm 												  		\n"


	    ".macro	PACK_KERNEL4x8_K2_4xN								\n"

		"	prfm	PLDL1KEEP, [x14, #128]							\n"
		"	ldr  	q10, [x11], #16 								\n"
		"	fmla	v24.4s, v8.4s, v0.s[2]							\n"
		"	fmla	v25.4s, v9.4s, v0.s[2] 							\n"

		"	ldr  	q11, [x11], #16 								\n"
		"	fmla	v26.4s, v8.4s, v1.s[2]							\n"
		"	fmla	v27.4s, v9.4s, v1.s[2] 							\n"

		"	ldr 	q6, [x14], #16 									\n"
		"	fmla	v28.4s, v8.4s, v2.s[2]							\n"
		"	fmla	v29.4s, v9.4s, v2.s[2] 							\n"

		"	st4		{v0.s, v1.s, v2.s, v3.s}[2], [x24], #16			\n"

		"	fmla	v30.4s, v8.4s, v3.s[2]							\n"
		"	fmla	v31.4s, v9.4s, v3.s[2] 							\n"

	    ".endm 												  		\n"



	    ".macro	PACK_KERNEL4x8_K3_4xN								\n"

		"	prfm	PLDL1KEEP, [x15, #128]							\n"
		"	ldr  	q8, [x11], #16 									\n"
		"	fmla	v24.4s, v10.4s, v0.s[3]							\n"
		"	fmla	v25.4s, v11.4s, v0.s[3] 						\n"

		"	ldr  	q9, [x11], #16 									\n"
		"	fmla	v26.4s, v10.4s, v1.s[3]							\n"
		"	fmla	v27.4s, v11.4s, v1.s[3] 						\n"

		"	ldr 	q7, [x15], #16 									\n"
		"	fmla	v28.4s, v10.4s, v2.s[3]							\n"
		"	fmla	v29.4s, v11.4s, v2.s[3] 						\n"

		"	st4		{v0.s, v1.s, v2.s, v3.s}[3], [x24], #16			\n"

		"	fmla	v30.4s, v10.4s, v3.s[3]							\n"
		"	fmla	v31.4s, v11.4s, v3.s[3] 						\n"

	    ".endm 												  		\n"



//-------------------------------------------------------------------------------


		".macro	PACK_KERNEL4x8_K4_4xN								\n"

		"	prfm	PLDL1KEEP, [x12, #128]							\n"
		"	ldr  	q10, [x11], #16 								\n"
		"	fmla	v24.4s, v8.4s, v4.s[0]							\n"
		"	fmla	v25.4s, v9.4s, v4.s[0] 							\n"

		"	ldr  	q11, [x11], #16 								\n"
		"	fmla	v26.4s, v8.4s, v5.s[0]							\n"
		"	fmla	v27.4s, v9.4s, v5.s[0] 							\n"

		"	ldr 	q0, [x12], #16 									\n"
		"	fmla	v28.4s, v8.4s, v6.s[0]							\n"
		"	fmla	v29.4s, v9.4s, v6.s[0] 							\n"

		"	st4		{v4.s, v5.s, v6.s, v7.s}[0], [x24], #16			\n"

		"	fmla	v30.4s, v8.4s, v7.s[0]							\n"
		"	fmla	v31.4s, v9.4s, v7.s[0] 							\n"

	    ".endm 												  		\n"



	    ".macro	PACK_KERNEL4x8_K5_4xN								\n"

		"	prfm	PLDL1KEEP, [x13, #128]							\n"
		"	ldr  	q8, [x11], #16 									\n"
		"	fmla	v24.4s, v10.4s, v4.s[1]							\n"
		"	fmla	v25.4s, v11.4s, v4.s[1] 						\n"

		"	ldr  	q9, [x11], #16 									\n"
		"	fmla	v26.4s, v10.4s, v5.s[1]							\n"
		"	fmla	v27.4s, v11.4s, v5.s[1] 						\n"

		"	ldr 	q1, [x13], #16 									\n"
		"	fmla	v28.4s, v10.4s, v6.s[1]							\n"
		"	fmla	v29.4s, v11.4s, v6.s[1] 						\n"

		"	st4		{v4.s, v5.s, v6.s, v7.s}[1], [x24], #16			\n"

		"	fmla	v30.4s, v10.4s, v7.s[1]							\n"
		"	fmla	v31.4s, v11.4s, v7.s[1] 						\n"

	    ".endm 												  		\n"


	    ".macro	PACK_KERNEL4x8_K6_4xN								\n"

		"	prfm	PLDL1KEEP, [x14, #128]							\n"
		"	ldr  	q10, [x11], #16 								\n"
		"	fmla	v24.4s, v8.4s, v4.s[2]							\n"
		"	fmla	v25.4s, v9.4s, v4.s[2] 							\n"

		"	ldr  	q11, [x11], #16 								\n"
		"	fmla	v26.4s, v8.4s, v5.s[2]							\n"
		"	fmla	v27.4s, v9.4s, v5.s[2] 							\n"

		"	ldr 	q2, [x14], #16 									\n"
		"	fmla	v28.4s, v8.4s, v6.s[2]							\n"
		"	fmla	v29.4s, v9.4s, v6.s[2] 							\n"

		"	st4		{v4.s, v5.s, v6.s, v7.s}[2], [x24], #16			\n"

		"	fmla	v30.4s, v8.4s, v7.s[2]							\n"
		"	fmla	v31.4s, v9.4s, v7.s[2] 							\n"

	    ".endm 												  		\n"



	    ".macro	PACK_KERNEL4x8_K7_4xN								\n"

		"	prfm	PLDL1KEEP, [x15, #128]							\n"
		"	ldr  	q8, [x11], #16 									\n"
		"	fmla	v24.4s, v10.4s, v4.s[3]							\n"
		"	fmla	v25.4s, v11.4s, v4.s[3] 						\n"

		"	ldr  	q9, [x11], #16 									\n"
		"	fmla	v26.4s, v10.4s, v5.s[3]							\n"
		"	fmla	v27.4s, v11.4s, v5.s[3] 						\n"

		"	ldr 	q3, [x15], #16 									\n"
		"	fmla	v28.4s, v10.4s, v6.s[3]							\n"
		"	fmla	v29.4s, v11.4s, v6.s[3] 						\n"

		"	st4		{v4.s, v5.s, v6.s, v7.s}[3], [x24], #16			\n"

		"	fmla	v30.4s, v10.4s, v7.s[3]							\n"
		"	fmla	v31.4s, v11.4s, v7.s[3] 						\n"

	    ".endm 												  		\n"



	    ".macro	PACK_KERNEL4x8_END_K_4xN							\n"

	    "	mov 	x25, x0   						 				\n"   //C1*
	    "	prfm	PLDL2KEEP, [x25, #64]							\n"
	    "	add		x26, x25, x9, lsl #2 			 				\n"   //C2*
	    "	prfm	PLDL2KEEP, [x26, #64]							\n"

		"	fmla	v24.4s, v10.4s, v4.s[3]							\n"
		"	fmla	v25.4s, v11.4s, v4.s[3] 						\n"
		"	fmla	v26.4s, v10.4s, v5.s[3]							\n"
		"	fmla	v27.4s, v11.4s, v5.s[3] 						\n"

		"	add 	x27, x25, x9, lsl #3 			 				\n"   //C3*
	    "	prfm	PLDL2KEEP, [x27, #64]							\n"
	    "	add 	x28, x26, x9, lsl #3 			 				\n"   //C4*
	    "	prfm	PLDL2KEEP, [x28, #64]							\n"

		"	fmla	v28.4s, v10.4s, v6.s[3]							\n"
		"	st4		{v4.s, v5.s, v6.s, v7.s}[3], [x24], #16			\n"
		"	fmla	v29.4s, v11.4s, v6.s[3] 						\n"
		"	fmla	v30.4s, v10.4s, v7.s[3]							\n"
		"	fmla	v31.4s, v11.4s, v7.s[3] 						\n"

	    ".endm 												  		\n"

//---------------------------------------------------------------------------------


	    ".macro	SAVE_ADD_C_4xN										\n"

	    "	ldp		q0, q1, [x25]									\n"
	    "	fadd	v24.4s, v24.4s, v0.4s							\n"
	    "	ldp		q2, q3, [x26]									\n"
	    "	fadd	v25.4s, v25.4s, v1.4s							\n"
	    "	ldp		q4, q5, [x27]									\n"
	    "	fadd	v26.4s, v26.4s, v2.4s							\n"
	    "	ldp		q6, q7, [x28]									\n"
	    "	fadd	v27.4s, v27.4s, v3.4s							\n"

	    "	fadd	v28.4s, v28.4s, v4.4s							\n"
	    "	fadd	v29.4s, v29.4s, v5.4s							\n"
	    "	fadd	v30.4s, v30.4s, v6.4s							\n"
	    "	fadd	v31.4s, v31.4s, v7.4s							\n"

	    ".endm														\n"



	    ".macro	SAVE4x8_4xN											\n"

	    "	subs	x21, x21, #1					 				\n"
	    "	stp		q24, q25, [x25], #32			 		 		\n"
	    "	prfm	PLDL2KEEP, [x25, #32]							\n"
	    "	stp		q26, q27, [x26], #32			 	    		\n"
	    "	prfm	PLDL2KEEP, [x26, #32]							\n"
	    "	stp		q28, q29, [x27], #32					 		\n"
	    "	prfm	PLDL2KEEP, [x27, #32]							\n"
	    "	stp		q30, q31, [x28], #32					 		\n"
	    "	prfm	PLDL2KEEP, [x28, #32]							\n"

	    ".endm										 				\n"



	    ".macro	KERNEL4x8_BEGIN_K_4xN 								\n"

	    "	prfm	PLDL1KEEP, [x11, #1024]							\n"
	    "	ldp		q0,	q1, [x11], #32								\n"

	    "	ldr		q4, [x24], #16									\n"
	    "	fmul	v24.4s, v0.4s, v4.s[0]			 				\n"
	    "	fmul	v25.4s, v1.4s, v4.s[0]			 				\n"

	    "	fmul	v26.4s, v0.4s, v4.s[1]			 				\n"
	    "	fmul	v27.4s, v1.4s, v4.s[1]			 				\n"

	    "	ldr		q2,	[x11], #16									\n"
	    "	fmul	v28.4s, v0.4s, v4.s[2]			 				\n"
	    "	fmul	v29.4s, v1.4s, v4.s[2]			 				\n"

	    "	ldr		q5,	[x24], #16									\n"

	    "	fmul	v30.4s, v0.4s, v4.s[3]			 				\n"
	    "	fmul	v31.4s, v1.4s, v4.s[3]			 				\n"
	    "	ldr		q3,	[x11], #16									\n"
	    "	prfm	PLDL1KEEP, [x24, #256]							\n"


	    ".endm 														\n"


	    ".macro	KERNEL4x8_K0_4xN 									\n"

	    "	ldr 	q2, [x11], #16 									\n"
	    "	fmla	v24.4s, v0.4s, v4.s[0] 							\n"
	    "	fmla	v25.4s, v1.4s, v4.s[0] 							\n"

	    "	prfm	PLDL1KEEP, [x24, #256]							\n"
		"	ldr 	q3, [x11], #16 									\n"
	    "	fmla	v26.4s, v0.4s, v4.s[1] 							\n"
	    "	fmla	v27.4s, v1.4s, v4.s[1] 							\n"

	    "	ldr		q5, [x24], #16 									\n"
	    "	fmla	v28.4s, v0.4s, v4.s[2] 							\n"
	    "	fmla	v29.4s, v1.4s, v4.s[2] 							\n"

	    "	fmla	v30.4s, v0.4s, v4.s[3] 							\n"
	    "	fmla	v31.4s, v1.4s, v4.s[3] 							\n"

	    ".endm 														\n"


	    ".macro	KERNEL4x8_K1_4xN 									\n"

	    "	ldr 	q0, [x11], #16 									\n"
	    "	fmla	v24.4s, v2.4s, v5.s[0] 							\n"
	    "	fmla	v25.4s, v3.4s, v5.s[0] 							\n"

	    "	prfm	PLDL1KEEP, [x11, #512]							\n"
		"	ldr 	q1, [x11], #16 									\n"
	    "	fmla	v26.4s, v2.4s, v5.s[1] 							\n"
	    "	fmla	v27.4s, v3.4s, v5.s[1] 							\n"

	    "	ldr		q4, [x24], #16 									\n"
	    "	fmla	v28.4s, v2.4s, v5.s[2] 							\n"
	    "	fmla	v29.4s, v3.4s, v5.s[2] 							\n"

	    "	fmla	v30.4s, v2.4s, v5.s[3] 							\n"
	    "	fmla	v31.4s, v3.4s, v5.s[3] 							\n"

	    ".endm 														\n"


	    ".macro	KERNEL4x8_END_K_4xN 								\n"


	    "	fmla	v24.4s, v2.4s, v5.s[0] 							\n"
	    "	fmla	v25.4s, v3.4s, v5.s[0] 							\n"
	    "	fmla	v26.4s, v2.4s, v5.s[1] 							\n"
	    "	fmla	v27.4s, v3.4s, v5.s[1] 							\n"
	    "	fmla	v28.4s, v2.4s, v5.s[2] 							\n"
	    "	fmla	v29.4s, v3.4s, v5.s[2] 							\n"
	    "	fmla	v30.4s, v2.4s, v5.s[3] 							\n"
	    "	fmla	v31.4s, v3.4s, v5.s[3] 							\n"

	    ".endm 														\n"


//---------------------------------------------------------------------------

		"SMM_NN_KERNEL4xN:											\n"

	    "	ldr		x0, %[C]						 				\n"
	    "	ldr		x1, %[A]						 				\n"
	    "	ldr		x2, %[B]						 				\n"

	    "	ldr		x3, %[M]						 				\n"
	    "	ldr		x4, %[N]						 				\n"
	    "	ldr		x5, %[K]						 				\n"
	    "	ldr		x9, %[LN] 						 				\n"
	    "	ldr		x6, %[LK]						 				\n"

	    "	prfm	PLDL1KEEP, [x1, #512]           				\n"
	    "	prfm	PLDL1KEEP, [x2, #64]          					\n"


	    "	ldr		x10, %[SA]						 				\n"
	    "	ldr		x8, %[k_tag] 					 				\n"
//---------------------------------------------------------------------------------

	    "	mov		x11, x2 										\n"		//address_B
	    "	lsr		x21, x4, #3						 				\n"    // N / 8

	    //--------------------------------------------------------------PACK

	    "BEGIN_PACKA_NN_4xN:								 		\n"

	    "	mov		x24, x10						 				\n"   //还原SA的地址

	    "	prfm	PLDL1KEEP, [x11, #256]			 				\n"

	    "	mov		x12, x1						 	 				\n"   //A0*
	    "	add		x13, x12, x6, lsl #2 			 				\n"
	    "	add		x14, x12, x6, lsl #3 			 				\n"
	    "	add		x15, x13, x6, lsl #3 			 				\n"

	    "	prfm	PLDL1KEEP, [x12, #64]			 				\n"
	    "	prfm	PLDL1KEEP, [x13, #64]			 				\n"
	    "	prfm	PLDL1KEEP, [x14, #64]			 				\n"
	    "	prfm	PLDL1KEEP, [x15, #64]			 				\n"

	    "PACK_Body_K_4xN:								 			\n"

	    "	lsr		x22, x5, #3 					 				\n"   // K / 8
	    "	PACK_KERNEL4x8_BEGIN_K_4xN				 	 			\n" 

	    "	subs	x22, x22, #1		     		 				\n"
	    "	b 		PACK_K1_7_4xN						 			\n"

	    "PACK_K_4xN:									 			\n"

	    "	PACK_KERNEL4x8_K0_4xN						 			\n"
	    "PACK_K1_7_4xN:									 			\n"

	    "	PACK_KERNEL4x8_K1_4xN						 			\n"
	    "	PACK_KERNEL4x8_K2_4xN						 			\n"
	    "	PACK_KERNEL4x8_K3_4xN						 			\n"
	    "	PACK_KERNEL4x8_K4_4xN								 	\n"
	    "	PACK_KERNEL4x8_K5_4xN						 			\n"
	    "	PACK_KERNEL4x8_K6_4xN						 			\n"

	    "	beq		PACK_Edge_K_4xN						 			\n"

	    "	PACK_KERNEL4x8_K7_4xN						 		 	\n"

	    "	subs	x22, x22, #1			 		 				\n"
	    "	b 		PACK_K_4xN			 				 			\n"

	    "PACK_Edge_K_4xN:							     			\n"
	    "	PACK_KERNEL4x8_END_K_4xN			         			\n"

	    "	cmp		x8, #0							 				\n"
	    "	beq		M4N8_PACK_SAVE_4xN 					 			\n"
	    "	SAVE_ADD_C_4xN 								 			\n"
	    "M4N8_PACK_SAVE_4xN: 							 			\n"
	    "	SAVE4x8_4xN								 				\n"

	    "END_PACKA_4xN:									 			\n"
	    "	beq		N8_END_4xN							 			\n"


//--------------------------------------------------------------
//--------------------------------------------------------------



	    "BEGIN_N8_4xN:                          		 			\n"

	    " mov		x24, x10                    					\n"
	    " prfm	PLDL1KEEP, [x24, #256]    		 					\n"
	    " prfm	PLDL1KEEP, [x11, #256]    		 					\n"

	    "Body_K_4xN:                            		 			\n"

	    " lsr 	x22, x5, #2                 		 				\n"     // K / 8
	    " KERNEL4x8_BEGIN_K_4xN                		 	 			\n"
	    " subs  x22, x22, #1                		 				\n"
	    " b 	K1_3_4xN                            	 			\n"

	    "Main_K_4xN:                            					\n"

	    " KERNEL4x8_K0_4xN                     		 				\n"
	    "K1_3_4xN:                              					\n"

	    " KERNEL4x8_K1_4xN                     		 				\n"
	    " KERNEL4x8_K0_4xN                     		 				\n"

	    " beq   Edge_K_4xN                      					\n"

	    " KERNEL4x8_K1_4xN                     		 				\n"
	    " subs  x22, x22, #1                						\n"
	    " b     Main_K_4xN                      					\n"

	    "Edge_K_4xN:                            					\n"

	    " KERNEL4x8_END_K_4xN                 						\n"

	    "	cmp		x8, #0							 				\n"
	    "	beq		M4N8_SAVE_4xN 					 	 			\n"
	    "	SAVE_ADD_C_4xN 							 	 			\n"
	    "M4N8_SAVE_4xN: 							     			\n"
	    "	SAVE4x8_4xN								 	 			\n"

	    " bgt		BEGIN_N8_4xN                					\n"

	    "N8_END_4xN:									 			\n"
	    "N_END_4xN: 												\n"

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
	    [SA] "m" (SA),
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



void SMM_NN_KERNEL8xN(float *C, float *A, float *B, long	M, long N, long K,
                      long LN, long LK, float *SA, long k_tag)
{
	asm volatile(


	    ".macro	PACK_KERNEL8x8_BEGIN_K								\n"

	    "	ldp 	q8, q9, [x11], #32 								\n"
	    "	ldr 	q0, [x12], #16 									\n"

	    "	fmul	v16.4s, v8.4s, v0.s[0] 				 			\n"
	    "	ldr 	q1, [x13], #16 									\n"
	    "	fmul	v17.4s, v9.4s, v0.s[0] 				 			\n"

	    "	prfm	PLDL1KEEP, [x12, #128]							\n"

	    "	ldr 	q2, [x14], #16 									\n"
	    "	fmul	v18.4s, v8.4s, v1.s[0] 				 			\n"
	    "	ldr 	q3, [x15], #16 									\n"
	    "	fmul	v19.4s, v9.4s, v1.s[0] 				 			\n"

	    "	prfm	PLDL1KEEP, [x13, #128]							\n"

	    "	ldr 	q4, [x16], #16 									\n"
	    "	fmul	v20.4s, v8.4s, v2.s[0] 				 			\n"
	    "	ldr 	q5, [x17], #16 									\n"
	    "	fmul	v21.4s, v9.4s, v2.s[0] 				 			\n"

	    "	prfm	PLDL1KEEP, [x14, #128]							\n"

	    "	ldr 	q6, [x18], #16 									\n"
	    "	fmul	v22.4s, v8.4s, v3.s[0] 				 			\n"
	    "	ldr 	q7, [x19], #16 									\n"
	    "	fmul	v23.4s, v9.4s, v3.s[0] 				 			\n"

	    "	prfm	PLDL1KEEP, [x15, #128]							\n"
	    "	fmul	v24.4s, v8.4s, v4.s[0] 				 			\n"
	    "	fmul	v25.4s, v9.4s, v4.s[0] 				 			\n"

	    "	st4		{v0.s, v1.s, v2.s, v3.s}[0], [x24], #16			\n"

	    "	ldr		q14, [x11], #16 								\n"
	    "	prfm	PLDL1KEEP, [x16, #128]							\n"
	    "	fmul	v26.4s, v8.4s, v5.s[0] 				 			\n"
	    "	fmul	v27.4s, v9.4s, v5.s[0] 				 			\n"

	    "	ldr		q15, [x11], #16 								\n"
	    "	prfm	PLDL1KEEP, [x17, #128]							\n"

	    "	fmul	v28.4s, v8.4s, v6.s[0] 				 			\n"
	    "	fmul	v29.4s, v9.4s, v6.s[0] 				 			\n"

	    "	ldr		q10, [x12], #16 								\n"
	    "	prfm	PLDL1KEEP, [x18, #128]							\n"
	    "	fmul	v30.4s, v8.4s, v7.s[0] 				 			\n"
	    "	fmul	v31.4s, v9.4s, v7.s[0] 				 			\n"
	    "	st4		{v4.s, v5.s, v6.s, v7.s}[0], [x24], #16			\n"

	    ".endm 														\n"



	    ".macro	PACK_KERNEL8x8_K0									\n"

	    "	prfm	PLDL1KEEP, [x12, #64]							\n"
	    "	ldr 	q14, [x11], #16 								\n"
	    "	fmla	v22.4s, v8.4s, v3.s[0] 				 			\n"
	    "	fmla	v23.4s, v9.4s, v3.s[0] 				 			\n"

	    "	ldr 	q15, [x11], #16 								\n"
	    "	fmla	v24.4s, v8.4s, v4.s[0] 				 			\n"
	    "	fmla	v25.4s, v9.4s, v4.s[0] 				 			\n"

	    "	prfm	PLDL1KEEP, [x13, #64]							\n"
	    "	fmla	v26.4s, v8.4s, v5.s[0] 				 			\n"
	    "	fmla	v27.4s, v9.4s, v5.s[0] 				 			\n"

	    "	ldr		q10, [x12], #16 								\n"
	    "	fmla	v28.4s, v8.4s, v6.s[0] 				 			\n"
	    "	fmla	v29.4s, v9.4s, v6.s[0] 				 			\n"
	    "	st4		{v0.s, v1.s, v2.s, v3.s}[0], [x24], #16			\n"

	    "	fmla	v30.4s, v8.4s, v7.s[0] 				 			\n"
	    "	fmla	v31.4s, v9.4s, v7.s[0] 				 			\n"

	    "	fmla	v16.4s, v8.4s, v0.s[0] 				 			\n"
	    "	fmla	v17.4s, v9.4s, v0.s[0] 				 			\n"

	    "	fmla	v18.4s, v8.4s, v1.s[0] 				 			\n"
	    "	fmla	v19.4s, v9.4s, v1.s[0] 				 			\n"

	    "	fmla	v20.4s, v8.4s, v2.s[0] 				 			\n"
	    "	fmla	v21.4s, v9.4s, v2.s[0] 				 			\n"
	    "	st4		{v4.s, v5.s, v6.s, v7.s}[0], [x24], #16			\n"

	    ".endm 												  		\n"



	    ".macro	PACK_KERNEL8x8_K1									\n"

	    "	prfm	PLDL1KEEP, [x14, #64]							\n"
	    "	ldr 	q8, [x11], #16 									\n"

	    "	fmla	v16.4s, v14.4s, v0.s[1] 				 		\n"
	    "	fmla	v17.4s, v15.4s, v0.s[1] 				 		\n"

	    "	ldr 	q9, [x11], #16 									\n"
	    "	fmla	v18.4s, v14.4s, v1.s[1] 				 		\n"
	    "	fmla	v19.4s, v15.4s, v1.s[1] 				 		\n"

	    "	ldr		q11, [x13], #16 								\n"
	    "	fmla	v20.4s, v14.4s, v2.s[1] 				 		\n"
	    "	fmla	v21.4s, v15.4s, v2.s[1] 				 		\n"

	    "	st4		{v0.s, v1.s, v2.s, v3.s}[1], [x24], #16			\n"

	    "	fmla	v22.4s, v14.4s, v3.s[1] 				 		\n"
	    "	fmla	v23.4s, v15.4s, v3.s[1] 				 		\n"

	    "	prfm	PLDL1KEEP, [x15, #64]							\n"

	    "	fmla	v24.4s, v14.4s, v4.s[1] 				 		\n"
	    "	fmla	v25.4s, v15.4s, v4.s[1] 				 		\n"

	    "	prfm	PLDL1KEEP, [x11, #512]							\n"

	    "	fmla	v26.4s, v14.4s, v5.s[1] 				 		\n"
	    "	fmla	v27.4s, v15.4s, v5.s[1] 				 		\n"

	    "	fmla	v28.4s, v14.4s, v6.s[1] 				 		\n"
	    "	fmla	v29.4s, v15.4s, v6.s[1] 				 		\n"

	    "	prfm	PLDL1KEEP, [x11, #576]							\n"

	    "	fmla	v30.4s, v14.4s, v7.s[1] 				 		\n"
	    "	fmla	v31.4s, v15.4s, v7.s[1] 				 		\n"
	    "	st4		{v4.s, v5.s, v6.s, v7.s}[1], [x24], #16			\n"

	    ".endm 												  		\n"


	    ".macro	PACK_KERNEL8x8_K2									\n"


	    "	prfm	PLDL1KEEP, [x16, #64]							\n"
	    "	ldr 	q14, [x11], #16 								\n"

	    "	fmla	v16.4s, v8.4s, v0.s[2] 				 			\n"
	    "	fmla	v17.4s, v9.4s, v0.s[2] 				 			\n"

	    "	ldr 	q15, [x11], #16 								\n"
	    "	fmla	v18.4s, v8.4s, v1.s[2] 				 			\n"
	    "	fmla	v19.4s, v9.4s, v1.s[2] 				 			\n"

	    "	ldr		q12, [x14], #16 								\n"
	    "	fmla	v20.4s, v8.4s, v2.s[2] 				 			\n"
	    "	fmla	v21.4s, v9.4s, v2.s[2] 				 			\n"

	    "	st4		{v0.s, v1.s, v2.s, v3.s}[2], [x24], #16			\n"

	    "	fmla	v22.4s, v8.4s, v3.s[2] 				 			\n"
	    "	fmla	v23.4s, v9.4s, v3.s[2] 				 			\n"

	    "	prfm	PLDL1KEEP, [x17, #64]							\n"

	    "	fmla	v24.4s, v8.4s, v4.s[2] 				 			\n"
	    "	fmla	v25.4s, v9.4s, v4.s[2] 				 			\n"

	    "	fmla	v26.4s, v8.4s, v5.s[2] 				 			\n"
	    "	fmla	v27.4s, v9.4s, v5.s[2] 				 			\n"

	    "	fmla	v28.4s, v8.4s, v6.s[2] 				 			\n"
	    "	fmla	v29.4s, v9.4s, v6.s[2] 				 			\n"

	    "	fmla	v30.4s, v8.4s, v7.s[2] 				 			\n"
	    "	fmla	v31.4s, v9.4s, v7.s[2] 				 			\n"
	    "	st4		{v4.s, v5.s, v6.s, v7.s}[2], [x24], #16			\n"

	    ".endm 												  		\n"



	    ".macro	PACK_KERNEL8x8_K3									\n"


	    "	prfm	PLDL1KEEP, [x18, #64]							\n"
	    "	ldr 	q8, [x11], #16 									\n"

	    "	fmla	v16.4s, v14.4s, v0.s[3] 				 		\n"
	    "	fmla	v17.4s, v15.4s, v0.s[3] 				 		\n"

	    "	ldr 	q9, [x11], #16 									\n"
	    "	fmla	v18.4s, v14.4s, v1.s[3] 				 		\n"
	    "	fmla	v19.4s, v15.4s, v1.s[3] 				 		\n"

	    "	ldr		q13, [x15], #16 								\n"
	    "	fmla	v20.4s, v14.4s, v2.s[3] 				 		\n"
	    "	st4		{v0.s, v1.s, v2.s, v3.s}[3], [x24], #16			\n"
	    "	fmla	v21.4s, v15.4s, v2.s[3] 				 		\n"
	    "	st4		{v4.s, v5.s, v6.s, v7.s}[3], [x24], #16			\n"

	    "	fmla	v22.4s, v14.4s, v3.s[3] 				 			\n"
	    "	fmla	v23.4s, v15.4s, v3.s[3] 				 			\n"
	    "	ldr 	q0, [x16], #16 												\n"

	    "	prfm	PLDL1KEEP, [x19, #64]									\n"
	    "	fmla	v24.4s, v14.4s, v4.s[3] 				 			\n"
	    "	fmla	v25.4s, v15.4s, v4.s[3] 				 			\n"
	    "	ldr 	q1, [x17], #16 												\n"

	    "	prfm	PLDL1KEEP, [x11, #512]								\n"
	    "	fmla	v26.4s, v14.4s, v5.s[3] 				 			\n"
	    "	fmla	v27.4s, v15.4s, v5.s[3] 				 			\n"
	    "	ldr 	q2, [x18], #16 												\n"

	    "	fmla	v28.4s, v14.4s, v6.s[3] 				 			\n"
	    "	fmla	v29.4s, v15.4s, v6.s[3] 				 			\n"
	    "	ldr 	q3, [x19], #16 												\n"

	    "	prfm	PLDL1KEEP, [x11, #576]								\n"
	    "	fmla	v30.4s, v14.4s, v7.s[3] 				 			\n"
	    "	fmla	v31.4s, v15.4s, v7.s[3] 				 			\n"

	    ".endm 												  			\n"



	    ".macro	PACK_KERNEL8x8_K4										\n"


	    "	ldr 	q14, [x11], #16 									\n"
	    "	fmla	v16.4s, v8.4s, v10.s[0] 				 			\n"
	    "	fmla	v17.4s, v9.4s, v10.s[0] 				 			\n"

	    "	prfm	PLDL1KEEP, [x12, #64]								\n"

	    "	ldr 	q15, [x11], #16 									\n"
	    "	fmla	v18.4s, v8.4s, v11.s[0] 				 			\n"
	    "	fmla	v19.4s, v9.4s, v11.s[0] 				 			\n"

	    "	ldr		q4, [x16], #16 										\n"

	    "	fmla	v20.4s, v8.4s, v12.s[0] 				 			\n"
	    "	fmla	v21.4s, v9.4s, v12.s[0] 				 			\n"

	    "	prfm	PLDL1KEEP, [x13, #64]								\n"
	    "	fmla	v22.4s, v8.4s, v13.s[0] 				 			\n"
	    "	fmla	v23.4s, v9.4s, v13.s[0] 				 			\n"

	    "	st4		{v10.s, v11.s, v12.s, v13.s}[0], [x24], #16			\n"

	    "	fmla	v24.4s, v8.4s, v0.s[0] 				 				\n"
	    "	fmla	v25.4s, v9.4s, v0.s[0] 				 				\n"

	    "	fmla	v26.4s, v8.4s, v1.s[0] 				 				\n"
	    "	fmla	v27.4s, v9.4s, v1.s[0] 				 				\n"

	    "	fmla	v28.4s, v8.4s, v2.s[0] 				 				\n"
	    "	fmla	v29.4s, v9.4s, v2.s[0] 				 				\n"

	    "	fmla	v30.4s, v8.4s, v3.s[0] 				 				\n"
	    "	fmla	v31.4s, v9.4s, v3.s[0] 				 				\n"

	    "	st4		{v0.s, v1.s, v2.s, v3.s}[0], [x24], #16				\n"

	    ".endm 															\n"



	    ".macro	PACK_KERNEL8x8_K5										\n"

	    "	ldr 	q8, [x11], #16 										\n"
	    "	fmla	v16.4s, v14.4s, v10.s[1] 				 			\n"
	    "	fmla	v17.4s, v15.4s, v10.s[1] 				 			\n"

	    "	prfm	PLDL1KEEP, [x14, #64]								\n"

	    "	ldr 	q9, [x11], #16 										\n"
	    "	fmla	v18.4s, v14.4s, v11.s[1] 				 			\n"
	    "	fmla	v19.4s, v15.4s, v11.s[1] 				 			\n"

	    "	ldr		q5, [x17], #16 										\n"
	    "	fmla	v20.4s, v14.4s, v12.s[1] 				 			\n"
	    "	fmla	v21.4s, v15.4s, v12.s[1] 				 			\n"

	    "	prfm	PLDL1KEEP, [x15, #64]								\n"
	    "	fmla	v22.4s, v14.4s, v13.s[1] 				 			\n"
	    "	fmla	v23.4s, v15.4s, v13.s[1] 				 			\n"

	    "	st4		{v10.s, v11.s, v12.s, v13.s}[1], [x24], #16			\n"
	    "	fmla	v24.4s, v14.4s, v0.s[1] 				 			\n"
	    "	fmla	v25.4s, v15.4s, v0.s[1] 				 			\n"

	    "	prfm	PLDL1KEEP, [x11, #512]								\n"
	    "	fmla	v26.4s, v14.4s, v1.s[1] 				 			\n"
	    "	fmla	v27.4s, v15.4s, v1.s[1] 				 			\n"

	    "	fmla	v28.4s, v14.4s, v2.s[1] 				 			\n"
	    "	fmla	v29.4s, v15.4s, v2.s[1] 				 			\n"

	    "	prfm	PLDL1KEEP, [x11, #576]								\n"
	    "	fmla	v30.4s, v14.4s, v3.s[1] 				 			\n"
	    "	fmla	v31.4s, v15.4s, v3.s[1] 				 			\n"

	    "	st4		{v0.s, v1.s, v2.s, v3.s}[1], [x24], #16				\n"

	    ".endm 															\n"



	    ".macro	PACK_KERNEL8x8_K6										\n"

	    "	ldr 	q14, [x11], #16 									\n"
	    "	fmla	v16.4s, v8.4s, v10.s[2] 				 			\n"
	    "	fmla	v17.4s, v9.4s, v10.s[2] 				 			\n"

	    "	prfm	PLDL1KEEP, [x16, #64]								\n"

	    "	ldr 	q15, [x11], #16 									\n"
	    "	fmla	v18.4s, v8.4s, v11.s[2] 				 			\n"
	    "	fmla	v19.4s, v9.4s, v11.s[2] 				 			\n"

	    "	ldr		q6, [x18], #16 										\n"
	    "	fmla	v20.4s, v8.4s, v12.s[2] 				 			\n"
	    "	fmla	v21.4s, v9.4s, v12.s[2] 				 			\n"

	    "	prfm	PLDL1KEEP, [x17, #64]								\n"
	    "	fmla	v22.4s, v8.4s, v13.s[2] 				 			\n"
	    "	fmla	v23.4s, v9.4s, v13.s[2] 				 			\n"

	    "	st4		{v10.s, v11.s, v12.s, v13.s}[2], [x24], #16			\n"
	    "	fmla	v24.4s, v8.4s, v0.s[2] 				 				\n"
	    "	fmla	v25.4s, v9.4s, v0.s[2] 				 				\n"

	    "	fmla	v26.4s, v8.4s, v1.s[2] 				 				\n"
	    "	fmla	v27.4s, v9.4s, v1.s[2] 				 				\n"

	    "	fmla	v28.4s, v8.4s, v2.s[2] 				 				\n"
	    "	fmla	v29.4s, v9.4s, v2.s[2] 				 				\n"

	    "	fmla	v30.4s, v8.4s, v3.s[2] 				 				\n"
	    "	fmla	v31.4s, v9.4s, v3.s[2] 				 				\n"

	    "	st4		{v0.s, v1.s, v2.s, v3.s}[2], [x24], #16				\n"

	    ".endm 															\n"



	    ".macro	PACK_KERNEL8x8_K7										\n"


	    "	add 	x24, x24, #16 										\n"
	    "	ldr 	q8, [x11], #16 										\n"
	    "	fmla	v24.4s, v14.4s, v0.s[3] 				 			\n"
	    "	st4		{v0.s, v1.s, v2.s, v3.s}[3], [x24]					\n"
	    "	fmla	v25.4s, v15.4s, v0.s[3] 				 			\n"
	    "	ldr		q0, [x12], #16 										\n"

	    "	prfm	PLDL1KEEP, [x18, #64]								\n"
	    "	ldr 	q9, [x11], #16 										\n"
	    "	fmla	v26.4s, v14.4s, v1.s[3] 				 			\n"
	    "	fmla	v27.4s, v15.4s, v1.s[3] 				 			\n"

	    "	sub 	x24, x24, #16 										\n"
	    "	ldr		q1, [x13], #16 										\n"
	    "	fmla	v28.4s, v14.4s, v2.s[3] 				 			\n"
	    "	fmla	v29.4s, v15.4s, v2.s[3] 				 			\n"

	    "	prfm	PLDL1KEEP, [x19, #64]								\n"
	    "	ldr		q2, [x14], #16 										\n"
	    "	fmla	v30.4s, v14.4s, v3.s[3] 				 			\n"
	    "	fmla	v31.4s, v15.4s, v3.s[3] 				 			\n"

	    "	ldr		q3, [x15], #16 										\n"
	    "	fmla	v16.4s, v14.4s, v10.s[3] 				 			\n"
	    "	fmla	v17.4s, v15.4s, v10.s[3] 				 			\n"

	    "	ldr		q7, [x19], #16 										\n"
	    "	fmla	v18.4s, v14.4s, v11.s[3] 				 			\n"
	    "	fmla	v19.4s, v15.4s, v11.s[3] 				 			\n"

	    "	prfm	PLDL1KEEP, [x11, #512]								\n"

	    "	fmla	v20.4s, v14.4s, v12.s[3] 				 			\n"
	    "	fmla	v21.4s, v15.4s, v12.s[3] 				 			\n"

	    "	fmla	v22.4s, v14.4s, v13.s[3] 				 			\n"
	    "	fmla	v23.4s, v15.4s, v13.s[3] 				 			\n"
	    "	st4		{v10.s, v11.s, v12.s, v13.s}[3], [x24]				\n"
	    "	add  	x24, x24, #32 										\n"


	    ".endm 															\n"



	    ".macro	PACK_KERNEL8x8_END_K									\n"


	    "	mov 	x25, x0   						 					\n"   //C1*
	    "	prfm	PLDL2KEEP, [x25, #64]								\n"
	    "	add		x26, x25, x9, lsl #2 			 					\n"   //C2*
	    "	prfm	PLDL2KEEP, [x26, #64]								\n"

	    "	fmla	v24.4s, v14.4s, v0.s[3] 				 			\n"
	    "	fmla	v25.4s, v15.4s, v0.s[3] 				 			\n"

	    "	add 	x27, x25, x9, lsl #3 			 					\n"   //C3*
	    "	prfm	PLDL2KEEP, [x27, #64]								\n"
	    "	add 	x28, x26, x9, lsl #3 			 					\n"   //C4*
	    "	prfm	PLDL2KEEP, [x28, #64]								\n"

	    "	fmla	v26.4s, v14.4s, v1.s[3] 				 			\n"
	    "	fmla	v27.4s, v15.4s, v1.s[3] 				 			\n"

	    "	add 	x15, x27, x9, lsl #3 								\n"		//C5*
	    "	prfm	PLDL2KEEP, [x15, #64]								\n"

	    "	fmla	v28.4s, v14.4s, v2.s[3] 				 			\n"
	    "	fmla	v29.4s, v15.4s, v2.s[3] 				 			\n"

	    "	add 	x16, x28, x9, lsl #3 								\n"		//C6*
	    "	prfm	PLDL2KEEP, [x16, #64]								\n"

	    "	fmla	v30.4s, v14.4s, v3.s[3] 				 			\n"
	    "	fmla	v31.4s, v15.4s, v3.s[3] 				 			\n"

	    "	st4		{v10.s, v11.s, v12.s, v13.s}[3], [x24], #16			\n"

	    "	fmla	v16.4s, v14.4s, v10.s[3] 				 			\n"
	    "	fmla	v17.4s, v15.4s, v10.s[3] 				 			\n"

	    "	add 	x17, x15, x9, lsl #3 								\n"		//C7*
	    "	prfm	PLDL2KEEP, [x17, #64]								\n"

	    "	fmla	v18.4s, v14.4s, v11.s[3] 				 			\n"
	    "	fmla	v19.4s, v15.4s, v11.s[3] 				 			\n"

	    "	add 	x18, x16, x9, lsl #3 								\n"		//C8*
	    "	prfm	PLDL2KEEP, [x18, #64]								\n"

	    "	fmla	v20.4s, v14.4s, v12.s[3] 				 			\n"
	    "	fmla	v21.4s, v15.4s, v12.s[3] 				 			\n"

	    "	fmla	v22.4s, v14.4s, v13.s[3] 				 			\n"
	    "	fmla	v23.4s, v15.4s, v13.s[3] 				 			\n"

	    "	st4		{v0.s, v1.s, v2.s, v3.s}[3], [x24], #16				\n"


	    ".endm 															\n"



	    ".macro	SAVE_ADD_C												\n"

	    "	ldp		q0, q1, [x25]										\n"
	    "	fadd	v16.4s, v16.4s, v0.4s								\n"
	    "	ldp		q2, q3, [x26]										\n"
	    "	fadd	v17.4s, v17.4s, v1.4s								\n"
	    "	ldp		q4, q5, [x27]										\n"
	    "	fadd	v18.4s, v18.4s, v2.4s								\n"
	    "	ldp		q6, q7, [x28]										\n"
	    "	fadd	v19.4s, v19.4s, v3.4s								\n"

	    "	ldp		q8, q9, [x15]										\n"
	    "	fadd	v20.4s, v20.4s, v4.4s								\n"
	    "	ldp		q10, q11, [x16]										\n"
	    "	fadd	v21.4s, v21.4s, v5.4s								\n"
	    "	ldp		q12, q13, [x17]										\n"
	    "	fadd	v22.4s, v22.4s, v6.4s								\n"
	    "	ldp		q14, q15, [x18]										\n"
	    "	fadd	v23.4s, v23.4s, v7.4s								\n"

	    "	fadd	v24.4s, v24.4s, v8.4s								\n"
	    "	fadd	v25.4s, v25.4s, v9.4s								\n"
	    "	fadd	v26.4s, v26.4s, v10.4s								\n"
	    "	fadd	v27.4s, v27.4s, v11.4s								\n"
	    "	fadd	v28.4s, v28.4s, v12.4s								\n"
	    "	fadd	v29.4s, v29.4s, v13.4s								\n"
	    "	fadd	v30.4s, v30.4s, v14.4s								\n"
	    "	fadd	v31.4s, v31.4s, v15.4s								\n"


	    ".endm															\n"



	    ".macro	SAVE8x8													\n"

	    "	subs	x21, x21, #1					 					\n"

	    "	stp		q16, q17, [x25], #32			 		 			\n"
	    "	prfm	PLDL2KEEP, [x25, #32]								\n"
	    "	stp		q18, q19, [x26], #32			 	    			\n"
	    "	prfm	PLDL2KEEP, [x26, #32]								\n"
	    "	stp		q20, q21, [x27], #32					 			\n"
	    "	prfm	PLDL2KEEP, [x27, #32]								\n"
	    "	stp		q22, q23, [x28], #32					 			\n"
	    "	prfm	PLDL2KEEP, [x28, #32]								\n"

	    "	stp		q24, q25, [x15], #32			 		 			\n"
	    "	prfm	PLDL2KEEP, [x15, #32]								\n"
	    "	stp		q26, q27, [x16], #32			 	    			\n"
	    "	prfm	PLDL2KEEP, [x16, #32]								\n"
	    "	stp		q28, q29, [x17], #32					 			\n"
	    "	prfm	PLDL2KEEP, [x17, #32]								\n"
	    "	stp		q30, q31, [x18], #32					 			\n"
	    "	prfm	PLDL2KEEP, [x18, #32]								\n"

	    ".endm										 					\n"




//------------------------------------------------------ MAIN KERNEL
	    ".macro	KERNEL8x8_BEGIN_K 										\n"

	    "	prfm	PLDL1KEEP, [x11, #1024]								\n"
	    "	ldp		q0,	q1, [x11], #32									\n"

	    "	ldr		q4, [x24], #16										\n"

	    "	fmul	v16.4s, v0.4s, v4.s[0]			 					\n"
	    "	fmul	v17.4s, v1.4s, v4.s[0]			 					\n"

	    "	ldr		q5,	[x24], #16										\n"

	    "	fmul	v18.4s, v0.4s, v4.s[1]			 					\n"
	    "	fmul	v19.4s, v1.4s, v4.s[1]			 					\n"

	    "	ldr		q2,	[x11], #16										\n"

	    "	fmul	v20.4s, v0.4s, v4.s[2]			 					\n"
	    "	fmul	v21.4s, v1.4s, v4.s[2]			 					\n"

	    "	ldr		q6,	[x24], #16										\n"

	    "	fmul	v22.4s, v0.4s, v4.s[3]			 					\n"
	    "	fmul	v23.4s, v1.4s, v4.s[3]			 					\n"

	    "	ldr		q3,	[x11], #16										\n"
	    "	fmul	v24.4s, v0.4s, v5.s[0]			 					\n"
	    "	fmul	v25.4s, v1.4s, v5.s[0]			 					\n"

	    "	ldr		q7,	[x24], #16										\n"

	    "	fmul	v26.4s, v0.4s, v5.s[1]			 					\n"
	    "	fmul	v27.4s, v1.4s, v5.s[1]			 					\n"

	    "	prfm	PLDL1KEEP, [x24, #256]								\n"

	    "	fmul	v28.4s, v0.4s, v5.s[2]			 					\n"
	    "	fmul	v29.4s, v1.4s, v5.s[2]			 					\n"

	    "	fmul	v30.4s, v0.4s, v5.s[3]			 					\n"
	    "	fmul	v31.4s, v1.4s, v5.s[3]			 					\n"

	    ".endm 															\n"



	    ".macro	KERNEL8x8_K0											\n"


	    "	prfm	PLDL1KEEP, [x11, #1024]								\n"

	    "	ldr		q2, [x11], #16										\n"

	    "	fmla	v16.4s, v0.4s, v4.s[0]			 					\n"
	    "	fmla	v17.4s, v1.4s, v4.s[0]			 					\n"
	    "	ldr		q6, [x24], #16										\n"

	    "	fmla	v18.4s, v0.4s, v4.s[1]			 					\n"
	    "	fmla	v19.4s, v1.4s, v4.s[1]			 					\n"
	    "	ldr		q3, [x11], #16										\n"

	    "	fmla	v20.4s, v0.4s, v4.s[2]			 					\n"
	    "	fmla	v21.4s, v1.4s, v4.s[2]			 					\n"

	    "	ldr		q7, [x24], #16										\n"

	    "	fmla	v22.4s, v0.4s, v4.s[3]			 					\n"
	    "	fmla	v23.4s, v1.4s, v4.s[3]			 					\n"

	    "	prfm	PLDL1KEEP, [x11, #1024]								\n"

	    "	fmla	v24.4s, v0.4s, v5.s[0]			 					\n"
	    "	fmla	v25.4s, v1.4s, v5.s[0]			 					\n"
	    "	fmla	v26.4s, v0.4s, v5.s[1]			 					\n"
	    "	fmla	v27.4s, v1.4s, v5.s[1]			 					\n"
	    "	fmla	v28.4s, v0.4s, v5.s[2]			 					\n"
	    "	fmla	v29.4s, v1.4s, v5.s[2]			 					\n"
	    "	fmla	v30.4s, v0.4s, v5.s[3]			 					\n"
	    "	fmla	v31.4s, v1.4s, v5.s[3]			 					\n"


	    ".endm 										 					\n"


	    ".macro	KERNEL8x8_K1											\n"


	    "	ldr		q0, [x11], #16										\n"
	    "	fmla	v16.4s, v2.4s, v6.s[0]			 					\n"
	    "	fmla	v17.4s, v3.4s, v6.s[0]			 					\n"
	    "	ldr		q4, [x24], #16										\n"

	    "	fmla	v18.4s, v2.4s, v6.s[1]			 					\n"
	    "	fmla	v19.4s, v3.4s, v6.s[1]			 					\n"
	    "	ldr		q1, [x11], #16										\n"

	    "	fmla	v20.4s, v2.4s, v6.s[2]			 					\n"
	    "	fmla	v21.4s, v3.4s, v6.s[2]			 					\n"
	    "	ldr		q5, [x24], #16										\n"

	    "	fmla	v22.4s, v2.4s, v6.s[3]			 					\n"
	    "	fmla	v23.4s, v3.4s, v6.s[3]			 					\n"

	    "	prfm	PLDL1KEEP, [x24, #256]								\n"

	    "	fmla	v24.4s, v2.4s, v7.s[0]			 					\n"
	    "	fmla	v25.4s, v3.4s, v7.s[0]			 					\n"
	    "	fmla	v26.4s, v2.4s, v7.s[1]			 					\n"
	    "	fmla	v27.4s, v3.4s, v7.s[1]			 					\n"
	    "	fmla	v28.4s, v2.4s, v7.s[2]			 					\n"
	    "	fmla	v29.4s, v3.4s, v7.s[2]			 					\n"
	    "	fmla	v30.4s, v2.4s, v7.s[3]			 					\n"
	    "	fmla	v31.4s, v3.4s, v7.s[3]			 					\n"


	    ".endm 										 					\n"



	    ".macro	KERNEL8x8_END_K											\n"


	    "	prfm  PLDL1KEEP, [x25, #32]      							\n"
	    " 	prfm  PLDL1KEEP, [x26, #32]      							\n"
	    " 	prfm  PLDL1KEEP, [x27, #32]      							\n"
	    " 	prfm  PLDL1KEEP, [x28, #32]      							\n"

	    "	fmla	v16.4s, v2.4s, v6.s[0]			 					\n"
	    "	fmla	v17.4s, v3.4s, v6.s[0]			 					\n"
	    "	fmla	v18.4s, v2.4s, v6.s[1]			 					\n"
	    "	fmla	v19.4s, v3.4s, v6.s[1]			 					\n"
	    "	fmla	v20.4s, v2.4s, v6.s[2]			 					\n"
	    "	prfm  PLDL1KEEP, [x15, #32]      							\n"
	    " 	prfm  PLDL1KEEP, [x16, #32]      							\n"
	    "	fmla	v21.4s, v3.4s, v6.s[2]			 					\n"
	    "	fmla	v22.4s, v2.4s, v6.s[3]			 					\n"
	    "	prfm  PLDL1KEEP, [x17, #32]      							\n"
	    "	prfm  PLDL1KEEP, [x18, #32]      							\n"
	    "	fmla	v23.4s, v3.4s, v6.s[3]			 					\n"

	    "	fmla	v24.4s, v2.4s, v7.s[0]			 					\n"
	    "	fmla	v25.4s, v3.4s, v7.s[0]			 					\n"
	    "	fmla	v26.4s, v2.4s, v7.s[1]			 					\n"
	    "	fmla	v27.4s, v3.4s, v7.s[1]			 					\n"
	    "	fmla	v28.4s, v2.4s, v7.s[2]			 					\n"
	    "	fmla	v29.4s, v3.4s, v7.s[2]			 					\n"
	    "	fmla	v30.4s, v2.4s, v7.s[3]			 					\n"
	    "	fmla	v31.4s, v3.4s, v7.s[3]			 					\n"

	    ".endm 										 					\n"



	    ".macro	KERNEL8x2_K 											\n"

	    "	ldp		q0, q1, [x24], #32 									\n"
	    "	ldp		q2, q3, [x11], #32 									\n"

	    "	fmla	v28.4s, v0.4s, v2.s[0]								\n"
	    "	ldp		q4, q5, [x24], #32 									\n"
	    "	fmla	v29.4s, v0.4s, v2.s[1]								\n"
	    "	fmla	v30.4s, v1.4s, v2.s[0] 								\n"
	    "	ldp		q6, q7, [x24], #32 									\n"
	    "	fmla	v31.4s, v1.4s, v2.s[1] 								\n"

	    "	prfm	PLDL1KEEP, [x11, #64]								\n"

	    "	fmla	v28.4s, v4.4s, v2.s[2]								\n"
	    "	ldp		q8, q9, [x24], #32 									\n"
	    "	fmla	v29.4s, v4.4s, v2.s[3]								\n"
	    "	fmla	v30.4s, v5.4s, v2.s[2]								\n"
	    "	fmla	v31.4s, v5.4s, v2.s[3]								\n"

	    "	fmla	v28.4s, v6.4s, v3.s[0]								\n" 
	    "	fmla	v29.4s, v6.4s, v3.s[1]								\n"
	    "	fmla	v30.4s, v7.4s, v3.s[0]								\n"
	    "	fmla	v31.4s, v7.4s, v3.s[1]								\n"

	    "	prfm	PLDL1KEEP, [x24, #64]								\n"

	    "	fmla	v28.4s, v8.4s, v3.s[2]								\n"
	    "	fmla	v29.4s, v8.4s, v3.s[3]								\n"
	    "	fmla	v30.4s, v9.4s, v3.s[2]								\n"
	    "	fmla	v31.4s, v9.4s, v3.s[3]								\n"

	    ".endm 															\n"



	    ".macro	SAVE8x2_ADD_C  											\n"

	    "	ld2		{v0.s, v1.s}[0], [x25] 								\n"
	    "	ld2		{v0.s, v1.s}[1], [x26] 								\n"
	    "	ld2		{v0.s, v1.s}[2], [x27] 								\n"
	    "	ld2		{v0.s, v1.s}[3], [x28] 								\n"

	    "	fadd	v28.4s, v28.4s, v0.4s 								\n"
	    "	ld2		{v2.s, v3.s}[0], [x15] 								\n"
	    "	fadd  	v29.4s, v29.4s, v1.4s 								\n"
	    "	ld2		{v2.s, v3.s}[1], [x16] 								\n"
	    "	ld2		{v2.s, v3.s}[2], [x17] 								\n"
	    "	ld2		{v2.s, v3.s}[3], [x18] 								\n"

	    "	fadd	v30.4s, v30.4s, v2.4s 								\n"
	    "	fadd	v31.4s, v31.4s, v3.4s 								\n"

	    ".endm 															\n"



	    ".macro	T_SAVE8x2												\n"


	    "	st2		{v28.s, v29.s}[0], [x25]			 		 		\n"
	    "	st2 	{v28.s, v29.s}[1], [x26]							\n"
	    "	st2 	{v28.s, v29.s}[2], [x27]							\n"
	    "	st2 	{v28.s, v29.s}[3], [x28]							\n"

	    "	st2		{v30.s, v31.s}[0], [x15]			 		 		\n"
	    "	st2 	{v30.s, v31.s}[1], [x16]							\n"
	    "	st2 	{v30.s, v31.s}[2], [x17]							\n"
	    "	st2 	{v30.s, v31.s}[3], [x18]							\n"

	    ".endm										 					\n"



	    ".macro	KERNEL8x1_K 											\n"

	    "	ldp		q0, q1, [x24], #32 									\n"
	    "	ldr		q2, [x11], #16 										\n"

	    "	fmla	v30.4s, v0.4s, v2.s[0]								\n"
	    "	ldp		q3, q4, [x24], #32 									\n"
	    "	fmla	v31.4s, v1.4s, v2.s[0]								\n"

	    "	prfm	PLDL1KEEP, [x11, #64]								\n"

	    "	ldp		q5, q6, [x24], #32 									\n"
	    "	fmla	v30.4s, v3.4s, v2.s[1]								\n"
	    "	fmla	v31.4s, v4.4s, v2.s[1]								\n"

	    "	prfm	PLDL1KEEP, [x24, #64]								\n"

	    "	ldp		q7, q8, [x24], #32 									\n"
	    "	fmla	v30.4s, v5.4s, v2.s[2]								\n"
	    "	fmla	v31.4s, v6.4s, v2.s[2]								\n"

	    "	fmla	v30.4s, v7.4s, v2.s[3]								\n"
	    "	fmla	v31.4s, v8.4s, v2.s[3]								\n"

	    ".endm 															\n"



	    ".macro	SAVE8x1_ADD_C  											\n"

	    "	ld1		{v0.s}[0], [x25] 									\n"
	    "	ld1		{v0.s}[1], [x26] 									\n"
	    "	ld1		{v0.s}[2], [x27] 									\n"
	    "	ld1		{v0.s}[3], [x28] 									\n"

	    "	fadd	v30.4s, v30.4s, v0.4s 								\n"

	    "	ld1		{v1.s}[0], [x15] 									\n"
	    "	ld1		{v1.s}[1], [x16] 									\n"
	    "	ld1		{v1.s}[2], [x17] 									\n"
	    "	ld1		{v1.s}[3], [x18] 									\n"

	    "	fadd	v31.4s, v31.4s, v1.4s 								\n"

	    ".endm 															\n"



	    ".macro	T_SAVE8x1												\n"


	    "	st1		{v30.s}[0], [x25]			 		 				\n"
	    "	st1 	{v30.s}[1], [x26]									\n"
	    "	st1 	{v30.s}[2], [x27]									\n"
	    "	st1 	{v30.s}[3], [x28]									\n"


	    "	st1		{v31.s}[0], [x15]			 		 				\n"
	    "	st1 	{v31.s}[1], [x16]									\n"
	    "	st1 	{v31.s}[2], [x17]									\n"
	    "	st1 	{v31.s}[3], [x18]									\n"

	    ".endm										 					\n"





//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++





	    // the order of loop is MNK

	    "T_SMM_NN:														\n"

	    "	ldr		x0, %[C]						 					\n"
	    "	ldr		x1, %[A]						 					\n"
	    "	ldr		x2, %[B]						 					\n"

	    "	ldr		x3, %[M]						 					\n"
	    "	ldr		x4, %[N]						 					\n"
	    "	ldr		x5, %[K]						 					\n"
	    "	ldr		x9, %[LN] 						 					\n"
	    "	ldr		x6, %[LK]						 					\n"

	    "	prfm	PLDL1KEEP, [x1, #512]           					\n"
	    "	prfm	PLDL1KEEP, [x2, #64]          						\n"


	    "	ldr		x10, %[SA]						 					\n"
	    "	ldr		x8, %[k_tag] 					 					\n"

	    "	lsr		x20, x3, #3						 					\n"   // M / 8



//--------------------------------------------------------
//----------------------------------------------------LOOP

	    "T_BEGIN_M8:								 	 				\n"

	    "	mov		x11, x2 											\n"		//address_B
	    "	lsr		x21, x4, #3						 					\n"    // N / 8


	    //-----------------------------------------------------PACK

	    "BEGIN_PACKA:								 					\n"

	    "	mov		x24, x10						 					\n"   //还原SA的地址

	    "	prfm	PLDL1KEEP, [x11, #256]			 					\n"

	    "	mov		x12, x1						 	 					\n"   //A0*
	    "	add		x13, x12, x6, lsl #2 			 					\n"
	    "	add		x14, x12, x6, lsl #3 			 					\n"
	    "	add		x15, x13, x6, lsl #3 			 					\n"

	    "	prfm	PLDL1KEEP, [x12, #64]			 					\n"
	    "	prfm	PLDL1KEEP, [x13, #64]			 					\n"
	    "	prfm	PLDL1KEEP, [x14, #64]			 					\n"
	    "	prfm	PLDL1KEEP, [x15, #64]			 					\n"

	    "	add		x16, x14, x6, lsl #3 			 					\n"
	    "	add		x17, x15, x6, lsl #3 			 					\n"
	    "	add		x18, x16, x6, lsl #3 			 					\n"
	    "	add		x19, x17, x6, lsl #3 			 					\n"

	    "	prfm	PLDL1KEEP, [x16, #64]			 					\n"
	    "	prfm	PLDL1KEEP, [x17, #64]			 					\n"
	    "	prfm	PLDL1KEEP, [x18, #64]			 					\n"
	    "	prfm	PLDL1KEEP, [x19, #64]			 					\n"

	    "T_PACK_Body_K:								 					\n"

	    "	lsr		x22, x5, #3 					 					\n"   // K / 8
	    "	PACK_KERNEL8x8_BEGIN_K				 	 					\n"

	    "	subs	x22, x22, #1		     		 					\n"
	    "	b 		T_PACK_K1_7						 					\n"

	    "T_PACK_K:									 					\n"

	    "	PACK_KERNEL8x8_K0						 					\n"
	    "T_PACK_K1_7:									 				\n"

	    "	PACK_KERNEL8x8_K1						 					\n"
	    "	PACK_KERNEL8x8_K2						 					\n"
	    "	PACK_KERNEL8x8_K3						 					\n"
	    "	PACK_KERNEL8x8_K4								 			\n"
	    "	PACK_KERNEL8x8_K5						 					\n"
	    "	PACK_KERNEL8x8_K6						 					\n"

	    "	beq		T_PACK_Edge_K						 				\n"

	    "	PACK_KERNEL8x8_K7						 		 			\n"

	    "	subs	x22, x22, #1			 		 					\n"
	    "	b 		T_PACK_K			 				 				\n"

	    "T_PACK_Edge_K:							     					\n"
	    "	PACK_KERNEL8x8_END_K			         					\n"

	    "	cmp		x8, #0							 					\n"
	    "	beq		M8N8_PACK_SAVE 					 					\n"
	    "	SAVE_ADD_C 								 					\n"
	    "M8N8_PACK_SAVE: 							 					\n"
	    "	SAVE8x8								 						\n"

	    "END_PACKA:									 					\n"
	    "	beq		N8_END							 					\n"


//--------------------------------------------------------------
//--------------------------------------------------------------



	    "T_BEGIN_N8:                          		 					\n"

	    " mov		x24, x10                    						\n"
	    " prfm	PLDL1KEEP, [x24, #256]    		 						\n"
	    " prfm	PLDL1KEEP, [x11, #256]    		 						\n"

	    "T_Body_K:                            		 					\n"

	    " lsr 	x22, x5, #3                 		 					\n"     // K / 8
	    " KERNEL8x8_BEGIN_K                		 	 					\n"
	    " subs  x22, x22, #1                		 					\n"
	    " b 	T_K1_7                            	 					\n"

	    "T_Main_K:                            							\n"

	    " KERNEL8x8_K0                     		 						\n"
	    "T_K1_7:                              							\n"

	    " KERNEL8x8_K1                     		 						\n"
	    " KERNEL8x8_K0                     		 						\n"
	    " KERNEL8x8_K1                     		 	 					\n"
	    " KERNEL8x8_K0                     		 						\n"
	    " KERNEL8x8_K1                     		 						\n"
	    " KERNEL8x8_K0                     								\n"

	    " beq   T_Edge_K                      							\n"

	    " KERNEL8x8_K1                     		 						\n"
	    " subs  x22, x22, #1                							\n"
	    " b     T_Main_K                      							\n"

	    "T_Edge_K:                            							\n"

	    " KERNEL8x8_END_K                 								\n"

	    "	cmp		x8, #0							 					\n"
	    "	beq		M8N8_SAVE 					 	 					\n"
	    "	SAVE_ADD_C 							 	 					\n"
	    "M8N8_SAVE: 							     					\n"
	    "	SAVE8x8								 	 					\n"

	    " bgt		T_BEGIN_N8                							\n"

	    "N8_END:									 					\n"

	    "	ands	x21, x4, #7 										\n"
	    "	beq		N_END 												\n"

	    "BEGIN_N4:														\n"
	    "	cmp		x21, #4 											\n"
	    "	blt 	BEGIN_N2 											\n"


//-----------------------------------------------------------------------------
	    "BEGIN_N2:														\n"
	    "	cmp		x21, #2 											\n"
	    "	blt 	BEGIN_N1 											\n"
	    "	sub    	x21, x21, #2 										\n"

	    "	mov		x24, x10 											\n"			// address_A
	    " 	prfm	PLDL1KEEP, [x24, #256]    		 					\n"
	    " 	prfm	PLDL1KEEP, [x11, #256]    		 					\n"

	    "N2_Body_K:                            		 					\n"

	    " 	lsr 	x22, x5, #2                 		 				\n"     // K / 4
	    "	dup		v28.4s, wzr 										\n"
	    "	dup		v29.4s, wzr 										\n"
	    "	dup		v30.4s, wzr 										\n"
	    "	dup		v31.4s, wzr 										\n"

	    "N2_Main_K:														\n"

	    "	subs	x22, x22, #1 										\n"
	    "	KERNEL8x2_K 												\n"

	    "	bgt 	N2_Main_K											\n"

	    "	cmp		x8, #0							 					\n"
	    "	beq		T_M8N2_SAVE 					 	 				\n"
	    "	SAVE8x2_ADD_C 							 	 				\n"
	    "T_M8N2_SAVE: 							     					\n"
	    "	T_SAVE8x2								 	 				\n"




//--------------------------------------------------------------
//--------------------------------------------------------------
	    "BEGIN_N1:														\n"
	    "	cmp		x21, #1 											\n"
	    "	blt 	N_END 												\n"

//--------------------------------------------------------------

	    "	mov		x24, x10 											\n"			// address_A

	    " prfm	PLDL1KEEP, [x24, #256]    		 						\n"
	    " prfm	PLDL1KEEP, [x11, #256]    		 						\n"

	    "N1_Body_K:                            		 					\n"

	    " 	lsr 	x22, x5, #2                 		 				\n"     // K / 4
	    "	dup		v30.4s, wzr 										\n"
	    "	dup		v31.4s, wzr 										\n"

	    "N1_Main_K:														\n"

	    "	subs	x22, x22, #1 										\n"
	    "	KERNEL8x1_K 												\n"

	    "	bgt 	N1_Main_K											\n"

	    "	cmp		x8, #0							 					\n"
	    "	beq		T_M8N1_SAVE 					 	 				\n"
	    "	SAVE8x1_ADD_C 							 	 				\n"
	    "T_M8N1_SAVE: 							     					\n"
	    "	T_SAVE8x1								 	 				\n"



	    "N_END: 														\n"
	    "	subs	x20, x20, #1					 					\n"
	    "	add		x0, x0, x9, lsl #5						 			\n"
	    "	add 	x1, x1, x6, lsl #5 									\n"   // A0* + 8 * sizeof(K)
	    "	bgt		T_BEGIN_M8						 					\n"


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
	    [SA] "m" (SA),
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
