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
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//�������
	
	if ( !AddData(m_userId) ) return false;//�û�Id

	return true;
}

bool GetUserData::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//�������
	if ( !GetData(m_userId) ) return false;//�û�Id	SNS������д

	return true;
}

}
