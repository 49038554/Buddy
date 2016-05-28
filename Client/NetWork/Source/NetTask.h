#ifndef NETTASK_H
#define NETTASK_H

#include<ctime>

class NetTask
{
public:
	NetTask(void);
	virtual ~NetTask(void);

public:
	enum State
	{
		finished = 0,
		unsend = 1,
		unresult = 2,
	};
	int id;
	short type;
	State state;
	time_t flushTime;
	void *pData;
	int dataSize;
};

#endif //NETTASK_H