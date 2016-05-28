#include "BattleResult.h"

namespace msg
{

BattleResult::BattleResult()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

BattleResult::~BattleResult()
{
}

bool BattleResult::Build(bool isResult)
{
	SetId(MsgId::battleResult, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
	if ( !AddData(m_winnerId) ) return false;		//胜方id
	if ( !AddData(m_loserId) ) return false;		//负方id
	
	//回应参数

	return true;
}

bool BattleResult::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数
	if ( !GetData(m_winnerId) ) return false;		//胜方id
	if ( !GetData(m_loserId) ) return false;		//负方id

	//回应参数

	return true;
}

}
