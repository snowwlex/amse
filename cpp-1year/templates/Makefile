default: template

main.o: main.cpp complex.h
	g++ -Wall -c main.cpp 

template: main.o 
	g++ -Wall -o template main.o

clean: 
	rm *.o template
	
all: template

