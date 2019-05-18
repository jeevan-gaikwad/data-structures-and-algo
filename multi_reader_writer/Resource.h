#ifndef _RESOURCE_H_
#define _RESOURCE_H_
#include<iostream>

class Resource {
	std::string id;
public:
	virtual bool open() = 0;
	virtual int  write(std::string buff) = 0;
	virtual int  read(int noOfBytesToRead, std::string& buff) = 0;
 	virtual bool isOpen() = 0;
};


#endif
