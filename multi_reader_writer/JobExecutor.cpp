#include<ctime>
#include"JobExecutor.h"

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
	std::cout<<"JobExecutor: I should wakeup worker threads now.."<<std::endl;
}

//thread func
void JobExecutor::executeJob() {
	Job job;
	while(true) {
		std::cout<<"Checking for worker Q size and Shutdown signal.."<<std::endl;
		if(getExecQueueSize() > 0 && globalExecutionStatus->getIsShuttingDown() == false) {
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
		}else {
			if(globalExecutionStatus->getIsShuttingDown() == true) {
				std::cout<<"System is shutting down..exiting.."<<std::endl;
				break;
			}
			std::this_thread::sleep_for(std::chrono::seconds(2));
		}
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

