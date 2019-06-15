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

		std::unique_lock<std::mutex> lock(*run_schedular_cond_mtx);
		std::cout<<"runSchedular is now going to wait until someone wakesup.."<<std::endl;
		run_schedular_cond->wait(lock, std::bind(&JobSchedular::isSchedularCondMet, this));
		
		if(globalExecutionStatus->getIsShuttingDown() == true) { //This is to ensure that after waking up, system is NOT shutting down
			std::cout<<"System is shutting down. Terminating schedular thread."<<std::endl;
			break;
		}

		/* 
			1. Ensure that only one WRITE job is running
			2. No starvation for read/write requests
		*/	
		std::cout<<"Running job schedular."<<std::endl;
		if(ioQueueManager->getWriteJobCurrentQSize() > 0) {
			jobExecutor->addJobForExecution(ioQueueManager->getWriteJob());
		}

		if(ioQueueManager->getReadJobCurrentQSize() > 0) {
			jobExecutor->addJobForExecution(ioQueueManager->getReadJob());
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
