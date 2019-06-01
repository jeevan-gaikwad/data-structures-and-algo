#include"JobExecutor.h"

JobExecutor::JobExecutor(std::shared_ptr<GlobalExecutionStatus> globalExecutionStatus) {
		jobExecutionQueue = std::make_shared<std::queue<Job>>();
		globalExecutionStatus = globalExecutionStatus;
}


void JobExecutor::addJobForExecution(Job& job) {
	enQueue(job);
	//wakeup all threads waiting for work
}

//thread func
void JobExecutor::executeJob(Job& job) {
	job.setJobStatus(job_status_t::IN_PROGRESS);
	//Check request type
	IORequest& ioRequest = job.getIORequest();
	if(IORequest::Type::READ == ioRequest.type) {

	}
}

void JobExecutor::enQueue(Job& job) {
	std::lock_guard<std::mutex> lock(jobExecutionQueue);
	jobExecutionQueue.push(job);
}

void JobExecutor::deQueue(Job& job) {
	std::lock_guard<std::mutex> lock(jobExecutionQueue);
	job = jobExecutionQueue.front(job);
	jobExecutionQueue.pop();
}
