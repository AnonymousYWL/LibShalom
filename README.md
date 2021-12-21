# LibShalom

LibShalom is a Library for Small Irregular-shaped Matrix Multiplications on ARMv8-based processors. It improves the performance of small and irregular-shaped GEMMs on ARMv8-based
processors by improving the shortcomings of existing BLAS libraries, such as packing accounts for a large portion of the runtime, inefficient edge case processing and unreasonable parallelization methods.

# Software dependences
- [GNU Compiler (GCC)](https://gcc.gnu.org/) (>=v8.2)
- [OpenMP](https://www.openmp.org/)

# hardware platform
Phytium 2000+, Kunpeng 920, ThunderX2 or otther ARMv8-based processors
![image](https://github.com/AnonymousYWL/MYLIB/blob/main/platforms.png)

# Compile and install
```bash
$ cd NN_LIB && make  
$ make install PREFIX= the installation path
```
These commands will copy LibShalom library and headers in the installation path PREFIX.

# Compiling with LibShalom
All LibShalom definitions and prototypes may be included in your C source file by including a single header file, ```LibShalom.h```:
```C
#include <stdio.h>
#include <stdlib.h>
#include "LibShalom.h"
```

# API
```LibShalom_sgemm(int transa, int transb, float *C, float *A, float *B, long M, long N, long K)```   // Interface of small SGEMM  
```LibShalom_sgemm_mp(int transa, int transb, float *C, float *A, float *B, long M, long N, long K)```   // Interface of irregular-shaped SGEMM  
```LibShalom_dgemm(int transa, int transb, double *C, double *A, double *B, long M, long N, long K)```   // Interface of small DGEMM  
```LibShalom_set_thread_nums(int num)```                  // Set the total number of threads

# Running Benchmark
The command
```bash
$ cd benchmark/small_SGEMM && make  
```
will compile the benchmark program of fp32 small GEMM to generate the executable file ```main```. By executing ```main```, the user can get the evaluation result of the matrices of sizes from 8x8x8 to 128x128x128.

# Getting Started
the following C code is focused on a specific functionality but may be considered as Hello LibShalom.
```C
#include <stdlib.h>
#include <stdlib.h>
#include "LibShalom.h"

int main()
{

	int i,j,k;
	int loop= 100;
	long M, N, K;
        M= N = K = 80;
        /* row-major */   	
	float *A = ( float * ) malloc( K* M * sizeof( float ) );
	float *B = ( float * ) malloc( K* N * sizeof( float ) );
	float *C = ( float * ) malloc( M* N * sizeof( float ) );

	double drand48();
	/* initialize input matrices A and B*/
	for ( i = 0; i < M; i++ )
	{
		for ( j = 0; j < K; j++ )
			A [i* K + j]= 2.0 * (float)drand48( ) - 1.0 ;
	}

	for ( i = 0; i < K; i++ )
	{
		for ( j = 0; j < N; j++ )
			B [i * K + j]= 2.0 * (float)drand48( ) - 1.0 ;
	}

	// warm up
	//perform C = A * B (B is transposed)
	for( i =0 ;i< 5; i++)
		LibShalom_sgemm(NoTrans, Trans, C, A, B, M, N, K);

	for( i= 0; i< loop ;i++)
		LibShalom_sgemm(NoTrans, Trans, C, A, B, M, N, K);


	free(A);
	free(B);
	free(C);
	return 0;
}
```
The makefile corresponding to this program:
```bash
LibShalom_PREFIX = $ path to install LibShalom 
LibShalom_INC    = $(LibShalom_PREFIX)/SMM/include
LibShalom_LIB    = $(LibShalom_PREFIX)/SMM/lib/libsmm.a 

OTHER_LIBS  =-fopenmp

CC          = g++
CFLAGS      = -O3 -I$(LibShalom_INC)
LINKER      = $(CC)

OBJS        = Hello.o

%.o: %.c
	 $(CC) $(CFLAGS) -c -fopenmp $< -o $@

all: $(OBJS)
	$(LINKER) $(OBJS) $(LibShalom_LIB) $(OTHER_LIBS) -o a.out
```
# Note
The matrices are stored in the row-major format in this library.
We will keep this library updated and maintained.
```
Contact: Weiling Yang (2954427597@qq.com)
