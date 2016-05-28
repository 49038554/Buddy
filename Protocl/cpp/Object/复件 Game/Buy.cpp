#include "Buy.h"

namespace msg
{

Buy::Buy()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

Buy::~Buy()
{
}

bool Buy::Build(bool isResult)
{
	SetId(MsgId::buy, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
	if ( !AddData(m_itemId) ) return false;
	if ( !AddData(m_count) ) return false;
	if ( !AddData(m_SellerId) ) return false;
	
	//回应参数

	return true;
}

bool Buy::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数
	if ( !GetData(m_itemId) ) return false;
	if ( !GetData(m_count) ) return false;
	if ( !GetData(m_SellerId) ) return false;

	//回应参数

	return true;
}

}
