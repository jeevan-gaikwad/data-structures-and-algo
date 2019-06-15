#include<iostream>
#include"InputOutputManager.h"
#include<string>

void sample_write_req_thread_func(std::shared_ptr<InputOutputManager> ioManager) {

	for(int itr=0; itr < 5; itr++) {
		//std::cout<<"Sample thread func...sleeping for 2s"<<std::endl;
		std::string buff("Hello world!\n");
		buff += std::to_string(itr);
		jobid_t jobId = ioManager->write(buff);
		std::cout<<"New write req submitted Jobid is:"<<jobId<<std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}

	std::cout<<"Main: Done with 5 write reqs. Setting shutting down flag."<<std::endl;
	ioManager->setShuttingDownFlag();
}

void sample_read_req_thread_func(std::shared_ptr<InputOutputManager> ioManager) {

	for(int itr=0; itr < 5; itr++) {
		std::string buff;
		jobid_t jobId = ioManager->read(6, buff);
		std::cout<<"New read req submitted Jobid is:"<<jobId<<std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}

	std::cout<<"Main: Done with 5 read reqs. Setting shutting down flag."<<std::endl;
	ioManager->setShuttingDownFlag();
}

void show_execution_status(std::shared_ptr<InputOutputManager> ioManager) {

	while(ioManager->isShuttingDown() == false) {
		
		//Display no of write operations performed
		std::cout<<"No fo write operations performed:"<<ioManager->getNoOfWriteOperationsPerformed()<<std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(3));
	}
}

int main(void) {
		std::shared_ptr<InputOutputManager>  io; 
		try {
			io = std::make_shared<InputOutputManager>(TYPE_FILE,"abc.txt"); // OR InputOut io = new InputOut(PIPE,"myPipe");
		}catch(std::exception &e) {
			std::cout<<"Exception occurred while initialzing InputOutputManager:Cause:"<<e.what()<<std::endl;
		}
		std::cout<<"In main. InputOutputManager initialized successfully"<<std::endl;

		if(io->open()) {
			std::cout<<"File opened successfully."<<std::endl;
		}else {
			std::cout<<"Failed to open the file."<<std::endl;
		}

		/*
		int noOfBytesread = io->read(6, emptyBuff);
		noOfBytesread = io->read(6, emptyBuff);
		noOfBytesread = io->read(6, emptyBuff);
		*/
		//I can have multiple thread/program which can call above write and read on io object. InputOut class should ensure that client get the services.
		std::thread writeReqsThread(sample_write_req_thread_func, io);
		//std::thread readReqsThread(sample_read_req_thread_func, io);
		
		std::thread execStatusThread(show_execution_status, io);

		writeReqsThread.join();
		//readReqsThread.join();
		execStatusThread.join();
	return 0;
}
