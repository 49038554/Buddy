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
	enum RecvType
	{
		buddy = 0,
		buddys = 1,
		group = 2,
	};
	//请求参数
	unsigned int	m_senderId;//发言人Id	SNS服务填写
	std::string		m_senderName;//发言人昵称	SNS服务填写
	std::string		m_senderFace;//发言人头像	SNS服务填写
	RecvType		m_recvType;//接收方类型
	unsigned int	m_recverId;//接收方Id
	std::string		m_talk;//聊天内容
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
