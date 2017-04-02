#ifndef UNDEKARON_H
#define UNDEKARON_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//挑战取消
class Undekaron : public Buffer
{
public:
	//请求参数
	unsigned int			m_playerId;	// 被挑战玩家id
	//回应参数

public:
	Undekaron();
	virtual ~Undekaron();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //UNDEKARON_H
