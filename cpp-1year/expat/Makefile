default: expat

expat.o: expat.cpp data.h
	g++ -c expat.cpp

expat: expat.o
	g++ -Wall -o expat expat.o -lexpat

clean: 
	rm *.o expat
	
all: expat




