#ifndef _JOB_MGR_H_
#define _JOB_MGR_H_
#include"Job.h"
#include<memory>
#include<map>

class JobManager {

	std::shared_ptr<std::map<jobid_t, std::shared_ptr<Job> >> jobsMap;
	jobid_t currentJobId;
public:
	JobManager();	
	jobid_t createJob();
	std::shared_ptr<Job>&    getJob(jobid_t id);
	void    deleteJob(jobid_t id);
};

#endif
