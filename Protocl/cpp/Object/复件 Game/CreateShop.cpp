#include "CreateShop.h"

namespace msg
{

CreateShop::CreateShop()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

CreateShop::~CreateShop()
{
}

bool CreateShop::Build(bool isResult)
{
	SetId(MsgId::createShop, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
	if ( !AddData(m_longitude) ) return false;
	if ( !AddData(m_latitude) ) return false;
	if ( !AddData(m_address) ) return false;

	//回应参数

	return true;
}

bool CreateShop::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数
	if ( !GetData(m_longitude) ) return false;
	if ( !GetData(m_latitude) ) return false;
	if ( !GetData(m_address) ) return false;

	//回应参数

	return true;
}

}
