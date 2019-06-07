#ifndef _JOB_EXECUTOR_H_
#define _JOB_EXECUTOR_H_

#include <iostream>
#include <queue>
#include <memory>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <functional>
#include "Job.h"
#include "GlobalExecutionStatus.h"

const int MAX_NO_OF_THREADS = 5;

class JobExecutor {

private:
	std::shared_ptr<GlobalExecutionStatus> globalExecutionStatus;
	std::queue<std::shared_ptr<Job> > jobExecutionQueue;
	std::mutex jobExecutionQueue_mtx;
	
	std::shared_ptr<std::condition_variable> workQueueFilled_cond;
	std::shared_ptr<std::mutex> workQueueFilled_cond_mtx;
	
	//Internal functions
	void executeJob();
	void enQueue(std::shared_ptr<Job> job);
	std::shared_ptr<Job> deQueue();
	int  getExecQueueSize();

	bool isWorkQueueCondMet();//For spurious wakup
	//Now here, we'll have thread pool to pick jobs from the queue. 

	std::shared_ptr<std::thread> workderThreadPool[MAX_NO_OF_THREADS];

public: 
	JobExecutor(std::shared_ptr<GlobalExecutionStatus> globalExecutionStatus); 
	void addJobForExecution(std::shared_ptr<Job> job);
	~JobExecutor(); //Wait for all worker threads to finish

};
#endif
