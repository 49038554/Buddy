#include "GetEvent.h"


namespace msg
{

GetEvent::GetEvent()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Notify);
}

GetEvent::~GetEvent()
{
}

bool GetEvent::Build(bool isResult)
{
	SetId(MsgId::getEvent, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//�������

	return true;
}

bool GetEvent::Parse()
{
	if ( !Buffer::Parse() ) return false;
	if ( IsResult() ) return false;
	//�������

	return true;
}

}
