#ifndef DEL_BUDDY_H
#define DEL_BUDDY_H

#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

/*
删除伙伴
*/
class DelBuddy : public Buffer
{
public:
	//请求参数
	unsigned int	m_userId;//用户Id	SNS服务填写
	unsigned int	m_buddyId;//伙伴Id

public:
	DelBuddy();
	virtual ~DelBuddy();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}

#endif //DEL_BUDDY_H

