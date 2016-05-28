#include "Dekaron.h"

namespace msg
{

Dekaron::Dekaron()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

Dekaron::~Dekaron()
{
}

bool Dekaron::Build(bool isResult)
{
	SetId(MsgId::dekaron, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
	if ( !AddData(m_battleId) ) return false;//战斗Id Game服务器统一设置
	if ( !AddData(m_playerId) ) return false;//被挑战玩家id
	if ( !AddData(m_nick) ) return false;		// 挑战者昵称
	if ( !AddData(m_pet.size()) ) return false;		// 挑战者昵称

	int i = 0;
	for ( i = 0; i < m_pet.size(); i++ )
	{
		if ( !AddData(m_pet[i].id) ) return false;//宠物Id
		if ( !AddData(m_pet[i].number) ) return false;//巴迪编号
		if ( !AddData(m_pet[i].talent) ) return false;//特性
		if ( !AddData(m_pet[i].nature) ) return false;//性格
		if ( !AddData(m_pet[i].skill1) ) return false;//技能1
		if ( !AddData(m_pet[i].skill2) ) return false;//技能2
		if ( !AddData(m_pet[i].skill3) ) return false;//技能3
		if ( !AddData(m_pet[i].skill4) ) return false;//技能4
		if ( !AddData(m_pet[i].itemId) ) return false;//物品id
		if ( !AddData(m_pet[i].HP) ) return false;//血
		if ( !AddData(m_pet[i].WG) ) return false;//攻
		if ( !AddData(m_pet[i].WF) ) return false;//防
		if ( !AddData(m_pet[i].TG) ) return false;//特攻
		if ( !AddData(m_pet[i].TF) ) return false;//特防
		if ( !AddData(m_pet[i].SD) ) return false;//速度
		if ( !AddData(m_pet[i].nick) ) return false;//昵称
	}
	
	//回应参数

	return true;
}

bool Dekaron::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数
	if ( !GetData(m_battleId) ) return false;//战斗Id Game服务器统一设置
	if ( !GetData(m_playerId) ) return false;//被挑战玩家id
	if ( !GetData(m_nick) ) return false;		// 挑战者昵称
	int count = 0;
	if ( !GetData(count) ) return false;		// 挑战者昵称
	if ( 0 >= count || 6 < count ) return false;

	int i = 0;
	data::PET info;
	info.state = 0;//状态
	info.sleepRound = 0;//催眠剩余回合
	info.frozenRound = 0;//冰封剩余回合
	for ( i = 0; i < count; i++ )
	{
		if ( !GetData(info.id) ) return false;//宠物Id
		if ( !GetData(info.number) ) return false;//巴迪编号
		if ( !GetData(info.talent) ) return false;//特性
		if ( !GetData(info.nature) ) return false;//性格
		if ( !GetData(info.skill1) ) return false;//技能1
		if ( !GetData(info.skill2) ) return false;//技能2
		if ( !GetData(info.skill3) ) return false;//技能3
		if ( !GetData(info.skill4) ) return false;//技能4
		if ( !GetData(info.itemId) ) return false;//物品id
		if ( !GetData(info.HP) ) return false;//血
		if ( !GetData(info.WG) ) return false;//攻
		if ( !GetData(info.WF) ) return false;//防
		if ( !GetData(info.TG) ) return false;//特攻
		if ( !GetData(info.TF) ) return false;//特防
		if ( !GetData(info.SD) ) return false;//速度
		if ( !GetData(info.nick) ) return false;//昵称
		info.curHP = info.HP;//当前血量
		m_pet.push_back(info);
	}

	//回应参数

	return true;
}

}
