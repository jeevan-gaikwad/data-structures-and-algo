#include"GlobalExecutionStatus.h"

GlobalExecutionStatus::GlobalExecutionStatus() {
	//For now, nothing to be initialized explicitely
}

void GlobalExecutionStatus::setIsShuttingDown(bool isShuttingDown) {
	std::unique_lock<std::mutex> lock(isShuttingDown_mtx);
	if(!this->isShuttingDown) {
		this->isShuttingDown = isShuttingDown;
		lock.unlock();
		//notify all shutdown event listeners
		std::vector<EventListener>::iterator itr = eventListeners.begin();
		for(; itr != eventListeners.end(); itr++) {
			EventListener& eventLister = *itr;
			if(eventLister.eventType == EventType::SHUTDOWN) {
				std::lock_guard<std::mutex> lock(*(eventLister.condition_var_mtx));
				eventLister.condition_var->notify_all();
				std::cout<<"Notified all shutdown event listeners.."<<std::endl;
			}
		}
	}else {
		lock.unlock();
	}
}

void GlobalExecutionStatus::insertJobIntoMap(std::shared_ptr<Job> job) {
	std::lock_guard<std::mutex> lock(jobsMap_mtx);
	jobsMap.insert(std::pair<jobid_t, std::shared_ptr<Job>>(job->getJobId(), job));
}

const std::shared_ptr<Job> GlobalExecutionStatus::getJob(jobid_t jobid) {
	std::lock_guard<std::mutex> lock(jobsMap_mtx);
	std::map<jobid_t, std::shared_ptr<Job> >::iterator job_itr = this->jobsMap.find(jobid);
	if(job_itr != jobsMap.end()) {
		return job_itr->second;
	}//else throw JobNotFound exception
}


bool GlobalExecutionStatus::getIsShuttingDown() {
	std::lock_guard<std::mutex> lock(isShuttingDown_mtx);
	return isShuttingDown;
}

void GlobalExecutionStatus::setIsWriteOperationInProgress(bool isWriteOperationInProgress) {
	std::lock_guard<std::mutex> lock(isWriteOperationInProgress_mtx);
	this->writeOperationInProgress = isWriteOperationInProgress;
}

bool GlobalExecutionStatus::isWriteOperationInProgress() {
	std::lock_guard<std::mutex> lock(isWriteOperationInProgress_mtx);
	return writeOperationInProgress;
}

void GlobalExecutionStatus::incrementNoOfReadOperationsInProgressByOne() {
	std::lock_guard<std::mutex> lock(noOfReadOperationsInProgress_mtx);
	noOfReadOperationsInProgress += 1;
}

void GlobalExecutionStatus::decrementNoOfReadOperationsInProgressByOne() {
	std::lock_guard<std::mutex> lock(noOfReadOperationsInProgress_mtx);
	noOfReadOperationsInProgress -= 1;
}

void GlobalExecutionStatus::incrementNoOfWriteOperationsPerformed() {
	std::lock_guard<std::mutex> lock(noOfWriteOperationsPerformed_mtx);
	noOfWriteOperationsPerformed += 1;
}

void GlobalExecutionStatus::decrementNoOfWriteOperationsPerformed() {
	std::lock_guard<std::mutex> lock(noOfWriteOperationsPerformed_mtx);
	noOfWriteOperationsPerformed -= 1;
}

void GlobalExecutionStatus::registerEventListener(EventListener& eventListener) {
	eventListeners.push_back(eventListener);
}

int  GlobalExecutionStatus::getNoOfWriteOperationsPerformed() {
	
	std::lock_guard<std::mutex> lock(noOfWriteOperationsPerformed_mtx);
	return noOfWriteOperationsPerformed;
}

void GlobalExecutionStatus::resetNoOfWriteOperationsPerformed() {
	std::lock_guard<std::mutex> lock(noOfWriteOperationsPerformed_mtx);
	noOfWriteOperationsPerformed = 0;
}

//READ
void GlobalExecutionStatus::incrementNoOfReadOperationsPerformed() {
	std::lock_guard<std::mutex> lock(noOfReadOperationsPerformed_mtx);
	noOfReadOperationsPerformed += 1;	
}

void GlobalExecutionStatus::decrementNoOfReadOperationsPerformed() {
	std::lock_guard<std::mutex> lock(noOfReadOperationsPerformed_mtx);
	noOfReadOperationsPerformed -= 1;	
}

int  GlobalExecutionStatus::getNoOfReadOperationsPerformed() {
	std::lock_guard<std::mutex> lock(noOfReadOperationsPerformed_mtx);
	return noOfReadOperationsPerformed;
}

void GlobalExecutionStatus::resetNoOfReadOperationsPerformed() {
	std::lock_guard<std::mutex> lock(noOfReadOperationsPerformed_mtx);
	noOfReadOperationsPerformed = 0;
}

