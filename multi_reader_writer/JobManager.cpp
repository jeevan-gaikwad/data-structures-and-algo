#include "JobManager.h"

JobManager::JobManager(std::shared_ptr<GlobalExecutionStatus> globalExecutionStatus) {

	this->globalExecutionStatus = globalExecutionStatus;
	jobSchedular = std::make_shared<JobSchedular>(globalExecutionStatus);
	
	currentJobId = 0;
}

jobid_t JobManager::createJob(std::shared_ptr<IORequest> ioRequest) {

	std::unique_lock<std::mutex> jobidLock(currentJobId_mtx);
	currentJobId++;
	jobid_t newJobId = currentJobId;
	jobidLock.unlock();

	std::shared_ptr<Job> newJob = std::make_shared<Job>(); //Once we create this job. This Job should be refferred by everybody instead of creating copies of it.
	newJob->setJobId(newJobId);
	newJob->setProgressPercentage(0.0f);
	newJob->setIORequest(ioRequest);
	//Note job's start time
	std::time_t now;
	std::time(&now);
	newJob->setStartTime(now);
	std::cout<<"New job is created id:"<<newJobId<<". Adding for scheduling.."<<std::endl;
	jobSchedular->addJobForScheduling(newJob);
	return newJobId;
}

const std::shared_ptr<Job>    JobManager::getJob(jobid_t id) {
	return globalExecutionStatus->getJob(id);
}

