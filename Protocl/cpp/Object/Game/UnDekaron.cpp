#include "Undekaron.h"

namespace msg
{

Undekaron::Undekaron()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

Undekaron::~Undekaron()
{
}

bool Undekaron::Build(bool isResult)
{
	SetId(MsgId::undekaron, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
	if ( !AddData(m_playerId) ) return false;//被挑战玩家id
	
	//回应参数

	return true;
}

bool Undekaron::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数
	if ( !GetData(m_playerId) ) return false;//被挑战玩家id

	//回应参数

	return true;
}

}
