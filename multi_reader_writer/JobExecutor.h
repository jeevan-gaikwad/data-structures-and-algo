#ifndef _JOB_EXECUTOR_H_
#define _JOB_EXECUTOR_H_

#include<iostream>
#include<queue>
#include<memory>
#include<mutex>
#include<thread>
#include"Job.h"
#include"GlobalExecutionStatus.h"

const int MAX_NO_OF_THREADS = 5;

class JobExecutor {
private:
	std::shared_ptr<GlobalExecutionStatus> globalExecutionStatus;
	std::shared_ptr<std::queue<Job>> jobExecutionQueue;
	std::mutex jobExecutionQueue_mtx;
	void executeJob();
	void enQueue(Job& job);
	void deQueue(Job& job); //job is out param
	int  getExecQueueSize();

	//Now here, we'll have thread pool to pick jobs from the queue. 

	std::shared_ptr<std::thread> workderThreadPool[MAX_NO_OF_THREADS];

public: 
	JobExecutor(std::shared_ptr<GlobalExecutionStatus> globalExecutionStatus); 
	void addJobForExecution(Job& job);
	~JobExecutor(); //Wait for all worker threads to finish

};
#endif
