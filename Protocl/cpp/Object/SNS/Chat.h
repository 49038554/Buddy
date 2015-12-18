#ifndef CHAT_H
#define CHAT_H

#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

/*
聊天
*/
class Chat : public Buffer
{
public:
	//请求参数
	unsigned int	m_senderId;//发言人Id	SNS服务填写
	char			m_chatType;//聊天类型，单聊，群聊，...
	unsigned int	m_recverId;//接收方Id
	//回应参数

public:
	Chat();
	virtual ~Chat();
	bool Build(bool isResult = false);//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}


#endif //CHAT_H
