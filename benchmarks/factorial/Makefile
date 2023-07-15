FILE1 = fact.c

all: fact

CC = cgracc #gcc
ARMCC = arm-linux-gnueabi-gcc
LIB = -lm
fact: ${FILE1} Makefile
	$(CC) -static -O3 ${FILE1} -o factorial
clean:
	rm -rf factorial CGRAExec m5out *.ll *.bc *~
