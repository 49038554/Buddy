#pragma once
#include "../Buffer.h"

namespace msg
{

//连接认证
class ConnectAuth : public Buffer
{
public:
	//请求参数
	Moudle::Moudle	m_moudleId;//模块Id
	int				m_nodeId;//结点Id
	//回应参数

public:
	ConnectAuth();
	virtual ~ConnectAuth();
	bool Build();//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}
