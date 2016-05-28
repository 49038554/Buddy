#ifndef SENDPET_H
#define SENDPET_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//放出宠物
class SendPet : public Buffer
{
public:
	//请求参数
	int					m_battleId;		//战斗Id
	short				m_petId;		//宠物id

	//回应参数

public:
	SendPet();
	virtual ~SendPet();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //SENDPET_H
