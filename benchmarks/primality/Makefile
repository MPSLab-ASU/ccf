FILE1 = prime.c

all: prime

CC = cgracc #gcc
ARMCC = arm-linux-gnueabi-gcc
LIB = -lm
prime: ${FILE1} Makefile
	$(CC) -static -O3 ${FILE1} -o prime
clean:
	rm -rf prime CGRAExec m5out
