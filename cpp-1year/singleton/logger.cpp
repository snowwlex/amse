#include <iostream>
#include "singleton.h"
#include "logger.h"

bool Logger::openLogFile() {
	std::cout << "Opening log file" << std::endl;
	return true;
}

bool Logger::writeLogFile() {
	std::cout << "Writing to log file" << std::endl;
	return true;
}

bool Logger::closeLogFile() {
	std::cout << "Closing log file "  << std::endl;

	return true;
}
