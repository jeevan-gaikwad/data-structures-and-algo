#ifndef _INPUT_OUTPUT_MANAGER
#define _INPUT_OUTPUT_MANAGER
#include<iostream>
#include<mutex>
#include<memory>
#include"Resource.h"

#define FILE 1
#define PIPE 2
#define MAX_PARALLEL_READ_REQ 10
#define MAX_PARALLEL_WRITE_REQ 1

class InputOutputManager {

private:
	std::shared_ptr<Resource> resource;
	std::mutex writeMtx;
    std::shared_ptr<QueueManager> ioQueueManager;
public:

	InputOutputManager(int resourceType, std::string id);

	bool open();
	int  write(std::string buff);
	int  read(int noOfBytesToRead, std:: string& buff);

private:
    void process_queues();
	
	//thread function to actually perform read opeation
	void processReadRequest(void *queueManager); 

	//thread function to actually perform read opeation
	void processWriteRequest(void *queueManager); 

	void createReaderThreads(); 
	void createWriterThreads();
		
};

#endif
