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
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
 	if ( !AddData(m_count) ) return false;

	//回应参数

	return true;
}

bool SyncCoin::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数
	if ( !GetData(m_count) ) return false;

	//回应参数

	return true;
}

}
