#include "SyncCoin.h"

namespace msg
{

SyncCoin::SyncCoin()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::DBEntry);
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
	if (!isResult || ResultCode::Success != m_code) return true;
	if ( !AddData(m_coin) ) return false;
	
	return true;
}

bool SyncCoin::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//�������
	if ( !GetData(m_count) ) return false;

	//��Ӧ����
	if (!IsResult() || ResultCode::Success != m_code) return true;

	if (!GetData(m_coin)) return false;

	return true;
}

}
