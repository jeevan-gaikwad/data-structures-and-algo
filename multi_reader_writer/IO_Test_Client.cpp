#include<iostream>
#include"InputOutputManager.h"
#include<string>

void sample_thread_fun(std::shared_ptr<InputOutputManager> ioManager) {

	for(int itr=0; itr < 5; itr++) {
		//std::cout<<"Sample thread func...sleeping for 2s"<<std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
	std::cout<<"Main: Done with 5 iterations. Setting shutting down flag."<<std::endl;
	ioManager->setShuttingDownFlag();
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
		std::string buff("Hello world!\n"), emptyBuff;

		//io->write(buff);
		
		std::string buff2("-----How you are doing?\n");
		io->write(buff);
		//io->write(buff2);
		/*
		int noOfBytesread = io->read(6, emptyBuff);
		noOfBytesread = io->read(6, emptyBuff);
		noOfBytesread = io->read(6, emptyBuff);
		*/
		//I can have multiple thread/program which can call above write and read on io object. InputOut class should ensure that client get the services.
		std::thread t(sample_thread_fun, io);
		t.join();
	return 0;
}
