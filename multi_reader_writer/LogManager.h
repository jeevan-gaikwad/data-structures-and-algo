#ifndef _LOG_MANAGER_H_
#define _LOG_MANAGER_H_

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <exception>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <mutex>


#define LOG_INFO(logger, x) { std::ostringstream oss; oss<<x; logger.logMsg(LogManager::INFO, oss); }
#define LOG_DEBUG(logger, x) { logger.logMsg(LogManager::DEBUG, __FILE__, __LINE__ , __func__, x); }
#define LOG_ERROR(logger, x) { logger.logMsg(LogManager::ERROR,x); }

class IOException;

class LogManager {

private:
	std::ofstream ofLogFile;
	std::mutex    ofLogFile_mtx;
	bool		  isDebugLogOn;

	void insertCurrentDateAndTime();
public:
	enum Category { INFO, DEBUG, ERROR };
	LogManager(std::string logFileName);
	~LogManager(); //close the log file
	void setIsDebugLogOn(bool isDebugLogOn) { this->isDebugLogOn = isDebugLogOn; }
	void logMsg(Category category, const char* fileName, int lineNo, const char* funcName, const std::string msg);

	void logMsg(Category category, std::ostringstream& oss);
	void logMsg(Category category, std::string msg);

/*	void logDebugMsg(std::ostringstream oss);
	void logDebugMsg(std::string msg);
	
	void logDebugMsg(std::ostringstream oss);
	void logErrorMsg(std::string msg);
*/	
};

#endif
