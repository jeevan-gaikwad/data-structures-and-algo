#ifndef _IO_REQUEST_H_
#define _IO_REQUEST_H_
#include<iostream>
#include<memory>
#include"Resource.h"

class IORequest {
	
public:
	int         noOfBytes;
	std::string content;
	std::shared_ptr<Resource> resource;
	
	enum Type { READ, WRITE};
	Type type;
};



#endif
