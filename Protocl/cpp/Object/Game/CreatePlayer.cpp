#include "CreatePlayer.h"

namespace msg
{

CreatePlayer::CreatePlayer()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

CreatePlayer::~CreatePlayer()
{
}

bool CreatePlayer::Build(bool isResult)
{
	SetId(MsgId::createPlayer, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数

	//回应参数
	if ( !isResult || ResultCode::Success != m_code ) return true;

	return true;
}

bool CreatePlayer::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数

	//回应参数
	if ( !IsResult() || ResultCode::Success != m_code ) return true;

	return true;
}

}
