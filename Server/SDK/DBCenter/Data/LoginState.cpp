#include "LoginState.h"

namespace Cache
{

LoginState::LoginState()
{
	Bind(m_buffer, 1024);//绑定一个缓冲区，在此之上做转换
	SetByteOrder( net::smallOrder );//设置字节序
	flashOnline = false;		//flash端在线
	androidOnline = false;		//android端在线
	iphoneOnline = false;		//iphone端在线
}

LoginState::~LoginState()
{

}

bool LoginState::Build()
{
	Clear();
	if ( !AddData((char)flashOnline) ) return false;	//flash端在线
	if ( !AddData((char)androidOnline) ) return false;	//android端在线
	if ( !AddData((char)iphoneOnline) ) return false;	//iphone端在线

	return true;
}

bool LoginState::Parse()
{
	if ( -1 == Size() ) return false;
	char val;
	if ( !GetData(val) ) return false;		//flash端在线
	flashOnline = val;
	if ( !GetData(val) ) return false;	//android端在线
	androidOnline = val;
	if ( !GetData(val) ) return false;		//iphone端在线
	iphoneOnline = val;

	return true;
}

}
