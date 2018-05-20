FILE1 = basicmath_small.c   rad2deg.c  cubic.c   isqrt.c
FILE2 = basicmath_large.c   rad2deg.c  cubic.c   isqrt.c

all: basicmath_small 
#basicmath_large

CC = cgracc #gcc
LIB = -lm
basicmath_small: ${FILE1} Makefile
	$(CC) -static -O3 ${FILE1} -o basicmath_small $(LIB)
basicmath_large: ${FILE2} Makefile
	$(CC) -static -O3 ${FILE2} -o basicmath_large $(LIB)

clean:
	rm -rf basicmath_small basicmath_large output* *.ll CGRAExec m5out *.s
