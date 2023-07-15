FILE1 = nested-if.c

all: nested_small nested_arm

CC = cgracc #gcc
ARMCC = arm-linux-gnueabi-gcc
LIB = -lm
nested_small: ${FILE1} Makefile
	$(CC) -static -O3 ${FILE1} -o nested_small 
nested_arm: ${FILE1} Makefile
	$(ARMCC) -static -O3 ${FILE1} -o nested_arm

clean:
	rm -rf nested_small nested_arm output* *.ll CGRAExec m5out *.s
