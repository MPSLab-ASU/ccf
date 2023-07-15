FILE1 = add.c

all: loop

CC = cgracc #gcc
ARMCC = arm-linux-gnueabi-gcc
LIB = -lm
loop: ${FILE1} Makefile
	$(CC) -static -O3 ${FILE1} -o loop
clean:
	rm -rf loop CGRAExec m5out
