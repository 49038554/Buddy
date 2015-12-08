#pragma once
#include "Protocl/cpp/base/Stream.h"
#include <string>

namespace Cache
{

class LoginState : public net::Stream
{
public:
	bool		flashOnline;		//flash������
	bool		androidOnline;		//android������
	bool		iphoneOnline;		//iphone������

public:
	LoginState();
	virtual ~LoginState();

	bool Build();//����
	bool Parse();//����

private:
	unsigned char m_buffer[1024];

};

}
