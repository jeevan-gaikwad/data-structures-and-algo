#ifndef _JOB_H_
#define _JOB_H_

#include<iostream>
#include<chrono>
#include<ctime>

using jobid_t = long int;

class Job {

private:
	jobid_t id;
	float progressPercentage;
	std::time_t startTime, completionTime;
//	std::chrono::time_point<std::chrono::system_clock> startTime, completionTime;


public:
	Job() { }
	Job(jobid_t id, float progressPercentage, std::time_t startTime,std::time_t completionTime):
	id(id), progressPercentage(progressPercentage), startTime(startTime), completionTime(completionTime) { }
	//Getter and setters
	jobid_t     getJobId() { return id; }
	void        setJobId(jobid_t id) { this->id = id; }
	
	float       getProgressPercentage() { return progressPercentage; }
	void        setProgressPercentage(float progressPercentage) { this->progressPercentage = progressPercentage; }

	std::time_t getStartTime() { return startTime; }
	void        setStartTime(std::time_t startTime) { this->startTime = startTime; }

	
	std::time_t getCompletionTime() { return completionTime; }
	void        setCompletionTime(std::time_t completionTime) { this->completionTime = completionTime; }

};

#endif
