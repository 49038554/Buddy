#include "SetupVersion.h"


namespace msg
{

SetupVersion::SetupVersion()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
	m_dataVersion = 0;
}

SetupVersion::~SetupVersion()
{
}

bool SetupVersion::Build(bool isResult)
{
	SetId(MsgId::setupVersion, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
	//请求参数
	if ( !AddData(m_dataVersion) ) return false;

	//回应参数

	return true;
}

bool SetupVersion::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数
	if ( !GetData(m_dataVersion) ) return false;

	//回应参数

	return true;
}

}
