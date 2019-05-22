#ifndef _FILE_RESOURCE_H
#define _FILE_RESOURCE_
#include<iostream>
#include<fstream>
#include"Resource.h"

class FileResource: public Resource {

private:
	std::ofstream outputStream;
	std::ifstream inputStream;

public:

	FileResource(std::string name) {
		setId(name);
	}
	bool open();//Open a file. Throwns an exception if open fails
	int  write(std::string buff);
	int  read(int noOfBytesToRead, std::string&  buff);
 	bool isOpen();
};



#endif
