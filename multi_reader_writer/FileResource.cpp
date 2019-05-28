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
	outputStream.flush();
	std::cout<<"Contentes have been written to the file successfully!"<<std::endl;
	return SUCCESS;
}

int  FileResource::read(int noOfBytesToRead, std::string&  buff) {
	std::string str;
	inputStream>>str;
	buff = str.substr(0,noOfBytesToRead);
	std::cout<<"Contentes have been READ from the file successfully!"<<std::endl;
	return SUCCESS;
}

bool FileResource::isOpen() {
	return outputStream.is_open() && inputStream.is_open();
}

FileResource::~FileResource() { 
	std::cout<<"FileResource destructor"<<std::endl;
}

