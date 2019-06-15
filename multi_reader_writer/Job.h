#ifndef _JOB_H_
#define _JOB_H_

#include<iostream>
#include<chrono>
#include<ctime>
#include"IORequest.h"

using jobid_t = long int;

class Job {

public:
	enum Status { QUEUED, IN_PROGRESS, COMPLETED, ERROR};

private:
	jobid_t id;
	float progressPercentage;
	std::time_t startTime, completionTime;
	std::shared_ptr<IORequest> ioRequest;
	Status status;

public:

	Job() { }
	Job(jobid_t id, float progressPercentage, std::time_t startTime,std::time_t completionTime, std::shared_ptr<IORequest> ioRequest):
	id(id), progressPercentage(progressPercentage), startTime(startTime), completionTime(completionTime), ioRequest(ioRequest) { }
	//Getter and setters
	jobid_t     getJobId() { return id; }
	void        setJobId(jobid_t id) { this->id = id; }
	
	float       getProgressPercentage() { return progressPercentage; }
	void        setProgressPercentage(float progressPercentage) { this->progressPercentage = progressPercentage; }

	std::time_t getStartTime() { return startTime; }
	void        setStartTime(std::time_t startTime) { this->startTime = startTime; }

	
	std::time_t getCompletionTime() { return completionTime; }
	void        setCompletionTime(std::time_t completionTime) { this->completionTime = completionTime; }

	std::shared_ptr<IORequest>  getIORequest() { return ioRequest; }
	void 	    setIORequest(std::shared_ptr<IORequest> ioRequest) { this->ioRequest = ioRequest; }
	
	void        setStatus(Status status) { this->status = status; }
	Status		getStatus() { this->status; }
};
//aliases
using job_status_t = Job::Status;
#endif
