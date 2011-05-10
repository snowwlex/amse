default: loggerS

logger.o: logger.cpp logger.h singleton.h
	g++ -Wall -c logger.cpp

main.o: main.cpp logger.h singleton.h
	g++ -Wall -c main.cpp 

loggerS: main.o logger.o
	g++ -Wall -o loggerS main.o logger.o 

clean: 
	rm *.o loggerS

