#include"JobManager.h"

JobManager::JobManager(std::shared_ptr<GlobalExecutionStatus> globalExecutionStatus) {

	globalExecutionStatus = globalExecutionStatus;
	jobSchedular = std::make_shared<JobSchedular>(globalExecutionStatus);
	
	currentJobId = 0;
}

jobid_t JobManager::createJob(IORequest& ioRequest) {
	std::unique_lock<std::mutex> jobidLock(currentJobId_mtx);
	jobidLock.lock();
	jobid_t newJobId = currentJobId + 1;
	jobidLock.unlock();

	Job newJob;
	newJob.setJobId(newJobId);
	newJob.setProgressPercentage(0.0f);
	newJob.setIORequest(ioRequest);
	//Note job's start time
	std::time_t now;
	std::time(&now);
	newJob.setStartTime(now);

	jobSchedular->addJobForScheduling(newJob);
	return newJobId;
}

const Job    JobManager::getJob(jobid_t id) {
	return globalExecutionStatus->getJob(id);
}

