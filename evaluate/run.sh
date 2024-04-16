export GOMP_CPU_AFFINITY="0-63"
make -C ../NN_LIB/ -f ../NN_LIB/Makefile
make
numactl -i 0-1 ./a.out 
