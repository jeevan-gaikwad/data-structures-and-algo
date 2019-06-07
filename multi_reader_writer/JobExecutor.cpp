#include <ctime>
#include "JobExecutor.h"

JobExecutor::JobExecutor(std::shared_ptr<GlobalExecutionStatus> globalExecutionStatus) {
	this->globalExecutionStatus = globalExecutionStatus;
	workQueueFilled_cond = std::make_shared<std::condition_variable>();
	workQueueFilled_cond_mtx = std::make_shared<std::mutex>();

	//Register our intterest in shutdown event
	EventListener shutdownEventListener;
	shutdownEventListener.eventType = EventType::SHUTDOWN;
	shutdownEventListener.condition_var = workQueueFilled_cond;
	shutdownEventListener.condition_var_mtx = workQueueFilled_cond_mtx;
	
	globalExecutionStatus->registerEventListener(shutdownEventListener);


	//create threads
	for(int itr=0; itr < MAX_NO_OF_THREADS; itr++) {
		workderThreadPool[itr] = std::make_shared<std::thread>(&JobExecutor::executeJob, this);
	}
	std::cout<<"Worker TP initialized successfully."<<std::endl;
}


void JobExecutor::addJobForExecution(std::shared_ptr<Job> job) {

	enQueue(job);
	
	//wakeup all threads waiting for work
	std::lock_guard<std::mutex> lock(*workQueueFilled_cond_mtx);
	workQueueFilled_cond->notify_all();
}

bool JobExecutor::isWorkQueueCondMet() {//For spurious wakup

	std::cout<<"JobExecutor::isWorkQueueCondMet: I woke up"<<std::endl;
	if(globalExecutionStatus->getIsShuttingDown() == true) {
		return true;
	}

	if(getExecQueueSize() > 0) {
		return true;
	}else {
		return false;
	}
}
//thread func
void JobExecutor::executeJob() {
	std::shared_ptr<Job> job;
	while(true) {
		std::unique_lock<std::mutex> lock(*workQueueFilled_cond_mtx);
		workQueueFilled_cond->wait(lock,std::bind(&JobExecutor::isWorkQueueCondMet, this));
		
		if(globalExecutionStatus->getIsShuttingDown() == true) {
			std::cout<<"Worker thread exiting. System is shutting down."<<std::endl;
			break;
		}

		job = deQueue();//ensure that Q is not empty and if it is continue on wait
		if(job == nullptr) {
			//Queue is empty. job is already taken by other thread
			continue;
		}
		lock.unlock();//ensure that we are giving fair chance to other threads
		std::cout<<"JobExecutor: Jod "<<job->getJobId()<<" picked from the queue successfully!"<<std::endl;
		job->setStatus(job_status_t::IN_PROGRESS);
		//Check request type
		IORequest& ioRequest = job->getIORequest();
		std::shared_ptr<Resource> resource = ioRequest.resource;
		if(IORequest::Type::READ == ioRequest.type) {
			resource->read(ioRequest.noOfBytes, ioRequest.content);	
			//Process part by part reading/writting and update % in the job status
		} else if(IORequest::Type::WRITE == ioRequest.type) {
			resource->write(ioRequest.content);
		}
		job->setStatus(job_status_t::COMPLETED);
		job->setProgressPercentage(100.0f);
		std::time_t now;
		std::time(&now);
		job->setCompletionTime(now);
		std::cout<<"Job executed successfully!"<<std::endl;
		//Handle error cases too
	}
}

void JobExecutor::enQueue(std::shared_ptr<Job> job) {
	std::lock_guard<std::mutex> lock(jobExecutionQueue_mtx);
	jobExecutionQueue.push(job);
}

std::shared_ptr<Job>  JobExecutor::deQueue() {
	std::lock_guard<std::mutex> lock(jobExecutionQueue_mtx);
	std::shared_ptr<Job> job = nullptr;
	if(jobExecutionQueue.size() > 0 ) {
		job = jobExecutionQueue.front();
		jobExecutionQueue.pop();
	}
	return job; // throw an exception if queue is empty
}

int  JobExecutor::getExecQueueSize() {
	std::lock_guard<std::mutex> lock(jobExecutionQueue_mtx);
	return jobExecutionQueue.size();
}

JobExecutor::~JobExecutor() {
	//Wait for all threads to finish
	for(int itr = 0; itr < MAX_NO_OF_THREADS; itr++) {
		workderThreadPool[itr]->join();
	}
}

