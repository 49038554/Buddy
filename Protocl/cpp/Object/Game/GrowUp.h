#ifndef GROWUP_H
#define GROWUP_H

#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

//进化
class GrowUp : public Buffer
{
public:
	//请求参数
	unsigned int	m_petId;			//宠物id
	//回应参数

public:
	GrowUp();
	virtual ~GrowUp();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}


#endif //GROWUP_H
