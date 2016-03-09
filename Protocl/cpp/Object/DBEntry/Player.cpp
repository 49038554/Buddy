#include "Player.h"

namespace msg
{

Player::Player()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::DBEntry);
}

Player::~Player()
{
}

bool Player::Build()
{
	if ( m_coin < 0 ) 
	{
		return false;
	}
	SetId(MsgId::player, true);
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//��Ӧ����
	if ( !AddData(m_coin) )
	{
		return false;
	}

	return true;
}

bool Player::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//��Ӧ����
	if ( !GetData(m_coin) )
	{
		return false;
	}

	return true;
}

}
