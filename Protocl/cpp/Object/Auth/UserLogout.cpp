#include "UserLogout.h"

namespace msg
{

UserLogout::UserLogout()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Auth);
}

UserLogout::~UserLogout()
{
}

bool UserLogout::Build()
{
	SetId(MsgId::userLogout, false);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	// 请求参数
	AddData((char)m_clientType);//Client类型（回应不填写）

	return true;
}

bool UserLogout::Parse()
{
	if ( !Buffer::Parse() ) return false;

	// 请求参数
	char val;
	if ( !GetData(val) ) return false; m_clientType  = (ClientType::ClientType)val;

	//回应参数
	if ( !IsResult() || ResultCode::success != m_code ) return true;

	return true;
}

}