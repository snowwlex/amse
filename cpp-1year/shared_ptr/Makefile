default: shared_ptr

shared_ptr.o: shared_ptr.cpp shared_ptr.h
	gcc -Wall -c shared_ptr.cpp

object.o: object.cpp object.h
	gcc -Wall -c object.cpp

main.o: main.cpp object.h shared_ptr.h main.h
	gcc -Wall -c main.cpp 

shared_ptr: main.o object.o shared_ptr.o
	gcc -Wall -o shared_ptr -lstdc++ main.o object.o shared_ptr.o

clean: 
	rm *.o shared_ptr




