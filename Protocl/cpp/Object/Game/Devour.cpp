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
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//�������
	if ( !AddData(m_itemId) ) return false;

	//��Ӧ����
	if ( !isResult || ResultCode::Success != m_code ) return true;
	if ( !AddData(m_coin) ) return false;

	return true;
}

bool Devour::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//�������
	if ( !GetData(m_itemId) ) return false;

	//��Ӧ����
	if ( !IsResult() || ResultCode::Success != m_code ) return true;
	if ( !GetData(m_coin) ) return false;

	return true;
}

}
