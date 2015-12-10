#pragma once
#include "Protocl/cpp/base/Stream.h"

namespace Cache
{

class LoginState : public net::Stream
{
public:

public:
	LoginState();
	virtual ~LoginState();

	bool Build();//ππ‘Ï
	bool Parse();//Ω‚Œˆ

private:
	unsigned char m_buffer[1024];

};

}
