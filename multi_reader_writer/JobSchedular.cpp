#include<iostream>
#include"JobSchedular.h"

JobSchedular::JobSchedular(std::shared_ptr<GlobalExecutionStatus> globalExecutionStatus) {
	ioQueueManager = std::make_shared<QueueManager>();
	jobExecutor = std::make_shared<JobExecutor>(globalExecutionStatus);
	globalExecutionStatus = globalExecutionStatus;
}


void JobSchedular::runSchedular() {
	/* 
		1. Ensure that only one WRITE job is running
		2. No starvation for read/write requests
	*/	
	std::cout<<"Running job schedular."<<std::endl;
	if(ioQueueManager->getWriteJobCurrentQSize() > 0) {
		jobExecutor->addJobForExecution(ioQueueManager->getWriteJob());
	}

	if(ioQueueManager->getReadJobCurrentQSize() > 0) {
		jobExecutor->addJobForExecution(ioQueueManager->getReadJob());
	}
}

void JobSchedular::addJobForScheduling(Job& job) {

	IORequest& request = job.getIORequest();
	IORequest::Type reqType = request.type;
	if(reqType == IORequest::Type::READ) {
		ioQueueManager->addReadJob(job);
	}else if(reqType == IORequest::Type::WRITE) {
		ioQueueManager->addWriteJob(job);
	}//else throw an exception InvalidJobType
	
	//Update Global execution status for job status
	globalExecutionStatus->getJobExecStatusMap()->insert(std::pair<jobid_t, Job>(job.getJobId(), job));

	runSchedular();//New job is added into the Queue. Run schedular
}

