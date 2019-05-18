#include<iostream>
#include"InputOutputManager.h"
#include<string>
int main(void) {

		std::shared_ptr<InputOutManager>  io = std::make_shared<InputOut>(FILE,"abc.txt"); // OR InputOut io = new InputOut(PIPE,"myPipe");

		io.open();
		std::string buff("Hello world!"), emptyBuff;
		io.write(buff);
		int noOfBytesread = io.read(emptyBuff,12);
		//I can have multiple thread/program which can call above write and read on io object. InputOut class should ensure that client get the services.
	return 0;
}
