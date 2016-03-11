#include "SyncPlayer.h"

namespace msg
{

SyncPlayer::SyncPlayer()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::DBEntry);
}

SyncPlayer::~SyncPlayer()
{
}

bool SyncPlayer::Build(bool isResult)
{
	if ( m_player.coin < 0 
		|| m_player.petCount > 6 
		|| m_player.petCount <= 0 )
	{
		return false;
	}
	SetId(MsgId::syncPlayer, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//回应参数
	if ( !AddData(m_player.playerId) )
	{
		return false;
	}
	if ( !AddData(m_player.coin) )
	{
		return false;
	}
	if ( !AddData(m_player.petCount) )
	{
		return false;
	}
	int i = 0;
	for ( i = 0; i < m_player.petCount; i++ )
	{
		if ( !AddData(m_player.pet[i]) )
		{
			return false;
		}
	}
	if ( !AddData(m_player.luckCoin) )
	{
		return false;
	}

	return true;
}

bool SyncPlayer::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//回应参数
	if ( !GetData(m_player.playerId) )
	{
		return false;
	}
	if ( !GetData(m_player.coin) )
	{
		return false;
	}
	if ( m_player.coin < 0 )
	{
		return false;
	}
	if ( !GetData(m_player.petCount) )
	{
		return false;
	}
	if ( m_player.petCount > 6 || m_player.petCount <= 0 )
	{
		return false;
	}
	int i = 0;
	for ( i = 0; i < m_player.petCount; i++ )
	{
		if ( !GetData(m_player.pet[i]) )
		{
			return false;
		}
	}
	if ( !GetData(m_player.luckCoin) )
	{
		return false;
	}

	return true;
}

}
