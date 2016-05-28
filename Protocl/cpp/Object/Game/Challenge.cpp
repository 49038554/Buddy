#include "Challenge.h"

namespace msg
{

Challenge::Challenge()
{
	BindSetting(m_buffer, MAX_MSG_SIZE, BYTE_ORDER, Moudle::Game);
}

Challenge::~Challenge()
{
}

bool Challenge::Build(bool isResult)
{
	SetId(MsgId::challenge, isResult);
	if ( !FillTransmitParam() ) return false;//Tcp服务填写参数
	//请求参数
	if ( !AddData(m_battleId) ) return false;	// 战斗Id 从Dekaron复制
	if ( !AddData(m_playerId) ) return false;	// 挑战玩家id
	char val = m_accepted?1:0;
	if ( !AddData(val) ) return false;// 接受挑战
	if ( !m_accepted ) return true;
	if ( !AddData(m_dNick) ) return false;		// 挑战者昵称

	//挑战者宠物
	if ( !AddData(m_dPet.size()) ) return false;
	int i = 0;
	for ( i = 0; i < m_dPet.size(); i++ )
	{
		if ( !AddData(m_dPet[i].id) ) return false;//宠物Id
		if ( !AddData(m_dPet[i].number) ) return false;//巴迪编号
		if ( !AddData(m_dPet[i].talent) ) return false;//特性
		if ( !AddData(m_dPet[i].nature) ) return false;//性格
		if ( !AddData(m_dPet[i].skill1) ) return false;//技能1
		if ( !AddData(m_dPet[i].skill2) ) return false;//技能2
		if ( !AddData(m_dPet[i].skill3) ) return false;//技能3
		if ( !AddData(m_dPet[i].skill4) ) return false;//技能4
		if ( !AddData(m_dPet[i].itemId) ) return false;//物品id
		if ( !AddData(m_dPet[i].HP) ) return false;//血
		if ( !AddData(m_dPet[i].WG) ) return false;//攻
		if ( !AddData(m_dPet[i].WF) ) return false;//防
		if ( !AddData(m_dPet[i].TG) ) return false;//特攻
		if ( !AddData(m_dPet[i].TF) ) return false;//特防
		if ( !AddData(m_dPet[i].SD) ) return false;//速度
		if ( !AddData(m_dPet[i].nick) ) return false;//昵称
	}
	if ( !AddData(m_cNick) ) return false;		// 应战者昵称
	// 应战者宠物
	if ( !AddData(m_cPet.size()) ) return false;
	for ( i = 0; i < m_cPet.size(); i++ )
	{
		if ( !AddData(m_cPet[i].id) ) return false;//宠物Id
		if ( !AddData(m_cPet[i].number) ) return false;//巴迪编号
		if ( !AddData(m_cPet[i].talent) ) return false;//特性
		if ( !AddData(m_cPet[i].nature) ) return false;//性格
		if ( !AddData(m_cPet[i].skill1) ) return false;//技能1
		if ( !AddData(m_cPet[i].skill2) ) return false;//技能2
		if ( !AddData(m_cPet[i].skill3) ) return false;//技能3
		if ( !AddData(m_cPet[i].skill4) ) return false;//技能4
		if ( !AddData(m_cPet[i].itemId) ) return false;//物品id
		if ( !AddData(m_cPet[i].HP) ) return false;//血
		if ( !AddData(m_cPet[i].WG) ) return false;//攻
		if ( !AddData(m_cPet[i].WF) ) return false;//防
		if ( !AddData(m_cPet[i].TG) ) return false;//特攻
		if ( !AddData(m_cPet[i].TF) ) return false;//特防
		if ( !AddData(m_cPet[i].SD) ) return false;//速度
		if ( !AddData(m_cPet[i].nick) ) return false;//昵称
	}
	
	//回应参数

	return true;
}

bool Challenge::Parse()
{
	if ( !Buffer::Parse() ) return false;
	//请求参数
	if ( !GetData(m_battleId) ) return false;// 战斗Id 从Dekaron复制
	if ( !GetData(m_playerId) ) return false;// 挑战玩家id
	char val;
	if ( !GetData(val) ) return false;// 接受挑战
	m_accepted = 1 == val?true:false;
	if ( !m_accepted ) return true;
	if ( !GetData(m_dNick) ) return false;		// 挑战者昵称

	//挑战者宠物
	data::PET info;
	info.state = 0;//状态
	info.sleepRound = 0;//催眠剩余回合
	info.frozenRound = 0;//冰封剩余回合

	int count = 0;
	if ( !GetData(count) ) return false;
	if ( 0 >= count || 6 < count ) return false;

	int i = 0;
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
		m_dPet.push_back(info);
	}

	if ( !GetData(m_cNick) ) return false;		// 应战者昵称
	// 应战者宠物
	if ( !GetData(count) ) return false;
	if ( 0 >= count || 6 < count ) return false;

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
		m_cPet.push_back(info);
	}

	//回应参数

	return true;
}

}
