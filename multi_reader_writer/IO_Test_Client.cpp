#include<iostream>
#include"InputOutputManager.h"
#include<string>
void sample_thread_fun() {

	while(true) {
		std::cout<<"Sample thread func...sleeping for 2s"<<std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));
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
		std::thread t(sample_thread_fun);
		/*	
		if(io->open()) {
			std::cout<<"File opened successfully."<<std::endl;
		}else {
			std::cout<<"Failed to open the file."<<std::endl;
		}
		*/
		std::string buff("Hello world!"), emptyBuff;
		/*
		io->write(buff);
		io->write(buff);
		int noOfBytesread = io->read(6, emptyBuff);
		noOfBytesread = io->read(6, emptyBuff);
		noOfBytesread = io->read(6, emptyBuff);
		*/
		//I can have multiple thread/program which can call above write and read on io object. InputOut class should ensure that client get the services.
	return 0;
}
