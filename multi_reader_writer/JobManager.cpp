#include"JobManager.h"

JobManager::JobManager(std::shared_ptr<GlobalExecutionStatus> globalExecutionStatus) {

	globalExecutionStatus = globalExecutionStatus;
	jobSchedular = std::make_shared<JobSchedular>(globalExecutionStatus);
	
	currentJobId = 0;
}

jobid_t JobManager::createJob(IORequest& ioRequest) {
	jobid_t newJobId = currentJobId + 1;
	std::shared_ptr<Job> newJob = std::make_shared<Job>();
	newJob->setJobId(newJobId);
	newJob->setProgressPercentage(0.0f);
	newJob->setIORequest(ioRequest);
	//Note job's start time
	std::time_t now;
	std::time(&now);
	newJob->setStartTime(now);

	jobsExecStatus->getJobExecStatusMap()->insert(std::pair<jobid_t,std::shared_ptr<Job>>(newJobId, newJob));
	jobSchedular->addJobForScheduling(newJob);
}

std::shared_ptr<Job>&    getJob(jobid_t id) {

}

void    deleteJob(jobid_t id) {

}
