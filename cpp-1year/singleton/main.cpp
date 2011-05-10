#include "singleton.h"
#include "logger.h"

int main(int argc, char* argv[]) {

	Logger* loggerPtr = Logger::Instance();
	Logger* loggerPtr2 = Logger::Instance();
	
	loggerPtr->openLogFile();
	loggerPtr2->writeLogFile();
	loggerPtr->closeLogFile();

	return 0;	
}
