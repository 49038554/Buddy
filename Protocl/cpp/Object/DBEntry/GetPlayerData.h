#ifndef GETPLAYERDATA_H
#define GETPLAYERDATA_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//取玩家数据
class GetPlayerData : public Buffer
{
public:
	//请求参数
	//回应参数

public:
	GetPlayerData();
	virtual ~GetPlayerData();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //GETPLAYERDATA_H
