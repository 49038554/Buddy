#ifndef NETTASK_H
#define NETTASK_H

#include<ctime>

class NetTask
{
public:
	NetTask(void);
	virtual ~NetTask(void);

public:
	//通信作用
	enum Type
	{
		vs = 0,//对战
		save = 1,//保存数据
	};
	short type;
	void *pData;//数据
	int dataSize;//pData指向buffer中数据实际大小
	int spaceSize;//pData指向buffer空间
};

#endif //NETTASK_H