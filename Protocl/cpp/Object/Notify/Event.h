#pragma once
#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

//购买商品
class Event : public Buffer
{
public:
	//请求参数
	int			m_senderId;//发送者Id
	std::string	m_sender;//发送者昵称
	int			m_senderlevel;//发送者等级
	Buffer		m_msg;//原始消息
	//回应参数

public:
	Event();
	virtual ~Event();
	bool Build();//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}
