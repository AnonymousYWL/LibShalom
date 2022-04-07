#pragma once
void SMM_NN_KERNEL8x1(float *C, float *A, float *B, long M, long N, long K,
                       long LN, long LK, float *SB, long k_tag)
{

	asm volatile(

	    ".macro	PACK_KERNEL8x1_BEGIN_K							\n"

	    "	ldp		q0,	q1, [x11], #32							\n"
	    "	prfm	PLDL1KEEP, [x11, #512]						\n"

	    "	ld1 	{v2.s}[0], [x12]							\n"
	    "	add  	x12, x12, x9, lsl #3 						\n"
	    "	fmul	v30.4s, v0.4s, v2.s[0]						\n"
	    "	ld1		{v5.s}[0], [x13]							\n"
	    "	fmul	v31.4s, v1.4s, v2.s[0]						\n"

	    "	ldp		q3,	q4, [x11], #32							\n"
	    "	add  	x13, x13, x9, lsl #3 						\n"

	    "	str 	s2, [x24], #4 								\n"

	    ".endm 													\n"


	    ".macro	PACK_KERNEL8x1_K0								\n"

	    "	ldp		q3, q4, [x11], #32 							\n"
	    "	fmla	v30.4s, v0.4s, v2.s[0]						\n"
	    "	ld1 	{v5.s}[0], [x13] 							\n"
	    "	fmla	v31.4s, v1.4s, v2.s[0]						\n"
	    "	add  	x13, x13, x9, lsl #3 						\n"
	    "	str 	s2, [x24], #4 								\n"

	    ".endm 													\n"


	    ".macro	PACK_KERNEL8x1_K1								\n"

	    "	ldp		q0, q1, [x11], #32 							\n"
	    "	fmla	v30.4s, v3.4s, v5.s[0]						\n"
	    "	ld1 	{v2.s}[0], [x12] 							\n"
	    "	fmla	v31.4s, v4.4s, v5.s[0]						\n"
	    "	add  	x12, x12, x9, lsl #3 						\n"
	    "	st1   {v5.s}[0], [x24], #4 							\n"

	    ".endm 													\n"


	    ".macro	PACK_KERNEL8x1_END_K							\n"

	    "	fmla	v30.4s, v3.4s, v5.s[0]						\n"
	    "	fmla	v31.4s, v4.4s, v5.s[0]						\n"
	    "	st1   {v5.s}[0], [x24], #4 							\n"

	    ".endm 													\n"


	    ".macro	M8N1_PACK_ADD_C									\n"

	    "	ld1		{v0.s}[0], [x25] 							\n"
	    "	ld1		{v0.s}[1], [x26] 							\n"
	    "	ld1		{v0.s}[2], [x27] 							\n"
	    "	ld1		{v0.s}[3], [x28] 							\n"

	    "	fadd 	v30.4s, v30.4s, v0.4s 						\n"

	    "	ld1		{v1.s}[0], [x15] 										\n"
	    "	ld1		{v1.s}[1], [x16] 										\n"
	    "	ld1		{v1.s}[2], [x17] 										\n"
	    "	ld1		{v1.s}[3], [x18] 										\n"

	    "	fadd 	v31.4s, v31.4s, v1.4s 							\n"

	    ".endm 																			\n"


	    ".macro	SAVE8x1								 							\n"

	    "	subs	x21, x21, #1 												\n"

	    "	st1 	{v30.s}[0], [x25] 									\n"
	    "	add  	x25, x25, x9, lsl #5 								\n"

	    "	st1 	{v30.s}[1], [x26] 									\n"
	    "	add  	x26, x26, x9, lsl #5 								\n"

	    "	st1 	{v30.s}[2], [x27] 									\n"
	    "	add  	x27, x27, x9, lsl #5 								\n"

	    "	st1 	{v30.s}[3], [x28] 									\n"
	    "	add  	x28, x28, x9, lsl #5 								\n"

	    "	st1 	{v31.s}[0], [x15] 									\n"
	    "	add  	x15, x15, x9, lsl #5 								\n"

	    "	st1 	{v31.s}[1], [x16] 									\n"
	    "	add  	x16, x16, x9, lsl #5 								\n"

	    "	st1 	{v31.s}[2], [x17] 									\n"
	    "	add  	x17, x17, x9, lsl #5 								\n"

	    "	st1 	{v31.s}[3], [x18] 									\n"
	    "	add  	x18, x18, x9, lsl #5 								\n"

	    ".endm 																			\n"


	    ".macro KERNEL8x1_BEGIN_K 									\n"

	    "	ldp		q0,	q1, [x11], #32									\n"
	    "	prfm	PLDL1KEEP, [x11, #512]							\n"

	    "	ldr 	s2, [x24], #4												\n"
	    "	fmul	v30.4s, v0.4s, v2.s[0]							\n"
	    "	ldr		s5, [x24], #4												\n"
	    "	fmul	v31.4s, v1.4s, v2.s[0]							\n"

	    "	ldp		q3,	q4, [x11], #32									\n"

	    ".endm 																			\n"


	    ".macro KERNEL8x1_K0 												\n"

	    "	ldp		q3,	q4, [x11], #32									\n"
	    "	prfm	PLDL1KEEP, [x24, #256]							\n"

	    "	ldr 	s5, [x24], #4												\n"
	    "	fmla	v30.4s, v0.4s, v2.s[0]							\n"
	    "	fmla	v31.4s, v1.4s, v2.s[0]							\n"

	    ".endm 																			\n"



	    ".macro KERNEL8x1_K1 												\n"

	    "	ldp		q0,	q1, [x11], #32									\n"
	    "	prfm	PLDL1KEEP, [x11, #256]							\n"

	    "	ldr 	s2, [x24], #4												\n"
	    "	fmla	v30.4s, v3.4s, v5.s[0]							\n"
	    "	fmla	v31.4s, v4.4s, v5.s[0]							\n"

	    ".endm 																			\n"


	    ".macro KERNEL8x1_END_K 										\n"

	    "	fmla	v30.4s, v3.4s, v5.s[0]							\n"
	    "	fmla	v31.4s, v4.4s, v5.s[0]							\n"

	    ".endm 																			\n"


	    "SMM_NN_KERNEL8x1:									 	 		\n"

	    "	ldr		x0, %[C]						 					\n"
	    "	ldr		x1, %[A]						 					\n"
	    "	ldr		x2, %[B]						 					\n"

	    "	ldr		x3, %[M]						 					\n"
	    "	ldr		x4, %[N]						 					\n"  // N = 4
	    "	ldr		x5, %[K]						 					\n"
	    "	ldr		x9, %[LN] 						 					\n"
	    "	ldr		x6, %[LK]						 					\n"

	    "	prfm		PLDL1KEEP, [x1, #512]            				\n"

	    "	ldr		x10, %[SB]						 					\n"
	    "	ldr		x8, %[k_tag] 					 					\n"


	    "	mov 	x25, x0   						 					\n"   //C1*
	    "	add		x26, x25, x9, lsl #2 			 					\n"   //C2*
	    "	add 	x27, x25, x9, lsl #3 			 					\n"   //C3*
	    "	add 	x28, x26, x9, lsl #3 			 					\n"   //C4*

	    "	add 	x15, x27, x9, lsl #3 								\n"		//C5*
	    "	add 	x16, x28, x9, lsl #3 								\n"		//C6*
	    "	add 	x17, x15, x9, lsl #3 								\n"		//C7*
	    "	add 	x18, x16, x9, lsl #3 								\n"		//C8*

	    "	mov		x11, x1   						 							\n"   // address_A
	    "	lsr		x21, x3, #3						 							\n"   // M / 8


	    "PACK_NN_KERNEL8x1:													\n"

	    "	mov		x24, x10						 								\n"   //还原SB的地址
	    "	mov		x12, x2						 	 								\n"   // B0*
	    "	add		x13, x12, x9, lsl #2 			 					\n"   // B0 + sizeof(float) * LN

	    "	prfm    PLDL1KEEP, [x11, #256]			 			\n"

	    "PACK_Body_NN_KERNEL8x1:								 		\n"

	    "	lsr		x22, x5, #2 					 							\n"     // K / 8

	    "	PACK_KERNEL8x1_BEGIN_K				 	 					\n"

	    "	subs	x22, x22, #1		     		 						\n"
	    "	b 		PACK_K1_3_8x1						 						\n"

	    "PACK_K_8x1:									 							\n"

	    "	PACK_KERNEL8x1_K0						 							\n"

	    "PACK_K1_3_8x1:									 						\n"

	    "	PACK_KERNEL8x1_K1						 							\n"
	    "	PACK_KERNEL8x1_K0						 							\n"

	    "	beq		PACK_Edge_K_8x1						 					\n"

	    "	PACK_KERNEL8x1_K1						 							\n"

	    "	subs	x22, x22, #1			 		 							\n"
	    "	b 		PACK_K_8x1			 				 						\n"

	    "PACK_Edge_K_8x1:							     					\n"

	    "	PACK_KERNEL8x1_END_K			         				\n"

	    "	cmp		x8, #0							 								\n"
	    "	beq		M8N1_PACK_SAVE 					 						\n"
	    "	M8N1_PACK_ADD_C														\n"

	    "M8N1_PACK_SAVE: 							 							\n"
	    "	SAVE8x1								 										\n"

	    "	beq		M8x1_END							 							\n"



//--------------------------------------------------------------------
	    "BEGIN_M8_KERNEL_8x1:                       \n"


	    " mov     x24, x10                    		 	\n"   //  address_B, x11 is address_A + offset

	    " prfm    PLDL1KEEP, [x24, #256]    		 		\n"
	    " prfm    PLDL1KEEP, [x11, #256]    		 		\n"

	    "Body_K_8x1:                            		\n"

	    " lsr   x22, x5, #2                 		 		\n"     // K / 8
	    " KERNEL8x1_BEGIN_K                		 	 		\n"
	    " subs  x22, x22, #1                		 		\n"
	    " b 	K1_7_8x1                            	\n"

	    "Main_K_8x1:                            		\n"

	    " KERNEL8x1_K0                     					\n"
	    "K1_7_8x1:                              		\n"

	    " KERNEL8x1_K1                     		 	 		\n"
	    " KERNEL8x1_K0                     		 	 		\n"

	    " beq   Edge_K_8x1                      		\n"

	    " KERNEL8x1_K1                     		 	 		\n"

	    " subs  x22, x22, #1                		 		\n"
	    " b     Main_K_8x1                      		\n"

	    "Edge_K_8x1:                            		\n"

	    " KERNEL8x1_END_K                 			 		\n"

	    "	cmp		x8, #0							 								\n"
	    "	beq		M8N1_SAVE 					 	 							\n"
	    "	M8N1_PACK_ADD_C 							 	 					\n"
	    "M8N1_SAVE: 							     							\n"
	    "	SAVE8x1								 	 									\n"

	    " bgt     BEGIN_M8_KERNEL_8x1               \n"


	    "M8x1_END:																	\n"

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

void SMM_NN_KERNEL8x2(float *C, float *A, float *B, long M, long N, long K,
                      long LN, long LK, float *SB, long k_tag)
{

	asm volatile(

	    ".macro	PACK_KERNEL8x2_BEGIN_K							\n"

	    "	ldr		q0,	[x11], #16											\n"
	    "	prfm	PLDL1KEEP, [x11, #512]							\n"

	    "	ld1		{v2.2s}, [x12]											\n"

	    "	fmul	v28.4s, v0.4s, v2.s[0]							\n"
	    "	ldr		q1,	[x11], #16											\n"
	    "	fmul	v29.4s, v0.4s, v2.s[1]							\n"

	    "	add  	x12, x12, x9, lsl #3 								\n"
	    "	ld1		{v5.2s}, [x13] 											\n"

	    "	fmul	v30.4s, v1.4s, v2.s[0]							\n"
	    "	ldp		q3, q4, [x11], #32 									\n"
	    "	fmul	v31.4s, v1.4s, v2.s[1]							\n"

	    "	add  	x13, x13, x9, lsl #3 								\n"
	    "	st1 	{v2.2s}, [x24], #8 									\n"

	    ".endm 																			\n"


	    ".macro	PACK_KERNEL8x2_K0										\n"

	    "	fmla	v28.4s, v0.4s, v2.s[0]							\n"
	    "	ld1		{v5.2s}, [x13]											\n"
	    "	fmla	v29.4s, v0.4s, v2.s[1]							\n"

	    "	add  	x13, x13, x9, lsl #3 								\n"
	    "	ldp		q3, q4, [x11], #32 									\n"

	    "	fmla	v30.4s, v1.4s, v2.s[0]							\n"
	    "	fmla	v31.4s, v1.4s, v2.s[1]							\n"

	    "	st1 	{v2.2s}, [x24], #8 									\n"

	    ".endm 																			\n"


	    ".macro	PACK_KERNEL8x2_K1										\n"

	    "	prfm	PLDL1KEEP, [x11, #512]							\n"

	    "	fmla	v28.4s, v3.4s, v5.s[0]							\n"
	    "	ld1		{v2.2s}, [x12]											\n"
	    "	fmla	v29.4s, v3.4s, v5.s[1]							\n"

	    "	add  	x12, x12, x9, lsl #3 								\n"
	    "	ldp		q0, q1, [x11], #32 									\n"

	    "	fmla	v30.4s, v4.4s, v5.s[0]							\n"
	    "	fmla	v31.4s, v4.4s, v5.s[1]							\n"
	    "	st1 	{v5.2s}, [x24], #8 									\n"

	    ".endm 																			\n"


	    ".macro	PACK_KERNEL8x2_END_K								\n"

	    "	fmla	v28.4s, v3.4s, v5.s[0]							\n"
	    "	fmla	v29.4s, v3.4s, v5.s[1]							\n"

	    "	fmla	v30.4s, v4.4s, v5.s[0]							\n"
	    "	fmla	v31.4s, v4.4s, v5.s[1]							\n"

	    "	st1 	{v5.2s}, [x24], #8 									\n"

	    ".endm 																			\n"


	    ".macro	M8N2_PACK_ADD_C											\n"

	    "	ld2		{v0.s, v1.s}[0], [x25] 							\n"
	    "	ld2		{v0.s, v1.s}[1], [x26] 							\n"
	    "	ld2		{v0.s, v1.s}[2], [x27] 							\n"
	    "	ld2		{v0.s, v1.s}[3], [x28] 							\n"

	    "	fadd 	v28.4s, v28.4s, v0.4s 							\n"
	    "	fadd 	v29.4s, v29.4s, v1.4s 							\n"

	    "	ld2		{v2.s, v3.s}[0], [x15] 							\n"
	    "	ld2		{v2.s, v3.s}[1], [x16] 							\n"
	    "	ld2		{v2.s, v3.s}[2], [x17] 							\n"
	    "	ld2		{v2.s, v3.s}[3], [x18] 							\n"

	    "	fadd 	v30.4s, v30.4s, v2.4s 							\n"
	    "	fadd 	v31.4s, v31.4s, v3.4s 							\n"

	    ".endm 																			\n"


	    ".macro	SAVE8x2								 							\n"

	    "	subs	x21, x21, #1 												\n"

	    "	st2 	{v28.s, v29.s}[0], [x25] 						\n"
	    "	add  	x25, x25, x9, lsl #5 								\n"
	    "	prfm 	PLDL2KEEP, [x25, #8]								\n"

	    "	st2 	{v28.s, v29.s}[1], [x26] 						\n"
	    "	add  	x26, x26, x9, lsl #5 								\n"
	    "	prfm 	PLDL2KEEP, [x26, #8]								\n"

	    "	st2 	{v28.s, v29.s}[2], [x27] 						\n"
	    "	add  	x27, x27, x9, lsl #5 								\n"
	    "	prfm 	PLDL2KEEP, [x27, #8]								\n"

	    "	st2 	{v28.s, v29.s}[3], [x28] 						\n"
	    "	add  	x28, x28, x9, lsl #5 								\n"
	    "	prfm 	PLDL2KEEP, [x28, #8]								\n"


	    "	st2 	{v30.s, v31.s}[0], [x15] 						\n"
	    "	add  	x15, x15, x9, lsl #5 								\n"
	    "	prfm 	PLDL2KEEP, [x15, #8]								\n"

	    "	st2 	{v30.s, v31.s}[1], [x16] 						\n"
	    "	add  	x16, x16, x9, lsl #5 								\n"
	    "	prfm 	PLDL2KEEP, [x16, #8]								\n"

	    "	st2 	{v30.s, v31.s}[2], [x17] 						\n"
	    "	add  	x17, x17, x9, lsl #5 								\n"
	    "	prfm 	PLDL2KEEP, [x17, #8]								\n"

	    "	st2 	{v30.s, v31.s}[3], [x18] 						\n"
	    "	add  	x18, x18, x9, lsl #5 								\n"
	    "	prfm 	PLDL2KEEP, [x18, #8]								\n"

	    ".endm 																			\n"


	    ".macro KERNEL8x2_BEGIN_K 									\n"

	    "	ldr		q0,	[x11], #16											\n"
	    "	prfm	PLDL1KEEP, [x11, #512]							\n"

	    "	ld1		{v2.2s}, [x24], #8 									\n"

	    "	fmul	v28.4s, v0.4s, v2.s[0]							\n"
	    "	ldr		q1,	[x11], #16											\n"
	    "	fmul	v29.4s, v0.4s, v2.s[1]							\n"

	    "	ld1		{v5.2s}, [x24], #8									\n"

	    "	fmul	v30.4s, v1.4s, v2.s[0]							\n"
	    "	ldp		q3, q4, [x11], #32 									\n"
	    "	fmul	v31.4s, v1.4s, v2.s[1]							\n"

	    ".endm 																			\n"


	    ".macro KERNEL8x2_K0 												\n"

	    "	prfm 	PLDL1KEEP, [x24, #256]							\n"

	    "	fmla	v28.4s, v0.4s, v2.s[0]							\n"
	    "	ld1		{v5.2s}, [x24], #8									\n"
	    "	fmla	v29.4s, v0.4s, v2.s[1]							\n"

	    "	ldp		q3, q4, [x11], #32 									\n"

	    "	fmla	v30.4s, v1.4s, v2.s[0]							\n"
	    "	fmla	v31.4s, v1.4s, v2.s[1]							\n"

	    ".endm 																			\n"



	    ".macro KERNEL8x2_K1 												\n"

	    "	prfm	PLDL1KEEP, [x11, #512]							\n"

	    "	fmla	v28.4s, v3.4s, v5.s[0]							\n"
	    "	ld1		{v2.2s}, [x24], #8									\n"
	    "	fmla	v29.4s, v3.4s, v5.s[1]							\n"

	    "	ldp		q0, q1, [x11], #32 									\n"

	    "	fmla	v30.4s, v4.4s, v5.s[0]							\n"
	    "	fmla	v31.4s, v4.4s, v5.s[1]							\n"

	    ".endm 																			\n"


	    ".macro KERNEL8x2_END_K 										\n"

	    "	fmla	v28.4s, v3.4s, v5.s[0]							\n"
	    "	fmla	v29.4s, v3.4s, v5.s[1]							\n"

	    "	fmla	v30.4s, v4.4s, v5.s[0]							\n"
	    "	fmla	v31.4s, v4.4s, v5.s[1]							\n"

	    ".endm 																			\n"


	    "SMM_NN_KERNEL8x2:									 	 			\n"

	    "	ldr		x0, %[C]						 								\n"
	    "	ldr		x1, %[A]						 								\n"
	    "	ldr		x2, %[B]						 								\n"

	    "	ldr		x3, %[M]						 								\n"
	    "	ldr		x4, %[N]						 								\n"  // N = 4
	    "	ldr		x5, %[K]						 								\n"
	    "	ldr		x9, %[LN] 						 							\n"
	    "	ldr		x6, %[LK]						 								\n"

	    "	prfm		PLDL1KEEP, [x1, #512]            	\n"

	    "	ldr		x10, %[SB]						 							\n"
	    "	ldr		x8, %[k_tag] 					 							\n"


	    "	mov 	x25, x0   						 							\n"   //C1*
	    "	add		x26, x25, x9, lsl #2 			 					\n"   //C2*
	    "	add 	x27, x25, x9, lsl #3 			 					\n"   //C3*
	    "	add 	x28, x26, x9, lsl #3 			 					\n"   //C4*

	    "	add 	x15, x27, x9, lsl #3 								\n"		//C5*
	    "	add 	x16, x28, x9, lsl #3 								\n"		//C6*
	    "	add 	x17, x15, x9, lsl #3 								\n"		//C7*
	    "	add 	x18, x16, x9, lsl #3 								\n"		//C8*

	    "	mov		x11, x1   						 							\n"   // address_A
	    "	lsr		x21, x3, #3						 							\n"   // M / 8


	    "PACK_NN_KERNEL8x2:													\n"

	    "	mov		x24, x10						 								\n"   //还原SB的地址
	    "	mov		x12, x2						 	 								\n"   // B0*
	    "	add		x13, x12, x9, lsl #2 			 					\n"   // B0 + sizeof(float) * LN

	    "	prfm    PLDL1KEEP, [x11, #256]			 			\n"

	    "PACK_Body_NN_KERNEL8x2:								 		\n"

	    "	lsr		x22, x5, #2 					 							\n"     // K / 8

	    "	PACK_KERNEL8x2_BEGIN_K				 	 					\n"

	    "	subs	x22, x22, #1		     		 						\n"
	    "	b 		PACK_K1_3_8x2						 						\n"

	    "PACK_K_8x2:									 							\n"

	    "	PACK_KERNEL8x2_K0						 							\n"

	    "PACK_K1_3_8x2:									 						\n"

	    "	PACK_KERNEL8x2_K1						 							\n"
	    "	PACK_KERNEL8x2_K0						 							\n"

	    "	beq		PACK_Edge_K_8x2						 					\n"

	    "	PACK_KERNEL8x2_K1						 							\n"

	    "	subs	x22, x22, #1			 		 							\n"
	    "	b 		PACK_K_8x2			 				 						\n"

	    "PACK_Edge_K_8x2:							     					\n"

	    "	PACK_KERNEL8x2_END_K			         				\n"

	    "	cmp		x8, #0							 								\n"
	    "	beq		M8N2_PACK_SAVE 					 						\n"
	    "	M8N2_PACK_ADD_C														\n"

	    "M8N2_PACK_SAVE: 							 							\n"
	    "	SAVE8x2								 										\n"

	    "	beq		M8x2_END							 							\n"



//--------------------------------------------------------------------
	    "BEGIN_M8_KERNEL_8x2:                       \n"


	    " mov     x24, x10                    		 	\n"   //  address_B, x11 is address_A + offset

	    " prfm    PLDL1KEEP, [x24, #256]    		 		\n"
	    " prfm    PLDL1KEEP, [x11, #256]    		 		\n"

	    "Body_K_8x2:                            		\n"

	    " lsr   x22, x5, #2                 		 		\n"     // K / 8
	    " KERNEL8x2_BEGIN_K                		 	 		\n"
	    " subs  x22, x22, #1                		 		\n"
	    " b 	K1_7_8x2                            	\n"

	    "Main_K_8x2:                            		\n"

	    " KERNEL8x2_K0                     					\n"
	    "K1_7_8x2:                              		\n"

	    " KERNEL8x2_K1                     		 	 		\n"
	    " KERNEL8x2_K0                     		 	 		\n"

	    " beq   Edge_K_8x2                      		\n"

	    " KERNEL8x2_K1                     		 	 		\n"

	    " subs  x22, x22, #1                		 		\n"
	    " b     Main_K_8x2                      		\n"

	    "Edge_K_8x2:                            		\n"

	    " KERNEL8x2_END_K                 			 		\n"

	    "	cmp		x8, #0							 								\n"
	    "	beq		M8N2_SAVE 					 	 							\n"
	    "	M8N2_PACK_ADD_C 							 	 					\n"
	    "M8N2_SAVE: 							     							\n"
	    "	SAVE8x2								 	 									\n"

	    " bgt     BEGIN_M8_KERNEL_8x2               \n"


	    "M8x2_END:																	\n"

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

void SMM_NN_KERNEL8x4(float *C, float *A, float *B, long	M, long N, long K,
                       long LN, long LK, float *SB, long k_tag)
{
	asm volatile(

	    ".macro	PACK_KERNEL8x4_BEGIN_K							\n"

	    "	ldr		q0,	[x11], #16											\n"
	    "	prfm	PLDL1KEEP, [x11, #512]							\n"

	    "	ldr		q2, [x12]														\n"
	    "	fmul	v24.4s, v2.4s, v0.s[0] 							\n"
	    "	ldr		q1,	[x11], #16											\n"
	    "	fmul	v25.4s, v2.4s, v0.s[1] 							\n"
	    "	fmul	v26.4s, v2.4s, v0.s[2] 							\n"
	    "	ldr		q5, [x13]														\n"
	    "	fmul	v27.4s, v2.4s, v0.s[3] 							\n"

	    "	add  	x12, x12, x9, lsl #3 								\n"   //addr_B + 2 * sizeof(column)

	    "	fmul	v28.4s, v2.4s, v1.s[0] 							\n"
	    "	add  	x13, x13, x9, lsl #3 								\n"
	    "	fmul	v29.4s, v2.4s, v1.s[1] 							\n"
	    "	ldr		q3,	[x11], #16											\n"
	    "	fmul	v30.4s, v2.4s, v1.s[2] 							\n"
	    "	prfm	PLDL1KEEP, [x12, #16]								\n"
	    "	fmul	v31.4s, v2.4s, v1.s[3] 							\n"

	    "	ldr		q4,	[x11], #16											\n"
	    "	str		q2, [x24], #16 											\n"

	    ".endm 																			\n"


	    ".macro	PACK_KERNEL8x4_K0										\n"

	    "	fmla	v24.4s, v2.4s, v0.s[0] 							\n"
	    "	ldr		q5,	[x13]														\n"
	    "	fmla	v25.4s, v2.4s, v0.s[1] 							\n"
	    "	fmla	v26.4s, v2.4s, v0.s[2] 							\n"
	    "	ldr		q3, [x11], #16											\n"
	    "	fmla	v27.4s, v2.4s, v0.s[3] 							\n"

	    "	add  	x13, x13, x9, lsl #3 								\n"

	    "	fmla	v28.4s, v2.4s, v1.s[0] 							\n"
	    "	ldr		q4,	[x11], #16											\n"
	    "	fmla	v29.4s, v2.4s, v1.s[1] 							\n"
	    "	fmla	v30.4s, v2.4s, v1.s[2] 							\n"
	    "	prfm	PLDL1KEEP, [x13, #16]								\n"
	    "	fmla	v31.4s, v2.4s, v1.s[3] 							\n"

	    "	str 	q2, [x24], #16 											\n"

	    ".endm 																			\n"


	    ".macro	PACK_KERNEL8x4_K1										\n"

	    "	fmla	v24.4s, v5.4s, v3.s[0] 							\n"
	    "	ldr		q2,	[x12]														\n"
	    "	fmla	v25.4s, v5.4s, v3.s[1] 							\n"
	    "	prfm	PLDL1KEEP, [x11, #512]							\n"
	    "	fmla	v26.4s, v5.4s, v3.s[2] 							\n"
	    "	ldr		q0, [x11], #16											\n"
	    "	fmla	v27.4s, v5.4s, v3.s[3] 							\n"

	    "	add  	x12, x12, x9, lsl #3 								\n"

	    "	fmla	v28.4s, v5.4s, v4.s[0] 							\n"
	    "	ldr		q1,	[x11], #16											\n"
	    "	fmla	v29.4s, v5.4s, v4.s[1] 							\n"
	    "	fmla	v30.4s, v5.4s, v4.s[2] 							\n"
	    "	prfm	PLDL1KEEP, [x12, #16]								\n"
	    "	fmla	v31.4s, v5.4s, v4.s[3] 							\n"

	    "	str 	q5, [x24], #16 											\n"

	    ".endm 																			\n"


	    ".macro	PACK_KERNEL8x4_END_K								\n"

	    "	fmla	v24.4s, v5.4s, v3.s[0] 							\n"
	    "	fmla	v25.4s, v5.4s, v3.s[1] 							\n"
	    "	fmla	v26.4s, v5.4s, v3.s[2] 							\n"
	    "	fmla	v27.4s, v5.4s, v3.s[3] 							\n"

	    "	str 	q5, [x24], #16 											\n"

	    "	fmla	v28.4s, v5.4s, v4.s[0] 							\n"
	    "	fmla	v29.4s, v5.4s, v4.s[1] 							\n"
	    "	fmla	v30.4s, v5.4s, v4.s[2] 							\n"
	    "	fmla	v31.4s, v5.4s, v4.s[3] 							\n"

	    ".endm 																			\n"


	    ".macro	PACK_KERNEL8x4_RIGHT_K 							\n"

	    "	ldr		q2, [x12]														\n"
	    "	ldr		q0, [x11], #16 											\n"

	    "	fmla	v24.4s, v2.4s, v0.s[0]							\n"
	    "	add 	x12, x12, x9, lsl #2 								\n"
	    "	fmla	v25.4s, v2.4s, v0.s[1]							\n"
	    "	ldr		q1, [x11], #16 											\n"
	    "	fmla	v26.4s, v2.4s, v0.s[2]							\n"
	    "	prfm	PLDL1KEEP, [x12, #64] 							\n"
	    "	fmla	v27.4s, v2.4s, v0.s[3]							\n"

	    "	fmla	v28.4s, v2.4s, v1.s[0]							\n"
	    "	fmla	v29.4s, v2.4s, v1.s[1]							\n"
	    "	str  	q2, [x24], #16 											\n"
	    "	fmla	v30.4s, v2.4s, v1.s[2]							\n"
	    "	fmla	v31.4s, v2.4s, v1.s[3]							\n"


	    ".endm 																			\n"


	    ".macro	M8N4_PACK_ADD_C											\n"

	    "	prfm	PLDL1KEEP, [x25, #16]								\n"
	    "	prfm 	PLDL1KEEP, [x26, #16]								\n"

	    "	ldr		q0, [x25] 													\n"
	    "	fadd 	v24.4s, v24.4s, v0.4s 							\n"
	    "	prfm 	PLDL1KEEP, [x27, #16]								\n"
	    "	ldr  	q1, [x26]   												\n"
	    "	fadd 	v25.4s, v25.4s, v1.4s 							\n"
	    "	ldr		q2, [x27] 													\n"
	    "	prfm 	PLDL1KEEP, [x28, #64]								\n"
	    "	fadd 	v26.4s, v26.4s, v2.4s 							\n"

	    "	ldr		q3, [x28] 													\n"
	    "	prfm 	PLDL1KEEP, [x15, #16]								\n"
	    "	fadd 	v27.4s, v27.4s, v3.4s 							\n"

	    "	ldr		q4, [x15] 													\n"
	    "	prfm 	PLDL1KEEP, [x16, #16]								\n"
	    "	fadd 	v28.4s, v28.4s, v4.4s 							\n"

	    "	ldr		q5, [x16] 													\n"
	    "	prfm 	PLDL1KEEP, [x17, #16]								\n"
	    "	fadd 	v29.4s, v29.4s, v5.4s 							\n"

	    "	ldr		q6, [x17] 													\n"
	    "	prfm 	PLDL1KEEP, [x18, #16]								\n"
	    "	fadd 	v30.4s, v30.4s, v6.4s 							\n"

	    "	ldr		q7, [x18] 													\n"
	    "	fadd 	v31.4s, v31.4s, v7.4s 							\n"

	    ".endm 																			\n"


	    ".macro	SAVE8x4								 							\n"

	    "	subs	x21, x21, #1 												\n"

	    "	str 	q24, [x25] 													\n"
	    "	add  	x25, x25, x9, lsl #5 								\n"
	    "	prfm 	PLDL2KEEP, [x25, #16]								\n"

	    "	str 	q25, [x26] 													\n"
	    "	add  	x26, x26, x9, lsl #5 								\n"
	    "	prfm 	PLDL2KEEP, [x26, #16]								\n"

	    "	str 	q26, [x27] 													\n"
	    "	add  	x27, x27, x9, lsl #5 								\n"
	    "	prfm 	PLDL2KEEP, [x27, #16]								\n"

	    "	str 	q27, [x28] 													\n"
	    "	add  	x28, x28, x9, lsl #5 								\n"
	    "	prfm 	PLDL2KEEP, [x28, #16]								\n"


	    "	str 	q28, [x15] 													\n"
	    "	add  	x15, x15, x9, lsl #5 								\n"
	    "	prfm 	PLDL2KEEP, [x15, #16]								\n"

	    "	str 	q29, [x16] 													\n"
	    "	add  	x16, x16, x9, lsl #5 								\n"
	    "	prfm 	PLDL2KEEP, [x16, #16]								\n"

	    "	str 	q30, [x17] 													\n"
	    "	add  	x17, x17, x9, lsl #5 								\n"
	    "	prfm 	PLDL2KEEP, [x17, #16]								\n"

	    "	str 	q31, [x18] 													\n"
	    "	add  	x18, x18, x9, lsl #5 								\n"
	    "	prfm 	PLDL2KEEP, [x18, #16]								\n"

	    ".endm 																			\n"



	    ".macro KERNEL8x4_BEGIN_K 									\n"

	    "	ldr		q0, [x11], #16 											\n"
	    "	ldr 	q2, [x24], #16 											\n"

	    "	prfm 	PLDL1KEEP, [x11, #256]							\n"

	    "	fmul	v24.4s, v2.4s, v0.s[0]							\n"
	    "	fmul	v25.4s, v2.4s, v0.s[1]							\n"
	    "	ldr		q1, [x11], #16 											\n"
	    "	fmul	v26.4s, v2.4s, v0.s[2]							\n"
	    "	fmul	v27.4s, v2.4s, v0.s[3]							\n"

	    "	ldr		q3, [x11], #16 											\n"
	    "	prfm 	PLDL1KEEP, [x24, #256]							\n"

	    "	fmul	v28.4s, v2.4s, v1.s[0]							\n"
	    "	fmul	v29.4s, v2.4s, v1.s[1]							\n"
	    "	ldr		q5, [x24], #16 											\n"
	    "	fmul	v30.4s, v2.4s, v1.s[2]							\n"
	    "	fmul	v31.4s, v2.4s, v1.s[3]							\n"
	    "	ldr		q4, [x11], #16 											\n"

	    ".endm 																			\n"


	    ".macro KERNEL8x4_K0 												\n"

	    "	ldr		q3, [x11], #16 											\n"

	    "	prfm 	PLDL1KEEP, [x24, #256]							\n"
	    "	fmla	v24.4s, v2.4s, v0.s[0]							\n"
	    "	fmla	v25.4s, v2.4s, v0.s[1]							\n"
	    "	ldr		q5, [x24], #16 											\n"
	    "	fmla	v26.4s, v2.4s, v0.s[2]							\n"
	    "	fmla	v27.4s, v2.4s, v0.s[3]							\n"

	    "	ldr		q4, [x11], #16 											\n"
	    "	fmla	v28.4s, v2.4s, v1.s[0]							\n"
	    "	fmla	v29.4s, v2.4s, v1.s[1]							\n"
	    "	fmla	v30.4s, v2.4s, v1.s[2]							\n"
	    "	fmla	v31.4s, v2.4s, v1.s[3]							\n"

	    ".endm 																			\n"



	    ".macro KERNEL8x4_K1 												\n"

	    "	ldr		q0, [x11], #16 											\n"

	    "	prfm 	PLDL1KEEP, [x11, #256]							\n"
	    "	fmla	v24.4s, v5.4s, v3.s[0]							\n"
	    "	fmla	v25.4s, v5.4s, v3.s[1]							\n"
	    "	ldr		q2, [x24], #16 											\n"
	    "	fmla	v26.4s, v5.4s, v3.s[2]							\n"
	    "	fmla	v27.4s, v5.4s, v3.s[3]							\n"

	    "	ldr		q1, [x11], #16 											\n"
	    "	fmla	v28.4s, v5.4s, v4.s[0]							\n"
	    "	fmla	v29.4s, v5.4s, v4.s[1]							\n"
	    "	fmla	v30.4s, v5.4s, v4.s[2]							\n"
	    "	fmla	v31.4s, v5.4s, v4.s[3]							\n"

	    ".endm 																			\n"


	    ".macro KERNEL8x4_END_K 										\n"


	    "	fmla	v24.4s, v5.4s, v3.s[0]							\n"
	    "	fmla	v25.4s, v5.4s, v3.s[1]							\n"
	    "	fmla	v26.4s, v5.4s, v3.s[2]							\n"
	    "	fmla	v27.4s, v5.4s, v3.s[3]							\n"

	    "	fmla	v28.4s, v5.4s, v4.s[0]							\n"
	    "	fmla	v29.4s, v5.4s, v4.s[1]							\n"
	    "	fmla	v30.4s, v5.4s, v4.s[2]							\n"
	    "	fmla	v31.4s, v5.4s, v4.s[3]							\n"

	    ".endm 																			\n"


	    ".macro	KERNEL8x4_RIGHT_K 									\n"

	    "	ldr		q2, [x24], #16											\n"
	    "	ldr		q0, [x11], #16 											\n"

	    "	fmla	v24.4s, v2.4s, v0.s[0]							\n"
	    "	fmla	v25.4s, v2.4s, v0.s[1]							\n"
	    "	ldr		q1, [x11], #16 											\n"
	    "	fmla	v26.4s, v2.4s, v0.s[2]							\n"
	    "	fmla	v27.4s, v2.4s, v0.s[3]							\n"

	    "	fmla	v28.4s, v2.4s, v1.s[0]							\n"
	    "	fmla	v29.4s, v2.4s, v1.s[1]							\n"
	    "	fmla	v30.4s, v2.4s, v1.s[2]							\n"
	    "	fmla	v31.4s, v2.4s, v1.s[3]							\n"

	    ".endm 																			\n"




	    "SMM_NN_KERNEL8x4:									 	 			\n"

	    "	ldr		x0, %[C]						 								\n"
	    "	ldr		x1, %[A]						 								\n"
	    "	ldr		x2, %[B]						 								\n"

	    "	ldr		x3, %[M]						 								\n"
	    "	ldr		x4, %[N]						 								\n"  // N = 4
	    "	ldr		x5, %[K]						 								\n"
	    "	ldr		x9, %[LN] 						 							\n"
	    "	ldr		x6, %[LK]						 								\n"

	    "	prfm		PLDL1KEEP, [x1, #512]            	\n"

	    "	ldr		x10, %[SB]						 							\n"
	    "	ldr		x8, %[k_tag] 					 							\n"


	    "	mov 	x25, x0   						 							\n"   //C1*
	    "	prfm	PLDL2KEEP, [x25, #16]								\n"
	    "	add		x26, x25, x9, lsl #2 			 					\n"   //C2*
	    "	prfm	PLDL2KEEP, [x26, #16]								\n"
	    "	add 	x27, x25, x9, lsl #3 			 					\n"   //C3*
	    "	prfm	PLDL2KEEP, [x27, #16]								\n"
	    "	add 	x28, x26, x9, lsl #3 			 					\n"   //C4*
	    "	prfm	PLDL2KEEP, [x28, #16]								\n"

	    "	add 	x15, x27, x9, lsl #3 								\n"		//C5*
	    "	prfm	PLDL2KEEP, [x15, #16]								\n"
	    "	add 	x16, x28, x9, lsl #3 								\n"		//C6*
	    "	prfm	PLDL2KEEP, [x16, #16]								\n"
	    "	add 	x17, x15, x9, lsl #3 								\n"		//C7*
	    "	prfm	PLDL2KEEP, [x17, #16]								\n"
	    "	add 	x18, x16, x9, lsl #3 								\n"		//C8*
	    "	prfm	PLDL2KEEP, [x18, #16]								\n"


	    "	mov		x11, x1   						 							\n"   // address_A
	    "	lsr		x21, x3, #3						 							\n"   // M / 8


	    "PACK_NN_KERNEL8x4:													\n"

	    "	mov		x24, x10						 								\n"   //还原SB的地址
	    "	mov		x12, x2						 	 								\n"   // B0*
	    "	add		x13, x12, x9, lsl #2 			 					\n"   // B0 + sizeof(float) * LN

	    "	prfm    PLDL1KEEP, [x12, #16]			 				\n"
	    "	prfm    PLDL1KEEP, [x13, #16]			 				\n"
	    "	prfm    PLDL1KEEP, [x11, #256]			 			\n"

	    "PACK_Body_NN_KERNEL8x4:								 		\n"

	    "	lsr		x22, x5, #2 					 							\n"     // K / 8

	    "	PACK_KERNEL8x4_BEGIN_K				 	 					\n"

	    "	subs	x22, x22, #1		     		 						\n"
	    "	b 		PACK_K1_3_8x4						 						\n"

	    "PACK_K_8x4:									 							\n"

	    "	PACK_KERNEL8x4_K0						 							\n"

	    "PACK_K1_3_8x4:									 						\n"

	    "	PACK_KERNEL8x4_K1						 							\n"
	    "	PACK_KERNEL8x4_K0						 							\n"

	    "	beq		PACK_Edge_K_8x4						 					\n"

	    "	PACK_KERNEL8x4_K1						 							\n"

	    "	subs	x22, x22, #1			 		 							\n"
	    "	b 		PACK_K_8x4			 				 						\n"

	    "PACK_Edge_K_8x4:							     					\n"

	    "	PACK_KERNEL8x4_END_K			         				\n"

	    "	ands 	x22, x5, #3 												\n"
	    "	beq		PACK_BEGIN_SAVE8x4 									\n"
	    "PACK_RIGHT_LOOP_K_8x4: 										\n"
	    "	subs 	x22, x22, #1 												\n"
	    "	PACK_KERNEL8x4_RIGHT_K										\n"
	    "bgt		PACK_RIGHT_LOOP_K_8x4								\n"

	    "PACK_BEGIN_SAVE8x4:												\n"

	    "	cmp		x8, #0							 								\n"
	    "	beq		M8N4_PACK_SAVE 					 						\n"
	    "	M8N4_PACK_ADD_C														\n"

	    "M8N4_PACK_SAVE: 							 							\n"
	    "	SAVE8x4								 										\n"

	    "	beq		M8x4_END							 							\n"



//--------------------------------------------------------------------
	    "BEGIN_M8_KERNEL_8x4:                       \n"


	    " mov     x24, x10                    		 	\n"   //  address_B, x11 is address_A + offset

	    " prfm    PLDL1KEEP, [x24, #256]    		 		\n"
	    " prfm    PLDL1KEEP, [x11, #256]    		 		\n"

	    "Body_K_8x4:                            		\n"

	    " lsr   x22, x5, #2                 		 		\n"     // K / 8
	    " KERNEL8x4_BEGIN_K                		 	 		\n"
	    " subs  x22, x22, #1                		 		\n"
	    " b 	K1_7_8x4                            	\n"

	    "Main_K_8x4:                            		\n"

	    " KERNEL8x4_K0                     					\n"
	    "K1_7_8x4:                              		\n"

	    " KERNEL8x4_K1                     		 	 		\n"
	    " KERNEL8x4_K0                     		 	 		\n"

	    " beq   Edge_K_8x4                      		 \n"

	    " KERNEL8x4_K1                     		 	 		\n"

	    " subs  x22, x22, #1                		 		\n"
	    " b     Main_K_8x4                      		\n"

	    "Edge_K_8x4:                            		\n"

	    " KERNEL8x4_END_K                 			 		\n"

	    "	ands 	x22, x5, #3 												\n"
	    "	beq		BEGIN_SAVE8x4 											\n"
	    "RIGHT_LOOP_K_8x4: 													\n"
	    "	subs 	x22, x22, #1 												\n"
	    "	KERNEL8x4_RIGHT_K													\n"
	    "bgt		RIGHT_LOOP_K_8x4										\n"

	    "BEGIN_SAVE8x4: 														\n"

	    "	cmp		x8, #0							 								\n"
	    "	beq		M8N4_SAVE 					 	 							\n"
	    "	M8N4_PACK_ADD_C 							 	 					\n"
	    "M8N4_SAVE: 							     							\n"
	    "	SAVE8x4								 	 									\n"

	    " bgt     BEGIN_M8_KERNEL_8x4               \n"


	    "M8x4_END:																	\n"


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

void SMM(float *C, float *A, float *B, long	M, long N, long K,
          long LN, long LK, float *SB, long k_tag)
{
	asm volatile(

	    ".macro	PACK_KERNEL8x12_BEGIN_K							\n"


	    "	ldr		q0,	[x11], #16											\n"

	    "	prfm	PLDL1KEEP, [x11, #512]							\n"

	    "	ldp		q2, q3, [x12]												\n"
	    "	fmul	v8.4s, v2.4s, v0.s[0]								\n"
	    "	ldr 	q4, [x12, #32]											\n"
	    "	fmul	v9.4s, v3.4s, v0.s[0]								\n"
	    "	ldr		q1, [x11], #16 											\n"
	    "	fmul	v10.4s, v4.4s, v0.s[0]							\n"

	    "	prfm	PLDL1KEEP, [x13, #48]								\n"

	    "	add  	x12, x12, x9, lsl #3 								\n"
	    "	fmul	v11.4s, v2.4s, v0.s[1]							\n"
	    "	fmul	v12.4s, v3.4s, v0.s[1]							\n"
	    "	fmul	v13.4s, v4.4s, v0.s[1]							\n"

	    "	ldr 	q5, [x13]														\n"
	    "	fmul	v14.4s, v2.4s, v0.s[2]							\n"
	    "	fmul	v15.4s, v3.4s, v0.s[2]							\n"
	    "	fmul	v16.4s, v4.4s, v0.s[2]							\n"

	    "	ldr 	q6, [x13, #16]											\n"

	    "	fmul	v17.4s, v2.4s, v0.s[3]							\n"
	    "	fmul	v18.4s, v3.4s, v0.s[3]							\n"
	    "	fmul	v19.4s, v4.4s, v0.s[3]							\n"

	    "	ldr  	q0, [x11], #16 											\n"

	    "	fmul	v20.4s, v2.4s, v1.s[0]							\n"
	    "	fmul	v21.4s, v3.4s, v1.s[0]							\n"
	    "	fmul	v22.4s, v4.4s, v1.s[0]							\n"

	    "	ldr 	q7, [x13, #32]											\n"

	    "	fmul	v23.4s, v2.4s, v1.s[1]							\n"
	    "	fmul	v24.4s, v3.4s, v1.s[1]							\n"
	    "	fmul	v25.4s, v4.4s, v1.s[1]							\n"

	    "	stp		q2, q3, [x24], #32 									\n"
	    "	add 	x13, x13, x9, lsl #3 								\n"

	    "	fmul	v26.4s, v2.4s, v1.s[2]							\n"
	    "	fmul	v27.4s, v3.4s, v1.s[2]							\n"
	    "	fmul	v28.4s, v4.4s, v1.s[2]							\n"

	    "	str		q4, [x24], #16 											\n"

	    "	fmul	v29.4s, v2.4s, v1.s[3]							\n"
	    "	fmul	v30.4s, v3.4s, v1.s[3]							\n"
	    "	fmul	v31.4s, v4.4s, v1.s[3]							\n"

	    "	ldr 	q1, [x11], #16 											\n"


	    ".endm 																			\n"



	    ".macro	PACK_KERNEL8x12_K0									\n"


	    "	prfm PLDL1KEEP, [x11, #512] 							\n"
	    "	ldp 	q5, q6, [x13] 											\n"
	    "	fmla	v8.4s, v2.4s, v0.s[0]								\n"
	    "	fmla	v9.4s, v3.4s, v0.s[0]								\n"
	    "	fmla	v10.4s, v4.4s, v0.s[0]							\n"

	    "	ldr 	q7, [x13, #32] 											\n"

	    "	fmla	v11.4s, v2.4s, v0.s[1]							\n"
	    "	fmla	v12.4s, v3.4s, v0.s[1]							\n"
	    "	fmla	v13.4s, v4.4s, v0.s[1]							\n"

	    "	add 	x13, x13, x9, lsl #3  							\n"

	    "	fmla	v14.4s, v2.4s, v0.s[2]							\n"
	    "	fmla	v15.4s, v3.4s, v0.s[2]							\n"
	    "	fmla	v16.4s, v4.4s, v0.s[2]							\n"

	    "	prfm PLDL1KEEP, [x13, #48] 								\n"

	    "	fmla	v17.4s, v2.4s, v0.s[3]							\n"
	    "	fmla	v18.4s, v3.4s, v0.s[3]							\n"
	    "	fmla	v19.4s, v4.4s, v0.s[3]							\n"

	    "	ldr 	q0, [x11], #16 											\n"

	    "	fmla	v20.4s, v2.4s, v1.s[0]							\n"
	    "	fmla	v21.4s, v3.4s, v1.s[0]							\n"
	    "	fmla	v22.4s, v4.4s, v1.s[0]							\n"

	    "	stp 	q2, q3, [x24], #32 									\n"

	    "	fmla	v23.4s, v2.4s, v1.s[1]							\n"
	    "	fmla	v24.4s, v3.4s, v1.s[1]							\n"
	    "	fmla	v25.4s, v4.4s, v1.s[1]							\n"

	    "	str 	q4, [x24], #16 											\n"

	    "	fmla	v26.4s, v2.4s, v1.s[2]							\n"
	    "	fmla	v27.4s, v3.4s, v1.s[2]							\n"
	    "	fmla	v28.4s, v4.4s, v1.s[2]							\n"

	    "	prfm PLDL1KEEP, [x11, #576] 							\n"

	    "	fmla	v29.4s, v2.4s, v1.s[3]							\n"
	    "	fmla	v30.4s, v3.4s, v1.s[3]							\n"
	    "	fmla	v31.4s, v4.4s, v1.s[3]							\n"

	    "	ldr 	q1, [x11], #16 											\n"

	    ".endm 																			\n"




	    ".macro	PACK_KERNEL8x12_K1									\n"

	    "	ldp 	q2, q3, [x12] 											\n"
	    "	fmla	v8.4s, v5.4s, v0.s[0]								\n"
	    "	fmla	v9.4s, v6.4s, v0.s[0]								\n"
	    "	fmla	v10.4s, v7.4s, v0.s[0]							\n"

	    "	ldr 	q4, [x12, #32] 											\n"

	    "	fmla	v11.4s, v5.4s, v0.s[1]							\n"
	    "	fmla	v12.4s, v6.4s, v0.s[1]							\n"
	    "	fmla	v13.4s, v7.4s, v0.s[1]							\n"

	    "	add 	x12, x12, x9, lsl #3  							\n"

	    "	fmla	v14.4s, v5.4s, v0.s[2]							\n"
	    "	fmla	v15.4s, v6.4s, v0.s[2]							\n"
	    "	fmla	v16.4s, v7.4s, v0.s[2]							\n"

	    "	prfm PLDL1KEEP, [x12, #48] 								\n"

	    "	fmla	v17.4s, v5.4s, v0.s[3]							\n"
	    "	fmla	v18.4s, v6.4s, v0.s[3]							\n"
	    "	fmla	v19.4s, v7.4s, v0.s[3]							\n"

	    "	ldr 	q0, [x11], #16 											\n"

	    "	fmla	v20.4s, v5.4s, v1.s[0]							\n"
	    "	fmla	v21.4s, v6.4s, v1.s[0]							\n"
	    "	fmla	v22.4s, v7.4s, v1.s[0]							\n"

	    "	stp 	q5, q6, [x24], #32 									\n"

	    "	fmla	v23.4s, v5.4s, v1.s[1]							\n"
	    "	fmla	v24.4s, v6.4s, v1.s[1]							\n"
	    "	fmla	v25.4s, v7.4s, v1.s[1]							\n"

	    "	str 	q7, [x24], #16 											\n"

	    "	fmla	v26.4s, v5.4s, v1.s[2]							\n"
	    "	fmla	v27.4s, v6.4s, v1.s[2]							\n"
	    "	fmla	v28.4s, v7.4s, v1.s[2]							\n"

	    "	fmla	v29.4s, v5.4s, v1.s[3]							\n"
	    "	fmla	v30.4s, v6.4s, v1.s[3]							\n"
	    "	fmla	v31.4s, v7.4s, v1.s[3]							\n"

	    "	ldr 	q1, [x11], #16 											\n"

	    ".endm 																			\n"



	    ".macro	PACK_KERNEL8x12_END_K								\n"

	    "	fmla	v8.4s, v5.4s, v0.s[0]								\n"
	    "	fmla	v9.4s, v6.4s, v0.s[0]								\n"
	    "	fmla	v10.4s, v7.4s, v0.s[0]							\n"

	    "	fmla	v11.4s, v5.4s, v0.s[1]							\n"
	    "	fmla	v12.4s, v6.4s, v0.s[1]							\n"
	    "	fmla	v13.4s, v7.4s, v0.s[1]							\n"

	    "	fmla	v14.4s, v5.4s, v0.s[2]							\n"
	    "	fmla	v15.4s, v6.4s, v0.s[2]							\n"
	    "	fmla	v16.4s, v7.4s, v0.s[2]							\n"

	    "	fmla	v17.4s, v5.4s, v0.s[3]							\n"
	    "	fmla	v18.4s, v6.4s, v0.s[3]							\n"
	    "	fmla	v19.4s, v7.4s, v0.s[3]							\n"

	    "	fmla	v20.4s, v5.4s, v1.s[0]							\n"
	    "	fmla	v21.4s, v6.4s, v1.s[0]							\n"
	    "	fmla	v22.4s, v7.4s, v1.s[0]							\n"

	    "	stp 	q5, q6, [x24], #32 									\n"

	    "	fmla	v23.4s, v5.4s, v1.s[1]							\n"
	    "	fmla	v24.4s, v6.4s, v1.s[1]							\n"
	    "	fmla	v25.4s, v7.4s, v1.s[1]							\n"

	    "	str 	q7, [x24], #16 											\n"

	    "	fmla	v26.4s, v5.4s, v1.s[2]							\n"
	    "	fmla	v27.4s, v6.4s, v1.s[2]							\n"
	    "	fmla	v28.4s, v7.4s, v1.s[2]							\n"

	    "	fmla	v29.4s, v5.4s, v1.s[3]							\n"
	    "	fmla	v30.4s, v6.4s, v1.s[3]							\n"
	    "	fmla	v31.4s, v7.4s, v1.s[3]							\n"

	    ".endm 																			\n"



	    ".macro	PACK_KERNEL8x12_RIGHT_K 						\n"


	    "	ldp		q2, q3, [x12]												\n"
	    "	ldr		q0, [x11], #16 											\n"

	    "	fmla	v8.4s, v2.4s, v0.s[0]								\n"
	    "	ldr		q4, [x12, #32]											\n"
	    "	fmla	v9.4s, v3.4s, v0.s[0]								\n"
	    "	fmla	v10.4s, v4.4s, v0.s[0]							\n"

	    "	add  	x12, x12, x9, lsl #2 								\n"
	    "	fmla	v11.4s, v2.4s, v0.s[1]							\n"
	    "	ldr		q1, [x11], #16 											\n"
	    "	fmla	v12.4s, v3.4s, v0.s[1]							\n"
	    "	prfm	PLDL1KEEP, [x12, #64]								\n"
	    "	fmla	v13.4s, v4.4s, v0.s[1]							\n"

	    "	fmla	v14.4s, v2.4s, v0.s[2]							\n"
	    "	fmla	v15.4s, v3.4s, v0.s[2]							\n"
	    "	fmla	v16.4s, v4.4s, v0.s[2]							\n"

	    "	fmla	v17.4s, v2.4s, v0.s[3]							\n"
	    "	fmla	v18.4s, v3.4s, v0.s[3]							\n"
	    "	fmla	v19.4s, v4.4s, v0.s[3]							\n"

	    "	fmla	v20.4s, v2.4s, v1.s[0]							\n"
	    "	fmla	v21.4s, v3.4s, v1.s[0]							\n"
	    "	fmla	v22.4s, v4.4s, v1.s[0]							\n"

	    "	stp 	q2, q3, [x24], #32 									\n"

	    "	fmla	v23.4s, v2.4s, v1.s[1]							\n"
	    "	fmla	v24.4s, v3.4s, v1.s[1]							\n"
	    "	fmla	v25.4s, v4.4s, v1.s[1]							\n"

	    "	str 	q4, [x24], #16 											\n"

	    "	fmla	v26.4s, v2.4s, v1.s[2]							\n"
	    "	fmla	v27.4s, v3.4s, v1.s[2]							\n"
	    "	fmla	v28.4s, v4.4s, v1.s[2]							\n"


	    "	fmla	v29.4s, v2.4s, v1.s[3]							\n"
	    "	fmla	v30.4s, v3.4s, v1.s[3]							\n"
	    "	fmla	v31.4s, v4.4s, v1.s[3]							\n"


	    ".endm 																			\n"



	    ".macro	M8N12_PACK_ADD_C										\n"


	    "	prfm	PLDL1KEEP, [x25, #64]								\n"
	    "	prfm 	PLDL1KEEP, [x26, #64]								\n"

	    "	ldp		q0, q1, [x25] 											\n"
	    "	fadd 	v8.4s, v8.4s, v0.4s 								\n"
	    "	prfm 	PLDL1KEEP, [x27, #64]								\n"
	    "	ldr  	q2, [x25, #32]   										\n"
	    "	fadd 	v9.4s, v9.4s, v1.4s 								\n"
	    "	ldp		q3, q4, [x26] 											\n"
	    "	prfm 	PLDL1KEEP, [x28, #64]								\n"
	    "	fadd 	v10.4s, v10.4s, v2.4s 							\n"

	    "	prfm 	PLDL1KEEP, [x15, #64]								\n"
	    "	ldr 	q5, [x26, #32] 											\n"
	    "	fadd 	v11.4s, v11.4s, v3.4s 							\n"
	    "	prfm 	PLDL1KEEP, [x16, #64]								\n"
	    "	ldp		q6, q7, [x27] 											\n"
	    "	fadd 	v12.4s, v12.4s, v4.4s 							\n"
	    "	prfm 	PLDL1KEEP, [x17, #64]								\n"
	    "	ldr 	q0, [x27, #32] 											\n"
	    "	fadd 	v13.4s, v13.4s, v5.4s 							\n"

	    "	prfm 	PLDL1KEEP, [x18, #64]								\n"

	    "	ldp 	q1, q2, [x28] 											\n"
	    "	fadd 	v14.4s, v14.4s, v6.4s 							\n"
	    "	ldr 	q3, [x28, #32] 											\n"
	    "	fadd 	v15.4s, v15.4s, v7.4s 							\n"
	    "	fadd 	v16.4s, v16.4s, v0.4s 							\n"

	    "	ldp 	q4, q5, [x15] 											\n"
	    "	fadd 	v17.4s, v17.4s, v1.4s 							\n"
	    "	ldr  	q6, [x15, #32] 											\n"
	    "	fadd 	v18.4s, v18.4s, v2.4s 							\n"
	    "	ldr 	q7, [x16] 													\n"
	    "	fadd 	v19.4s, v19.4s, v3.4s 							\n"

	    "	ldp 	q0, q1, [x16, #16] 									\n"
	    "	fadd 	v20.4s, v20.4s, v4.4s 							\n"
	    "	ldp 	q2, q3, [x17] 											\n"
	    "	fadd 	v21.4s, v21.4s, v5.4s 							\n"
	    "	ldr 	q4, [x17, #32]											\n"
	    "	fadd 	v22.4s, v22.4s, v6.4s 							\n"


	    "	ldp 	q5, q6, [x18]  											\n"
	    "	fadd 	v23.4s, v23.4s, v7.4s 				 			\n"
	    "	ldr 	q7, [x18, #32]											\n"
	    "	fadd 	v24.4s, v24.4s, v0.4s 							\n"
	    "	fadd 	v25.4s, v25.4s, v1.4s 							\n"

	    "	fadd 	v26.4s, v26.4s, v2.4s 							\n"
	    "	fadd 	v27.4s, v27.4s, v3.4s 							\n"
	    "	fadd 	v28.4s, v28.4s, v4.4s 							\n"


	    "	fadd 	v29.4s, v29.4s, v5.4s 							\n"
	    "	fadd 	v30.4s, v30.4s, v6.4s 							\n"
	    "	fadd 	v31.4s, v31.4s, v7.4s 							\n"

	    ".endm	 																		\n"



	    ".macro	SAVE8x12														\n"


	    "	subs	x21, x21, #1 												\n"

	    "	stp 	q8, q9, [x25]												\n"
	    "	str		q10, [x25, #32]											\n"
	    "	add  	x25, x25, x9, lsl #5 								\n"

	    "	prfm 	PLDL2KEEP, [x25, x29]								\n"

	    "	stp 	q11, q12, [x26]											\n"
	    "	str		q13, [x26, #32]											\n"
	    "	add  	x26, x26, x9, lsl #5 								\n"

	    "	prfm 	PLDL2KEEP, [x26, #64]								\n"

	    "	stp 	q14, q15, [x27]											\n"
	    "	str		q16, [x27, #32]											\n"
	    "	add  	x27, x27, x9, lsl #5 								\n"

	    "	prfm 	PLDL2KEEP, [x27, #64]								\n"

	    "	stp 	q17, q18, [x28]											\n"
	    "	str		q19, [x28, #32]											\n"
	    "	add  	x28, x28, x9, lsl #5 								\n"

	    "	prfm 	PLDL2KEEP, [x28, #64]								\n"

	    "	stp 	q20, q21, [x15]											\n"
	    "	str		q22, [x15, #32]											\n"
	    "	add  	x15, x15, x9, lsl #5 								\n"

	    "	prfm 	PLDL2KEEP, [x15, #64]								\n"

	    "	stp 	q23, q24, [x16]											\n"
	    "	str		q25, [x16, #32]											\n"
	    "	add  	x16, x16, x9, lsl #5 								\n"

	    "	prfm 	PLDL2KEEP, [x16, #64]								\n"

	    "	stp 	q26, q27, [x17]											\n"
	    "	str		q28, [x17, #32]											\n"
	    "	add  	x17, x17, x9, lsl #5 								\n"

	    "	prfm 	PLDL2KEEP, [x17, #64]								\n"

	    "	stp 	q29, q30, [x18]											\n"
	    "	str		q31, [x18, #32]											\n"
	    "	add  	x18, x18, x9, lsl #5 								\n"

	    "	prfm 	PLDL2KEEP, [x18, #64]								\n"

	    ".endm	 																		\n"



	    ".macro	KERNEL8x12_BEGIN_K									\n"


	    "	ldp		q0, q1, [x11], #32									\n"
	    "	prfm	PLDL1KEEP, [x11, #2560]							\n"
	    "	ldr		q2, [x24]														\n"

	    "	fmul	v8.4s,	v2.4s, v0.s[0]							\n"
	    "	fmul	v11.4s,	v2.4s, v0.s[1]							\n"
	    "	ldr 	q3, [x24, #16] 											\n"
	    "	fmul	v14.4s,	v2.4s, v0.s[2]							\n"
	    "	fmul	v17.4s,	v2.4s, v0.s[3]							\n"
	    "	ldr 	q4, [x24, #32] 											\n"
	    "	fmul	v20.4s,	v2.4s, v1.s[0]							\n"
	    "	fmul	v23.4s,	v2.4s, v1.s[1]							\n"
	    "	ldr 	q7, [x24, #48] 											\n"
	    "	fmul	v26.4s,	v2.4s, v1.s[2]							\n"
	    "	fmul	v29.4s,	v2.4s, v1.s[3]							\n"

	    "	ldr 	q2, [x24, #64] 											\n"

	    "	fmul	v9.4s,	v3.4s, v0.s[0]							\n"
	    "	fmul	v12.4s,	v3.4s, v0.s[1]							\n"
	    "	ldr 	q5, [x11], #16											\n"
	    "	fmul	v15.4s,	v3.4s, v0.s[2]							\n"
	    "	fmul	v18.4s,	v3.4s, v0.s[3]							\n"
	    "	ldr 	q6, [x11], #16											\n"
	    "	fmul	v21.4s,	v3.4s, v1.s[0]							\n"
	    "	fmul	v24.4s,	v3.4s, v1.s[1]							\n"
	    "	fmul	v27.4s,	v3.4s, v1.s[2]							\n"
	    "	fmul	v30.4s,	v3.4s, v1.s[3]							\n"

	    "	ldr 	q3, [x24, #80] 											\n"

	    "	fmul	v10.4s,	v4.4s, v0.s[0]							\n"
	    "	fmul	v13.4s,	v4.4s, v0.s[1]							\n"
	    "	fmul	v16.4s,	v4.4s, v0.s[2]							\n"
	    "	prfm	PLDL1KEEP, [x11, #2560]							\n"
	    "	fmul	v19.4s,	v4.4s, v0.s[3]							\n"
	    "	fmul	v22.4s,	v4.4s, v1.s[0]							\n"
	    "	fmul	v25.4s,	v4.4s, v1.s[1]							\n"
	    "	fmul	v28.4s,	v4.4s, v1.s[2]							\n"
	    "	fmul	v31.4s,	v4.4s, v1.s[3]							\n"

	    "	add 	x24, x24, #96 											\n"

	    ".endm 																			\n"



	    ".macro	KERNEL8x12_K0												\n"

	    "	ldr 	q5, [x11], #16											\n"
	    "	fmla	v8.4s,	v2.4s, v0.s[0]							\n"
	    "	fmla	v11.4s,	v2.4s, v0.s[1]							\n"
	    "	ldr 	q6, [x11], #16											\n"
	    "	fmla	v14.4s,	v2.4s, v0.s[2]							\n"
	    "	fmla	v17.4s,	v2.4s, v0.s[3]							\n"
	    "	ldr 	q7, [x24]														\n"
	    "	fmla	v20.4s,	v2.4s, v1.s[0]							\n"
	    "	fmla	v23.4s,	v2.4s, v1.s[1]							\n"
	    "	fmla	v26.4s,	v2.4s, v1.s[2]							\n"
	    "	fmla	v29.4s,	v2.4s, v1.s[3]							\n"

	    "	ldr 	q2, [x24, #16] 											\n"

	    "	fmla	v9.4s,	v3.4s, v0.s[0]							\n"
	    "	fmla	v12.4s,	v3.4s, v0.s[1]							\n"
	    "	fmla	v15.4s,	v3.4s, v0.s[2]							\n"
	    "	fmla	v18.4s,	v3.4s, v0.s[3]							\n"
	    "	prfm PLDL1KEEP, [x11, #2560] 							\n"
	    "	fmla	v21.4s,	v3.4s, v1.s[0]							\n"
	    "	fmla	v24.4s,	v3.4s, v1.s[1]							\n"
	    "	fmla	v27.4s,	v3.4s, v1.s[2]							\n"
	    "	fmla	v30.4s,	v3.4s, v1.s[3]							\n"

	    "	ldr 	q3, [x24, #32] 											\n"

	    "	fmla	v10.4s,	v4.4s, v0.s[0]							\n"
	    "	fmla	v13.4s,	v4.4s, v0.s[1]							\n"
	    "	fmla	v16.4s,	v4.4s, v0.s[2]							\n"
	    "	fmla	v19.4s,	v4.4s, v0.s[3]							\n"
	    "	prfm PLDL1KEEP, [x11, #2624] 							\n"
	    "	fmla	v22.4s,	v4.4s, v1.s[0]							\n"
	    "	fmla	v25.4s,	v4.4s, v1.s[1]							\n"
	    "	fmla	v28.4s,	v4.4s, v1.s[2]							\n"
	    "	fmla	v31.4s,	v4.4s, v1.s[3]							\n"

	    "	add 	x24, x24, #48 											\n"

	    ".endm 																			\n"



	    ".macro	KERNEL8x12_K1												\n"

	    "	ldr 	q0, [x11], #16											\n"
	    "	fmla	v8.4s,	v7.4s, v5.s[0]							\n"
	    "	fmla	v11.4s,	v7.4s, v5.s[1]							\n"
	    "	ldr 	q1, [x11], #16											\n"
	    "	fmla	v14.4s,	v7.4s, v5.s[2]							\n"
	    "	fmla	v17.4s,	v7.4s, v5.s[3]							\n"
	    "	ldr 	q4, [x24] 													\n"
	    "	fmla	v20.4s,	v7.4s, v6.s[0]							\n"
	    "	fmla	v23.4s,	v7.4s, v6.s[1]							\n"
	    "	fmla	v26.4s,	v7.4s, v6.s[2]							\n"
	    "	fmla	v29.4s,	v7.4s, v6.s[3]							\n"

	    "	ldr 	q7, [x24, #16] 											\n"

	    "	fmla	v9.4s,	v2.4s, v5.s[0]							\n"
	    "	fmla	v12.4s,	v2.4s, v5.s[1]							\n"
	    "	fmla	v15.4s,	v2.4s, v5.s[2]							\n"
	    "	fmla	v18.4s,	v2.4s, v5.s[3]							\n"
	    "	prfm PLDL1KEEP, [x24, #256] 							\n"
	    "	fmla	v21.4s,	v2.4s, v6.s[0]							\n"
	    "	fmla	v24.4s,	v2.4s, v6.s[1]							\n"
	    "	fmla	v27.4s,	v2.4s, v6.s[2]							\n"
	    "	fmla	v30.4s,	v2.4s, v6.s[3]							\n"

	    "	ldr 	q2, [x24, #32] 											\n"

	    "	fmla	v10.4s,	v3.4s, v5.s[0]							\n"
	    "	fmla	v13.4s,	v3.4s, v5.s[1]							\n"
	    "	fmla	v16.4s,	v3.4s, v5.s[2]							\n"
	    "	fmla	v19.4s,	v3.4s, v5.s[3]							\n"
	    "	fmla	v22.4s,	v3.4s, v6.s[0]							\n"
	    "	fmla	v25.4s,	v3.4s, v6.s[1]							\n"
	    "	fmla	v28.4s,	v3.4s, v6.s[2]							\n"
	    "	fmla	v31.4s,	v3.4s, v6.s[3]							\n"

	    "	add 	x24, x24, #48 											\n"


	    ".endm 																			\n"


	    ".macro	KERNEL8x12_K2												\n"

	    "	ldr 	q5, [x11], #16											\n"
	    "	fmla	v8.4s,	v4.4s, v0.s[0]							\n"
	    "	fmla	v11.4s,	v4.4s, v0.s[1]							\n"
	    "	ldr 	q6, [x11], #16											\n"
	    "	fmla	v14.4s,	v4.4s, v0.s[2]							\n"
	    "	fmla	v17.4s,	v4.4s, v0.s[3]							\n"
	    "	ldr 	q3, [x24] 													\n"
	    "	fmla	v20.4s,	v4.4s, v1.s[0]							\n"
	    "	fmla	v23.4s,	v4.4s, v1.s[1]							\n"
	    "	fmla	v26.4s,	v4.4s, v1.s[2]							\n"
	    "	fmla	v29.4s,	v4.4s, v1.s[3]							\n"

	    "	ldr 	q4, [x24, #16] 											\n"

	    "	fmla	v9.4s,	v7.4s, v0.s[0]							\n"
	    "	fmla	v12.4s,	v7.4s, v0.s[1]							\n"
	    "	fmla	v15.4s,	v7.4s, v0.s[2]							\n"
	    "	fmla	v18.4s,	v7.4s, v0.s[3]							\n"
	    "	prfm PLDL1KEEP, [x11, #2560] 							\n"
	    "	fmla	v21.4s,	v7.4s, v1.s[0]							\n"
	    "	fmla	v24.4s,	v7.4s, v1.s[1]							\n"
	    "	fmla	v27.4s,	v7.4s, v1.s[2]							\n"
	    "	fmla	v30.4s,	v7.4s, v1.s[3]							\n"

	    "	ldr 	q7, [x24, #32] 											\n"

	    "	fmla	v10.4s,	v2.4s, v0.s[0]							\n"
	    "	fmla	v13.4s,	v2.4s, v0.s[1]							\n"
	    "	fmla	v16.4s,	v2.4s, v0.s[2]							\n"
	    "	prfm PLDL1KEEP, [x11, #2624] 							\n"
	    "	fmla	v19.4s,	v2.4s, v0.s[3]							\n"
	    "	fmla	v22.4s,	v2.4s, v1.s[0]							\n"
	    "	fmla	v25.4s,	v2.4s, v1.s[1]							\n"
	    "	fmla	v28.4s,	v2.4s, v1.s[2]							\n"
	    "	fmla	v31.4s,	v2.4s, v1.s[3]							\n"

	    "	add 	x24, x24, #48 											\n"


	    ".endm 																			\n"



	    ".macro	KERNEL8x12_K3												\n"

	    "	ldr 	q0, [x11], #16											\n"
	    "	fmla	v8.4s,	v3.4s, v5.s[0]							\n"
	    "	fmla	v11.4s,	v3.4s, v5.s[1]							\n"
	    "	ldr 	q1, [x11], #16											\n"
	    "	fmla	v14.4s,	v3.4s, v5.s[2]							\n"
	    "	fmla	v17.4s,	v3.4s, v5.s[3]							\n"
	    "	ldr 	q2, [x24] 													\n"
	    "	fmla	v20.4s,	v3.4s, v6.s[0]							\n"
	    "	fmla	v23.4s,	v3.4s, v6.s[1]							\n"
	    "	fmla	v26.4s,	v3.4s, v6.s[2]							\n"
	    "	fmla	v29.4s,	v3.4s, v6.s[3]							\n"

	    "	ldr 	q3, [x24, #16] 											\n"

	    "	fmla	v9.4s,	v4.4s, v5.s[0]							\n"
	    "	fmla	v12.4s,	v4.4s, v5.s[1]							\n"
	    "	fmla	v15.4s,	v4.4s, v5.s[2]							\n"
	    "	fmla	v18.4s,	v4.4s, v5.s[3]							\n"
	    "	prfm PLDL1KEEP, [x24, #256] 							\n"
	    "	fmla	v21.4s,	v4.4s, v6.s[0]							\n"
	    "	fmla	v24.4s,	v4.4s, v6.s[1]							\n"
	    "	fmla	v27.4s,	v4.4s, v6.s[2]							\n"
	    "	fmla	v30.4s,	v4.4s, v6.s[3]							\n"

	    "	ldr 	q4, [x24, #32] 											\n"

	    "	fmla	v10.4s,	v7.4s, v5.s[0]							\n"
	    "	fmla	v13.4s,	v7.4s, v5.s[1]							\n"
	    "	fmla	v16.4s,	v7.4s, v5.s[2]							\n"
	    "	fmla	v19.4s,	v7.4s, v5.s[3]							\n"
	    "	fmla	v22.4s,	v7.4s, v6.s[0]							\n"
	    "	fmla	v25.4s,	v7.4s, v6.s[1]							\n"
	    "	fmla	v28.4s,	v7.4s, v6.s[2]							\n"
	    "	fmla	v31.4s,	v7.4s, v6.s[3]							\n"

	    "	add 	x24, x24, #48 											\n"


	    ".endm 																			\n"



	    ".macro	KERNEL8x12_END_K										\n"

	    "	fmla	v8.4s,	v3.4s, v5.s[0]							\n"
	    "	fmla	v11.4s,	v3.4s, v5.s[1]							\n"
	    "	fmla	v14.4s,	v3.4s, v5.s[2]							\n"
	    "	fmla	v17.4s,	v3.4s, v5.s[3]							\n"
	    "	fmla	v20.4s,	v3.4s, v6.s[0]							\n"
	    "	fmla	v23.4s,	v3.4s, v6.s[1]							\n"
	    "	fmla	v26.4s,	v3.4s, v6.s[2]							\n"
	    "	fmla	v29.4s,	v3.4s, v6.s[3]							\n"

	    "	fmla	v9.4s,	v4.4s, v5.s[0]							\n"
	    "	fmla	v12.4s,	v4.4s, v5.s[1]							\n"
	    "	fmla	v15.4s,	v4.4s, v5.s[2]							\n"
	    "	fmla	v18.4s,	v4.4s, v5.s[3]							\n"
	    "	fmla	v21.4s,	v4.4s, v6.s[0]							\n"
	    "	fmla	v24.4s,	v4.4s, v6.s[1]							\n"
	    "	fmla	v27.4s,	v4.4s, v6.s[2]							\n"
	    "	fmla	v30.4s,	v4.4s, v6.s[3]							\n"

	    "	fmla	v10.4s,	v7.4s, v5.s[0]							\n"
	    "	fmla	v13.4s,	v7.4s, v5.s[1]							\n"
	    "	fmla	v16.4s,	v7.4s, v5.s[2]							\n"
	    "	fmla	v19.4s,	v7.4s, v5.s[3]							\n"
	    "	fmla	v22.4s,	v7.4s, v6.s[0]							\n"
	    "	fmla	v25.4s,	v7.4s, v6.s[1]							\n"
	    "	fmla	v28.4s,	v7.4s, v6.s[2]							\n"
	    "	fmla	v31.4s,	v7.4s, v6.s[3]							\n"

	    ".endm 																			\n"


	    ".macro	KERNEL8x12_RIGHT_K 									\n"


	    "	ldp		q2, q3, [x24], #32									\n"
	    "	ldr		q0, [x11], #16 											\n"

	    "	fmla	v8.4s, v2.4s, v0.s[0]								\n"
	    "	ldr		q4, [x24], #16											\n"
	    "	fmla	v9.4s, v3.4s, v0.s[0]								\n"
	    "	fmla	v10.4s, v4.4s, v0.s[0]							\n"

	    "	fmla	v11.4s, v2.4s, v0.s[1]							\n"
	    "	ldr		q1, [x11], #16 											\n"
	    "	fmla	v12.4s, v3.4s, v0.s[1]							\n"
	    "	prfm	PLDL1KEEP, [x12, #64]								\n"
	    "	fmla	v13.4s, v4.4s, v0.s[1]							\n"

	    "	fmla	v14.4s, v2.4s, v0.s[2]							\n"
	    "	fmla	v15.4s, v3.4s, v0.s[2]							\n"
	    "	fmla	v16.4s, v4.4s, v0.s[2]							\n"

	    "	fmla	v17.4s, v2.4s, v0.s[3]							\n"
	    "	fmla	v18.4s, v3.4s, v0.s[3]							\n"
	    "	fmla	v19.4s, v4.4s, v0.s[3]							\n"

	    "	fmla	v20.4s, v2.4s, v1.s[0]							\n"
	    "	fmla	v21.4s, v3.4s, v1.s[0]							\n"
	    "	fmla	v22.4s, v4.4s, v1.s[0]							\n"

	    "	fmla	v23.4s, v2.4s, v1.s[1]							\n"
	    "	fmla	v24.4s, v3.4s, v1.s[1]							\n"
	    "	fmla	v25.4s, v4.4s, v1.s[1]							\n"

	    "	fmla	v26.4s, v2.4s, v1.s[2]							\n"
	    "	fmla	v27.4s, v3.4s, v1.s[2]							\n"
	    "	fmla	v28.4s, v4.4s, v1.s[2]							\n"

	    "	fmla	v29.4s, v2.4s, v1.s[3]							\n"
	    "	fmla	v30.4s, v3.4s, v1.s[3]							\n"
	    "	fmla	v31.4s, v4.4s, v1.s[3]							\n"


	    ".endm 																			\n"



	    //----------------------------------------------------

	    "SMM_NN:									 	 								\n"

	    "	ldr		x0, %[C]						 								\n"
	    "	ldr		x1, %[A]						 								\n"
	    "	ldr		x2, %[B]						 								\n"

	    "	ldr		x3, %[M]						 								\n"
	    "	ldr		x4, %[N]						 								\n"
	    "	ldr		x5, %[K]						 								\n"
	    "	ldr		x9, %[LN] 						 							\n"
	    "	ldr		x6, %[LK]						 								\n"

	    "	prfm		PLDL1KEEP, [x1, #512]            	\n"
	    "	prfm    PLDL1KEEP, [x2, #64]            	\n"

	    "	ldr		x10, %[SB]						 							\n"
	    "	ldr		x8, %[k_tag] 					 							\n"


	    "	mov		x21, #12 														\n"
	    "	udiv	x20, x4, x21						 						\n"          // N / 12


	    //----------------------------------------------------LOOP

	    "BEGIN_N12:								 	 								\n"

	    "	mov 	x25, x0   						 							\n"   //C1*
	    "	prfm	PLDL2KEEP, [x25, #64]								\n"
	    "	add		x26, x25, x9, lsl #2 			 					\n"   //C2*
	    "	prfm	PLDL2KEEP, [x26, #64]								\n"
	    "	add 	x27, x25, x9, lsl #3 			 					\n"   //C3*
	    "	prfm	PLDL2KEEP, [x27, #64]								\n"
	    "	add 	x28, x26, x9, lsl #3 			 					\n"   //C4*
	    "	prfm	PLDL2KEEP, [x28, #64]								\n"

	    "	add 	x15, x27, x9, lsl #3 								\n"		//C5*
	    "	prfm	PLDL2KEEP, [x15, #64]								\n"
	    "	add 	x16, x28, x9, lsl #3 								\n"		//C6*
	    "	prfm	PLDL2KEEP, [x16, #64]								\n"
	    "	add 	x17, x15, x9, lsl #3 								\n"		//C7*
	    "	prfm	PLDL2KEEP, [x17, #64]								\n"
	    "	add 	x18, x16, x9, lsl #3 								\n"		//C8*
	    "	prfm	PLDL2KEEP, [x18, #64]								\n"

	    "	mov		x11, x1   						 							\n"   // address_A
	    "	lsr		x21, x3, #3						 							\n"   // M / 8



	    "	mov		x29, #128 													\n"   // prfm  bytes to L2
	    "	cmp		x20, #1 														\n"
	    "	bgt 	BEGIN_PACKB													\n"
	    "	mov 	x29, #0 														\n"

	    //-----------------------------------------------------PACK

	    "BEGIN_PACKB:								 								\n"

	    "	mov		x24, x10						 								\n"   //还原SB的地址

	    "	mov		x12, x2						 	 								\n"   // B0*
	    "	add		x13, x12, x9, lsl #2 			 					\n"   // B0 + sizeof(float) * LN

	    "	prfm    PLDL1KEEP, [x12, #64]			 				\n"
	    "	prfm    PLDL1KEEP, [x13, #64]			 				\n"
	    "	prfm    PLDL1KEEP, [x11, #256]			 			\n"


	    "PACK_Body_K:								 								\n"

	    "	lsr		x22, x5, #2 					 							\n"     // K / 8

	    "	PACK_KERNEL8x12_BEGIN_K				 	 					\n"

	    "	subs	x22, x22, #1		     		 						\n"
	    "	b 		PACK_K1_7						 								\n"

	    "PACK_K:									 									\n"

	    "	PACK_KERNEL8x12_K0						 						\n"
	    "PACK_K1_7:									 								\n"

	    "	PACK_KERNEL8x12_K1						 						\n"
	    "	PACK_KERNEL8x12_K0						 						\n"
//		"	PACK_KERNEL8x12_K1						 						\n"
//		"	PACK_KERNEL8x12_K0						 						\n"
//		"	PACK_KERNEL8x12_K1						 						\n"
//		"	PACK_KERNEL8x12_K0						 						\n"

	    "	beq		PACK_Edge_K						 							\n"

	    "	PACK_KERNEL8x12_K1						 						\n"

	    "	subs	x22, x22, #1			 		 							\n"
	    "	b 		PACK_K			 				 								\n"

	    "PACK_Edge_K:							     							\n"

	    "	PACK_KERNEL8x12_END_K			         				\n"

	    "	ands 	x22, x5, #3 												\n"
	    "	beq		BEGIN_PACK_SAVE											\n"

	    "PACK_RIGHT_LOOP_K:													\n"
	    "	subs 	x22, x22, #1 												\n"
	    "	PACK_KERNEL8x12_RIGHT_K										\n"
	    "	bgt 	PACK_RIGHT_LOOP_K										\n"

	    "BEGIN_PACK_SAVE:														\n"

	    "	cmp		x8, #0							 								\n"
	    "	beq		M8N12_PACK_SAVE 					 					\n"
	    "	M8N12_PACK_ADD_C													\n"
	    "M8N12_PACK_SAVE: 							 						\n"
	    "	SAVE8x12								 									\n"

	    "	beq		M8_END							 								\n"



	    //----------------------------------------------------------
	    "BEGIN_M8:                          		 		\n"

	    " mov     x24, x10                    		 	\n"   //  address_B, x11 is address_A + offset
	    " prfm    PLDL1KEEP, [x24, #128]    		 		\n"
	    " prfm    PLDL1KEEP, [x11, #256]    		 		\n"

	    "Body_K:                            		 		\n"

	    " lsr   x22, x5, #3                 		 		\n"     // K / 8
	    " KERNEL8x12_BEGIN_K                		 	 	\n"
	    " subs  x22, x22, #1                		 		\n"
	    " b 	K1_7                            	 		\n"

	    "Main_K:                            		 		\n"

	    " KERNEL8x12_K0                     				\n"
	    "K1_7:                              				\n"

	    " KERNEL8x12_K1                     		 	 	\n"
	    " KERNEL8x12_K2                     		 	 	\n"
	    " KERNEL8x12_K3                     		 	 	\n"
	    " KERNEL8x12_K0                     		 	 	\n"
	    " KERNEL8x12_K1                     		 	 	\n"
	    " KERNEL8x12_K2                     			 	\n"

	    " beq   Edge_K                      		 		\n"

	    " KERNEL8x12_K3                     		 	 	\n"

	    " subs  x22, x22, #1                		 		\n"
	    " b     Main_K                      		 		\n"

	    "Edge_K:                            		 		\n"

	    " KERNEL8x12_END_K                 			 		\n"

	    "	ands x22, x5, #7 													\n"
	    "	beq		BEGIN_SAVE8x12											\n"

	    "RIGHT_LOOP_K:															\n"
	    "	subs 	x22, x22, #1 												\n"
	    "	KERNEL8x12_RIGHT_K 												\n"
	    "	bgt 	RIGHT_LOOP_K												\n"

	    "BEGIN_SAVE8x12:														\n"

	    "	cmp		x8, #0							 								\n"
	    "	beq		M8N12_SAVE 					 	 							\n"
	    "	M8N12_PACK_ADD_C 							 	 					\n"
	    "M8N12_SAVE: 							     							\n"
	    "	SAVE8x12								 	 								\n"

	    " bgt     BEGIN_M8                					\n"
	    "M8_END:									 									\n"

	    "	subs	x20, x20, #1					 							\n"
	    "	add		x0, x0, #48						 							\n"
	    "	add   x2, x2, #48  												\n"  // address_B + offset
	    "	bgt		BEGIN_N12						 								\n"

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