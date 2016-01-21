#include "DelHouse.h"

namespace msg
{

DelHouse::DelHouse()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

DelHouse::~DelHouse()
{
}

bool DelHouse::Build(bool isResult)
{
	SetId(MsgId::delHouse, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
	if ( !AddData(m_buildId) ) return false;
	
	//回应参数

	return true;
}

bool DelHouse::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数
	if ( !GetData(m_buildId) ) return false;
	
	//回应参数

	return true;
}

}
