FILE1 = pow.c

all: pow

CC = cgracc
ARMCC = arm-linux-gnueabi-gcc
LIB = -lm
pow: ${FILE1} Makefile
	$(CC) -static -O3 ${FILE1} -o pow
clean:
	rm -rf pow CGRAExec m5out *.bc *.ll
