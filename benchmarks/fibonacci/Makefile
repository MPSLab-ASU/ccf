FILE1 = fib.c

all: fib

CC = cgracc #gcc
ARMCC = arm-linux-gnueabi-gcc
LIB = -lm
fib: ${FILE1} Makefile
	$(CC) -static -O3 ${FILE1} -o fibonacci
clean:
	rm -rf fibonacci CGRAExec m5out
