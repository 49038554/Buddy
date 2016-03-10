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
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
 	if ( !AddData(m_count) ) return false;

	//回应参数
	if (!isResult || ResultCode::Success != m_code) return true;
	if ( !AddData(m_coin) ) return false;
	
	return true;
}

bool SyncCoin::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数
	if ( !GetData(m_count) ) return false;

	//回应参数
	if (!IsResult() || ResultCode::Success != m_code) return true;

	if (!GetData(m_coin)) return false;

	return true;
}

}
