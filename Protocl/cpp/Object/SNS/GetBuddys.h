#ifndef GET_BUDDY_H
#define GET_BUDDY_H

#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

/*
取伙伴列表
*/
class GetBuddys : public Buffer
{
public:
	//请求参数
	unsigned int	m_userId;//用户Id	SNS服务填写
	//回应参数

public:
	GetBuddys();
	virtual ~GetBuddys();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}


#endif //GET_BUDDY_H
