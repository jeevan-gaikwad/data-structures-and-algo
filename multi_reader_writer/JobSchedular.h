#ifndef _JOB_SCHEDULAR_H_
#define _JOB_SCHEDULAR_H_
#include<iostream>
#include<memory>
#include"Job.h"
#include"JobExecutor.h"

class JobSchedular {

private:

    std::shared_ptr<QueueManager> ioQueueManager;
	//Now here, we'll have thread pool to pick jobs from the queue. To add ajob in the 
	//thread pool, we need a sheduling policy
	std::shared_ptr<JobExecutor> jobExecutor;
	std::shared_ptr<GlobalExecutionStatus> globalExecutionStatus;

	//functions
	void runSchedular();
public:
	JobSchedular(std::shared_ptr<GlobalExecutionStatus> globalExecutionStatus);
	void addJobForScheduling(Job& job);

};
#endif
