#ifndef _QUEUE_MANAGER_H_
#define _QUEUE_MANAGER_H_
#include<iostream>
#include<queue>
#include<mutex>

#define MAX_PARALLEL_READ_REQ 10
#define MAX_PARALLEL_WRITE_REQ 1

class IORequest {
	
public:
	int         noOfBytes;
	std::string content;
	
	enum Type { READ, WRITE};
	Type type;
};


class QueueManager {
	std::queue<IORequest> readReqQueue;
	std::queue<IORequest> writeReqQueue;
	std::mutex            readQueueMtx;
	std::mutex            writeQueueMtx;
	int                   maxNoOfReadReq = MAX_PARALLEL_READ_REQ;
	int                   maxNoOfWriteReq = MAX_PARALLEL_WRITE_REQ;

public:
	void addReadReq(IORequest& request);//should throw an exception if max no of READ req limit is reached
	void addWriteReq(IORequest& request);//should thrown an exception if max no of WRITE req limit is reached
	IORequest& getReadReq();
	IORequest& getWriteReq();
	int getWriteReqCurrentSize();
	int getReadReqCurrentSize();
};



#endif
