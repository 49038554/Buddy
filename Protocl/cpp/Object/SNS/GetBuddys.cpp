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
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//�������
	
	if ( !AddData(m_userId) ) return false;//�û�Id	SNS������д

	return true;
}

bool GetBuddys::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//�������
	if ( !GetData(m_userId) ) return false;//�û�Id	SNS������д

	return true;
}

}
