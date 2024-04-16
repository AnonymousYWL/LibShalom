#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <omp.h>

#define NUM 64

static double gtod_ref_time_sec = 0.0;

double dclock()
{
	double the_time, norm_sec;
	struct timeval tv;

	gettimeofday( &tv, NULL );

	if ( gtod_ref_time_sec == 0.0 )
		gtod_ref_time_sec = ( double ) tv.tv_sec;

	norm_sec = ( double ) tv.tv_sec - gtod_ref_time_sec;

	the_time = norm_sec + tv.tv_usec * 1.0e-6;

	return the_time;
}

void mem_to_reg(float *A, long K, long LN, long N)
{


	int i,j,kk;

	for(kk = 0; kk < K; kk = kk + 1600)
	{
		float *temp = A + kk * LN;
		long kc = 1600;
		asm volatile(


		        "	ldr	x0, %[A]					\n"
		        "	ldr	x4, %[LN] 					\n"
		        "	ldr	x28, %[N]					\n"
		        "	ldr 	x15, %[kc] 					\n"


		        "	mov	x30, #12 					\n"
		        "	udiv  	x29, x28, x30 					\n"
		        "	mov 	x21, x29 					\n"

		        "BEGIN_MEM_N:							\n"

		        "	mov	x5, x0 						\n"
		        "	add  	x6, x5, x4, lsl #2 				\n"
		        "	add  	x7, x5, x4, lsl #3 				\n"
		        "	add  	x8, x6, x4, lsl #3 				\n"

		       	"	add  	x9, x7, x4, lsl #3 				\n"
		        "	add  	x10, x8, x4, lsl #3 				\n"
		        "	add  	x11, x9, x4, lsl #3 				\n"
		        "	add  	x12, x10, x4, lsl #3 				\n"

		        "	lsr	x22, x15, #3 					\n"

		        "BEGIN_MEM_INNER_K:						\n"

		        "	subs  	x22, x22, #1 					\n"

		        "	ldr	q0, [x5]					\n"
		        "	ldp	q1, q2, [x5, #16]				\n"
		        "	add  	x5, x5, x4, lsl #5 				\n"

		       	"	ldr	q3, [x6]					\n"
		        "	ldp	q4, q5, [x6, #16]				\n"
		        "	add  	x6, x6, x4, lsl #5 				\n"

		        "	ldr	q6, [x7]					\n"
		        "	ldp	q7, q8, [x7, #16]				\n"
		        "	add  	x7, x7, x4, lsl #5 				\n"

		        "	ldr	q9, [x8]					\n"
		        "	ldp	q10, q11, [x8, #16]				\n"
		        "	add  	x8, x8, x4, lsl #5 				\n"

		        "	ldr	q12, [x9]					\n"
		        "	ldp	q13, q14, [x9, #16]				\n"
		        "	add  	x9, x9, x4, lsl #5 				\n"

		        "	ldr	q15, [x10]					\n"
		        "	ldp	q16, q17, [x10, #16]				\n"
		        "	add  	x10, x10, x4, lsl #5 				\n"

		        "	ldr	q18, [x11]					\n"
		        "	ldp	q19, q20, [x11, #16]				\n"
		        "	add  	x11, x11, x4, lsl #5 				\n"

		        "	ldr	q21, [x12]					\n"
		        "	ldp	q22, q23, [x12, #16]				\n"
		        "	add  	x12, x12, x4, lsl #5 				\n"	        


		        "	bgt	BEGIN_MEM_INNER_K 				\n"

		        "	add  	x0, x0, #48 					\n"
		        "	subs  	x21, x21, #1 					\n"
		        "	bgt 	BEGIN_MEM_N 					\n"
		        :
		        :
		        [A] "m" (A),
		        [N] "m" (N),
		        [LN] "m" (LN),
		        [kc] "m" (kc)
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

}

int main()
{
	float *A;
	
	A = ( float * ) malloc( NUM * 4800 * 240 * sizeof( float ) );

	double start = dclock();
	#pragma omp parallel num_threads(NUM)
	{
		int i;
		for (i = 0 ; i < 100; i++)
		{
			
			int id = omp_get_thread_num();
			mem_to_reg(&A[id * 240] , 4800, 240*NUM, 240);
		}

	}
	double cost = (dclock() - start) / 100;

	printf("cost = %lf ms\n", cost * 1000);
	free(A);
}