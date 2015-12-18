#include "DelBuddy.h"


namespace msg
{

DelBuddy::DelBuddy()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::SNS);
	m_userId = 0;
}

DelBuddy::~DelBuddy()
{
}

bool DelBuddy::Build(bool isResult)
{
	SetId(MsgId::delBuddy, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
	if ( !AddData(m_userId) ) return false;//用户Id	SNS服务填写
	if ( !AddData(m_buddyId) ) return false;//伙伴Id

	return true;
}

bool DelBuddy::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数
	if ( !GetData(m_userId) ) return false;//用户Id
	if ( !GetData(m_buddyId) ) return false;//伙伴Id

	return true;
}

}
