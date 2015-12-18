#include "GetBuddys.h"


namespace msg
{

GetBuddys::GetBuddys()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::SNS);
	m_userId = 0;
}

GetBuddys::~GetBuddys()
{
}

bool GetBuddys::Build(bool isResult)
{
	SetId(MsgId::getBuddys, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
	
	if ( !AddData(m_userId) ) return false;//用户Id	SNS服务填写

	return true;
}

bool GetBuddys::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数
	if ( !GetData(m_userId) ) return false;//用户Id	SNS服务填写

	return true;
}

}
