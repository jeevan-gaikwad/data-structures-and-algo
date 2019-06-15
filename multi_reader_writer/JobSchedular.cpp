#include<iostream>
#include<functional>
#include"JobSchedular.h"

JobSchedular::JobSchedular(std::shared_ptr<GlobalExecutionStatus> globalExecutionStatus) {
	ioQueueManager = std::make_shared<QueueManager>();
	jobExecutor = std::make_shared<JobExecutor>(globalExecutionStatus);
	run_schedular_cond = std::make_shared<std::condition_variable>();
	run_schedular_cond_mtx = std::make_shared<std::mutex>();

	this->globalExecutionStatus = globalExecutionStatus;
	//Register our intterest in shutdown event
	EventListener shutdownEventListener;
	shutdownEventListener.eventType = EventType::SHUTDOWN;
	shutdownEventListener.condition_var = run_schedular_cond;
	shutdownEventListener.condition_var_mtx = run_schedular_cond_mtx;
	
	globalExecutionStatus->registerEventListener(shutdownEventListener);
	//Run a schedular thread
	schedularThread = std::make_shared<std::thread>(&JobSchedular::runSchedular, this);
}

bool JobSchedular::isSchedularCondMet() { //For Spurious wakeup
	std::cout<<"JobSchedular::isSchedularCondMet : I woke up.."<<std::endl;
	if(globalExecutionStatus->getIsShuttingDown() == true) 
		return true;

	if(ioQueueManager->getWriteJobCurrentQSize() > 0 || ioQueueManager->getReadJobCurrentQSize() > 0) {
		return true;//Schedular should run
	}else {
		return false;
	}
}

void JobSchedular::runSchedular() {
	
	while(globalExecutionStatus->getIsShuttingDown() == false) {
		
		if(ioQueueManager->getWriteJobCurrentQSize() == 0 && ioQueueManager->getReadJobCurrentQSize() == 0) { //Nothing to schedule, wait for job(s)
			std::unique_lock<std::mutex> lock(*run_schedular_cond_mtx);
			std::cout<<"Nothing in read/write Queue. Schedular is now going to wait until someone wakesup.."<<std::endl;
			run_schedular_cond->wait(lock, std::bind(&JobSchedular::isSchedularCondMet, this));
	
			if(globalExecutionStatus->getIsShuttingDown() == true) { //This is to ensure that after waking up, system is NOT shutting down
				std::cout<<"System is shutting down. Terminating schedular thread."<<std::endl;
				break; //break while loop
			}
		}

		/* Scheduling logic. Little complex. Think on simplifying it
			1. Ensure that only one WRITE job is running
			2. No starvation for read/write requests
		*/	
	
		std::cout<<"Running job schedular."<<std::endl;
		if(ioQueueManager->getWriteJobCurrentQSize() > 0) {
			if(globalExecutionStatus->isWriteOperationInProgress() == false && (
			globalExecutionStatus->getNoOfWriteOperationsPerformed() <= 5 || ioQueueManager->getReadJobCurrentQSize() == 0)) {
				
				jobExecutor->addJobForExecution(ioQueueManager->getWriteJob());

			}else {
				//Hold write requests execution fot a while and prioritize READ				
				globalExecutionStatus->resetNoOfWriteOperationsPerformed();
			}
		}

		if(ioQueueManager->getReadJobCurrentQSize() > 0) {
			if(globalExecutionStatus->getNoOfReadOperationsPerformed() <= 4||!(ioQueueManager->getWriteJobCurrentQSize() > 0)) {

				jobExecutor->addJobForExecution(ioQueueManager->getReadJob());

			} else {
				//Hold read requests execution for a while and prioritize WRITE
				globalExecutionStatus->resetNoOfReadOperationsPerformed();
			}
			std::this_thread::sleep_for(std::chrono::seconds(1)); //This is when queue(s) are not empty and give some time for existing job execution
		}
	}

}

void JobSchedular::addJobForScheduling(std::shared_ptr<Job> job) {

	std::shared_ptr<IORequest> request = job->getIORequest();//Return const
	IORequest::Type reqType = request->type;
	if(reqType == IORequest::Type::READ) {
		ioQueueManager->addReadJob(job);
	}else if(reqType == IORequest::Type::WRITE) {
		ioQueueManager->addWriteJob(job);
	}//else throw an exception InvalidJobType
	
	//Update Global execution status for job status
	globalExecutionStatus->insertJobIntoMap(job);//map of jobid and job
	std::cout<<"Job is added to I/O req queue. Running a schedular"<<std::endl;
	//New job is added into the Queue. Notify schedular
	std::lock_guard<std::mutex> lock(*run_schedular_cond_mtx);
	run_schedular_cond->notify_one();//We've only one thread for Schedular

}

JobSchedular::~JobSchedular() {
	schedularThread->join();
}
