#include "PACK.h"


/*

author: Weiling Yang 2021
*/

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

void random_matrix( int m, int n, float *a)
{
  double drand48();
  int i,j;

  for ( i=0; i<m; i++ )
    for ( j=0; j<n; j++ )
    {
		a[i*n+j]= 2.0 * (float)drand48( ) - 1.0 + 0.000001 * (i+j);
    }
}

void transpose( int m, int n, float *a)
{
  float *temp_a = ( float * ) malloc( m* n * sizeof( float ) );

  int i, j;

  for( i =0 ;i< m; i++)
  {
    for( j= 0;j < n; j++)
    {
      temp_a[j * m + i] = a[i*n + j];
    }
  }


  for( i =0 ; i< n; i++)
  {
    for( j =0; j< m; j++)
    {
      a[ i * m+j] = temp_a[i*m+j];
    }
  }
}


void SMM(float *C, float *A, float *B, long	M, long N, long K, 
			long *temp, long LN, long LK, float *SB, long k_tag)
{
	asm volatile(

		".macro	PACK_KERNEL8x8_BEGIN_K					\n"


		"	ldr		q0,	[x11], #16						\n"

		"	ldr		q4, [x12], #16						\n"
		"	ldr		q5, [x13], #16						\n"

		"	fmul	v16.4s, v0.4s, v4.s[0]			 	\n"
		"	ldr		q6, [x14], #16					 	\n"
		"	fmul	v17.4s, v0.4s, v5.s[0]			 	\n"

		"	ldr		q7, [x15], #16					 	\n"
		"	fmul	v18.4s, v0.4s, v6.s[0]			 	\n"
		"	ldr		q8, [x16], #16					 	\n"
		"	fmul	v19.4s, v0.4s, v7.s[0]			 	\n"

		"	ldr		q9, [x17], #16						\n"
		"	fmul	v20.4s, v0.4s, v8.s[0]				\n"
		"	ldr		q10, [x18], #16						\n"
		"	fmul	v21.4s, v0.4s, v9.s[0]				\n"
		"	ldr		q11, [x19], #16						\n"
		"	fmul	v22.4s, v0.4s, v10.s[0]				\n"

		"	ldr		q1, [x11], #16						\n"
		"	prfm	PLDL1KEEP, [x11, #256]				\n"
		"	fmul	v23.4s, v0.4s, v11.s[0]				\n"

		"	ldp		q2, q3, [x11], #32				    \n"

		"	fmul	v24.4s, v1.4s, v4.s[0]			 	\n"
		"	fmul	v25.4s, v1.4s, v5.s[0]			 	\n"
		"	fmul	v26.4s, v1.4s, v6.s[0]			 	\n"
		"	fmul	v27.4s, v1.4s, v7.s[0]			 	\n"

		"	st4		{v4.s, v5.s, v6.s, v7.s}[0], [x24], #16		\n"

		"	fmul	v28.4s, v1.4s, v8.s[0]				\n"
		"	fmul	v29.4s, v1.4s, v9.s[0]				\n"
		"	fmul	v30.4s, v1.4s, v10.s[0]				\n"
		"	fmul	v31.4s, v1.4s, v11.s[0]				\n"

		"	st4		{v8.s, v9.s, v10.s, v11.s}[0], [x24], #16	\n"

		"	ldr		q12, [x12], #16						\n"


		".endm 										 	\n"



		".macro	PACK_KERNEL8x8_K0						\n"

		"	prfm	PLDL1KEEP, [x12, #64]				\n"

		"	ldr		q2, [x11], #16						\n"
		"	fmla	v16.4s, v0.4s, v4.s[0]			 	\n"
		"	fmla	v17.4s, v0.4s, v5.s[0]			 	\n"
		"	fmla	v18.4s, v0.4s, v6.s[0]			 	\n"
		"	fmla	v19.4s, v0.4s, v7.s[0]			 	\n"

		"	st4		{v4.s, v5.s, v6.s, v7.s}[0], [x24], #16			\n"

		"	fmla	v20.4s, v0.4s, v8.s[0]				\n"
		"	fmla	v21.4s, v0.4s, v9.s[0]				\n"
		"	fmla	v22.4s, v0.4s, v10.s[0]				\n"
		"	fmla	v23.4s, v0.4s, v11.s[0]				\n"

		"	prfm	PLDL1KEEP, [x13, #64]				\n"


		"	ldr		q3, [x11], #16						\n"
		"	fmla	v24.4s, v1.4s, v4.s[0]			 	\n"
		"	fmla	v25.4s, v1.4s, v5.s[0]			 	\n"
		"	fmla	v26.4s, v1.4s, v6.s[0]			 	\n"
		"	fmla	v27.4s, v1.4s, v7.s[0]			 	\n"

		"	st4		{v8.s, v9.s, v10.s, v11.s}[0], [x24], #16		\n"

		"	fmla	v28.4s, v1.4s, v8.s[0]				\n"
		"	fmla	v29.4s, v1.4s, v9.s[0]				\n"
		"	fmla	v30.4s, v1.4s, v10.s[0]				\n"
		"	fmla	v31.4s, v1.4s, v11.s[0]				\n"

		"	prfm	PLDL1KEEP, [x11, #512]				\n"

		"	ldr		q12, [x12], #16						\n"


		".endm 											\n"



		".macro	PACK_KERNEL8x8_K1						\n"


		"	prfm	PLDL1KEEP, [x14, #128]				\n"

		"	ldr		q0, [x11], #16						\n"
		"	fmla	v16.4s, v2.4s, v4.s[1]			 	\n"
		"	fmla	v17.4s, v2.4s, v5.s[1]			 	\n"
		"	fmla	v18.4s, v2.4s, v6.s[1]			 	\n"
		"	fmla	v19.4s, v2.4s, v7.s[1]			 	\n"

		"	st4		{v4.s, v5.s, v6.s, v7.s}[1], [x24], #16		\n"

		"	fmla	v20.4s, v2.4s, v8.s[1]				\n"
		"	fmla	v21.4s, v2.4s, v9.s[1]				\n"
		"	fmla	v22.4s, v2.4s, v10.s[1]				\n"
		"	fmla	v23.4s, v2.4s, v11.s[1]				\n"

		"	prfm	PLDL1KEEP, [x15, #64]				\n"


		"	ldr		q1, [x11], #16						\n"
		"	fmla	v24.4s, v3.4s, v4.s[1]			 	\n"
		"	fmla	v25.4s, v3.4s, v5.s[1]			 	\n"
		"	fmla	v26.4s, v3.4s, v6.s[1]			 	\n"
		"	fmla	v27.4s, v3.4s, v7.s[1]			 	\n"

		"	st4		{v8.s, v9.s, v10.s, v11.s}[1], [x24], #16	\n"

		"	fmla	v28.4s, v3.4s, v8.s[1]				\n"
		"	fmla	v29.4s, v3.4s, v9.s[1]				\n"
		"	fmla	v30.4s, v3.4s, v10.s[1]				\n"
		"	fmla	v31.4s, v3.4s, v11.s[1]				\n"

		"	ldr		q13, [x13], #16						\n"

		".endm 											\n"





		".macro	PACK_KERNEL8x8_K2						\n"

		"	prfm	PLDL1KEEP, [x16, #128]				\n"

		"	ldr		q2, [x11], #16						\n"
		"	fmla	v16.4s, v0.4s, v4.s[2]			 	\n"
		"	fmla	v17.4s, v0.4s, v5.s[2]			 	\n"
		"	fmla	v18.4s, v0.4s, v6.s[2]			 	\n"
		"	fmla	v19.4s, v0.4s, v7.s[2]			 	\n"

		"	st4		{v4.s, v5.s, v6.s, v7.s}[2], [x24], #16		\n"

		"	fmla	v20.4s, v0.4s, v8.s[2]				\n"
		"	fmla	v21.4s, v0.4s, v9.s[2]				\n"

		"	ldr		q14, [x14], #16						\n"

		"	fmla	v22.4s, v0.4s, v10.s[2]				\n"
		"	fmla	v23.4s, v0.4s, v11.s[2]				\n"


		"	prfm	PLDL1KEEP, [x17, #64]				\n"


		"	ldr		q3, [x11], #16						\n"
		"	fmla	v24.4s, v1.4s, v4.s[2]			 	\n"
		"	fmla	v25.4s, v1.4s, v5.s[2]			 	\n"

		"	ldr		q15, [x15], #16						\n"

		"	fmla	v26.4s, v1.4s, v6.s[2]			 	\n"
		"	fmla	v27.4s, v1.4s, v7.s[2]			 	\n"

		"	st4		{v8.s, v9.s, v10.s, v11.s}[2], [x24], #16		\n"

		"	fmla	v28.4s, v1.4s, v8.s[2]				\n"
		"	fmla	v29.4s, v1.4s, v9.s[2]				\n"
		"	fmla	v30.4s, v1.4s, v10.s[2]				\n"
		"	fmla	v31.4s, v1.4s, v11.s[2]				\n"


		".endm 											\n"



		".macro	PACK_KERNEL8x8_K3						\n"


		"	prfm	PLDL1KEEP, [x18, #128]				\n"

		"	st4		{v4.s, v5.s, v6.s, v7.s}[3], [x24], #16		\n"

		"	ldr		q0, [x11], #16						\n"
		"	fmla	v16.4s, v2.4s, v4.s[3]			 	\n"
		"	fmla	v24.4s, v3.4s, v4.s[3]			 	\n"
		"	ldr		q4, [x16], #16						\n"

		"	prfm	PLDL1KEEP, [x19, #64]				\n"

		"	fmla	v17.4s, v2.4s, v5.s[3]			 	\n"
		"	fmla	v25.4s, v3.4s, v5.s[3]			 	\n"
		"	ldr		q5, [x17], #16						\n"

		"	fmla	v18.4s, v2.4s, v6.s[3]			 	\n"
		"	fmla	v26.4s, v3.4s, v6.s[3]			 	\n"
		"	ldr		q6, [x18], #16						\n"

		"	fmla	v19.4s, v2.4s, v7.s[3]			 	\n"
		"	fmla	v27.4s, v3.4s, v7.s[3]			 	\n"
		"	ldr		q7, [x19], #16						\n"

		"	fmla	v20.4s, v2.4s, v8.s[3]				\n"
		"	fmla	v21.4s, v2.4s, v9.s[3]				\n"
		"	fmla	v22.4s, v2.4s, v10.s[3]				\n"
		"	fmla	v23.4s, v2.4s, v11.s[3]				\n"

		"	ldr		q1, [x11], #16						\n"

		"	fmla	v28.4s, v3.4s, v8.s[3]				\n"
		"	fmla	v29.4s, v3.4s, v9.s[3]				\n"
		"	fmla	v30.4s, v3.4s, v10.s[3]				\n"
		"	fmla	v31.4s, v3.4s, v11.s[3]				\n"

		"	st4		{v8.s, v9.s, v10.s, v11.s}[3], [x24], #16	\n"

		".endm 											\n"



		".macro	PACK_KERNEL8x8_K4						\n"


		"	prfm	PLDL1KEEP, [x12, #128]				\n"

		"	ldr		q2, [x11], #16						\n"
		"	fmla	v16.4s, v0.4s, v12.s[0]				\n"
		"	fmla	v17.4s, v0.4s, v13.s[0]				\n"

		"	st4		{v12.s, v13.s, v14.s, v15.s}[0], [x24], #16		\n"

		"	fmla	v18.4s, v0.4s, v14.s[0]				\n"
		"	fmla	v19.4s, v0.4s, v15.s[0]				\n"

		"	ldr		q8, [x16], #16						\n"

		"	fmla	v20.4s, v0.4s, v4.s[0]				\n"
		"	fmla	v21.4s, v0.4s, v5.s[0]				\n"
		"	fmla	v22.4s, v0.4s, v6.s[0]				\n"
		"	fmla	v23.4s, v0.4s, v7.s[0]				\n"

		"	ldr		q3, [x11], #16						\n"
		"	prfm	PLDL1KEEP, [x13, #128]				\n"

		"	fmla	v24.4s, v1.4s, v12.s[0]				\n"
		"	fmla	v25.4s, v1.4s, v13.s[0]				\n"
		"	fmla	v26.4s, v1.4s, v14.s[0]				\n"
		"	fmla	v27.4s, v1.4s, v15.s[0]				\n"

		"	ldr		q9, [x17], #16						\n"

		"	fmla	v28.4s, v1.4s, v4.s[0]				\n"
		"	fmla	v29.4s, v1.4s, v5.s[0]				\n"
		"	fmla	v30.4s, v1.4s, v6.s[0]				\n"
		"	fmla	v31.4s, v1.4s, v7.s[0]				\n"


		"	st4		{v4.s, v5.s, v6.s, v7.s}[0], [x24], #16		\n"
		"	prfm	PLDL1KEEP, [x11, #512]				\n"


		".endm 											\n"




		".macro	PACK_KERNEL8x8_K5						\n"


		"	prfm	PLDL1KEEP, [x14, #128]				\n"

		"	ldr		q0, [x11], #16						\n"
		"	fmla	v16.4s, v2.4s, v12.s[1]				\n"
		"	fmla	v17.4s, v2.4s, v13.s[1]				\n"

		"	st4		{v12.s, v13.s, v14.s, v15.s}[1], [x24], #16		\n"

		"	fmla	v18.4s, v2.4s, v14.s[1]				\n"
		"	fmla	v19.4s, v2.4s, v15.s[1]				\n"

		"	ldr		q10, [x18], #16						\n"

		"	fmla	v20.4s, v2.4s, v4.s[1]				\n"
		"	fmla	v21.4s, v2.4s, v5.s[1]				\n"
		"	fmla	v22.4s, v2.4s, v6.s[1]				\n"
		"	fmla	v23.4s, v2.4s, v7.s[1]				\n"

		"	ldr		q1, [x11], #16						\n"
		"	prfm	PLDL1KEEP, [x15, #128]				\n"

		"	fmla	v24.4s, v3.4s, v12.s[1]				\n"
		"	fmla	v25.4s, v3.4s, v13.s[1]				\n"
		"	fmla	v26.4s, v3.4s, v14.s[1]				\n"
		"	fmla	v27.4s, v3.4s, v15.s[1]				\n"

		"	fmla	v28.4s, v3.4s, v4.s[1]				\n"
		"	fmla	v29.4s, v3.4s, v5.s[1]				\n"
		"	fmla	v30.4s, v3.4s, v6.s[1]				\n"
		"	fmla	v31.4s, v3.4s, v7.s[1]				\n"

		"	st4		{v4.s, v5.s, v6.s, v7.s}[1], [x24], #16		\n"


		".endm 											\n"




		".macro	PACK_KERNEL8x8_K6						\n"


		"	prfm	PLDL1KEEP, [x16, #128]				\n"

		"	ldr		q2, [x11], #16						\n"
		"	fmla	v16.4s, v0.4s, v12.s[2]				\n"
		"	fmla	v17.4s, v0.4s, v13.s[2]				\n"

		"	st4		{v12.s, v13.s, v14.s, v15.s}[2], [x24], #16		\n"

		"	fmla	v18.4s, v0.4s, v14.s[2]				\n"
		"	fmla	v19.4s, v0.4s, v15.s[2]				\n"

		"	ldr		q11, [x19], #16						\n"

		"	fmla	v20.4s, v0.4s, v4.s[2]				\n"
		"	fmla	v21.4s, v0.4s, v5.s[2]				\n"
		"	fmla	v22.4s, v0.4s, v6.s[2]				\n"
		"	fmla	v23.4s, v0.4s, v7.s[2]				\n"

		"	ldr		q3, [x11], #16						\n"
		"	prfm	PLDL1KEEP, [x17, #128]				\n"

		"	st4		{v4.s, v5.s, v6.s, v7.s}[2], [x24], #16		\n"

		"	fmla	v24.4s, v1.4s, v12.s[2]				\n"
		"	fmla	v25.4s, v1.4s, v13.s[2]				\n"
		"	fmla	v26.4s, v1.4s, v14.s[2]				\n"
		"	fmla	v27.4s, v1.4s, v15.s[2]				\n"

		"	fmla	v28.4s, v1.4s, v4.s[2]				\n"
		"	fmla	v29.4s, v1.4s, v5.s[2]				\n"
		"	fmla	v30.4s, v1.4s, v6.s[2]				\n"
		"	fmla	v31.4s, v1.4s, v7.s[2]				\n"


		".endm 											\n"



		".macro	PACK_KERNEL8x8_K7						\n"


		"	prfm	PLDL1KEEP, [x18, #64]				\n"

		"	add 	x24, x24, #16						\n"
		"	ldr		q0, [x11], #16						\n"
		"	fmla	v20.4s, v2.4s, v4.s[3]				\n"
		"	fmla	v28.4s, v3.4s, v4.s[3]				\n"
		"	st4		{v4.s, v5.s, v6.s, v7.s}[3], [x24]			\n"
		"	ldr		q4, [x12], #16						\n"

		"	fmla	v29.4s, v3.4s, v5.s[3]				\n"
		"	fmla	v21.4s, v2.4s, v5.s[3]				\n"
		"	ldr		q5, [x13], #16						\n"

		"	fmla	v22.4s, v2.4s, v6.s[3]				\n"
		"	fmla	v30.4s, v3.4s, v6.s[3]				\n"
		"	ldr		q6, [x14], #16						\n"    

		"	fmla	v23.4s, v2.4s, v7.s[3]				\n"
		"	fmla	v31.4s, v3.4s, v7.s[3]				\n"
		"	ldr		q7, [x15], #16						\n"

		"	sub 	x24, x24, #16						\n"

		"	ldr		q1, [x11], #16						\n"
		"	prfm	PLDL1KEEP, [x19, #64]				\n"

		"	fmla	v16.4s, v2.4s, v12.s[3]				\n"
		"	fmla	v17.4s, v2.4s, v13.s[3]				\n"
		"	fmla	v18.4s, v2.4s, v14.s[3]				\n"
		"	fmla	v19.4s, v2.4s, v15.s[3]				\n"

		"	st4		{v12.s, v13.s, v14.s, v15.s}[3], [x24]		\n"

		"	fmla	v24.4s, v3.4s, v12.s[3]				\n"
		"	fmla	v25.4s, v3.4s, v13.s[3]				\n"
		"	fmla	v26.4s, v3.4s, v14.s[3]				\n"
		"	fmla	v27.4s, v3.4s, v15.s[3]				\n"

		"	add 	x24, x24, #32						\n"

		".endm 											\n"



		".macro	PACK_KERNEL8x8_END_K					\n"


		"	prfm	PLDL1KEEP, [x25, #32]			 	\n"

		"	fmla	v20.4s, v2.4s, v4.s[3]				\n"
		"	fmla	v28.4s, v3.4s, v4.s[3]				\n"
		"	fmla	v21.4s, v2.4s, v5.s[3]				\n"
		"	fmla	v29.4s, v3.4s, v5.s[3]				\n"

		"	st4		{v12.s, v13.s, v14.s, v15.s}[3], [x24], #16		\n"

		"	prfm	PLDL1KEEP, [x26, #32]			 	\n"

		"	fmla	v22.4s, v2.4s, v6.s[3]				\n"
		"	fmla	v30.4s, v3.4s, v6.s[3]				\n"
		"	fmla	v23.4s, v2.4s, v7.s[3]				\n"
		"	fmla	v31.4s, v3.4s, v7.s[3]				\n"

		"	fmla	v16.4s, v2.4s, v12.s[3]				\n"
		"	fmla	v17.4s, v2.4s, v13.s[3]				\n"
		"	fmla	v18.4s, v2.4s, v14.s[3]				\n"
		"	fmla	v19.4s, v2.4s, v15.s[3]				\n"

		"	st4		{v4.s, v5.s, v6.s, v7.s}[3], [x24], #16		\n"
		"	prfm	PLDL1KEEP, [x27, #32]			 	\n"
		"	fmla	v24.4s, v3.4s, v12.s[3]				\n"
		"	fmla	v25.4s, v3.4s, v13.s[3]				\n"
		"	fmla	v26.4s, v3.4s, v14.s[3]				\n"
		"	fmla	v27.4s, v3.4s, v15.s[3]				\n"


		"	prfm	PLDL1KEEP, [x28, #32]			    \n"



		".endm 											\n"



		".macro	PACK_SAVE0								\n"

		" 	subs  x21, x21, #1                			\n"

		"	st4		{v16.s, v17.s, v18.s, v19.s}[0], [x25], #16	    \n"
		"	st4		{v20.s, v21.s, v22.s, v23.s}[0], [x25]			\n"

		"	add 	x2, x2, x6, lsl #5					\n"

		"	st4		{v16.s, v17.s, v18.s, v19.s}[1], [x26], #16		\n"
		"	st4		{v20.s, v21.s, v22.s, v23.s}[1], [x26]		    \n"
		"	sub 	x25, x25, #16									\n"
		"	st4		{v16.s, v17.s, v18.s, v19.s}[2], [x27], #16		\n"
		"	st4		{v20.s, v21.s, v22.s, v23.s}[2], [x27]		    \n"
		"	sub 	x26, x26, #16									\n"
		"	st4		{v16.s, v17.s, v18.s, v19.s}[3], [x28], #16	    \n"
		"	st4		{v20.s, v21.s, v22.s, v23.s}[3], [x28]		    \n"
		"	sub 	x27, x27, #16									\n"
		"	sub 	x28, x28, #16									\n"

		"	add		x25, x25, x9, lsl #4				\n"
		"	add		x26, x26, x9, lsl #4				\n"
		"	add		x27, x27, x9, lsl #4				\n"
		"	add		x28, x28, x9, lsl #4				\n"

		"	st4		{v24.s, v25.s, v26.s, v27.s}[0], [x25], #16		\n"
		"	st4		{v28.s, v29.s, v30.s, v31.s}[0], [x25]		    \n"
		"	st4		{v24.s, v25.s, v26.s, v27.s}[1], [x26], #16		\n"
		"	st4		{v28.s, v29.s, v30.s, v31.s}[1], [x26]	        \n"
		"	sub 	x25, x25, #16									\n"
		"	st4		{v24.s, v25.s, v26.s, v27.s}[2], [x27], #16		\n"
		"	st4		{v28.s, v29.s, v30.s, v31.s}[2], [x27]		    \n"
		"	sub 	x26, x26, #16									\n"
		"	st4		{v24.s, v25.s, v26.s, v27.s}[3], [x28], #16		\n"
		"	st4		{v28.s, v29.s, v30.s, v31.s}[3], [x28]			\n"
		"	sub 	x27, x27, #16									\n"
		"	sub 	x28, x28, #16									\n"

		"	add		x25, x25, x9, lsl #4				\n"
		"	add		x26, x26, x9, lsl #4				\n"
		"	add		x27, x27, x9, lsl #4				\n"
		"	add		x28, x28, x9, lsl #4				\n"


		".endm	 										\n"



		".macro	PACK_SAVE								\n"

		"	sub 	sp, sp ,#256						\n"
		"	mov 	x22, sp								\n"

		"	prfm	PLDL1KEEP, [x25, #64]				\n"
		"	prfm	PLDL1KEEP, [x26, #64]				\n"

		"	st4		{v16.s, v17.s, v18.s, v19.s}[0], [sp], #16	    \n"
		"	st4		{v20.s, v21.s, v22.s, v23.s}[0], [sp], #16		\n"
		"	st4		{v16.s, v17.s, v18.s, v19.s}[1], [sp], #16		\n"
		"	st4		{v20.s, v21.s, v22.s, v23.s}[1], [sp], #16		\n"
		"	st4		{v16.s, v17.s, v18.s, v19.s}[2], [sp], #16		\n"
		"	st4		{v20.s, v21.s, v22.s, v23.s}[2], [sp], #16		\n"
		"	st4		{v16.s, v17.s, v18.s, v19.s}[3], [sp], #16		\n"
		"	st4		{v20.s, v21.s, v22.s, v23.s}[3], [sp], #16		\n"

		"	add 	x2, x2, x6, lsl #5					\n"

		"	ldp		q0, q1, [x22], #32					\n"
		"	ldp		q2, q3, [x22], #32					\n"
		"	ldp		q4, q5, [x22], #32					\n"
		"	ldp		q6, q7, [x22], #32					\n"


		"	prfm	PLDL1KEEP, [x27, #64]				\n"
		"	prfm	PLDL1KEEP, [x28, #64]				\n"


		"	st4		{v24.s, v25.s, v26.s, v27.s}[0], [sp], #16		\n"
		"	st4		{v28.s, v29.s, v30.s, v31.s}[0], [sp], #16		\n"
		"	st4		{v24.s, v25.s, v26.s, v27.s}[1], [sp], #16		\n"
		"	st4		{v28.s, v29.s, v30.s, v31.s}[1], [sp], #16		\n"

		"	ldp		q16, q17, [x25]						\n"
		"	ldp		q18, q19, [x26]						\n"

		"	st4		{v24.s, v25.s, v26.s, v27.s}[2], [sp], #16		\n"
		"	st4		{v28.s, v29.s, v30.s, v31.s}[2], [sp], #16		\n"
		"	st4		{v24.s, v25.s, v26.s, v27.s}[3], [sp], #16		\n"
		"	st4		{v28.s, v29.s, v30.s, v31.s}[3], [sp], #16		\n"

		"	ldp		q20, q21, [x27]						\n"
		"	ldp		q22, q23, [x28]						\n"


		"	fadd	v0.4s, v0.4s, v16.4s 				\n"
		"	ldp		q8, q9, [x22], #32					\n"		
		"	fadd	v1.4s, v1.4s, v17.4s 				\n"
		"	stp		q0, q1, [x25]						\n"

		"	ldp		q10, q11, [x22], #32				\n"		
		"	fadd	v2.4s, v2.4s, v18.4s 				\n"
		"	fadd	v3.4s, v3.4s, v19.4s 				\n"
		"	stp		q2, q3, [x26]						\n"
		"	add 	x25, x25, x9, lsl #4				\n"
		"	prfm	PLDL1KEEP, [x25, #64]				\n"

		"	ldp		q12, q13, [x22], #32				\n"		
		"	fadd	v4.4s, v4.4s, v20.4s 				\n"
		"	ldp		q14, q15, [x22], #32				\n"		
		"	fadd	v5.4s, v5.4s, v21.4s 				\n"
		"	stp		q4, q5, [x27]						\n"
		"	add 	x26, x26, x9, lsl #4				\n"
		"	prfm	PLDL1KEEP, [x26, #64]				\n"

		"	ldp		q24, q25, [x25]						\n"

		"	fadd	v6.4s, v6.4s, v22.4s 				\n"
		"	fadd	v7.4s, v7.4s, v23.4s 				\n"
		"	stp		q6, q7, [x28]						\n"
		"	add 	x27, x27, x9, lsl #4				\n"
		"	prfm	PLDL1KEEP, [x27, #64]				\n"

		"	ldp		q26, q27, [x26]						\n"

		"	add 	x28, x28, x9, lsl #4				\n"
		"	prfm	PLDL1KEEP, [x28, #64]				\n"


		"	fadd	v8.4s, v8.4s, v24.4s 				\n"
		"	fadd	v9.4s, v9.4s, v25.4s 				\n"
		"	ldp		q28, q29, [x27]						\n"

		"	fadd	v10.4s, v10.4s, v26.4s 				\n"
		"	fadd	v11.4s, v11.4s, v27.4s 				\n"
		"	ldp		q30, q31, [x28]						\n"
		"	stp		q8, q9, [x25] 						\n"	

		"	fadd	v12.4s, v12.4s, v28.4s 				\n"
		"	fadd	v13.4s, v13.4s, v29.4s 				\n"		
		"	stp		q10, q11, [x26] 					\n"

		" 	subs  x21, x21, #1                			\n"

		"	add		x25, x25, x9, lsl #4				\n"
		"	prfm	PLDL2KEEP, [x25, #64]				\n"
		"	add		x26, x26, x9, lsl #4				\n"
		"	prfm	PLDL2KEEP, [x25, #64]				\n"

		"	fadd	v14.4s, v14.4s, v30.4s 				\n"
		"	stp		q12, q13, [x27] 					\n"
		"	add		x27, x27, x9, lsl #4				\n"
		"	prfm	PLDL2KEEP, [x27, #64]				\n"
		"	fadd	v15.4s, v15.4s, v31.4s 				\n"
		"	stp		q14, q15, [x28] 					\n"
		"	add		x28, x28, x9, lsl #4				\n"
		"	prfm	PLDL2KEEP, [x28, #64]				\n"

		".endm	 										\n"



		".macro	KERNEL8x8_BEGIN_K						\n"


		"	prfm	PLDL1KEEP, [x11, #1024]				\n"
		"	ldr		q0,	[x11], #16						\n"

		"	ldr		q4, [x24], #16						\n"
		"	ldr		q5, [x24], #16						\n"

		"	fmul	v16.4s, v4.4s, v0.s[0]			 	\n"
		"	fmul	v17.4s, v5.4s, v0.s[0]			 	\n"

		"	ldr		q1,	[x11], #16						\n"

		"	fmul	v18.4s, v4.4s, v0.s[1]			 	\n"
		"	fmul	v19.4s, v5.4s, v0.s[1]			 	\n"

		"	ldr		q2,	[x11], #16						\n"

		"	fmul	v20.4s, v4.4s, v0.s[2]			 	\n"
		"	fmul	v21.4s, v5.4s, v0.s[2]			 	\n"

		"	ldr		q6,	[x24], #16						\n"

		"	fmul	v22.4s, v4.4s, v0.s[3]			 	\n"
		"	fmul	v23.4s, v5.4s, v0.s[3]			 	\n"


		"	ldr		q3,	[x11], #16						\n"
		"	fmul	v24.4s, v4.4s, v1.s[0]			 	\n"
		"	fmul	v25.4s, v5.4s, v1.s[0]			 	\n"

		"	ldr		q7,	[x24], #16						\n"

		"	fmul	v26.4s, v4.4s, v1.s[1]			 	\n"
		"	fmul	v27.4s, v5.4s, v1.s[1]			 	\n"

		"	prfm	PLDL1KEEP, [x24, #256]				\n"

		"	fmul	v28.4s, v4.4s, v1.s[2]			 	\n"
		"	fmul	v29.4s, v5.4s, v1.s[2]			 	\n"

		"	fmul	v30.4s, v4.4s, v1.s[3]			 	\n"
		"	fmul	v31.4s, v5.4s, v1.s[3]			 	\n"


		".endm 										 	\n"



		".macro	KERNEL8x8_K0							\n"


		"	prfm	PLDL1KEEP, [x11, #1024]				\n"

		"	ldr		q2, [x11], #16						\n"

		"	fmla	v16.4s, v4.4s, v0.s[0]			 	\n"
		"	fmla	v17.4s, v5.4s, v0.s[0]			 	\n"
		"	ldr		q6, [x24], #16						\n"

		"	fmla	v18.4s, v4.4s, v0.s[1]			 	\n"
		"	fmla	v19.4s, v5.4s, v0.s[1]			 	\n"
		"	ldr		q3, [x11], #16						\n"

		"	fmla	v20.4s, v4.4s, v0.s[2]			 	\n"
		"	fmla	v21.4s, v5.4s, v0.s[2]			 	\n"

		"	ldr		q7, [x24], #16						\n"

		"	fmla	v22.4s, v4.4s, v0.s[3]			 	\n"
		"	fmla	v23.4s, v5.4s, v0.s[3]			 	\n"

		"	prfm	PLDL1KEEP, [x11, #1024]				\n"

		"	fmla	v24.4s, v4.4s, v1.s[0]			 	\n"
		"	fmla	v25.4s, v5.4s, v1.s[0]			 	\n"
		"	fmla	v26.4s, v4.4s, v1.s[1]			 	\n"
		"	fmla	v27.4s, v5.4s, v1.s[1]			 	\n"
		"	fmla	v28.4s, v4.4s, v1.s[2]			 	\n"
		"	fmla	v29.4s, v5.4s, v1.s[2]			 	\n"
		"	fmla	v30.4s, v4.4s, v1.s[3]			 	\n"
		"	fmla	v31.4s, v5.4s, v1.s[3]			 	\n"


		".endm 										 	\n"


		".macro	KERNEL8x8_K1							\n"


		"	ldr		q0, [x11], #16						\n"
		"	fmla	v16.4s, v6.4s, v2.s[0]			 	\n"
		"	fmla	v17.4s, v7.4s, v2.s[0]			 	\n"
		"	ldr		q4, [x24], #16						\n"

		"	fmla	v18.4s, v6.4s, v2.s[1]			 	\n"
		"	fmla	v19.4s, v7.4s, v2.s[1]			 	\n"
		"	ldr		q1, [x11], #16						\n"

		"	fmla	v20.4s, v6.4s, v2.s[2]			 	\n"
		"	fmla	v21.4s, v7.4s, v2.s[2]			 	\n"
		"	ldr		q5, [x24], #16						\n"

		"	fmla	v22.4s, v6.4s, v2.s[3]			 	\n"
		"	fmla	v23.4s, v7.4s, v2.s[3]			 	\n"

		"	prfm	PLDL1KEEP, [x24, #256]				\n"

		"	fmla	v24.4s, v6.4s, v3.s[0]			 	\n"
		"	fmla	v25.4s, v7.4s, v3.s[0]			 	\n"
		"	fmla	v26.4s, v6.4s, v3.s[1]			 	\n"
		"	fmla	v27.4s, v7.4s, v3.s[1]			 	\n"
		"	fmla	v28.4s, v6.4s, v3.s[2]			 	\n"
		"	fmla	v29.4s, v7.4s, v3.s[2]			 	\n"
		"	fmla	v30.4s, v6.4s, v3.s[3]			 	\n"
		"	fmla	v31.4s, v7.4s, v3.s[3]			 	\n"


		".endm 										 	\n"



		".macro	KERNEL8x8_END_K							\n"


      	" 	prfm  PLDL1KEEP, [x25, #32]      			\n"
      	" 	prfm  PLDL1KEEP, [x26, #32]      			\n"
      	" 	prfm  PLDL1KEEP, [x27, #32]      			\n"
      	" 	prfm  PLDL1KEEP, [x28, #32]      			\n"

		"	fmla	v16.4s, v6.4s, v2.s[0]			 	\n"
		"	fmla	v17.4s, v7.4s, v2.s[0]			 	\n"
		"	fmla	v18.4s, v6.4s, v2.s[1]			 	\n"
		"	fmla	v19.4s, v7.4s, v2.s[1]			 	\n"
		"	fmla	v20.4s, v6.4s, v2.s[2]			 	\n"
		" 	prfm  PLDL1KEEP, [x15, #32]      			\n"
      	" 	prfm  PLDL1KEEP, [x16, #32]      			\n"
		"	fmla	v21.4s, v7.4s, v2.s[2]			 	\n"
		"	fmla	v22.4s, v6.4s, v2.s[3]			 	\n"
		" 	prfm  PLDL1KEEP, [x17, #32]      			\n"
      	" 	prfm  PLDL1KEEP, [x18, #32]      			\n"
		"	fmla	v23.4s, v7.4s, v2.s[3]			 	\n"

		"	fmla	v24.4s, v6.4s, v3.s[0]			 	\n"
		"	fmla	v25.4s, v7.4s, v3.s[0]			 	\n"
		"	fmla	v26.4s, v6.4s, v3.s[1]			 	\n"
		"	fmla	v27.4s, v7.4s, v3.s[1]			 	\n"
		"	fmla	v28.4s, v6.4s, v3.s[2]			 	\n"
		"	fmla	v29.4s, v7.4s, v3.s[2]			 	\n"
		"	fmla	v30.4s, v6.4s, v3.s[3]			 	\n"
		"	fmla	v31.4s, v7.4s, v3.s[3]			 	\n"

		".endm 										 	\n"


		".macro	M8N8_ADD_C								\n"

		"	ldp		q0, q1, [x25]						\n"
		"	fadd	v16.4s, v16.4s, v0.4s				\n"
		"	ldp		q2, q3, [x26]						\n"
		"	fadd	v17.4s, v17.4s, v1.4s				\n"
		"	ldp		q4, q5, [x27]						\n"
		"	fadd	v18.4s, v18.4s, v2.4s				\n"
		"	ldp		q6, q7, [x28]						\n"
		"	fadd	v19.4s, v19.4s, v3.4s				\n"

		"	ldp		q8, q9, [x15]						\n"
		"	fadd	v20.4s, v20.4s, v4.4s				\n"
		"	ldp		q10, q11, [x16]						\n"
		"	fadd	v21.4s, v21.4s, v5.4s				\n"
		"	ldp		q12, q13, [x17]						\n"
		"	fadd	v22.4s, v22.4s, v6.4s				\n"
		"	ldp		q14, q15, [x18]						\n"
		"	fadd	v23.4s, v23.4s, v7.4s				\n"


		"	fadd	v24.4s, v24.4s, v8.4s				\n"
		"	fadd	v25.4s, v25.4s, v9.4s				\n"
		"	fadd	v26.4s, v26.4s, v10.4s				\n"
		"	fadd	v27.4s, v27.4s, v11.4s				\n"
		"	fadd	v28.4s, v28.4s, v12.4s				\n"
		"	fadd	v29.4s, v29.4s, v13.4s				\n"
		"	fadd	v30.4s, v30.4s, v14.4s				\n"
		"	fadd	v31.4s, v31.4s, v15.4s				\n"


		".endm											\n"


		".macro	SAVE8x8									\n"

		"	subs	x21, x21, #1					 	\n"

		"	stp		q16, q17, [x25]			 		 	\n"
		"	add 	x25, x25, x9, lsl #5				\n"
		"	prfm	PLDL2KEEP, [x25, #32]				\n"
		"	stp		q18, q19, [x26]			 	     	\n"
		"	add 	x26, x26, x9, lsl #5				\n"
		"	prfm	PLDL2KEEP, [x26, #32]				\n"
		"	stp		q20, q21, [x27]					 	\n"
		"	add 	x27, x27, x9, lsl #5				\n"
		"	prfm	PLDL2KEEP, [x27, #32]				\n"
		"	stp		q22, q23, [x28]					 	\n"
		"	add 	x28, x28, x9, lsl #5				\n"
		"	prfm	PLDL2KEEP, [x28, #32]				\n"

		"	stp		q24, q25, [x15]			 		 	\n"
		"	add 	x15, x15, x9, lsl #5				\n"
		"	prfm	PLDL2KEEP, [x15, #32]				\n"
		"	stp		q26, q27, [x16]			 	     	\n"
		"	add 	x16, x16, x9, lsl #5				\n"
		"	prfm	PLDL2KEEP, [x16, #32]				\n"
		"	stp		q28, q29, [x17]					 	\n"
		"	add 	x17, x17, x9, lsl #5				\n"
		"	prfm	PLDL2KEEP, [x17, #32]				\n"
		"	stp		q30, q31, [x18]					 	\n"
		"	add 	x18, x18, x9, lsl #5				\n"
		"	prfm	PLDL2KEEP, [x18, #32]				\n"

		".endm										 	\n"



		//----------------------------------------------------

		"SMM:									 	 \n"

		"	ldr		x0, %[C]						 \n"
		"	ldr		x1, %[A]						 \n"
		"	ldr		x2, %[B]						 \n"

		"	ldr		x3, %[M]						 \n"
		"	ldr		x4, %[N]						 \n"
		"	ldr		x5, %[K]						 \n"
		"	ldr		x7, %[temp]						 \n"
		"	ldr		x9, %[LN] 						 \n"
		"	ldr		x6, %[LK]						 \n"

		"   prfm    PLDL1KEEP, [x1, #512]            \n"
		"   prfm    PLDL1KEEP, [x2, #128]            \n"

		"	ldr		x10, %[SB]						 \n"
		"	ldr		x8, %[k_tag] 					 \n"

		"	lsr		x20, x4, #3						 \n"

		"BEGIN_N8:								 	 \n"

		"	mov 	x25, x0   						 \n"   //C1*
		"	add		x26, x25, x9, lsl #2 			 \n"   //C2*
		"	add 	x27, x25, x9, lsl #3 			 \n"   //C3*
		" 	add 	x28, x26, x9, lsl #3 			 \n"   //C4*

		"	mov		x11, x1   						 \n"
		"	lsr		x21, x3, #3						 \n"

		"BEGIN_PACKB:								 \n"

		"	mov		x24, x10						 \n"   //还原SB的地址

		"	mov		x12, x2						 	 \n"   //B0*
		"	add		x13, x12, x6, lsl #2 			 \n"
		"	add		x14, x12, x6, lsl #3 			 \n"
		"	add		x15, x13, x6, lsl #3 			 \n"

		"	prfm    PLDL1KEEP, [x12, #64]			 \n"
		"	prfm    PLDL1KEEP, [x13, #64]			 \n"
		"	prfm    PLDL1KEEP, [x14, #64]			 \n"
		"	prfm    PLDL1KEEP, [x15, #64]			 \n"

		"	add		x16, x14, x6, lsl #3 			 \n"
		"	add		x17, x15, x6, lsl #3 			 \n"
		"	add		x18, x16, x6, lsl #3 			 \n"
		"	add		x19, x17, x6, lsl #3 			 \n"

		"	prfm    PLDL1KEEP, [x16, #64]			 \n"
		"	prfm    PLDL1KEEP, [x17, #64]			 \n"
		"	prfm    PLDL1KEEP, [x18, #64]			 \n"
		"	prfm    PLDL1KEEP, [x19, #64]			 \n"

		"	prfm    PLDL1KEEP, [x11, #256]			 \n"


		"PACK_Body_K:								 \n"

		"	lsr		x22, x5, #3 					 \n"     // K / 8

		"	PACK_KERNEL8x8_BEGIN_K				 	 \n"

		"	subs	x22, x22, #1		     		 \n"		
		"	b 		PACK_K1_7						 \n"

		"PACK_K:									 \n"
		
		"	PACK_KERNEL8x8_K0						 \n"
		"PACK_K1_7:									 \n"

		"	PACK_KERNEL8x8_K1						 \n"
		"	PACK_KERNEL8x8_K2						 \n"
		"	PACK_KERNEL8x8_K3						 \n"
		"	PACK_KERNEL8x8_K4						 \n"
		"	PACK_KERNEL8x8_K5						 \n"
		"	PACK_KERNEL8x8_K6						 \n"

		"	beq		PACK_Edge_K						 \n"

		"	PACK_KERNEL8x8_K7						 \n"
		
		"	subs	x22, x22, #1			 		 \n"
		"	b 		PACK_K			 				 \n"	

		"PACK_Edge_K:							     \n"


		"	PACK_KERNEL8x8_END_K			         \n"


		"	cmp		x8, #0							 \n"
		"	beq		M8N8_PACK_SAVE 					 \n"
		"	PACK_SAVE 								 \n"
		"	b 	END_PACKB						 	 \n"
		"M8N8_PACK_SAVE: 							 \n"
		"	PACK_SAVE0								 \n"

		"END_PACKB:									 \n"

		"	beq		M8_END							 \n"

		"	add		x15, x25, x9, lsl #4			 \n"
		"	add		x16, x26, x9, lsl #4			 \n"
		"	add		x17, x27, x9, lsl #4			 \n"
		"	add		x18, x28, x9, lsl #4			 \n"




		//----------------------------------------------------------
        "BEGIN_M8:                          		 \n" 

        " mov     x24, x10                    		 \n"
        " prfm    PLDL1KEEP, [x24, #256]    		 \n"
        " prfm    PLDL1KEEP, [x11, #256]    		 \n"

        "Body_K:                            		 \n"

        " lsr   x22, x5, #3                 		 \n"     // K / 8
        " KERNEL8x8_BEGIN_K                		 	 \n"
        " subs  x22, x22, #1                		 \n"  
        " b 	K1_7                            	 \n"

        "Main_K:                            		 \n"
    
      	" KERNEL8x8_K0                     		 	 \n"
      	"K1_7:                              		 \n"
      	
      	" KERNEL8x8_K1                     		 	 \n"
      	" KERNEL8x8_K0                     		 	 \n"
     	" KERNEL8x8_K1                     		 		 \n"
      	" KERNEL8x8_K0                     		 	 \n"
      	" KERNEL8x8_K1                     		 	 \n"
      	" KERNEL8x8_K0                     			 \n"

      	" beq   Edge_K                      		 \n"

      	" KERNEL8x8_K1                     		 	 \n"
      
      	" subs  x22, x22, #1                		 \n"
      	" b     Main_K                      		 \n"  

      	"Edge_K:                            		 \n"

      	" KERNEL8x8_END_K                 			 \n"  

     	"	cmp		x8, #0							 							\n"
		"	beq		M8N8_SAVE 					 	 							\n"
		"	M8N8_ADD_C 							 	 								\n"
		"M8N8_SAVE: 							     							\n"
		"	SAVE8x8								 	 									\n"

      	" bgt     BEGIN_M8                			\n"
		"M8_END:									 									\n"



		"	subs	x20, x20, #1					 							\n"
		"	add		x0, x0, #32						 							\n"
		"	bgt		BEGIN_N8						 								\n"

		:
		:	
			[C] "m" (C),
			[A] "m" (A),
      [B] "m" (B), 
      [M] "m" (M),
      [N] "m" (N),
      [K] "m" (K),
      [temp] "m" (temp),
      [LN] "m" (LN),
      [LK] "m" (LK),
      [SB] "m" (SB),
      [k_tag] "m" (k_tag)
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


void Small_SGEMM(float *C, float *A, float *B, long M, long N, 
					long K, long *temp, long LD)
{

	long LK = K;
  void *ptr;
  int flag =0 ;
  float *Ac = ( float * ) malloc( K * M * sizeof( float ) );
  posix_memalign(&ptr, 64, NUM * GEMM_K * 8 *sizeof( float ));
  float *SSB = (float *)ptr;

  long Num_K_block = K / GEMM_K;
	long Edge_K = K % GEMM_K;
	long Num_M_block = M / 8;
	long Num_blocks0 = Num_K_block * Num_M_block;
	long Num_blocks = Num_blocks0; 
	if(Edge_K > 0)
	{
		Num_blocks = Num_blocks0 + Num_M_block;
	}

  #pragma omp parallel num_threads(NUM)
	{

		int i, j;
		int id = omp_get_thread_num();

		//double start = dclock();
		for( i = id; i < Num_blocks; i = i + NUM)
		{


			long start_M = (i % Num_M_block) * 8;					// note
			long start_K = (i / Num_M_block) * GEMM_K;		// note

			float *AA = A + start_M * K + start_K;        // address
			float *AAc; 

			if( Edge_K > 0 && i >= Num_blocks0)
			{
				AAc = Ac + start_K * M + start_M * Edge_K;  // note
				PACKA(AA, AAc, 8, Edge_K, K);
			}
			else
			{
				AAc = Ac + start_K * M + start_M * GEMM_K;  // note
				PACKA(AA, AAc, 8, GEMM_K, K);
			}
		}

		#pragma omp barrier
		// finish packing matrix A


		long ii,jj,kk,iis, jjs, kks, n_to, m_to, k_to;
		long nb = N / NUM;
		long nc, mc, kc;
		jjs = id * nb;
		n_to = jjs + nb;
		iis = 0;
		m_to = M;
		kks = 0;
		k_to = K;


		for( jj = jjs; jj < n_to; jj= jj + nc)
		{

				nc = GEMM_N;
    		if(n_to - jj < GEMM_N)
    		{
    			nc = n_to - jj;
    		}

    		for( ii = iis ; ii < m_to; ii = ii + mc)
				{
					mc = GEMM_M;
					if(m_to - ii < GEMM_M)
					{
						mc = m_to - ii;
					}

					float *CC = C + ii * N + jj;

					for(kk= kks; kk < k_to; kk = kk + kc)
	    		{
	    			kc = GEMM_K;
	    			if(k_to - kk < GEMM_K)
						kc = k_to - kk;

						float *AAc = Ac + kk * mc;
						float *BB = B + jj * K + kk;

						SMM(CC, AAc, BB, mc, nc, kc, temp, N, K, &SSB[id * GEMM_K * 8], kk);
					}


			}
		}
		
	}


	free(SSB);
	free(Ac);
}



int main()
{

	int i,j,k;
	int loop=5;
	long M, N, K;
	double start, cost;
	double gflops;
	long lda,ldb,ldc;
	int flag =0 ;
	long temp =-1;
	float *A, *B, *C;

  M = 128;
  N = 4096;
  K = 4096;

	A = ( float * ) malloc( K* M * sizeof( float ) );
	B = ( float * ) malloc( K* N * sizeof( float ) );
	C = ( float * ) malloc( M* N * sizeof( float ) );

	double ops = (double)M *N *K * 1.0e-09 * 2;

	random_matrix(M,K,A);
	random_matrix(K,N,B);
	transpose(K, N, B);

	for( i =0 ;i< 2; i++)
		Small_SGEMM(C, A, B, M, N, K, &temp, N);

	start = dclock();
	for( i= 0; i< loop ;i++)
		Small_SGEMM(C, A, B, M, N, K, &temp, N);
	cost =(dclock()-start)/loop; 

	printf("\nLibShalom:  M= %d N=%d K=%d cost = %lfms flops = %lf\n", 
		M, N, K, cost * 1000, ops/cost);


	free(A);
	free(B);
	free(C);

  return 0;

}