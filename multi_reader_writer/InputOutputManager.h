#ifndef _INPUT_OUTPUT_MANAGER
#define _INPUT_OUTPUT_MANAGER
#include <iostream>
#include <mutex>
#include <memory>
#include <thread>
#include "Resource.h"
#include "JobManager.h"
#include "GlobalExecutionStatus.h"

#define TYPE_FILE 1
#define TYPE_PIPE 2

#define MAX_NO_OF_READER_THREADS 5

class InputOutputManager {

private:
	std::shared_ptr<Resource> resource;
	std::mutex writeMtx;
	std::shared_ptr<JobManager>    jobManager;
	/* globalExecutionStatus would be a shared objects between class to set/get current execution status.*/

	std::shared_ptr<GlobalExecutionStatus> globalExecutionStatus;
public:

	InputOutputManager(int resourceType, std::string id);
	~InputOutputManager();

	bool open();
	jobid_t  write(std::string buff);
	jobid_t  read(int noOfBytesToRead, std:: string& buff);
	const Job getJobExecInfo(jobid_t jobId); //see if we can return Job&

private:
	/*
	//thread function to actually perform read opeation
	void processReadRequest();

	//thread function to actually perform read opeation
	void processWriteRequest();

	void createReaderThreads(); 
	void createWriterThreads();
	void waitForAllThreadsToFinish();
	*/
};

#endif
