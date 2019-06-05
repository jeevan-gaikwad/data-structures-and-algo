#ifndef _JOB_EXECUTOR_H_
#define _JOB_EXECUTOR_H_

#include<iostream>
#include<queue>
#include<memory>
#include"Job.h"
#include"GlobalExecutionStatus.h"
#include<mutex>

class JobExecutor {
private:
	std::shared_ptr<GlobalExecutionStatus> globalExecutionStatus;
	std::shared_ptr<std::queue<Job>> jobExecutionQueue;
	std::mutex jobExecutionQueue_mtx;
	void executeJob(Job& job);
	void enQueue(Job& job);
	void deQueue(Job& job); //job is out param

public: 
	JobExecutor(std::shared_ptr<GlobalExecutionStatus> globalExecutionStatus); 
	void addJobForExecution(Job& job);

};
#endif
