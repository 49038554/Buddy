#include "LoginState.h"

namespace Cache
{

LoginState::LoginState()
{
	Bind(m_buffer, 1024);//��һ�����������ڴ�֮����ת��
	SetByteOrder( net::smallOrder );//�����ֽ���
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
