#include"JobManager.h"

JobManager::JobManager(std::shared_ptr<GlobalExecutionStatus> globalExecutionStatus) {

	globalExecutionStatus = globalExecutionStatus;
	jobSchedular = std::make_shared<JobSchedular>(globalExecutionStatus);
	
	currentJobId = 0;
}

jobid_t JobManager::createJob(IORequest& ioRequest) {

	std::unique_lock<std::mutex> jobidLock(currentJobId_mtx);
	currentJobId++;
	jobid_t newJobId = currentJobId;
	jobidLock.unlock();

	Job newJob;
	newJob.setJobId(newJobId);
	newJob.setProgressPercentage(0.0f);
	newJob.setIORequest(ioRequest);
	//Note job's start time
	std::time_t now;
	std::time(&now);
	newJob.setStartTime(now);
	std::cout<<"New job is created id:"<<newJobId<<". Adding for scheduling.."<<std::endl;
	jobSchedular->addJobForScheduling(newJob);
	return newJobId;
}

const Job    JobManager::getJob(jobid_t id) {
	return globalExecutionStatus->getJob(id);
}

