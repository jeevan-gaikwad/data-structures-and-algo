#ifndef _INPUT_OUTPUT_MANAGER
#define _INPUT_OUTPUT_MANAGER
#include<iostream>
#include<mutex>
#include<memory>
#include<thread>
#include"Resource.h"
#include"QueueManager.h"

#define TYPE_FILE 1
#define TYPE_PIPE 2

#define MAX_NO_OF_READER_THREADS 5

class InputOutputManager {

private:
	std::shared_ptr<Resource> resource;
	std::mutex writeMtx;
    std::shared_ptr<QueueManager> ioQueueManager;
	std::shared_ptr<std::thread>  readerThreadPool[MAX_NO_OF_READER_THREADS]; 
	std::shared_ptr<std::thread>  writerThread; 
public:

	InputOutputManager(int resourceType, std::string id);
	~InputOutputManager();

	bool open();
	int  write(std::string buff);
	int  read(int noOfBytesToRead, std:: string& buff);

private:
	
	//thread function to actually perform read opeation
	void processReadRequest();

	//thread function to actually perform read opeation
	void processWriteRequest();

	void createReaderThreads(); 
	void createWriterThreads();
	void waitForAllThreadsToFinish();
};

#endif
