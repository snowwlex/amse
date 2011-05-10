default: stl

stl.o: stl.cpp predicates.h
	g++ -Wall -c stl.cpp 


stl: stl.o
	g++ -Wall -o stl stl.o

clean: 
	rm *.o stl

all: stl
