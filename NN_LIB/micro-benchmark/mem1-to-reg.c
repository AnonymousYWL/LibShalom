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

void mem_to_reg(float *A, long M, long K, long LM)
{

	int kk;
	long MK = M * 320;

	for(kk =0 ; kk < K; kk = kk + 320)
	{
		float *temp = A + LM * kk;
		asm volatile(


				"		ldr	x0, %[temp]								\n"
				"		ldr	x1, %[MK] 								\n"

				"		lsr			x2, x1, #6 						\n"

				"BEGIN_MEM:											\n"
				"		ldr	q0, [x0], #16 							\n"
				"		ldr	q1, [x0], #16 							\n"
				"		ldr	q2, [x0], #16 							\n"
				//"		prfm	PLDL1KEEP, [x0, #256]				\n"
				"		ldr	q3, [x0], #16 							\n"
				"		ldr	q4, [x0], #16 							\n"
				"		ldr	q5, [x0], #16 							\n"
				"		ldr	q6, [x0], #16 							\n"
				"		ldr	q7, [x0], #16 							\n"

				//"		prfm	PLDL1KEEP, [x0, #256]				\n"
				"		subs x2, x2, #1 							\n"
				"		ldr	q8, [x0], #16 							\n"
				"		ldr	q9, [x0], #16 							\n"
				"		ldr	q10, [x0], #16 							\n"
				"		ldr	q11, [x0], #16 							\n"
				"		ldr	q12, [x0], #16 							\n"
				"		ldr	q13, [x0], #16 							\n"
				"		ldr	q14, [x0], #16 							\n"
				"		ldr	q15, [x0], #16 							\n"

				"		bgt BEGIN_MEM 								\n"
				:
				:
				[temp] "m" (temp),
				[MK] "m" (MK)
				: "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8",
		  		"x9", "x10", "x11", "x12", "x13","x14", "x15", "x16",
		  		"x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24","x25",
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
	
	A = ( float * ) malloc( NUM * 240 * 4800 * sizeof( float ) ); 

	
	double start = dclock();
	#pragma omp parallel num_threads(NUM)
	{
		int i;
		for(i = 0 ; i < 100; i++)
		{
			int id = omp_get_thread_num();
			mem_to_reg(&A[id * 240 * 320] , 240 , 4800, 240 *NUM);
		}

	}
	double cost = (dclock() - start) / 100;

	printf("cost = %lf ms\n", cost * 1000);
	free(A);
}