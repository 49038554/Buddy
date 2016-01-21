#include "TameBeast.h"


namespace msg
{

TameBeast::TameBeast()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

TameBeast::~TameBeast()
{
}

bool TameBeast::Build(bool isResult)
{
	SetId(MsgId::tameBeast, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
	if ( !AddData(m_petId) ) return false;
	if ( !AddData(m_buddyNo) ) return false;
	if ( !AddData(m_nickname) ) return false;
	if ( !AddData(m_longitude) ) return false;
	if ( !AddData(m_latitude) ) return false;
	if ( !AddData(m_address) ) return false;
	
	return true;
}

bool TameBeast::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数
	if ( !GetData(m_petId) ) return false;
	if ( !GetData(m_buddyNo) ) return false;
	if ( !GetData(m_nickname) ) return false;
	if ( !GetData(m_longitude) ) return false;
	if ( !GetData(m_latitude) ) return false;
	if ( !GetData(m_address) ) return false;

	return true;
}

}
