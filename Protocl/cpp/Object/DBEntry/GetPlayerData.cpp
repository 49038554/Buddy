#include "GetPlayerData.h"

namespace msg
{

GetPlayerData::GetPlayerData()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::DBEntry);
}

GetPlayerData::~GetPlayerData()
{
}

bool GetPlayerData::Build(bool isResult)
{
	SetId(MsgId::getPlayerData, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
	//回应参数

	return true;
}

bool GetPlayerData::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数
	//回应参数

	return true;
}

}
