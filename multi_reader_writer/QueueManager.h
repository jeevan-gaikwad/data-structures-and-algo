#ifndef _QUEUE_MANAGER_H_
#define _QUEUE_MANAGER_H_
#include <iostream>
#include <queue>
#include <mutex>
#include "Job.h"

#define MAX_PARALLEL_READ_REQ 10
#define MAX_PARALLEL_WRITE_REQ 1

class QueueManager {
	std::queue<std::shared_ptr<Job> > readJobQueue;
	std::queue<std::shared_ptr<Job>> writeJobQueue;
	std::mutex            readQueueMtx;
	std::mutex            writeQueueMtx;
	int                   maxNoOfReadJob = MAX_PARALLEL_READ_REQ;
	int                   maxNoOfWriteJob = MAX_PARALLEL_WRITE_REQ;

public:
	void addReadJob(std::shared_ptr<Job> job);//should throw an exception if max no of READ req limit is reached
	void addWriteJob(std::shared_ptr<Job> job);//should throw an exception if max no of WRITE req limit is reached
	std::shared_ptr<Job> getReadJob();
	std::shared_ptr<Job> getWriteJob();
	int getWriteJobCurrentQSize();
	int getReadJobCurrentQSize();
};



#endif
