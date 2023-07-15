FILE1 = add.c

all: add_small add_arm

CC = cgracc #gcc
ARMCC = arm-linux-gnueabi-gcc
LIB = -lm
add_small: ${FILE1} Makefile
	$(CC) -static -O3 ${FILE1} -o add_small 
add_arm: ${FILE1} Makefile
	$(ARMCC) -static -O3 ${FILE1} -o add_arm

clean:
	rm -rf add_small add_arm output* *.ll CGRAExec m5out *.s
