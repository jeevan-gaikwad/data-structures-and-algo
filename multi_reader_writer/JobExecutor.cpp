#include <ctime>
#include "JobExecutor.h"

JobExecutor::JobExecutor(std::shared_ptr<GlobalExecutionStatus> globalExecutionStatus) {
		jobExecutionQueue = std::make_shared<std::queue<Job>>();
		this->globalExecutionStatus = globalExecutionStatus;

	//create threads
	for(int itr=0; itr < MAX_NO_OF_THREADS; itr++) {
		workderThreadPool[itr] = std::make_shared<std::thread>(&JobExecutor::executeJob, this);
	}
	std::cout<<"Worker TP initialized successfully."<<std::endl;
}


void JobExecutor::addJobForExecution(Job& job) {

	enQueue(job);
	
	//wakeup all threads waiting for work
	std::lock_guard<std::mutex> lock(workQueueFilled_cond_mtx);
	workQueueFilled_cond.notify_all();
}

bool JobExecutor::isWorkQueueCondMet() {//For spurious wakup

	if(getExecQueueSize() > 0 && globalExecutionStatus->getIsShuttingDown() == false) {
		return true;
	}else {
		return false;
	}
}
//thread func
void JobExecutor::executeJob() {
	Job job;
	while(true) {
		std::unique_lock<std::mutex> lock(workQueueFilled_cond_mtx);
		workQueueFilled_cond.wait(lock,std::bind(&JobExecutor::isWorkQueueCondMet, this));
		
		if(globalExecutionStatus->getIsShuttingDown() == true) {
			std::cout<<"Worker thread exiting. System is shutting down."<<std::endl;
			break;
		}

		deQueue(job);
		std::cout<<"JobExecutor: Jod "<<job.getJobId()<<" picked from the queue successfully!"<<std::endl;
		job.setStatus(job_status_t::IN_PROGRESS);
		//Check request type
		IORequest& ioRequest = job.getIORequest();
		std::shared_ptr<Resource> resource = ioRequest.resource;
		if(IORequest::Type::READ == ioRequest.type) {
			resource->read(ioRequest.noOfBytes, ioRequest.content);	
			//Process part by part reading/writting and update % in the job status
		} else if(IORequest::Type::WRITE == ioRequest.type) {
			resource->write(ioRequest.content);
		}
		job.setStatus(job_status_t::COMPLETED);
		std::time_t now;
		std::time(&now);
		job.setCompletionTime(now);
		std::cout<<"Job executed successfully!"<<std::endl;
		//Handle error cases too
	}
}

void JobExecutor::enQueue(Job& job) {
	std::lock_guard<std::mutex> lock(jobExecutionQueue_mtx);
	jobExecutionQueue->push(job);
}

void JobExecutor::deQueue(Job& job) {
	std::lock_guard<std::mutex> lock(jobExecutionQueue_mtx);
	job = jobExecutionQueue->front();
	jobExecutionQueue->pop();
}

int  JobExecutor::getExecQueueSize() {
	std::lock_guard<std::mutex> lock(jobExecutionQueue_mtx);
	return jobExecutionQueue->size();
}

JobExecutor::~JobExecutor() {
	//Wait for all threads to finish
	for(int itr = 0; itr < MAX_NO_OF_THREADS; itr++) {
		workderThreadPool[itr]->join();
	}
}

