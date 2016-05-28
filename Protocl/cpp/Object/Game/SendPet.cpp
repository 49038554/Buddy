#include "SendPet.h"

namespace msg
{

SendPet::SendPet()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

SendPet::~SendPet()
{
}

bool SendPet::Build(bool isResult)
{
	SetId(MsgId::sendPet, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
	if ( !AddData(m_battleId) ) return false;		//战斗Id
	if ( !AddData(m_petId) ) return false;		//宠物id

	//回应参数

	return true;
}

bool SendPet::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数
	if ( !GetData(m_battleId) ) return false;		//战斗Id
	if ( !GetData(m_petId) ) return false;		//宠物id

	//回应参数

	return true;
}

}
