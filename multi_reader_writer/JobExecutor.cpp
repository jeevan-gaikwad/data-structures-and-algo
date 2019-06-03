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
	std::shared_ptr<Resource> resource = ioRequest.resource;
	if(IORequest::Type::READ == ioRequest.type) {
		resource->write(ioRequest.content);
		//Process part by part reading/writting and update % in the job status
	} else if(IORequest::Type::WRITE == ioRequest.type)
		resource.read(ioRequest.noOfBytes, ioRequest.content);	
	}
	job.setJobStatus(job_status_t::COMPLETED);
	std::time_t now;
	std::time(&now);
	job.setCompletionTime(now);
	//Handle error cases too
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
