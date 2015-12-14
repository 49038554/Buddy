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
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数

	return true;
}

bool GetEvent::Parse()
{
	if ( !Buffer::Parse() ) return false;
	if ( IsResult() ) return false;
	//请求参数

	return true;
}

}
