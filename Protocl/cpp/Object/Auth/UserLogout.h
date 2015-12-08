#pragma once
#include "../../Buffer.h"
#include <string>

namespace msg
{

//登出
class UserLogout : public Buffer
{
public:
	//请求参数
	ClientType::ClientType		m_clientType;//Client类型（回应不填写）

public:
	UserLogout();
	virtual ~UserLogout();
	bool Build();//构造报文，发送
	bool Parse();//解析解析，接收

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}