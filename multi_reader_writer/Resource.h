#ifndef _RESOURCE_H_
#define _RESOURCE_H_
#include<iostream>

const short int SUCCESS = 1;
const short int FAILED = 0;

class Resource {
	std::string id;
public:
	virtual bool open() = 0;
	virtual int  write(std::string buff) = 0;
	virtual int  read(int noOfBytesToRead, std::string& buff) = 0;
 	virtual bool isOpen() = 0;
	//Cleanup is neccessary
	virtual ~Resource() { /* Empty */  } 
	void         setId(std::string id) { this->id = id; }
	std::string  getId() { return id; }

};


#endif
