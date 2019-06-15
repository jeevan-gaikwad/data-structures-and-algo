#ifndef _GLOBAL_EXEC_STS_H_
#define _GLOBAL_EXEC_STS_H_

#include <iostream>
#include <memory>
#include <condition_variable>
#include <mutex>
#include <map>
#include <vector>
#include "Job.h"

enum EventType { SHUTDOWN };

class EventListener {

public:
	std::shared_ptr<std::condition_variable> condition_var;
	std::shared_ptr<std::mutex> condition_var_mtx;
	EventType eventType; //we can have Event as a seperate struct to store more information about an event
	
};

class GlobalExecutionStatus {

private: 

	std::vector<EventListener> eventListeners; //We can have a map<EventType, std::vector<EventListeners> >
	std::map<jobid_t, std::shared_ptr<Job> > jobsMap;//we can also use unordered map for performance, but in future if we start supporting an API like getAllJobs, then it wont be possible through std::map
	std::mutex jobsMap_mtx;

	bool isShuttingDown; //Need to have explicit synchronization mechanism for this instead of local lock_gurad
	std::mutex isShuttingDown_mtx;

	/*Following isWriteOperationInProgress, noOfReadOperationsInProgress  and noOfWriteOperationsPerformed are used to avoid startvation of the requests. And JobSchedular uses these variables. */

	bool writeOperationInProgress;
	std::mutex isWriteOperationInProgress_mtx;

	int  noOfReadOperationsInProgress;
	std::mutex noOfReadOperationsInProgress_mtx;

	int  noOfWriteOperationsPerformed;
	std::mutex noOfWriteOperationsPerformed_mtx;

	int  noOfReadOperationsPerformed;
	std::mutex noOfReadOperationsPerformed_mtx;

public:
	GlobalExecutionStatus();
	void insertJobIntoMap(std::shared_ptr<Job> job);
	void setIsShuttingDown(bool isShuttingDown);
	bool getIsShuttingDown();

	void setIsWriteOperationInProgress(bool isWriteOperationInProgress);
	bool isWriteOperationInProgress();

	void incrementNoOfReadOperationsInProgressByOne();
	void decrementNoOfReadOperationsInProgressByOne();

	//Following functions are helpful for job scheduling
	//READ
	void incrementNoOfReadOperationsPerformed();
	void decrementNoOfReadOperationsPerformed();
	int  getNoOfReadOperationsPerformed();
	void resetNoOfReadOperationsPerformed();

	//WRITE
	void incrementNoOfWriteOperationsPerformed();
	void decrementNoOfWriteOperationsPerformed();
	int  getNoOfWriteOperationsPerformed();
	void resetNoOfWriteOperationsPerformed();
	
	void registerEventListener(EventListener& eventListener);
	const std::shared_ptr<Job>  getJob(jobid_t id);
};

#endif
