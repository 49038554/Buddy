#include "RoundReady.h"

namespace msg
{

RoundReady::RoundReady()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

RoundReady::~RoundReady()
{
}

bool RoundReady::Build(bool isResult)
{
	SetId(MsgId::roundReady, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
	if ( !AddData(m_battleId) ) return false;		//战斗id
	if ( !AddData((char)m_action) ) return false;		//动作
	if ( !AddData(m_gameObjId) ) return false;		//对象id
	//随机数
	if ( !AddData(m_rp.miss) ) return false;//命中随机数
	if ( !AddData(m_rp.sePro) ) return false;//技能特效随机数
	if ( !AddData(m_rp.iePro) ) return false;//物品特效随机数
	if ( !AddData(m_rp.tePro) ) return false;//特性特效随机数
	if ( !AddData(m_rp.luanWu) ) return false;//乱舞回合数
	if ( !AddData(m_rp.sleepRound) ) return false;//睡眠回合随机数
	if ( !AddData(m_rp.frozenRound) ) return false;//冰冻回合随机数
	if ( !AddData(m_rp.dian) ) return false;//麻痹随机数
	char val = m_rp.luan?1:0;
	if ( !AddData(val) ) return false;//混乱随机数
	if ( !AddData(m_rp.hurt) ) return false;//伤害随机数217~255
	if ( !AddData(m_rp.speed) ) return false;//速度随机数
	if ( !AddData(m_rp.fouceChange) ) return false;//强制换上第?只Pet，随机数1~5
	
	//回应参数

	return true;
}

bool RoundReady::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数
	if ( !GetData(m_battleId) ) return false;		//战斗Id
	char val;
	if ( !GetData(val) ) return false;		//动作
	m_action = val;
	if ( !GetData(m_gameObjId) ) return false;		//对象id
	//随机数
	if ( !GetData(m_rp.miss) ) return false;//命中随机数
	if ( !GetData(m_rp.sePro) ) return false;//技能特效随机数
	if ( !GetData(m_rp.iePro) ) return false;//物品特效随机数
	if ( !GetData(m_rp.tePro) ) return false;//特性特效随机数
	if ( !GetData(m_rp.luanWu) ) return false;//乱舞回合数
	if ( !GetData(m_rp.sleepRound) ) return false;//睡眠回合随机数
	if ( !GetData(m_rp.frozenRound) ) return false;//冰冻回合随机数
	if ( !GetData(m_rp.dian) ) return false;//麻痹随机数
	if ( !GetData(val) ) return false;//混乱随机数
	m_rp.luan = 1 == val?true:false;
	if ( !GetData(m_rp.hurt) ) return false;//伤害随机数217~255
	if ( !GetData(m_rp.speed) ) return false;//速度随机数
	if ( !GetData(m_rp.fouceChange) ) return false;//强制换上第?只Pet，随机数1~5

	//回应参数

	return true;
}

}
