#ifndef _JOB_MGR_H_
#define _JOB_MGR_H_
#include <iostream>
#include <memory>
#include <map>
#include "Job.h"
#include "QueueManager.h"
#include "JobSchedular.h"

class JobManager {

	std::shared_ptr<JobSchedular> jobSchedular;
	jobid_t currentJobId;
	std::mutex currentJobId_mtx;

public:
	JobManager(std::shared_ptr<GlobalExecutionStatus> globalExecutionStatus);	
	jobid_t 				createJob(std::shared_ptr<IORequest> ioRequest);
	const std::shared_ptr<Job>    getJob(jobid_t id);
	job_status_t		    deleteJob(jobid_t id);//Should Throw JobNotFound exception
	std::shared_ptr<GlobalExecutionStatus> globalExecutionStatus;
};

#endif
