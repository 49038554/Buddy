#pragma once
#include "../../Buffer.h"
#include "../../Protocl.h"

namespace msg
{

//购买商品
class GetEvent : public Buffer
{
public:
	//请求参数

	//回应参数

public:
	GetEvent();
	virtual ~GetEvent();
	bool Build();//构造报文，发送
	bool Parse();//解析报文，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}
