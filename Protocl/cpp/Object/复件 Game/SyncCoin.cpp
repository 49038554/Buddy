#include "SyncCoin.h"

namespace msg
{

SyncCoin::SyncCoin()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

SyncCoin::~SyncCoin()
{
}

bool SyncCoin::Build(bool isResult)
{
	SetId(MsgId::syncCoin, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp������д����
	//�������
 	if ( !AddData(m_count) ) return false;

	//��Ӧ����

	return true;
}

bool SyncCoin::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//�������
	if ( !GetData(m_count) ) return false;

	//��Ӧ����

	return true;
}

}
