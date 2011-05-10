default: complex

complex.o: complex.cpp complex.h
	g++ -Wall -c complex.cpp

main.o: main.cpp complex.h
	g++ -Wall -c main.cpp 

complex: main.o complex.o
	g++ -Wall -o complex main.o complex.o

clean: 
	rm *.o complex

all: complex
