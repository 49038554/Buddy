#include "LoginState.h"

namespace Cache
{

LoginState::LoginState()
{
	Bind(m_buffer, 1024);//��һ�����������ڴ�֮����ת��
	SetByteOrder( net::smallOrder );//�����ֽ���
	flashOnline = false;		//flash������
	androidOnline = false;		//android������
	iphoneOnline = false;		//iphone������
}

LoginState::~LoginState()
{

}

bool LoginState::Build()
{
	Clear();
	if ( !AddData((char)flashOnline) ) return false;	//flash������
	if ( !AddData((char)androidOnline) ) return false;	//android������
	if ( !AddData((char)iphoneOnline) ) return false;	//iphone������

	return true;
}

bool LoginState::Parse()
{
	if ( -1 == Size() ) return false;
	char val;
	if ( !GetData(val) ) return false;		//flash������
	flashOnline = val;
	if ( !GetData(val) ) return false;	//android������
	androidOnline = val;
	if ( !GetData(val) ) return false;		//iphone������
	iphoneOnline = val;

	return true;
}

}
