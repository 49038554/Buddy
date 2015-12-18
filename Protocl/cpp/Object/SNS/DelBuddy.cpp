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
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//�������
	if ( !AddData(m_userId) ) return false;//�û�Id	SNS������д
	if ( !AddData(m_buddyId) ) return false;//���Id

	return true;
}

bool DelBuddy::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//�������
	if ( !GetData(m_userId) ) return false;//�û�Id
	if ( !GetData(m_buddyId) ) return false;//���Id

	return true;
}

}
