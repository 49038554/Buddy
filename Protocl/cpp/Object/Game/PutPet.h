#ifndef PUTPET_H
#define PUTPET_H

#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

//移动宠物
class PutPet : public Buffer
{
public:
	//请求参数
	unsigned int	m_petId;			//宠物id
	bool			m_toHouse;//身上到寄存
	short			m_houseId;//建筑id，1开始

public:
	PutPet();
	virtual ~PutPet();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}

#endif //PUTPET_H

