#include "GrowUp.h"


namespace msg
{

GrowUp::GrowUp()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

GrowUp::~GrowUp()
{
}

bool GrowUp::Build(bool isResult)
{
	SetId(MsgId::growUp, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
	if ( !AddData(m_petId) ) return false;

	return true;
}

bool GrowUp::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数
	if ( !GetData(m_petId) ) return false;

	return true;
}

}
