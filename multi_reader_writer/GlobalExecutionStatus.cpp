#include"GlobalExecutionStatus.h"

GlobalExecutionStatus::GlobalExecutionStatus() {
	jobsMap = std::make_shared<std::map<jobid_t, Job >>();
}

void GlobalExecutionStatus::setIsShuttingDown(bool isShuttingDown) {
	std::unique_lock<std::mutex> lock(isShuttingDown_mtx);
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
}

bool GlobalExecutionStatus::getIsShuttingDown() {
	std::lock_guard<std::mutex> lock(isShuttingDown_mtx);
	return isShuttingDown;
}

void GlobalExecutionStatus::setIsWriteOperationInProgress(bool isWriteOperationInProgress) {
	std::lock_guard<std::mutex> lock(isWriteOperationInProgress_mtx);
	this->isWriteOperationInProgress = isWriteOperationInProgress;
}

bool GlobalExecutionStatus::getIsWriteOperationInProgress() {
	std::lock_guard<std::mutex> lock(isWriteOperationInProgress_mtx);
	return isWriteOperationInProgress;
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
Job GlobalExecutionStatus::getJob(jobid_t jobid) {
	std::lock_guard<std::mutex> lock(jobsMap_mtx);
	std::map<jobid_t, Job>::iterator job_itr = jobsMap->find(jobid);
	if(job_itr != jobsMap->end()) {
		return job_itr->second;
	}//else throw JobNotFound exception
}

std::shared_ptr<std::map<jobid_t, Job>> 	GlobalExecutionStatus::getJobExecStatusMap() {
	return jobsMap;	
}


void GlobalExecutionStatus::registerEventListener(EventListener& eventListener) {
	eventListeners.push_back(eventListener);
}

