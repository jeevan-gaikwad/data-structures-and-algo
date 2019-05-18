#ifndef _FILE_RESOURCE_H
#define _FILE_RESOURCE_
#include<iostream>

class FileResource: public Resource {

private:
	ofstream outputStream;
	ifstream inputStream;

public:

	FileResource(std::string name) {
		this->id = name;
	}
	void open();//Open a file. Throwns an exception if open fails
	int  write(std::string buff);
	int  read(int noOfBytesToRead, std::string&  buff);
 	bool isOpen();
};



#endif
