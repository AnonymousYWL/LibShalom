objects= LibShalom_dgemm.o test_temp_L1.o test_temp.o NN_SMM.o test_SMM_NT.o SMM_thread.o SMM_NT_thread.o DGEMM_NN.o

main: $(objects)
	g++ $(objects) -O3 -fopenmp  -o main

LibShalom_dgemm.o:LibShalom_dgemm.c
	g++ -c -O3 LibShalom_dgemm.c -o LibShalom_dgemm.o
test_temp_L1.o:../../NN_LIB/test_temp_L1.c
	g++ -c -O3 ../../NN_LIB/test_temp_L1.c -o test_temp_L1.o
test_temp.o:../../NN_LIB/test_temp.c 
	g++ -c -O3 ../../NN_LIB/test_temp.c -o test_temp.o
NN_SMM.o:../../NN_LIB/NN_SMM.c
	g++ -c -O3 ../../NN_LIB/NN_SMM.c -o NN_SMM.o
test_SMM_NT.o:../../NN_LIB/test_SMM_NT.c
	g++ -c -O3 ../../NN_LIB/test_SMM_NT.c -o test_SMM_NT.o
SMM_thread.o:../../NN_LIB/SMM_thread.c
	g++ -c  -O3 -fopenmp ../../NN_LIB/SMM_thread.c -o SMM_thread.o
SMM_NT_thread.o:../../NN_LIB/SMM_NT_thread.c
	g++ -c -O3 -fopenmp ../../NN_LIB/SMM_NT_thread.c -o SMM_NT_thread.o 
DGEMM_NN.o:../../NN_LIB/DGEMM_NN.c
	g++ -c -O3 ../../NN_LIB/DGEMM_NN.c -o DGEMM_NN.o

.PHONY:clean
clean:
	rm -f *.o *.a *.so
