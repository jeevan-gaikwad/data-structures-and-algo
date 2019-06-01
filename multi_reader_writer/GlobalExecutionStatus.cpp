#include"GlobalExecutionStatus.h"

GlobalExecutionStatus::GlobalExecutionStatus() {
	jobsMap = std::make_shared<std::map<jobid_t, std::shared_ptr<Job> >>();
}
