#include "Battle.h"
#include "Game.h"

Battle::Battle()
{
	m_player.playerId = 0;
	m_enemy.playerId = 0;
	m_player.name = "";
	m_enemy.name = "";
}


Battle::~Battle()
{
}

Battle::WARRIOR::WARRIOR()
{
	playerId = 0;//玩家Id，0npc
	pCurPet = NULL;//当前场上巴迪
	lose = false;//未失败
	isChanged = false;//未换人
	pBuddy = NULL;
	pItem = NULL;
	pTalent = NULL;
	pSkill = NULL;
	pUseItem = NULL;

	outputHurt = 0;//输出伤害
	doomDesireRound = -1;//破灭之愿剩余回合
	predictRound = -1;//预知未来剩余回合
	attacked = false;//被攻击
	int i = 0;
	for ( i = 0; i < 18; i++ ) nail[i] = false;//没有钉子，属性id为下标，目前只有地属性钉子
	wall[0] = 0;
	wall[1] = 0;
}

void Battle::WARRIOR::NewRound()
{
	isReady = false;//未选择操作
	isActioned = false;//未行动
	isEnd = false;//未完成回合结束动作
	attacked = false;//被攻击
}

void Battle::WARRIOR::ChangePet()
{
	isEnd = false;//未完成回合结束动作
	wgCorrect = 1;//物攻修正
	tgCorrect = 1;//特攻修正
	sdCorrect = 1;//速修正
	wg = 0;//攻强化等级
	wf = 0;//防强化等级
	tg = 0;//特攻强化等级
	tf = 0;//特防强化等级
	sd = 0;//速度强化等级
	ct = 0;//暴击强化等级
	banChangeRound = 0;//禁止交换回合数,-1永久
	lockSkill = 0;//锁定技能
	lockSkillTime = ("专爱头巾" == pItem->name?-1:0);//锁定技能回合数量,-1永久
	guZhang = false;//中了鼓掌
	smell = false;//嗅觉
	recvHP = 0;//回复HP
	seed = false;//种子
	haQian = -1;//中了哈欠
	mieGe = 0;//中了灭亡歌
	tiShen = 0;//替身生命
	xunXing = false;//中寻衅
	tiaoDou = 0;//挑逗回合
	ban = false;//中封印
	tongGui = false;//中同归
	hunLuan = -1;//混乱剩余回合数
	defensed = false;//防守过了
	rest = false;//休息
	luanWu = 0;//乱舞剩余回合
	sunReady = false;//阳光烈焰准备中
	fear = false;//害怕

	int i = 0;
	for ( i = 0; i < 18; i++ ) race[i] = false;//属性强化
	lookSkill.clear();//圣斗士见过的技能
	duRound = 0;//中毒回合
	act = change;
}


bool Battle::Init(Game *game, int id,
	const std::string &playerName, const std::string &enemyName,
	unsigned int playerId, unsigned int enemyId,
	std::vector<data::PET> &me, std::vector<data::PET> &she)
{
	m_game = game;
	m_id = id;
	m_player.playerId = playerId;
	m_player.name = playerName;
	m_enemy.playerId = enemyId;
	m_enemy.name = enemyName;
	m_playerInitPets.clear();
	m_player.pets.clear();
	m_enemyInitPets.clear();
	m_enemy.pets.clear();
	int i = 0;
	for ( i = 0; i < me.size(); i++ )
	{
		m_playerInitPets.push_back(me[i]);
		m_player.pets.push_back(me[i]);
	}
	for ( i = 0; i < she.size(); i++ )
	{
		m_enemyInitPets.push_back(she[i]);
		m_enemy.pets.push_back(she[i]);
	}
	const char *ret = SetPetInfo(m_player, m_player.pets[0].id);
	if ( NULL != ret ) return false;
	ret = SetPetInfo(m_enemy, m_enemy.pets[0].id);
	if ( NULL != ret ) return false;

	m_curRound = -1;
	m_weather = 0;
	m_weatherCount = 0;
	m_foolSpace = 0;

	Battle::ROUND round;
	round.me = unknow;
	round.she = unknow;
	m_log.push_back(round);
	m_curRound++;
	m_pCurRound = &m_log[m_curRound];
	m_pCurRound->log.push_back(m_player.name + " VS " + m_enemy.name);
	m_pCurRound->showPos = 0;
	m_player.isChanged = ChangePet(m_player, m_player.pets[0].id);
	m_enemy.isChanged = ChangePet(m_enemy, m_enemy.pets[0].id);
	m_player.pCurPet->sleepRound = 0;//催眠剩余回合
	m_enemy.pCurPet->frozenRound = 0;//冰封剩余回合
	StepChange();
	StepStart();

	return true;
}

const char* Battle::SetPetInfo(Battle::WARRIOR &player, int petId)
{
	static std::string reason;
	player.pSkill = NULL;

	int i = 0;
	for ( i = 0; i < player.pets.size(); i++ )
	{
		if ( petId == player.pets[i].id ) 
		{
			player.pBuddy = Buddy(player.pets[i].number, m_game->BuddyBook());
			if ( NULL == player.pBuddy ) return (reason = "巴迪不存在").c_str();
			player.pTalent = Talent(player.pets[i].talent, m_game->TalentBook());
			if ( NULL == player.pTalent ) return (reason = "特性不存在").c_str();
			if ( 0 >= player.pets[i].curHP ) return (reason = "巴迪已失去战斗能力").c_str();
			player.pItem = Item(player.pets[i].itemId, m_game->ItemBook());
			if ( NULL == player.pItem ) player.pItem = m_game->NullItem();
			return NULL;
		}
	}

	return (reason = "不存在的宠物").c_str();
}

void Battle::CreateRP(bool me, Battle::RAND_PARAM &rp)
{
	Battle::WARRIOR &player = me?m_player:m_enemy;
	if ( player.defensed ) player.defensed = rand()%2;
	else player.defensed = true;
	rp.miss = rand()%100 + 1;//命中随机数
	rp.sePro = rand()%100 + 1;//技能特效随机数
	rp.iePro = rand()%100 + 1;//物品特效随机数
	rp.tePro = rand()%100 + 1;//特性特效随机数
	rp.luanWu = rand()%2 + 2;//乱舞回合数
	rp.sleepRound = rand()%7;//睡眠随机数
	rp.frozenRound = rand()%7;//冰冻随机数
	rp.dian = rand()%100 + 1;//麻痹随机数
	rp.luan = rand()%2;//混乱随机数
	rp.hurt = rand()%(255 - 217 + 1) + 217;//伤害随机数217~255
	rp.speed = rand()%100;//速度随机数
}

const char* Battle::CheckReady(bool me, Battle::Action act, short objectId)
{
	static std::string reason;
	if ( IsEnd() ) return (reason = "战斗已结束").c_str();

	Battle::WARRIOR &player = me?m_player:m_enemy;
	if ( player.isReady ) return (reason = "动作已选定").c_str();

	if ( Battle::attack == act ) 
	{
		if ( objectId != player.pCurPet->skill1 && objectId != player.pCurPet->skill2
			&& objectId != player.pCurPet->skill3 && objectId != player.pCurPet->skill4 ) return (reason = "不会此技能").c_str();

		if (player.xunXing
			&& 0 != player.lockSkill 
			&& 0 != player.lockSkillTime 
			&& (1 == player.pets.size() || player.banChangeRound)
			) //同时中专爱，寻衅, 没有技能可用,且不能换人
		{
			player.pSkill = m_game->BornSkill();
			return NULL;
		}

		if ( 0 != player.lockSkill 
			&& 0 != player.lockSkillTime 
			&& objectId != player.lockSkill ) //技能被锁定
		{
			return (reason = "不能更换技能").c_str();
		}
		if ( player.xunXing && objectId == player.lockSkill ) 
		{
			return (reason = "被寻衅，不能连续使用相同技能").c_str();
		}
		player.pSkill = Skill(objectId, m_game->SkillBook());
		if ( NULL == player.pSkill ) return (reason = "技能不存在").c_str();
		if ( 0 < player.tiaoDou && 2 == player.pSkill->type ) 
		{
			int sid[4];
			sid[0] = player.pCurPet->skill1;
			sid[1] = player.pCurPet->skill2;
			sid[2] = player.pCurPet->skill3;
			sid[3] = player.pCurPet->skill4;
			int i = 0;
			for ( i = 0; i < 4; i++ )
			{
				if ( objectId == sid[i] ) continue;
				if ( player.xunXing && objectId == sid[i] ) continue;
				if ( 0 != player.lockSkill 	&& 0 != player.lockSkillTime && objectId != sid[i] ) continue;
				player.pSkill = Skill(sid[i], m_game->SkillBook());
				if ( NULL == player.pSkill || 2 == player.pSkill->type ) continue;
				player.pSkill = NULL;
				return (reason = "被挑逗，只能使用攻击技能").c_str();
			}
			//没有技能可用,且没有PM可换，或不准换PM
			if ( 1 == player.pets.size() || player.banChangeRound )
			{
				player.pSkill = m_game->BornSkill();
				return NULL;
			}
			player.pSkill = NULL;
			return (reason = "被挑逗，只能使用攻击技能").c_str();;
		}
	}
	if ( Battle::useItem == act ) 
	{
		player.pUseItem = Item(objectId, m_game->ItemBook());
		if ( NULL == player.pUseItem ) return (reason = "物品不存在").c_str();
	}
	if ( Battle::change == act ) 
	{
		if ( 0 != player.banChangeRound ) return (reason = "不能更换巴迪").c_str();
		if ( NULL != player.pCurPet && player.pCurPet->id == objectId )
		{
			return (reason = "巴迪已出场").c_str();
		}
		const char *ret = SetPetInfo(player, objectId);
		if ( NULL != ret ) return ret;
	}

	return NULL;
}

const char* Battle::Ready(bool me, Battle::Action act, short objectId, Battle::RAND_PARAM &rp)
{
	const char *ret = CheckReady(me, act, objectId);
	if ( NULL != ret ) return ret;

	Battle::WARRIOR &player = me?m_player:m_enemy;

	if ( Battle::attack != act )
	{
		player.defensed = false;
		player.rest = false;
		player.pSkill = NULL;
	}

	player.act = act;
	player.objId = objectId;
	player.rp = rp;
	player.isReady = true;

	if ( me )
	{
		m_pCurRound->me = act;
		m_pCurRound->meObjectId = objectId;
		m_pCurRound->meRP = m_player.rp;
	}
	else
	{
		m_pCurRound->she = act;
		m_pCurRound->sheObjectId = objectId;
		m_pCurRound->sheRP = m_enemy.rp;
	}
	if ( !m_player.isReady || !m_enemy.isReady ) return NULL;

	if ( Battle::change == m_player.act )
	{
		m_player.isChanged = ChangePet(m_player, m_player.objId);
		m_enemy.tongGui = false;//未中同归
	}
	if ( Battle::change == m_enemy.act )
	{
		m_enemy.isChanged = ChangePet(m_enemy, m_enemy.objId);
		m_player.tongGui = false;//未中同归
	}
	PlayRound();
	return NULL;
}

const char* Battle::ChangePet(bool me, short petId)
{
	static std::string reason;
	Battle::WARRIOR &player = me?m_player:m_enemy;
	Battle::WARRIOR &enemy = me?m_enemy:m_player;
	if ( NULL != player.pCurPet && player.pCurPet->id == petId )
	{
		return (reason = "巴迪已出场").c_str();
	}
	if ( me ) m_pCurRound->mePetId.push_back(petId);
	else m_pCurRound->shePetId.push_back(petId);

	const char *ret = SetPetInfo(player, petId);
	if ( NULL != ret ) return ret;
	player.isChanged = ChangePet(player, petId);
	m_player.tongGui = m_enemy.tongGui = false;//未中同归
	PlayRound();

	return NULL;
}

bool Battle::PlayRound()
{
	if ( WaitPlayerCMD() || m_player.lose || m_enemy.lose ) return false;

	m_pCurRound->log.push_back("\n");
	StepChange();
	if ( !StepAttack() ) return false;
	if ( !StepEnd() ) return false;
	End();
	StepStart();

	return true;
}

void Battle::End()
{
	if ( !IsEnd() ) return;

	int i = 0;
	int enemyCount = 0;
	for ( i = 0; i < m_enemy.pets.size(); i++ )
	{
		if ( 0 < m_enemy.pets[i].curHP ) enemyCount++;
	}
	int playerCount = 0;
	for ( i = 0; i < m_player.pets.size(); i++ )
	{
		if ( 0 < m_player.pets[i].curHP ) playerCount++;
	}
	char result[256];
	if ( m_player.lose && m_enemy.lose ) 
	{
		sprintf( result, "平局" );
	}
	else if ( m_player.lose )
	{
		sprintf( result, "%d:%d %s胜", 
			playerCount, enemyCount, m_enemy.name.c_str() );
	}
	else 
	{
		sprintf( result, "%d:%d %s胜", 
			playerCount, enemyCount, m_player.name.c_str() );
	}
	m_pCurRound->log.push_back( result );

	return;
}

bool Battle::IsEnd()
{
	if ( m_player.lose || m_enemy.lose ) return true;

	return false;
}

bool Battle::Log( std::vector<std::string> &log )
{
	log.clear();
	int i = 0;
	for ( i = 0; i <= m_curRound; i++ )
	{
		for ( ; m_log[i].showPos < m_log[i].log.size(); m_log[i].showPos++ )
		{
			log.push_back(m_log[i].log[m_log[i].showPos]);
		}
	}
	if ( 0 >= log.size() ) return false;
	return true;
}

void Battle::StepStart()
{
	m_player.NewRound();
	m_enemy.NewRound();
	if ( IsEnd() ) return;
	Battle::ROUND round;
	round.me = unknow;
	round.she = unknow;
	m_log.push_back(round);
	m_curRound++;
	m_pCurRound = &m_log[m_curRound];
	char log[256];
	sprintf( log, "回合%d开始", m_curRound );
	m_pCurRound->log.push_back(log);
	m_pCurRound->showPos = 0;
}

bool Battle::StepChange()
{
	EntryStage(m_player, m_enemy);
	EntryStage(m_enemy, m_player);

	return true;
}

bool Battle::StepAttack()
{
	if ( m_player.isActioned && m_enemy.isActioned ) return true;

	if ( Battle::attack == m_player.act && ActionAble(m_player) 
		&& ( "气合拳" == m_player.pSkill->name || "裸奔气合拳" == m_player.pSkill->name ) )
	{
		if ( "裸奔气合拳" == m_player.pSkill->name ) 
		{
			m_player.tiShen = 0;
			m_pCurRound->log.push_back( m_player.pCurPet->nick + "舍弃了替身" );
		}
		m_pCurRound->log.push_back( m_player.pCurPet->nick + "开始聚气" );
	}
	if ( Battle::attack == m_enemy.act && ActionAble(m_enemy)
		&& ( "气合拳" == m_enemy.pSkill->name || "裸奔气合拳" == m_enemy.pSkill->name ) )
	{
		if ( "裸奔气合拳" == m_enemy.pSkill->name ) 
		{
			m_enemy.tiShen = 0;
			m_pCurRound->log.push_back( m_enemy.pCurPet->nick + "舍弃了替身" );
		}
		m_pCurRound->log.push_back( m_enemy.pCurPet->nick + "开始聚气" );
	}

	if ( NULL == m_player.pSkill || 
		("保护" != m_player.pSkill->name && "忍耐" != m_player.pSkill->name) ) m_player.defensed = false;
	if ( NULL == m_enemy.pSkill || 
		("保护" != m_enemy.pSkill->name && "忍耐" != m_enemy.pSkill->name) ) m_enemy.defensed = false;
	if ( AttackOrder(m_player, m_enemy) )
	{
		if ( Battle::attack == m_player.act && !m_player.isActioned )
		{
			PetAction(m_player, m_enemy);
			m_player.tongGui = false;//未中同归
			if ( WaitPlayerCMD() || m_player.lose || m_enemy.lose ) return false;
		}
		if ( Battle::attack == m_enemy.act && !m_enemy.isActioned )
		{
			PetAction(m_enemy, m_player);
			m_enemy.tongGui = false;//未中同归
			if ( WaitPlayerCMD() || m_player.lose || m_enemy.lose ) return false;
		}
	}
	else 
	{
		if ( Battle::attack == m_enemy.act && !m_enemy.isActioned )
		{
			PetAction(m_enemy, m_player);
			m_enemy.tongGui = false;//未中同归
			if ( WaitPlayerCMD() || m_player.lose || m_enemy.lose ) return false;
		}
		if ( Battle::attack == m_player.act && !m_player.isActioned )
		{
			PetAction(m_player, m_enemy);
			m_player.tongGui = false;//未中同归
			if ( WaitPlayerCMD() || m_player.lose || m_enemy.lose ) return false;
		}
	}

	return true;
}

bool Battle::StepEnd()
{
	PlayerEnd(m_player, m_enemy);
	if ( 0 >= m_player.pCurPet->curHP ) return false; 
	PlayerEnd(m_enemy, m_player);
	if ( 0 >= m_enemy.pCurPet->curHP ) return false; 

	if ( 0 < m_weatherCount )
	{
		m_weatherCount--;
		if ( 0 == m_weatherCount )
		{
			if ( Race::shui == m_weather ) m_pCurRound->log.push_back("雨停了");
			else if ( Race::bing == m_weather ) m_pCurRound->log.push_back("雪停了");
			else if ( Race::huo == m_weather ) m_pCurRound->log.push_back("太阳弱了");
			else if ( Race::yan == m_weather ) m_pCurRound->log.push_back("沙尘暴停了");
			m_weather = 0;
		}
	}
	if ( 0 < m_foolSpace ) //欺骗空间剩余回合数 -1永久
	{
		m_foolSpace--;
		if ( 0 == m_foolSpace )
		{
			m_pCurRound->log.push_back("欺骗空间消失了");
		}
	}

	m_pCurRound->log.push_back("\n");
	char log[256];
	sprintf( log, "回合%d结束", m_curRound );
	m_pCurRound->log.push_back(log);
	m_pCurRound->log.push_back("\n");
	return true;
}

bool Battle::WaitPlayerCMD()
{
	if ( m_player.lose || m_enemy.lose ) return false;
	if ( NULL == m_player.pCurPet || 0 >= m_player.pCurPet->curHP 
		|| NULL == m_enemy.pCurPet || 0 >= m_enemy.pCurPet->curHP ) 
	{
		return true;
	}

	return false;
}

void Battle::EntryStage(Battle::WARRIOR &player, Battle::WARRIOR &enemy)
{
	if ( !player.isChanged ) return;
	player.isChanged = false;

	if ( "威吓" == player.pTalent->name ) 
	{
		if ( "净体" == enemy.pTalent->name )
		{
			m_pCurRound->log.push_back( enemy.pCurPet->nick + "的净体防止了能力下降");
		}
		else
		{
			if ( -6 == enemy.wg ) 
			{
				m_pCurRound->log.push_back(enemy.pCurPet->nick + "的物攻已经降到极限");
				return;
			}
			enemy.wg--;
			m_pCurRound->log.push_back(player.pCurPet->nick + "的威吓降低了" + enemy.pCurPet->nick + "的物攻");
		}
	}
	else if ( "暴雨" == player.pTalent->name ) 
	{
		m_weather = Race::shui;
		m_weatherCount = -1;
		m_pCurRound->log.push_back("下雨了");
	}
	else if ( "暴雪" == player.pTalent->name ) 
	{
		m_weather = Race::bing;
		m_weatherCount = -1;
		m_pCurRound->log.push_back("下雪了");
	}
	else if ( "干旱" == player.pTalent->name ) 
	{
		m_weather = Race::huo;
		m_weatherCount = -1;
		m_pCurRound->log.push_back("太阳出来了");
	}
	else if ( "扬沙" == player.pTalent->name ) 
	{
		m_weather = Race::yan;
		m_weatherCount = -1;
		m_pCurRound->log.push_back("沙尘暴来了");
	}
	else if ( "花之礼" == player.pTalent->name && Race::huo == m_weather) 
	{
		player.tg += 1;
		player.tf += 1;
	}
	else if ( "咬脚" == player.pTalent->name && "逃走" != enemy.pTalent->name ) 
	{
		m_pCurRound->log.push_back(player.pCurPet->nick + "咬住了" 
			+ enemy.pCurPet->nick + "不能逃跑，速度急剧下降，物理攻击和特殊攻击急剧上升" );
		enemy.banChangeRound = -1;
		enemy.wgCorrect *= 2;
		enemy.tgCorrect *= 2;
		enemy.sdCorrect /= 2;
	}
}

void Battle::LeaveStage(Battle::WARRIOR &player)
{
	m_pCurRound->log.push_back(player.name + "收回" + player.pCurPet->nick);
	if ( 0 >= player.pCurPet->curHP ) 
	{
		std::vector<data::PET>::iterator it = player.pets.begin();
		int i = 0;
		for ( ; it != player.pets.end(); it++, i++ )
		{
			if ( player.pCurPet != &player.pets[i] ) continue;
			player.pets.erase(it);
			break;
		}
		player.pCurPet = NULL;
		return;
	}

	data::TALENT *pTalent = Talent(player.pCurPet->talent, m_game->TalentBook());
	if ( "自然恢复" == pTalent->name ) //
	{
		player.pCurPet->state = 0;
		m_pCurRound->log.push_back("恢复了正常状态");
	}
	if ( "再生" == pTalent->name ) //
	{
		player.pCurPet->curHP += player.pCurPet->HP / 3;
		m_pCurRound->log.push_back("恢复了1/3的HP");
	}
	player.pCurPet = NULL;
}

int Battle::Hurt(Battle::WARRIOR &player, int HP, bool unFaint)
{
	HP = HP <= player.pCurPet->curHP?HP:player.pCurPet->curHP;
	char strHurt[256];
	sprintf( strHurt, "%s受到%d%%(%d点)伤害", player.pCurPet->nick.c_str(), 
		HP*100/player.pCurPet->HP, HP );
	m_pCurRound->log.push_back(strHurt);
	player.pCurPet->curHP -= HP;

	if ( unFaint && player.pCurPet->curHP <= 0 )
	{
		player.pCurPet->curHP = 1;
		m_pCurRound->log.push_back(player.pCurPet->nick + "承受住了伤害");
	}
	if ( Faint(player) ) return HP;

	if ( player.pCurPet->curHP > player.pCurPet->HP/3 ) return HP;
	if ( 5 == player.pCurPet->itemId ) 
	{
		player.wgCorrect *= 1.5;
		player.pCurPet->itemId = 0;
		m_pCurRound->log.push_back(player.pCurPet->nick + "吃了兴奋剂-攻");
	}
	if ( 6 == player.pCurPet->itemId ) 
	{
		player.tgCorrect = 1.5;
		player.pCurPet->itemId = 0;
		m_pCurRound->log.push_back(player.pCurPet->nick + "吃了兴奋剂-特攻");
	}
	if ( 7 == player.pCurPet->itemId )
	{
		player.sdCorrect = 1.5;
		player.pCurPet->itemId = 0;
		m_pCurRound->log.push_back(player.pCurPet->nick + "吃了兴奋剂-速度");
	}

	return HP;
}

bool Battle::ChangePet(Battle::WARRIOR &player, int petId)
{
	player.isActioned = true;//已经行动

	int i = 0;
	if ( NULL != player.pCurPet && player.pCurPet->id != petId )
	{
		LeaveStage(player);
	}
	if ( NULL != player.pCurPet ) 
	{
		m_pCurRound->log.push_back("放出已出场的巴迪 ");
		return false;
	}

	for ( i = 0; i < player.pets.size(); i++ )
	{
		if ( petId == player.pets[i].id )
		{
			player.pCurPet = &player.pets[i];
			m_pCurRound->log.push_back(player.name + "放出" + player.pBuddy->name);
			break;
		}
	}
	if ( NULL == player.pCurPet ) 
	{
		m_pCurRound->log.push_back("指定不存在的巴迪出场");
		return false;
	}
	player.ChangePet();

	if ( player.nail[Race::di] )
	{
		if ( Race::fei != player.pBuddy->race1 
			&& Race::fei != player.pBuddy->race2 
			&& "魔法防御" != player.pTalent->name 
			&& "浮游" != player.pTalent->name ) 
		{
			Hurt( player, player.pCurPet->HP / 4 );
			if ( 0 >= player.pCurPet->curHP ) return false;//出场就挂了
		}
	}
	if ( "毒珠" == player.pItem->name )
	{
		player.pCurPet->state = Race::du;
		player.duRound = 0;
		m_pCurRound->log.push_back( m_player.pCurPet->nick + "携带毒珠中毒了" );
	}
	else if ( "火珠" == player.pItem->name )
	{
		player.pCurPet->state = Race::huo;
		m_pCurRound->log.push_back( m_player.pCurPet->nick + "携带火珠烧伤了" );
	}

	return true;
}

bool Battle::AttackOrder(Battle::WARRIOR &player, Battle::WARRIOR &enemy)
{
	bool meXianShou = IsUnwait(player);
	bool sheXianShou = IsUnwait(enemy);
	bool meWait = IsWait(player);
	bool sheWait = IsWait(enemy);
	int meSpeed = CalSpeed(player, enemy);
	int sheSpeed = CalSpeed(enemy, player);

	if ( meWait && sheWait )
	{
		if (meSpeed == sheSpeed)
		{
			if (player.rp.speed == enemy.rp.speed)
			{
				if ( player.playerId > enemy.playerId ) return true;//me 
			}
			else if (player.rp.speed > enemy.rp.speed) return true;//me 
		}
		else if ( meSpeed > sheSpeed ) 
		{
			if ( 0 == m_foolSpace ) return true;//me 
		}
		else   
		{
			if ( 0 == m_foolSpace ) return false;//she 
			return true;
		}
	}
	else if ( meWait ) return false;//she 
	else if ( sheWait ) return true;//me 

	if ( meXianShou == sheXianShou )
	{
		if (meSpeed == sheSpeed)
		{
			if (player.rp.speed == enemy.rp.speed)
			{
				if ( player.playerId > enemy.playerId ) return true;//me 
			}
			else if (player.rp.speed > enemy.rp.speed) return true;//me 
		}
		else if ( meSpeed > sheSpeed )  
		{
			if ( 0 == m_foolSpace ) return true;//me 
		}
		else   
		{
			if ( 0 == m_foolSpace ) return false;//she 
			return true;
		}
	}
	else if ( meXianShou ) return true;//me 

	return false;
}

bool Battle::IsUnwait(Battle::WARRIOR &player)
{
	if ( Battle::attack != player.act ) return false;

	if ( "冰刀" == player.pSkill->name ) return true;
	if ( "飞叶快刀" == player.pSkill->name ) return true;
	if ( "闪电突袭" == player.pSkill->name ) return true;
	if ( "音速拳" == player.pSkill->name ) return true;
	if ( "暗算" == player.pSkill->name ) return true;
	if ( "子弹拳" == player.pSkill->name ) return true;
	if ( "神速" == player.pSkill->name ) return true;
	if ( "先制爪" == player.pItem->name && player.rp.iePro < 21 ) return true;
	if ( "疾风之翼" == player.pTalent->name && Race::fei == player.pSkill->race ) return true;

	return false;
}

bool Battle::IsWait(Battle::WARRIOR &player)
{
	if ( Battle::attack != player.act ) return false;
	if ( "飘花淡雪浮香吹" == player.pSkill->name ) return true;
	if ( "吹飞" == player.pSkill->name ) return true;
	if ( "吼叫" == player.pSkill->name ) return true;
	if ( "气合拳" == player.pSkill->name ) return true;
	if ( "裸奔气合拳" == player.pSkill->name ) return true;

	return false;
}

int Battle::CalSpeed(Battle::WARRIOR &player, Battle::WARRIOR &enemy)
{
	int level = player.sd;
	if ( "天然" == player.pTalent->name && 0 > level ) level = 0;
	if ( "天然" == enemy.pTalent->name && 0 < level ) level = 0;
	int speed = 0;
	if ( level > 0 ) speed = player.pCurPet->SD * (1 + level * 0.5) * player.sdCorrect;
	else if ( level < 0 ) speed = player.pCurPet->SD * (1 + level * 0.5 * -1) * player.sdCorrect;
	else speed = player.pCurPet->SD * player.sdCorrect;

	if ( "轻快" == player.pTalent->name && Race::shui == m_weather ) speed *= 2;
	else if ( "叶绿素" == player.pTalent->name && Race::huo == m_weather ) speed *= 2; 
	else if ( "挖沙" == player.pTalent->name && Race::yan == m_weather ) speed *= 2;
	else if ( "滑雪" == player.pTalent->name && Race::bing == m_weather ) speed *= 2;
	else if ( "超级炮台" == player.pTalent->name ) speed *= 1.2;
	else if ( "飞毛腿" == player.pTalent->name && 0 != player.pCurPet->state) speed *= 1.5;

	if ( 4 == player.pCurPet->state && "飞毛腿" != player.pTalent->name ) speed /= 4;

	return speed;
}

bool Battle::CalHitRate(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef)
{
	if ( 100 < playerAck.pSkill->hitRate
		|| "锐利目光" == playerAck.pTalent->name
		|| "放大镜" == playerAck.pItem->name ) return true;

	float hr = playerAck.pSkill->hitRate;
	if ( "压力" == playerDef.pTalent->name ) hr *= 0.85;
	if ( "沙隐" == playerDef.pTalent->name && Race::yan == m_weather ) hr *= 0.7;
	if ( "雪隐" == playerDef.pTalent->name && Race::bing == m_weather ) hr *= 0.7;
	if ( "光粉" == playerDef.pItem->name ) hr *= 0.8;
	if ( "复眼" == playerAck.pTalent->name ) hr *= 1.3;
	if ( playerAck.rp.miss > hr) return false;

	return true;
}

bool Battle::CriticalHit(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef)
{
	if ( "盔甲" == playerDef.pTalent->name ) return false;

	int i = 0;
	float probability = 0;
	for ( i = 0; i < playerAck.pSkill->effects.size(); i++ )
	{
		if ( 88 != playerAck.pSkill->effects[i].id ) continue;//CT
		probability = playerAck.pSkill->effects[i].probability;
		break;
	}
	if ( "四叶草" == playerAck.pItem->name ) probability += 15;
	if ( "强运" == playerAck.pTalent->name ) probability += 15;
	probability += playerAck.ct * 10;
	if ( playerAck.rp.sePro < probability ) return true;

	return false;
}

int Battle::CalWG(Battle::WARRIOR &player, bool ct, Battle::WARRIOR &enemy)
{
	int level = player.wg;
	if ( "天然" == player.pTalent->name && 0 > level ) level = 0;
	if ( "天然" == enemy.pTalent->name && 0 < level ) level = 0;
	int WG = 0;
	if ( level > 0 ) WG = player.pCurPet->WG * (1 + level * 0.5) * player.wgCorrect;
	else if ( level < 0 && !ct ) WG = player.pCurPet->WG / (1 + level * 0.5 * -1) * player.wgCorrect;
	else WG = player.pCurPet->WG * player.wgCorrect;
	if ( "专爱头巾" == player.pItem->name ) WG *= 1.5;

	if ( "暴力" == player.pTalent->name ) WG *= 2;
	else if ( "超级炮台" == player.pTalent->name ) WG *= 1.2;
	else if ( "根性" == player.pTalent->name && 0 != player.pCurPet->state ) WG *= 1.5;
	if ( 2 == player.pCurPet->state && "根性" != player.pTalent->name ) WG /= 2;

	return WG;
}

int Battle::CalWF(Battle::WARRIOR &player, bool ct, Battle::WARRIOR &enemy)
{
	int level = player.wf;
	if ( "天然" == player.pTalent->name && 0 > level ) level = 0;
	if ( "天然" == enemy.pTalent->name && 0 < level ) level = 0;
	if ( level > 0 && ct ) level = 0;
	int WF = 0;
	if ( level > 0 ) WF = player.pCurPet->WF * (1 + level * 0.5);
	else if ( level < 0 ) WF = player.pCurPet->WF / (1 + level * 0.5 * -1);
	else WF = player.pCurPet->WF;

	if ( "神秘鳞片" == player.pTalent->name && 0 != player.pCurPet->state ) WF *= 1.5;

	return WF;
}

int Battle::CalTG(Battle::WARRIOR &player, bool ct, Battle::WARRIOR &enemy)
{
	int level = player.tg;
	if ( "天然" == player.pTalent->name && 0 > level ) level = 0;
	if ( "天然" == enemy.pTalent->name && 0 < level ) level = 0;
	int TG = 0;
	if ( level > 0 ) TG = player.pCurPet->TG * (1 + level * 0.5) * player.tgCorrect;
	else if ( level < 0 && !ct ) TG = player.pCurPet->TG / (1 + level * 0.5 * -1) * player.tgCorrect;
	else TG = player.pCurPet->TG * player.tgCorrect;

	if ( "太阳力量" == player.pTalent->name && Race::huo == m_weather ) TG *= 1.5;

	return TG;
}

int Battle::CalTF(Battle::WARRIOR &player, bool ct, Battle::WARRIOR &enemy)
{
	int level = player.tf;
	if ( "天然" == player.pTalent->name && 0 > level ) level = 0;
	if ( "天然" == enemy.pTalent->name && 0 < level ) level = 0;
	if ( level > 0 && ct ) level = 0;
	int TF = 0;
	if ( level > 0 ) TF = player.pCurPet->TF * (1 + level * 0.5);
	else if ( level < 0 ) TF = player.pCurPet->TF / (1 + level * 0.5 * -1);
	else TF = player.pCurPet->TF;

	return TF;
}

static float RaceGood( char raceAck, char raceDef )
{
	if ( 0 == raceDef ) return 1;

	if ( Race::pu == raceAck )
	{
		if ( Race::gui == raceDef ) return 0;
		else if ( Race::yan == raceDef 
			||Race::gang == raceDef ) return 0.5;
		return 1;
	}
	else if ( Race::huo == raceAck )
	{
		if ( Race::cao == raceDef
			|| Race::chong == raceDef 
			|| Race::bing == raceDef 
			|| Race::gang == raceDef ) return 2;
		else if ( Race::huo == raceDef 
			|| Race::shui == raceDef 
			|| Race::yan == raceDef 
			|| Race::longx == raceDef ) return 0.5;
		return 1;
	}
	else if ( Race::shui == raceAck )
	{
		if ( Race::huo == raceDef
			|| Race::di == raceDef 
			|| Race::yan == raceDef) return 2;
		else if ( Race::shui == raceDef 
			|| Race::longx == raceDef 
			|| Race::cao == raceDef) return 0.5;
		return 1;
	}
	else if ( Race::dian == raceAck )
	{
		if ( Race::fei == raceDef
			|| Race::shui == raceDef ) return 2;
		else if ( Race::cao == raceDef 
			|| Race::longx == raceDef ) return 0.5;
		else if ( Race::di == raceDef ) return 0;
		return 1;
	}
	else if ( Race::bing == raceAck )
	{
		if ( Race::fei == raceDef
			|| Race::cao == raceDef
			|| Race::longx == raceDef
			|| Race::di == raceDef) return 2;
		else if ( Race::bing == raceDef 
			|| Race::shui == raceDef
			|| Race::huo== raceDef
			|| Race::gang == raceDef) return 0.5;
		return 1;
	}
	else if ( Race::gui == raceAck )
	{
		if ( Race::chao == raceDef
			|| Race::gui == raceDef) return 2;
		else if ( Race::gang == raceDef 
			|| Race::e == raceDef) return 0.5;
		else if ( Race::pu == raceDef ) return 0;
		return 1;
	}
	else if ( Race::e == raceAck )
	{
		if ( Race::chao == raceDef
			|| Race::gui == raceDef) return 2;
		else if ( Race::gang == raceDef 
			|| Race::e == raceDef) return 0.5;
		return 1;
	}
	else if ( Race::chao == raceAck )
	{
		if ( Race::dou == raceDef
			|| Race::du == raceDef) return 2;
		else if ( Race::gang == raceDef 
			|| Race::chao == raceDef) return 0.5;
		else if ( Race::e == raceDef ) return 0;
		return 1;
	}
	else if ( Race::longx == raceAck )
	{
		if ( Race::longx == raceDef ) return 2;
		else if ( Race::gang == raceDef ) return 0.5;
		return 1;
	}
	else if ( Race::fei == raceAck )
	{
		if ( Race::chong == raceDef
			|| Race::dou == raceDef
			|| Race::cao == raceDef) return 2;
		else if ( Race::gang == raceDef
			|| Race::yan == raceDef
			|| Race::dian == raceDef) return 0.5;
		return 1;
	}
	else if ( Race::chong == raceAck )
	{
		if ( Race::cao == raceDef
			|| Race::chao == raceDef
			|| Race::e == raceDef) return 2;
		else if ( Race::gang == raceDef
			|| Race::dou == raceDef
			|| Race::huo == raceDef
			|| Race::fei == raceDef
			|| Race::du == raceDef) return 0.5;
		return 1;
	}
	else if ( Race::cao == raceAck )
	{
		if ( Race::shui == raceDef
			|| Race::di == raceDef
			|| Race::yan == raceDef) return 2;
		else if ( Race::cao == raceDef
			|| Race::huo == raceDef
			|| Race::fei == raceDef
			|| Race::du == raceDef
			|| Race::gang == raceDef
			|| Race::longx == raceDef
			|| Race::chong == raceDef) return 0.5;
		return 1;
	}
	else if ( Race::di == raceAck )
	{
		if ( Race::du == raceDef
			|| Race::gang == raceDef
			|| Race::yan == raceDef
			|| Race::huo == raceDef
			|| Race::dian == raceDef) return 2;
		else if ( Race::cao == raceDef
			|| Race::chong == raceDef) return 0.5;
		else if ( Race::fei == raceDef ) return 0;
		return 1;
	}
	else if ( Race::yan == raceAck )
	{
		if ( Race::bing == raceDef
			|| Race::huo == raceDef
			|| Race::fei == raceDef
			|| Race::chong == raceDef) return 2;
		else if ( Race::gang == raceDef
			|| Race::di == raceDef
			|| Race::dou == raceDef) return 0.5;
		return 1;
	}
	else if ( Race::gang == raceAck )
	{
		if ( Race::bing == raceDef
			|| Race::yan == raceDef) return 2;
		else if ( Race::gang == raceDef
			|| Race::huo == raceDef
			|| Race::shui == raceDef
			|| Race::dian == raceDef) return 0.5;
		return 1;
	}
	else if ( Race::dou == raceAck )
	{
		if ( Race::bing == raceDef
			|| Race::yan == raceDef
			|| Race::pu == raceDef
			|| Race::gang == raceDef
			|| Race::e == raceDef) return 2;
		else if ( Race::fei== raceDef
			|| Race::du == raceDef
			|| Race::chong == raceDef
			|| Race::chao == raceDef) return 0.5;
		else if ( Race::gui == raceDef ) return 0;
		return 1;
	}
	else if ( Race::du == raceAck )
	{
		if ( Race::cao == raceDef ) return 2;
		else if ( Race::du == raceDef
			|| Race::di == raceDef
			|| Race::gui == raceDef
			|| Race::yan == raceDef) return 0.5;
		else if ( Race::gang == raceDef ) return 0;
		return 1;
	}

	return 0;
}

static bool MagicSkill(data::SKILL *pSkill)
{
	if ( "劈开" == pSkill->name 
		|| "燃烧殆尽" == pSkill->name 
		|| "炽热推进" == pSkill->name 
		|| "大字爆" == pSkill->name 
		|| "火焰拳" == pSkill->name 
		|| "雪崩" == pSkill->name 
		|| "暴风雪" == pSkill->name 
		|| "急冻光线" == pSkill->name 
		|| "急冻拳" == pSkill->name 
		|| "沸水" == pSkill->name 
		|| "跃龙门" == pSkill->name 
		|| "冲浪" == pSkill->name 
		|| "打雷" == pSkill->name 
		|| "十万伏特" == pSkill->name 
		|| "雷光拳" == pSkill->name 
		|| "空气切割" == pSkill->name 
		|| "空气爆炸" == pSkill->name 
		|| "叶刃斩" == pSkill->name 
		|| "能源球" == pSkill->name 
		|| "飘花淡雪浮香吹" == pSkill->name 
		|| "鬼爪" == pSkill->name 
		|| "影子球" == pSkill->name 
		|| "试刀" == pSkill->name 
		|| "恶念波动" == pSkill->name 
		|| "咬碎" == pSkill->name 
		|| "精神切割" == pSkill->name 
		|| "心灵压迫" == pSkill->name 
		|| "精神干扰" == pSkill->name 
		|| "精神冲击" == pSkill->name 
		|| "彗星拳" == pSkill->name 
		|| "光栅加农" == pSkill->name 
		|| "十字剪" == pSkill->name 
		|| "虫鸣" == pSkill->name 
		|| "岩崩" == pSkill->name 
		|| "原始力量" == pSkill->name 
		|| "地震" == pSkill->name 
		|| "大地之力" == pSkill->name 
		|| "毒突" == pSkill->name 
		|| "淤泥爆弹" == pSkill->name ) return true;

	return false;
}

static bool BiteSkill(data::SKILL *pSkill)
{
	if ( "木槌" == pSkill->name
		|| "闪电突袭" == pSkill->name
		|| "勇鸟" == pSkill->name
		|| "炽热推进" == pSkill->name
		|| "舍身撞" == pSkill->name
		) return true;

	return false;
}

int Battle::CalPower(Battle::WARRIOR &playerAck, bool ct, Battle::WARRIOR &playerDef)
{
	int power = playerAck.pSkill->power;
	if ( "七伤拳" == playerAck.pSkill->name 
		|| "垂死挣扎" == playerAck.pSkill->name
		|| "终极针刺" == playerAck.pSkill->name
		)
	{
		if ( 4 >= playerAck.pCurPet->curHP / playerAck.pCurPet->HP ) power = 200;
		else if ( 10 >= playerAck.pCurPet->curHP / playerAck.pCurPet->HP ) power = 150;
		else if ( 21 >= playerAck.pCurPet->curHP / playerAck.pCurPet->HP ) power = 100;
		else if ( 35 >= playerAck.pCurPet->curHP / playerAck.pCurPet->HP ) power = 80;
		else if ( 69 >= playerAck.pCurPet->curHP / playerAck.pCurPet->HP ) power = 40;
		else  power = 20;
	}

	if ( ct ) power *= 2;
	char skillRace = playerAck.pSkill->race;
	if ( "天空皮肤" == playerAck.pTalent->name && Race::pu == skillRace ) 
	{
		skillRace = Race::fei;
		power *= 1.3;
	}
	else if ( "冰冻皮肤" == playerAck.pTalent->name && Race::pu == skillRace )
	{
		skillRace = Race::bing;
		power *= 1.3;
	}
	float vf1 = RaceGood(skillRace, playerDef.pBuddy->race1);
	if ( 0.4 > vf1 ) 
	{
		if ( Race::gui == playerDef.pBuddy->race1
			|| Race::gui == playerDef.pBuddy->race2 )
		{
			if ( "胆量" != playerAck.pTalent->name && !playerAck.smell ) return 0;
			vf1 = 1;
		}
		else return 0;
	}

	float vf2 = RaceGood(skillRace, playerDef.pBuddy->race2);
	if ( 0.4 > vf2 ) 
	{
		if ( Race::gui == playerDef.pBuddy->race1
			|| Race::gui == playerDef.pBuddy->race2 )
		{
			if ( "胆量" != playerAck.pTalent->name && !playerAck.smell ) return 0;
			vf2 = 1;
		}
		else return 0;
	}

	power = power * vf1 * vf2;
	if ( skillRace == playerAck.pBuddy->race1 
		|| skillRace == playerAck.pBuddy->race2 ) power *= 1.5;

	if ( playerAck.race[skillRace] ) power *= 1.5;
	if ( "技师" == playerAck.pTalent->name && 60 >= playerAck.pSkill->power ) power *= 1.5;
	else if ( "分析" == playerAck.pTalent->name && playerDef.isActioned ) power *= 2;
	else if ( "有色眼镜" == playerAck.pTalent->name && 1.5 > vf1 * vf2 ) power *= 2;
	else if ( "给力" == playerAck.pTalent->name && MagicSkill(playerAck.pSkill) )
	{
		playerAck.rp.sePro = 101;
		power *= 1.3;
	}
	else if ( "茂盛" == playerAck.pTalent->name 
		&& Race::cao == skillRace
		&& playerAck.pCurPet->curHP < playerAck.pCurPet->HP / 3 ) power *= 1.5;
	else if ( "猛火" == playerAck.pTalent->name 
		&& Race::huo == skillRace
		&& playerAck.pCurPet->curHP < playerAck.pCurPet->HP / 3 ) power *= 1.5;
	else if ( "激流" == playerAck.pTalent->name 
		&& Race::shui == skillRace
		&& playerAck.pCurPet->curHP < playerAck.pCurPet->HP / 3 ) power *= 1.5;
	else if ( "虫族报警" == playerAck.pTalent->name 
		&& Race::chong == skillRace
		&& playerAck.pCurPet->curHP < playerAck.pCurPet->HP / 3 ) power *= 1.5;
	else if ( "舍身" == playerAck.pTalent->name && BiteSkill(playerAck.pSkill) ) power *= 1.2;
	if ( "沙之力" == playerAck.pTalent->name && Race::yan == m_weather
		&& (Race::di == skillRace||Race::yan == skillRace||Race::gang == skillRace) ) 
	{
		power *= 1.5;
	}
	if ( "黑暗气息" == playerAck.pTalent->name && Race::e == skillRace ) power *= 1.5;

	if ( 0 < playerDef.wall[0] && 1 == playerAck.pSkill->type 
		&& "穿透" != playerAck.pTalent->name && !ct ) power /= 2;
	if ( 0 < playerDef.wall[1] && 3 == playerAck.pSkill->type 
		&& "穿透" != playerAck.pTalent->name && !ct ) power /= 2;

	//////////////////////////////////////////////////////////////////////////
	//装备
	if ( "生命玉" == playerAck.pItem->name ) power *= 1.25;

	//////////////////////////////////////////////////////////////////////////
	//技能
	if ( "空元气" == playerAck.pSkill->name && 0 != playerAck.pCurPet->state ) power *= 2;


	//////////////////////////////////////////////////////////////////////////
	//防御特性
	if ( "肥脂肪" == playerDef.pTalent->name 
		&& (Race::huo == skillRace || Race::bing == skillRace) ) power /= 2;

	return power;
}

bool Battle::PetAction(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef)
{
	playerAck.isActioned = true;
	playerAck.lockSkill = playerAck.pSkill->id;
	if ( playerAck.rest )
	{
		playerAck.rest = false;
		m_pCurRound->log.push_back(playerAck.pCurPet->nick + "需要休息,不能行动");
		return false;
	}
	if ( playerAck.fear )
	{
		m_pCurRound->log.push_back(playerAck.pCurPet->nick + "害怕了,不能行动");
		if ( "不屈" == playerAck.pTalent->name && playerAck.sd < 6 )
		{
			playerAck.sd++;
			m_pCurRound->log.push_back(playerAck.pCurPet->nick + "因为不屈提升了速度");
		}
		return false;
	}
	if ( playerAck.attacked 
		&& ( "气合拳" == playerAck.pSkill->name 
		|| "裸奔气合拳" == playerAck.pSkill->name ) )
	{
		m_pCurRound->log.push_back(playerAck.pCurPet->nick + "聚气被中断");
		return false;
	}
	if ( LaunchState(playerAck) && !Medication(playerAck) ) 
	{
		if ( Race::pu == playerAck.pCurPet->state && "梦话" == playerAck.pSkill->name )
		{
			UseSkill(playerAck, playerDef);
			return true;
		}
		return false;
	}
	if ( Confusion(playerAck) ) 
	{
		return UseSkill(playerAck, playerAck);
	}

	ChangeSkill(playerAck);
	if ( BanSkill(playerAck, playerDef) ) 
	{
		m_pCurRound->log.push_back(playerAck.pCurPet->nick 
			+ "受到封印，不能使用地方掌握的技能:" + playerAck.pSkill->name);
		return false;
	}

	if ( !UseSkill(playerAck, playerDef) )
	{
		if ( "大爆炸" == playerAck.pSkill->name ) Hurt(playerAck, playerAck.pCurPet->curHP);
	}

	return true;
}

bool Battle::UseSkill(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef)
{
	m_pCurRound->log.push_back(playerAck.pCurPet->nick + "使用了" + playerAck.pSkill->name);

	//////////////////////////////////////////////////////////////////////////
	//辅助技能
	if ( 2 == playerAck.pSkill->type ) 
	{
		if ( 0 < playerAck.tiaoDou )
		{
			m_pCurRound->log.push_back(playerAck.pCurPet->nick + "被挑逗不能使用辅助技能");
			return false;
		}
		if ( HelpSkill(playerAck, playerDef) ) return true;
	}
	if ( "阳光烈焰" == playerAck.pSkill->name && Race::huo != m_weather )
	{
		if ( !playerAck.sunReady )
		{
			playerAck.sunReady = true;
			if ( 0 == playerAck.lockSkillTime ) playerAck.lockSkillTime = 2;
			playerAck.banChangeRound = 2;
			m_pCurRound->log.push_back(playerAck.pCurPet->nick + "吸收了太阳光线");
			return true;
		}
		else playerAck.sunReady = false;
	}
	if ( "破灭之愿" == playerAck.pSkill->name )
	{
		if ( 0 < playerAck.doomDesireRound ) 
		{
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "失败了");
			return false;
		}
		if ( -1 == playerAck.doomDesireRound )
		{
			playerAck.doomDesireRound = 3;
			return true;
		}
		playerAck.doomDesireRound = -1;
	}
	if ( "预知未来" == playerAck.pSkill->name )
	{
		if ( 0 < playerAck.predictRound ) 
		{
			m_pCurRound->log.push_back(playerAck.pCurPet->nick + "失败了");
			return false;
		}
		if ( -1 == playerAck.predictRound )
		{
			playerAck.predictRound = 3;
			return true;
		}
		playerAck.predictRound = -1;
	}


	//非辅助技能
	if ( ImmuneSkill(playerAck, playerDef) ) //免疫
	{
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "免疫了技能");
		return false;
	}
	if ( !CalHitRate(playerAck, playerDef) )//miss
	{
		m_pCurRound->log.push_back(playerAck.pCurPet->nick + "的攻击没有命中" );
		return false;
	}
	if ( Battle::attack == playerDef.act && "保护" == playerDef.pSkill->name && playerDef.defensed ) 
	{
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "保护住了自己");
		return false;
	}
	//干扰技能
	if ( 2 == playerAck.pSkill->type ) 
	{
		if ( "魔装反射" == playerDef.pTalent->name ) 
		{
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "的" 
				+ playerDef.pTalent->name + "反弹了技能");
			return InterfereSkill(playerAck, playerAck);
		}
		return InterfereSkill(playerAck, playerDef);
	}
	//攻击技能
	bool ct = CriticalHit(playerAck, playerDef);
	int G = 0, F = 0, Power = 0;
	if ( 1 == playerAck.pSkill->type ) 
	{
		G = CalWG(playerAck, ct, playerDef);
		F = CalWF(playerDef, ct, playerAck);
	}
	else if ( 3 == playerAck.pSkill->type )
	{
		G = CalTG(playerAck, ct, playerDef);
		if ( "精神冲击" == playerAck.pSkill->name ) F = CalWF(playerDef, ct, playerAck);
		else F = CalTF(playerDef, ct, playerAck);
	}
	Power = CalPower(playerAck, ct, playerDef);
	if ( 0 == Power ) 
	{
		m_pCurRound->log.push_back( "发现程序漏洞：ImmuneSkill()方法未考虑所有免疫情况" );
		return false;
	}

	if ( ct ) 		
	{
		m_pCurRound->log.push_back(playerAck.pCurPet->nick + "暴击！！！宝宝威武！！！" );
	}


	playerAck.outputHurt = G*Power/F*0.84*playerAck.rp.hurt/255;
	bool unFaint = false;
	if ( "逆刃刀" == playerAck.pSkill->name ) unFaint = true;
	else if ( Battle::attack == playerDef.act && "忍耐" == playerDef.pSkill->name && playerDef.defensed ) unFaint = true;
	else if ( ("结实" == playerDef.pTalent->name || "免死金牌" == playerAck.pItem->name)
		&& playerDef.pCurPet->curHP == playerDef.pCurPet->HP ) 
	{
		unFaint = true;
		playerDef.pCurPet->itemId = 0;
	}
	if ( 0 == playerDef.tiShen )
	{
		playerAck.outputHurt = Hurt(playerDef, playerAck.outputHurt, unFaint); 
		playerDef.attacked = true;
	}
	else
	{
		playerAck.outputHurt = playerAck.outputHurt <= playerDef.tiShen?playerAck.outputHurt:playerDef.tiShen;
		m_pCurRound->log.push_back( playerDef.pCurPet->nick + "的替身承受了伤害" );
	}
	if ( "破灭之愿" == playerAck.pSkill->name || "预知未来" == playerAck.pSkill->name ) return true;

	AttackEffect(playerAck, playerDef);

	return true;
}

bool Battle::InterfereSkill(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef)
{
	if ( "撒菱" == playerAck.pSkill->name )//		地	0	2	101	0	使上场巴迪受到1/8伤害
	{
		if ( playerDef.nail[Race::di] )
		{
			m_pCurRound->log.push_back("不能再撒钉子了");
			return false;
		}
		playerDef.nail[Race::di] = true;
		m_pCurRound->log.push_back(playerDef.name + "场上撒满了钉子，巴迪上场时将受到最大HP的1/4的伤害");
		return true;
	}


	if ( "吼叫" == playerAck.pSkill->name //	普	0	2	101	0	最后出手，强制对方退场
		|| "吹飞" == playerAck.pSkill->name )//		飞	0	2	101	0	最后出手，强制对方退场
	{
		ForcedLeave(playerDef);
		EntryStage(playerDef, playerAck);
		return true;
	}

	if ( 0 < playerDef.tiShen )
	{
		m_pCurRound->log.push_back("替身承受了攻击");
		return false;
	}
	if ( "鬼火" == playerAck.pSkill->name )//		火	0	2	95	0	烧伤
	{
		if ( ImmuneState(playerDef) )
		{
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "免疫了攻击");
			return false;
		}
		playerDef.pCurPet->state = Race::huo;
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "被烧伤");
		return true;
	}
	if ( "电磁波" == playerAck.pSkill->name )//		电	0	2	100	0	麻痹
	{
		if ( ImmuneState(playerDef) || "柔软" == playerDef.pTalent->name )
		{
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "免疫了攻击");
			return false;
		}
		playerDef.pCurPet->state = Race::dian;
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "被麻痹");
		return true;
	}
	if ( "麻痹粉" == playerAck.pSkill->name )//		草	0	2	75	0	麻痹
	{
		if ( ImmuneState(playerDef) || "柔软" == playerDef.pTalent->name )
		{
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "免疫了攻击");
			return false;
		}
		playerDef.pCurPet->state = Race::dian;
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "被麻痹");
		return true;
	}
	if ( "剧毒" == playerAck.pSkill->name )//		毒	0	2	85	0	中毒
	{
		if ( ImmuneState(playerDef) || "免疫" == playerDef.pTalent->name )
		{
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "免疫了攻击");
			return false;
		}
		playerDef.pCurPet->state = Race::du;
		playerDef.duRound = 0;
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "中毒了");
		return true;
	}
	if ( "催眠术" == playerAck.pSkill->name //		超	0	2	60	0	催眠对方
		||"恶魔吻" == playerAck.pSkill->name //		普	0	2	75	0	催眠对方
		||"黑洞" == playerAck.pSkill->name //		恶	0	2	80	0	催眠对方
		||"睡眠粉" == playerAck.pSkill->name )//		草	0	2	100	0	催眠对方
	{
		if ( ImmuneState(playerDef) || "失眠" == playerDef.pTalent->name )
		{
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "免疫了攻击");
			return false;
		}
		playerDef.pCurPet->state = Race::pu;
		playerDef.pCurPet->sleepRound = playerDef.rp.sleepRound;
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "睡着了");
		return true;
	}

	if ( "吞噬" == playerAck.pSkill->name //	恶	0	2	101	1	禁止换人和逃跑。非战斗使用可以吞掉物品，法宝转换为正能量
		|| "黑眼" == playerAck.pSkill->name )//	黑眼	普	0	2	101	0	防止对方换人和逃跑
	{
		if ( "逃走" == playerDef.pTalent->name ) 
		{
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "免疫了攻击");
			return false;
		}
		playerDef.banChangeRound = -1;
		playerDef.sdCorrect *= 2;
		if ( "吞噬" == playerAck.pSkill->name ) 
		{
			playerDef.wgCorrect *= 1.5;
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "不能逃跑,但速度急剧上升，物理攻击提升");
		}
		if ( "黑眼" == playerAck.pSkill->name )
		{
			playerDef.tgCorrect *= 1.5;
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "不能逃跑,但速度急剧上升，特殊攻击提升");
		}
		return true;
	}
	if ( "莽撞" == playerAck.pSkill->name )//		普	0	2	100	0	对方HP减少到与自己相同
	{
		if ( playerDef.pCurPet->curHP > playerAck.pCurPet->curHP )
		{
			playerAck.outputHurt = Hurt(playerDef, playerDef.pCurPet->curHP - playerAck.pCurPet->curHP);
		}
		return true;
	}
	if ( "掉包" == playerAck.pSkill->name )//		恶	0	2	100	0	交换物品
	{
		if ( 0 == playerAck.pCurPet->itemId || 0 == playerDef.pCurPet->itemId )
		{
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "没有物品失败");
			return false;
		}
		int id = playerAck.pCurPet->itemId;
		playerAck.pCurPet->itemId = playerDef.pCurPet->itemId;
		playerDef.pCurPet->itemId = id;
		data::ITEM *pItem = playerAck.pItem;
		playerAck.pItem = playerDef.pItem;
		playerDef.pItem = pItem;
		if ( "专爱头巾" == playerAck.pItem->name )
		{
			playerAck.lockSkillTime = -1;
		}
		else
		{
			playerAck.lockSkillTime = playerAck.luanWu;
			if ( playerAck.sunReady ) playerAck.lockSkillTime = 2;
		}
		if ( "专爱头巾" == playerDef.pItem->name )
		{
			playerDef.lockSkillTime = -1;
		}
		else
		{
			playerDef.lockSkillTime = playerDef.luanWu;
			if ( playerDef.sunReady ) playerDef.lockSkillTime = 2;
		}
		playerDef.lockSkillTime = ("专爱头巾" == playerDef.pItem->name?-1:0);//锁定技能回合数量,-1永久
		m_pCurRound->log.push_back(playerAck.pCurPet->nick + "得到了" + playerAck.pItem->name);
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "得到了" + playerDef.pItem->name);
		return true;
	}
	if ( "寄生种子" == playerAck.pSkill->name )//		草	0	2	85	0	每回合吸收1/8体力，对方换下解除
	{
		if ( playerDef.seed || Race::cao == playerDef.pBuddy->race1 || Race::cao == playerDef.pBuddy->race2 )
		{
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "免疫了攻击");
			return false;
		}
		playerDef.seed = true;
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "被种下了种子");
		return true;
	}
	if ( "哈欠" == playerAck.pSkill->name )//		普	0	2	100	0	对方下回合被催眠
	{
		if ( -1 != playerDef.haQian )
		{
			m_pCurRound->log.push_back("失败了");
			return false;
		}
		if ( ImmuneState(playerDef) || "失眠" == playerDef.pTalent->name )
		{
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "免疫了攻击");
			return false;
		}
		playerDef.haQian = 1;
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "昏昏欲睡");
		return true;
	}
	if ( "鼓掌" == playerAck.pSkill->name )//		普	0	2	100	0	3回合不能更换技能
	{
		if ( 0 == playerDef.lockSkill || 0 != playerDef.lockSkillTime )
		{
			m_pCurRound->log.push_back("失败了");
			return false;
		}
		playerDef.guZhang = true;
		playerDef.lockSkillTime = 3;
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "受到怂恿，连续3回合不能更换技能");
		return true;
	}
	if ( "寻衅" == playerAck.pSkill->name )//		恶	0	2	100	0	使对方不能连续使用相同技能
	{
		if ( playerDef.xunXing )
		{
			m_pCurRound->log.push_back("没有效果");
			return false;
		}
		playerDef.xunXing = true;
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "不能连续使用相同技能");
		return true;
	}
	if ( "挑拨" == playerAck.pSkill->name )//		恶	0	2	100	0	使对方3回合不能使用变化技能
	{
		playerDef.tiaoDou = 3;
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "3回合不能使用辅助技能");
		return true;
	}
	if ( "封印" == playerAck.pSkill->name )//		超	0	2	101	0	对方不能使用我方会的技能
	{
		if ( playerDef.ban )
		{
			m_pCurRound->log.push_back("失败了");
			return false;
		}
		playerDef.ban = true;
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "不能使用我方拥有的技能");
		return true;
	}
	if ( "同归" == playerAck.pSkill->name )//		鬼	0	2	101	0	受到攻击体力归0时，拉对手一起倒下
	{
		if ( playerDef.tongGui )
		{
			m_pCurRound->log.push_back("失败了");
			return false;
		}
		playerDef.tongGui = true;
		m_pCurRound->log.push_back(playerAck.pCurPet->nick + "牺牲时拖" + playerDef.pCurPet->nick + "一起");
		return true;
	}
	if ( "奇怪光" == playerAck.pSkill->name )//		鬼	0	2	100	0	混乱
	{
		if ( ImmuneState(playerDef) )
		{
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "免疫了攻击");
			return false;
		}
		playerDef.pCurPet->state = Race::gui;
		playerDef.hunLuan = 3;
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "混乱了");
		return true;
	}

	return false;
}

bool Battle::HelpSkill(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef)
{
	if ( "保护" == playerAck.pSkill->name || "忍耐" == playerAck.pSkill->name )
	{
		if ( !playerAck.defensed ) m_pCurRound->log.push_back("但是失败了");
		return true;
	}
	if ( "晴天" == playerAck.pSkill->name )//		火	0	2	101	0	5回合晴天
	{
		if ( Race::huo == m_weather )
		{
			m_pCurRound->log.push_back("已经是晴天，失败了");
			return true;
		}
		m_weather = Race::huo;//天气属性对应
		m_weatherCount = 5;//天气剩余回合数 -1永久
		if ( "磁盘" == playerAck.pItem->name ) m_weatherCount = 8;
		m_pCurRound->log.push_back("太阳出来了");
	}
	else if ( "求雨" == playerAck.pSkill->name )//		水	0	2	101	0	5回合雨天
	{
		if ( Race::shui == m_weather )
		{
			m_pCurRound->log.push_back("已经在下雨，失败了");
			return true;
		}
		m_weather = Race::shui;//天气属性对应
		m_weatherCount = 5;//天气剩余回合数 -1永久
		if ( "磁盘" == playerAck.pItem->name ) m_weatherCount = 8;
		m_pCurRound->log.push_back("下雨了");
	}
	else if ( "冰雹" == playerAck.pSkill->name )//		冰	0	2	101	0	5回合冰雹天气，冰属性以外的巴迪每回合开始受到1/16伤害
	{
		if ( Race::bing == m_weather )
		{
			m_pCurRound->log.push_back("已经在下雪，失败了");
			return true;
		}
		m_weather = Race::bing;//天气属性对应
		m_weatherCount = 5;//天气剩余回合数 -1永久
		if ( "磁盘" == playerAck.pItem->name ) m_weatherCount = 8;
		m_pCurRound->log.push_back("下雪了");
	}
	else if ( "沙暴" == playerAck.pSkill->name )//		岩	0	2	101	0	5回合沙暴天气，地岩钢以外属性巴迪受到1/16伤害
	{
		if ( Race::yan == m_weather )
		{
			m_pCurRound->log.push_back("已经在沙尘暴中，失败了");
			return true;
		}
		m_weather = Race::yan;//天气属性对应
		m_weatherCount = 5;//天气剩余回合数 -1永久
		if ( "磁盘" == playerAck.pItem->name ) m_weatherCount = 8;

		m_pCurRound->log.push_back("沙尘暴来了");
	}
	else if ( "光之壁" == playerAck.pSkill->name )//		超	0	2	101	0	5回合特殊伤害减半
	{
		if ( 0 < playerAck.wall[1] )
		{
			m_pCurRound->log.push_back("已经有了光之壁，失败了");
			return true;
		}
		if ( "磁盘" == playerAck.pItem->name ) 
		{
			playerAck.wall[1] = 8;
			m_pCurRound->log.push_back("我方受到光之壁保护，8回合内特殊伤害减半");
		}
		else
		{
			playerAck.wall[1] = 5;
			m_pCurRound->log.push_back("我方受到光之壁保护，5回合内特殊伤害减半");
		}
	}
	else if ( "反射盾" == playerAck.pSkill->name )//		超	0	2	101	0	5回合物理伤害减半
	{
		if ( 0 < playerAck.wall[0] )
		{
			m_pCurRound->log.push_back("已经有了反射盾，失败了");
			return true;
		}
		if ( "磁盘" == playerAck.pItem->name ) 
		{
			playerAck.wall[0] = 8;
			m_pCurRound->log.push_back("我方受到反射盾保护，8回合内物理伤害减半");
		}
		else
		{
			playerAck.wall[0] = 5;
			m_pCurRound->log.push_back("我方受到反射盾保护，5回合内物理伤害减半");
		}
	}
	else if ( "欺骗空间" == playerAck.pSkill->name )//		超	0	2	101	0	5回合速度慢的先行动
	{
		if ( 0 != m_foolSpace )
		{
			m_pCurRound->log.push_back("已经制造欺骗空间，失败了");
			return true;
		}
		if ( "磁盘" == playerAck.pItem->name ) 
		{
			m_foolSpace = 8;
			m_pCurRound->log.push_back("场地转换为欺骗空间，8回合内速度低的先行动");
		}
		else
		{
			m_foolSpace = 5;
			m_pCurRound->log.push_back("场地转换为欺骗空间，5回合内速度低的先行动");
		}
	}
	else if ( "黑雾" == playerAck.pSkill->name )//		冰	0	2	101	0	全体能力等级复原
	{
		playerAck.wg = playerDef.wg = 0;//攻强化等级
		playerAck.wf = playerDef.wf = 0;//防强化等级
		playerAck.tg = playerDef.tg = 0;//特攻强化等级
		playerAck.tf = playerDef.tf = 0;//特防强化等级
		playerAck.sd = playerDef.sd = 0;//速度强化等级
		playerAck.ct = playerDef.ct = 0;//暴击强化等级
		m_pCurRound->log.push_back("双方能力变化消除了");
	}
	else if ( "吃撑了" == playerAck.pSkill->name )//		普	0	2	101	0	极限物攻
	{
		if ( playerAck.pCurPet->curHP < playerAck.pCurPet->HP / 2 )
		{
			m_pCurRound->log.push_back("体力不足，失败了");
			return true;
		}
		m_pCurRound->log.push_back("损失一半的体力，物理攻击提升到极限了");
		playerAck.wg = 6;
		Hurt(playerAck, playerAck.pCurPet->HP / 2);
	}
	else if ( "剑舞" == playerAck.pSkill->name )//		普	0	2	101	0	大幅强化物攻
	{
		if ( 6 == playerAck.wg )
		{
			m_pCurRound->log.push_back("物理攻击已达到极限，失败了");
			return true;
		}
		playerAck.wg += 2;
		m_pCurRound->log.push_back("物理攻击大幅上升");
	}
	else if ( "硬梆梆" == playerAck.pSkill->name )//		普 	0	2	100	0	大幅强化物防
	{
		if ( 6 == playerAck.wf )
		{
			m_pCurRound->log.push_back("物理防御已达到极限，失败了");
			return true;
		}
		if ( 6 > playerAck.wf  )
		{
			playerAck.wf += 2;
			m_pCurRound->log.push_back("物理防御大幅上升");
		}
	}
	else if ( "萤火" == playerAck.pSkill->name )//		虫	0	2	101	0	大幅强化特攻
	{
		if ( 6 == playerAck.tg )
		{
			m_pCurRound->log.push_back("特殊攻击已达到极限，失败了");
			return true;
		}
		if ( 6 > playerAck.tg  )
		{
			playerAck.tg += 2;
			m_pCurRound->log.push_back("特殊攻击大幅上升");
		}
	}
	else if ( "超级健忘" == playerAck.pSkill->name )//		超	0	2	101	0	大幅强化特防
	{
		if ( 6 == playerAck.tf )
		{
			m_pCurRound->log.push_back("特殊防御已达到极限，失败了");
			return true;
		}
		if ( 6 > playerAck.tf  )
		{
			playerAck.tf += 2;
			m_pCurRound->log.push_back("特殊防御大幅上升");
		}
	}
	else if ( "高速移动" == playerAck.pSkill->name )//		超	0	2	101	0	大幅强化速度
	{
		if ( 6 == playerAck.sd )
		{
			m_pCurRound->log.push_back("速度已达到极限，失败了");
			return true;
		}
		if ( 6 > playerAck.sd  )
		{
			playerAck.sd += 2;
			m_pCurRound->log.push_back("速度大幅上升");
		}
	}
	else if ( "巨大化" == playerAck.pSkill->name )//		斗	0	2	101	0	强化物攻，强化物防
	{
		if ( 6 == playerAck.wg && 6 == playerAck.wf )
		{
			m_pCurRound->log.push_back("物理攻击和物理防御已达到极限，失败了");
			return true;
		}
		if ( 6 > playerAck.wg  )
		{
			playerAck.wg++;
			m_pCurRound->log.push_back("提高了物理攻击");
		}
		if ( 6 > playerAck.wf  )
		{
			playerAck.wf++;
			m_pCurRound->log.push_back("提高了物理防御");
		}
	}
	else if ( "冥想" == playerAck.pSkill->name )//		超	0	2	101	0	强化特攻，强化特防
	{
		if ( 6 == playerAck.tg && 6 == playerAck.tf )
		{
			m_pCurRound->log.push_back("特殊攻击和特殊防御已达到极限，失败了");
			return true;
		}
		if ( 6 > playerAck.tg  )
		{
			playerAck.tg++;
			m_pCurRound->log.push_back("提高了特殊攻击");
		}
		if ( 6 > playerAck.tf  )
		{
			playerAck.tf++;
			m_pCurRound->log.push_back("提高了特殊防御");
		}
	}
	else if ( "磨爪子" == playerAck.pSkill->name )//		恶	0	2	101	0	强化物攻，提高暴击率
	{
		if ( 6 == playerAck.wg && 6 == playerAck.ct )
		{
			m_pCurRound->log.push_back("强化已达到极限，失败了");
			return true;
		}
		if ( 6 > playerAck.wg  )
		{
			playerAck.wg++;
			m_pCurRound->log.push_back("提高了物理攻击");
		}
		if ( 6 > playerAck.ct ) 
		{
			playerAck.ct++;
			m_pCurRound->log.push_back("提高了提高暴击率");
		}
	}
	else if ( "充电" == playerAck.pSkill->name )//		电	0	2	101	0	强化特攻，电系威力*1.5
	{
		if ( 6 == playerAck.tg && playerAck.race[Race::dian] )
		{
			m_pCurRound->log.push_back("强化已达到极限，失败了");
			return true;
		}
		if ( 6 > playerAck.tg  )
		{
			playerAck.tg++;
			m_pCurRound->log.push_back("提高了特殊攻击");
		}
		if ( !playerAck.race[Race::dian] ) 
		{
			playerAck.race[Race::dian] = true;
			m_pCurRound->log.push_back("强化了电系威力");
		}
	}
	else if ( "龙之舞" == playerAck.pSkill->name )//		龙	0	2	101	0	强化物攻，强化速度
	{
		if ( 6 == playerAck.wg && 6 == playerAck.sd )
		{
			m_pCurRound->log.push_back("物理攻击和速度已达到极限，失败了");
			return true;
		}
		if ( 6 > playerAck.wg  )
		{
			playerAck.wg++;
			m_pCurRound->log.push_back("提高了物理攻击");
		}
		if ( 6 > playerAck.sd  )
		{
			playerAck.sd++;
			m_pCurRound->log.push_back("提高了速度");
		}
	}
	else if ( "蝶之舞" == playerAck.pSkill->name )//		虫	0	2	101	0	强化特攻，强化特防，强化速度
	{
		if ( 6 == playerAck.tg && 6 == playerAck.tf && 6 == playerAck.sd )
		{
			m_pCurRound->log.push_back("特殊攻击、特殊防御、速度已达到极限，失败了");
			return true;
		}
		if ( 6 > playerAck.tg  )
		{
			playerAck.tg++;
			m_pCurRound->log.push_back("提高了特殊攻击");
		}
		if ( 6 > playerAck.tf  )
		{
			playerAck.tf++;
			m_pCurRound->log.push_back("提高了特殊防御");
		}
		if ( 6 > playerAck.sd  )
		{
			playerAck.sd++;
			m_pCurRound->log.push_back("提高了速度");
		}
	}
	else if ( "吃奶" == playerAck.pSkill->name //		普	0	2	101	0	回复50%体力
		|| "羽栖" == playerAck.pSkill->name ) //		飞	0	2	101	0	回复50%体力
	{
		playerAck.pCurPet->curHP += playerAck.pCurPet->HP / 2;
		if ( playerAck.pCurPet->curHP > playerAck.pCurPet->HP ) 
		{
			playerAck.pCurPet->curHP = playerAck.pCurPet->HP;
		}
		m_pCurRound->log.push_back("回复了体力");
	}
	else if ( "光合作用" == playerAck.pSkill->name //		草	0	2	101	0	回复50%HP,晴天2倍，其它天气减半
		|| "晨光" == playerAck.pSkill->name //		普	0	2	101	0	回复50%HP,晴天2倍，其它天气减半
		|| "月光" == playerAck.pSkill->name )//		普	0	2	101	0	回复50%HP,晴天2倍，其它天气减半
	{
		if ( 0 == m_weather ) playerAck.pCurPet->curHP += playerAck.pCurPet->HP / 2;
		else if ( Race::huo == m_weather ) playerAck.pCurPet->curHP += playerAck.pCurPet->HP;
		else if ( Race::huo != m_weather ) playerAck.pCurPet->curHP += playerAck.pCurPet->HP / 4;
		if ( playerAck.pCurPet->curHP > playerAck.pCurPet->HP ) 
		{
			playerAck.pCurPet->curHP = playerAck.pCurPet->HP;
		}
		m_pCurRound->log.push_back("回复了体力");
	}
	else if ( "清新" == playerAck.pSkill->name )//		普	0	2	101	0	解除自身异常
	{
		if ( 0 == playerAck.pCurPet->state )
		{
			m_pCurRound->log.push_back("没有异常状态,失败了");
			return true;
		}
		playerAck.pCurPet->state = 0;
		m_pCurRound->log.push_back("恢复了健康");
	}
	else if ( "治愈铃声" == playerAck.pSkill->name //		普	0	2	101	0	我方全体异常解除
		|| "芳香疗法" == playerAck.pSkill->name )//		草	0	2	101	0	全体异常解除
	{
		int i = 0;
		for ( i = 0; i < playerAck.pets.size(); i++ )
		{
			if ( playerAck.pets[i].curHP <= 0 ) continue;
			playerAck.pets[i].state = 0;
		}
		m_pCurRound->log.push_back("全队巴迪恢复了健康");
	}
	else if ( "急救液" == playerAck.pSkill->name )//		水	0	2	101	0	5回合，每回合回复1/16HP
	{
		playerAck.recvHP = playerAck.pCurPet->HP / 16;
		m_pCurRound->log.push_back("被营养液包围着，每回合回复体力");
	}
	else if ( "睡觉" == playerAck.pSkill->name )//		超	0	2	101	0	睡3回合，HP全满，状态解除
	{
		playerAck.pCurPet->curHP = playerAck.pCurPet->HP;
		playerAck.pCurPet->state = Race::pu;
		playerAck.pCurPet->sleepRound = 3;
		m_pCurRound->log.push_back("睡着了，回复了全盛状态");
	}
	else if ( "替身" == playerAck.pSkill->name )//		普	0	2	100	0	损失1/4HP制造替身
	{
		if ( 0 < playerAck.tiShen )
		{
			m_pCurRound->log.push_back("已经有替身，失败了");
			return true;
		}
		if ( playerAck.pCurPet->curHP <= playerAck.pCurPet->HP / 4 )
		{
			m_pCurRound->log.push_back("体力不足，失败了");
			return true;
		}
		m_pCurRound->log.push_back("消耗1/4体力，制造了一个替身");
		playerAck.tiShen = playerAck.pCurPet->HP / 4;
		Hurt(playerAck, playerAck.tiShen);
	}
	else if ( "梦话" == playerAck.pSkill->name )//		普	0	2	101	0	睡着时，随机使用技能
	{
		if ( Race::pu != playerAck.pCurPet->state )
		{
			m_pCurRound->log.push_back("失败了");
			return true;
		}
		int skills[5];
		skills[0] = playerAck.pCurPet->skill1;
		skills[1] = playerAck.pCurPet->skill2;
		skills[2] = playerAck.pCurPet->skill3;
		skills[3] = playerAck.pCurPet->skill4;
		skills[4] = 0;
		int i = 0;
		int j = 0;
		for ( i = 0; i < 4; i++ )
		{
			if ( playerAck.pSkill->id == skills[i] 
			|| (0 != skills[i] && !Skill(skills[i], m_game->SkillBook()))
				) 
			{
				for ( j = i; j < 4; j++ ) skills[j] = skills[j+1];
				i = -1;
				continue;
			}
		}
		for ( i = 0; i < 3 && 0 != skills[i]; i++ );
		if ( 0 == i )
		{
			m_pCurRound->log.push_back("无技能可用");
			return true;
		}
		int id = skills[rand()%i];
		playerAck.pSkill = Skill(id, m_game->SkillBook());
		if ( !UseSkill(playerAck, playerDef) )
		{
			if ( "大爆炸" == playerAck.pSkill->name ) Hurt(playerAck, playerAck.pCurPet->curHP);
		}
	}
	else if ( "香甜气息" == playerAck.pSkill->name )//		草	0	2	100	1	非战斗使用可引出野生巴迪
	{
		m_pCurRound->log.push_back("没有效果");
	}
	else if ( "嗅觉" == playerAck.pSkill->name )//		普	0	2	101	0	普和斗属性技能可打中鬼系巴迪
	{
		playerAck.smell = true;
		m_pCurRound->log.push_back("普系技能可以击中鬼系");
	}
	else if ( "末日歌" == playerAck.pSkill->name )//		普	0	2	101	0	听过的巴迪，3回合内不换下，则濒死
	{
		if ( 0 < playerDef.mieGe && 0 < playerAck.mieGe )
		{
			m_pCurRound->log.push_back("没有效果");
			return true;
		}
		if ( 0 == playerAck.mieGe ) playerAck.mieGe = 3;
		if ( 0 == playerDef.mieGe ) playerDef.mieGe = 3;
	}
	else if ( "分享痛楚" == playerAck.pSkill->name )//		普	0	2	100	0	双方HP平分
	{
		m_pCurRound->log.push_back("与对方平分了体力");
		int n = (playerAck.pCurPet->curHP + playerDef.pCurPet->curHP) / 2;
		if ( n >= playerAck.pCurPet->curHP ) playerAck.pCurPet->curHP = n;
		else Hurt(playerAck, playerAck.pCurPet->curHP - n);
		if ( n >= playerDef.pCurPet->curHP ) playerDef.pCurPet->curHP = n;
		else Hurt(playerDef, playerDef.pCurPet->curHP - n);
	}
	else return false;
	
	return true;
}

bool Battle::ActionAble(Battle::WARRIOR &player)
{
	if ( player.isActioned ) return false;

	if ( Race::pu == player.pCurPet->state ) 
	{
		if ( 0 < player.pCurPet->sleepRound ) return false;
	}
	else if ( Race::bing == player.pCurPet->state ) 
	{
		if ( 0 < player.pCurPet->frozenRound ) return false;
	}
	else if ( Race::dian == player.pCurPet->state ) 
	{
		if ( 25 >= player.rp.dian ) return false;
	}

	return true;
}

bool Battle::LaunchState(Battle::WARRIOR &player)
{
	if ( Race::pu == player.pCurPet->state ) 
	{
		if ( 0 == player.pCurPet->sleepRound ) 
		{
			player.pCurPet->state = 0;
			m_pCurRound->log.push_back( player.pCurPet->nick + "醒来了" );
			return false;
		}
		m_pCurRound->log.push_back( player.pCurPet->nick + "正在睡觉" );
		return true;
	}
	else if ( Race::bing == player.pCurPet->state ) 
	{
		if ( 0 == player.pCurPet->frozenRound )
		{
			player.pCurPet->state = 0;
			m_pCurRound->log.push_back( player.pCurPet->nick + "解除了冰封" );
			return false;
		}
		m_pCurRound->log.push_back( player.pCurPet->nick + "被冰封着" );
		return true;
	}
	else if ( Race::dian == player.pCurPet->state ) 
	{
		if ( 25 >= player.rp.dian ) return false;
		m_pCurRound->log.push_back( player.pCurPet->nick + "麻痹了，不能行动" );
		return true;
	}

	return false;
}

bool Battle::Medication(Battle::WARRIOR &player)
{
	if ( 0 == player.pCurPet->state || "万能药" != player.pItem->name ) return false;

	if ( Race::pu == player.pCurPet->state ) 
	{
		if ( 0 == player.pCurPet->sleepRound ) return false;
		player.pCurPet->state = 0;
		m_pCurRound->log.push_back( m_player.pCurPet->nick + "使用了万能药，解除了睡眠" );
	}
	else if ( Race::bing == player.pCurPet->state ) 
	{
		if ( 0 == player.pCurPet->frozenRound ) return false;
		player.pCurPet->state = 0;
		m_pCurRound->log.push_back( m_player.pCurPet->nick + "使用了万能药，解除了冰封" );
	}
	else if ( Race::dian == player.pCurPet->state ) 
	{
		if ( 25 >= player.rp.dian ) return false;
		m_pCurRound->log.push_back( m_player.pCurPet->nick + "使用了万能药，解除了麻痹" );
	}
	else if ( Race::huo == player.pCurPet->state ) 
	{
		m_pCurRound->log.push_back( m_player.pCurPet->nick + "使用了万能药，解除了烧伤" );
	}
	else if ( Race::gui == player.pCurPet->state ) 
	{
		m_pCurRound->log.push_back( m_player.pCurPet->nick + "使用了万能药，解除了混乱" );
	}
	player.pCurPet->state = 0;

	return false;
}

void Battle::ChangeSkill(Battle::WARRIOR &player)
{
	if ( player.xunXing ) 
	{
		if ( player.lockSkill == player.pSkill->id ) 
		{
			player.pSkill = m_game->BornSkill();
		}
		return;
	}
	if ( 0 == player.lockSkillTime ) return;
	data::SKILL *pSkill = Skill(player.lockSkill, m_game->SkillBook());
	if ( NULL == pSkill || pSkill == player.pSkill ) return;
	player.pSkill = pSkill;
	m_pCurRound->log.push_back(player.pCurPet->nick + "在掌声中不能更换技能");
}

bool Battle::BanSkill(Battle::WARRIOR &player, Battle::WARRIOR &enemy)
{
	if ( !player.ban ) return false;
	if ( player.pSkill->id == enemy.pCurPet->skill1
		|| player.pSkill->id == enemy.pCurPet->skill2
		|| player.pSkill->id == enemy.pCurPet->skill3
		|| player.pSkill->id == enemy.pCurPet->skill4
		) return true; 

	return false;
}

bool Battle::Confusion(Battle::WARRIOR &player)
{
	if ( Race::gui != player.pCurPet->state ) return false;
	if ( 0 == player.hunLuan )
	{
		player.pCurPet->state = 0;
		player.hunLuan = -1;
		m_pCurRound->log.push_back( player.pCurPet->nick + "解除了混乱" );
		return false;
	}
	m_pCurRound->log.push_back( player.pCurPet->nick + "混乱了" );
	if ( !player.rp.luan ) return false;
	m_pCurRound->log.push_back( player.pCurPet->nick + "误伤了自己" );
	player.pSkill = m_game->BornSkill();

	return true;
}

bool Battle::AttackEffect(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef)
{
	bool duShou = false;
	bool haiPa = false;
	int i = 0;
	for ( i = 0; i < playerAck.pSkill->effects.size(); i++ )
	{
		if ( 0 < playerDef.pCurPet->curHP && !ImmuneState(playerDef) )
		{
			if ( 1 == playerAck.pSkill->effects[i].id ) //huo
			{
				if ( playerAck.rp.sePro > playerAck.pSkill->effects[i].probability )  continue;
				playerDef.pCurPet->state = Race::huo;
				m_pCurRound->log.push_back(playerDef.pCurPet->nick + "烧伤了");
			}
			else if ( 2 == playerAck.pSkill->effects[i].id 
				&& "柔软" != playerDef.pTalent->name ) //dian
			{
				if ( playerAck.rp.sePro > playerAck.pSkill->effects[i].probability )  continue;
				playerDef.pCurPet->state = Race::dian;
				m_pCurRound->log.push_back(playerDef.pCurPet->nick + "麻痹了");
			}
			else if ( 3 == playerAck.pSkill->effects[i].id 
				&& "火焰体" != playerDef.pTalent->name ) //frozen
			{
				if ( playerAck.rp.sePro > playerAck.pSkill->effects[i].probability )  continue;
				playerDef.pCurPet->state = Race::bing;
				playerDef.pCurPet->frozenRound = playerDef.rp.frozenRound;
				m_pCurRound->log.push_back(playerDef.pCurPet->nick + "冰冻了");
			}
			else if ( 5 == playerAck.pSkill->effects[i].id
				&& "免疫" != playerDef.pTalent->name ) //du
			{
				duShou = true;
				char addCount = 0;
				if ( "毒手" == playerAck.pTalent->name ) addCount = 30;
				if ( playerAck.rp.sePro > playerAck.pSkill->effects[i].probability + addCount )  continue;
				playerDef.pCurPet->state = Race::du;
				playerDef.duRound = 0;
				m_pCurRound->log.push_back(playerDef.pCurPet->nick + "中毒了");
			}
		}
		if ( 0 < playerDef.pCurPet->curHP && "磷粉" != playerDef.pTalent->name )
		{
			if ( 42 == playerAck.pSkill->effects[i].id && "净体" != playerDef.pTalent->name ) //-WF
			{
				if ( playerDef.wf <= -6
					|| 100 == playerAck.pSkill->effects[i].probability 
					|| playerAck.rp.sePro > playerAck.pSkill->effects[i].probability 
					)  continue;
				playerDef.wf--;
				m_pCurRound->log.push_back(playerDef.pCurPet->nick + "防御下降了");
			}
			else if ( 44 == playerAck.pSkill->effects[i].id && "净体" != playerDef.pTalent->name ) //-TF
			{
				if ( playerDef.tf <= -6
					|| 100 == playerAck.pSkill->effects[i].probability 
					|| playerAck.rp.sePro > playerAck.pSkill->effects[i].probability 
					)  continue;
				playerDef.tf--;
				m_pCurRound->log.push_back(playerDef.pCurPet->nick + "特殊防御下降了");
			}
			else if ( 0 < playerDef.pCurPet->curHP 
				&& 86 == playerAck.pSkill->effects[i].id 
				&& "精神力" != playerDef.pTalent->name ) //haki
			{
				haiPa = true;
				char addCount = 0;
				if ( "王冠" == playerAck.pItem->name ) addCount = 15;
				if ( "霸气" == playerAck.pTalent->name ) addCount += 20;
				if ( playerAck.rp.sePro > playerAck.pSkill->effects[i].probability + addCount )  continue;
				playerDef.fear = true;
			}
		}
		if ( 34 == playerAck.pSkill->effects[i].id ) //+WG
		{
			if ( playerAck.wg >= 6 
				|| playerAck.rp.sePro > playerAck.pSkill->effects[i].probability )  continue;
			playerAck.wg++;
			m_pCurRound->log.push_back(playerAck.pCurPet->nick + "攻击上升了");
		}
		if ( 120 == playerAck.pSkill->effects[i].id ) //清场
		{
			if ( playerAck.nail[Race::di] )
			{
				playerAck.nail[Race::di] = false;
				m_pCurRound->log.push_back(playerAck.name + "场上钉子被清除干净了");
			}
			if ( playerAck.seed )
			{
				playerAck.seed = false;
				m_pCurRound->log.push_back(playerAck.pCurPet->nick + "拜托了种子");
			}
		}
	}

	if ( 0 < playerDef.pCurPet->curHP
		&& ("王冠" == playerAck.pItem->name || "霸气" == playerAck.pTalent->name)
		&& "给力" != playerAck.pTalent->name
		&& "精神力" != playerDef.pTalent->name
		&& !haiPa )
	{
		char addCount = 0;
		if ( "王冠" == playerAck.pItem->name ) addCount = 15;
		if ( "霸气" == playerAck.pTalent->name ) addCount += 20;
		if ( playerAck.rp.sePro <= addCount )
		{
			playerDef.fear = true;
		}
	}
	if ( 0 < playerDef.pCurPet->curHP
		&& "毒手" == playerAck.pTalent->name 
		&& !ImmuneState(playerDef)
		&& "免疫" != playerDef.pTalent->name
		&& !duShou )
	{
		if ( playerAck.rp.tePro <= 30 )  
		{
			playerDef.pCurPet->state = Race::du;
			playerDef.duRound = 0;
			m_pCurRound->log.push_back(playerAck.pCurPet->nick + "使用" 
				+ playerAck.pTalent->name + playerDef.pCurPet->nick + "中毒了");
		}
	}

	if ( "蜻蜓回转" == playerAck.pSkill->name ) 
	{
		LeaveStage(playerAck);
		return true;
	}
	else if ( "飘花淡雪浮香吹" == playerAck.pSkill->name 
		|| "鲤鱼摆尾" == playerAck.pSkill->name 
		|| "龙之尾" == playerAck.pSkill->name ) 
	{
		AttackCost(playerAck, playerDef);
		ForcedLeave(playerDef);
		EntryStage(playerDef, playerAck);
		return true;
	}
	AttackCost(playerAck, playerDef);

	return true;
}

bool Battle::ForcedLeave(Battle::WARRIOR &player) 
{
	if ( 1 == player.pets.size() )
	{
		m_pCurRound->log.push_back(player.name + "没有可替换的巴迪");
		return false;
	}
	int i = 0;
	for ( i = 0; true; i++ )
	{
		if ( i >= player.pets.size() ) i = 0;
		if ( player.pCurPet == &player.pets[i] ) continue;
		if ( 0 == rand()%2 ) continue;
		if ( NULL != SetPetInfo(player, player.pets[i].id) ) 
		{
			m_pCurRound->log.push_back(player.name + "队伍中存在非法巴迪");
			return false;
		}

		player.isChanged = ChangePet(player, player.pets[i].id);
		return true;
	}

	return true;
}

bool Battle::AttackCost(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef)
{
	char skillRace = playerAck.pSkill->race;
	if ( "天空皮肤" == playerAck.pTalent->name && Race::pu == skillRace ) skillRace = Race::fei;
	else if ( "冰冻皮肤" == playerAck.pTalent->name && Race::pu == skillRace ) skillRace = Race::bing;

	if ( "大爆炸" == playerAck.pSkill->name ) 
	{
		Hurt(playerAck, playerAck.pCurPet->curHP);
		return true;
	}
	if ( 0 >= playerDef.pCurPet->curHP && playerAck.tongGui ) //同归
	{
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "拉" 
			+ playerAck.pCurPet->nick + "一起倒下了");
		playerAck.pCurPet->curHP = 0;
		Faint(playerAck);
		return true;
	}
	//特性影响
	if ( 1 == playerAck.pSkill->type && !ImmuneState(playerAck) )
	{
		if ( "静电" == playerDef.pTalent->name //受到物理攻击时，30%麻痹对手
			&& "柔软" != playerAck.pTalent->name
			)
		{
			playerAck.pCurPet->state = Race::dian;
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "的" 
				+ playerDef.pTalent->name + "使" + playerAck.pCurPet->nick + "麻痹了");
		}
		if ( "孢子" == playerDef.pTalent->name //受到物理攻击时，30%中毒
			&& "免疫" != playerAck.pTalent->name
			)
		{
			playerAck.pCurPet->state = Race::du;
			playerDef.duRound = 0;
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "的" 
				+ playerDef.pTalent->name + "使" + playerAck.pCurPet->nick + "中毒了");
		}
		if ( "火焰体" == playerDef.pTalent->name )//	不冰冻，受到物理攻击时，30%烧伤
		{
			playerAck.pCurPet->state = Race::huo;
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "的" 
				+ playerDef.pTalent->name + "使" + playerAck.pCurPet->nick + "烧伤了");
		}
		if ( "冰冻身体" == playerDef.pTalent->name //	受到物理攻击时，30%冰冻
			&& "火焰体" != playerAck.pTalent->name)
		{
			playerAck.pCurPet->state = Race::bing;
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "的" 
				+ playerDef.pTalent->name + "使" + playerAck.pCurPet->nick + "冰封了");
		}
	}
	if ( "正义心" == playerDef.pTalent->name && Race::e == skillRace 
		&& 6 > playerDef.wg )//	被恶属性攻击，强化物攻
	{
		playerDef.wg++;
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "的" 
			+ playerDef.pTalent->name + "提高了攻击");
	}
	if ( 1 == playerAck.pSkill->type && "蛇皮" == playerDef.pTalent->name 
		&& "魔法防御" != playerAck.pTalent->name
		&& "石脑" != playerAck.pTalent->name )//反噬
	{
		float hurt = playerAck.outputHurt / 3;
		bool unFaint = false;
		if ( "舍身" == playerAck.pTalent->name ) unFaint = true;
		m_pCurRound->log.push_back(playerAck.pCurPet->nick + "受到反噬");
		Hurt( playerAck, hurt, unFaint );
		if ( 0 >= playerAck.pCurPet->curHP ) return true;//挂了
	}
	if ( ("生命玉" == playerAck.pItem->name || BiteSkill(playerAck.pSkill) )
		&& "魔法防御" != playerAck.pTalent->name 
		&& "石脑" != playerAck.pTalent->name )//反噬
	{
		float hurt = 0;
		if ( BiteSkill(playerAck.pSkill) ) hurt = playerAck.outputHurt / 3;
		if ( "生命玉" == playerAck.pItem->name ) hurt += playerAck.pCurPet->HP / 8;
		bool unFaint = false;
		if ( "舍身" == playerAck.pTalent->name ) unFaint = true;
		m_pCurRound->log.push_back(playerAck.pCurPet->nick + "受到反噬");
		Hurt( playerAck, hurt, unFaint );
		if ( 0 >= playerAck.pCurPet->curHP ) return true;//挂了
	}
	//技能
	if ( "最终手段" == playerAck.pSkill->name )//不能更换技能，不能交换巴迪，不能逃跑
	{
		playerAck.lockSkillTime = -1;
		playerAck.banChangeRound = -1;
	}
	if ( "高压水炮" == playerAck.pSkill->name
		|| "亿万冲击" == playerAck.pSkill->name)//下回合不能行动
	{
		playerAck.rest = true;
	}
	if ( 0 == playerAck.luanWu
		&&
		( "花瓣舞" == playerAck.pSkill->name
		|| "鲤鱼摆尾" == playerAck.pSkill->name
		|| "蜂涌" == playerAck.pSkill->name
		|| "精神病" == playerAck.pSkill->name
		|| "风之伤" == playerAck.pSkill->name
		|| "暴走" == playerAck.pSkill->name
		|| "逆鳞" == playerAck.pSkill->name
		|| "异次元冲锋" == playerAck.pSkill->name
		|| "激流涌进" == playerAck.pSkill->name
		|| "龙之尾" == playerAck.pSkill->name )
		)//乱舞，连续使用2~3回合
	{
		playerAck.luanWu = playerAck.rp.luanWu;
		if ( -1 != playerAck.lockSkillTime ) playerAck.lockSkillTime = playerAck.luanWu;
		if ( -1 != playerAck.banChangeRound ) playerAck.banChangeRound = playerAck.luanWu;
	}
	if ( "净体" != playerAck.pTalent->name )
	{
		if ( "进身击" == playerAck.pSkill->name )//弱化自身物防，弱化自身特防
		{
			if ( -6 < playerAck.wf )
			{
				playerAck.wf--;
				m_pCurRound->log.push_back(playerAck.pCurPet->nick + "的防御降低了");
			}
			if ( -6 < playerAck.tf )
			{
				playerAck.tf--;
				m_pCurRound->log.push_back(playerAck.pCurPet->nick + "的特殊防御降低了");
			}
		}
		if ( "最终手段" == playerAck.pSkill->name )//大幅弱化自身物防
		{
			if ( -6 < playerAck.wf )
			{
				playerAck.wf -=2;
				if ( -6 > playerAck.wf ) playerAck.wf = -6;
				m_pCurRound->log.push_back(playerAck.pCurPet->nick + "的防御大幅下降");
			}
		}
		if ( "燃烧殆尽" == playerAck.pSkill->name 
			|| "叶风暴" == playerAck.pSkill->name 
			|| "精神增压" == playerAck.pSkill->name 
			|| "龙星群" == playerAck.pSkill->name 
			)//大幅弱化自身特攻
		{
			if ( -6 < playerAck.tg )
			{
				playerAck.tg -=2;
				if ( -6 > playerAck.tg ) playerAck.tg = -6;
				m_pCurRound->log.push_back(playerAck.pCurPet->nick + "的特殊攻击大幅下降");
			}
		}			
	}

	return true;
}

bool Battle::ImmuneState(Battle::WARRIOR &player)
{
	if ( 0 < player.tiShen ) return true;
	if ( "磷粉" == player.pTalent->name ) return true;
	if ( ("湿润身躯" == player.pTalent->name && Race::shui == m_weather) ) return true;
	if ( ("叶片防御" == player.pTalent->name && Race::huo == m_weather) ) return true;
	if ( 0 != player.pCurPet->state ) return true;

	return false;
}

bool Battle::Faint(Battle::WARRIOR &player)
{
	if ( player.pCurPet->curHP > 0 ) return false;

	player.pCurPet->curHP = 0;
	m_pCurRound->log.push_back(player.pCurPet->nick + "倒下了");
	if ( 1 == player.pets.size() ) player.lose = true;
	return true;
}

void Battle::PlayerEnd(Battle::WARRIOR &player, Battle::WARRIOR &enemy)
{
	if ( player.isEnd ) return;
	player.isEnd = true;

	if ( 0 < player.doomDesireRound )//破灭之愿剩余回合
	{
		player.doomDesireRound--;
		if ( 0 == player.doomDesireRound ) 
		{
			player.pSkill = Skill("破灭之愿", m_game->SkillBook());
			if ( NULL != player.pSkill )
			{
				UseSkill(player, enemy);
				if (WaitPlayerCMD()) return;
			}
		}
	}
	if ( 0 < player.predictRound )//预知未来剩余回合
	{
		player.predictRound--;
		if ( 0 == player.predictRound ) 
		{
			player.pSkill = Skill("预知未来", m_game->SkillBook());
			if ( NULL != player.pSkill )
			{
				UseSkill(player, enemy);
				if (WaitPlayerCMD()) return;
			}
		}
	}

	if ( player.seed )//种子
	{
		m_pCurRound->log.push_back(enemy.pCurPet->nick + "吸收了" 
			+ player.pCurPet->nick + "的体力");
		int hurt = player.pCurPet->HP / 8;
		hurt = hurt > player.pCurPet->curHP?player.pCurPet->curHP:hurt;
		hurt = Hurt(player, hurt);
		enemy.pCurPet->curHP += hurt;
		if ( 0 >= player.pCurPet->curHP ) return;//挂了
	}

	if ( 0 < player.mieGe )//中了灭亡歌
	{
		player.mieGe--;
		if ( 2 == player.mieGe ) m_pCurRound->log.push_back(player.pCurPet->nick + "只剩下2回合生命");
		if ( 1 == player.mieGe ) m_pCurRound->log.push_back(player.pCurPet->nick + "只剩下1回合生命");
		if ( 0 == player.mieGe )
		{
			player.pCurPet->curHP = 0;
			Faint(player);
			return;
		}
	}

	if ( -1 != player.haQian )//中了哈欠
	{
		player.haQian--;
		if ( -1 == player.haQian )
		{
			player.pCurPet->state = Race::pu;
			player.pCurPet->sleepRound = player.rp.sleepRound;
			m_pCurRound->log.push_back(player.pCurPet->nick + "睡着了");
			player.pCurPet->sleepRound--;
		}
	}
	//锁定技能回合数量:0未锁定，>0鼓掌,-1专爱/最终手段
	if ( 0 < player.lockSkillTime )
	{
		player.lockSkillTime--;
		if ( 0 == player.lockSkillTime )
		{
			if ( player.guZhang )
			{
				player.guZhang = false;
				m_pCurRound->log.push_back(player.pCurPet->nick + "的鼓掌结束了");
			}
		}
	}
	if ( 0 < player.tiaoDou ) //挑逗剩余回合
	{
		player.tiaoDou--;
		if ( 0 == player.tiaoDou )
		{
			m_pCurRound->log.push_back(player.pCurPet->nick + "不受挑逗影响了");
		}
	}
	if ( 0 < player.pCurPet->sleepRound ) player.pCurPet->sleepRound--;//催眠剩余回合
	if ( 0 < player.pCurPet->frozenRound ) player.pCurPet->frozenRound--;//冰封剩余回合
	if ( 0 < player.hunLuan ) player.hunLuan--;//混乱剩余回合数
	if ( 0 < player.luanWu )//乱舞剩余回合
	{
		player.luanWu--;
		if ( 0 == player.luanWu )
		{
			m_pCurRound->log.push_back(player.pCurPet->nick + "停止了乱舞");
		}
	}

	if ( 0 != player.pCurPet->state && "蜕皮" == player.pTalent->name && player.rp.tePro <= 30 )
	{
		player.pCurPet->state = 0;
		m_pCurRound->log.push_back(player.pCurPet->nick + "蜕皮解除了异常");
	}
	if ( "魔法防御" != player.pTalent->name )
	{
		if ( Race::bing == m_weather 
			&& Race::bing != player.pBuddy->race1
			&& Race::bing != player.pBuddy->race2 ) 
		{
			m_pCurRound->log.push_back(player.pCurPet->nick + "受到冰雹伤害");
			Hurt(player, player.pCurPet->HP / 16);
		}
		else if ( Race::huo == m_weather && "太阳力量" == player.pTalent->name )
		{
			m_pCurRound->log.push_back(player.pCurPet->nick + "受到太阳力量反噬");
			Hurt(player, player.pCurPet->HP / 8);
		}
		else if ( Race::yan == m_weather
			&& Race::di != player.pBuddy->race1
			&& Race::di != player.pBuddy->race2
			&& Race::yan != player.pBuddy->race1
			&& Race::yan != player.pBuddy->race2
			&& Race::gang != player.pBuddy->race1
			&& Race::gang != player.pBuddy->race2
			&& "挖沙" != player.pTalent->name ) 
		{
			m_pCurRound->log.push_back(player.pCurPet->nick + "受到沙尘暴伤害");
			Hurt(player, player.pCurPet->HP / 16);
		}
		if ( 0 >= player.pCurPet->curHP ) return;//挂了

		if ( Race::huo == player.pCurPet->state )
		{
			m_pCurRound->log.push_back(player.pCurPet->nick + "被烧伤");
			Hurt(player, player.pCurPet->HP / 8);
		}
		if ( 0 >= player.pCurPet->curHP ) return;//挂了

		if ( Race::du == player.pCurPet->state
			&& "毒疗" != player.pTalent->name )
		{
			m_pCurRound->log.push_back(player.pCurPet->nick + "中毒了");
			player.duRound++;
			Hurt(player, player.pCurPet->HP * player.duRound / 8);
		}
		if ( 0 >= player.pCurPet->curHP ) return;//挂了
	}

	if ( 6 > player.sd )
	{
		if ( "加速" == player.pTalent->name )
		{
			player.sd++;
			m_pCurRound->log.push_back(player.pCurPet->nick + "的速度提升了");
		}
	}
	if ( player.pCurPet->curHP < player.pCurPet->HP )
	{
		if ( 0 < player.recvHP )//急救液
		{
			player.pCurPet->curHP += player.pCurPet->HP/16;
			m_pCurRound->log.push_back(player.pCurPet->nick + "吸收了急救液,恢复了体力");
		}
		if ( Race::shui == m_weather && "雨盘" == player.pTalent->name ) 
		{
			player.pCurPet->curHP += player.pCurPet->HP/16;
			m_pCurRound->log.push_back(player.pCurPet->nick + "喝了口雨水,恢复了体力");
		}
		if ( Race::du == player.pCurPet->state && "毒疗" == player.pTalent->name )
		{
			m_pCurRound->log.push_back(player.pCurPet->nick + "吸收了毒素,恢复了体力");
			player.pCurPet->curHP += player.pCurPet->HP / 8;
		}
		if ( "剩饭" == player.pItem->name )
		{
			player.pCurPet->curHP += player.pCurPet->HP/16;
			m_pCurRound->log.push_back(player.pCurPet->nick + "吃了口剩饭,恢复了体力");
		}
		if ( player.pCurPet->curHP > player.pCurPet->HP )
		{
			player.pCurPet->curHP = player.pCurPet->HP;
		}
	}
	if ( 0 < player.banChangeRound )//禁止换人
	{
		player.banChangeRound--;
	}

	if ( 0 < player.wall[0] )
	{
		player.wall[0]--;
		if ( 0 == player.wall[0] ) m_pCurRound->log.push_back(player.name + "的反射盾消失了");
	}
	if ( 0 < player.wall[1] )
	{
		player.wall[1]--;
		if ( 0 == player.wall[1] ) m_pCurRound->log.push_back(player.name + "的光之墙消失了");
	}
	player.fear = false;//害怕
}

Battle::WARRIOR* Battle::Player(bool me)
{
	if ( me ) return &m_player;
	return &m_enemy;
}

bool Battle::ImmuneSkill(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef)
{
	//全技能免疫
	if ( "圣斗士" == playerDef.pTalent->name )
	{
		if ( playerDef.lookSkill[playerAck.pSkill->id] ) 
		{
			m_pCurRound->log.push_back("被圣斗士看过1次的技能是无效的");
			return true;
		}
		playerDef.lookSkill[playerAck.pSkill->id] = true;
	}

	//不能被按照属性区分免疫的技能
	if ( "吼叫" == playerAck.pSkill->name //	普	0	2	101	0	最后出手，强制对方退场
		|| "吹飞" == playerAck.pSkill->name //		飞	0	2	101	0	最后出手，强制对方退场
		|| "催眠术" == playerAck.pSkill->name //		超	0	2	60	0	催眠对方
		|| "恶魔吻" == playerAck.pSkill->name //		普	0	2	75	0	催眠对方
		|| "黑洞" == playerAck.pSkill->name //		恶	0	2	80	0	催眠对方
		|| "吞噬" == playerAck.pSkill->name //	恶	0	2	101	1	禁止换人和逃跑。非战斗使用可以吞掉物品，法宝转换为正能量
		|| "黑眼" == playerAck.pSkill->name //	黑眼	普	0	2	101	0	防止对方换人和逃跑
		|| "掉包" == playerAck.pSkill->name //		恶	0	2	100	0	交换物品
		|| "哈欠" == playerAck.pSkill->name //		普	0	2	100	0	对方下回合被催眠
		|| "鼓掌" == playerAck.pSkill->name //		普	0	2	100	0	3回合不能更换技能
		|| "寻衅" == playerAck.pSkill->name //		恶	0	2	100	0	使对方不能连续使用相同技能
		|| "挑拨" == playerAck.pSkill->name //		恶	0	2	100	0	使对方3回合不能使用变化技能
		|| "封印" == playerAck.pSkill->name //		超	0	2	101	0	对方不能使用我方会的技能
		|| "同归" == playerAck.pSkill->name //		鬼	0	2	101	0	受到攻击体力归0时，拉对手一起倒下
		|| "奇怪光" == playerAck.pSkill->name //		鬼	0	2	100	0	混乱
		) return false;

	//按照属性免疫
	char skillRace = playerAck.pSkill->race;
	if ( "天空皮肤" == playerAck.pTalent->name && Race::pu == skillRace ) skillRace = Race::fei;
	else if ( "冰冻皮肤" == playerAck.pTalent->name && Race::pu == skillRace ) skillRace = Race::bing;

	if ( Race::huo == skillRace )//火系免疫
	{
		if ( "引火" == playerDef.pTalent->name )
		{
			if ( !playerDef.race[Race::huo] ) 
			{
				playerDef.race[Race::huo] = true;
				m_pCurRound->log.push_back(playerDef.pCurPet->nick + "吸收了火焰，火系威力提升");
			}
			return true;
		}
	}
	if ( Race::shui == skillRace )//水系免疫
	{
		if ( "蓄水" == playerDef.pTalent->name )
		{
			if ( playerDef.pCurPet->curHP < playerDef.pCurPet->HP ) 
			{
				playerDef.pCurPet->curHP += playerDef.pCurPet->HP / 4;
				if ( playerDef.pCurPet->HP > playerDef.pCurPet->HP ) playerDef.pCurPet->HP = playerDef.pCurPet->HP;
				m_pCurRound->log.push_back(playerDef.pCurPet->nick + "吸收了水分，回复1/4体力");
			}
			return true;
		}
	}
	if ( Race::dian == skillRace )//电系免疫
	{
		if ( "电气引擎" == playerDef.pTalent->name )
		{
			if ( 6 > playerDef.sd ) 
			{
				playerDef.sd++;
				m_pCurRound->log.push_back(playerDef.pCurPet->nick + "吸收了能量，速度提升");
			}
			return true;
		}
		if ( Race::di == playerDef.pBuddy->race1 || Race::di == playerDef.pBuddy->race2 ) return true;
	}
	if ( Race::cao == skillRace )//草系免疫
	{
		if ( "食草" == playerDef.pTalent->name )
		{
			if ( 6 > playerDef.wg ) 
			{
				playerDef.wg++;
				m_pCurRound->log.push_back(playerDef.pCurPet->nick + "吸收了攻击，物攻提升");
			}
			return true;
		}
	}
	if ( Race::du == skillRace )//毒系免疫
	{
		if ( "免疫" == playerDef.pTalent->name 
			|| Race::gang == playerDef.pBuddy->race1
			|| Race::gang == playerDef.pBuddy->race2 )
		{
			return true;
		}
	}
	if ( Race::di == skillRace )//地系免疫
	{
		if ( "浮游" == playerDef.pTalent->name 
			|| Race::fei == playerDef.pBuddy->race1
			|| Race::fei == playerDef.pBuddy->race2 )
		{
			return true;
		}
	}
	if ( Race::chao == skillRace )//超系免疫
	{
		if ( Race::e == playerDef.pBuddy->race1
			|| Race::e == playerDef.pBuddy->race2 )
		{
			return true;
		}
	}
	if ( Race::dou == skillRace )//斗系免疫
	{
		if ( Race::gui == playerDef.pBuddy->race1
			|| Race::gui == playerDef.pBuddy->race2 )
		{
			if ( "胆量" == playerAck.pTalent->name || playerAck.smell ) return false;
			return true;
		}
	}
	if ( Race::pu == skillRace )//普系免疫
	{
		if ( Race::gui == playerDef.pBuddy->race1
			|| Race::gui == playerDef.pBuddy->race2 )
		{
			if ( "胆量" == playerAck.pTalent->name || playerAck.smell ) return false;
			return true;
		}
	}
	if ( Race::gui == skillRace )//鬼系免疫
	{
		if ( Race::pu == playerDef.pBuddy->race1
			|| Race::pu == playerDef.pBuddy->race2 )
		{
			return true;
		}
	}

	return false;
}

bool Battle::Save()
{
	char name[256];
	sprintf(name, "battle %d.log", m_id);
	mdk::File logFile("D:/data/battle", name);
	if ( mdk::File::success != logFile.Open( mdk::File::write, mdk::File::assii ) ) return false;

	logFile.Write( &m_player.playerId, sizeof(unsigned int) );
	logFile.Write( m_player.name, 20 );
	SavePets( logFile, m_playerInitPets, m_game->BuddyBook() );

	logFile.Write( &m_enemy.playerId, sizeof(unsigned int) );
	logFile.Write( m_enemy.name, 20 );
	SavePets( logFile, m_enemyInitPets, m_game->BuddyBook() );

	short rCount = m_log.size();
	logFile.Write( &rCount, sizeof(short) );
	int i = 0;
	for ( i = 0; i < rCount; i++ )
	{
		WriteAction( logFile, m_log[i].me, m_log[i].meObjectId, m_log[i].meRP, m_log[i].mePetId);
		WriteAction( logFile, m_log[i].she, m_log[i].sheObjectId, m_log[i].sheRP, m_log[i].shePetId);
	}

	return true;
}

void Battle::WriteAction(mdk::File &logFile, Battle::Action act, int oId, Battle::RAND_PARAM &rp, std::vector<short> &petId)
{
	char val = act;
	logFile.Write(&val, sizeof(char));
	logFile.Write(&oId, sizeof(int));
	logFile.Write(&rp.miss, sizeof(char));//命中随机数
	logFile.Write(&rp.sePro, sizeof(char));//技能特效随机数
	logFile.Write(&rp.iePro, sizeof(char));//物品特效随机数
	logFile.Write(&rp.tePro, sizeof(char));//特性特效随机数
	logFile.Write(&rp.luanWu, sizeof(char));//乱舞回合数
	logFile.Write(&rp.sleepRound, sizeof(char));//睡眠回合随机数
	logFile.Write(&rp.frozenRound, sizeof(char));//冰冻回合随机数
	logFile.Write(&rp.dian, sizeof(char));//麻痹随机数
	val = rp.luan?1:0;
	logFile.Write(&val, sizeof(char));//混乱随机数
	logFile.Write(&rp.hurt, sizeof(unsigned char));//伤害随机数217~255
	logFile.Write(&rp.speed, sizeof(unsigned char));//速度随机数
	char count = petId.size();
	logFile.Write(&count, sizeof(char));//上场PMID数
	int i = 0;
	for ( i = 0; i < petId.size(); i++ )
	{
		logFile.Write(&petId[i], sizeof(short));
	}
}


int Battle::Load(int bid, std::string &playerName, std::string &enemyName,
	unsigned int &playerId, unsigned int &enemyId, 
	std::vector<data::PET> &mePets, std::vector<data::PET> &shePets,
	std::vector<Battle::ROUND> &log)

{
	char name[256];
	sprintf(name, "battle %d.log", bid);
	mdk::File logFile("D:/data/battle", name);
	if ( mdk::File::success != logFile.Open(mdk::File::read, mdk::File::assii) ) return -1;

	if ( mdk::File::success != logFile.Read(&playerId, sizeof(unsigned int)) ) return -2;
	if ( mdk::File::success != logFile.Read(playerName, 20) ) return -3;
	int ret = LoadPets(logFile, mePets, m_game->BuddyBook());
	if ( 0 != ret ) return ret;

	if ( mdk::File::success != logFile.Read(&enemyId, sizeof(unsigned int)) ) return -4;
	if ( mdk::File::success != logFile.Read(enemyName, 20) ) return -5;
	ret = LoadPets(logFile, shePets, m_game->BuddyBook());
	if ( 0 != ret ) return ret;

	short rCount;
	if ( mdk::File::success != logFile.Read(&rCount, sizeof(short)) ) return -6;
	int i = 0;
	Battle::ROUND  rnd;
	for ( i = 0; i < rCount; i++ )
	{
		ret = ReadAction(logFile, rnd.me, rnd.meObjectId, rnd.meRP, rnd.mePetId);
		if ( 0 != ret ) return ret;
		ret = ReadAction(logFile, rnd.she, rnd.sheObjectId, rnd.sheRP, rnd.shePetId);
		if ( 0 != ret ) return ret;
		log.push_back(rnd);
	}

	return 0;
}

int Battle::ReadAction(mdk::File &logFile, Battle::Action &act, int &oId, Battle::RAND_PARAM &rp, std::vector<short> &petId)
{
	char val;
	if ( mdk::File::success != logFile.Read(&val, sizeof(char)) ) return 1;
	act = (Action)val;
	if ( mdk::File::success != logFile.Read(&oId, sizeof(int)) ) return 2;
	if ( mdk::File::success != logFile.Read(&rp.miss, sizeof(char)) ) return 3;//命中随机数
	if ( mdk::File::success != logFile.Read(&rp.sePro, sizeof(char)) ) return 4;//技能特效随机数
	if ( mdk::File::success != logFile.Read(&rp.iePro, sizeof(char)) ) return 5;//物品特效随机数
	if ( mdk::File::success != logFile.Read(&rp.tePro, sizeof(char)) ) return 6;//特性特效随机数
	if ( mdk::File::success != logFile.Read(&rp.luanWu, sizeof(char)) ) return 7;//乱舞回合数
	if ( mdk::File::success != logFile.Read(&rp.sleepRound, sizeof(char)) ) return 8;//睡眠回合随机数
	if ( mdk::File::success != logFile.Read(&rp.frozenRound, sizeof(char)) ) return 9;//冰冻回合随机数
	if ( mdk::File::success != logFile.Read(&rp.dian, sizeof(char)) ) return 10;//麻痹随机数
	val = rp.luan?1:0;
	if ( mdk::File::success != logFile.Read(&val, sizeof(char)) ) return 11;//混乱随机数
	if ( mdk::File::success != logFile.Read(&rp.hurt, sizeof(unsigned char)) ) return 12;//伤害随机数217~255
	if ( mdk::File::success != logFile.Read(&rp.speed, sizeof(unsigned char)) ) return 13;//速度随机数
	int i = 0;
	char count = 0;
	if ( mdk::File::success != logFile.Read(&count, sizeof(char)) ) return 14;//上场PMID数
	short id;
	for ( i = 0; i < count; i++ )
	{
		if ( mdk::File::success != logFile.Read(&id, sizeof(short)) ) return 15;
		petId.push_back(id);
	}

	return 0;
}


bool Battle::Load(Game *game, int bid)
{
	std::string playerName, enemyName;
	unsigned int playerId, enemyId;
	std::vector<data::PET> mePets, shePets;
	std::vector<Battle::ROUND> log;
	if ( 0 != Load( bid, playerName, enemyName, playerId, enemyId, mePets, shePets, log ) ) return false;
	if ( !Init(game, bid, playerName, enemyName, playerId, enemyId, mePets, shePets) ) return false;
	int i = 0;
	int mePos = 0, shePos = 0;
	for ( i = 0; i < log.size(); )
	{
		if ( unknow != log[i].me ) 
		{
			Ready( true, log[i].me, log[i].meObjectId, log[i].meRP );
		}
		if ( unknow != log[i].she ) 
		{
			Ready( false, log[i].she, log[i].sheObjectId, log[i].sheRP );
		}
		while ( true )
		{
			if ( mePos < log[i].mePetId.size() && 0 >= m_player.pCurPet->curHP )
			{
				ChangePet(true, log[i].mePetId[mePos++]);
				continue;
			}
			if ( shePos < log[i].shePetId.size() && 0 >= m_enemy.pCurPet->curHP )
			{
				ChangePet(false, log[i].shePetId[shePos++]);
				continue;
			}
			break;
		}
		i++;
	}

	return true;
}

bool Battle::IsAI()
{
	return 0 == m_enemy.playerId;
}

Battle& Battle::operator = ( Battle &copy )
{
	m_game = copy.m_game;
	m_id = copy.m_id;
	m_playerInitPets = copy.m_playerInitPets;
	m_enemyInitPets = copy.m_enemyInitPets;
	m_log = copy.m_log;
	m_curRound = copy.m_curRound;
	m_pCurRound = &m_log[m_curRound];
	m_player = copy.m_player;
	m_enemy = copy.m_enemy;
	m_weather = copy.m_weather;//天气属性对应
	m_weatherCount = copy.m_weatherCount;//天气剩余回合数 -1永久
	m_foolSpace = copy.m_foolSpace;//欺骗空间剩余回合数 -1永久

	int i = 0;
	m_player.pCurPet = NULL;
	for ( i = 0; i < copy.m_player.pets.size(); i++ )
	{
		if ( copy.m_player.pCurPet == &(copy.m_player.pets[i]) )
		{
			m_player.pCurPet = &m_player.pets[i];
			break;
		}
	}

	m_enemy.pCurPet = NULL;
	for ( i = 0; i < copy.m_enemy.pets.size(); i++ )
	{
		if ( copy.m_enemy.pCurPet == &(copy.m_enemy.pets[i]) )
		{
			m_enemy.pCurPet = &m_enemy.pets[i];
			break;
		}
	}

	return *this;
}

bool Battle::AI()
{
	if ( 0 < m_enemy.pCurPet->curHP )//有战斗能力，战斗
	{
		m_enemy.rp.miss = rand()%100 + 1;//命中随机数
		m_enemy.rp.sePro = rand()%100 + 1;//技能特效随机数
		m_enemy.rp.iePro = rand()%100 + 1;//物品特效随机数
		m_enemy.rp.tePro = rand()%100 + 1;//特性特效随机数
		m_enemy.rp.luanWu = rand()%2 + 2;//乱舞回合数
		m_enemy.rp.sleepRound = rand()%7;//睡眠随机数
		m_enemy.rp.frozenRound = rand()%7;//冰冻随机数
		m_enemy.rp.dian = rand()%100 + 1;//麻痹随机数
		m_enemy.rp.luan = rand()%2;//混乱随机数
		m_enemy.rp.hurt = rand()%(255 - 217 + 1) + 217;//伤害随机数217~255
		m_enemy.rp.speed = rand()%100;//速度随机数

		TestResult act[10];
		int count = 0;
		act[count++] = TestSkill(m_enemy.pCurPet->skill1, m_enemy.rp);
		act[count++] = TestSkill(m_enemy.pCurPet->skill2, m_enemy.rp);
		act[count++] = TestSkill(m_enemy.pCurPet->skill3, m_enemy.rp);
		act[count++] = TestSkill(m_enemy.pCurPet->skill4, m_enemy.rp);
		int i = 0;
		for ( i = 0; i < m_enemy.pets.size(); i++ ) act[count++] = TestChange(i, m_enemy.rp);
		int minResult = act[0];
		int pos = 0;
		for ( i = 1; i < count; i++ )
		{
			if ( minResult > act[i] ) 
			{
				pos = i;
				minResult = act[pos];
			}
		}
		if ( refuse == minResult || m_enemy.isReady ) return true;

		if ( 0 == pos ) Ready(false, Action::attack, m_enemy.pCurPet->skill1, m_enemy.rp);
		else if ( 1 == pos ) Ready(false, Action::attack, m_enemy.pCurPet->skill2, m_enemy.rp);
		else if ( 2 == pos ) Ready(false, Action::attack, m_enemy.pCurPet->skill3, m_enemy.rp);
		else if ( 3 == pos ) Ready(false, Action::attack, m_enemy.pCurPet->skill4, m_enemy.rp);
		else Ready(false, Action::change, m_enemy.pets[pos - 4].id, m_enemy.rp);
		if ( 0 < m_enemy.pCurPet->curHP ) return true;

		return AI();
	}
	//无战斗能力，AI换人
	int i = 0;
	while ( 0 == m_enemy.pCurPet->curHP )
	{
		for ( i = 0; i < m_enemy.pets.size(); i++ ) 
		{
			if ( m_enemy.pCurPet == &m_enemy.pets[i] ) continue;
			const char *ret = ChangePet(false, m_enemy.pets[i].id);
			if ( NULL != ret ) continue;
			Save();
			break;
		}
	}
	return true;
}

Battle::TestResult Battle::TestSkill( int skillId, RAND_PARAM &rp )
{
	Battle testBattle;
	testBattle = *this;
	if ( NULL != testBattle.Ready( false, attack, skillId, rp ) ) return refuse;
	TestResult ret = ActionLevel(testBattle.m_player, testBattle.m_enemy);

	return ret;
}

Battle::TestResult Battle::TestChange( int petPos, RAND_PARAM &rp )
{
	if ( m_enemy.pCurPet == &m_enemy.pets[petPos] ) return refuse;//已出场拒绝交换

	Battle testBattle;
	testBattle = *this;
	if ( NULL != testBattle.Ready( false, change, m_enemy.pets[petPos].id, rp ) ) return refuse;
	TestResult ret = ActionLevel(testBattle.m_player, testBattle.m_enemy);
	if ( ack == ret ) return def;
	if ( badSkill == ret ) return badDef;
	return ret;
}

Battle::TestResult Battle::ActionLevel( Battle::WARRIOR &player, Battle::WARRIOR &npc )
{
	int playerHurt = player.outputHurt * 100 / npc.pCurPet->HP;
	int npcHurt = npc.outputHurt * 100 / player.pCurPet->HP;
	if ( 0 == player.pCurPet->curHP && 0 == npc.pCurPet->curHP ) return dead100;//同归
	if ( 0 == player.pCurPet->curHP )//对方挂
	{
		if ( 0 == playerHurt ) return kill0;
		if ( 30 >= playerHurt ) return kill30;
		if ( 60 >= playerHurt ) return kill60;
		if ( 100 > playerHurt ) return kill99;
	}
	if ( 0 == npc.pCurPet->curHP )//我方挂
	{
		if ( 70 <= npcHurt ) return dead70;
		if ( 50 <= npcHurt ) return dead50;
		if ( 30 <= npcHurt ) return dead30;
		return dead;
	}
	if ( npcHurt - playerHurt >= 20 )
	{
		if ( 0 == playerHurt ) return nohurt;
		if ( 30 >= playerHurt ) return hurt30;
		if ( 60 >= playerHurt ) return hurt60;
		if ( 80 > playerHurt ) return dying;
	}
	if ( 30 >= playerHurt ) return ack;

	return badSkill;
}

bool Battle::AutoRound(bool me)
{
	if ( me )
	{
		if ( 0 >= m_player.pCurPet->curHP ) return false;
		if ( (1 == m_player.pets.size() || 0 != m_player.banChangeRound )
			&& 0 != m_player.lockSkillTime && 0 != m_player.lockSkill ) return true;
	}
	else
	{
		if ( 0 >= m_enemy.pCurPet->curHP ) return false;
		if ( (1 == m_player.pets.size() || 0 != m_player.banChangeRound )
			&& 0 != m_enemy.lockSkillTime && 0 != m_enemy.lockSkill ) return true;
	}

	return false;
}
