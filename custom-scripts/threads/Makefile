BUILDROOT_DIR := ../buildroot-2017.08
KDIR := $(BUILDROOT_DIR)/output/build/linux-custom

#COMPILER := $(BUILDROOT_DIR)/output/host/bin/i686-buildroot-linux-uclibc-gcc
COMPILER := gcc

all:
	$(COMPILER) -o thread_runner thread_runner.c -lpthread -Wall
	
clean:
	rm -f *.o 
	rm -f thread_runner
