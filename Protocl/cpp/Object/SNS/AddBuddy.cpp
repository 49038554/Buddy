#include "AddBuddy.h"


namespace msg
{

AddBuddy::AddBuddy()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::SNS);
	m_userId = 0;
	m_step = 0;
	m_nickName = "NULL";
	m_msg = "Hey Buddy。一起玩吧";
	m_accepted = false;
}

AddBuddy::~AddBuddy()
{
}

bool AddBuddy::Build(bool isResult)
{
	SetId(MsgId::addBuddy, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
	
	if ( !AddData(m_userId) ) return false;//用户Id
	if ( !AddData(m_nickName) ) return false;//昵称
	if ( !AddData(m_buddyId) ) return false;//伙伴Id
	if ( !AddData(m_step) ) return false;//当前步骤
	if ( !AddData(m_msg) ) return false;//消息
	else if ( 1 == m_step )
	{
		if ( !AddData((char)(m_accepted?1:0)) ) return false;//接受
	}

	return true;
}

bool AddBuddy::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数
	if ( !GetData(m_userId) ) return false;//用户Id
	if ( !GetData(m_nickName) ) return false;//昵称
	if ( !GetData(m_buddyId) ) return false;//伙伴Id
	if ( !GetData(m_step) ) return false;//当前步骤
	if ( !GetData(m_msg) ) return false;//消息
	if ( 1 == m_step )
	{
		char val;
		if ( !GetData(val) ) return false;//接受
		m_accepted = 1 == val?true:false;
	}

	return true;
}

}
