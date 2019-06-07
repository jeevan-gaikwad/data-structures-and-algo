#ifndef _JOB_SCHEDULAR_H_
#define _JOB_SCHEDULAR_H_
#include <iostream>
#include <memory>
#include <condition_variable>
#include "Job.h"
#include "JobExecutor.h"
#include "QueueManager.h"


class JobSchedular {

private:

    std::shared_ptr<QueueManager> ioQueueManager;
	std::shared_ptr<JobExecutor> jobExecutor;
	std::shared_ptr<GlobalExecutionStatus> globalExecutionStatus;
	std::shared_ptr<std::thread> schedularThread;
	std::shared_ptr<std::condition_variable> run_schedular_cond;
	std::shared_ptr<std::mutex> run_schedular_cond_mtx;

	//internal functions
	void runSchedular();
	bool isSchedularCondMet();
public:
	JobSchedular(std::shared_ptr<GlobalExecutionStatus> globalExecutionStatus);
	~JobSchedular();//Should destroy shedular thread
	void addJobForScheduling(std::shared_ptr<Job> job);

};
#endif
