#pragma once
#include "Protocl/cpp/base/Stream.h"
#include <string>

namespace Cache
{

class LoginState : public net::Stream
{
public:
	bool		flashOnline;		//flash端在线
	bool		androidOnline;		//android端在线
	bool		iphoneOnline;		//iphone端在线

public:
	LoginState();
	virtual ~LoginState();

	bool Build();//构造
	bool Parse();//解析

private:
	unsigned char m_buffer[1024];

};

}
