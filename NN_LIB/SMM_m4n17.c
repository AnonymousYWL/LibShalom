#include <stdio.h>
#include "LibShalom.h"

void SGEMM_NT_kernel_exist_1(float *C, float *A, float *B, long	M, long N, long K, 
			 	long LN, long LK, float *SB, long k_tag)
{

	asm volatile(


				".macro PACK_KERNEL4x5_BEGIN_K      \n"
				"										 	 							\n"

				"	ldr		q0, [x11], #16					 		\n"
				"	ldr		q1, [x12], #16				     	\n"
				"	ldr		q4, [x15], #16					 		\n"

				" prfm		PLDL1KEEP, [x11, #64]   	\n"
				"	prfm		PLDL1KEEP, [x12, #64] 		\n"
				" prfm 		PLDL1KEEP, [x13, #64]   	\n"
				"	prfm		PLDL1KEEP, [x14, #64] 		\n"
				"	prfm		PLDL1KEEP, [x15, #64]			\n"

				"	prfm		PLDL1KEEP, [x16, #128]		\n"

				"	fmul	v12.4s, v0.4s, v4.4s			 	\n"
				"	ldr		q2, [x13], #16					 		\n"
				"	fmul	v13.4s, v1.4s, v4.4s			 	\n"

				"	ldr		q3, [x14], #16					 		\n"
				"	prfm		PLDL1KEEP, [x17, #128]		\n"

				"	fmul	v14.4s, v2.4s, v4.4s		 	 	\n"
				"	ldr		q5, [x16], #16					 		\n"
				"	fmul	v15.4s, v3.4s, v4.4s			 	\n"

				"	ldr		q6, [x17], #16					 		\n"

				"	fmul	v16.4s,	v0.4s, v5.4s 				\n"
				"	ldr		q7, [x18], #16					 		\n"
				"	fmul	v17.4s,	v1.4s, v5.4s 				\n"
				"	ldr		q8, [x19], #16					 		\n"

				"	prfm		PLDL1KEEP, [x18, #128]		\n"

				"	fmul	v18.4s,	v2.4s, v5.4s 				\n"
				"	fmul	v19.4s,	v3.4s, v5.4s 				\n"

				"	st4		{v4.s, v5.s, v6.s, v7.s}[0], [x24]		\n"
				"	add		x24, x24, #16			 					\n"

				"	ldr		q9, [x11], #16 							\n"

				"	fmul	v20.4s,	v0.4s, v6.4s 				\n"
				"	fmul	v21.4s,	v1.4s, v6.4s 				\n"
				"	ldr		q10, [x12], #16 						\n"

				"	str 	q8, [x24], #64 							\n"
				"	prfm		PLDL1KEEP, [x19, #128]		\n"

				"	fmul	v22.4s,	v2.4s, v6.4s 				\n"
				"	fmul	v23.4s,	v3.4s, v6.4s 				\n"
				"	ldr		q11, [x13], #16 						\n"

				"	st4		{v4.s, v5.s, v6.s, v7.s}[1], [x24]		\n"
				"	add 	x24, x24, #64 							\n"
				
				"	fmul	v24.4s,	v0.4s, v7.4s 				\n"
				"	fmul	v25.4s,	v1.4s, v7.4s 				\n"

				"	st4		{v4.s, v5.s, v6.s, v7.s}[2], [x24]		\n"
				"	add 	x24, x24, #64 							\n"

				"	fmul	v26.4s,	v2.4s, v7.4s 				\n"
				"	fmul	v27.4s,	v3.4s, v7.4s 				\n"

				"	st4		{v4.s, v5.s, v6.s, v7.s}[3], [x24]		\n"
				"	add 	x24, x24, #64 								\n"

				"	ldr 		q4, [x15], #16 							\n"
				"	fmul		v28.4s,	v0.4s, v8.4s 				\n"
				"	ldr 		q0, [x14], #16 							\n"
				"	fmul		v29.4s,	v1.4s, v8.4s 				\n"
				"	fmul		v30.4s,	v2.4s, v8.4s 				\n"
				"	fmul		v31.4s,	v3.4s, v8.4s 				\n"


				".endm									 	 						\n"



				".macro PACK_KERNEL4x5_K0      				\n"

				" prfm		PLDL1KEEP, [x15, #128]   		\n"
				"	prfm		PLDL1KEEP, [x16, #128] 			\n"

				"	ldr			q5, [x16], #16 							\n"

				"	fmla		v12.4s, v0.4s, v4.4s 				\n"
				"	fmla		v13.4s, v1.4s, v4.4s 				\n"
				"	ldr			q6, [x17], #16 							\n"
				"	fmla 		v14.4s, v2.4s, v4.4s 				\n"
				"	fmla 		v15.4s, v3.4s, v4.4s 				\n"

				"	ldr			q7, [x18], #16 							\n"
				" prfm		PLDL1KEEP, [x17, #128]   		\n"
				"	prfm		PLDL1KEEP, [x18, #128] 			\n"

				"	fmla 		v16.4s, v0.4s, v5.4s 				\n"
				"	fmla 		v17.4s, v1.4s, v5.4s 				\n"
				"	ldr			q8, [x19], #16 							\n"
				"	st4			{v4.s, v5.s, v6.s, v7.s}[0], [x24]		\n"
				"	add 		x24, x24, #16 							\n"
				"	fmla 		v18.4s, v2.4s, v5.4s 				\n"
				"	fmla 		v19.4s, v3.4s, v5.4s 				\n"

				"	prfm		PLDL1KEEP, [x19, #128] 			\n"

				"	ldr 		q9, [x11], #16         			\n"
				"	str 		q8, [x24], #64 							\n"		
				"	fmla		v20.4s, v0.4s, v6.4s 				\n"
				"	fmla 		v21.4s, v1.4s, v6.4s 				\n"
				"	st4			{v4.s, v5.s, v6.s, v7.s}[1], [x24]		\n"
				"	add 		x24, x24, #64 							\n"						
				"	fmla 		v22.4s, v2.4s, v6.4s 				\n"
				"	fmla 		v23.4s, v3.4s, v6.4s 				\n"

				"	ldr			q10, [x12], #16         	  \n"

				"	st4			{v4.s, v5.s, v6.s, v7.s}[2], [x24]		\n"
				"	add 		x24, x24, #64 							\n"
				"	fmla 		v24.4s, v0.4s, v7.4s 				\n"
				"	fmla 		v25.4s, v1.4s, v7.4s 				\n"
				"	ldr			q11, [x13], #16         	  \n"
				"	st4			{v4.s, v5.s, v6.s, v7.s}[3], [x24]		\n"
				"	add 		x24, x24, #64 							\n"
				"	ldr 		q4, [x15], #16 							\n"

				"	fmla 		v28.4s, v0.4s, v8.4s 				\n"
				"	ldr 		q0, [x14], #16 							\n"
				"	fmla		v26.4s, v2.4s, v7.4s 				\n"
				"	fmla 		v27.4s, v3.4s, v7.4s 				\n"
				"	fmla 		v29.4s, v1.4s, v8.4s 				\n"
				"	fmla 		v30.4s, v2.4s, v8.4s 				\n"
				"	fmla 		v31.4s, v3.4s, v8.4s 				\n"

				".endm 																\n"




				".macro PACK_KERNEL4x5_K1      				\n"


				" prfm		PLDL1KEEP, [x11, #128]   		\n"
				"	prfm		PLDL1KEEP, [x12, #128] 			\n"

				"	ldr			q5, [x16], #16 							\n"
				"	fmla		v12.4s, v9.4s, v4.4s 				\n"
				"	fmla		v13.4s, v10.4s, v4.4s 			\n"
				"	ldr			q6, [x17], #16 							\n"
				"	fmla 		v14.4s, v11.4s, v4.4s   		\n"
				"	fmla 		v15.4s, v0.4s, v4.4s   			\n"
				"	ldr			q7, [x18], #16 							\n"

				"	st4			{v4.s, v5.s, v6.s, v7.s}[0], [x24]		\n"
				"	add 		x24, x24, #16 							\n"

				" prfm		PLDL1KEEP, [x13, #128]   		\n"
				"	prfm		PLDL1KEEP, [x14, #128] 			\n"

				"	fmla		v16.4s, v9.4s, v5.4s 				\n"
				"	fmla		v17.4s, v10.4s, v5.4s 			\n"
				"	ldr 		q8, [x19], #16 							\n"
				"	fmla 		v18.4s, v11.4s, v5.4s   		\n"
				"	fmla 		v19.4s, v0.4s, v5.4s   			\n"
				"	str 		q8, [x24], #64 							\n"

				"	ldr 		q1, [x12], #16        			\n"
				"	fmla		v20.4s, v9.4s, v6.4s 				\n"
				"	fmla		v21.4s, v10.4s, v6.4s 				\n"
				"	ldr 		q2, [x13], #16        			\n"
				"	st4			{v4.s, v5.s, v6.s, v7.s}[1], [x24]		\n"
				"	add 		x24, x24, #64 							\n"
				"	fmla 		v22.4s, v11.4s, v6.4s   		\n"
				"	fmla 		v23.4s, v0.4s, v6.4s   			\n"

				"	ldr 		q3, [x14], #16        			\n"
				"	st4			{v4.s, v5.s, v6.s, v7.s}[2], [x24]		\n"
				"	add 		x24, x24, #64 							\n"
				"	fmla		v24.4s, v9.4s, v7.4s 				\n"
				"	fmla		v25.4s, v10.4s, v7.4s 				\n"
				"	st4			{v4.s, v5.s, v6.s, v7.s}[3], [x24]		\n"
				"	add 		x24, x24, #64 							\n"
				"	ldr 		q4, [x15], #16 							\n"

				"	fmla 		v27.4s, v0.4s, v7.4s   			\n"
				"	fmla		v31.4s, v0.4s, v8.4s 				\n"
				"	ldr 		q0, [x11], #16 							\n"

				"	fmla 		v26.4s, v11.4s, v7.4s   		\n"
				"	fmla		v28.4s, v9.4s, v8.4s 				\n"
				"	fmla		v29.4s, v10.4s, v8.4s 				\n"
				"	fmla		v30.4s, v11.4s, v8.4s 			\n"

				".endm 																\n"



				".macro PACK_KERNEL4x5_END_K      		\n"

				"	ldr			q5, [x16], #16 							\n"
				"	fmla		v12.4s, v9.4s, v4.4s 				\n"
				"	fmla		v13.4s, v10.4s, v4.4s 			\n"
				"	ldr			q6, [x17], #16 							\n"
				"	fmla 		v14.4s, v11.4s, v4.4s   		\n"
				"	fmla 		v15.4s, v0.4s, v4.4s   			\n"
				"	ldr			q7, [x18], #16 							\n"

				"	st4			{v4.s, v5.s, v6.s, v7.s}[0], [x24]		\n"
				"	add 		x24, x24, #16 							\n"

				"	fmla		v16.4s, v9.4s, v5.4s 				\n"
				"	fmla		v17.4s, v10.4s, v5.4s 			\n"
				"	ldr 		q8, [x19], #16 							\n"
				"	fmla 		v18.4s, v11.4s, v5.4s   		\n"
				"	fmla 		v19.4s, v0.4s, v5.4s   			\n"
				"	str 		q8, [x24], #64 							\n"


				"	fmla		v20.4s, v9.4s, v6.4s 				\n"
				"	fmla		v21.4s, v10.4s, v6.4s 				\n"
				"	st4			{v4.s, v5.s, v6.s, v7.s}[1], [x24]		\n"
				"	add 		x24, x24, #64 							\n"
				"	fmla 		v22.4s, v11.4s, v6.4s   		\n"
				"	fmla 		v23.4s, v0.4s, v6.4s   			\n"

				"	st4			{v4.s, v5.s, v6.s, v7.s}[2], [x24]		\n"
				"	add 		x24, x24, #64 							\n"
				"	fmla		v24.4s, v9.4s, v7.4s 				\n"
				"	fmla		v25.4s, v10.4s, v7.4s 				\n"
				"	st4			{v4.s, v5.s, v6.s, v7.s}[3], [x24]		\n"
				"	add 		x24, x24, #64 							\n"

				"	fmla 		v27.4s, v0.4s, v7.4s   			\n"
				"	fmla		v31.4s, v0.4s, v8.4s 				\n"
				"	fmla 		v26.4s, v11.4s, v7.4s   		\n"
				"	fmla		v28.4s, v9.4s, v8.4s 				\n"
				"	fmla		v29.4s, v10.4s, v8.4s 				\n"
				"	fmla		v30.4s, v11.4s, v8.4s 			\n"

				"	faddp		v12.4s, v12.4s, v16.4s 			\n"
				"	faddp		v13.4s, v13.4s, v17.4s 			\n"
				"	faddp		v14.4s, v14.4s, v18.4s 			\n"
				"	faddp		v15.4s, v15.4s, v19.4s 			\n"

				"	faddp		v20.4s, v20.4s, v24.4s 			\n"
				"	faddp		v21.4s, v21.4s, v25.4s 			\n"
				"	faddp		v22.4s, v22.4s, v26.4s 			\n"
				"	faddp		v23.4s, v23.4s, v27.4s 			\n"

				"	faddp		v16.4s, v28.4s, v29.4s 			\n"
				"	faddp		v17.4s, v30.4s, v31.4s 			\n"

				"	faddp		v12.4s, v12.4s, v20.4s 			\n"
				"	faddp		v13.4s, v13.4s, v21.4s 			\n"
				"	faddp		v14.4s, v14.4s, v22.4s 			\n"
				"	faddp		v15.4s, v15.4s, v23.4s 			\n"

				"	faddp		v16.4s, v16.4s, v17.4s 			\n"

				".endm 																\n"


				".macro	PACK4x5_ADD_C 						 		\n"

				"	ldr		q0, [x25], #16						 		\n"
				"	ldr		q1, [x26], #16						 		\n"

				"	fadd 	v12.4s, v12.4s, v0.4s			 		\n"
				"	ldr		q2, [x27], #16						 		\n"
				"	fadd 	v13.4s, v13.4s, v1.4s			 		\n"
				"	ldr		q3, [x28], #16						 		\n"
				"	fadd 	v14.4s, v14.4s, v2.4s			 		\n"
				"	ld1		{v4.s}[0], [x25]							\n"
				"	sub 	x25, x25, #16 								\n"
				"	ld1		{v4.s}[1], [x26]							\n"
				"	sub 	x26, x26, #16 								\n"
				"	ld1		{v4.s}[2], [x27]							\n"
				"	sub 	x27, x27, #16 								\n"
				"	ld1		{v4.s}[3], [x28]							\n"
				"	sub 	x28, x28, #16 								\n"
				"	fadd 	v15.4s, v15.4s, v3.4s			 		\n"
				"	fadd 	v16.4s, v16.4s, v4.4s			 		\n"


				".endm 									 	 						\n"



				".macro SAVE4x5							 	 				\n"

				"	str		q12, [x25], #16					 			\n"
				"	prfm	PLDL2KEEP, [x25, #64]			 		\n"
				"	str		q13, [x26], #16					 			\n"
				"	prfm	PLDL2KEEP, [x26, #64]			 		\n"
				"	str		q14, [x27], #16		 			 			\n"
				"	prfm	PLDL2KEEP, [x27, #64]			 		\n"
				"	str		q15, [x28], #16		 			 			\n"
				"	prfm	PLDL2KEEP, [x28, #64]			 		\n"
				"	st1		{v16.s}[0], [x25], #4					\n"
				"	st1		{v16.s}[1], [x26], #4					\n"

				"	st1		{v16.s}[2], [x27], #4					\n"
				"	st1		{v16.s}[3], [x28], #4					\n"

				".endm 									 	 						\n"



				".macro PACK_KERNEL4x4_BEGIN_K      	\n"
				"										 	 								\n"

				"	ldr		q0, [x11], #16					 			\n"
				"	ldr		q1, [x12], #16				     		\n"
				"	ldr		q4, [x15], #16					 			\n"

				" prfm		PLDL1KEEP, [x11, #128]   		\n"
				"	prfm		PLDL1KEEP, [x12, #128] 			\n"

				"	fmul	v16.4s, v0.4s, v4.4s 					\n"
				"	ldr		q2, [x13], #16					 			\n"
				"	fmul	v17.4s, v1.4s, v4.4s 					\n"
				"	ldr		q3, [x14], #16					 			\n"
				"	fmul	v18.4s, v2.4s, v4.4s 					\n"
				"	ldr		q5, [x16], #16					 			\n"
				"	fmul	v19.4s, v3.4s, v4.4s 					\n"

				"	ldr		q6, [x17], #16					 			\n"
				"	fmul	v20.4s, v0.4s, v5.4s 					\n"
				"	ldr		q7, [x18], #16					 			\n"
				"	fmul	v21.4s, v1.4s, v5.4s 					\n"
				"	ldr		q8, [x11], #16 								\n"
				"	fmul	v22.4s, v2.4s, v5.4s 					\n"
				"	ldr		q9, [x12], #16 								\n"
				"	fmul	v23.4s, v3.4s, v5.4s 					\n"
				"	st4		{v4.s, v5.s, v6.s, v7.s}[0], [x24]		\n"
				"	add 		x24, x24, #80 							\n"

				" prfm		PLDL1KEEP, [x13, #128]   		\n"
				"	prfm		PLDL1KEEP, [x14, #128] 			\n"

				"	fmul	v24.4s, v0.4s, v6.4s 					\n"
				"	ldr		q12, [x15], #16 							\n"
				"	fmul	v25.4s, v1.4s, v6.4s 					\n"
				"	st4		{v4.s, v5.s, v6.s, v7.s}[1], [x24]		\n"
				"	add 		x24, x24, #64 							\n"
				"	fmul	v26.4s, v2.4s, v6.4s 					\n"
				"	ldr		q10, [x13], #16 								\n"
				"	fmul	v27.4s, v3.4s, v6.4s 					\n"
				"	st4		{v4.s, v5.s, v6.s, v7.s}[2], [x24]		\n"
				"	add 		x24, x24, #64 							\n"

				"	fmul	v28.4s, v0.4s, v7.4s 					\n"
				"	ldr		q13, [x16], #16 							\n"
				"	fmul	v29.4s, v1.4s, v7.4s 					\n"
				"	st4		{v4.s, v5.s, v6.s, v7.s}[3], [x24]		\n"
				"	add 		x24, x24, #64 							\n"
				"	fmul	v30.4s, v2.4s, v7.4s 					\n"
				"	ldr		q14, [x17], #16 							\n"
				"	fmul	v31.4s, v3.4s, v7.4s 					\n"

				".endm 																\n"


				".macro PACK_KERNEL4x4_K0      				\n"

				"	prfm	PLDL1KEEP, [x11, #128] 				\n"

				"	ldr 	q3, [x14], #16 								\n"
				"	fmla	v16.4s, v0.4s, v4.4s 					\n"
				"	fmla	v17.4s, v1.4s, v4.4s 					\n"
				"	ldr 	q7, [x18], #16 								\n"
				"	fmla	v18.4s, v2.4s, v4.4s 					\n"

				"	st4		{v4.s, v5.s, v6.s, v7.s}[0], [x24]		\n"
				"	add 	x24, x24, #80 								\n"

				"	prfm	PLDL1KEEP, [x12, #128] 				\n"
				"	fmla	v20.4s, v0.4s, v5.4s 					\n"
				"	ldr 	q8, [x11], #16  							\n"
				"	fmla	v21.4s, v1.4s, v5.4s 					\n"
				"	ldr 	q9, [x12], #16  							\n"
				"	fmla	v22.4s, v2.4s, v5.4s 					\n"

				"	st4		{v4.s, v5.s, v6.s, v7.s}[1], [x24]		\n"
				"	add 	x24, x24, #64 								\n"

				"	prfm	PLDL1KEEP, [x13, #128] 				\n"
				"	fmla	v24.4s, v0.4s, v6.4s 					\n"
				"	ldr 	q12, [x15], #16  							\n"
				"	fmla	v25.4s, v1.4s, v6.4s 					\n"
				"	ldr 	q10, [x13], #16  							\n"
				"	fmla	v26.4s, v2.4s, v6.4s 					\n"

				"	ldr 	q13, [x16], #16  							\n"
				"	prfm	PLDL1KEEP, [x14, #128] 				\n"

				"	fmla	v19.4s, v3.4s, v4.4s 					\n"
				"	fmla	v23.4s, v3.4s, v5.4s 					\n"
				"	ldr 	q14, [x17], #16  							\n"
				"	fmla	v27.4s, v3.4s, v6.4s 					\n"

				"	st4		{v4.s, v5.s, v6.s, v7.s}[2], [x24]		\n"
				"	add 	x24, x24, #64 								\n"

				"	fmla	v28.4s, v0.4s, v7.4s 					\n"
				"	fmla	v29.4s, v1.4s, v7.4s 					\n"
				"	fmla	v30.4s, v2.4s, v7.4s 					\n"
				"	fmla	v31.4s, v3.4s, v7.4s 					\n"
				"	st4		{v4.s, v5.s, v6.s, v7.s}[3], [x24]		\n"
				"	add 	x24, x24, #64 								\n"



				".endm 																\n"



				".macro PACK_KERNEL4x4_K1      				\n"
				"										 	 								\n"

				" prfm		PLDL1KEEP, [x15, #128]   		\n"

				"	ldr 	q11, [x14], #16 							\n"
				"	fmla	v16.4s, v8.4s, v12.4s 				\n"
				"	fmla	v17.4s, v9.4s, v12.4s 				\n"
				"	fmla	v18.4s, v10.4s, v12.4s 				\n"

				"	prfm		PLDL1KEEP, [x16, #128] 			\n"
				"	ldr 	q15, [x18], #16 							\n"

				"	fmla	v20.4s, v8.4s, v13.4s 				\n"
				"	fmla	v21.4s, v9.4s, v13.4s 				\n"
				"	st4		{v12.s, v13.s, v14.s, v15.s}[0], [x24]		\n"
				"	add 		x24, x24, #80 							\n"
				"	fmla	v22.4s, v10.4s, v13.4s 				\n"

				"	prfm		PLDL1KEEP, [x17, #128] 			\n"
				"	ldr 	q0, [x11], #16  							\n"

				"	fmla	v24.4s, v8.4s, v14.4s 				\n"
				"	fmla	v25.4s, v9.4s, v14.4s 				\n"
				"	st4		{v12.s, v13.s, v14.s, v15.s}[1], [x24]		\n"
				"	add 		x24, x24, #64 							\n"
				"	fmla	v26.4s, v10.4s, v14.4s 				\n"

				"	prfm		PLDL1KEEP, [x18, #128] 			\n"

				"	ldr 	q1, [x12], #16  							\n"
				"	fmla	v19.4s, v11.4s, v12.4s 				\n"
				"	fmla	v23.4s, v11.4s, v13.4s 				\n"
				"	ldr 	q4, [x15], #16 								\n"
				"	fmla	v27.4s, v11.4s, v14.4s 				\n"

				"	st4		{v12.s, v13.s, v14.s, v15.s}[2], [x24]		\n"
				"	add 		x24, x24, #64 							\n"

				"	fmla	v28.4s, v8.4s, v15.4s 				\n"
				"	ldr 	q2, [x13], #16 								\n"
				"	fmla	v29.4s, v9.4s, v15.4s 				\n"
				"	ldr 	q5, [x16], #16 								\n"
				"	fmla	v30.4s, v10.4s, v15.4s 				\n"
				"	ldr 	q6, [x17], #16 								\n"
				"	fmla	v31.4s, v11.4s, v15.4s 				\n"
				"	st4		{v12.s, v13.s, v14.s, v15.s}[3], [x24]		\n"
				"	add 		x24, x24, #64 							\n"

				".endm 																\n"


				".macro PACK_KERNEL4x4_END_K      		\n"
				"										 	 								\n"

				"	ldr 	q11, [x14], #16 							\n"
				"	fmla	v16.4s, v8.4s, v12.4s 				\n"
				"	fmla	v17.4s, v9.4s, v12.4s 				\n"
				"	fmla	v18.4s, v10.4s, v12.4s 				\n"

				"	ldr 	q15, [x18], #16 							\n"

				"	fmla	v20.4s, v8.4s, v13.4s 				\n"
				"	fmla	v21.4s, v9.4s, v13.4s 				\n"
				"	st4		{v12.s, v13.s, v14.s, v15.s}[0], [x24]		\n"
				"	add 		x24, x24, #80 							\n"
				"	fmla	v22.4s, v10.4s, v13.4s 				\n"

				"	fmla	v24.4s, v8.4s, v14.4s 				\n"
				"	fmla	v25.4s, v9.4s, v14.4s 				\n"
				"	st4		{v12.s, v13.s, v14.s, v15.s}[1], [x24]		\n"
				"	add 		x24, x24, #64 							\n"
				"	fmla	v26.4s, v10.4s, v14.4s 				\n"

				"	fmla	v19.4s, v11.4s, v12.4s 				\n"
				"	fmla	v23.4s, v11.4s, v13.4s 				\n"
				"	fmla	v27.4s, v11.4s, v14.4s 				\n"

				"	st4		{v12.s, v13.s, v14.s, v15.s}[2], [x24]		\n"
				"	add 		x24, x24, #64 							\n"

				"	fmla	v28.4s, v8.4s, v15.4s 				\n"
				"	fmla	v29.4s, v9.4s, v15.4s 				\n"
				"	fmla	v30.4s, v10.4s, v15.4s 				\n"
				"	fmla	v31.4s, v11.4s, v15.4s 				\n"
				"	st4		{v12.s, v13.s, v14.s, v15.s}[3], [x24]		\n"
				"	add 		x24, x24, #64 							\n"


				"	faddp		v16.4s, v16.4s, v20.4s 			\n"
				"	faddp 	v17.4s, v17.4s, v21.4s 			\n"
				"	faddp		v18.4s, v18.4s, v22.4s 			\n"
				"	faddp		v19.4s, v19.4s, v23.4s 			\n"

				"	faddp		v24.4s, v24.4s, v28.4s 			\n"
				"	faddp 	v25.4s, v25.4s, v29.4s 			\n"
				"	faddp		v26.4s, v26.4s, v30.4s 			\n"
				"	faddp		v27.4s, v27.4s, v31.4s 			\n"

				"	faddp 	v16.4s, v16.4s, v24.4s 			\n"
				"	faddp 	v17.4s, v17.4s, v25.4s 			\n"
				"	faddp 	v18.4s, v18.4s, v26.4s 			\n"
				"	faddp 	v19.4s, v19.4s, v27.4s 			\n"

				".endm 																\n"


				".macro	PACK4x4_ADD_C 						 		\n"

				"	ldr		q0, [x25]						 					\n"
				"	ldr		q1, [x26]						 					\n"

				"	fadd 	v16.4s, v16.4s, v0.4s			 		\n"
				"	ldr		q2, [x27]						 					\n"
				"	fadd 	v17.4s, v17.4s, v1.4s			 		\n"
				"	ldr		q3, [x28]						 					\n"
				"	fadd 	v18.4s, v18.4s, v2.4s			 		\n"
				"	fadd 	v19.4s, v19.4s, v3.4s			 		\n"

				".endm 									 	 						\n"


				".macro SAVE4x4							 	 				\n"

				"	str		q16, [x25], #16					 			\n"
				"	prfm	PLDL2KEEP, [x25, #64]			 		\n"
				"	str		q17, [x26], #16					 			\n"
				"	prfm	PLDL2KEEP, [x26, #64]			 		\n"
				"	str		q18, [x27], #16		 			 			\n"
				"	prfm	PLDL2KEEP, [x27, #64]			 		\n"
				"	str		q19, [x28], #16		 			 			\n"
				"	prfm	PLDL2KEEP, [x28, #64]			 		\n"

				"	add 	x10, x10, x6 ,lsl #4 					\n"

				".endm 									 	 						\n"



				".macro KERNEL4x17_BEGIN_K       			\n"
				"										 	 								\n"

				"	ldr		q0, [x11], #16 								\n"
				"	ldr		q1, [x12], #16 								\n"

				"	prfm	PLDL1KEEP, [x11, #128]				\n"
				"	prfm	PLDL1KEEP, [x12, #128]				\n"

				"	ldr		q4, [x24], #16 								\n"
				"	fmul	v12.4s, v4.4s, v0.s[0]				\n"

				"	ldr		q2, [x13], #16 								\n"
				"	fmul	v13.4s, v4.4s, v1.s[0]				\n"
				"	ldr		q3, [x14], #16 								\n"
				"	fmul	v14.4s, v4.4s, v2.s[0] 				\n"

				"	prfm	PLDL1KEEP, [x13, #128]				\n"
				"	prfm	PLDL1KEEP, [x14, #128]				\n"

				"	ldr		q5, [x24], #16 								\n"
				"	fmul	v15.4s, v4.4s, v3.s[0] 				\n"
				"	ldr		q6, [x24], #16 								\n"

				"	fmul	v16.4s, v5.4s, v0.s[0]				\n"
				"	ldr		q7, [x24], #16 								\n"
				"	fmul	v17.4s, v5.4s, v1.s[0]				\n"
				"	fmul	v18.4s, v5.4s, v2.s[0]				\n"
				"	fmul	v19.4s, v5.4s, v3.s[0]				\n"

				"	prfm	PLDL1KEEP, [x24, #256]				\n"

				"	ldr		q8, [x24], #16 								\n"

				"	fmul	v20.4s, v6.4s, v0.s[0]				\n"
				"	fmul	v21.4s, v6.4s, v1.s[0]				\n"
				"	fmul	v22.4s, v6.4s, v2.s[0]				\n"
				"	fmul	v23.4s, v6.4s, v3.s[0]				\n"

				"	ldp 	q4, q5, [x24], #32 						\n"

				"	fmul	v24.4s, v7.4s, v0.s[0]				\n"
				"	fmul	v25.4s, v7.4s, v1.s[0]				\n"
				"	fmul	v26.4s, v7.4s, v2.s[0]				\n"
				"	fmul	v27.4s, v7.4s, v3.s[0]				\n"

				"	ldp 	q6, q7, [x24], #32 						\n"

				"	fmul	v28.4s, v8.4s, v0.4s 				  \n"
				"	fmul	v29.4s, v8.4s, v1.4s 					\n"
				"	fmul	v30.4s, v8.4s, v2.4s 					\n"
				"	fmul 	v31.4s, v8.4s, v3.4s 					\n"

				".endm 																\n"


				".macro KERNEL4x17_K0       					\n"

				
				"	ldr		q8, [x24], #16 								\n"
				"	fmla	v12.4s, v4.4s, v0.s[0]				\n"
				"	fmla	v13.4s, v4.4s, v1.s[0]				\n"
//				"	ldr 	q9, [x11], #16 								\n"          //特别注意
				"	fmla	v14.4s, v4.4s, v2.s[0]				\n"
				"	fmla	v15.4s, v4.4s, v3.s[0]				\n"
				"	ldr 	q4, [x24], #16 								\n"

				"	fmla	v16.4s, v5.4s, v0.s[0]				\n"
				"	fmla	v17.4s, v5.4s, v1.s[0]				\n"
				"	fmla	v18.4s, v5.4s, v2.s[0]				\n"
				"	fmla	v19.4s, v5.4s, v3.s[0]				\n"
				"	ldr 	q5, [x24], #16 								\n"

				"	prfm	PLDL1KEEP, [x24, #256]				\n"

				"	fmla	v20.4s, v6.4s, v0.s[0]				\n"
				"	fmla	v21.4s, v6.4s, v1.s[0]				\n"
				"	fmla	v22.4s, v6.4s, v2.s[0]				\n"
				"	fmla	v23.4s, v6.4s, v3.s[0]				\n"
				"	ldr 	q6, [x24], #16 								\n"

				"	fmla	v24.4s, v7.4s, v0.s[0]				\n"
				"	fmla	v25.4s, v7.4s, v1.s[0]				\n"
				"	fmla	v26.4s, v7.4s, v2.s[0]				\n"
				"	fmla	v27.4s, v7.4s, v3.s[0]				\n"
				"	ldr 	q7, [x24], #16 								\n"


				"	fmla	v28.4s, v8.4s, v0.4s					\n"
				"	fmla	v29.4s, v8.4s, v1.4s					\n"
				"	fmla	v30.4s, v8.4s, v2.4s					\n"
				"	fmla	v31.4s, v8.4s, v3.4s					\n"

				".endm 																\n"


				".macro KERNEL4x17_K1       					\n"

				"	prfm	PLDL1KEEP, [x11, #128]				\n"

				"	ldr 	q9, [x11], #16 								\n"
				"	fmla	v12.4s, v4.4s, v0.s[1]				\n"
				"	fmla	v13.4s, v4.4s, v1.s[1]				\n"
				"	fmla	v14.4s, v4.4s, v2.s[1]				\n"
				"	fmla	v15.4s, v4.4s, v3.s[1]				\n"
				"	ldr 	q4, [x24], #16 								\n"

				"	prfm	PLDL1KEEP, [x12, #128]				\n"

				"	fmla	v16.4s, v5.4s, v0.s[1]				\n"
				"	fmla	v17.4s, v5.4s, v1.s[1]				\n"
				"	fmla	v18.4s, v5.4s, v2.s[1]				\n"
				"	fmla	v19.4s, v5.4s, v3.s[1]				\n"
				"	ldr 	q5, [x24], #16 								\n"

				"	prfm	PLDL1KEEP, [x24, #256]				\n"

				"	fmla	v20.4s, v6.4s, v0.s[1]				\n"
				"	fmla	v21.4s, v6.4s, v1.s[1]				\n"
				"	ldr 	q10, [x12], #16 							\n"
				"	fmla	v22.4s, v6.4s, v2.s[1]				\n"
				"	fmla	v23.4s, v6.4s, v3.s[1]				\n"
				"	ldr 	q6, [x24], #16 								\n"

				"	fmla	v24.4s, v7.4s, v0.s[1]				\n"
				"	fmla	v25.4s, v7.4s, v1.s[1]				\n"
				"	fmla	v26.4s, v7.4s, v2.s[1]				\n"
				"	fmla	v27.4s, v7.4s, v3.s[1]				\n"
				"	ldr 	q7, [x24], #16 								\n"

				".endm 																\n"


				".macro KERNEL4x17_K2       					\n"

				"	prfm	PLDL1KEEP, [x13, #128]				\n"

				"	ldr 	q11, [x13], #16 							\n"
				"	fmla	v12.4s, v4.4s, v0.s[2]				\n"
				"	fmla	v13.4s, v4.4s, v1.s[2]				\n"
				"	fmla	v14.4s, v4.4s, v2.s[2]				\n"
				"	fmla	v15.4s, v4.4s, v3.s[2]				\n"
				"	ldr 	q4, [x24], #16 								\n"

				"	prfm	PLDL1KEEP, [x14, #128]				\n"

				"	fmla	v16.4s, v5.4s, v0.s[2]				\n"
				"	fmla	v17.4s, v5.4s, v1.s[2]				\n"
				"	fmla	v18.4s, v5.4s, v2.s[2]				\n"
				"	fmla	v19.4s, v5.4s, v3.s[2]				\n"
				"	ldr 	q5, [x24], #16 								\n"

				"	fmla	v20.4s, v6.4s, v0.s[2]				\n"
				"	fmla	v21.4s, v6.4s, v1.s[2]				\n"
				"	fmla	v22.4s, v6.4s, v2.s[2]				\n"
				"	fmla	v23.4s, v6.4s, v3.s[2]				\n"
				"	ldr 	q6, [x24], #16 								\n"

				"	fmla	v24.4s, v7.4s, v0.s[2]				\n"
				"	fmla	v25.4s, v7.4s, v1.s[2]				\n"
				"	fmla	v26.4s, v7.4s, v2.s[2]				\n"
				"	fmla	v27.4s, v7.4s, v3.s[2]				\n"
				"	ldr 	q7, [x24], #16 								\n"

				".endm 																\n"



				".macro KERNEL4x17_K3       					\n"

				"	fmla	v12.4s, v4.4s, v0.s[3]				\n"
				"	fmla	v13.4s, v4.4s, v1.s[3]				\n"
				"	fmla	v14.4s, v4.4s, v2.s[3]				\n"
				"	fmla	v15.4s, v4.4s, v3.s[3]				\n"
				"	ldr 	q4, [x24], #16 								\n"

				"	fmla	v16.4s, v5.4s, v0.s[3]				\n"
				"	fmla	v17.4s, v5.4s, v1.s[3]				\n"
				"	fmla	v18.4s, v5.4s, v2.s[3]				\n"
				"	fmla	v19.4s, v5.4s, v3.s[3]				\n"
				"	ldr 	q5, [x24], #16 								\n"

				"	fmla	v20.4s, v6.4s, v0.s[3]				\n"
				"	fmla	v24.4s, v7.4s, v0.s[3]				\n"

				"	ldr		q0, [x14], #16 								\n"
				"	fmla	v21.4s, v6.4s, v1.s[3]				\n"
				"	fmla	v22.4s, v6.4s, v2.s[3]				\n"
				"	fmla	v23.4s, v6.4s, v3.s[3]				\n"
				"	ldr 	q6, [x24], #16 								\n"

				"	fmla	v25.4s, v7.4s, v1.s[3]				\n"
				"	fmla	v26.4s, v7.4s, v2.s[3]				\n"
				"	fmla	v27.4s, v7.4s, v3.s[3]				\n"
				"	ldr 	q7, [x24], #16 								\n"

				".endm 																\n"



				".macro KERNEL4x17_K4       					\n"

				"	prfm PLDL1KEEP, [x11, #128]					\n"
				"	ldr		q8, [x24], #16 								\n"
				"	fmla	v12.4s, v4.4s, v9.s[0]				\n"
				"	fmla	v13.4s, v4.4s, v10.s[0]				\n"
				"	ldr 	q1, [x12], #16 								\n"
				"	fmla	v14.4s, v4.4s, v11.s[0]				\n"
				"	fmla	v15.4s, v4.4s, v0.s[0]				\n"
				"	ldr 	q4, [x24], #16 								\n"

				"	prfm PLDL1KEEP, [x12, #128]					\n"

				"	fmla	v16.4s, v5.4s, v9.s[0]				\n"
				"	fmla	v17.4s, v5.4s, v10.s[0]				\n"
				"	fmla	v18.4s, v5.4s, v11.s[0]				\n"
				"	fmla	v19.4s, v5.4s, v0.s[0]				\n"
				"	ldr 	q5, [x24], #16 								\n"

				"	prfm	PLDL1KEEP, [x24, #256]				\n"

				"	fmla	v20.4s, v6.4s, v9.s[0]				\n"
				"	fmla	v21.4s, v6.4s, v10.s[0]				\n"
				"	fmla	v22.4s, v6.4s, v11.s[0]				\n"
				"	fmla	v23.4s, v6.4s, v0.s[0]				\n"
				"	ldr 	q6, [x24], #16 								\n"

				"	fmla	v24.4s, v7.4s, v9.s[0]				\n"
				"	fmla	v25.4s, v7.4s, v10.s[0]				\n"
				"	fmla	v26.4s, v7.4s, v11.s[0]				\n"
				"	fmla	v27.4s, v7.4s, v0.s[0]				\n"
				"	ldr 	q7, [x24], #16 								\n"


				"	fmla	v28.4s, v8.4s, v9.4s					\n"
				"	fmla	v29.4s, v8.4s, v10.4s					\n"
				"	fmla	v30.4s, v8.4s, v11.4s					\n"
				"	fmla	v31.4s, v8.4s, v0.4s					\n"

				".endm 																\n"



				".macro KERNEL4x17_K5       					\n"

				"	prfm PLDL1KEEP, [x13, #128]					\n"
				"	ldr 	q2, [x13], #16 								\n"

				"	fmla	v12.4s, v4.4s, v9.s[1]				\n"
				"	fmla	v13.4s, v4.4s, v10.s[1]				\n"
				"	fmla	v14.4s, v4.4s, v11.s[1]				\n"
				"	fmla	v15.4s, v4.4s, v0.s[1]				\n"
				"	ldr 	q4, [x24], #16 								\n"

				"	prfm PLDL1KEEP, [x14, #128]					\n"

				"	fmla	v16.4s, v5.4s, v9.s[1]				\n"
				"	fmla	v17.4s, v5.4s, v10.s[1]				\n"
				"	fmla	v18.4s, v5.4s, v11.s[1]				\n"
				"	fmla	v19.4s, v5.4s, v0.s[1]				\n"
				"	ldr 	q5, [x24], #16 								\n"

				"	prfm	PLDL1KEEP, [x24, #256]				\n"

				"	fmla	v20.4s, v6.4s, v9.s[1]				\n"
				"	fmla	v21.4s, v6.4s, v10.s[1]				\n"
				"	fmla	v22.4s, v6.4s, v11.s[1]				\n"
				"	fmla	v23.4s, v6.4s, v0.s[1]				\n"
				"	ldr 	q6, [x24], #16 								\n"

				"	fmla	v24.4s, v7.4s, v9.s[1]				\n"
				"	fmla	v25.4s, v7.4s, v10.s[1]				\n"
				"	fmla	v26.4s, v7.4s, v11.s[1]				\n"
				"	fmla	v27.4s, v7.4s, v0.s[1]				\n"
				"	ldr 	q7, [x24], #16 								\n"

				".endm 																\n"



				".macro KERNEL4x17_K6       					\n"

				"	prfm PLDL1KEEP, [x11, #128]					\n"
				"	ldr 	q3, [x14], #16 								\n"

				"	fmla	v12.4s, v4.4s, v9.s[2]				\n"
				"	fmla	v13.4s, v4.4s, v10.s[2]				\n"
				"	fmla	v14.4s, v4.4s, v11.s[2]				\n"
				"	fmla	v15.4s, v4.4s, v0.s[2]				\n"
				"	ldr 	q4, [x24], #16 								\n"

				"	prfm PLDL1KEEP, [x12, #128]					\n"

				"	fmla	v16.4s, v5.4s, v9.s[2]				\n"
				"	fmla	v17.4s, v5.4s, v10.s[2]				\n"
				"	fmla	v18.4s, v5.4s, v11.s[2]				\n"
				"	fmla	v19.4s, v5.4s, v0.s[2]				\n"
				"	ldr 	q5, [x24], #16 								\n"

				"	fmla	v20.4s, v6.4s, v9.s[2]				\n"
				"	fmla	v21.4s, v6.4s, v10.s[2]				\n"
				"	fmla	v22.4s, v6.4s, v11.s[2]				\n"
				"	fmla	v23.4s, v6.4s, v0.s[2]				\n"
				"	ldr 	q6, [x24], #16 								\n"

				"	fmla	v24.4s, v7.4s, v9.s[2]				\n"
				"	fmla	v25.4s, v7.4s, v10.s[2]				\n"
				"	fmla	v26.4s, v7.4s, v11.s[2]				\n"
				"	fmla	v27.4s, v7.4s, v0.s[2]				\n"
				"	ldr 	q7, [x24], #16 								\n"

				".endm 																\n"


				".macro KERNEL4x17_K7       					\n"

				"	prfm PLDL1KEEP, [x13, #128]					\n"

				"	fmla	v12.4s, v4.4s, v9.s[3]				\n"
				"	fmla	v13.4s, v4.4s, v10.s[3]				\n"
				"	fmla	v14.4s, v4.4s, v11.s[3]				\n"
				"	fmla	v15.4s, v4.4s, v0.s[3]				\n"
				"	ldr 	q4, [x24], #16 								\n"

				"	fmla	v19.4s, v5.4s, v0.s[3]				\n"
				"	fmla	v23.4s, v6.4s, v0.s[3]				\n"
				"	fmla	v27.4s, v7.4s, v0.s[3]				\n"
				"	ldr		q0, [x11], #16 								\n"

				"	prfm PLDL1KEEP, [x14, #128]					\n"
				"	fmla	v16.4s, v5.4s, v9.s[3]				\n"
				"	fmla	v17.4s, v5.4s, v10.s[3]				\n"
				"	fmla	v18.4s, v5.4s, v11.s[3]				\n"
				"	ldr 	q5, [x24], #16 								\n"

				"	fmla	v20.4s, v6.4s, v9.s[3]				\n"
				"	fmla	v21.4s, v6.4s, v10.s[3]				\n"
				"	fmla	v22.4s, v6.4s, v11.s[3]				\n"
				
				"	ldr 	q6, [x24], #16 								\n"
				"	fmla	v24.4s, v7.4s, v9.s[3]				\n"
				"	fmla	v25.4s, v7.4s, v10.s[3]				\n"
				"	fmla	v26.4s, v7.4s, v11.s[3]				\n"
				"	ldr 	q7, [x24], #16 								\n"

				".endm 																\n"



				".macro KERNEL4x17_END_K       				\n"

				"	fmla	v12.4s, v4.4s, v9.s[3]				\n"
				"	fmla	v13.4s, v4.4s, v10.s[3]				\n"
				"	fmla	v14.4s, v4.4s, v11.s[3]				\n"
				"	fmla	v15.4s, v4.4s, v0.s[3]				\n"

				"	fmla	v19.4s, v5.4s, v0.s[3]				\n"
				"	fmla	v23.4s, v6.4s, v0.s[3]				\n"
				"	fmla	v27.4s, v7.4s, v0.s[3]				\n"

				"	fmla	v16.4s, v5.4s, v9.s[3]				\n"
				"	fmla	v17.4s, v5.4s, v10.s[3]				\n"
				"	fmla	v18.4s, v5.4s, v11.s[3]				\n"

				"	fmla	v20.4s, v6.4s, v9.s[3]				\n"
				"	fmla	v21.4s, v6.4s, v10.s[3]				\n"
				"	fmla	v22.4s, v6.4s, v11.s[3]				\n"

				"	faddp	v28.4s, v28.4s, v29.4s 				\n"
				"	faddp	v30.4s, v30.4s, v31.4s 				\n"

				"	fmla	v24.4s, v7.4s, v9.s[3]				\n"
				"	fmla	v25.4s, v7.4s, v10.s[3]				\n"
				"	fmla	v26.4s, v7.4s, v11.s[3]				\n"

				"	faddp	v28.4s, v28.4s, v30.4s 				\n"

				".endm 																\n"



				".macro	M4N17_ADD_C 						 			\n"

				"	prfm	PLDL1KEEP, [x25, #64]					\n"
				"	prfm	PLDL1KEEP, [x26, #64]					\n"
				"	ldp		q0, q1, [x25]						 			\n"
				"	ldp		q2, q3, [x26]					 				\n"

				"	fadd 	v12.4s, v12.4s, v0.4s 				\n"
				"	fadd 	v16.4s, v16.4s, v1.4s 				\n"
				
				"	prfm	PLDL1KEEP, [x27, #64]					\n"
				"	prfm	PLDL1KEEP, [x28, #64]					\n"

				"	ldp		q4, q5, [x27]									\n"
				"	fadd 	v13.4s, v13.4s, v2.4s 				\n"
				"	fadd 	v17.4s, v17.4s, v3.4s 				\n"

				"	ldp		q6, q7, [x28]									\n"
				"	fadd 	v14.4s, v14.4s, v4.4s 				\n"
				"	fadd 	v18.4s, v18.4s, v5.4s 				\n"

				"	ldp		q0, q1, [x25, #32]						\n"
				"	add 	x25, x25, #64 								\n"
				"	fadd 	v15.4s, v15.4s, v6.4s 				\n"
				"	fadd 	v19.4s, v19.4s, v7.4s 				\n"

				"	ldp		q2, q3, [x26, #32]						\n"
				"	add 	x26, x26, #64 								\n"
				"	fadd 	v20.4s, v20.4s, v0.4s 				\n"
				"	ldp		q4, q5, [x27, #32]						\n"
				"	add 	x27, x27, #64 								\n"
				"	fadd 	v24.4s, v24.4s, v1.4s 				\n"

				"	ldp		q6, q7, [x28, #32]						\n"
				"	add 	x28, x28, #64 								\n"
				"	fadd 	v21.4s, v21.4s, v2.4s 				\n"
				"	ld1		{v0.s}[0], [x25]							\n"
				"	sub 	x25, x25, #64 								\n"
				"	fadd 	v25.4s, v25.4s, v3.4s 				\n"

				"	ld1		{v0.s}[1], [x26]							\n"
				"	sub 	x26, x26, #64 								\n"
				"	fadd 	v22.4s, v22.4s, v4.4s 				\n"
				"	ld1		{v0.s}[2], [x27]							\n"
				"	sub 	x27, x27, #64 								\n"
				"	fadd 	v26.4s, v26.4s, v5.4s 				\n"
				"	ld1		{v0.s}[3], [x28]							\n"
				"	sub 	x28, x28, #64 								\n"

				"	fadd 	v23.4s, v23.4s, v6.4s 				\n"
				"	fadd 	v27.4s, v27.4s, v7.4s 				\n"

				"	fadd 	v28.4s, v28.4s, v0.4s 				\n"

				".endm 									 	 						\n"



				".macro SAVE4x17							 	 			\n"

				"	stp		q12, q16, [x25]					 			\n"
				"	stp		q13, q17, [x26]					 			\n"
				"	stp		q14, q18, [x27]					 			\n"
				"	stp		q15, q19, [x28]					 			\n"

				"	subs 	x21, x21, #1 									\n"

				"	stp		q20, q24, [x25, #32]					\n"
				"	add 	x25, x25, #64 								\n"
				"	stp		q21, q25, [x26, #32]					\n"
				"	add 	x26, x26, #64 								\n"
				"	stp		q22, q26, [x27, #32]					\n"
				"	add 	x27, x27, #64 								\n"
				"	stp		q23, q27, [x28, #32]					\n"
				"	add 	x28, x28, #64 								\n"

				"	st1 	{v28.s}[0], [x25] 						\n"
				"	sub  	x25, x25, #64 								\n"
				"	st1 	{v28.s}[1], [x26] 						\n"
				"	sub  	x26, x26, #64 								\n"
				"	st1 	{v28.s}[2], [x27] 						\n"
				"	sub  	x27, x27, #64 								\n"
				"	st1 	{v28.s}[3], [x28] 						\n"
				"	sub  	x28, x28, #64 								\n"


				"	add 	x25, x25, x9, lsl #4 					\n"
				"	prfm	PLDL2KEEP, [x25, #64]			 		\n"
				"	add 	x26, x26, x9, lsl #4 					\n"
				"	prfm	PLDL2KEEP, [x26, #64]			 		\n"
				"	add 	x27, x27, x9, lsl #4 					\n"
				"	prfm	PLDL2KEEP, [x27, #64]			 		\n"
				"	add 	x28, x28, x9, lsl #4 					\n"
				"	prfm	PLDL2KEEP, [x28, #64]			 		\n"
				"	add 	x10, x10, x6, lsl #4 					\n"

				".endm 									 	 						\n"


				//----------------------------------------------------

				"SMM_exist_1:									 	 		\n"

				"	ldr		x0, %[C]						 				\n"
				"	ldr		x1, %[A]						 				\n"
				"	ldr		x2, %[B]						 				\n"

				"	ldr		x3, %[M]						 				\n"
				"	ldr		x4, %[N]						 				\n"
				"	ldr		x5, %[K]						 				\n"
				"	ldr		x9, %[LN] 						 			\n"
				"	ldr		x6, %[LK]						 				\n"

				"	ldr		x19, %[SB]						 			\n"
				"	ldr		x8, %[k_tag] 					 			\n"

				"	add		sp, sp, #-32					 			\n"

				"	prfm		PLDL1KEEP, [x1]     			\n"
				"	prfm    PLDL1KEEP, [x2]     			\n"

				"	lsr 	x21, x3, #2				 	 				\n"	// M / 4

				"	and	 	x23, x3, #3 			 					\n"	// M % 4
				"	str 	x23, [sp] 						 			\n"
				"	mov 	x23, x19 						 				\n" //SB
				"	str  	x8, [sp, #16]					 			\n" // store k_tag


				"	mov		x25, x0 										\n"
				"	add		x26, x25, x9, lsl #2 			  \n"   //C1*
				"	add		x27, x26, x9, lsl #2 			  \n"   //C2*
				"	add		x28, x27, x9, lsl #2 			  \n"   //C3*

				"	mov		x10, x2 										\n"
				"	mov 	x7, #3 											\n"



				//----------------------------------------------------

				"N4_BEGIN_PACKB:								 		\n"

				"	mov		x15, x10						 	 			\n"   //B0*
				"	add		x16, x15, x6, lsl #2 			 	\n"
				"	add		x17, x16, x6, lsl #2 			 	\n"
				"	add		x18, x17, x6, lsl #2 			 	\n"

				"	prfm    PLDL1KEEP, [x15, #64]			\n"
				"	prfm    PLDL1KEEP, [x16, #64]			\n"
				"	prfm    PLDL1KEEP, [x17, #64]			\n"
				"	prfm    PLDL1KEEP, [x18, #64]			\n"

				"	mov		x11, x1						 					\n" //A0*
				"	add		x12, x11, x6, lsl #2			 	\n" //A1*
				"	add		x13, x12, x6, lsl #2			 	\n"	//A2*
				"	add		x14, x13, x6, lsl #2			 	\n"	//A3*

				"	prfm	PLDL1KEEP, [x11, #64]			\n"
				"	prfm    PLDL1KEEP, [x12, #64]			\n"
				"	prfm    PLDL1KEEP, [x13, #64]			\n"
				"	prfm    PLDL1KEEP, [x14, #64]			\n"		

				"	mov		x24, x23 										\n"

				"N4_PACK_Body_K:								 		\n"

				"	lsr		x22, x5, #3 					 			\n"     // K / 8

				"	PACK_KERNEL4x4_BEGIN_K				 	 	\n"

				"	subs	x22, x22, #1		     		 		\n"		
				"	b 		N4_PACK_K1_7						 		\n"

				"N4_PACK_K:									 				\n"
		
				"	PACK_KERNEL4x4_K0						 			\n"
				"N4_PACK_K1_7:									 		\n"

				"	beq		N4_PACK_Edge_K						 	\n"

				"	PACK_KERNEL4x4_K1						 			\n"
		
				"	subs	x22, x22, #1			 		 			\n"
				"	b 		N4_PACK_K			 				 			\n"	

				"N4_PACK_Edge_K:							     	\n"

				"	PACK_KERNEL4x4_END_K			        \n"

				"	ldr		x8, [sp, #16]					 			\n"	
				"	cmp		x8, #0							 				\n"
				"	beq		M4N4_SAVE 						 			\n"
				"	PACK4x4_ADD_C 								 	 	\n"
				"M4N4_SAVE: 									 			\n"
				"	SAVE4x4								 						\n"
				"	subs 	x7, x7, #1  								\n"
				"	add 	x23, x23, #16 							\n"

				"	bgt		N4_BEGIN_PACKB							\n"

//----------------------------------------------



				"N5_BEGIN_PACKB:								 		\n"

				"	mov		x15, x10						 	 			\n"   //B0*
				"	add		x16, x15, x6, lsl #2 			 	\n"
				"	add		x17, x16, x6, lsl #2 			 	\n"
				"	add		x18, x17, x6, lsl #2 			 	\n"
				"	add		x19, x18, x6, lsl #2 			 	\n"

				"	prfm    PLDL1KEEP, [x15, #64]			\n"
				"	prfm    PLDL1KEEP, [x16, #64]			\n"
				"	prfm    PLDL1KEEP, [x17, #64]			\n"
				"	prfm    PLDL1KEEP, [x18, #64]			\n"
				"	prfm    PLDL1KEEP, [x19, #64]			\n"		

				"	mov		x11, x1						 					\n" //A0*
				"	add		x12, x11, x6, lsl #2			 	\n" //A1*
				"	add		x13, x12, x6, lsl #2			 	\n"	//A2*
				"	add		x14, x13, x6, lsl #2			 	\n"	//A3*

				"	prfm	PLDL1KEEP, [x11, #64]			\n"
				"	prfm    PLDL1KEEP, [x12, #64]			\n"
				"	prfm    PLDL1KEEP, [x13, #64]			\n"
				"	prfm    PLDL1KEEP, [x14, #64]			\n"

				"	mov		x24, x23 										\n"

				"N5_PACK_Body_K:								 		\n"

				"	lsr		x22, x5, #3 					 			\n"     // K / 8

				"	PACK_KERNEL4x5_BEGIN_K				 	 	\n"

				"	subs	x22, x22, #1		     		 		\n"		
				"	b 		N5_PACK_K1_7						 		\n"

				"N5_PACK_K:									 				\n"
		
				"	PACK_KERNEL4x5_K0						 			\n"
				"N5_PACK_K1_7:									 		\n"

				"	beq		N5_PACK_Edge_K						 	\n"

				"	PACK_KERNEL4x5_K1						 			\n"
		
				"	subs	x22, x22, #1			 		 			\n"
				"	b 		N5_PACK_K			 				 			\n"	

				"N5_PACK_Edge_K:							     	\n"

				"	PACK_KERNEL4x5_END_K			        \n"

				"	ldr		x8, [sp, #16]					 			\n"	
				"	cmp		x8, #0							 				\n"
				"	beq		M4N5_SAVE 						 			\n"
				"	PACK4x5_ADD_C 								 	 	\n"
				"M4N5_SAVE: 									 			\n"
				"	SAVE4x5								 						\n"

				"	subs	x21, x21, #1 								\n"
				"	sub 	x23, x23, #48 							\n"
				"	mov 	x10, x1 										\n"  
				"	add  	x10, x10, x6, lsl #4 				\n"	 //A
				"	sub   x25, x25, #68 							\n"  //C
				"	add 	x25, x25, x9, lsl #4 				\n"
				"	add 	x26, x25, x9, lsl #2 				\n"
				"	add 	x27, x26, x9, lsl #2 				\n"
				"	add 	x28, x27, x9, lsl #2 				\n"
				"	beq 	END_M4N17 									\n"






//--------------------------------------------------------

				"BEGIN_M4:													\n"

				"	mov 	x11, x10										\n"  //A0*
				"	add 	x12, x11, x6, lsl #2 				\n"  //A1*
				"	add 	x13, x12, x6, lsl #2 				\n"	 //A2*
				"	add 	x14, x13, x6, lsl #2 				\n"  //A3*

				"	prfm    PLDL1KEEP, [x11, #64]			\n"
				"	prfm    PLDL1KEEP, [x12, #64]			\n"
				"	prfm    PLDL1KEEP, [x13, #64]			\n"
				"	prfm    PLDL1KEEP, [x14, #64]			\n"

				"	mov 	x24, x23 										\n"  //B*

				"	prfm    PLDL1KEEP, [x24, #256]		\n"

//----------------------------------------------------
				"Body_K:								 	 					\n"

				"	lsr		x22, x5, #3 					 			\n"     // K / 8
				"	KERNEL4x17_BEGIN_K						 		\n"
				"	subs	x22, x22, #1		     		 		\n"	
				"	b K1_7									 					\n"

				"Main_K:									 					\n"
				
				"	KERNEL4x17_K0						 	 				\n"
				"K1_7:										 					\n"
				
				"	KERNEL4x17_K1							 				\n"
				"	KERNEL4x17_K2							 				\n"
				"	KERNEL4x17_K3							 				\n"
				"	KERNEL4x17_K4							 				\n"
				"	KERNEL4x17_K5							 				\n"
				"	KERNEL4x17_K6							 				\n"

				"	beq		Edge_K							 				\n"

				"	KERNEL4x17_K7							 				\n"
				
				"	subs	x22, x22, #1			 		 			\n"
				"	b 		Main_K			 				 				\n"	

				"Edge_K:									 					\n"

				"	KERNEL4x17_END_K						 			\n"

				"	ldr		x8, [sp, #16]					 			\n"	
				"	cmp		x8, #0							 				\n"
				"	beq		M4N17_SAVE 						 			\n"
				"	M4N17_ADD_C 								 	 		\n"
				"M4N17_SAVE: 									 			\n"
				"	SAVE4x17								 					\n"	

				"	bgt   	BEGIN_M4						 			\n"


				"END_M4N17: 												\n"
				"	add 	sp, sp ,#32  								\n"


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