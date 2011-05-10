default: newdelete

complex.o: complex.cpp complex.h
	g++ -Wall -c complex.cpp

main.o: main.cpp complex.h
	g++ -Wall -c main.cpp 

newdelete: main.o complex.o
	g++ -Wall -o newdelete main.o complex.o

clean: 
	rm *.o newdelete

all: newdelete
