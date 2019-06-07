#include "QueueManager.h"

void QueueManager::addReadJob(std::shared_ptr<Job> job)//should throw an exception if max no of READ req limit is reached
{
	std::lock_guard<std::mutex> lock(readQueueMtx);//Aquire mutex before we access the queue
	if(readJobQueue.size() < maxNoOfReadJob) {
		readJobQueue.push(job);
		//std::cout<<"Read job added successfully."<<std::endl;
		//std::cout<<"Read queue size is:"<<readJobQueue.size()<<std::endl;
	}//else throw an exception
}

void QueueManager::addWriteJob(std::shared_ptr<Job> job)//should thrown an exception if max no of WRITE req limit is reached
{
	std::lock_guard<std::mutex> lock(writeQueueMtx);
	writeJobQueue.push(job); //We can put a limit if needed
	//std::cout<<"write job added successfully for:"<<job.content<<std::endl;
	//std::cout<<"Write queue size is:"<<writeJobQueue.size()<<std::endl;
}

std::shared_ptr<Job> QueueManager::getReadJob() {
	
	std::lock_guard<std::mutex> lock(readQueueMtx);//Aquire mutex before we access the queue
	std::shared_ptr<Job> job = readJobQueue.front();
	readJobQueue.pop();
	return job;
}

std::shared_ptr<Job> QueueManager::getWriteJob() {

	std::lock_guard<std::mutex> lock(writeQueueMtx);
	std::shared_ptr<Job> job = writeJobQueue.front();
	writeJobQueue.pop();
	return job;
}

int QueueManager::getWriteJobCurrentQSize() {

	std::lock_guard<std::mutex> lock(writeQueueMtx);
	return writeJobQueue.size();
}

int QueueManager::getReadJobCurrentQSize() {
	std::lock_guard<std::mutex> lock(readQueueMtx);//Aquire mutex before we access the queue
	return readJobQueue.size();
}

