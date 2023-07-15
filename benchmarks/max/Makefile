FILE1 = max.c

all: max

CC = cgracc
LIB = -lm
max: ${FILE1} Makefile
	$(CC) -static -O3 ${FILE1} -o max
clean:
	rm -rf max CGRAExec m5out
