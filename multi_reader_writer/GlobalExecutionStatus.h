#ifndef _GLOBAL_EXEC_STS_H_
#define _GLOBAL_EXEC_STS_H_
#include<iostream>
#include<memory>
#include<mutex>
#include"Job.h"

class GlobalExecutionStatus {

private: 
	std::shared_ptr<std::map<jobid_t, std::shared_ptr<Job> >> jobsMap;//we can also use unordered map for performance, but in future if we start supporting an API like getAllJobs, then it wont be possible through std::map
	std::mutex jobsMap_mtx;

	bool isShuttingDown; //Need to have explicit synchronization mechanism for this instead of local lock_gurad

	/*Following isWriteOperationInProgress, noOfReadOperationsInProgress  and noOfWriteOperationsPerformed are used to avoid startvation of the requests. And JobSchedular uses these variables. */

	bool isWriteOperationInProgress;

	int  noOfReadOperationsInProgress;

	int  noOfWriteOperationsPerformed;

public:
	void setIsShuttingDown(bool isShuttingDown);
	bool getIsShuttingDown();

	void setIsWriteOperationInProgress(bool isWriteOperationInProgress);
	bool getIsWriteOperationInProgress();

	void incrementNoOfReadOperationsInProgressByOne();
	void derementNoOfReadOperationsInProgressByOne();

	void incrementNoOfWriteOperationsPerformed();
	void decrementNoOfWriteOperationsPerformed();
};
#endif
