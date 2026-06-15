CC = cc
CC_CROSS=/home/arturo/Downloads/arm-gnu-toolchain-13.3.rel1-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-gcc

default: all

all:
	$(CROSS_COMPILE)gcc -Wall writer.c -o writer	

clean:
	rm -f *.o writer *.out

old:
ifdef CROSS_COMPILE
	@echo "cross_compile enabled"	
	$(CC_CROSS) -Wall writer.c -o writer

endif
ifndef CROSS_COMPILE
	@echo "native build enabled"
	$(CC)gcc -Wall writer.c -o writer
endif
