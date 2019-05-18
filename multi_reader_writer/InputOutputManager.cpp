#include<iostream>
#include"InputOutputManager.h"
#include"FileResource.h"

InputOutputManager::InputOutputManager(int resourceType, std::string id) {
		switch(resourceType) {
			case FILE:
				 resource = std::make_shared<FileResource>(id);
					break;
			case PIPE:
					break;
			
			default: //throw InvalidResourceTypeException()			
		}
		//Initialize a pool of reader and writer threads which would poll our queues
		
	}
	
bool InputOutputManager::open()  { // should throw an exception if there is an error while opening the resource
            return resource->open();
}

int  InputOutputManager::InputOutputManager::write(std::string buff) {
			IORequest writeReq;
			writeReq.type = IORequest::Type:WRITE;
			writeReq.noOfBytes = buff.length();
			writeReq.content = buff;
            ioQueueManager->addWriteReq(writeReq);
}

int  InputOutputManager::read(int noOfBytesToRead, std::string& buff)
{
        IORequest readReq;
		readReq.type = IORequest::Type:READ;
		readReq.noOfBytes = noOfBytesToRead;
		readReq.content = buff;
        ioQueueManager->addWriteReq(readReq);    
}
//Internal functions
void InputOutputManager::process_queues() {
             

}
	
//thread function to actually perform read opeation
void InputOutputManager::processReadRequest(void *queueManager) {
		static int noOfReadReqsProcessed;
		std::shared_ptr<QueueManager > ioQueueManager = reinterpret_cast<std::shared_ptr>(queueManager);
		if(ioQueueManager->getReadReqCurrentSize > 0) {
			IORequest& readReq= ioQueueManager->getReadReq();
			if(noOfReadReqsProcessed > 5 ) {
				noOfReadReqsProcessed = 0;
				sleep(5); //For fair share among read and write requests
			}else {
				noOfReadReqsProcessed++;
				resource->read(writeReq.noOfBytes, writeReq.content);
			}
		} else {
			sleep(2);
		}
		
	}
	
	//thread function to actually perform read opeation
void InputOutputManager::processWriteRequest(void *queueManager) {
		static int noOfWriteReqsProcessed = 0;
		
		std::shared_ptr<QueueManager > ioQueueManager = reinterpret_cast<std::shared_ptr>(queueManager);
		IORequest& writeReq= ioQueueManager->getWriteReq();
		if(getWriteReqCurrentSize()> 0 && noOfWriteReqsProcessed > 5 ) {
			noOfWriteReqsProcessed = 0;
			sleep(5); //For fair share among read and write requests
		}else {
			noOfWriteReqsProcessed++;
			resource->write(writeReq.content);
		}
	}
	
void InputOutputManager::createReaderThreads() {
		//create 10(MAX read threads) threads and pass processIORequest function with ioQueueManager as paramater
	}
	
void InputOutputManager::createWriterThreads() {
		//create single thread to perform write operation and pass processWriteRequest function as t_func.
		
		
	}
};

class IORequest {
	
public:
	int         noOfBytes;
	std::string content;
	
	enum Type { READ, WRITE};
	Type type;
};

class QueueManager {
	std::queue<IORequest> readReqQueue;
	std::queue<IORequest> writeReqQueue;
	std::mutex            readQueueMtx;
	std::mutex            writeQueueMtx;
	int                   maxNoOfReadReq = MAX_PARALLEL_READ_REQ;
	int                   maxNoOfWriteReq = MAX_PARALLEL_WRITE_REQ;

public:
	void addReadReq(IORequest& request);//should throw an exception if max no of READ req limit is reached
	void addWriteReq(IORequest& request);//should thrown an exception if max no of WRITE req limit is reached
	IORequest& getReadReq();
	IORequest& getWriteReq();
	int getWriteReqCurrentSize();
	int getReadReqCurrentSize();
};

void QueueManager::addReadReq(IORequest& request)//should throw an exception if max no of READ req limit is reached
{
	std::lock_guard<std::mutex> lock(readQueueMtx);//Aquire mutex before we access the queue
	if(readReqQueue.size() < maxNoOfReadReq) {
		readReqQueue.push(request);
	}//else throw an exception
}
void QueueManager::addWriteReq(IORequest& request);//should thrown an exception if max no of WRITE req limit is reached
{
	std::lock_guard<std::mutex> lock(writeQueueMtx);
	writeReqQueue.push(request); //We can put a limit if needed
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

