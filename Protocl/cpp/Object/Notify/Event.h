#pragma once
#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

//购买商品
class Event : public Buffer
{
public:
	enum RecvType
	{
		user = 0,
		buddys = 1,
		group = 2,
	};
	//请求参数
	unsigned int	m_eventId;//事件Id Nofity填写
	unsigned int	m_senderId;//发送者Id
	std::string	m_sender;//发送者昵称
	RecvType	m_recvType;//接收者类型 业务服务器填写
	unsigned int	m_recverId;//接收者Id  业务服务器填写
	int64		m_holdTime;//有效时间，-1永久保存
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
