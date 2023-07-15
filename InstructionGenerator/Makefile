FILE = insgen.cpp
FILE1 = falcon_insgen.cpp
EXE = insgen
EXE1 = falcon_insgen
CXX=g++

LDFLAGS=
CFLAGS= -g -std=c++11


insgen: ${FILE} Makefile
	$(CXX) $(CFLAGS) -static -O3 ${FILE} -o $(EXE) $(LDFLAGS)
falcon_insgen: ${FILE1} Makefile
	$(CXX) $(CFLAGS) -static -O3 ${FILE1} -o $(EXE1) $(LDFLAGS)
 
clean:
	rm -rf $(EXE) $(EXE1)
