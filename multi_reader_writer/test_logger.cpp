#include <iostream>
#include "LogManager.h"

//Uncomment following code to test logger
/*
class Test {

public:
	void log_sample_line(LogManager& logger) {
		LOG_INFO(logger,"normal msg");
	}

	void log_sample_debug_line(LogManager& logger) {
		LOG_DEBUG(logger,"sample DEBUG msg");
	}
};

int main(void) {
	std::string fileName("testLogFile.log");
	LogManager logger(fileName);
	int i=30;
	//logger.logDebugMsg("myMsg");
	Test test;
	test.log_sample_line(logger);
	test.log_sample_debug_line(logger);
	std::string fileName2("testLogFile2.log");
	LogManager logger2(fileName2);
	
	//logger.logDebugMsg("myMsg");
	
	test.log_sample_line(logger2);
	test.log_sample_debug_line(logger2);

	//logger.logMsg("normal msg");

	//logger.logDebugMsg();

	return 0;
}
*/
