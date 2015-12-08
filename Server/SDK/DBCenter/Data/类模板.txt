#include "LoginState.h"

namespace Cache
{

LoginState::LoginState()
{
	Bind(m_buffer, 1024);//绑定一个缓冲区，在此之上做转换
	SetByteOrder( net::smallOrder );//设置字节序
}

LoginState::~LoginState()
{

}

bool LoginState::Build()
{

	return true;
}

bool LoginState::Parse()
{
	if ( -1 == Size() ) return false;

	return true;
}

}
