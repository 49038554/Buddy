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
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//�������

	//��Ӧ����
	if ( !isResult || ResultCode::Success != m_code ) return true;

	return true;
}

bool CreatePlayer::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//�������

	//��Ӧ����
	if ( !IsResult() || ResultCode::Success != m_code ) return true;

	return true;
}

}
