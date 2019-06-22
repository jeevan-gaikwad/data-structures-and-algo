#include "LogManager.h"

class IOException:public std::exception
{
	std::string cause;
	public:
	IOException(std::ostringstream& message) {
		this->cause = message.str();
	}
	const char* what() const throw(){
		return cause.c_str();
	}
};

LogManager::LogManager(std::string logFileName) {
	ofLogFile.open(logFileName, std::ofstream::app);
	if(!ofLogFile.is_open()) {
		std::ostringstream msg;
		msg << "Failed to open log file "<<logFileName<<std::endl;
		throw IOException(msg);
	}
	std::lock_guard<std::mutex> lock(ofLogFile_mtx);
	insertCurrentDateAndTime();
	ofLogFile<<"Logging started"<<std::endl;
}

void LogManager::insertCurrentDateAndTime() { //This function must be called with ofLogFile_mtx aquired to avoid inconsistency
	using std::chrono::system_clock;
	std::time_t tt = system_clock::to_time_t(system_clock::now());
	std::tm* pCalenderTime = std::localtime(&tt);
	ofLogFile<<std::put_time(pCalenderTime,"%c")<<": ";
}

/*
void LogManager::logDebugMsg(std::ostringstream oss) {
	std::lock_guard<std::mutex> lock(ofLogFile_mtx);
	insertCurrentDateAndTime();
	ofLogFile << oss.str() <<std::endl;	
	ofLogFile << std::flush;
}

void LogManager::logDebugMsg(std::string msg) {
#ifdef DEBUG	
	std::lock_guard<std::mutex> lock(ofLogFile_mtx);
	insertCurrentDateAndTime();
	std::cout<<"Logging:"<<msg<<std::endl; 
	ofLogFile <<"DEBUG: "<< msg <<std::endl;	
	ofLogFile << std::flush;
#endif
}
*/

void LogManager::logMsg(Category category, std::ostringstream& oss) {
	std::lock_guard<std::mutex> lock(ofLogFile_mtx);
	insertCurrentDateAndTime();
	if(category == INFO) {
		ofLogFile << "INFO:";
	}else if(category == DEBUG && isDebugLogOn) {
		ofLogFile << "DEBUG:";
		ofLogFile << oss.str() <<std::endl;	
		ofLogFile << std::flush;
		return;
	}else if(category == ERROR) {
		ofLogFile << "ERROR:";
	}
	ofLogFile << oss.str() <<std::endl;	
	ofLogFile << std::flush;
}

void LogManager::logMsg(Category category, std::string msg) {
	std::ostringstream oss(msg);
	logMsg(category, oss);
}

void LogManager::logMsg(Category category, const char* fileName, int lineNo, const char* funcName, const std::string msg) {
	std::ostringstream formattedMsg;
	formattedMsg << fileName <<" :" << lineNo << ": "<< funcName << " :" << msg ;	
	
	logMsg(category, formattedMsg);

}

