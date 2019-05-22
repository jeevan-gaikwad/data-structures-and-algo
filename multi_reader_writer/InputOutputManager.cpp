#include<iostream>
#include<thread>
#include <chrono>
#include"InputOutputManager.h"
#include"FileResource.h"

InputOutputManager::InputOutputManager(int resourceType, std::string id) {
		ioQueueManager = std::make_shared<QueueManager>();
		switch(resourceType) {
			case TYPE_FILE:
				 resource = std::make_shared<FileResource>(id);
					break;
			case TYPE_PIPE:
					break;
			
			default: //throw InvalidResourceTypeException()			
				break;
		}
		//Initialize a pool of reader and writer threads which would poll our queues
		createReaderThreads();
		createWriterThreads();
		//std::thread wait(&InputOutputManager::waitForAllThreadsToFinish, this);
		//wait.join();
}

InputOutputManager::~InputOutputManager() {
	waitForAllThreadsToFinish();
}
	
bool InputOutputManager::open()  { // should throw an exception if there is an error while opening the resource
            return resource->open();
}

int  InputOutputManager::write(std::string buff) {
			IORequest writeReq;
			writeReq.type = IORequest::Type::WRITE;
			writeReq.noOfBytes = buff.length();
			writeReq.content = buff;
            ioQueueManager->addWriteReq(writeReq);
}

int  InputOutputManager::read(int noOfBytesToRead, std::string& buff)
{
        IORequest readReq;
		readReq.type = IORequest::Type::READ;
		readReq.noOfBytes = noOfBytesToRead;
		readReq.content = buff;
        ioQueueManager->addReadReq(readReq);    
}
	
//thread function to actually perform read opeation
void InputOutputManager::processReadRequest() {
		static int noOfReadReqsProcessed = 0;
		while(true) {
			std::cout<<"Checking readReqQueue size..."<<std::endl;
			if(ioQueueManager->getReadReqCurrentSize() > 0) {
				IORequest& readReq= ioQueueManager->getReadReq();
				if(noOfReadReqsProcessed > 5 ) {
					noOfReadReqsProcessed = 0;
					std::this_thread::sleep_for(std::chrono::seconds(2)); //For fair share among read and write requests
				}else {
					noOfReadReqsProcessed++;
					resource->read(readReq.noOfBytes, readReq.content);
				}
			} else {
				std::cout<<"Nothing in the queue..sleeping for few seconds"<<std::endl;
				std::this_thread::sleep_for(std::chrono::seconds(3));//sleep if nothing to be processed in the queue
			}
		}
		
}
	
	//thread function to actually perform read opeation
void InputOutputManager::processWriteRequest() {
		 int noOfWriteReqsProcessed = 0;
		 std::cout<<"Writer Thread started executing.."<<std::endl;
		while(true) {	
		/*
			if(ioQueueManager->getWriteReqCurrentSize()> 0 && noOfWriteReqsProcessed > 5 ) {
				noOfWriteReqsProcessed = 0;
				std::this_thread::sleep_for(std::chrono::seconds(2)); //For fair share among read and write requests
			}else if(ioQueueManager->getWriteReqCurrentSize()> 0) {
				IORequest& writeReq= ioQueueManager->getWriteReq();
				noOfWriteReqsProcessed++;
				resource->write(writeReq.content);
			}else {
				std::cout<<"Nothing in the writer queue. Sleeping for few seconds.."<<std::endl;
				std::this_thread::sleep_for(std::chrono::seconds(2));//sleep if nothing to be processed in the queue
			}
			*/
			std::cout<<"Sleeping..."<<std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(2));
		}
}
	
void InputOutputManager::createReaderThreads() {
		//create 10(MAX read threads) threads and pass processIORequest function with ioQueueManager as paramater
		for(int i = 0; i< MAX_NO_OF_READER_THREADS; i++) {
			readerThreadPool[i] = std::make_shared<std::thread>(&InputOutputManager::processReadRequest, this);			
		}
		std::cout<<"Readers thread pool created successfully"<<std::endl;
}
	
void InputOutputManager::createWriterThreads() {
		//create single thread to perform write operation and pass processWriteRequest function as t_func.
		writerThread = std::make_shared<std::thread>(&InputOutputManager::processWriteRequest, this); //we just need single thread to perform write operation.1 at a time.
		//writerThread = new std::thread(&InputOutputManager::processWriteRequest, this);
		//writerThread->join();
		std::cout<<"Writer thread created successfully"<<std::endl;
		
}

void InputOutputManager::waitForAllThreadsToFinish() {
	std::cout<<"Waiting thread started"<<std::endl;
	for(int i=0; i < MAX_NO_OF_READER_THREADS;i++) {
		readerThreadPool[i]->join();
	}
	writerThread->join();
}


