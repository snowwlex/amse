default: listener

analogclock.o: analogclock.cpp analogclock.h clockmodel.h  listenerpattern.h
	g++ -Wall -c analogclock.cpp
	
digitalclock.o: digitalclock.cpp digitalclock.h clockmodel.h listenerpattern.h
	g++ -Wall -c digitalclock.cpp
	
clockmodel.o: clockmodel.cpp clockmodel.h listenerpattern.h
	g++ -Wall -c clockmodel.cpp

main.o: main.cpp clockmodel.h digitalclock.h analogclock.h  listenerpattern.h
	g++ -Wall -c main.cpp 

listener: main.o  clockmodel.o  digitalclock.o analogclock.o
	g++ -Wall -o listener main.o clockmodel.o  digitalclock.o analogclock.o 

clean: 
	rm *.o listener

all: listener
