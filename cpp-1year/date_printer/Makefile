default: dateprinter

dateprinter.o: dateprinter.cpp dateprinter.h
	g++ -Wall -c dateprinter.cpp

main.o: main.cpp dateprinter.h
	g++ -Wall -c main.cpp 

dateprinter: main.o dateprinter.o
	g++ -Wall -o dateprinter main.o dateprinter.o

clean: 
	rm *.o dateprinter




