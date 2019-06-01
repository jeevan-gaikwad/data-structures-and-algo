#include<iostream>
#include"JobSchedular.h"

JobSchedular::JobSchedular(std::shared_ptr<GlobalExecutionStatus> globalExecutionStatus) {
	ioQueueManager = std::make_shared<QueueManager>();
	jobExecutor = std::make_shared<JobExecutor>(globalExecutionStatus);
	globalExecutionStatus = globalExecutionStatus;
}


void JobSchedular::runSchedular() {
	
}

void JobSchedular::addJobForScheduling(Job& job) {

	IORequest& request = job.getIORequest();
	IORequest::Type reqType = request.type;
	if(reqType == IORequest::Type::READ) {
		ioQueueManager->addReadReq(request);
	}else if(reqType == IORequest::Type::WRITE) {
		ioQueueManager->addWriteReq(request);
	}//else throw an exception InvalidReqType

	runSchedular();//New job is added into the Queue. Run schedular
}

