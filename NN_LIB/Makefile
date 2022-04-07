objects= test_temp_L1.o test_temp.o NN_SMM.o test_SMM_NT.o SMM_m4n17.o \
SMM_thread.o SMM_NT_thread.o DGEMM_NN.o SMM_NN_new.o PACK.o

#main: $(objects)
#	g++ $(objects) -fopenmp -lopenblas -O3 -o main

all:$(objects)
	g++ $(objects) -shared -fPIC -fopenmp -O3 -o libsmm.so
	ar rcs libsmm.a $(objects)

#bench.o:benchmark.c
#	g++ -c -fPIC benchmark.c -o bench.o
test_temp_L1.o:test_temp_L1.c
	g++ -c -fPIC test_temp_L1.c -o test_temp_L1.o
test_temp.o:test_temp.c 
	g++ -c -fPIC test_temp.c -o test_temp.o
NN_SMM.o:NN_SMM.c
	g++ -c -fPIC NN_SMM.c -o NN_SMM.o
test_SMM_NT.o:test_SMM_NT.c
	g++ -c -fPIC test_SMM_NT.c -o test_SMM_NT.o
SMM_thread.o:SMM_thread.c
	g++ -c -fPIC -fopenmp SMM_thread.c -o SMM_thread.o
SMM_m4n17.o:SMM_m4n17.c
	g++ -c -fPIC -fopenmp SMM_m4n17.c -o SMM_m4n17.o
SMM_NT_thread.o:SMM_NT_thread.c
ifeq ($(TARGET), KP920)
	g++ -c -fPIC -fopenmp -D XXX SMM_NT_thread.c -o SMM_NT_thread.o 
else
	g++ -c -fPIC -fopenmp -D XXX SMM_NT_thread.c -o SMM_NT_thread.o
endif
SMM_NN_new.o:SMM_NN_new.c
	g++ -c -fPIC -fopenmp SMM_NN_new.c -o SMM_NN_new.o
DGEMM_NN.o:DGEMM_NN.c
	g++ -c -fPIC DGEMM_NN.c -o DGEMM_NN.o
PACK.o:PACK.c
	g++ -c -fPIC -fopenmp PACK.c -o PACK.o

install:
		mkdir -p $(PREFIX)/SMM
		mkdir -p $(PREFIX)/SMM/lib
		cp -f ./libsmm.so $(PREFIX)/SMM/lib/
		cp -f ./libsmm.a $(PREFIX)/SMM/lib/
		mkdir -p $(PREFIX)/SMM/include
		cp -f ./*.h $(PREFIX)/SMM/include/

.PHONY:clean
clean:
	rm -f *.o *.a *.so
