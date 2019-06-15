#include <iostream>
#include <thread>
#include <chrono>

#include "InputOutputManager.h"
#include "FileResource.h"

InputOutputManager::InputOutputManager(int resourceType, std::string id) {
		globalExecutionStatus  = std::make_shared<GlobalExecutionStatus>();
		jobManager = std::make_shared<JobManager>(globalExecutionStatus);

		switch(resourceType) {
			case TYPE_FILE:
				 resource = std::make_shared<FileResource>(id);
					break;
			case TYPE_PIPE:
				// Unimplemented
					break;
			
			default: //throw InvalidResourceTypeException()			
				break;
		}
}

InputOutputManager::~InputOutputManager() {
	//For now, nothing to be cleaned up explicitely
}
	
bool InputOutputManager::open()  { // should throw an exception if there is an error while opening the resource

    return resource->open();
}

jobid_t  InputOutputManager::read(int noOfBytesToRead, std::string& buff)
{
    std::shared_ptr<IORequest> readReq = std::make_shared<IORequest>();
	readReq->type = IORequest::Type::READ;
	readReq->noOfBytes = noOfBytesToRead;
	readReq->content = buff;
	readReq->resource = resource; // Read operation would be performed on this request
    return jobManager->createJob(readReq);  //returns jobid
}
	
jobid_t  InputOutputManager::write(std::string buff) {

	std::shared_ptr<IORequest> writeReq = std::make_shared<IORequest>();
	writeReq->type = IORequest::Type::WRITE;
	writeReq->noOfBytes = buff.length();
	writeReq->content = buff;
	writeReq->resource = resource; // Write operation would be performed on this request

	return jobManager->createJob(writeReq); //returns jobid
}


const std::shared_ptr<Job> InputOutputManager::getJobExecInfo(jobid_t jobId) {
	return jobManager->getJob(jobId);//Throw JobNotFound exception

}

void InputOutputManager::setShuttingDownFlag() {
	globalExecutionStatus->setIsShuttingDown(true);
}

int InputOutputManager::getNoOfWriteOperationsPerformed() {
	return globalExecutionStatus->getNoOfWriteOperationsPerformed();
}

bool InputOutputManager::isShuttingDown() {
	return globalExecutionStatus->getIsShuttingDown();
}

