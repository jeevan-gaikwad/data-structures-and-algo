#include"FileResource.h"
#include<string>
bool FileResource::open()//Open a file. Throwns an exception if open fails
{
	outputStream.open(getId());
	inputStream.open(getId());

	return outputStream.is_open() && inputStream.is_open();
}

int  FileResource::write(std::string buff){
	outputStream<<buff;
}

int  FileResource::read(int noOfBytesToRead, std::string&  buff) {
	std::string str;
	inputStream>>str;
	buff = str.substr(0,noOfBytesToRead);
}

bool FileResource::isOpen() {
	return outputStream.is_open() && inputStream.is_open();
}

