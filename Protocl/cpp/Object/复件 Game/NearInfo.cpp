#include "NearInfo.h"

namespace msg
{

NearInfo::NearInfo()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

NearInfo::~NearInfo()
{
}

bool NearInfo::Build(bool isResult)
{
	SetId(MsgId::nearInfo, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
	if ( !AddData(m_type) ) return false;

	//回应参数

	return true;
}

bool NearInfo::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数
	if ( !GetData(m_type) ) return false;

	//回应参数

	return true;
}

}
