#include "GetUserData.h"


namespace msg
{

GetUserData::GetUserData()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::SNS);
	m_userId = 0;
}

GetUserData::~GetUserData()
{
}

bool GetUserData::Build(bool isResult)
{
	SetId(MsgId::getUserData, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
	
	if ( !AddData(m_userId) ) return false;//用户Id

	return true;
}

bool GetUserData::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数
	if ( !GetData(m_userId) ) return false;//用户Id	SNS服务填写

	return true;
}

}
