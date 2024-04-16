LibShalom_PREFIX = $ ../NN_LIB
LibShalom_INC    = $(LibShalom_PREFIX)/
LibShalom_LIB    = $(LibShalom_PREFIX)/libsmm.a 

OTHER_LIBS  =-fopenmp

CC          = g++
CFLAGS      = -O3 -I$(LibShalom_INC)
LINKER      = $(CC)

OBJS        = test.o

%.o: %.c
	 $(CC) $(CFLAGS) -c -fopenmp $< -o $@

all: $(OBJS)
	$(LINKER) $(OBJS) $(LibShalom_LIB) $(OTHER_LIBS) -o a.out
