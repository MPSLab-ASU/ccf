FILE1 = sum.c

all: sum

CC = cgracc #gcc
ARMCC = arm-linux-gnueabi-gcc
LIB = -lm
sum: ${FILE1} Makefile
	$(CC) -static -O3 ${FILE1} -o sum
clean:
	rm -rf sum CGRAExec m5out
