#include"QueueManager.h"

void QueueManager::addReadReq(IORequest& request)//should throw an exception if max no of READ req limit is reached
{
	std::lock_guard<std::mutex> lock(readQueueMtx);//Aquire mutex before we access the queue
	if(readReqQueue.size() < maxNoOfReadReq) {
		readReqQueue.push(request);
		std::cout<<"Read request added successfully."<<std::endl;
		std::cout<<"Read queue size is:"<<readReqQueue.size()<<std::endl;
	}//else throw an exception
}

void QueueManager::addWriteReq(IORequest& request)//should thrown an exception if max no of WRITE req limit is reached
{
	std::lock_guard<std::mutex> lock(writeQueueMtx);
	writeReqQueue.push(request); //We can put a limit if needed
	std::cout<<"write request added successfully for:"<<request.content<<std::endl;
	std::cout<<"Write queue size is:"<<writeReqQueue.size()<<std::endl;
}

IORequest& QueueManager::getReadReq() {
	
	std::lock_guard<std::mutex> lock(readQueueMtx);//Aquire mutex before we access the queue
	IORequest& request = readReqQueue.front();
	readReqQueue.pop();
	return request;
}

IORequest& QueueManager::getWriteReq() {

	std::lock_guard<std::mutex> lock(writeQueueMtx);
	IORequest& request = writeReqQueue.front();
	writeReqQueue.pop();
	return request;
}

int QueueManager::getWriteReqCurrentSize() {

	std::lock_guard<std::mutex> lock(writeQueueMtx);
	return writeReqQueue.size();
}

int QueueManager::getReadReqCurrentSize() {
	std::lock_guard<std::mutex> lock(readQueueMtx);//Aquire mutex before we access the queue
	return readReqQueue.size();
}

