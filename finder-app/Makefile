CC = cc
CC_CROSS=/home/arturo/Downloads/arm-gnu-toolchain-13.3.rel1-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-gcc

default: all

all:
ifdef CROSS_COMPILE
	@echo "cross_compile enabled"	
	$(CC_CROSS) -static -Wall writer.c -o writer_a64

endif
ifndef CROSS_COMPILE
	@echo "native build enabled"
	$(CC) -Wall writer.c -o writer
endif
	

clean:
	rm -f *.o writer writer_a64 *.out
