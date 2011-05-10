default: tests

main.o: main.cpp
	g++  -c main.cpp
Test.o: Test.cpp
	g++  -c Test.cpp
ModelTest.o: ModelTest.cpp
	g++  -c ModelTest.cpp
model.o: Model/model.cpp
	g++  -c Model/model.cpp
rules.o: Model/rules.cpp
	g++  -c Model/rules.cpp
board.o: Model/board.cpp
	g++  -c Model/board.cpp
	

tests: main.o Test.o ModelTest.o model.o rules.o board.o
	g++  -o tests main.o Test.o ModelTest.o model.o rules.o board.o

clean: 
	rm *.o tests




