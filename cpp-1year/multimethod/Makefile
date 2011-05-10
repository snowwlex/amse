default: multimethod

intersects.o: intersects.cpp intersects.h shapes.h
	g++ -Wall -c intersects.cpp

main.o: main.cpp intersects.h shapes.h
	g++ -Wall -c main.cpp 

multimethod: main.o intersects.o
	g++ -Wall -o multimethod main.o intersects.o

clean: 
	rm *.o multimethod

all: multimethod
