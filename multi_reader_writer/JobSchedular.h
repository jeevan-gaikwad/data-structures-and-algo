#ifndef _JOB_SCHEDULAR_H_
#define _JOB_SCHEDULAR_H_
#include<iostream>
#include<memory>
#include"Job.h"
#include"JobExecutor.h"
#include"QueueManager.h"


class JobSchedular {

private:

    std::shared_ptr<QueueManager> ioQueueManager;
	std::shared_ptr<JobExecutor> jobExecutor;
	std::shared_ptr<GlobalExecutionStatus> globalExecutionStatus;

	//functions
	void runSchedular();
public:
	JobSchedular(std::shared_ptr<GlobalExecutionStatus> globalExecutionStatus);
	void addJobForScheduling(Job& job);

};
#endif
