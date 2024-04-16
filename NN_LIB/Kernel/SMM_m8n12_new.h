#pragma once
void SMM_NN_KERNEL8x1(float *C, float *A, float *B, long	M, long N, long K,
                      long LN, long LK, float *Bc, float *SB,
                      long k_tag, long m_tag, long id_tag)
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
	    "	prfm	PLDL1KEEP, [x11, #512]						\n"
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
	    "	st1   	{v5.s}[0], [x24], #4 						\n"

	    ".endm 													\n"


	    ".macro	PACK_KERNEL8x1_END_K							\n"

	    "	fmla	v30.4s, v3.4s, v5.s[0]						\n"
	    "	fmla	v31.4s, v4.4s, v5.s[0]						\n"
	    "	st1		{v5.s}[0], [x24], #4 						\n"

	    ".endm 													\n"


	    ".macro	PACK_KERNEL8x1_RIGHT_K_NGM 						\n"


	    "	ldr		s2, [x12]									\n"
	    "	ldp		q0, q1, [x11], #32 							\n"
	    "	add 	x12, x12, x9, lsl #2 						\n"
	    "	fmla	v30.4s, v0.4s, v2.s[0]						\n"
	    "	fmla	v31.4s, v1.4s, v2.s[0]						\n"

	    "	str  	s2, [x24], #4 								\n"

	    ".endm 													\n"



	    ".macro	M8N1_PACK_ADD_C									\n"

	    "	ld1		{v0.s}[0], [x25] 							\n"
	    "	ld1		{v0.s}[1], [x26] 							\n"
	    "	ld1		{v0.s}[2], [x27] 							\n"
	    "	ld1		{v0.s}[3], [x28] 							\n"

	    "	fadd 	v30.4s, v30.4s, v0.4s 						\n"

	    "	ld1		{v1.s}[0], [x15] 							\n"
	    "	ld1		{v1.s}[1], [x16] 							\n"
	    "	ld1		{v1.s}[2], [x17] 							\n"
	    "	ld1		{v1.s}[3], [x18] 							\n"

	    "	fadd 	v31.4s, v31.4s, v1.4s 						\n"

	    ".endm 													\n"


	    ".macro	SAVE8x1								 			\n"

	    "	subs	x21, x21, #1 								\n"

	    "	st1 	{v30.s}[0], [x25] 							\n"
	    "	add  	x25, x25, x9, lsl #5 						\n"

	    "	st1 	{v30.s}[1], [x26] 							\n"
	    "	add  	x26, x26, x9, lsl #5 						\n"

	    "	st1 	{v30.s}[2], [x27] 							\n"
	    "	add  	x27, x27, x9, lsl #5 						\n"

	    "	st1 	{v30.s}[3], [x28] 							\n"
	    "	add  	x28, x28, x9, lsl #5 						\n"

	    "	st1 	{v31.s}[0], [x15] 							\n"
	    "	add  	x15, x15, x9, lsl #5 						\n"

	    "	st1 	{v31.s}[1], [x16] 							\n"
	    "	add  	x16, x16, x9, lsl #5 						\n"

	    "	st1 	{v31.s}[2], [x17] 							\n"
	    "	add  	x17, x17, x9, lsl #5 						\n"

	    "	st1 	{v31.s}[3], [x18] 							\n"
	    "	add  	x18, x18, x9, lsl #5 						\n"

	    ".endm 													\n"


	    ".macro KERNEL8x1_BEGIN_K 								\n"

	    "	ldp		q0,	q1, [x11], #32							\n"
	    "	prfm	PLDL1KEEP, [x11, #512]						\n"

	    "	ldr 	s2, [x24], #4								\n"
	    "	fmul	v30.4s, v0.4s, v2.s[0]						\n"
	    "	ldr		s5, [x24], #4								\n"
	    "	fmul	v31.4s, v1.4s, v2.s[0]						\n"

	    "	ldp		q3,	q4, [x11], #32							\n"

	    ".endm 													\n"


	    ".macro KERNEL8x1_K0 											\n"

	    "	ldp		q3,	q4, [x11], #32								\n"
	    "	prfm	PLDL1KEEP, [x24, #128]						\n"

	    "	ldr 	s5, [x24], #4											\n"
	    "	fmla	v30.4s, v0.4s, v2.s[0]						\n"
	    "	fmla	v31.4s, v1.4s, v2.s[0]						\n"

	    ".endm 																		\n"



	    ".macro KERNEL8x1_K1 											\n"

	    "	ldp		q0,	q1, [x11], #32								\n"
	    "	prfm	PLDL1KEEP, [x11, #256]						\n"

	    "	ldr 	s2, [x24], #4											\n"
	    "	fmla	v30.4s, v3.4s, v5.s[0]						\n"
	    "	fmla	v31.4s, v4.4s, v5.s[0]						\n"

	    ".endm 																		\n"


	    ".macro KERNEL8x1_END_K 									\n"

	    "	fmla	v30.4s, v3.4s, v5.s[0]						\n"
	    "	fmla	v31.4s, v4.4s, v5.s[0]						\n"

	    ".endm 																		\n"


	    ".macro	KERNEL8x1_RIGHT_K_NGM 						\n"

	    "	ldr		s2, [x24], #4											\n"
	    "	ldp		q0, q1, [x11], #32 								\n"
	    "	fmla	v30.4s, v0.4s, v2.s[0]						\n"
	    "	fmla	v31.4s, v1.4s, v2.s[0]						\n"

	    ".endm 																		\n"


	    ".macro	M4N1_ADD_C												\n"

	    "	ldr		s0, [x25] 												\n"
	    "	ldr		s1, [x26] 												\n"
	    "	ldr		s2, [x27] 												\n"
	    "	ldr		s3, [x28] 												\n"

	    "	fadd 	s28, s28, s0 											\n"
	    "	fadd 	s29, s29, s1 											\n"
	    "	fadd 	s30, s30, s2 											\n"
	    "	fadd 	s31, s31, s3 											\n"

	    ".endm 																		\n"


	    ".macro	SAVE4x1														\n"

	    "	str 	s28, [x25] 												\n"
	    "	add 	x25, x25, x9, lsl #4 							\n"
	    "	str 	s29, [x26] 												\n"
	    "	add 	x26, x26, x9, lsl #4 							\n"
	    "	str 	s30, [x27] 												\n"
	    "	add 	x27, x27, x9, lsl #4 							\n"
	    "	str 	s31, [x28] 												\n"

	    ".endm 																		\n"


	    ".macro	M3N1_ADD_C												\n"

	    "	ldr		s0, [x25] 												\n"
	    "	ldr		s1, [x26] 												\n"
	    "	ldr		s2, [x27] 												\n"

	    "	fadd 	s29, s29, s0 											\n"
	    "	fadd 	s30, s30, s1 											\n"
	    "	fadd 	s31, s31, s2 											\n"

	    ".endm 																		\n"


	    ".macro	SAVE3x1														\n"

	    "	str 	s29, [x25] 												\n"
	    "	add 	x25, x25, x9, lsl #3 							\n"
	    "	add 	x25, x25, x9, lsl #2 							\n"
	    "	str 	s30, [x26] 												\n"
	    "	add 	x26, x26, x9, lsl #3 							\n"
	    "	add 	x26, x26, x9, lsl #2 							\n"
	    "	str 	s31, [x27] 												\n"

	    ".endm 																		\n"


	    ".macro	M2N1_ADD_C												\n"

	    "	ldr		s0, [x25] 												\n"
	    "	ldr		s1, [x26] 												\n"

	    "	fadd 	s30, s30, s0 											\n"
	    "	fadd 	s31, s31, s1 											\n"

	    ".endm 																		\n"


	    ".macro	SAVE2x1														\n"

	    "	str 	s30, [x25] 												\n"
	    "	add 	x25, x25, x9, lsl #3 							\n"
	    "	str 	s31, [x26] 												\n"

	    ".endm 																		\n"



	    "SMM_NN_KERNEL8x1:											\n"

	    "	ldr		x0, %[C]						 				\n"
	    "	ldr		x1, %[A]						 				\n"
	    "	ldr		x2, %[B]						 				\n"

	    "	ldr		x3, %[M]						 				\n"
	    "	ldr		x4, %[N]						 				\n"  // N = 4
	    "	ldr		x5, %[K]						 				\n"
	    "	ldr		x9, %[LN] 						 				\n"
	    "	ldr		x6, %[LK]						 				\n"

	    "	prfm		PLDL1KEEP, [x1, #512]            			\n"

	    "	ldr		x7, %[m_tag] 					 				\n"
	    "	ldr		x8, %[k_tag] 					 				\n"
	    "	ldr		x30, %[id_tag] 					 				\n"

	    "	cmp		x7, #0 											\n"
	    "	bne 	NN8x1_addr 										\n"
	    "	ldr		x10, %[SB]						 				\n"
	    "	cmp		x30, #0 										\n"
	    "	beq		BEGIN_NN8x1 									\n"
	    "NN8x1_addr:												\n"
	    "	ldr 	x10, %[Bc]										\n"


	    "BEGIN_NN8x1: 												\n"

	    "	mov 	x25, x0   						 				\n"   //C1*
	    "	add		x26, x25, x9, lsl #2 			 				\n"   //C2*
	    "	add 	x27, x25, x9, lsl #3 			 				\n"   //C3*
	    "	add 	x28, x26, x9, lsl #3 			 				\n"   //C4*

	    "	add 	x15, x27, x9, lsl #3 							\n"		//C5*
	    "	add 	x16, x28, x9, lsl #3 							\n"		//C6*
	    "	add 	x17, x15, x9, lsl #3 							\n"		//C7*
	    "	add 	x18, x16, x9, lsl #3 							\n"		//C8*

	    "	mov		x11, x1   						 						\n"   // address_A
	    "	lsr		x21, x3, #3						 						\n"   // M / 8


	    "PACK_NN_KERNEL8x1:												\n"

	    "	mov		x24, x10						 							\n"   //还原SB的地址
	    "	mov		x12, x2						 	 							\n"   // B0*
	    "	add		x13, x12, x9, lsl #2 			 				\n"   // B0 + sizeof(float) * LN

	    "	prfm    PLDL1KEEP, [x11, #256]					\n"

	    "PACK_Body_NN_KERNEL8x1:									\n"

	    "	lsr		x22, x5, #2 					 						\n"     // K / 8

	    "	PACK_KERNEL8x1_BEGIN_K				 	 				\n"

	    "	subs	x22, x22, #1		     		 					\n"
	    "	b 		PACK_K1_3_8x1						 					\n"

	    "PACK_K_8x1:									 						\n"

	    "	PACK_KERNEL8x1_K0						 						\n"

	    "PACK_K1_3_8x1:									 					\n"

	    "	PACK_KERNEL8x1_K1						 						\n"
	    "	PACK_KERNEL8x1_K0						 						\n"

	    "	beq		PACK_Edge_K_8x1						 				\n"

	    "	PACK_KERNEL8x1_K1						 							\n"

	    "	subs	x22, x22, #1			 		 							\n"
	    "	b 		PACK_K_8x1			 				 						\n"

	    "PACK_Edge_K_8x1:							     					\n"

	    "	PACK_KERNEL8x1_END_K			        				\n"


	    "	ands 	x22, x5, #3 												\n"
	    "	beq		PACK_BEGIN_SAVE8x1_NGM 							\n"
	    "PACK_RIGHT_LOOP_K_8x1_NGM: 								\n"
	    "	subs 	x22, x22, #1 												\n"
	    "	PACK_KERNEL8x1_RIGHT_K_NGM								\n"
	    "	bgt		PACK_RIGHT_LOOP_K_8x1_NGM						\n"


	    "PACK_BEGIN_SAVE8x1_NGM:										\n"

	    "	cmp		x8, #0							 								\n"
	    "	beq		M8N1_PACK_SAVE 					 						\n"
	    "	M8N1_PACK_ADD_C														\n"

	    "M8N1_PACK_SAVE: 							 							\n"
	    "	SAVE8x1								 										\n"

	    "	beq		M8x1_END							 							\n"



//--------------------------------------------------------------------
	    "BEGIN_M8_KERNEL_8x1:               				\n"


	    " mov     x24, x10                  				\n"   //  address_B, x11 is address_A + offset

	    " prfm    PLDL1KEEP, [x24, #256]    				\n"
	    " prfm    PLDL1KEEP, [x11, #256]    				\n"

	    "Body_K_8x1:                        				\n"

	    " lsr   x22, x5, #2                 				\n"     // K / 4
	    " KERNEL8x1_BEGIN_K                					\n"
	    " subs  x22, x22, #1                				\n"
	    " b 	K1_7_8x1                      				\n"

	    "Main_K_8x1:                        				\n"

	    " KERNEL8x1_K0                     					\n"
	    "K1_7_8x1:                          				\n"

	    " KERNEL8x1_K1                     					\n"
	    " KERNEL8x1_K0                     					\n"

	    " beq   Edge_K_8x1                  				\n"

	    " KERNEL8x1_K1                     					\n"

	    " subs  x22, x22, #1                				\n"
	    " b     Main_K_8x1                  				\n"

	    "Edge_K_8x1:                        				\n"

	    " KERNEL8x1_END_K                 					\n"


	    "	ands 	x22, x5, #3 												\n"
	    "	beq		BEGIN_SAVE8x1_NGM 									\n"
	    "RIGHT_LOOP_K_8x1_NGM: 											\n"
	    "	subs 	x22, x22, #1 												\n"
	    "	KERNEL8x1_RIGHT_K_NGM											\n"
	    "bgt		RIGHT_LOOP_K_8x1_NGM								\n"

	    "BEGIN_SAVE8x1_NGM:													\n"


	    "	cmp		x8, #0							 								\n"
	    "	beq		M8N1_SAVE 					 	 							\n"
	    "	M8N1_PACK_ADD_C 							 						\n"
	    "M8N1_SAVE: 							     							\n"
	    "	SAVE8x1								 	 									\n"

	    "	bgt     BEGIN_M8_KERNEL_8x1     					\n"


//---------------------------------------------------------
	    "	ands	x21, x3, #7 												\n"
	    "BEGIN_M4_KERNEL_4x1: 											\n"

	    "	cmp		x21, #4 														\n"
	    "	blt		BEGIN_M3_KERNEL_3x1 								\n"

	    "	mov     x24, x10                  				\n"
	    " prfm    PLDL1KEEP, [x24, #256]    				\n"
	    " prfm    PLDL1KEEP, [x11, #256]    				\n"

	    "	dup		v28.4s, wzr 												\n"
	    "	dup		v29.4s, wzr 												\n"
	    "	dup		v30.4s, wzr 												\n"
	    "	dup		v31.4s, wzr 												\n"
	    "	mov 	x22, x5 														\n"
	    "Body_K_4x1: 																\n"
	    "	ld4		{v0.s, v1.s, v2.s, v3.s}[0], [x11], #16 	\n"
	    "	ldr 	s4, [x24], #4 											\n"
	    "	subs	x22, x22, #1 												\n"
	    "	fmadd	s28, s4, s0, s28 										\n"
	    "	fmadd	s29, s4, s1, s29 										\n"
	    "	fmadd	s30, s4, s2, s30 										\n"
	    "	fmadd	s31, s4, s3, s31 										\n"
	    "	bgt 	Body_K_4x1													\n"

	    "	cmp		x8, #0							 								\n"
	    "	beq		M4N1_SAVE 					 	 							\n"
	    "	M4N1_ADD_C 							 									\n"
	    "M4N1_SAVE: 							     							\n"
	    "	SAVE4x1								 	 									\n"

	    "	sub 	x21, x21, #4 												\n"



//-----------------------------------------------------
	    "BEGIN_M3_KERNEL_3x1: 											\n"


	    "	cmp		x21, #3 														\n"
	    "	blt		BEGIN_M2_KERNEL_2x1 								\n"

	    "	mov     x24, x10                  				\n"
	    " prfm    PLDL1KEEP, [x24, #256]    				\n"
	    " prfm    PLDL1KEEP, [x11, #256]    				\n"

	    "	dup		v29.4s, wzr 												\n"
	    "	dup		v30.4s, wzr 												\n"
	    "	dup		v31.4s, wzr 												\n"
	    "	mov 	x22, x5 														\n"
	    "Body_K_3x1: 																\n"
	    "	ld3		{v0.s, v1.s, v2.s}[0], [x11], #12 	\n"
	    "	ldr 	s3, [x24], #4 											\n"
	    "	subs	x22, x22, #1 												\n"
	    "	fmadd	s29, s3, s0, s29 										\n"
	    "	fmadd	s30, s3, s1, s30 										\n"
	    "	fmadd	s31, s3, s2, s31 										\n"
	    "	bgt 	Body_K_3x1													\n"

	    "	cmp		x8, #0							 								\n"
	    "	beq		M3N1_SAVE 					 	 							\n"
	    "	M3N1_ADD_C 							 									\n"
	    "M3N1_SAVE: 							     							\n"
	    "	SAVE3x1								 	 									\n"

	    "	subs 	x21, x21, #3 												\n"

//--------------------------------------------------------
	    "BEGIN_M2_KERNEL_2x1: 											\n"

	    "	cmp		x21, #2															\n"
	    "	blt		M8x1_END 														\n"


	    "	mov     x24, x10                    			\n"
	    " prfm    PLDL1KEEP, [x24, #256]    		 		\n"
	    " prfm    PLDL1KEEP, [x11, #256]    		 		\n"

	    "	dup		v30.4s, wzr 												\n"
	    "	dup		v31.4s, wzr 												\n"
	    "	mov 	x22, x5 														\n"
	    "Body_K_2x1: 																\n"
	    "	ld2		{v0.s, v1.s}[0], [x11], #8 					\n"
	    "	ldr 	s2, [x24], #4 											\n"
	    "	subs	x22, x22, #1 												\n"
	    "	fmadd	s30, s2, s0, s30 										\n"
	    "	fmadd	s31, s2, s1, s31 										\n"
	    "	bgt 	Body_K_2x1													\n"

	    "	cmp		x8, #0							 								\n"
	    "	beq		M2N1_SAVE 					 	 							\n"
	    "	M2N1_ADD_C 							 									\n"
	    "M2N1_SAVE: 							     							\n"
	    "	SAVE2x1								 	 									\n"

	    "	subs 	x21, x21, #2 												\n"

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
	    [Bc] "m" (Bc),
	    [SB] "m" (SB),
	    [k_tag] "m" (k_tag),
	    [m_tag] "m" (m_tag),
	    [id_tag] "m" (id_tag)
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


void SMM_NN_KERNEL8x2(float *C, float *A, float *B, long	M, long N, long K,
                      long LN, long LK, float *Bc, float *SB,
                      long k_tag, long m_tag, long id_tag)
{

	asm volatile(

	    ".macro	PACK_KERNEL8x2_BEGIN_K							\n"

	    "	ldr		q0,	[x11], #16								\n"
	    "	prfm	PLDL1KEEP, [x11, #512]						\n"

	    "	ld1		{v2.2s}, [x12]								\n"

	    "	fmul	v28.4s, v0.4s, v2.s[0]						\n"
	    "	ldr		q1,	[x11], #16								\n"
	    "	fmul	v29.4s, v0.4s, v2.s[1]						\n"

	    "	add  	x12, x12, x9, lsl #3 						\n"
	    "	ld1		{v5.2s}, [x13] 								\n"

	    "	fmul	v30.4s, v1.4s, v2.s[0]						\n"
	    "	ldp		q3, q4, [x11], #32 							\n"
	    "	fmul	v31.4s, v1.4s, v2.s[1]						\n"

	    "	add  	x13, x13, x9, lsl #3 						\n"
	    "	st1 	{v2.2s}, [x24], #8 							\n"

	    ".endm 													\n"


	    ".macro	PACK_KERNEL8x2_K0								\n"

	    "	fmla	v28.4s, v0.4s, v2.s[0]						\n"
	    "	ld1		{v5.2s}, [x13]								\n"
	    "	fmla	v29.4s, v0.4s, v2.s[1]						\n"

	    "	add  	x13, x13, x9, lsl #3 						\n"
	    "	ldp		q3, q4, [x11], #32 							\n"

	    "	fmla	v30.4s, v1.4s, v2.s[0]						\n"
	    "	fmla	v31.4s, v1.4s, v2.s[1]						\n"

	    "	st1 	{v2.2s}, [x24], #8 							\n"

	    ".endm 													\n"


	    ".macro	PACK_KERNEL8x2_K1								\n"

	    "	prfm	PLDL1KEEP, [x11, #512]						\n"

	    "	fmla	v28.4s, v3.4s, v5.s[0]						\n"
	    "	ld1		{v2.2s}, [x12]								\n"
	    "	fmla	v29.4s, v3.4s, v5.s[1]						\n"

	    "	add  	x12, x12, x9, lsl #3 							\n"
	    "	ldp		q0, q1, [x11], #32 								\n"

	    "	fmla	v30.4s, v4.4s, v5.s[0]						\n"
	    "	fmla	v31.4s, v4.4s, v5.s[1]						\n"
	    "	st1 	{v5.2s}, [x24], #8 								\n"

	    ".endm 																		\n"


	    ".macro	PACK_KERNEL8x2_END_K							\n"

	    "	fmla	v28.4s, v3.4s, v5.s[0]						\n"
	    "	fmla	v29.4s, v3.4s, v5.s[1]						\n"

	    "	fmla	v30.4s, v4.4s, v5.s[0]						\n"
	    "	fmla	v31.4s, v4.4s, v5.s[1]						\n"

	    "	st1 	{v5.2s}, [x24], #8 								\n"

	    ".endm 																		\n"


	    ".macro	PACK_KERNEL8x2_RIGHT_K_NGM 				\n"


	    "	ld1		{v2.2s}, [x12]										\n"
	    "	ldp		q0, q1, [x11], #32 								\n"

	    "	fmla	v28.4s, v0.4s, v2.s[0]						\n"
	    "	fmla	v29.4s, v0.4s, v2.s[1]						\n"
	    "	add 	x12, x12, x9, lsl #2 							\n"
	    "	fmla	v30.4s, v1.4s, v2.s[0]						\n"
	    "	fmla	v31.4s, v1.4s, v2.s[1]						\n"

	    "	st1  	{v2.2s}, [x24], #8 							  \n"

	    ".endm 																		\n"


	    ".macro	M8N2_PACK_ADD_C										\n"

	    "	ld2		{v0.s, v1.s}[0], [x25] 						\n"
	    "	ld2		{v0.s, v1.s}[1], [x26] 						\n"
	    "	ld2		{v0.s, v1.s}[2], [x27] 						\n"
	    "	ld2		{v0.s, v1.s}[3], [x28] 						\n"

	    "	fadd 	v28.4s, v28.4s, v0.4s 						\n"
	    "	fadd 	v29.4s, v29.4s, v1.4s 						\n"

	    "	ld2		{v2.s, v3.s}[0], [x15] 						\n"
	    "	ld2		{v2.s, v3.s}[1], [x16] 						\n"
	    "	ld2		{v2.s, v3.s}[2], [x17] 						\n"
	    "	ld2		{v2.s, v3.s}[3], [x18] 						\n"

	    "	fadd 	v30.4s, v30.4s, v2.4s 						\n"
	    "	fadd 	v31.4s, v31.4s, v3.4s 						\n"

	    ".endm 																		\n"


	    ".macro	SAVE8x2								 						\n"

	    "	subs	x21, x21, #1 											\n"

	    "	st2 	{v28.s, v29.s}[0], [x25] 					\n"
	    "	add  	x25, x25, x9, lsl #5 							\n"
	    "	prfm 	PLDL2KEEP, [x25, #8]							\n"

	    "	st2 	{v28.s, v29.s}[1], [x26] 					\n"
	    "	add  	x26, x26, x9, lsl #5 							\n"
	    "	prfm 	PLDL2KEEP, [x26, #8]							\n"

	    "	st2 	{v28.s, v29.s}[2], [x27] 					\n"
	    "	add  	x27, x27, x9, lsl #5 							\n"
	    "	prfm 	PLDL2KEEP, [x27, #8]							\n"

	    "	st2 	{v28.s, v29.s}[3], [x28] 					\n"
	    "	add  	x28, x28, x9, lsl #5 							\n"
	    "	prfm 	PLDL2KEEP, [x28, #8]							\n"


	    "	st2 	{v30.s, v31.s}[0], [x15] 					\n"
	    "	add  	x15, x15, x9, lsl #5 							\n"
	    "	prfm 	PLDL2KEEP, [x15, #8]							\n"

	    "	st2 	{v30.s, v31.s}[1], [x16] 					\n"
	    "	add  	x16, x16, x9, lsl #5 							\n"
	    "	prfm 	PLDL2KEEP, [x16, #8]							\n"

	    "	st2 	{v30.s, v31.s}[2], [x17] 					\n"
	    "	add  	x17, x17, x9, lsl #5 							\n"
	    "	prfm 	PLDL2KEEP, [x17, #8]							\n"

	    "	st2 	{v30.s, v31.s}[3], [x18] 					\n"
	    "	add  	x18, x18, x9, lsl #5 							\n"
	    "	prfm 	PLDL2KEEP, [x18, #8]							\n"

	    ".endm 																		\n"


	    ".macro KERNEL8x2_BEGIN_K 								\n"

	    "	ldr		q0,	[x11], #16										\n"
	    "	prfm	PLDL1KEEP, [x11, #512]						\n"

	    "	ld1		{v2.2s}, [x24], #8 								\n"

	    "	fmul	v28.4s, v0.4s, v2.s[0]						\n"
	    "	ldr		q1,	[x11], #16										\n"
	    "	fmul	v29.4s, v0.4s, v2.s[1]						\n"

	    "	ld1		{v5.2s}, [x24], #8								\n"

	    "	fmul	v30.4s, v1.4s, v2.s[0]						\n"
	    "	ldp		q3, q4, [x11], #32 								\n"
	    "	fmul	v31.4s, v1.4s, v2.s[1]						\n"

	    ".endm 																		\n"


	    ".macro KERNEL8x2_K0 											\n"

	    "	prfm 	PLDL1KEEP, [x24, #128]						\n"

	    "	fmla	v28.4s, v0.4s, v2.s[0]						\n"
	    "	ld1		{v5.2s}, [x24], #8								\n"
	    "	fmla	v29.4s, v0.4s, v2.s[1]						\n"

	    "	ldp		q3, q4, [x11], #32 								\n"

	    "	fmla	v30.4s, v1.4s, v2.s[0]						\n"
	    "	fmla	v31.4s, v1.4s, v2.s[1]						\n"

	    ".endm 																		\n"



	    ".macro KERNEL8x2_K1 											\n"

	    "	prfm	PLDL1KEEP, [x11, #512]						\n"

	    "	fmla	v28.4s, v3.4s, v5.s[0]						\n"
	    "	ld1		{v2.2s}, [x24], #8								\n"
	    "	fmla	v29.4s, v3.4s, v5.s[1]						\n"

	    "	ldp		q0, q1, [x11], #32 								\n"

	    "	fmla	v30.4s, v4.4s, v5.s[0]						\n"
	    "	fmla	v31.4s, v4.4s, v5.s[1]						\n"

	    ".endm 																		\n"


	    ".macro KERNEL8x2_END_K 									\n"

	    "	fmla	v28.4s, v3.4s, v5.s[0]						\n"
	    "	fmla	v29.4s, v3.4s, v5.s[1]						\n"

	    "	fmla	v30.4s, v4.4s, v5.s[0]						\n"
	    "	fmla	v31.4s, v4.4s, v5.s[1]						\n"

	    ".endm 																		\n"


	    ".macro	KERNEL8x2_RIGHT_K_NGM 						\n"

	    "	ld1		{v2.2s}, [x24], #8								\n"

	    "	ldp		q0, q1, [x11], #32 								\n"
	    "	fmla	v28.4s, v0.4s, v2.s[0]						\n"
	    "	fmla	v29.4s, v0.4s, v2.s[1]						\n"

	    "	fmla	v30.4s, v1.4s, v2.s[0]						\n"
	    "	fmla	v31.4s, v1.4s, v2.s[1]						\n"

	    ".endm 																		\n"


	    ".macro KERNEL4x2_BEGIN_K 								\n"

	    "	ldr		q0,	[x11], #16										\n"
	    "	prfm	PLDL1KEEP, [x11, #256]						\n"

	    "	ld1		{v2.2s}, [x24], #8 								\n"

	    "	fmul	v30.4s, v0.4s, v2.s[0]						\n"
	    "	ldr		q1,	[x11], #16										\n"
	    "	fmul	v31.4s, v0.4s, v2.s[1]						\n"

	    "	ld1		{v3.2s}, [x24], #8								\n"

	    ".endm 																		\n"


	    ".macro KERNEL4x2_K0 											\n"

	    "	prfm 	PLDL1KEEP, [x24, #128]						\n"

	    "	ldr		q1,	[x11], #16										\n"
	    "	fmla	v30.4s, v0.4s, v2.s[0]						\n"
	    "	ld1		{v3.2s}, [x24], #8								\n"
	    "	fmla	v31.4s, v0.4s, v2.s[1]						\n"

	    ".endm 																		\n"



	    ".macro KERNEL4x2_K1 											\n"

	    "	prfm	PLDL1KEEP, [x11, #128]						\n"
	    "	ldr		q0,	[x11], #16										\n"
	    "	fmla	v30.4s, v1.4s, v3.s[0]						\n"
	    "	ld1		{v2.2s}, [x24], #8								\n"
	    "	fmla	v31.4s, v1.4s, v3.s[1]						\n"

	    ".endm 																		\n"


	    ".macro KERNEL4x2_END_K 									\n"

	    "	fmla	v30.4s, v1.4s, v3.s[0]						\n"
	    "	fmla	v31.4s, v1.4s, v3.s[1]						\n"

	    ".endm 																		\n"



	    ".macro	M4N2_PACK_ADD_C										\n"

	    "	ld2		{v0.s, v1.s}[0], [x25] 						\n"
	    "	ld2		{v0.s, v1.s}[1], [x26] 						\n"
	    "	ld2		{v0.s, v1.s}[2], [x27] 						\n"
	    "	ld2		{v0.s, v1.s}[3], [x28] 						\n"

	    "	fadd 	v30.4s, v30.4s, v0.4s 						\n"
	    "	fadd 	v31.4s, v31.4s, v1.4s 						\n"

	    ".endm 																		\n"


	    ".macro	SAVE4x2								 						\n"

	    "	st2 	{v30.s, v31.s}[0], [x25] 					\n"
	    "	add  	x25, x25, x9, lsl #4 							\n"
	    "	prfm 	PLDL2KEEP, [x25, #8]							\n"

	    "	st2 	{v30.s, v31.s}[1], [x26] 					\n"
	    "	add  	x26, x26, x9, lsl #4 							\n"
	    "	prfm 	PLDL2KEEP, [x26, #8]							\n"

	    "	st2 	{v30.s, v31.s}[2], [x27] 					\n"
	    "	add  	x27, x27, x9, lsl #4 							\n"
	    "	prfm 	PLDL2KEEP, [x27, #8]							\n"

	    "	st2 	{v30.s, v31.s}[3], [x28] 					\n"

	    ".endm 																		\n"



	    ".macro	M2N2_ADD_C_NGM										\n"

	    "	ld1		{v0.2s}, [x25] 										\n"
	    "	ld1		{v1.2s}, [x26] 										\n"

	    "	fadd 	v30.2s, v30.2s, v0.2s 						\n"
	    "	fadd 	v31.2s, v31.2s, v1.2s 						\n"

	    ".endm 																		\n"


	    ".macro	SAVE2x2_NGM								 			\n"

	    "	st1 	{v30.2s}, [x25] 								\n"
	    "	add  	x25, x25, x9, lsl #3 							\n"
	    "	prfm 	PLDL2KEEP, [x25, #8]							\n"

	    "	st1 	{v31.2s}, [x26] 								\n"

	    ".endm 														\n"


	    "SMM_NN_KERNEL8x2:									 	 	\n"

	    "	ldr		x0, %[C]						 				\n"
	    "	ldr		x1, %[A]						 				\n"
	    "	ldr		x2, %[B]						 				\n"

	    "	ldr		x3, %[M]						 				\n"
	    "	ldr		x4, %[N]						 				\n"  // N = 4
	    "	ldr		x5, %[K]						 				\n"
	    "	ldr		x9, %[LN] 						 				\n"
	    "	ldr		x6, %[LK]						 				\n"

	    "	prfm		PLDL1KEEP, [x1, #512]            			\n"

	    "	ldr		x7, %[m_tag] 					 				\n"
	    "	ldr		x8, %[k_tag] 					 				\n"
	    "	ldr		x30, %[id_tag] 					 				\n"

	    "	cmp		x7, #0 											\n"
	    "	bne 	NN8x2_addr 										\n"
	    "	ldr		x10, %[SB]						 				\n"
	    "	cmp		x30, #0 										\n"
	    "	beq		BEGIN_NN8x2 									\n"
	    "NN8x2_addr:												\n"
	    "	ldr 	x10, %[Bc]										\n"


	    "BEGIN_NN8x2: 												\n"



	    "	mov 	x25, x0   						 				\n"   //C1*
	    "	add		x26, x25, x9, lsl #2 			 				\n"   //C2*
	    "	add 	x27, x25, x9, lsl #3 			 				\n"   //C3*
	    "	add 	x28, x26, x9, lsl #3 			 				\n"   //C4*

	    "	add 	x15, x27, x9, lsl #3 							\n"		//C5*
	    "	add 	x16, x28, x9, lsl #3 							\n"		//C6*
	    "	add 	x17, x15, x9, lsl #3 							\n"		//C7*
	    "	add 	x18, x16, x9, lsl #3 							\n"		//C8*

	    "	mov		x11, x1   						 						\n"   // address_A
	    "	lsr		x21, x3, #3						 						\n"   // M / 8


	    "PACK_NN_KERNEL8x2:												\n"

	    "	mov		x24, x10						 							\n"   //还原SB的地址
	    "	mov		x12, x2						 	 							\n"   // B0*
	    "	add		x13, x12, x9, lsl #2 			 				\n"   // B0 + sizeof(float) * LN

	    "	prfm    PLDL1KEEP, [x11, #256]			 		\n"

	    "PACK_Body_NN_KERNEL8x2:								 	\n"

	    "	lsr		x22, x5, #2 					 						\n"     // K / 8

	    "	PACK_KERNEL8x2_BEGIN_K				 	 				\n"

	    "	subs	x22, x22, #1		     		 					\n"
	    "	b 		PACK_K1_3_8x2						 					\n"

	    "PACK_K_8x2:									 						\n"

	    "	PACK_KERNEL8x2_K0						 						\n"

	    "PACK_K1_3_8x2:									 					\n"

	    "	PACK_KERNEL8x2_K1						 						\n"
	    "	PACK_KERNEL8x2_K0						 						\n"

	    "	beq		PACK_Edge_K_8x2						 				\n"

	    "	PACK_KERNEL8x2_K1						 						\n"

	    "	subs	x22, x22, #1			 		 						\n"
	    "	b 		PACK_K_8x2			 				 					\n"

	    "PACK_Edge_K_8x2:							     				\n"

	    "	PACK_KERNEL8x2_END_K			         			\n"


	    "	ands 	x22, x5, #3 											\n"
	    "	beq		PACK_BEGIN_SAVE8x2_NGM 						\n"
	    "PACK_RIGHT_LOOP_K_8x2_NGM: 							\n"
	    "	subs 	x22, x22, #1 											\n"
	    "	PACK_KERNEL8x2_RIGHT_K_NGM							\n"
	    "bgt		PACK_RIGHT_LOOP_K_8x2_NGM					\n"

	    "PACK_BEGIN_SAVE8x2_NGM:									\n"


	    "	cmp		x8, #0							 							\n"
	    "	beq		M8N2_PACK_SAVE 					 					\n"
	    "	M8N2_PACK_ADD_C													\n"

	    "M8N2_PACK_SAVE: 							 						\n"
	    "	SAVE8x2								 									\n"

	    "	beq		M8x2_END							 						\n"



//--------------------------------------------------------------------
	    "BEGIN_M8_KERNEL_8x2:                     \n"


	    " mov     x24, x10                    		\n"   //  address_B, x11 is address_A + offset

	    " prfm    PLDL1KEEP, [x24, #256]    		 	\n"
	    " prfm    PLDL1KEEP, [x11, #256]    		 	\n"

	    "Body_K_8x2:                            	\n"

	    " lsr   x22, x5, #2                 		 	\n"     // K / 8
	    " KERNEL8x2_BEGIN_K                		 	 	\n"
	    " subs  x22, x22, #1                		 	\n"
	    " b 	K1_7_8x2                            \n"

	    "Main_K_8x2:                            	\n"

	    " KERNEL8x2_K0                     				\n"
	    "K1_7_8x2:                              	\n"

	    " KERNEL8x2_K1                     		 	 	\n"
	    " KERNEL8x2_K0                     		 	 	\n"

	    " beq   Edge_K_8x2                      	\n"

	    " KERNEL8x2_K1                     		 	 	\n"

	    " subs  x22, x22, #1                		 	\n"
	    " b     Main_K_8x2                      	\n"

	    "Edge_K_8x2:                            	\n"

	    " KERNEL8x2_END_K                 			 	\n"


	    "	ands 	x22, x5, #3 											\n"
	    "	beq		BEGIN_SAVE8x2_NGM 								\n"
	    "RIGHT_LOOP_K_8x2_NGM: 										\n"
	    "	subs 	x22, x22, #1 											\n"
	    "	KERNEL8x2_RIGHT_K_NGM										\n"
	    "bgt		RIGHT_LOOP_K_8x2_NGM							\n"

	    "BEGIN_SAVE8x2_NGM:												\n"


	    "	cmp		x8, #0							 							\n"
	    "	beq		M8N2_SAVE 					 	 						\n"
	    "	M8N2_PACK_ADD_C 							 	 				\n"
	    "M8N2_SAVE: 							     						\n"
	    "	SAVE8x2								 	 								\n"

	    " bgt     BEGIN_M8_KERNEL_8x2             \n"

	    "	ands	x21, x3, #7 											\n"

	    //-----------------------------------------------------
	    "BEGIN_M4_KERNEL_4x2: 										\n"


	    "	cmp		x21, #4 													\n"
	    "	blt		BEGIN_M2_KERNEL_2x2 							\n"

	    "	mov     x24, x10                    		\n"
	    " prfm    PLDL1KEEP, [x24, #256]    		 	\n"
	    " prfm    PLDL1KEEP, [x11, #256]    		 	\n"

	    "Body_K_4x2:                            	\n"

	    " lsr   x22, x5, #2                 		 	\n"     // K / 4
	    " KERNEL4x2_BEGIN_K                		 	 	\n"
	    " subs  x22, x22, #1                		 	\n"
	    " b 	K1_7_4x2                            \n"

	    "Main_K_4x2:                            	\n"

	    " KERNEL4x2_K0                     				\n"
	    "K1_7_4x2:                              	\n"

	    " KERNEL4x2_K1                     		 	 	\n"
	    " KERNEL4x2_K0                     		 	 	\n"

	    " beq   Edge_K_4x2                      	\n"

	    " KERNEL4x2_K1                     		 	 	\n"

	    " subs  x22, x22, #1                		 	\n"
	    " b     Main_K_4x2                      	\n"

	    "Edge_K_4x2:                            	\n"

	    " KERNEL4x2_END_K                 			 	\n"

	    "	cmp		x8, #0							 							\n"
	    "	beq		M4N2_SAVE 					 	 						\n"
	    "	M4N2_PACK_ADD_C 							 	 				\n"
	    "M4N2_SAVE: 							     						\n"
	    "	SAVE4x2								 	 								\n"

	    "subs		x21, x21, #4 											\n"


//-----------------------------------------------------
	    "BEGIN_M2_KERNEL_2x2: 										\n"


	    "	cmp		x21, #2 													\n"
	    "	blt		M8x2_END 													\n"


	    "	mov     x24, x10                    		\n"
	    " prfm    PLDL1KEEP, [x24, #256]    		 	\n"
	    " prfm    PLDL1KEEP, [x11, #256]    		 	\n"

	    "	dup		v30.4s, wzr 											\n"
	    "	dup		v31.4s, wzr 											\n"

	    " lsr   x22, x5, #2                 		 	\n"     // K / 4

	    "Body_K_2x2_NGM: 													\n"

	    "	ld1		{v0.2s, v1.2s}, [x24], #16 				\n"
	    "	ldr		q2, [x11], #16 										\n"

	    "	prfm	PLDL1KEEP, [x11, #64] 						\n"
	    "	fmla	v30.2s, v0.2s, v2.s[0] 						\n"
	    "	ld1		{v3.2s, v4.2s}, [x24], #16 				\n"
	    "	fmla	v31.2s, v0.2s, v2.s[1] 						\n"
	    "	ldr		q5, [x11], #16 										\n"
	    "	fmla	v30.2s, v1.2s, v2.s[2] 						\n"
	    "	fmla	v31.2s, v1.2s, v2.s[3] 						\n"

	    "	prfm	PLDL1KEEP, [x24, #64] 						\n"
	    "	subs	x22, x22, #1 											\n"
	    "	fmla	v30.2s, v3.2s, v5.s[0] 						\n"
	    "	fmla	v31.2s, v3.2s, v5.s[1] 						\n"
	    "	fmla	v30.2s, v4.2s, v5.s[2] 						\n"
	    "	fmla	v31.2s, v4.2s, v5.s[3] 						\n"

	    "	bgt 	Body_K_2x2_NGM										\n"

	    "	cmp		x8, #0							 							\n"
	    "	beq		M2N2_SAVE 					 	 						\n"
	    "	M2N2_ADD_C_NGM 							 	 					\n"
	    "M2N2_SAVE: 							     						\n"
	    "	SAVE2x2_NGM								 	 						\n"

	    "subs		x21, x21, #2 											\n"
	    /*
	    	    "Body_K_2x2_NGM:                            \n"

	    	    " lsr   x22, x5, #2                 		 		\n"     // K / 4
	    	    " KERNEL2x2_BEGIN_K                		 	 		\n"
	    	    " subs  x22, x22, #1                		 		\n"
	    	    " b 	K1_7_2x2_NGM                          \n"

	    	    "Main_K_2x2_NGM:                            \n"

	    	    " KERNEL2x2_K0                     					\n"
	    	    "K1_7_2x2_NGM:                              \n"

	    	    " KERNEL2x2_K1                     		 	 		\n"
	    	    " KERNEL2x2_K0                     		 	 		\n"

	    	    " beq   Edge_K_2x2_NGM                      \n"

	    	    " KERNEL2x2_K1                     		 	 		\n"

	    	    " subs  x22, x22, #1                		 		\n"
	    	    " b     Main_K_2x2_NGM                      \n"

	    	    "Edge_K_2x2_NGM:                            \n"

	    	    " KERNEL2x2_END_K                 			 		\n"

	    	    "	cmp		x8, #0							 								\n"
	    	    "	beq		M2N2_SAVE 					 	 							\n"
	    	    "	M2N2_ADD_C 							 	 								\n"
	    	    "M2N2_SAVE: 							     							\n"
	    	    "	SAVE2x2								 	 									\n"

	    */

	    "M8x2_END:															\n"

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
	    [Bc] "m" (Bc),
	    [SB] "m" (SB),
	    [k_tag] "m" (k_tag),
	    [m_tag] "m" (m_tag),
	    [id_tag] "m" (id_tag)
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
                      long LN, long LK, float *Bc, float *SB,
                      long k_tag, long m_tag, long id_tag)
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
	    "	prfm 	PLDL1KEEP, [x28, #16]								\n"
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


	    ".macro	KERNEL4x4_BEGIN_K 									\n"

	    "	ldr 	q0, [x11], #16 											\n"
	    "	prfm	PLDL1KEEP, [x11, #256]							\n"
	    "	ldr		q2, [x24], #16											\n"

	    "	fmul	v28.4s, v2.4s, v0.s[0]							\n"
	    "	ldr		q3, [x24], #16											\n"
	    "	fmul	v29.4s, v2.4s, v0.s[1] 							\n"
	    "	fmul	v30.4s, v2.4s, v0.s[2]							\n"
	    "	ldr		q1, [x11], #16											\n"
	    "	fmul	v31.4s, v2.4s, v0.s[3]							\n"

	    "	prfm	PLDL1KEEP, [x24, #256]							\n"

	    ".endm 																			\n"



	    ".macro	KERNEL4x4_K0 												\n"

	    "	ldr		q1, [x11], #16											\n"
	    "	fmla	v28.4s, v2.4s, v0.s[0]							\n"
	    "	fmla	v29.4s, v2.4s, v0.s[1] 							\n"
	    "	fmla	v30.4s, v2.4s, v0.s[2]							\n"
	    "	ldr		q3, [x24], #16											\n"
	    "	fmla	v31.4s, v2.4s, v0.s[3]							\n"
	    "	prfm	PLDL1KEEP, [x24, #64]								\n"

	    ".endm 																			\n"



	    ".macro	KERNEL4x4_K1 												\n"

	    "	ldr		q0, [x11], #16											\n"
	    "	fmla	v28.4s, v3.4s, v1.s[0]							\n"
	    "	fmla	v29.4s, v3.4s, v1.s[1] 							\n"
	    "	fmla	v30.4s, v3.4s, v1.s[2]							\n"
	    "	ldr		q2, [x24], #16											\n"
	    "	fmla	v31.4s, v3.4s, v1.s[3]							\n"
	    "	prfm	PLDL1KEEP, [x11, #64]								\n"

	    ".endm 																			\n"


	    ".macro	KERNEL4x4_END_K 										\n"

	    "	fmla	v28.4s, v3.4s, v1.s[0]							\n"
	    "	fmla	v29.4s, v3.4s, v1.s[1] 							\n"
	    "	fmla	v30.4s, v3.4s, v1.s[2]							\n"
	    "	fmla	v31.4s, v3.4s, v1.s[3]							\n"

	    ".endm 																			\n"


	    ".macro	M4N4_PACK_ADD_C 										\n"

	    "	ldr		q0, [x25] 													\n"
	    "	fadd 	v28.4s, v28.4s, v0.4s 							\n"
	    "	ldr  	q1, [x26]   												\n"
	    "	fadd 	v29.4s, v29.4s, v1.4s 							\n"
	    "	ldr  	q2, [x27]   												\n"
	    "	fadd 	v30.4s, v30.4s, v2.4s 							\n"

	    "	ldr 	q3, [x28] 													\n"
	    "	fadd 	v31.4s, v31.4s, v3.4s 							\n"

	    ".endm 																			\n"


	    ".macro	SAVE4x4 														\n"

	    "	str 	q28, [x25]													\n"
	    "	add 	x25, x25, x9, lsl #4 								\n"
	    "	prfm	PLDL2KEEP, [x25, #8] 								\n"
	    "	str 	q29, [x26]													\n"
	    "	add 	x26, x26, x9, lsl #4 								\n"
	    "	prfm	PLDL2KEEP, [x26, #8] 								\n"
	    "	str 	q30, [x27]													\n"
	    "	add 	x27, x26, x9, lsl #4 								\n"
	    "	prfm	PLDL2KEEP, [x27, #8] 								\n"
	    "	str 	q31, [x28]													\n"

	    ".endm 																			\n"



	    ".macro	KERNEL2x4_BEGIN_K 								\n"

	    "	ld2 	{v0.s, v1.s}[0], [x11], #8 				\n"
	    "	prfm	PLDL1KEEP, [x11, #128]						\n"
	    "	ldr		q2, [x24], #16										\n"

	    "	fmul	v30.4s, v2.4s, v0.s[0]						\n"
	    "	ld2 	{v3.s, v4.s}[0], [x11], #8 				\n"
	    "	fmul	v31.4s, v2.4s, v1.s[0]						\n"

	    "	ldr		q5, [x24], #16										\n"
	    "	prfm	PLDL1KEEP, [x24, #128]						\n"

	    ".endm 																		\n"



	    ".macro	KERNEL2x4_K0 											\n"

	    "	ld2 	{v3.s, v4.s}[0], [x11], #8 				\n"
	    "	fmla	v30.4s, v2.4s, v0.s[0]						\n"
	    "	ldr		q5, [x24], #16										\n"
	    "	fmla	v31.4s, v2.4s, v1.s[0]						\n"
	    "	prfm	PLDL1KEEP, [x24, #64]							\n"

	    ".endm 																		\n"



	    ".macro	KERNEL2x4_K1 											\n"

	    "	ld2 	{v0.s, v1.s}[0], [x11], #8 				\n"
	    "	fmla	v30.4s, v5.4s, v3.s[0]						\n"
	    "	ldr		q2, [x24], #16										\n"
	    "	fmla	v31.4s, v5.4s, v4.s[0]						\n"
	    "	prfm	PLDL1KEEP, [x11, #64]							\n"

	    ".endm 																		\n"


	    ".macro	KERNEL2x4_END_K 									\n"

	    "	fmla	v30.4s, v5.4s, v3.s[0]						\n"
	    "	fmla	v31.4s, v5.4s, v4.s[0]						\n"

	    ".endm 																		\n"


	    ".macro	M2N4_ADD_C_NGM 										\n"

	    "	ldr		q0, [x25] 												\n"
	    "	fadd 	v30.4s, v30.4s, v0.4s 						\n"
	    "	ldr  	q1, [x26]   											\n"
	    "	fadd 	v31.4s, v31.4s, v1.4s 						\n"

	    ".endm 																		\n"


	    ".macro	SAVE2x4_NGM 											\n"

	    "	str 	q30, [x25]												\n"
	    "	str 	q31, [x26]												\n"


	    ".endm 																		\n"




	    "SMM_NN_KERNEL8x4:									 	 	\n"

	    "	ldr		x0, %[C]						 				\n"
	    "	ldr		x1, %[A]						 				\n"
	    "	ldr		x2, %[B]						 				\n"

	    "	ldr		x3, %[M]						 				\n"
	    "	ldr		x4, %[N]						 				\n"  // N = 4
	    "	ldr		x5, %[K]						 				\n"
	    "	ldr		x9, %[LN] 						 				\n"
	    "	ldr		x6, %[LK]						 				\n"

	    "	prfm		PLDL1KEEP, [x1, #512]            			\n"

	    "	ldr		x7, %[m_tag] 					 				\n"
	    "	ldr		x8, %[k_tag] 					 				\n"
	    "	ldr		x30, %[id_tag] 					 				\n"

//-----------------------------------------------------------------------------
	    "	cmp		x7, #0 											\n"
	    "	bne 	NN8x4_addr 										\n"
	    "	ldr		x10, %[SB]						 				\n"
	    "	cmp		x30, #0 										\n"
	    "	beq		BEGIN_NN8x4 									\n"
	    "NN8x4_addr:												\n"
	    "	ldr 	x10, %[Bc]										\n"


	    "BEGIN_NN8x4: 												\n"


	    "	mov 	x25, x0   						 				\n"   //C1*
	    "	prfm	PLDL2KEEP, [x25, #16]							\n"
	    "	add		x26, x25, x9, lsl #2 			 				\n"   //C2*
	    "	prfm	PLDL2KEEP, [x26, #16]							\n"
	    "	add 	x27, x25, x9, lsl #3 			 				\n"   //C3*
	    "	prfm	PLDL2KEEP, [x27, #16]							\n"
	    "	add 	x28, x26, x9, lsl #3 			 				\n"   //C4*
	    "	prfm	PLDL2KEEP, [x28, #16]							\n"

	    "	add 	x15, x27, x9, lsl #3 							\n"		//C5*
	    "	prfm	PLDL2KEEP, [x15, #16]							\n"
	    "	add 	x16, x28, x9, lsl #3 							\n"		//C6*
	    "	prfm	PLDL2KEEP, [x16, #16]							\n"
	    "	add 	x17, x15, x9, lsl #3 							\n"		//C7*
	    "	prfm	PLDL2KEEP, [x17, #16]							\n"
	    "	add 	x18, x16, x9, lsl #3 							\n"		//C8*
	    "	prfm	PLDL2KEEP, [x18, #16]							\n"


	    "	mov		x11, x1   						 						\n"   // address_A
	    "	lsr		x21, x3, #3						 						\n"   // M / 8


	    "PACK_NN_KERNEL8x4:												\n"

	    "	mov		x24, x10						 							\n"   //还原SB的地址
	    "	mov		x12, x2						 	 							\n"   // B0*
	    "	add		x13, x12, x9, lsl #2 			 				\n"   // B0 + sizeof(float) * LN

	    "	prfm    PLDL1KEEP, [x12, #16]			 			\n"
	    "	prfm    PLDL1KEEP, [x13, #16]			 			\n"
	    "	prfm    PLDL1KEEP, [x11, #256]			 		\n"

	    "PACK_Body_NN_KERNEL8x4:								 	\n"

	    "	lsr		x22, x5, #2 					 						\n"     // K / 8

	    "	PACK_KERNEL8x4_BEGIN_K				 	 				\n"

	    "	subs	x22, x22, #1		     		 					\n"
	    "	b 		PACK_K1_3_8x4						 					\n"

	    "PACK_K_8x4:									 						\n"

	    "	PACK_KERNEL8x4_K0						 						\n"

	    "PACK_K1_3_8x4:									 					\n"

	    "	PACK_KERNEL8x4_K1						 						\n"
	    "	PACK_KERNEL8x4_K0						 						\n"

	    "	beq		PACK_Edge_K_8x4						 				\n"

	    "	PACK_KERNEL8x4_K1						 						\n"

	    "	subs	x22, x22, #1			 		 						\n"
	    "	b 		PACK_K_8x4			 				 					\n"

	    "PACK_Edge_K_8x4:							     				\n"

	    "	PACK_KERNEL8x4_END_K			         			\n"

	    "	ands 	x22, x5, #3 											\n"
	    "	beq		PACK_BEGIN_SAVE8x4 								\n"
	    "PACK_RIGHT_LOOP_K_8x4: 									\n"
	    "	subs 	x22, x22, #1 											\n"
	    "	PACK_KERNEL8x4_RIGHT_K									\n"
	    "bgt		PACK_RIGHT_LOOP_K_8x4							\n"

	    "PACK_BEGIN_SAVE8x4:											\n"

	    "	cmp		x8, #0							 							\n"
	    "	beq		M8N4_PACK_SAVE 					 					\n"
	    "	M8N4_PACK_ADD_C													\n"

	    "M8N4_PACK_SAVE: 							 						\n"
	    "	SAVE8x4								 									\n"

	    "	beq		N4_END_NGM							 					\n"



//--------------------------------------------------------------------
	    "BEGIN_M8_KERNEL_8x4:                     \n"


	    " mov     x24, x10                    		\n"   //  address_B, x11 is address_A + offset

	    " prfm    PLDL1KEEP, [x24, #256]    		 	\n"
	    " prfm    PLDL1KEEP, [x11, #256]    		 	\n"

	    "Body_K_8x4:                            	\n"

	    " lsr   x22, x5, #2                 		 	\n"     // K / 4
	    " KERNEL8x4_BEGIN_K                		 	 	\n"
	    " subs  x22, x22, #1                		 	\n"
	    " b 	K1_7_8x4                            \n"

	    "Main_K_8x4:                            	\n"

	    " KERNEL8x4_K0                     				\n"
	    "K1_7_8x4:                              	\n"

	    " KERNEL8x4_K1                     		 	 	\n"
	    " KERNEL8x4_K0                     		 	 	\n"

	    " beq   Edge_K_8x4                      	\n"

	    " KERNEL8x4_K1                     		 	 	\n"

	    " subs  x22, x22, #1                		 	\n"
	    " b     Main_K_8x4                      	\n"

	    "Edge_K_8x4:                            	\n"

	    " KERNEL8x4_END_K                 			 	\n"

	    "	ands 	x22, x5, #3 										\n"
	    "	beq		BEGIN_SAVE8x4 									\n"
	    "RIGHT_LOOP_K_8x4: 											\n"
	    "	subs 	x22, x22, #1 										\n"
	    "	KERNEL8x4_RIGHT_K											\n"
	    "bgt		RIGHT_LOOP_K_8x4								\n"

	    "BEGIN_SAVE8x4: 												\n"

	    "	cmp		x8, #0							 						\n"
	    "	beq		M8N4_SAVE 					 	 					\n"
	    "	M8N4_PACK_ADD_C 							 	 			\n"
	    "M8N4_SAVE: 							     					\n"
	    "	SAVE8x4								 	 							\n"

	    " bgt     BEGIN_M8_KERNEL_8x4         	\n"

	    "	ands	x21, x3, #7 										\n"
	    "	beq 	N4_END_NGM 											\n"

	    "BEGIN_N4_M4: 													\n"
	    "	cmp		x21, #4 												\n"
	    "	blt 	BEGIN_N4_M2 										\n"

	    " 	mov     x24, x10                    \n"   //  address_B, x11 is address_A + offset
	    " 	prfm    PLDL1KEEP, [x24, #256]    	\n"
	    " 	prfm    PLDL1KEEP, [x11, #256]    	\n"

//------------------------------------------------------------------------------------
	    "Body_K_N4_M4:                          \n"

	    " 	lsr   x22, x5, #2                 	\n"     // K / 4
	    " 	KERNEL4x4_BEGIN_K                		\n"
	    " 	subs  x22, x22, #1                	\n"
	    " 	b 	K1_3_N4_M4                      \n"

	    "Main_K_N4_M4:                          \n"

	    " 	KERNEL4x4_K0                     		\n"
	    "K1_3_N4_M4:                            \n"

	    " 	KERNEL4x4_K1                     		\n"
	    " 	KERNEL4x4_K0                     		\n"

	    " beq   Edge_K_N4_M4                    \n"

	    " 	KERNEL4x4_K1                     		\n"

	    " subs  x22, x22, #1                		\n"
	    " b     Main_K_N4_M4                    \n"

	    "Edge_K_N4_M4:                          \n"
	    " 	KERNEL4x4_END_K                 		\n"

	    "BEGIN_SAVE4x4:													\n"

	    "	cmp		x8, #0							 						\n"
	    "	beq		M4N4_SAVE 					 	 					\n"
	    "	M4N4_PACK_ADD_C 							 				\n"
	    "M4N4_SAVE: 							     					\n"
	    "	SAVE4x4								 	 							\n"

	    "	subs	x21, x21, #4 										\n"


//------------------------------------------------------------------------------------

	    "BEGIN_N4_M2: 													\n"
	    "	cmp		x21, #2 												\n"
	    "	blt 	N4_END_NGM 											\n"

	    " mov     x24, x10                    	\n"   //  address_B, x11 is address_A + offset
	    " prfm    PLDL1KEEP, [x24, #256]    		\n"
	    " prfm    PLDL1KEEP, [x11, #256]    		\n"

	    "Body_K_N4_M2:                          \n"

	    " 	lsr   x22, x5, #2                 	\n"     // K / 4
	    " 	KERNEL2x4_BEGIN_K                		\n"
	    " 	subs  x22, x22, #1                	\n"
	    " 	b 	K1_3_N4_M2                      \n"

	    "Main_K_N4_M2:                          \n"

	    " KERNEL2x4_K0                     			\n"
	    "K1_3_N4_M2:                            \n"

	    " KERNEL2x4_K1                     			\n"
	    "	KERNEL2x4_K0                     			\n"

	    " beq   Edge_K_N4_M2                    \n"

	    " KERNEL2x4_K1                     			\n"

	    " subs  x22, x22, #1                		\n"
	    " b     Main_K_N4_M2                    \n"

	    "Edge_K_N4_M2:                          \n"
	    " 	KERNEL2x4_END_K                 		\n"

	    "BEGIN_SAVE2x4:													\n"

	    "	cmp		x8, #0							 						\n"
	    "	beq		M2N4_SAVE 					 	 					\n"
	    "	M2N4_ADD_C_NGM 							 					\n"
	    "M2N4_SAVE: 							     					\n"
	    "	SAVE2x4_NGM								 	 					\n"

	    "	subs	x21, x21, #2 										\n"

	    "N4_END_NGM:														\n"


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
	    [Bc] "m" (Bc),
	    [SB] "m" (SB),
	    [k_tag] "m" (k_tag),
	    [m_tag] "m" (m_tag),
	    [id_tag] "m" (id_tag)
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



void SMM_NN_KERNEL8x8(float *C, float *A, float *B, long	M, long N, long K,
                      long LN, long LK, float *Bc, float *SB,
                      long k_tag, long m_tag, long id_tag)
{
	asm volatile(

	    ".macro	PACK_KERNEL8x8_BEGIN_K_NGM					\n"

	    "	ldr		q0,	[x11], #16											\n"
	    "	prfm	PLDL1KEEP, [x11, #512]							\n"

	    "	ldp		q2, q3, [x12]												\n"
	    "	fmul	v16.4s, v2.4s, v0.s[0] 							\n"
	    "	ldr		q1,	[x11], #16											\n"
	    "	fmul	v17.4s, v3.4s, v0.s[0] 							\n"
	    "	fmul	v18.4s, v2.4s, v0.s[1] 							\n"
	    "	ldr		q6, [x13]														\n"
	    "	fmul	v19.4s, v3.4s, v0.s[1] 							\n"

	    "	fmul	v20.4s, v2.4s, v0.s[2]							\n"
	    "	fmul	v21.4s, v3.4s, v0.s[2]							\n"
	    "	ldp		q4,	q5, [x11], #32									\n"
	    "	fmul	v22.4s, v2.4s, v0.s[3]							\n"
	    "	fmul	v23.4s, v3.4s, v0.s[3]							\n"

	    "	add  	x12, x12, x9, lsl #3 								\n"   //addr_B + 2 * sizeof(column)

	    "	fmul	v24.4s, v2.4s, v1.s[0] 							\n"
	    "	fmul	v25.4s, v3.4s, v1.s[0] 							\n"
	    "	ldr		q7, [x13, #16]											\n"
	    "	fmul	v26.4s, v2.4s, v1.s[1] 							\n"
	    "	prfm	PLDL1KEEP, [x12, #32]								\n"
	    "	fmul	v27.4s, v3.4s, v1.s[1] 							\n"

	    "	str 	q2, [x24], #16 											\n"
	    "	add  	x13, x13, x9, lsl #3 								\n"
	    "	fmul	v28.4s, v2.4s, v1.s[2] 							\n"
	    "	fmul	v29.4s, v3.4s, v1.s[2] 							\n"
	    "	fmul	v30.4s, v2.4s, v1.s[3] 							\n"
	    "	fmul	v31.4s, v3.4s, v1.s[3] 							\n"

	    "	str		q3, [x24], #16 											\n"

	    ".endm 																			\n"


	    ".macro	PACK_KERNEL8x8_K0_NGM								\n"


	    "	fmla	v16.4s, v2.4s, v0.s[0]						 	\n"
	    "	fmla	v17.4s, v3.4s, v0.s[0] 							\n"
	    "	ldp		q6, q7, [x13] 											\n"
	    "	fmla	v18.4s, v2.4s, v0.s[1]						 	\n"
	    "	fmla	v19.4s, v3.4s, v0.s[1] 							\n"
	    "	fmla	v20.4s, v2.4s, v0.s[2]						 	\n"
	    "	fmla	v21.4s, v3.4s, v0.s[2] 							\n"
	    "	ldr 	q4, [x11], #16 											\n"
	    "	fmla	v22.4s, v2.4s, v0.s[3]						 	\n"
	    "	fmla	v23.4s, v3.4s, v0.s[3] 							\n"

	    "	add  	x13, x13, x9, lsl #3 								\n"

	    "	ldr		q5,	[x11], #16											\n"
	    "	fmla	v24.4s, v2.4s, v1.s[0]						 	\n"
	    "	fmla	v25.4s, v3.4s, v1.s[0] 							\n"
	    "	fmla	v26.4s, v2.4s, v1.s[1]						 	\n"
	    "	fmla	v27.4s, v3.4s, v1.s[1] 							\n"
	    "	str 	q2, [x24], #16 											\n"
	    "	fmla	v28.4s, v2.4s, v1.s[2]						 	\n"
	    "	fmla	v29.4s, v3.4s, v1.s[2] 							\n"
	    "	fmla	v30.4s, v2.4s, v1.s[3]						 	\n"
	    "	fmla	v31.4s, v3.4s, v1.s[3] 							\n"
	    "	prfm	PLDL1KEEP, [x13, #32]								\n"
	    "	str 	q3, [x24], #16 											\n"

	    ".endm 																			\n"


	    ".macro	PACK_KERNEL8x8_K1_NGM								\n"

	    "	prfm	PLDL1KEEP, [x11, #512]							\n"

	    "	fmla	v16.4s, v6.4s, v4.s[0]						 	\n"
	    "	fmla	v17.4s, v7.4s, v4.s[0] 							\n"
	    "	ldp		q2, q3, [x12] 											\n"
	    "	fmla	v18.4s, v6.4s, v4.s[1]						 	\n"
	    "	fmla	v19.4s, v7.4s, v4.s[1] 							\n"
	    "	fmla	v20.4s, v6.4s, v4.s[2]						 	\n"
	    "	fmla	v21.4s, v7.4s, v4.s[2] 							\n"
	    "	ldr 	q0, [x11], #16 											\n"
	    "	fmla	v22.4s, v6.4s, v4.s[3]						 	\n"
	    "	fmla	v23.4s, v7.4s, v4.s[3] 							\n"

	    "	add  	x12, x12, x9, lsl #3 								\n"

	    "	ldr		q1,	[x11], #16											\n"
	    "	fmla	v24.4s, v6.4s, v5.s[0]						 	\n"
	    "	fmla	v25.4s, v7.4s, v5.s[0] 							\n"
	    "	fmla	v26.4s, v6.4s, v5.s[1]						 	\n"
	    "	fmla	v27.4s, v7.4s, v5.s[1] 							\n"
	    "	str 	q6, [x24], #16 											\n"
	    "	fmla	v28.4s, v6.4s, v5.s[2]						 	\n"
	    "	fmla	v29.4s, v7.4s, v5.s[2] 							\n"
	    "	fmla	v30.4s, v6.4s, v5.s[3]						 	\n"
	    "	fmla	v31.4s, v7.4s, v5.s[3] 							\n"
	    "	prfm	PLDL1KEEP, [x12, #32]								\n"
	    "	str 	q7, [x24], #16 											\n"

	    ".endm 																			\n"


	    ".macro	PACK_KERNEL8x8_END_K_NGM						\n"

	    "	fmla	v16.4s, v6.4s, v4.s[0]						 	\n"
	    "	fmla	v17.4s, v7.4s, v4.s[0] 							\n"
	    "	fmla	v18.4s, v6.4s, v4.s[1]						 	\n"
	    "	fmla	v19.4s, v7.4s, v4.s[1] 							\n"
	    "	fmla	v20.4s, v6.4s, v4.s[2]						 	\n"
	    "	fmla	v21.4s, v7.4s, v4.s[2] 							\n"
	    "	fmla	v22.4s, v6.4s, v4.s[3]						 	\n"
	    "	fmla	v23.4s, v7.4s, v4.s[3] 							\n"

	    "	fmla	v24.4s, v6.4s, v5.s[0]						 	\n"
	    "	fmla	v25.4s, v7.4s, v5.s[0] 							\n"
	    "	fmla	v26.4s, v6.4s, v5.s[1]						 	\n"
	    "	fmla	v27.4s, v7.4s, v5.s[1] 							\n"
	    "	str 	q6, [x24], #16 											\n"
	    "	fmla	v28.4s, v6.4s, v5.s[2]						 	\n"
	    "	fmla	v29.4s, v7.4s, v5.s[2] 							\n"
	    "	fmla	v30.4s, v6.4s, v5.s[3]						 	\n"
	    "	fmla	v31.4s, v7.4s, v5.s[3] 							\n"
	    "	str 	q7, [x24], #16 											\n"

	    ".endm 																			\n"


	    ".macro	PACK_KERNEL8x8_RIGHT_K_NGM					\n"


	    "	ldp		q2, q3, [x12]												\n"
	    "	ldr		q0, [x11], #16 											\n"

	    "	fmla	v16.4s, v2.4s, v0.s[0]							\n"
	    "	fmla	v17.4s, v3.4s, v0.s[0]							\n"

	    "	add  	x12, x12, x9, lsl #2 								\n"
	    "	fmla	v18.4s, v2.4s, v0.s[1]							\n"
	    "	ldr		q1, [x11], #16 											\n"
	    "	fmla	v19.4s, v3.4s, v0.s[1]							\n"

	    "	fmla	v20.4s, v2.4s, v0.s[2]							\n"
	    "	fmla	v21.4s, v3.4s, v0.s[2]							\n"

	    "	fmla	v22.4s, v2.4s, v0.s[3]							\n"
	    "	fmla	v23.4s, v3.4s, v0.s[3]							\n"

	    "	fmla	v24.4s, v2.4s, v1.s[0]							\n"
	    "	fmla	v25.4s, v3.4s, v1.s[0]							\n"
	    "	stp 	q2, q3, [x24], #32 									\n"
	    "	fmla	v26.4s, v2.4s, v1.s[1]							\n"
	    "	fmla	v27.4s, v3.4s, v1.s[1]							\n"

	    "	fmla	v28.4s, v2.4s, v1.s[2]							\n"
	    "	fmla	v29.4s, v3.4s, v1.s[2]							\n"

	    "	fmla	v30.4s, v2.4s, v1.s[3]							\n"
	    "	fmla	v31.4s, v3.4s, v1.s[3]							\n"

	    ".endm 																			\n"


	    ".macro	M8N8_PACK_ADD_C											\n"

	    "	prfm	PLDL1KEEP, [x25, #32]								\n"
	    "	prfm 	PLDL1KEEP, [x26, #32]								\n"

	    "	ldp		q0, q1, [x25] 											\n"
	    "	fadd  	v16.4s, v16.4s, v0.4s 						\n"
	    "	fadd 	v17.4s, v17.4s, v1.4s 							\n"
	    "	prfm 	PLDL1KEEP, [x27, #32]								\n"
	    "	ldp  	q2, q3, [x26]   										\n"
	    "	fadd 	v18.4s, v18.4s, v2.4s 							\n"
	    "	fadd 	v19.4s, v19.4s, v3.4s 							\n"
	    "	prfm 	PLDL1KEEP, [x28, #32]								\n"
	    "	ldp		q4, q5, [x27] 											\n"
	    "	fadd 	v20.4s, v20.4s, v4.4s 							\n"
	    "	fadd 	v21.4s, v21.4s, v5.4s 							\n"
	    "	prfm 	PLDL1KEEP, [x15, #32]								\n"
	    "	ldp		q6, q7, [x28] 											\n"
	    "	fadd 	v22.4s, v22.4s, v6.4s 							\n"
	    "	fadd 	v23.4s, v23.4s, v7.4s 							\n"

	    "	prfm 	PLDL1KEEP, [x16, #32]								\n"
	    "	ldp		q0, q1, [x15]												\n"
	    "	fadd 	v24.4s, v24.4s, v0.4s 							\n"
	    "	fadd 	v25.4s, v25.4s, v1.4s 							\n"

	    "	prfm 	PLDL1KEEP, [x17, #32]								\n"
	    "	ldp		q2, q3, [x16]												\n"
	    "	fadd 	v26.4s, v26.4s, v2.4s 							\n"
	    "	fadd 	v27.4s, v27.4s, v3.4s 							\n"

	    "	prfm 	PLDL1KEEP, [x18, #32]								\n"
	    "	ldp		q4, q5, [x17]												\n"
	    "	fadd 	v28.4s, v28.4s, v4.4s 							\n"
	    "	fadd 	v29.4s, v29.4s, v5.4s 							\n"

	    "	ldp		q6, q7, [x18]												\n"
	    "	fadd 	v30.4s, v30.4s, v6.4s 							\n"
	    "	fadd 	v31.4s, v31.4s, v7.4s 							\n"

	    ".endm 																			\n"


	    ".macro	SAVE8x8_NGM								 					\n"

	    "	subs	x21, x21, #1 												\n"

	    "	stp 	q16, q17, [x25] 										\n"
	    "	add  	x25, x25, x9, lsl #5 								\n"
	    "	prfm 	PLDL2KEEP, [x25, #32]								\n"

	    "	stp 	q18, q19, [x26] 									\n"
	    "	add  	x26, x26, x9, lsl #5 							\n"
	    "	prfm 	PLDL2KEEP, [x26, #32]							\n"

	    "	stp 	q20, q21, [x27] 									\n"
	    "	add  	x27, x27, x9, lsl #5 							\n"
	    "	prfm 	PLDL2KEEP, [x27, #32]							\n"

	    "	stp 	q22, q23, [x28] 									\n"
	    "	add  	x28, x28, x9, lsl #5 							\n"
	    "	prfm 	PLDL2KEEP, [x28, #32]							\n"


	    "	stp 	q24, q25, [x15] 									\n"
	    "	add  	x15, x15, x9, lsl #5 							\n"
	    "	prfm 	PLDL2KEEP, [x15, #32]							\n"

	    "	stp 	q26, q27, [x16] 									\n"
	    "	add  	x16, x16, x9, lsl #5 							\n"
	    "	prfm 	PLDL2KEEP, [x16, #32]							\n"

	    "	stp 	q28, q29, [x17] 									\n"
	    "	add  	x17, x17, x9, lsl #5 							\n"
	    "	prfm 	PLDL2KEEP, [x17, #32]							\n"

	    "	stp 	q30, q31, [x18] 									\n"
	    "	add  	x18, x18, x9, lsl #5 							\n"
	    "	prfm 	PLDL2KEEP, [x18, #32]							\n"

	    ".endm 																		\n"



	    ".macro KERNEL8x8_BEGIN_K_NGM 						\n"

	    "	ldr		q0,	[x11], #16										\n"
	    "	prfm	PLDL1KEEP, [x11, #512]						\n"

	    "	ldp		q2, q3, [x24], #32								\n"
	    "	fmul	v16.4s, v2.4s, v0.s[0] 						\n"
	    "	ldr		q1,	[x11], #16										\n"
	    "	fmul	v17.4s, v3.4s, v0.s[0] 						\n"
	    "	fmul	v18.4s, v2.4s, v0.s[1] 						\n"
	    "	ldr		q6, [x24], #16										\n"
	    "	fmul	v19.4s, v3.4s, v0.s[1] 						\n"

	    "	fmul	v20.4s, v2.4s, v0.s[2]						\n"
	    "	fmul	v21.4s, v3.4s, v0.s[2]						\n"
	    "	ldp		q4,	q5, [x11], #32								\n"
	    "	fmul	v22.4s, v2.4s, v0.s[3]						\n"
	    "	fmul	v23.4s, v3.4s, v0.s[3]						\n"

	    "	fmul	v24.4s, v2.4s, v1.s[0] 						\n"
	    "	fmul	v25.4s, v3.4s, v1.s[0] 						\n"
	    "	ldr		q7, [x24], #16										\n"
	    "	fmul	v26.4s, v2.4s, v1.s[1] 						\n"
	    "	prfm	PLDL1KEEP, [x24, #128]						\n"
	    "	fmul	v27.4s, v3.4s, v1.s[1] 						\n"

	    "	fmul	v28.4s, v2.4s, v1.s[2] 						\n"
	    "	fmul	v29.4s, v3.4s, v1.s[2] 						\n"
	    "	fmul	v30.4s, v2.4s, v1.s[3] 						\n"
	    "	fmul	v31.4s, v3.4s, v1.s[3] 						\n"

	    ".endm 																		\n"


	    ".macro KERNEL8x8_K0_NGM 									\n"

	    "	fmla	v16.4s, v2.4s, v0.s[0]						\n"
	    "	fmla	v17.4s, v3.4s, v0.s[0] 						\n"
	    "	ldp		q6, q7, [x24], #32 								\n"
	    "	fmla	v18.4s, v2.4s, v0.s[1]						\n"
	    "	fmla	v19.4s, v3.4s, v0.s[1] 						\n"
	    "	fmla	v20.4s, v2.4s, v0.s[2]						\n"
	    "	fmla	v21.4s, v3.4s, v0.s[2] 						\n"
	    "	ldr 	q4, [x11], #16 										\n"
	    "	fmla	v22.4s, v2.4s, v0.s[3]						\n"
	    "	fmla	v23.4s, v3.4s, v0.s[3] 						\n"

	    "	ldr		q5,	[x11], #16										\n"
	    "	fmla	v24.4s, v2.4s, v1.s[0]						\n"
	    "	fmla	v25.4s, v3.4s, v1.s[0] 						\n"
	    "	fmla	v26.4s, v2.4s, v1.s[1]						\n"
	    "	fmla	v27.4s, v3.4s, v1.s[1] 						\n"
	    "	fmla	v28.4s, v2.4s, v1.s[2]						\n"
	    "	fmla	v29.4s, v3.4s, v1.s[2] 						\n"
	    "	fmla	v30.4s, v2.4s, v1.s[3]						\n"
	    "	fmla	v31.4s, v3.4s, v1.s[3] 						\n"
	    "	prfm	PLDL1KEEP, [x24, #128]						\n"

	    ".endm 														\n"



	    ".macro KERNEL8x8_K1_NGM 									\n"

	    "	prfm	PLDL1KEEP, [x11, #512]							\n"

	    "	fmla	v16.4s, v6.4s, v4.s[0]						 	\n"
	    "	fmla	v17.4s, v7.4s, v4.s[0] 							\n"
	    "	ldp		q2, q3, [x24], #32 									\n"
	    "	fmla	v18.4s, v6.4s, v4.s[1]						 	\n"
	    "	fmla	v19.4s, v7.4s, v4.s[1] 							\n"
	    "	fmla	v20.4s, v6.4s, v4.s[2]						 	\n"
	    "	fmla	v21.4s, v7.4s, v4.s[2] 							\n"
	    "	ldr 	q0, [x11], #16 											\n"
	    "	fmla	v22.4s, v6.4s, v4.s[3]						 	\n"
	    "	fmla	v23.4s, v7.4s, v4.s[3] 							\n"

	    "	ldr		q1,	[x11], #16											\n"
	    "	fmla	v24.4s, v6.4s, v5.s[0]						 	\n"
	    "	fmla	v25.4s, v7.4s, v5.s[0] 							\n"
	    "	fmla	v26.4s, v6.4s, v5.s[1]						 	\n"
	    "	fmla	v27.4s, v7.4s, v5.s[1] 							\n"
	    "	fmla	v28.4s, v6.4s, v5.s[2]						 	\n"
	    "	fmla	v29.4s, v7.4s, v5.s[2] 							\n"
	    "	fmla	v30.4s, v6.4s, v5.s[3]						 	\n"
	    "	fmla	v31.4s, v7.4s, v5.s[3] 							\n"

	    ".endm 																			\n"


	    ".macro KERNEL8x8_END_K_NGM 								\n"

	    "	fmla	v16.4s, v6.4s, v4.s[0]						 	\n"
	    "	fmla	v17.4s, v7.4s, v4.s[0] 							\n"
	    "	fmla	v18.4s, v6.4s, v4.s[1]						 	\n"
	    "	fmla	v19.4s, v7.4s, v4.s[1] 							\n"
	    "	fmla	v20.4s, v6.4s, v4.s[2]						 	\n"
	    "	fmla	v21.4s, v7.4s, v4.s[2] 							\n"
	    "	fmla	v22.4s, v6.4s, v4.s[3]						 	\n"
	    "	fmla	v23.4s, v7.4s, v4.s[3] 							\n"

	    "	fmla	v24.4s, v6.4s, v5.s[0]						 	\n"
	    "	fmla	v25.4s, v7.4s, v5.s[0] 							\n"
	    "	fmla	v26.4s, v6.4s, v5.s[1]						 	\n"
	    "	fmla	v27.4s, v7.4s, v5.s[1] 							\n"
	    "	fmla	v28.4s, v6.4s, v5.s[2]						 	\n"
	    "	fmla	v29.4s, v7.4s, v5.s[2] 							\n"
	    "	fmla	v30.4s, v6.4s, v5.s[3]						 	\n"
	    "	fmla	v31.4s, v7.4s, v5.s[3] 							\n"

	    ".endm 																			\n"


	    ".macro	KERNEL8x8_RIGHT_K_NGM 							\n"


	    "	ldp		q2, q3, [x24], #32									\n"
	    "	ldr		q0, [x11], #16 											\n"

	    "	fmla	v16.4s, v2.4s, v0.s[0]							\n"
	    "	fmla	v17.4s, v3.4s, v0.s[0]							\n"

	    "	fmla	v18.4s, v2.4s, v0.s[1]							\n"
	    "	ldr		q1, [x11], #16 											\n"
	    "	fmla	v19.4s, v3.4s, v0.s[1]							\n"

	    "	fmla	v20.4s, v2.4s, v0.s[2]							\n"
	    "	fmla	v21.4s, v3.4s, v0.s[2]							\n"

	    "	fmla	v22.4s, v2.4s, v0.s[3]							\n"
	    "	fmla	v23.4s, v3.4s, v0.s[3]							\n"

	    "	fmla	v24.4s, v2.4s, v1.s[0]							\n"
	    "	fmla	v25.4s, v3.4s, v1.s[0]							\n"

	    "	fmla	v26.4s, v2.4s, v1.s[1]							\n"
	    "	fmla	v27.4s, v3.4s, v1.s[1]							\n"

	    "	fmla	v28.4s, v2.4s, v1.s[2]							\n"
	    "	fmla	v29.4s, v3.4s, v1.s[2]							\n"

	    "	fmla	v30.4s, v2.4s, v1.s[3]							\n"
	    "	fmla	v31.4s, v3.4s, v1.s[3]							\n"

	    ".endm 																			\n"



	    ".macro KERNEL4x8_BEGIN_K_NGM 							\n"

	    "	ldr		q0,	[x11], #16											\n"
	    "	prfm	PLDL1KEEP, [x11, #512]							\n"
	    "	ldp		q2, q3, [x24], #32									\n"

	    "	fmul	v24.4s, v2.4s, v0.s[0] 							\n"
	    "	fmul	v25.4s, v3.4s, v0.s[0] 							\n"
	    "	ldr		q1,	[x11], #16											\n"
	    "	fmul	v26.4s, v2.4s, v0.s[1] 							\n"
	    "	prfm	PLDL1KEEP, [x24, #128]							\n"
	    "	fmul	v27.4s, v3.4s, v0.s[1] 							\n"

	    "	ldp		q4, q5, [x24], #32									\n"
	    "	fmul	v28.4s, v2.4s, v0.s[2] 							\n"
	    "	fmul	v29.4s, v3.4s, v0.s[2] 							\n"
	    "	fmul	v30.4s, v2.4s, v0.s[3] 							\n"
	    "	fmul	v31.4s, v3.4s, v0.s[3] 							\n"

	    ".endm 																			\n"


	    ".macro KERNEL4x8_K0_NGM 										\n"


	    "	ldp		q4, q5, [x24], #32 									\n"
	    "	fmla	v24.4s, v2.4s, v0.s[0]						 	\n"
	    "	fmla	v25.4s, v3.4s, v0.s[0] 							\n"
	    "	fmla	v26.4s, v2.4s, v0.s[1]						 	\n"
	    "	fmla	v27.4s, v3.4s, v0.s[1] 							\n"
	    "	ldr 	q1, [x11], #16 											\n"
	    "	fmla	v28.4s, v2.4s, v0.s[2]						 	\n"
	    "	fmla	v29.4s, v3.4s, v0.s[2] 							\n"
	    "	fmla	v30.4s, v2.4s, v0.s[3]						 	\n"
	    "	fmla	v31.4s, v3.4s, v0.s[3] 							\n"
	    "	prfm	PLDL1KEEP, [x24, #128]							\n"

	    ".endm 																			\n"



	    ".macro KERNEL4x8_K1_NGM 										\n"

	    "	ldp		q2, q3, [x24], #32 									\n"
	    "	prfm	PLDL1KEEP, [x11, #512]							\n"
	    "	fmla	v24.4s, v4.4s, v1.s[0]						 	\n"
	    "	fmla	v25.4s, v5.4s, v1.s[0] 							\n"
	    "	fmla	v26.4s, v4.4s, v1.s[1]						 	\n"
	    "	fmla	v27.4s, v5.4s, v1.s[1] 							\n"
	    "	ldr		q0,	[x11], #16											\n"
	    "	fmla	v28.4s, v4.4s, v1.s[2]						 	\n"
	    "	fmla	v29.4s, v5.4s, v1.s[2] 							\n"
	    "	fmla	v30.4s, v4.4s, v1.s[3]						 	\n"
	    "	fmla	v31.4s, v5.4s, v1.s[3] 							\n"

	    ".endm 																			\n"


	    ".macro KERNEL4x8_END_K_NGM 								\n"


	    "	fmla	v24.4s, v4.4s, v1.s[0]						 	\n"
	    "	fmla	v25.4s, v5.4s, v1.s[0] 							\n"
	    "	fmla	v26.4s, v4.4s, v1.s[1]						 	\n"
	    "	fmla	v27.4s, v5.4s, v1.s[1] 							\n"
	    "	fmla	v28.4s, v4.4s, v1.s[2]						 	\n"
	    "	fmla	v29.4s, v5.4s, v1.s[2] 							\n"
	    "	fmla	v30.4s, v4.4s, v1.s[3]						 	\n"
	    "	fmla	v31.4s, v5.4s, v1.s[3] 							\n"

	    ".endm 																			\n"


	    ".macro	M4N8_ADD_C_NGM											\n"

	    "	prfm	PLDL1KEEP, [x25, #32]								\n"
	    "	prfm 	PLDL1KEEP, [x26, #32]								\n"

	    "	ldp		q0, q1, [x25] 											\n"
	    "	fadd  v24.4s, v24.4s, v0.4s 							\n"
	    "	fadd 	v25.4s, v25.4s, v1.4s 							\n"
	    "	prfm 	PLDL1KEEP, [x27, #32]								\n"
	    "	ldp  	q2, q3, [x26]   										\n"
	    "	fadd 	v26.4s, v26.4s, v2.4s 							\n"
	    "	fadd 	v27.4s, v27.4s, v3.4s 							\n"
	    "	prfm 	PLDL1KEEP, [x28, #32]								\n"
	    "	ldp		q4, q5, [x27] 											\n"
	    "	fadd 	v28.4s, v28.4s, v4.4s 							\n"
	    "	fadd 	v29.4s, v29.4s, v5.4s 							\n"
	    "	ldp		q6, q7, [x28] 											\n"
	    "	fadd 	v30.4s, v30.4s, v6.4s 							\n"
	    "	fadd 	v31.4s, v31.4s, v7.4s 							\n"

	    ".endm 																			\n"


	    ".macro	SAVE4x8_NGM								 					\n"

	    "	stp 	q24, q25, [x25] 										\n"
	    "	add  	x25, x25, x9, lsl #4 								\n"
	    "	prfm 	PLDL2KEEP, [x25, #32]								\n"

	    "	stp 	q26, q27, [x26] 										\n"
	    "	add  	x26, x26, x9, lsl #4 								\n"
	    "	prfm 	PLDL2KEEP, [x26, #32]								\n"

	    "	stp 	q28, q29, [x27] 										\n"
	    "	add  	x27, x27, x9, lsl #4 								\n"
	    "	prfm 	PLDL2KEEP, [x27, #32]								\n"

	    "	stp 	q30, q31, [x28] 										\n"

	    ".endm 																			\n"


	    ".macro KERNEL2x8_BEGIN_K_NGM 							\n"

	    "	ld2		{v0.s, v1.s}[0],	[x11], #8					\n"
	    "	prfm	PLDL1KEEP, [x11, #256]							\n"
	    "	ldp		q2, q3, [x24], #32									\n"

	    "	fmul	v28.4s, v2.4s, v0.s[0] 							\n"
	    "	fmul	v29.4s, v3.4s, v0.s[0] 							\n"
	    "	ld2		{v4.s, v5.s}[0],	[x11], #8					\n"
	    "	fmul	v30.4s, v2.4s, v1.s[0] 							\n"
	    "	prfm	PLDL1KEEP, [x24, #128]							\n"
	    "	fmul	v31.4s, v3.4s, v1.s[0] 							\n"
	    "	ldp		q6, q7, [x24], #32									\n"

	    ".endm 																			\n"


	    ".macro KERNEL2x8_K0_NGM 										\n"


	    "	ldp		q6, q7, [x24], #32 									\n"
	    "	fmla	v28.4s, v2.4s, v0.s[0]						 	\n"
	    "	fmla	v29.4s, v3.4s, v0.s[0] 							\n"
	    "	ld2 	{v4.s, v5.s}[0], [x11], #8 					\n"
	    "	fmla	v30.4s, v2.4s, v1.s[0]						 	\n"
	    "	fmla	v31.4s, v3.4s, v1.s[0] 							\n"
	    "	prfm	PLDL1KEEP, [x24, #128]							\n"

	    ".endm 																			\n"



	    ".macro KERNEL2x8_K1_NGM 										\n"

	    "	ldp		q2, q3, [x24], #32 									\n"
	    "	prfm	PLDL1KEEP, [x11, #128]							\n"
	    "	fmla	v28.4s, v6.4s, v4.s[0]						 	\n"
	    "	fmla	v29.4s, v7.4s, v4.s[0] 							\n"
	    "	ld2		{v0.s, v1.s}[0], [x11], #8					\n"
	    "	fmla	v30.4s, v6.4s, v5.s[0]						 	\n"
	    "	fmla	v31.4s, v7.4s, v5.s[0] 							\n"

	    ".endm 																			\n"


	    ".macro KERNEL2x8_END_K_NGM 								\n"

	    "	fmla	v28.4s, v6.4s, v4.s[0]						 	\n"
	    "	fmla	v29.4s, v7.4s, v4.s[0] 							\n"
	    "	fmla	v30.4s, v6.4s, v5.s[0]						 	\n"
	    "	fmla	v31.4s, v7.4s, v5.s[0] 							\n"

	    ".endm 																			\n"


	    ".macro	M2N8_ADD_C_NGM											\n"

	    "	prfm	PLDL1KEEP, [x25, #32]								\n"
	    "	prfm 	PLDL1KEEP, [x26, #32]								\n"

	    "	ldp		q0, q1, [x25] 											\n"
	    "	fadd  v28.4s, v28.4s, v0.4s 							\n"
	    "	fadd 	v29.4s, v29.4s, v1.4s 							\n"
	    "	ldp  	q2, q3, [x26]   										\n"
	    "	fadd 	v30.4s, v30.4s, v2.4s 							\n"
	    "	fadd 	v31.4s, v31.4s, v3.4s 							\n"

	    ".endm 																			\n"


	    ".macro	SAVE2x8_NGM								 					\n"

	    "	stp 	q28, q29, [x25] 										\n"
	    "	add  	x25, x25, x9, lsl #3 								\n"
	    "	prfm 	PLDL2KEEP, [x25, #16]								\n"
	    "	stp 	q30, q31, [x26] 										\n"

	    ".endm 																			\n"



	    "SMM_NN_KERNEL8x8:									 	 	\n"

	    "	ldr		x0, %[C]						 				\n"
	    "	ldr		x1, %[A]						 				\n"
	    "	ldr		x2, %[B]						 				\n"

	    "	ldr		x3, %[M]						 				\n"
	    "	ldr		x4, %[N]						 				\n"  // N = 8
	    "	ldr		x5, %[K]						 				\n"
	    "	ldr		x9, %[LN] 						 				\n"
	    "	ldr		x6, %[LK]						 				\n"

	    "	prfm	PLDL1KEEP, [x1, #512]            				\n"

	    "	ldr		x7, %[m_tag] 					 				\n"
	    "	ldr		x8, %[k_tag] 					 				\n"
	    "	ldr		x30, %[id_tag] 					 				\n"

//-----------------------------------------------------------------------------
	    "	cmp		x7, #0 											\n"
	    "	bne 	NN8x8_addr 										\n"
	    "	ldr		x10, %[SB]						 				\n"
	    "	cmp		x30, #0 										\n"
	    "	beq		BEGIN_NN8x8 									\n"
	    "NN8x8_addr:												\n"
	    "	ldr 	x10, %[Bc]										\n"

	    "BEGIN_NN8x8:  												\n"



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


	    "PACK_NN_KERNEL8x8:													\n"

	    "	mov		x24, x10						 								\n"   //还原SB的地址
	    "	mov		x12, x2						 	 								\n"   // B0*
	    "	add		x13, x12, x9, lsl #2 			 					\n"   // B0 + sizeof(float) * LN

	    "	prfm    PLDL1KEEP, [x12, #16]			 				\n"
	    "	prfm    PLDL1KEEP, [x13, #16]			 				\n"
	    "	prfm    PLDL1KEEP, [x11, #256]			 			\n"

	    "PACK_Body_NN_KERNEL8x8:								 		\n"

	    "	lsr		x22, x5, #2 					 							\n"     // K / 8

	    "	PACK_KERNEL8x8_BEGIN_K_NGM				 	 			\n"

	    "	subs	x22, x22, #1		     		 						\n"
	    "	b 		PACK_K1_3_8x8						 						\n"

	    "PACK_K_8x8:									 							\n"

	    "	PACK_KERNEL8x8_K0_NGM						 					\n"

	    "PACK_K1_3_8x8:									 						\n"

	    "	PACK_KERNEL8x8_K1_NGM						 					\n"
	    "	PACK_KERNEL8x8_K0_NGM						 					\n"

	    "	beq		PACK_Edge_K_8x8_NGM						 			\n"

	    "	PACK_KERNEL8x8_K1_NGM						 					\n"

	    "	subs	x22, x22, #1			 		 							\n"
	    "	b 		PACK_K_8x8			 				 						\n"

	    "PACK_Edge_K_8x8_NGM:							     			\n"

	    "	PACK_KERNEL8x8_END_K_NGM			         		\n"

// the edge of K

	    "	ands 	x22, x5, #3 												\n"
	    "	beq		PACK_BEGIN_SAVE8x8_NGM 							\n"
	    "PACK_RIGHT_LOOP_K_8x8: 										\n"
	    "	subs 	x22, x22, #1 												\n"
	    "	PACK_KERNEL8x8_RIGHT_K_NGM								\n"
	    "bgt		PACK_RIGHT_LOOP_K_8x8								\n"


	    "PACK_BEGIN_SAVE8x8_NGM:										\n"

	    "	cmp		x8, #0							 								\n"
	    "	beq		M8N8_PACK_SAVE_NGM 					 				\n"
	    "	M8N8_PACK_ADD_C														\n"

	    "M8N8_PACK_SAVE_NGM: 							 					\n"
	    "	SAVE8x8_NGM								 								\n"

	    "	beq		M8x8_END							 							\n"



//--------------------------------------------------------------------
	    "BEGIN_M8_KERNEL_8x8:                       \n"


	    " mov     x24, x10                    		 	\n"   //  address_B, x11 is address_A + offset

	    " prfm    PLDL1KEEP, [x24, #256]    		 		\n"
	    " prfm    PLDL1KEEP, [x11, #256]    		 		\n"

	    "Body_K_8x8:                            		\n"

	    " lsr   x22, x5, #2                 		 		\n"     // K / 4
	    " KERNEL8x8_BEGIN_K_NGM                		 	\n"
	    " subs  x22, x22, #1                		 		\n"
	    " b 	K1_7_8x8                            	\n"

	    "Main_K_8x8:                            		\n"

	    " KERNEL8x8_K0_NGM                     			\n"
	    "K1_7_8x8:                              		\n"

	    " KERNEL8x8_K1_NGM                     		 	\n"
	    " KERNEL8x8_K0_NGM                     		 	\n"

	    " beq   Edge_K_8x8_NGM                      \n"

	    " KERNEL8x8_K1_NGM                     		 	\n"

	    " subs  x22, x22, #1                		 		\n"
	    " b     Main_K_8x8                      		\n"

	    "Edge_K_8x8_NGM:                            \n"

	    " KERNEL8x8_END_K_NGM                 			\n"

// the edge of K

	    "	ands 	x22, x5, #3 												\n"
	    "	beq		BEGIN_SAVE8x8_NGM 									\n"
	    "RIGHT_LOOP_K_8x8: 													\n"
	    "	subs 	x22, x22, #1 												\n"
	    "	KERNEL8x8_RIGHT_K_NGM											\n"
	    "bgt		RIGHT_LOOP_K_8x8										\n"

	    "BEGIN_SAVE8x8_NGM: 												\n"

	    "	cmp		x8, #0							 								\n"
	    "	beq		M8N8_SAVE_NGM 					 	 					\n"
	    "	M8N8_PACK_ADD_C 							 	 					\n"
	    "M8N8_SAVE_NGM: 							     					\n"
	    "	SAVE8x8_NGM								 	 							\n"

	    " bgt 	BEGIN_M8_KERNEL_8x8               	\n"
//---------------------------------------------------------------------------

	    "M8x8_END:																	\n"

	    "	ands	x21, x3, #7 												\n"
	    "	beq		N8_END_NGM 													\n"

	    "BEGIN_M4_KERNEL_4x8_NGM:										\n"

	    "	cmp		x21, #4 														\n"
	    "	blt 	BEGIN_M2_KERNEL_2x8_NGM 						\n"

	    "	mov     x24, x10                    			\n"
	    " prfm    PLDL1KEEP, [x24, #256]    		 		\n"
	    " prfm    PLDL1KEEP, [x11, #256]    		 		\n"

	    "Body_K_4x8_NGM:                            \n"

	    " lsr   x22, x5, #2                 		 		\n"     // K / 4
	    " KERNEL4x8_BEGIN_K_NGM                		 	\n"
	    " subs  x22, x22, #1                		 		\n"
	    " b 	K1_7_4x8_NGM                          \n"

	    "Main_K_4x8_NGM:                            \n"

	    " KERNEL4x8_K0_NGM                     			\n"
	    "K1_7_4x8_NGM:                              \n"

	    " KERNEL4x8_K1_NGM                     		 	\n"
	    " KERNEL4x8_K0_NGM                     		 	\n"

	    " beq   Edge_K_4x8_NGM                     	\n"

	    " KERNEL4x8_K1_NGM                     		 	\n"

	    " subs  x22, x22, #1                		 		\n"
	    " b     Main_K_4x8_NGM                     	\n"

	    "Edge_K_4x8_NGM:                            \n"

	    " KERNEL4x8_END_K_NGM                 			\n"

	    "	cmp		x8, #0							 								\n"
	    "	beq		M4N8_SAVE_NGM 					 	 					\n"
	    "	M4N8_ADD_C_NGM 							 	 						\n"
	    "M4N8_SAVE_NGM: 							     					\n"
	    "	SAVE4x8_NGM								 	 							\n"

	    "	sub 	x21, x21, #4 												\n"


	    "BEGIN_M2_KERNEL_2x8_NGM:										\n"

//-----------------------------------------------------------
//-----------------------------------------------------------

	    "	cmp		x21, #2 														\n"
	    "	blt 	N8_END_NGM 													\n"

	    "	mov     x24, x10                    			\n"
	    " prfm    PLDL1KEEP, [x24, #256]    		 		\n"
	    " prfm    PLDL1KEEP, [x11, #256]    		 		\n"

	    "Body_K_2x8_NGM:                            \n"

	    " lsr   x22, x5, #2                 		 		\n"     // K / 4
	    " KERNEL2x8_BEGIN_K_NGM                		 	\n"
	    " subs  x22, x22, #1                		 		\n"
	    " b 	K1_7_2x8_NGM                          \n"

	    "Main_K_2x8_NGM:                            \n"

	    " KERNEL2x8_K0_NGM                     			\n"
	    "K1_7_2x8_NGM:                              \n"

	    " KERNEL2x8_K1_NGM                     		  \n"
	    " KERNEL2x8_K0_NGM                     		 	\n"

	    " beq   Edge_K_2x8_NGM                      \n"

	    " KERNEL2x8_K1_NGM                     		 	\n"

	    " subs  x22, x22, #1                		 		\n"
	    " b     Main_K_2x8_NGM                      \n"

	    "Edge_K_2x8_NGM:                            \n"

	    " KERNEL2x8_END_K_NGM                 			\n"

	    "	cmp		x8, #0							 								\n"
	    "	beq		M2N8_SAVE_NGM 					 	 					\n"
	    "	M2N8_ADD_C_NGM 							 	 						\n"
	    "M2N8_SAVE_NGM: 							     					\n"
	    "	SAVE2x8_NGM								 	 							\n"

	    "	sub 	x21, x21, #2 												\n"


	    "N8_END_NGM: 																\n"


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
	    [Bc] "m" (Bc),
	    [SB] "m" (SB),
	    [k_tag] "m" (k_tag),
	    [m_tag] "m" (m_tag),
	    [id_tag] "m" (id_tag)
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
         long LN, long LK, float *Bc, float *SB, long stride_Bc,
         long k_tag, long m_tag, long id_tag)
{

	asm volatile(

	    ".macro	PACK_KERNEL8x12_BEGIN_K								\n"


	    "	ldr		q0,	[x11], #16									\n"
	    "	prfm	PLDL1KEEP, [x11, #512]							\n"

	    "	ldp		q2, q3, [x12]									\n"
	    "	fmul	v8.4s, v2.4s, v0.s[0]							\n"
	    "	ldr 	q4, [x12, #32]									\n"
	    "	fmul	v9.4s, v3.4s, v0.s[0]							\n"
	    "	ldr		q1, [x11], #16 									\n"
	    "	fmul	v10.4s, v4.4s, v0.s[0]							\n"

	    "	prfm	PLDL1KEEP, [x13, #96]							\n"

	    "	stp		q2, q3, [x24], #32 								\n"
	    "	fmul	v11.4s, v2.4s, v0.s[1]							\n"
	    "	str		q4, [x24], #16 									\n"
	    "	fmul	v12.4s, v3.4s, v0.s[1]							\n"
	    "	fmul	v13.4s, v4.4s, v0.s[1]							\n"

	    "	ldp 	q5, q6, [x12, #48]								\n"
	    "	fmul	v14.4s, v2.4s, v0.s[2]							\n"
	    "	fmul	v15.4s, v3.4s, v0.s[2]							\n"
	    "	stp 	q5, q6, [x23], #32 								\n"
	    "	fmul	v16.4s, v4.4s, v0.s[2]							\n"

	    "	ldr  	q7, [x12, #80] 									\n"
	    "	fmul	v17.4s, v2.4s, v0.s[3]							\n"
	    "	fmul	v18.4s, v3.4s, v0.s[3]							\n"
	    "	str 	q7, [x23], #16 									\n"
	    "	fmul	v19.4s, v4.4s, v0.s[3]							\n"

	    "	ldr  	q0, [x11], #16 									\n"

	    "	fmul	v20.4s, v2.4s, v1.s[0]							\n"
	    "	ldr 	q5, [x13]										\n"
	    "	fmul	v23.4s, v2.4s, v1.s[1]							\n"
	    "	ldr 	q6, [x13, #16]									\n"
	    "	fmul	v26.4s, v2.4s, v1.s[2]							\n"
	    "	ldr 	q7, [x13, #32]									\n"
	    "	fmul	v29.4s, v2.4s, v1.s[3]							\n"

	    "	ldr		q2, [x13, #48] 									\n"
	    "	fmul	v21.4s, v3.4s, v1.s[0]							\n"
	    "	fmul	v24.4s, v3.4s, v1.s[1]							\n"
	    "	str 	q2, [x23], #16 									\n"
	    "	fmul	v27.4s, v3.4s, v1.s[2]							\n"
	    "	fmul	v30.4s, v3.4s, v1.s[3]							\n"
	    "	ldr		q3, [x13, #64] 									\n"

	    "	fmul	v22.4s, v4.4s, v1.s[0]							\n"
	    "	fmul	v25.4s, v4.4s, v1.s[1]							\n"
	    "	str 	q3, [x23], #16 									\n"
	    "	fmul	v28.4s, v4.4s, v1.s[2]							\n"
	    "	fmul	v31.4s, v4.4s, v1.s[3]							\n"

	    "	add  	x12, x12, x9, lsl #3 							\n"
	    "	ldr 	q1, [x11], #16 									\n"
	    "	ldr		q4, [x13, #80] 									\n"
	    "	add 	x13, x13, x9, lsl #3 							\n"
	    "	str 	q4, [x23], #16 									\n"


	    ".endm 														\n"



	    ".macro	PACK_KERNEL8x12_K0									\n"


	    "	prfm 	PLDL1KEEP, [x11, #512] 							\n"
	    "	fmla	v8.4s, v2.4s, v0.s[0]							\n"
	    "	ldp 	q5, q6, [x13] 									\n"
	    "	fmla	v9.4s, v3.4s, v0.s[0]							\n"
	    "	fmla	v10.4s, v4.4s, v0.s[0]							\n"

	    "	ldr 	q7, [x13, #32] 									\n"
	    "	fmla	v11.4s, v2.4s, v0.s[1]							\n"
	    "	fmla	v12.4s, v3.4s, v0.s[1]							\n"
	    "	fmla	v13.4s, v4.4s, v0.s[1]							\n"

	    "	prfm 	PLDL1KEEP, [x11, #576] 							\n"

	    "	fmla	v14.4s, v2.4s, v0.s[2]							\n"
	    "	fmla	v15.4s, v3.4s, v0.s[2]							\n"
	    "	fmla	v16.4s, v4.4s, v0.s[2]							\n"

	    "	fmla	v17.4s, v2.4s, v0.s[3]							\n"
	    "	fmla	v18.4s, v3.4s, v0.s[3]							\n"
	    "	fmla	v19.4s, v4.4s, v0.s[3]							\n"

	    "	ldr 	q0, [x11], #16 									\n"
	    "	fmla	v20.4s, v2.4s, v1.s[0]							\n"
	    "	stp 	q2, q3, [x24], #32 								\n"
	    "	fmla	v23.4s, v2.4s, v1.s[1]							\n"
	    "	fmla	v26.4s, v2.4s, v1.s[2]							\n"
	    "	str 	q4, [x24], #16 									\n"
	    "	fmla	v29.4s, v2.4s, v1.s[3]							\n"

	    "	ldr 	q2, [x13, #48] 									\n"
	    "	fmla	v21.4s, v3.4s, v1.s[0]							\n"
	    "	fmla	v24.4s, v3.4s, v1.s[1]							\n"
	    "	str 	q2, [x23], #16 									\n"
	    "	fmla	v27.4s, v3.4s, v1.s[2]							\n"
	    "	fmla	v30.4s, v3.4s, v1.s[3]							\n"

	    "	ldr 	q3, [x13, #64] 									\n"
	    "	fmla	v22.4s, v4.4s, v1.s[0]							\n"
	    "	fmla	v25.4s, v4.4s, v1.s[1]							\n"
	    "	str 	q3, [x23], #16 									\n"
	    "	fmla	v28.4s, v4.4s, v1.s[2]							\n"
	    "	fmla	v31.4s, v4.4s, v1.s[3]							\n"

	    "	ldr 	q1, [x11], #16 									\n"
	    "	ldr 	q4, [x13, #80] 									\n"
	    "	add 	x13, x13, x9, lsl #3  							\n"
	    "	prfm 	PLDL1KEEP, [x13, #96] 							\n"
	    "	str 	q4, [x23], #16 									\n"


	    ".endm 														\n"




	    ".macro	PACK_KERNEL8x12_K1									\n"

	    "	fmla	v8.4s, v5.4s, v0.s[0]							\n"
	    "	fmla	v9.4s, v6.4s, v0.s[0]							\n"
	    "	ldp 	q2, q3, [x12] 									\n"
	    "	fmla	v10.4s, v7.4s, v0.s[0]							\n"

	    "	fmla	v11.4s, v5.4s, v0.s[1]							\n"
	    "	ldr 	q4, [x12, #32] 									\n"
	    "	fmla	v12.4s, v6.4s, v0.s[1]							\n"
	    "	fmla	v13.4s, v7.4s, v0.s[1]							\n"

	    "	fmla	v14.4s, v5.4s, v0.s[2]							\n"
	    "	fmla	v15.4s, v6.4s, v0.s[2]							\n"
	    "	fmla	v16.4s, v7.4s, v0.s[2]							\n"

	    "	fmla	v17.4s, v5.4s, v0.s[3]							\n"
	    "	fmla	v18.4s, v6.4s, v0.s[3]							\n"
	    "	fmla	v19.4s, v7.4s, v0.s[3]							\n"

	    "	ldr 	q0, [x11], #16 									\n"
	    "	fmla	v20.4s, v5.4s, v1.s[0]							\n"
	    "	stp 	q5, q6, [x24], #32 								\n"
	    "	fmla	v23.4s, v5.4s, v1.s[1]							\n"
	    "	fmla	v26.4s, v5.4s, v1.s[2]							\n"
	    "	str 	q7, [x24], #16 									\n"
	    "	fmla	v29.4s, v5.4s, v1.s[3]							\n"

	    "	ldr		q5, [x12, #48] 									\n"
	    "	fmla	v21.4s, v6.4s, v1.s[0]							\n"
	    "	fmla	v24.4s, v6.4s, v1.s[1]							\n"
	    "	str 	q5, [x23], #16 									\n"
	    "	fmla	v27.4s, v6.4s, v1.s[2]							\n"
	    "	fmla	v30.4s, v6.4s, v1.s[3]							\n"

	    "	ldr		q6, [x12, #64] 									\n"
	    "	fmla	v22.4s, v7.4s, v1.s[0]							\n"
	    "	fmla	v25.4s, v7.4s, v1.s[1]							\n"
	    "	str 	q6, [x23], #16 									\n"
	    "	fmla	v28.4s, v7.4s, v1.s[2]							\n"
	    "	fmla	v31.4s, v7.4s, v1.s[3]							\n"

	    "	ldr 	q1, [x11], #16 									\n"
	    "	ldr		q7, [x12, #80] 									\n"
	    "	add 	x12, x12, x9, lsl #3  							\n"
	    "	prfm 	PLDL1KEEP, [x12, #96] 							\n"
	    "	str  	q7, [x23], #16 									\n"

	    ".endm 														\n"



	    ".macro	PACK_KERNEL8x12_END_K								\n"

	    "	fmla	v8.4s, v5.4s, v0.s[0]							\n"
	    "	fmla	v9.4s, v6.4s, v0.s[0]							\n"
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

	    "	stp 	q5, q6, [x24], #32 								\n"
	    "	fmla	v23.4s, v5.4s, v1.s[1]							\n"
	    "	fmla	v24.4s, v6.4s, v1.s[1]							\n"
	    "	fmla	v25.4s, v7.4s, v1.s[1]							\n"

	    "	str 	q7, [x24], #16 									\n"
	    "	fmla	v26.4s, v5.4s, v1.s[2]							\n"
	    "	fmla	v27.4s, v6.4s, v1.s[2]							\n"
	    "	fmla	v28.4s, v7.4s, v1.s[2]							\n"

	    "	fmla	v29.4s, v5.4s, v1.s[3]							\n"
	    "	fmla	v30.4s, v6.4s, v1.s[3]							\n"
	    "	fmla	v31.4s, v7.4s, v1.s[3]							\n"

	    ".endm 														\n"



	    ".macro	PACK_KERNEL8x12_RIGHT_K 							\n"


	    "	ldp		q2, q3, [x12]									\n"
	    "	ldr		q0, [x11], #16 									\n"

	    "	fmla	v8.4s, v2.4s, v0.s[0]							\n"
	    "	ldr		q4, [x12, #32]									\n"
	    "	fmla	v9.4s, v3.4s, v0.s[0]							\n"
	    "	fmla	v10.4s, v4.4s, v0.s[0]							\n"

	    "	fmla	v11.4s, v2.4s, v0.s[1]							\n"
	    "	ldr		q1, [x11], #16 									\n"
	    "	fmla	v12.4s, v3.4s, v0.s[1]							\n"
	    "	fmla	v13.4s, v4.4s, v0.s[1]							\n"

	    "	ldp		q5, q6, [x12, #48]								\n"
	    "	fmla	v14.4s, v2.4s, v0.s[2]							\n"
	    "	fmla	v15.4s, v3.4s, v0.s[2]							\n"
	    "	fmla	v16.4s, v4.4s, v0.s[2]							\n"

	    "	ldr		q7, [x12, #80]									\n"
	    "	fmla	v17.4s, v2.4s, v0.s[3]							\n"
	    "	add  	x12, x12, x9, lsl #2 							\n"
	    "	fmla	v18.4s, v3.4s, v0.s[3]							\n"
	    "	prfm	PLDL1KEEP, [x12, #80] 							\n"
	    "	fmla	v19.4s, v4.4s, v0.s[3]							\n"

	    "	stp 	q2, q3, [x24], #32 								\n"
	    "	fmla	v20.4s, v2.4s, v1.s[0]							\n"
	    "	fmla	v21.4s, v3.4s, v1.s[0]							\n"
	    "	fmla	v22.4s, v4.4s, v1.s[0]							\n"

	    "	str 	q4, [x24], #16 									\n"
	    "	fmla	v23.4s, v2.4s, v1.s[1]							\n"
	    "	fmla	v24.4s, v3.4s, v1.s[1]							\n"
	    "	fmla	v25.4s, v4.4s, v1.s[1]							\n"

	    "	stp 	q5, q6, [x23], #32 								\n"
	    "	fmla	v26.4s, v2.4s, v1.s[2]							\n"
	    "	fmla	v27.4s, v3.4s, v1.s[2]							\n"
	    "	fmla	v28.4s, v4.4s, v1.s[2]							\n"

	    "	str 	q7, [x23], #16 									\n"
	    "	fmla	v29.4s, v2.4s, v1.s[3]							\n"
	    "	fmla	v30.4s, v3.4s, v1.s[3]							\n"
	    "	fmla	v31.4s, v4.4s, v1.s[3]							\n"


	    ".endm 														\n"



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	    ".macro	PACK_KERNEL8x12_N12_BEGIN_K							\n"


	    "	ldr		q0,	[x11], #16									\n"

	    "	prfm	PLDL1KEEP, [x11, #512]							\n"

	    "	ldp		q2, q3, [x12]									\n"
	    "	fmul	v8.4s, v2.4s, v0.s[0]							\n"
	    "	ldr 	q4, [x12, #32]									\n"
	    "	fmul	v9.4s, v3.4s, v0.s[0]							\n"
	    "	ldr		q1, [x11], #16 									\n"
	    "	fmul	v10.4s, v4.4s, v0.s[0]							\n"

	    "	prfm	PLDL1KEEP, [x13, #48]							\n"

	    "	add  	x12, x12, x9, lsl #3 							\n"
	    "	fmul	v11.4s, v2.4s, v0.s[1]							\n"
	    "	fmul	v12.4s, v3.4s, v0.s[1]							\n"
	    "	fmul	v13.4s, v4.4s, v0.s[1]							\n"

	    "	ldr 	q5, [x13]										\n"
	    "	fmul	v14.4s, v2.4s, v0.s[2]							\n"
	    "	fmul	v15.4s, v3.4s, v0.s[2]							\n"
	    "	fmul	v16.4s, v4.4s, v0.s[2]							\n"

	    "	ldr 	q6, [x13, #16]									\n"

	    "	fmul	v17.4s, v2.4s, v0.s[3]							\n"
	    "	fmul	v18.4s, v3.4s, v0.s[3]							\n"
	    "	fmul	v19.4s, v4.4s, v0.s[3]							\n"

	    "	ldr  	q0, [x11], #16 									\n"

	    "	fmul	v20.4s, v2.4s, v1.s[0]							\n"
	    "	fmul	v21.4s, v3.4s, v1.s[0]							\n"
	    "	fmul	v22.4s, v4.4s, v1.s[0]							\n"

	    "	ldr 	q7, [x13, #32]									\n"

	    "	fmul	v23.4s, v2.4s, v1.s[1]							\n"
	    "	fmul	v24.4s, v3.4s, v1.s[1]							\n"
	    "	fmul	v25.4s, v4.4s, v1.s[1]							\n"

	    "	stp		q2, q3, [x24], #32 								\n"
	    "	add 	x13, x13, x9, lsl #3 							\n"

	    "	fmul	v26.4s, v2.4s, v1.s[2]							\n"
	    "	fmul	v27.4s, v3.4s, v1.s[2]							\n"
	    "	fmul	v28.4s, v4.4s, v1.s[2]							\n"

	    "	str		q4, [x24], #16 									\n"

	    "	fmul	v29.4s, v2.4s, v1.s[3]							\n"
	    "	fmul	v30.4s, v3.4s, v1.s[3]							\n"
	    "	fmul	v31.4s, v4.4s, v1.s[3]							\n"

	    "	ldr 	q1, [x11], #16 									\n"


	    ".endm 														\n"



	    ".macro	PACK_KERNEL8x12_N12_K0								\n"


	    "	prfm PLDL1KEEP, [x11, #512] 							\n"
	    "	ldp 	q5, q6, [x13] 									\n"
	    "	fmla	v8.4s, v2.4s, v0.s[0]							\n"
	    "	fmla	v9.4s, v3.4s, v0.s[0]							\n"
	    "	fmla	v10.4s, v4.4s, v0.s[0]							\n"

	    "	ldr 	q7, [x13, #32] 									\n"

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

	    "	ldr 	q0, [x11], #16 									\n"

	    "	fmla	v20.4s, v2.4s, v1.s[0]							\n"
	    "	fmla	v21.4s, v3.4s, v1.s[0]							\n"
	    "	fmla	v22.4s, v4.4s, v1.s[0]							\n"

	    "	stp 	q2, q3, [x24], #32 								\n"

	    "	fmla	v23.4s, v2.4s, v1.s[1]							\n"
	    "	fmla	v24.4s, v3.4s, v1.s[1]							\n"
	    "	fmla	v25.4s, v4.4s, v1.s[1]							\n"

	    "	str 	q4, [x24], #16 									\n"

	    "	fmla	v26.4s, v2.4s, v1.s[2]							\n"
	    "	fmla	v27.4s, v3.4s, v1.s[2]							\n"
	    "	fmla	v28.4s, v4.4s, v1.s[2]							\n"

	    "	prfm PLDL1KEEP, [x11, #576] 							\n"

	    "	fmla	v29.4s, v2.4s, v1.s[3]							\n"
	    "	fmla	v30.4s, v3.4s, v1.s[3]							\n"
	    "	fmla	v31.4s, v4.4s, v1.s[3]							\n"

	    "	ldr 	q1, [x11], #16 									\n"

	    ".endm 														\n"




	    ".macro	PACK_KERNEL8x12_N12_K1								\n"

	    "	ldp 	q2, q3, [x12] 									\n"
	    "	fmla	v8.4s, v5.4s, v0.s[0]							\n"
	    "	fmla	v9.4s, v6.4s, v0.s[0]							\n"
	    "	fmla	v10.4s, v7.4s, v0.s[0]							\n"

	    "	ldr 	q4, [x12, #32] 									\n"

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

	    "	ldr 	q0, [x11], #16 									\n"

	    "	fmla	v20.4s, v5.4s, v1.s[0]							\n"
	    "	fmla	v21.4s, v6.4s, v1.s[0]							\n"
	    "	fmla	v22.4s, v7.4s, v1.s[0]							\n"

	    "	stp 	q5, q6, [x24], #32 								\n"

	    "	fmla	v23.4s, v5.4s, v1.s[1]							\n"
	    "	fmla	v24.4s, v6.4s, v1.s[1]							\n"
	    "	fmla	v25.4s, v7.4s, v1.s[1]							\n"

	    "	str 	q7, [x24], #16 									\n"

	    "	fmla	v26.4s, v5.4s, v1.s[2]							\n"
	    "	fmla	v27.4s, v6.4s, v1.s[2]							\n"
	    "	fmla	v28.4s, v7.4s, v1.s[2]							\n"

	    "	fmla	v29.4s, v5.4s, v1.s[3]							\n"
	    "	fmla	v30.4s, v6.4s, v1.s[3]							\n"
	    "	fmla	v31.4s, v7.4s, v1.s[3]							\n"

	    "	ldr 	q1, [x11], #16 									\n"

	    ".endm 														\n"


	    ".macro	PACK_KERNEL8x12_N12_END_K							\n"

	    "	fmla	v8.4s, v5.4s, v0.s[0]							\n"
	    "	fmla	v9.4s, v6.4s, v0.s[0]							\n"
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

	    "	stp 	q5, q6, [x24], #32 								\n"

	    "	fmla	v23.4s, v5.4s, v1.s[1]							\n"
	    "	fmla	v24.4s, v6.4s, v1.s[1]							\n"
	    "	fmla	v25.4s, v7.4s, v1.s[1]							\n"

	    "	str 	q7, [x24], #16 									\n"

	    "	fmla	v26.4s, v5.4s, v1.s[2]							\n"
	    "	fmla	v27.4s, v6.4s, v1.s[2]							\n"
	    "	fmla	v28.4s, v7.4s, v1.s[2]							\n"

	    "	fmla	v29.4s, v5.4s, v1.s[3]							\n"
	    "	fmla	v30.4s, v6.4s, v1.s[3]							\n"
	    "	fmla	v31.4s, v7.4s, v1.s[3]							\n"

	    ".endm 														\n"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


	    ".macro	M8N12_PACK_ADD_C									\n"


	    "	prfm	PLDL1KEEP, [x25, #64]							\n"
	    "	prfm 	PLDL1KEEP, [x26, #64]							\n"

	    "	ldp		q0, q1, [x25] 									\n"
	    "	fadd 	v8.4s, v8.4s, v0.4s 							\n"
	    "	prfm 	PLDL1KEEP, [x27, #64]							\n"
	    "	ldr  	q2, [x25, #32]   								\n"
	    "	fadd 	v9.4s, v9.4s, v1.4s 							\n"
	    "	ldp		q3, q4, [x26] 									\n"
	    "	prfm 	PLDL1KEEP, [x28, #64]							\n"
	    "	fadd 	v10.4s, v10.4s, v2.4s 							\n"

	    "	prfm 	PLDL1KEEP, [x15, #64]							\n"
	    "	ldr 	q5, [x26, #32] 									\n"
	    "	fadd 	v11.4s, v11.4s, v3.4s 							\n"
	    "	prfm 	PLDL1KEEP, [x16, #64]							\n"
	    "	ldp		q6, q7, [x27] 									\n"
	    "	fadd 	v12.4s, v12.4s, v4.4s 							\n"
	    "	prfm 	PLDL1KEEP, [x17, #64]							\n"
	    "	ldr 	q0, [x27, #32] 									\n"
	    "	fadd 	v13.4s, v13.4s, v5.4s 							\n"

	    "	prfm 	PLDL1KEEP, [x18, #64]							\n"

	    "	ldp 	q1, q2, [x28] 									\n"
	    "	fadd 	v14.4s, v14.4s, v6.4s 							\n"
	    "	ldr 	q3, [x28, #32] 									\n"
	    "	fadd 	v15.4s, v15.4s, v7.4s 							\n"
	    "	fadd 	v16.4s, v16.4s, v0.4s 							\n"

	    "	ldp 	q4, q5, [x15] 									\n"
	    "	fadd 	v17.4s, v17.4s, v1.4s 							\n"
	    "	ldr  	q6, [x15, #32] 									\n"
	    "	fadd 	v18.4s, v18.4s, v2.4s 							\n"
	    "	ldr 	q7, [x16] 										\n"
	    "	fadd 	v19.4s, v19.4s, v3.4s 							\n"

	    "	ldp 	q0, q1, [x16, #16] 								\n"
	    "	fadd 	v20.4s, v20.4s, v4.4s 							\n"
	    "	ldp 	q2, q3, [x17] 									\n"
	    "	fadd 	v21.4s, v21.4s, v5.4s 							\n"
	    "	ldr 	q4, [x17, #32]									\n"
	    "	fadd 	v22.4s, v22.4s, v6.4s 							\n"


	    "	ldp 	q5, q6, [x18]  									\n"
	    "	fadd 	v23.4s, v23.4s, v7.4s 				 			\n"
	    "	ldr 	q7, [x18, #32]									\n"
	    "	fadd 	v24.4s, v24.4s, v0.4s 							\n"
	    "	fadd 	v25.4s, v25.4s, v1.4s 							\n"

	    "	fadd 	v26.4s, v26.4s, v2.4s 							\n"
	    "	fadd 	v27.4s, v27.4s, v3.4s 							\n"
	    "	fadd 	v28.4s, v28.4s, v4.4s 							\n"


	    "	fadd 	v29.4s, v29.4s, v5.4s 							\n"
	    "	fadd 	v30.4s, v30.4s, v6.4s 							\n"
	    "	fadd 	v31.4s, v31.4s, v7.4s 							\n"

	    ".endm	 													\n"



	    ".macro	SAVE8x12											\n"


	    "	subs	x21, x21, #1 									\n"

	    "	stp 	q8, q9, [x25]									\n"
	    "	str		q10, [x25, #32]									\n"
	    "	add  	x25, x25, x9, lsl #5 							\n"

	    "	prfm 	PLDL2KEEP, [x25, #64]							\n"

	    "	stp 	q11, q12, [x26]									\n"
	    "	str		q13, [x26, #32]									\n"
	    "	add  	x26, x26, x9, lsl #5 							\n"

	    "	prfm 	PLDL2KEEP, [x26, #64]							\n"

	    "	stp 	q14, q15, [x27]									\n"
	    "	str		q16, [x27, #32]									\n"
	    "	add  	x27, x27, x9, lsl #5 							\n"

	    "	prfm 	PLDL2KEEP, [x27, #64]							\n"

	    "	stp 	q17, q18, [x28]									\n"
	    "	str		q19, [x28, #32]									\n"
	    "	add  	x28, x28, x9, lsl #5 							\n"

	    "	prfm 	PLDL2KEEP, [x28, #64]							\n"

	    "	stp 	q20, q21, [x15]									\n"
	    "	str		q22, [x15, #32]									\n"
	    "	add  	x15, x15, x9, lsl #5 							\n"

	    "	prfm 	PLDL2KEEP, [x15, #64]							\n"

	    "	stp 	q23, q24, [x16]									\n"
	    "	str		q25, [x16, #32]									\n"
	    "	add  	x16, x16, x9, lsl #5 							\n"

	    "	prfm 	PLDL2KEEP, [x16, #64]							\n"

	    "	stp 	q26, q27, [x17]									\n"
	    "	str		q28, [x17, #32]									\n"
	    "	add  	x17, x17, x9, lsl #5 							\n"

	    "	prfm 	PLDL2KEEP, [x17, #64]							\n"

	    "	stp 	q29, q30, [x18]									\n"
	    "	str		q31, [x18, #32]									\n"
	    "	add  	x18, x18, x9, lsl #5 							\n"

	    "	prfm 	PLDL2KEEP, [x18, #64]							\n"

	    ".endm	 													\n"



	    ".macro	KERNEL8x12_BEGIN_K									\n"


	    "	ldp		q0, q1, [x11], #32								\n"
	    "	prfm	PLDL1KEEP, [x11, #2560]							\n"
	    "	ldr		q2, [x24]										\n"

	    "	fmul	v8.4s,	v2.4s, v0.s[0]							\n"
	    "	fmul	v11.4s,	v2.4s, v0.s[1]							\n"
	    "	ldr 	q3, [x24, #16] 									\n"
	    "	fmul	v14.4s,	v2.4s, v0.s[2]							\n"
	    "	fmul	v17.4s,	v2.4s, v0.s[3]							\n"
	    "	ldr 	q4, [x24, #32] 									\n"
	    "	fmul	v20.4s,	v2.4s, v1.s[0]							\n"
	    "	fmul	v23.4s,	v2.4s, v1.s[1]							\n"
	    "	ldr 	q7, [x24, #48] 									\n"
	    "	fmul	v26.4s,	v2.4s, v1.s[2]							\n"
	    "	fmul	v29.4s,	v2.4s, v1.s[3]							\n"

	    "	ldr 	q2, [x24, #64] 									\n"

	    "	fmul	v9.4s,	v3.4s, v0.s[0]							\n"
	    "	fmul	v12.4s,	v3.4s, v0.s[1]							\n"
	    "	ldr 	q5, [x11], #16									\n"
	    "	fmul	v15.4s,	v3.4s, v0.s[2]							\n"
	    "	fmul	v18.4s,	v3.4s, v0.s[3]							\n"
	    "	ldr 	q6, [x11], #16									\n"
	    "	fmul	v21.4s,	v3.4s, v1.s[0]							\n"
	    "	fmul	v24.4s,	v3.4s, v1.s[1]							\n"
	    "	fmul	v27.4s,	v3.4s, v1.s[2]							\n"
	    "	fmul	v30.4s,	v3.4s, v1.s[3]							\n"

	    "	ldr 	q3, [x24, #80] 									\n"

	    "	fmul	v10.4s,	v4.4s, v0.s[0]							\n"
	    "	fmul	v13.4s,	v4.4s, v0.s[1]							\n"
	    "	fmul	v16.4s,	v4.4s, v0.s[2]							\n"
	    "	prfm	PLDL1KEEP, [x11, #2560]							\n"
	    "	fmul	v19.4s,	v4.4s, v0.s[3]							\n"
	    "	fmul	v22.4s,	v4.4s, v1.s[0]							\n"
	    "	fmul	v25.4s,	v4.4s, v1.s[1]							\n"
	    "	fmul	v28.4s,	v4.4s, v1.s[2]							\n"
	    "	fmul	v31.4s,	v4.4s, v1.s[3]							\n"

	    "	add 	x24, x24, #96 									\n"

	    ".endm 														\n"



	    ".macro	KERNEL8x12_K0										\n"

	    "	ldr 	q5, [x11], #16									\n"
	    "	fmla	v8.4s,	v2.4s, v0.s[0]							\n"
	    "	fmla	v11.4s,	v2.4s, v0.s[1]							\n"
	    "	ldr 	q6, [x11], #16									\n"
	    "	fmla	v14.4s,	v2.4s, v0.s[2]							\n"
	    "	fmla	v17.4s,	v2.4s, v0.s[3]							\n"
	    "	ldr 	q7, [x24]										\n"
	    "	fmla	v20.4s,	v2.4s, v1.s[0]							\n"
	    "	fmla	v23.4s,	v2.4s, v1.s[1]							\n"
	    "	fmla	v26.4s,	v2.4s, v1.s[2]							\n"
	    "	fmla	v29.4s,	v2.4s, v1.s[3]							\n"

	    "	ldr 	q2, [x24, #16] 									\n"

	    "	fmla	v9.4s,	v3.4s, v0.s[0]							\n"
	    "	fmla	v12.4s,	v3.4s, v0.s[1]							\n"
	    "	fmla	v15.4s,	v3.4s, v0.s[2]							\n"
	    "	fmla	v18.4s,	v3.4s, v0.s[3]							\n"
	    "	prfm 	PLDL1KEEP, [x11, #2560] 						\n"
	    "	fmla	v21.4s,	v3.4s, v1.s[0]							\n"
	    "	fmla	v24.4s,	v3.4s, v1.s[1]							\n"
	    "	fmla	v27.4s,	v3.4s, v1.s[2]							\n"
	    "	fmla	v30.4s,	v3.4s, v1.s[3]							\n"

	    "	ldr 	q3, [x24, #32] 									\n"

	    "	fmla	v10.4s,	v4.4s, v0.s[0]							\n"
	    "	fmla	v13.4s,	v4.4s, v0.s[1]							\n"
	    "	fmla	v16.4s,	v4.4s, v0.s[2]							\n"
	    "	fmla	v19.4s,	v4.4s, v0.s[3]							\n"
	    "	prfm 	PLDL1KEEP, [x11, #2624] 						\n"
	    "	fmla	v22.4s,	v4.4s, v1.s[0]							\n"
	    "	fmla	v25.4s,	v4.4s, v1.s[1]							\n"
	    "	fmla	v28.4s,	v4.4s, v1.s[2]							\n"
	    "	fmla	v31.4s,	v4.4s, v1.s[3]							\n"

	    "	add 	x24, x24, #48 									\n"

	    ".endm 														\n"



	    ".macro	KERNEL8x12_K1										\n"

	    "	ldr 	q0, [x11], #16									\n"
	    "	fmla	v8.4s,	v7.4s, v5.s[0]							\n"
	    "	fmla	v11.4s,	v7.4s, v5.s[1]							\n"
	    "	ldr 	q1, [x11], #16									\n"
	    "	fmla	v14.4s,	v7.4s, v5.s[2]							\n"
	    "	fmla	v17.4s,	v7.4s, v5.s[3]							\n"
	    "	ldr 	q4, [x24] 										\n"
	    "	fmla	v20.4s,	v7.4s, v6.s[0]							\n"
	    "	fmla	v23.4s,	v7.4s, v6.s[1]							\n"
	    "	fmla	v26.4s,	v7.4s, v6.s[2]							\n"
	    "	fmla	v29.4s,	v7.4s, v6.s[3]							\n"

	    "	ldr 	q7, [x24, #16] 									\n"

	    "	fmla	v9.4s,	v2.4s, v5.s[0]							\n"
	    "	fmla	v12.4s,	v2.4s, v5.s[1]							\n"
	    "	fmla	v15.4s,	v2.4s, v5.s[2]							\n"
	    "	fmla	v18.4s,	v2.4s, v5.s[3]							\n"
	    "	prfm 	PLDL1KEEP, [x24, #256] 							\n"
	    "	fmla	v21.4s,	v2.4s, v6.s[0]							\n"
	    "	fmla	v24.4s,	v2.4s, v6.s[1]							\n"
	    "	fmla	v27.4s,	v2.4s, v6.s[2]							\n"
	    "	fmla	v30.4s,	v2.4s, v6.s[3]							\n"

	    "	ldr 	q2, [x24, #32] 									\n"

	    "	fmla	v10.4s,	v3.4s, v5.s[0]							\n"
	    "	fmla	v13.4s,	v3.4s, v5.s[1]							\n"
	    "	fmla	v16.4s,	v3.4s, v5.s[2]							\n"
	    "	fmla	v19.4s,	v3.4s, v5.s[3]							\n"
	    "	fmla	v22.4s,	v3.4s, v6.s[0]							\n"
	    "	fmla	v25.4s,	v3.4s, v6.s[1]							\n"
	    "	fmla	v28.4s,	v3.4s, v6.s[2]							\n"
	    "	fmla	v31.4s,	v3.4s, v6.s[3]							\n"

	    "	add 	x24, x24, #48 									\n"


	    ".endm 														\n"


	    ".macro	KERNEL8x12_K2										\n"

	    "	ldr 	q5, [x11], #16									\n"
	    "	fmla	v8.4s,	v4.4s, v0.s[0]							\n"
	    "	fmla	v11.4s,	v4.4s, v0.s[1]							\n"
	    "	ldr 	q6, [x11], #16									\n"
	    "	fmla	v14.4s,	v4.4s, v0.s[2]							\n"
	    "	fmla	v17.4s,	v4.4s, v0.s[3]							\n"
	    "	ldr 	q3, [x24] 										\n"
	    "	fmla	v20.4s,	v4.4s, v1.s[0]							\n"
	    "	fmla	v23.4s,	v4.4s, v1.s[1]							\n"
	    "	fmla	v26.4s,	v4.4s, v1.s[2]							\n"
	    "	fmla	v29.4s,	v4.4s, v1.s[3]							\n"

	    "	ldr 	q4, [x24, #16] 									\n"

	    "	fmla	v9.4s,	v7.4s, v0.s[0]							\n"
	    "	fmla	v12.4s,	v7.4s, v0.s[1]							\n"
	    "	fmla	v15.4s,	v7.4s, v0.s[2]							\n"
	    "	fmla	v18.4s,	v7.4s, v0.s[3]							\n"
	    "	prfm 	PLDL1KEEP, [x11, #2560] 						\n"
	    "	fmla	v21.4s,	v7.4s, v1.s[0]							\n"
	    "	fmla	v24.4s,	v7.4s, v1.s[1]							\n"
	    "	fmla	v27.4s,	v7.4s, v1.s[2]							\n"
	    "	fmla	v30.4s,	v7.4s, v1.s[3]							\n"

	    "	ldr 	q7, [x24, #32] 									\n"

	    "	fmla	v10.4s,	v2.4s, v0.s[0]							\n"
	    "	fmla	v13.4s,	v2.4s, v0.s[1]							\n"
	    "	fmla	v16.4s,	v2.4s, v0.s[2]							\n"
	    "	prfm 	PLDL1KEEP, [x11, #2624] 						\n"
	    "	fmla	v19.4s,	v2.4s, v0.s[3]							\n"
	    "	fmla	v22.4s,	v2.4s, v1.s[0]							\n"
	    "	fmla	v25.4s,	v2.4s, v1.s[1]							\n"
	    "	fmla	v28.4s,	v2.4s, v1.s[2]							\n"
	    "	fmla	v31.4s,	v2.4s, v1.s[3]							\n"

	    "	add 	x24, x24, #48 									\n"


	    ".endm 														\n"



	    ".macro	KERNEL8x12_K3										\n"

	    "	ldr 	q0, [x11], #16									\n"
	    "	fmla	v8.4s,	v3.4s, v5.s[0]							\n"
	    "	fmla	v11.4s,	v3.4s, v5.s[1]							\n"
	    "	ldr 	q1, [x11], #16									\n"
	    "	fmla	v14.4s,	v3.4s, v5.s[2]							\n"
	    "	fmla	v17.4s,	v3.4s, v5.s[3]							\n"
	    "	ldr 	q2, [x24] 										\n"
	    "	fmla	v20.4s,	v3.4s, v6.s[0]							\n"
	    "	fmla	v23.4s,	v3.4s, v6.s[1]							\n"
	    "	fmla	v26.4s,	v3.4s, v6.s[2]							\n"
	    "	fmla	v29.4s,	v3.4s, v6.s[3]							\n"

	    "	ldr 	q3, [x24, #16] 									\n"

	    "	fmla	v9.4s,	v4.4s, v5.s[0]							\n"
	    "	fmla	v12.4s,	v4.4s, v5.s[1]							\n"
	    "	fmla	v15.4s,	v4.4s, v5.s[2]							\n"
	    "	fmla	v18.4s,	v4.4s, v5.s[3]							\n"
	    "	prfm 	PLDL1KEEP, [x24, #256] 							\n"
	    "	fmla	v21.4s,	v4.4s, v6.s[0]							\n"
	    "	fmla	v24.4s,	v4.4s, v6.s[1]							\n"
	    "	fmla	v27.4s,	v4.4s, v6.s[2]							\n"
	    "	fmla	v30.4s,	v4.4s, v6.s[3]							\n"

	    "	ldr 	q4, [x24, #32] 									\n"

	    "	fmla	v10.4s,	v7.4s, v5.s[0]							\n"
	    "	fmla	v13.4s,	v7.4s, v5.s[1]							\n"
	    "	fmla	v16.4s,	v7.4s, v5.s[2]							\n"
	    "	fmla	v19.4s,	v7.4s, v5.s[3]							\n"
	    "	fmla	v22.4s,	v7.4s, v6.s[0]							\n"
	    "	fmla	v25.4s,	v7.4s, v6.s[1]							\n"
	    "	fmla	v28.4s,	v7.4s, v6.s[2]							\n"
	    "	fmla	v31.4s,	v7.4s, v6.s[3]							\n"

	    "	add 	x24, x24, #48 									\n"


	    ".endm 														\n"



	    ".macro	KERNEL8x12_END_K									\n"

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

	    ".endm 														\n"


	    ".macro	KERNEL8x12_RIGHT_K 									\n"


	    "	ldp		q2, q3, [x24], #32								\n"
	    "	ldr		q0, [x11], #16 									\n"

	    "	fmla	v8.4s, v2.4s, v0.s[0]							\n"
	    "	ldr		q4, [x24], #16									\n"
	    "	fmla	v9.4s, v3.4s, v0.s[0]							\n"
	    "	fmla	v10.4s, v4.4s, v0.s[0]							\n"

	    "	fmla	v11.4s, v2.4s, v0.s[1]							\n"
	    "	ldr		q1, [x11], #16 									\n"
	    "	fmla	v12.4s, v3.4s, v0.s[1]							\n"
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


	    ".endm 														\n"



	    ".macro	KERNEL4x12_BEGIN_K 									\n"

	    "	ldr 	q0, [x11], #16 									\n"
	    "	prfm	PLDL1KEEP, [x11, #2560]							\n"
	    "	ldr		q2, [x24], #16									\n"

	    "	fmul	v20.4s, v2.4s, v0.s[0]							\n"
	    "	ldr		q3, [x24], #16									\n"
	    "	fmul	v23.4s, v2.4s, v0.s[1] 							\n"
	    "	ldr		q4, [x24], #16									\n"
	    "	fmul	v26.4s, v2.4s, v0.s[2]							\n"
	    "	fmul	v29.4s, v2.4s, v0.s[3]							\n"

	    "	prfm	PLDL1KEEP, [x24, #256]							\n"

	    "	ldr		q1, [x11], #16									\n"
	    "	fmul	v21.4s, v3.4s, v0.s[0]							\n"
	    "	fmul	v24.4s, v3.4s, v0.s[1]							\n"
	    "	ldp		q5, q6, [x24], #32								\n"
	    "	fmul	v27.4s, v3.4s, v0.s[2]							\n"
	    "	fmul	v30.4s, v3.4s, v0.s[3]							\n"

	    "	fmul	v22.4s, v4.4s, v0.s[0]							\n"
	    "	ldr 	q7, [x24], #16 									\n"
	    "	fmul	v25.4s, v4.4s, v0.s[1]							\n"
	    "	fmul	v28.4s, v4.4s, v0.s[2]							\n"
	    "	fmul	v31.4s, v4.4s, v0.s[3]							\n"

	    ".endm 														\n"




	    ".macro	KERNEL4x12_K0 										\n"

	    "	ldr		q1, [x11], #16 									\n"
	    "	fmla	v20.4s, v2.4s, v0.s[0]							\n"
	    "	fmla	v23.4s, v2.4s, v0.s[1] 							\n"
	    "	ldp 	q5, q6, [x24], #32 								\n"
	    "	fmla	v26.4s, v2.4s, v0.s[2]							\n"
	    "	fmla	v29.4s, v2.4s, v0.s[3]							\n"
	    "	ldr 	q7, [x24], #16 									\n"

	    "	prfm	PLDL1KEEP, [x24, #256]							\n"

	    "	fmla	v21.4s, v3.4s, v0.s[0]							\n"
	    "	fmla	v24.4s, v3.4s, v0.s[1]							\n"
	    "	fmla	v27.4s, v3.4s, v0.s[2]							\n"
	    "	fmla	v30.4s, v3.4s, v0.s[3]							\n"

	    "	fmla	v22.4s, v4.4s, v0.s[0]							\n"
	    "	fmla	v25.4s, v4.4s, v0.s[1]							\n"
	    "	fmla	v28.4s, v4.4s, v0.s[2]							\n"
	    "	fmla	v31.4s, v4.4s, v0.s[3]							\n"

	    ".endm 														\n"



	    ".macro	KERNEL4x12_K1 										\n"

	    "	ldr		q0, [x11], #16 									\n"
	    "	fmla	v20.4s, v5.4s, v1.s[0]							\n"
	    "	fmla	v23.4s, v5.4s, v1.s[1] 							\n"
	    "	ldp 	q2, q3, [x24], #32 								\n"
	    "	fmla	v26.4s, v5.4s, v1.s[2]							\n"
	    "	fmla	v29.4s, v5.4s, v1.s[3]							\n"
	    "	ldr 	q4, [x24], #16 									\n"

	    "	prfm	PLDL1KEEP, [x11, #256]							\n"

	    "	fmla	v21.4s, v6.4s, v1.s[0]							\n"
	    "	fmla	v24.4s, v6.4s, v1.s[1]							\n"
	    "	fmla	v27.4s, v6.4s, v1.s[2]							\n"
	    "	fmla	v30.4s, v6.4s, v1.s[3]							\n"

	    "	fmla	v22.4s, v7.4s, v1.s[0]							\n"
	    "	fmla	v25.4s, v7.4s, v1.s[1]							\n"
	    "	fmla	v28.4s, v7.4s, v1.s[2]							\n"
	    "	fmla	v31.4s, v7.4s, v1.s[3]							\n"

	    ".endm 														\n"


	    ".macro	KERNEL4x12_END_K 									\n"

	    "	fmla	v20.4s, v5.4s, v1.s[0]							\n"
	    "	fmla	v23.4s, v5.4s, v1.s[1] 							\n"
	    "	fmla	v26.4s, v5.4s, v1.s[2]							\n"
	    "	fmla	v29.4s, v5.4s, v1.s[3]							\n"

	    "	fmla	v21.4s, v6.4s, v1.s[0]							\n"
	    "	fmla	v24.4s, v6.4s, v1.s[1]							\n"
	    "	fmla	v27.4s, v6.4s, v1.s[2]							\n"
	    "	fmla	v30.4s, v6.4s, v1.s[3]							\n"

	    "	fmla	v22.4s, v7.4s, v1.s[0]							\n"
	    "	fmla	v25.4s, v7.4s, v1.s[1]							\n"
	    "	fmla	v28.4s, v7.4s, v1.s[2]							\n"
	    "	fmla	v31.4s, v7.4s, v1.s[3]							\n"

	    ".endm 														\n"


	    ".macro	M4N12_PACK_ADD_C 									\n"

	    "	prfm	PLDL1KEEP, [x25, #64]							\n"
	    "	prfm 	PLDL1KEEP, [x26, #64]							\n"

	    "	ldp		q0, q1, [x25] 									\n"
	    "	fadd 	v20.4s, v20.4s, v0.4s 							\n"
	    "	prfm 	PLDL1KEEP, [x27, #64]							\n"
	    "	ldr  	q2, [x25, #32]   								\n"
	    "	fadd 	v21.4s, v21.4s, v1.4s 							\n"
	    "	ldp		q3, q4, [x26] 									\n"
	    "	prfm 	PLDL1KEEP, [x28, #64]							\n"
	    "	fadd 	v22.4s, v22.4s, v2.4s 							\n"

	    "	ldr 	q5, [x26, #32] 									\n"
	    "	fadd 	v23.4s, v23.4s, v3.4s 							\n"
	    "	ldp		q6, q7, [x27] 									\n"
	    "	fadd 	v24.4s, v24.4s, v4.4s 							\n"
	    "	ldr 	q8, [x27, #32] 									\n"

	    "	fadd 	v25.4s, v25.4s, v5.4s 							\n"
	    "	ldp		q0, q1, [x28] 									\n"
	    "	fadd 	v26.4s, v26.4s, v6.4s 							\n"
	    "	ldr		q2, [x28, #32] 									\n"
	    "	fadd 	v27.4s, v27.4s, v7.4s 							\n"
	    "	fadd 	v28.4s, v28.4s, v8.4s 							\n"

	    "	fadd 	v29.4s, v29.4s, v0.4s 							\n"
	    "	fadd 	v30.4s, v30.4s, v1.4s 							\n"
	    "	fadd 	v31.4s, v31.4s, v2.4s 							\n"

	    ".endm 														\n"


	    ".macro	SAVE4x12 											\n"

	    "	stp 	q20, q21, [x25]									\n"
	    "	str		q22, [x25, #32]									\n"
	    "	add  	x25, x25, x9, lsl #4 							\n"

	    "	stp 	q23, q24, [x26]									\n"
	    "	str		q25, [x26, #32]									\n"
	    "	add  	x26, x26, x9, lsl #4 							\n"

	    "	stp 	q26, q27, [x27]									\n"
	    "	str		q28, [x27, #32]									\n"
	    "	add  	x27, x27, x9, lsl #4 							\n"

	    "	stp 	q29, q30, [x28]									\n"
	    "	str		q31, [x28, #32]									\n"
	    "	add  	x28, x28, x9, lsl #4 							\n"

	    ".endm 														\n"



	    ".macro	KERNEL3x12_BEGIN_K 									\n"

	    "	ld3		{v0.s, v1.s, v2.s}[0], [x11], #12				\n"
	    "	prfm	PLDL1KEEP, [x11, #512]							\n"
	    "	ldr		q3, [x24], #16									\n"

	    "	fmul	v23.4s, v3.4s, v0.s[0]							\n"
	    "	ldr		q4, [x24], #16									\n"
	    "	fmul	v26.4s, v3.4s, v1.s[0] 							\n"
	    "	ldr		q5, [x24], #16									\n"
	    "	fmul	v29.4s, v3.4s, v2.s[0]							\n"

	    "	prfm	PLDL1KEEP, [x24, #256]							\n"

	    "	ld3		{v6.s, v7.s, v8.s}[0], [x11], #12				\n"
	    "	fmul	v24.4s, v4.4s, v0.s[0]							\n"
	    "	fmul	v27.4s, v4.4s, v1.s[0]							\n"
	    "	ldp		q9, q10, [x24], #32								\n"
	    "	fmul	v30.4s, v4.4s, v2.s[0]							\n"

	    "	fmul	v25.4s, v5.4s, v0.s[0]							\n"
	    "	ldr 	q11, [x24], #16 								\n"
	    "	fmul	v28.4s, v5.4s, v1.s[0]							\n"
	    "	fmul	v31.4s, v5.4s, v2.s[0]							\n"

	    ".endm 														\n"




	    ".macro	KERNEL3x12_K0 										\n"

	    "	ld3		{v6.s, v7.s, v8.s}[0], [x11], #12				\n"

	    "	fmla	v23.4s, v3.4s, v0.s[0]							\n"
	    "	fmla	v24.4s, v4.4s, v0.s[0]							\n"
	    "	ldp		q9, q10, [x24], #32								\n"
	    "	fmla	v25.4s, v5.4s, v0.s[0]							\n"

	    "	fmla	v26.4s, v3.4s, v1.s[0] 							\n"
	    "	fmla	v27.4s, v4.4s, v1.s[0]							\n"
	    "	fmla	v28.4s, v5.4s, v1.s[0]							\n"
	    "	ldr 	q11, [x24], #16 								\n"

	    "	fmla	v29.4s, v3.4s, v2.s[0]							\n"
	    "	fmla	v30.4s, v4.4s, v2.s[0]							\n"
	    "	fmla	v31.4s, v5.4s, v2.s[0]							\n"

	    "	prfm	PLDL1KEEP, [x24, #256]							\n"

	    ".endm 														\n"



	    ".macro	KERNEL3x12_K1 										\n"

	    "	ld3		{v0.s, v1.s, v2.s}[0], [x11], #12				\n"

	    "	fmla	v23.4s, v9.4s, v6.s[0]							\n"
	    "	fmla	v24.4s, v10.4s, v6.s[0]							\n"
	    "	ldp		q3, q4, [x24], #32								\n"
	    "	fmla	v25.4s, v11.4s, v6.s[0]							\n"

	    "	prfm	PLDL1KEEP, [x11, #256]							\n"
	    "	fmla	v26.4s, v9.4s, v7.s[0] 							\n"
	    "	fmla	v27.4s, v10.4s, v7.s[0]							\n"
	    "	fmla	v28.4s, v11.4s, v7.s[0]							\n"
	    "	ldr 	q5, [x24], #16 									\n"

	    "	fmla	v29.4s, v9.4s, v8.s[0]							\n"
	    "	fmla	v30.4s, v10.4s, v8.s[0]							\n"
	    "	fmla	v31.4s, v11.4s, v8.s[0]							\n"

	    "	prfm	PLDL1KEEP, [x24, #256]							\n"

	    ".endm 														\n"


	    ".macro	KERNEL3x12_END_K 									\n"

	    "	fmla	v23.4s, v9.4s, v6.s[0]							\n"
	    "	fmla	v24.4s, v10.4s, v6.s[0]							\n"
	    "	fmla	v25.4s, v11.4s, v6.s[0]							\n"

	    "	fmla	v26.4s, v9.4s, v7.s[0] 							\n"
	    "	fmla	v27.4s, v10.4s, v7.s[0]							\n"
	    "	fmla	v28.4s, v11.4s, v7.s[0]							\n"

	    "	fmla	v29.4s, v9.4s, v8.s[0]							\n"
	    "	fmla	v30.4s, v10.4s, v8.s[0]							\n"
	    "	fmla	v31.4s, v11.4s, v8.s[0]							\n"

	    ".endm 														\n"



	    ".macro	M3N12_PACK_ADD_C									\n"


	    "	prfm	PLDL1KEEP, [x25, #64]							\n"
	    "	prfm 	PLDL1KEEP, [x26, #64]							\n"

	    "	ldp		q0, q1, [x25] 									\n"
	    "	fadd 	v23.4s, v23.4s, v0.4s 							\n"
	    "	prfm 	PLDL1KEEP, [x27, #64]							\n"
	    "	ldr  	q2, [x25, #32]   								\n"
	    "	fadd 	v24.4s, v24.4s, v1.4s 							\n"
	    "	ldp		q3, q4, [x26] 									\n"
	    "	fadd 	v25.4s, v25.4s, v2.4s 							\n"

	    "	ldr 	q5, [x26, #32] 									\n"
	    "	fadd 	v26.4s, v26.4s, v3.4s 							\n"
	    "	ldp		q6, q7, [x27] 									\n"
	    "	fadd 	v27.4s, v27.4s, v4.4s 							\n"
	    "	ldr 	q8, [x27, #32] 									\n"

	    "	fadd 	v28.4s, v28.4s, v5.4s 							\n"
	    "	fadd 	v29.4s, v29.4s, v6.4s 							\n"
	    "	fadd 	v30.4s, v30.4s, v7.4s 							\n"
	    "	fadd 	v31.4s, v31.4s, v8.4s 							\n"

	    ".endm 														\n"



	    ".macro	SAVE3x12 											\n"

	    "	stp 	q23, q24, [x25]									\n"
	    "	str		q25, [x25, #32]									\n"

	    "	stp 	q26, q27, [x26]									\n"
	    "	str		q28, [x26, #32]									\n"

	    "	stp 	q29, q30, [x27]									\n"
	    "	str		q31, [x27, #32]									\n"

	    ".endm 														\n"



	    ".macro	KERNEL2x12_BEGIN_K 									\n"

	    "	ld2		{v0.s, v1.s}[0], [x11], #8						\n"
	    "	prfm	PLDL1KEEP, [x11, #256]							\n"
	    "	ldr		q2, [x24], #16									\n"

	    "	fmul	v26.4s, v2.4s, v0.s[0]							\n"
	    "	ldr		q3, [x24], #16									\n"
	    "	fmul	v29.4s, v2.4s, v1.s[0] 							\n"
	    "	ldr		q4, [x24], #16									\n"

	    "	prfm	PLDL1KEEP, [x24, #256]							\n"

	    "	ld2		{v5.s, v6.s}[0], [x11], #8						\n"
	    "	fmul	v27.4s, v3.4s, v0.s[0]							\n"
	    "	fmul	v30.4s, v3.4s, v1.s[0]							\n"
	    "	ldp		q7, q8, [x24], #32								\n"

	    "	fmul	v28.4s, v4.4s, v0.s[0]							\n"
	    "	ldr 	q9, [x24], #16 									\n"
	    "	fmul	v31.4s, v4.4s, v1.s[0]							\n"

	    ".endm 														\n"



	    ".macro	KERNEL2x12_K0 										\n"


	    "	ld2		{v5.s, v6.s}[0], [x11], #8						\n"
	    "	fmla	v26.4s, v2.4s, v0.s[0] 							\n"
	    "	fmla	v27.4s, v3.4s, v0.s[0]							\n"
	    "	fmla	v28.4s, v4.4s, v0.s[0]							\n"

	    "	ldp		q7, q8, [x24], #32								\n"
	    "	fmla	v29.4s, v2.4s, v1.s[0]							\n"
	    "	fmla	v30.4s, v3.4s, v1.s[0]							\n"
	    "	ldr 	q9, [x24], #16 									\n"
	    "	fmla	v31.4s, v4.4s, v1.s[0]							\n"

	    "	prfm	PLDL1KEEP, [x24, #128]							\n"

	    ".endm 														\n"



	    ".macro	KERNEL2x12_K1 										\n"

	    "	ld2		{v0.s, v1.s}[0], [x11], #8						\n"

	    "	fmla	v26.4s, v7.4s, v5.s[0]							\n"
	    "	fmla	v27.4s, v8.4s, v5.s[0]							\n"
	    "	ldp		q2, q3, [x24], #32								\n"
	    "	fmla	v28.4s, v9.4s, v5.s[0]							\n"

	    "	prfm	PLDL1KEEP, [x11, #256]							\n"
	    "	fmla	v29.4s, v7.4s, v6.s[0] 							\n"
	    "	ldr 	q4, [x24], #16 									\n"
	    "	fmla	v30.4s, v8.4s, v6.s[0]							\n"
	    "	fmla	v31.4s, v9.4s, v6.s[0]							\n"

	    "	prfm	PLDL1KEEP, [x24, #128]							\n"

	    ".endm 														\n"


	    ".macro	KERNEL2x12_END_K 									\n"


	    "	fmla	v26.4s, v7.4s, v5.s[0]							\n"
	    "	fmla	v27.4s, v8.4s, v5.s[0]							\n"
	    "	fmla	v28.4s, v9.4s, v5.s[0]							\n"

	    "	fmla	v29.4s, v7.4s, v6.s[0] 							\n"
	    "	fmla	v30.4s, v8.4s, v6.s[0]							\n"
	    "	fmla	v31.4s, v9.4s, v6.s[0]							\n"

	    "	prfm	PLDL1KEEP, [x24, #128]							\n"

	    ".endm 														\n"



	    ".macro	M2N12_PACK_ADD_C									\n"


	    "	prfm	PLDL1KEEP, [x25, #64]							\n"
	    "	prfm 	PLDL1KEEP, [x26, #64]							\n"

	    "	ldp		q0, q1, [x25] 									\n"
	    "	fadd 	v26.4s, v26.4s, v0.4s 							\n"
	    "	prfm 	PLDL1KEEP, [x27, #64]							\n"
	    "	ldr  	q2, [x25, #32]   								\n"
	    "	fadd 	v27.4s, v27.4s, v1.4s 							\n"
	    "	ldp		q3, q4, [x26] 									\n"
	    "	fadd 	v28.4s, v28.4s, v2.4s 							\n"

	    "	ldr 	q5, [x26, #32] 									\n"
	    "	fadd 	v29.4s, v29.4s, v3.4s 							\n"
	    "	fadd 	v30.4s, v30.4s, v4.4s 							\n"
	    "	fadd 	v31.4s, v31.4s, v5.4s 							\n"

	    ".endm 														\n"



	    ".macro	SAVE2x12 											\n"

	    "	stp 	q26, q27, [x25]									\n"
	    "	str		q28, [x25, #32]									\n"

	    "	stp 	q29, q30, [x26]									\n"
	    "	str		q31, [x26, #32]									\n"

	    ".endm 														\n"





//-------------------------------------------------------------------------


	    "SMM_NN:									 	 			\n"

	    "	ldr		x0, %[C]						 				\n"
	    "	ldr		x1, %[A]						 				\n"
	    "	ldr		x2, %[B]						 				\n"

	    "	ldr		x3, %[M]						 				\n"
	    "	ldr		x4, %[N]						 				\n"
	    "	ldr		x5, %[K]						 				\n"
	    "	ldr		x9, %[LN] 						 				\n"
	    "	ldr		x6, %[LK]						 				\n"

	    "	prfm	PLDL1KEEP, [x1, #512]            				\n"
	    "	prfm    PLDL1KEEP, [x2, #64]            				\n"


	    "	ldr		x7, %[m_tag] 					 				\n"
	    "	ldr		x8, %[k_tag] 					 				\n"
	    "	ldr		x30, %[id_tag] 					 				\n"
	    "	ldr 	x29, %[stride_Bc] 								\n"


	    "	mov		x21, #12 										\n"
	    "	udiv	x20, x4, x21						 			\n"          // N / 12


//-----------------------------------------------------------------------------

	    "	cmp		x7, #0 											\n"
	    "	bne 	NN8x12_addr 									\n"
	    "	ldr		x10, %[SB]						 				\n"
	    "	cmp		x30, #0 										\n"
	    "	beq		BEGIN_N12 										\n"
	    "NN8x12_addr:												\n"
	    "	ldr 	x10, %[Bc]										\n"


//-----------------------------------------------------------------------------LOOP

	    "BEGIN_N12:								 	 				\n"

	    "	mov 	x25, x0   						 				\n"   //C1*
	    "	prfm	PLDL2KEEP, [x25, #64]							\n"
	    "	add		x26, x25, x9, lsl #2 			 				\n"   //C2*
	    "	prfm	PLDL2KEEP, [x26, #64]							\n"
	    "	add 	x27, x25, x9, lsl #3 			 				\n"   //C3*
	    "	prfm	PLDL2KEEP, [x27, #64]							\n"
	    "	add 	x28, x26, x9, lsl #3 			 				\n"   //C4*
	    "	prfm	PLDL2KEEP, [x28, #64]							\n"

	    "	add 	x15, x27, x9, lsl #3 							\n"	  //C5*
	    "	prfm	PLDL2KEEP, [x15, #64]							\n"
	    "	add 	x16, x28, x9, lsl #3 							\n"	  //C6*
	    "	prfm	PLDL2KEEP, [x16, #64]							\n"
	    "	add 	x17, x15, x9, lsl #3 							\n"	  //C7*
	    "	prfm	PLDL2KEEP, [x17, #64]							\n"
	    "	add 	x18, x16, x9, lsl #3 							\n"	  //C8*
	    "	prfm	PLDL2KEEP, [x18, #64]							\n"

	    "	mov		x11, x1   						 				\n"   // address_A
	    "	lsr		x21, x3, #3						 				\n"   // M / 8

	    "	cmp	 	x7, #0 											\n"   //import
	    "	bne 	BEGIN_M8 										\n"

	    "	cmp		x4, #12 										\n"
	    "	beq 	BEGIN_PACKB 									\n"

	    "	ands 	x22, x20, #1 									\n"  // x20 % 2
	    "	bne 	BEGIN_M8 										\n"

//---------------------------------------------------------------------------PACK

	    "BEGIN_PACKB:								 				\n"

	    "	mov		x24, x10						 				\n"   //还原SB的地址
	    "	add 	x23, x24, x29 									\n"   // SB + stride

	    "	mov		x12, x2						 	 				\n"   // B0*
	    "	add		x13, x12, x9, lsl #2 			 				\n"   // B0 + sizeof(float) * LN

	    "	prfm    PLDL1KEEP, [x12, #64]			 				\n"
	    "	prfm    PLDL1KEEP, [x13, #64]			 				\n"
	    "	prfm    PLDL1KEEP, [x11, #256]			 				\n"



//---------------------------------------------------------------------------
	    "	lsr		x22, x5, #2 					 				\n"     // K / 4

	    "	cmp		x4, #12 										\n"
	    "	bne 	PACK_Body_K 									\n"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


	    "PACK_N12_Body_K:								 			\n"

	    "	PACK_KERNEL8x12_N12_BEGIN_K				 	 			\n"
	    "	subs	x22, x22, #1		     		 				\n"
	    "	b 		PACK_N12_K1_7						 			\n"

	    "PACK_N12_K:									 			\n"

	    "	PACK_KERNEL8x12_N12_K0						 			\n"
	    "PACK_N12_K1_7:									 			\n"

	    "	PACK_KERNEL8x12_N12_K1						 			\n"
	    "	PACK_KERNEL8x12_N12_K0						 			\n"

	    "	beq		PACK_N12_Edge_K						 			\n"

	    "	PACK_KERNEL8x12_N12_K1						 			\n"

	    "	subs	x22, x22, #1			 		 				\n"
	    "	b 		PACK_N12_K			 				 			\n"

	    "PACK_N12_Edge_K:							     			\n"

	    "	PACK_KERNEL8x12_N12_END_K			         			\n"

	    "	b 		BEGIN_PACK_SAVE 								\n"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



	    "PACK_Body_K:								 				\n"

	    "	PACK_KERNEL8x12_BEGIN_K				 	 				\n"
	    "	subs	x22, x22, #1		     		 				\n"
	    "	b 		PACK_K1_7						 				\n"

	    "PACK_K:									 				\n"

	    "	PACK_KERNEL8x12_K0						 				\n"
	    "PACK_K1_7:									 				\n"

	    "	PACK_KERNEL8x12_K1						 				\n"
	    "	PACK_KERNEL8x12_K0						 				\n"

	    "	beq		PACK_Edge_K						 				\n"

	    "	PACK_KERNEL8x12_K1						 				\n"

	    "	subs	x22, x22, #1			 		 				\n"
	    "	b 		PACK_K			 				 				\n"

	    "PACK_Edge_K:							     				\n"

	    "	PACK_KERNEL8x12_END_K			         				\n"

//---------------------------------------------------------------------------


	    "PACK_EDGE_RIGHT_K:											\n"

	    "	ands 	x22, x5, #3 									\n"
	    "	beq		BEGIN_PACK_SAVE									\n"

	    "PACK_RIGHT_LOOP_K:											\n"
	    "	subs 	x22, x22, #1 									\n"
	    "	PACK_KERNEL8x12_RIGHT_K									\n"
	    "	bgt 	PACK_RIGHT_LOOP_K								\n"


//------------------------------------------------------------------------

	    "BEGIN_PACK_SAVE:											\n"

	    "	cmp		x8, #0							 				\n"
	    "	beq		M8N12_PACK_SAVE 					 			\n"
	    "	M8N12_PACK_ADD_C										\n"
	    "M8N12_PACK_SAVE: 							 				\n"
	    "	SAVE8x12								 				\n"


	    "	beq		M8_END							 				\n"


//--------------------------------------------------------------------------
	    "BEGIN_M8:                          		 				\n"

	    " 	mov     x24, x10                    		 			\n"   //  address_B, x11 is address_A + offset

	    "Body_K:                            		 				\n"

	    " 	prfm    PLDL1KEEP, [x24, #128]    		 				\n"
	    " 	prfm    PLDL1KEEP, [x11, #256]    		 				\n"

	    " 	lsr   x22, x5, #2                 		 				\n"     // K / 8
	    " 	KERNEL8x12_BEGIN_K                		 	 			\n"
	    " 	subs  x22, x22, #1                		 				\n"
	    " 	b 	K1_7                            	 				\n"

	    "Main_K:                            		 				\n"

	    " 	KERNEL8x12_K0                     						\n"
	    "K1_7:                              						\n"

	    " 	KERNEL8x12_K1                     		 	 			\n"
	    " 	KERNEL8x12_K2                     		 	 			\n"

	    "	beq   Edge_K                      		 				\n"

	    " 	KERNEL8x12_K3                     		 	 			\n"

	    " 	subs  x22, x22, #1                		 				\n"
	    " 	b     Main_K                      		 				\n"

	    "Edge_K:                            		 				\n"

	    " 	KERNEL8x12_END_K                 			 			\n"

	    "	ands x22, x5, #3 										\n"
	    "	beq		BEGIN_SAVE8x12									\n"

	    "RIGHT_LOOP_K:												\n"
	    "	subs 	x22, x22, #1 									\n"
	    "	KERNEL8x12_RIGHT_K 										\n"
	    "	bgt 	RIGHT_LOOP_K									\n"

	    "BEGIN_SAVE8x12:											\n"

	    "	cmp		x8, #0							 				\n"
	    "	beq		M8N12_SAVE 					 	 				\n"
	    "	M8N12_PACK_ADD_C 							 			\n"
	    "M8N12_SAVE: 							     				\n"
	    "	SAVE8x12								 	 			\n"

	    " bgt     BEGIN_M8                							\n"


	    "	ands	x21, x3, #7 									\n"
	    "	beq		M8_END 											\n"


//------------------------------------------------------------------------
	    " 	prfm    PLDL1KEEP, [x11, #256]    		 				\n"
	    "BEGIN_M4:                          		 				\n"

	    "	cmp		x21, #4 										\n"
	    "	blt		BEGIN_M3 										\n"

	    " 	mov     x24, x10                    		 			\n"   //  address_B, x11 is address_A + offset
	    " 	prfm    PLDL1KEEP, [x24, #128]    		 				\n"

	    "Body_K_M4:                            		 				\n"

	    " lsr   x22, x5, #2                 		 				\n"     // K / 4
	    " KERNEL4x12_BEGIN_K                		 	 			\n"
	    " subs  x22, x22, #1                		 				\n"
	    " b 	K1_3_M4                            	 				\n"

	    "Main_K_M4:                            		 				\n"

	    " KERNEL4x12_K0                     						\n"
	    "K1_3_M4:                              						\n"

	    " KERNEL4x12_K1                     		 	 			\n"
	    " KERNEL4x12_K0                     		 	 			\n"

	    " beq   Edge_K_M4                      		 				\n"

	    " KERNEL4x12_K1                     		 	 			\n"

	    " subs  x22, x22, #1                		 				\n"
	    " b     Main_K_M4                      		 				\n"

	    "Edge_K_M4:                            		 				\n"
	    " KERNEL4x12_END_K                 			 				\n"

	    "BEGIN_SAVE4x12:											\n"

	    "	cmp		x8, #0							 				\n"
	    "	beq		M4N12_SAVE 					 	 				\n"
	    "	M4N12_PACK_ADD_C 							 			\n"
	    "M4N12_SAVE: 							     				\n"
	    "	SAVE4x12								 	 			\n"

	    "	subs	x21, x21, #4 									\n"

//----------------------------------------------------------------------------
	    "BEGIN_M3:                          		 				\n"

	    "	cmp		x21, #3 										\n"
	    "	blt		BEGIN_M2 										\n"

	    " 	mov     x24, x10                    		 			\n"   //  address_B, x11 is address_A + offset
	    " 	prfm    PLDL1KEEP, [x24, #128]    		 				\n"

	    "Body_K_M3:                            		 				\n"

	    " lsr   x22, x5, #2                 		 				\n"     // K / 4
	    " KERNEL3x12_BEGIN_K                		 	 			\n"
	    " subs  x22, x22, #1                		 				\n"
	    " b 	K1_3_M3                            	 				\n"

	    "Main_K_M3:                            		 				\n"

	    " KERNEL3x12_K0                     						\n"
	    "K1_3_M3:                              						\n"

	    " KERNEL3x12_K1                     		 	 			\n"
	    " KERNEL3x12_K0                     		 	 			\n"

	    " beq   Edge_K_M3                      		 				\n"

	    " KERNEL3x12_K1                     		 	 			\n"

	    " subs  x22, x22, #1                		 				\n"
	    " b     Main_K_M3                      		 				\n"

	    "Edge_K_M3:                            		 				\n"

	    " KERNEL3x12_END_K                 			 				\n"

	    "BEGIN_SAVE3x12:											\n"

	    "	cmp		x8, #0							 				\n"
	    "	beq		M3N12_SAVE 					 	 				\n"
	    "	M3N12_PACK_ADD_C 							 			\n"
	    "M3N12_SAVE: 							     				\n"
	    "	SAVE3x12								 	 			\n"

	    "	subs 	x21, x21, #3 									\n"


//--------------------------------------------------------------------------
	    "BEGIN_M2:                          		 				\n"

	    "	cmp		x21, #2 										\n"
	    "	blt		M8_END 											\n"

	    "	mov     x24, x10                    					\n"   //  address_B, x11 is address_A + offset
	    "	prfm    PLDL1KEEP, [x24, #128]    		 				\n"

	    "Body_K_M2:                            						\n"

	    " lsr   x22, x5, #2                 		 				\n"     // K / 4
	    " KERNEL2x12_BEGIN_K                		 				\n"
	    " subs  x22, x22, #1                		 				\n"
	    " b 	K1_3_M2                            					\n"

	    "Main_K_M2:                            						\n"

	    " KERNEL2x12_K0                     						\n"
	    "K1_3_M2:                              						\n"

	    " KERNEL2x12_K1                     		 				\n"
	    " KERNEL2x12_K0                     		 				\n"

	    " beq   Edge_K_M2                      						\n"

	    " KERNEL2x12_K1                     		 				\n"

	    " subs  x22, x22, #1                		 				\n"
	    " b     Main_K_M2                      						\n"

	    "Edge_K_M2:                            						\n"

	    " KERNEL2x12_END_K                 			 				\n"

	    "BEGIN_SAVE2x12:											\n"

	    "	cmp		x8, #0							 				\n"
	    "	beq		M2N12_SAVE 					 	 				\n"
	    "	M2N12_PACK_ADD_C 										\n"
	    "M2N12_SAVE: 							     				\n"
	    "	SAVE2x12								 				\n"


	    "M8_END:									 				\n"
//------------------------------------------------------------------------------------

	    "	cmp 	x7, #0 											\n"
	    "	bne 	NN8x12_M_CASE1 									\n"
	    "	cmp		x30, #0 										\n"
	    "	bne 	NN8x12_M_CASE1 									\n"
	    "	ands 	x22, x20, #1 									\n"  // x20 % 2
	    "	beq 	NN8x12_M_CASE1 									\n"
	    "	sub 	x24, x24, x29, lsl #1 							\n"   // SB + stride

	    "NN8x12_M_CASE1:											\n"
	    "	mov 	x10, x24 										\n"
	    "NN8x12_N: 													\n"

//----------------------------------------------------------------------------------


	    "	subs	x20, x20, #1					 				\n"
	    "	add		x0, x0, #48						 				\n"
	    "	add   	x2, x2, #48  									\n"  // address_B + offset
	    "	bgt		BEGIN_N12						 				\n"

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
	    [Bc] "m" (Bc),
	    [SB] "m" (SB),
	    [stride_Bc] "m" (stride_Bc),
	    [k_tag] "m" (k_tag),
	    [m_tag] "m" (m_tag),
	    [id_tag] "m" (id_tag)
	    : "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
	    "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16",
	    "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24", "x25",
	    "x26", "x27", "x28", "x29", "x30",
	    "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
	    "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
	    "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
	    "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"
	);

//printf("N = %d \n", N);

}