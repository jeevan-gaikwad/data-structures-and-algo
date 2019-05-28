#include"JobManager.h"

JobManager::JobManager() {
	jobsMap = std::make_shared<std::map<jobid_t, std::shared_ptr<Job> >>();
	currentJobId = 0;
}
jobid_t JobManager::createJob() {
	jobid_t newJobId = currentJobId + 1;
	std::shared_ptr<Job> newJob = std::make_shared<Job>();
	newJob->setJobId(newJobId);
	newJob->setProgressPercentage(0.0f);
	//Note job's start time
	std::time_t now;
	std::time(&now);
	newJob->setStartTime(now);

	jobsMap->insert(std::pair<jobid_t,std::shared_ptr<Job>>(newJobId, newJob));

}

std::shared_ptr<Job>&    getJob(jobid_t id) {

}

void    deleteJob(jobid_t id) {

}
