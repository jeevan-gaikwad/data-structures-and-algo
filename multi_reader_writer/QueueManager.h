#ifndef _QUEUE_MANAGER_H_
#define _QUEUE_MANAGER_H_
#include <iostream>
#include <queue>
#include <mutex>
#include "Job.h"

#define MAX_PARALLEL_READ_REQ 10
#define MAX_PARALLEL_WRITE_REQ 1

class QueueManager {
	std::queue<Job> readJobQueue;
	std::queue<Job> writeJobQueue;
	std::mutex            readQueueMtx;
	std::mutex            writeQueueMtx;
	int                   maxNoOfReadJob = MAX_PARALLEL_READ_REQ;
	int                   maxNoOfWriteJob = MAX_PARALLEL_WRITE_REQ;

public:
	void addReadJob(Job& job);//should throw an exception if max no of READ req limit is reached
	void addWriteJob(Job& job);//should thrown an exception if max no of WRITE req limit is reached
	Job& getReadJob();
	Job& getWriteJob();
	int getWriteJobCurrentQSize();
	int getReadJobCurrentQSize();
};



#endif
