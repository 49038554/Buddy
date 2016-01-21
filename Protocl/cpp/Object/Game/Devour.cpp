#include "Devour.h"

namespace msg
{

Devour::Devour()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

Devour::~Devour()
{
}

bool Devour::Build(bool isResult)
{
	SetId(MsgId::devour, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
	if ( !AddData(m_itemId) ) return false;

	//回应参数
	if ( !isResult || ResultCode::Success != m_code ) return true;
	if ( !AddData(m_coin) ) return false;

	return true;
}

bool Devour::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数
	if ( !GetData(m_itemId) ) return false;

	//回应参数
	if ( !IsResult() || ResultCode::Success != m_code ) return true;
	if ( !GetData(m_coin) ) return false;

	return true;
}

}
