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

bool Battle::Init(Game *game, int id, 
	const std::string &playerName, const std::string &enemyName, 
	unsigned int playerId, unsigned int enemyId, 
	std::vector<data::PET*> &me, std::vector<data::PET*> &she)
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
		m_playerInitPets.push_back(*me[i]);
		m_player.pets.push_back(*me[i]);
	}
	for ( i = 0; i < she.size(); i++ )
	{
		m_enemyInitPets.push_back(*she[i]);
		m_enemy.pets.push_back(*she[i]);
	}
	m_player.pBuddy = Buddy(m_player.pets[0].number, m_game->BuddyBook());
	if ( NULL == m_player.pBuddy ) return false;
	m_player.pTalent = Talent(m_player.pets[0].talent, m_game->TalentBook());
	if ( NULL == m_player.pTalent ) return false;
	m_enemy.pBuddy = Buddy(m_enemy.pets[0].number, m_game->BuddyBook());
	if ( NULL == m_enemy.pBuddy ) return false;
	m_enemy.pTalent = Talent(m_enemy.pets[0].talent, m_game->TalentBook());
	if ( NULL == m_enemy.pTalent ) return false;

	Battle::ROUND round;
	m_log.push_back(round);
	m_curRound = 0;
	m_pCurRound = &m_log[m_curRound];
	m_player.isReady = m_enemy.isReady = false;
	m_player.pCurPet = NULL;
	m_player.beAttacked = false;
	m_player.wgCorrect = 1;
	m_player.tgCorrect = 1;
	m_player.sdCorrect = 1;
	m_player.wg = 0;
	m_player.wf = 0;
	m_player.tg = 0;
	m_player.tf = 0;
	m_player.sd = 0;
	m_player.ct = 0;
	m_player.miss = 0;
	m_player.unMiss = 0;
	m_player.changePetAble = true;
	m_player.lockSkill = 0;
	m_player.lockSkillTime = 0;
	m_player.smell = false;
	m_player.recvHP = 0;//回复HP
	m_player.sleep = 0;//剩余回合
	m_player.seed = false;//种子
	m_player.haQian = -1;//中了哈欠
	m_player.mieGe = -1;//中了灭亡歌
	m_player.tiShen = 0;//替身生命
	m_player.xunXing = false;//中寻衅
	m_player.tiaoDou = -1;//挑逗回合
	m_player.ban = false;//中封印
	m_player.tongGui = false;//中同归
	m_player.hunLuan = -1;//混乱剩余回合数

	m_enemy.pCurPet = NULL;
	m_enemy.beAttacked = false;
	m_enemy.wgCorrect = 1;
	m_enemy.tgCorrect = 1;
	m_enemy.sdCorrect = 1;
	m_enemy.wg = 0;
	m_enemy.wf = 0;
	m_enemy.tg = 0;
	m_enemy.tf = 0;
	m_enemy.sd = 0;
	m_enemy.ct = 0;
	m_enemy.miss = 0;
	m_enemy.unMiss = 0;
	m_enemy.changePetAble = true;
	m_enemy.lockSkill = 0;
	m_enemy.lockSkillTime = 0;
	m_enemy.smell = false;
	m_enemy.recvHP = 0;//回复HP
	m_enemy.sleep = 0;//剩余回合
	m_enemy.seed = false;//种子
	m_enemy.haQian = -1;//中了哈欠
	m_enemy.mieGe = -1;//中了灭亡歌
	m_enemy.tiShen = 0;//替身生命
	m_enemy.xunXing = false;//中寻衅
	m_enemy.tiaoDou = -1;//挑逗回合
	m_enemy.ban = false;//中封印
	m_enemy.tongGui = false;//中同归
	m_enemy.hunLuan = -1;//混乱剩余回合数

	for ( i = 0; i < 18; i++ ) m_player.nail[i] = m_player.race[i] = m_enemy.nail[i] = m_enemy.race[i] = false;
	m_player.wall[0] = 0;
	m_player.wall[1] = 0;
	m_enemy.wall[0] = 0;
	m_enemy.wall[1] = 0;
	m_player.beAttacked = false;
	m_enemy.beAttacked = false;
	m_player.lookSkill.clear();
	m_enemy.lookSkill.clear();
	
	m_weather = 0;
	m_weatherCount = 0;

	return true;
}

bool Battle::PlayerRand(bool me, RAND_PARAM &rp, Battle::Action act, short objectId)
{
	Battle::WARRIOR &player = me?m_player:m_enemy;
	Battle::WARRIOR &enemy = me?m_enemy:m_player;
	if ( Battle::attack == act ) 
	{
		player.pSkill = Skill(objectId, m_game->SkillBook());
		if ( NULL == player.pSkill ) return false;
	}
	if ( Battle::useItem == act ) 
	{
		player.pItem = Item(objectId, m_game->ItemBook());
		if ( NULL == player.pItem ) return false;
	}
	if ( Battle::change == act ) 
	{
		int i = 0;
		for ( i = 0; i < player.pets.size(); i++ )
		{
			if ( objectId == player.pets[i].id )
			{
				player.pBuddy = Buddy(player.pets[i].number, m_game->BuddyBook());
				if ( NULL == player.pBuddy ) return false;
				player.pTalent = Talent(player.pets[i].talent, m_game->TalentBook());
				if ( NULL == player.pTalent ) return false;
				if ( 0 >= player.pets[i].curHP ) return false;
			}
		}
	}

	//命中
}

bool Battle::PlayerAction(bool me, Battle::Action act, short objectId, bool skillPro, bool itemPro, int speed, unsigned char randSH)
{
	if ( me )
	{
		if ( Battle::attack == act ) 
		{
			m_player.pSkill = Skill(objectId, m_game->SkillBook());
			if ( NULL == m_player.pSkill ) return false;
		}
		if ( Battle::useItem == act ) 
		{
			if ( objectId != m_player.pCurPet->itemId ) return false;
			m_player.pItem = Item(objectId, m_game->ItemBook());
			if ( NULL == m_player.pItem ) return false;
		}
		if ( Battle::change == act ) 
		{
			int i = 0;
			for ( i = 0; i < m_player.pets.size(); i++ )
			{
				if ( objectId == m_player.pets[i].id )
				{
					m_player.pBuddy = Buddy(m_player.pets[i].number, m_game->BuddyBook());
					if ( NULL == m_player.pBuddy ) return false;
					m_player.pTalent = Talent(m_player.pets[i].talent, m_game->TalentBook());
					if ( NULL == m_player.pTalent ) return false;
					if ( 0 >= m_player.pets[i].curHP ) return false;
					if ( 0 != m_player.pCurPet->itemId ) 
					{
						m_player.pItem = Item(m_player.pCurPet->itemId, m_game->ItemBook());
						if ( NULL == m_player.pItem ) return false;
					}
				}
			}
		}

		m_player.act = act;
		m_player.objId = objectId;
		m_player.skillPro = skillPro;
		m_player.itemPro = itemPro;
		m_player.sdLevel = speed;
		m_player.isReady = true;
		m_player.randSH = randSH;//rand()%(255 - 217 + 1) + 217;

		m_pCurRound->me = act;
		m_pCurRound->meObjectId = objectId;
		m_pCurRound->meSkillPro = skillPro;
		m_pCurRound->meItemPro = itemPro;
		m_pCurRound->meSpeed = speed;
		m_pCurRound->meRandSH = randSH;
	}
	else
	{
		if ( Battle::attack == act ) 
		{
			m_enemy.pSkill = Skill(objectId, m_game->SkillBook());
			if ( NULL == m_enemy.pSkill ) return false;
		}
		if ( Battle::useItem == act ) 
		{
			m_enemy.pItem = Item(objectId, m_game->ItemBook());
			if ( NULL == m_enemy.pItem ) return false;
		}
		if ( Battle::change == act ) 
		{
			int i = 0;
			for ( i = 0; i < m_enemy.pets.size(); i++ )
			{
				if ( objectId == m_enemy.pets[i].id )
				{
					m_enemy.pBuddy = Buddy(m_enemy.pets[i].number, m_game->BuddyBook());
					if ( NULL == m_enemy.pBuddy ) return false;
					m_enemy.pTalent = Talent(m_enemy.pets[i].talent, m_game->TalentBook());
					if ( NULL == m_enemy.pTalent ) return false;
					if ( 0 >= m_enemy.pets[i].curHP ) return false;
				}
			}
		}

		m_enemy.act = act;
		m_enemy.objId = objectId;
		m_enemy.skillPro = skillPro;
		m_enemy.itemPro = itemPro;
		m_enemy.sdLevel = speed;
		m_enemy.isReady = true;
		m_enemy.randSH = randSH;

		m_pCurRound->she = act;
		m_pCurRound->sheObjectId = objectId;
		m_pCurRound->sheSkillPro = skillPro;
		m_pCurRound->sheItemPro = itemPro;
		m_pCurRound->sheSpeed = speed;
		m_pCurRound->sheRandSH = randSH;
	}
	if ( !m_player.isReady || !m_enemy.isReady ) return true;

	PlayRound();

	return true;
}

void Battle::PlayRound()
{
	m_pCurRound = &m_log[m_curRound];
	StepStart();
	StepChange();
	StepAttack();
	StepEnd();
	m_player.isReady = m_enemy.isReady = false;
}

void Battle::StepStart()
{
	m_pCurRound->log.push_back("回合开始");
	m_player.beAttacked = false;
	m_enemy.beAttacked = false;
}

void Battle::StepChange()
{
	int i = 0;
	bool meChanged = false;
	bool sheChanged = false;
	meChanged = ChangePet(m_player);
	sheChanged = ChangePet(m_enemy);
	if ( meChanged ) EntryStage(m_player, m_enemy);
	if ( sheChanged ) EntryStage(m_enemy, m_player);
}

void Battle::StepAttack()
{
	if ( Battle::attack == m_player.act )
	{
		if ( "忍耐" == m_player.pSkill->name ) 
		{
			if ( m_player.skillPro ) m_pCurRound->log.push_back("忍耐");
			else m_pCurRound->log.push_back("忍耐,但是失败了");
		}
		if ( "保护" == m_player.pSkill->name ) 
		{
			if ( m_player.skillPro ) m_pCurRound->log.push_back("保护自己");
			else m_pCurRound->log.push_back("保护自己,但是失败了");
		}
	}
	if ( Battle::attack == m_enemy.act )
	{
		if ( "忍耐" == m_enemy.pSkill->name ) 
		{
			if ( m_enemy.skillPro ) m_pCurRound->log.push_back("忍耐");
			else m_pCurRound->log.push_back("忍耐,但是失败了");
		}
		if ( "保护" == m_enemy.pSkill->name ) 
		{
			if ( m_enemy.skillPro ) m_pCurRound->log.push_back("保护自己");
			else m_pCurRound->log.push_back("保护自己,但是失败了");
		}
	}

	if ( AttackOrder(m_player, m_enemy) )
	{
		if ( Battle::attack == m_player.act )
		{
			Attack(m_player, m_enemy);
		}
		if ( Battle::attack == m_enemy.act )
		{
			Attack(m_enemy, m_player);
		}
	}
	else 
	{
		if ( Battle::attack == m_enemy.act )
		{
			Attack(m_enemy, m_player);
		}
		if ( Battle::attack == m_player.act )
		{
			Attack(m_player, m_enemy);
		}
	}
}

void Battle::StepEnd()
{
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
	if ( 0 < m_player.wall[0] )
	{
		m_player.wall[0]--;
		if ( 0 == m_player.wall[0] ) m_pCurRound->log.push_back("我方反射盾消失了");
	}
	if ( 0 < m_player.wall[1] )
	{
		m_player.wall[1]--;
		if ( 0 == m_player.wall[1] ) m_pCurRound->log.push_back("我方反射盾消失了");
	}
	if ( 0 < m_enemy.wall[0] )
	{
		m_enemy.wall[0]--;
		if ( 0 == m_enemy.wall[0] ) m_pCurRound->log.push_back("对方反射盾消失了");
	}
	if ( 0 < m_enemy.wall[1] )
	{
		m_enemy.wall[1]--;
		if ( 0 == m_enemy.wall[1] ) m_pCurRound->log.push_back("对方反射盾消失了");
	}
	m_pCurRound->log.push_back("回合结束");
}

void Battle::EntryStage(Battle::WARRIOR &player, Battle::WARRIOR &enemy)
{
	if ( "威吓" == player.pTalent->name ) 
	{
		enemy.wg = -1;
		m_pCurRound->log.push_back("降低了物攻");
	}
	if ( "暴雨" == player.pTalent->name ) 
	{
		m_weather = Race::shui;
		m_weatherCount = -1;
		m_pCurRound->log.push_back("下雨了");
	}
	if ( "暴雪" == player.pTalent->name ) 
	{
		m_weather = Race::bing;
		m_weatherCount = -1;
		m_pCurRound->log.push_back("下雪了");
	}
	if ( "干旱" == player.pTalent->name ) 
	{
		m_weather = Race::huo;
		m_weatherCount = -1;
		m_pCurRound->log.push_back("太阳出来了");
	}
	if ( "扬沙" == player.pTalent->name ) 
	{
		m_weather = Race::yan;
		m_weatherCount = -1;
		m_pCurRound->log.push_back("沙尘暴来了");
	}
	if ( "花之礼" == player.pTalent->name && Race::huo == m_weather) 
	{
		player.tg += 1;
		player.tf += 1;
	}
	player.recvHP = 0;
}

void Battle::LeaveStage(Battle::WARRIOR &player)
{
	m_pCurRound->log.push_back("收回");
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

	if ( "自然回复" == player.pTalent->name ) //
	{
		player.pCurPet->state = 0;
		m_pCurRound->log.push_back("恢复了正常状态");
	}
	if ( "再生" == player.pTalent->name ) //
	{
		player.pCurPet->curHP += player.pCurPet->HP / 3;
		m_pCurRound->log.push_back("恢复了1/3的HP");
	}
	player.pCurPet = NULL;
}

bool Battle::Hurt(Battle::WARRIOR &player, int HP)
{
	player.pCurPet->curHP -= HP;
	if ( player.pCurPet->curHP <= 0 )
	{
		player.pCurPet->curHP = 0;
		m_pCurRound->log.push_back("倒下了");
		return true;
	}
	if ( player.pCurPet->curHP > player.pCurPet->HP/3 ) return false;
	if ( 5 == player.pCurPet->itemId ) 
	{
		player.wgCorrect = 1.5;
		m_pCurRound->log.push_back("吃了兴奋剂-攻");
	}
	if ( 6 == player.pCurPet->itemId ) 
	{
		player.tgCorrect = 1.5;
		m_pCurRound->log.push_back("吃了兴奋剂-特攻");
	}
	if ( 7 == player.pCurPet->itemId )
	{
		player.sdCorrect = 1.5;
		m_pCurRound->log.push_back("吃了兴奋剂-速度");
	}

	return false;
}

bool Battle::ChangePet(Battle::WARRIOR &player, int petId)
{
	if ( Battle::change != player.act && NULL != m_player.pCurPet && 0 == petId ) return false;

	int id;
	if ( 0 < petId ) id = petId;
	else if ( Battle::change == player.act ) id = player.objId;
	else id = player.pets[0].id;

	int i = 0;
	if ( NULL != player.pCurPet && player.pCurPet->id != id )
	{
		LeaveStage(player);
	}
	if ( NULL != player.pCurPet ) return false;

	for ( i = 0; i < player.pets.size(); i++ )
	{
		if ( id == player.pets[i].id )
		{
			player.pCurPet = &player.pets[i];
			m_pCurRound->log.push_back("放出");
			break;
		}
	}
	if ( NULL == player.pCurPet ) return false;

	player.wgCorrect = 1;
	player.tgCorrect = 1;
	player.sdCorrect = 1;
	player.wg = 0;
	player.wf = 0;
	player.tg = 0;
	player.tf = 0;
	player.sd = 0;
	player.ct = 0;
	player.miss = 0;
	player.unMiss = 0;
	player.beAttacked = false;
	player.lookSkill.clear();
	player.changePetAble = true;
	player.lockSkill = 0;
	player.lockSkillTime = 0;
	player.smell = false;
	player.recvHP = 0;//回复HP
	player.sleep = 0;//剩余回合
	player.seed = false;//种子
	player.haQian = -1;//中了哈欠
	player.mieGe = -1;//中了灭亡歌
	player.tiShen = 0;//替身生命
	player.xunXing = false;//中寻衅
	player.tiaoDou = -1;//挑逗回合
	player.ban = false;//中封印
	player.tongGui = false;//中同归
	player.hunLuan = -1;//混乱剩余回合数

	for ( i = 0; i < 18; i++ ) player.race[i] = false;	
	if ( player.nail[13] )
	{
		if ( Race::fei == player.pBuddy->race1
			|| Race::fei == player.pBuddy->race2 ) return true;
		if ( "魔法防御" == player.pTalent->name 
			|| "浮游" == player.pTalent->name ) return true;

		Hurt(player, player.pCurPet->HP / 16);
	}

	return true;
}

bool Battle::AttackOrder(Battle::WARRIOR &player, Battle::WARRIOR &enemy)
{
	bool meXianShou = XianShou(player);
	bool sheXianShou = XianShou(enemy);
	int meSpeed = CalSpeed(player, enemy);
	int sheSpeed = CalSpeed(enemy, player);

	if ( meXianShou == sheXianShou )
	{
		if (meSpeed == sheSpeed)
		{
			if (m_pCurRound->meSpeed == m_pCurRound->sheSpeed)
			{
				if ( player.playerId > enemy.playerId ) return true;//me 
			}
			else if ( m_pCurRound->meSpeed > m_pCurRound->sheSpeed ) return true;//me 
		}
		else if ( meSpeed > sheSpeed ) return true;//me 
	}
	else if ( meXianShou ) return true;//me 

	return false;
}

bool Battle::XianShou(Battle::WARRIOR &player)
{
	if ( Battle::attack != player.act ) return false;

	if ( "冰刀" == player.pSkill->name ) return true;
	if ( "飞叶快刀" == player.pSkill->name ) return true;
	if ( "闪电突袭" == player.pSkill->name ) return true;
	if ( "音速拳" == player.pSkill->name ) return true;
	if ( "暗算" == player.pSkill->name ) return true;
	if ( "子弹拳" == player.pSkill->name ) return true;
	if ( "神速" == player.pSkill->name ) return true;

	if ( "先制爪" != player.pItem->name ) return false;

	return player.itemPro;
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

int Battle::CalWG(Battle::WARRIOR &player, Battle::WARRIOR &enemy)
{
	int level = player.wg;
	if ( "天然" == player.pTalent->name && 0 > level ) level = 0;
	if ( "天然" == enemy.pTalent->name && 0 < level ) level = 0;
	int WG = 0;
	if ( level > 0 ) WG = player.pCurPet->WG * (1 + level * 0.5) * player.wgCorrect;
	else if ( level < 0 ) WG = player.pCurPet->WG / (1 + level * 0.5 * -1) * player.wgCorrect;
	else WG = player.pCurPet->WG * player.wgCorrect;

	if ( "暴力" == player.pTalent->name ) WG *= 2;
	else if ( "超级炮台" == player.pTalent->name ) WG *= 1.2;
	else if ( "根性" == player.pTalent->name && 0 != player.pCurPet->state ) WG *= 1.5;
	if ( 2 == player.pCurPet->state && "根性" != player.pTalent->name ) WG /= 2;

	return WG;
}

int Battle::CalWF(Battle::WARRIOR &player, Battle::WARRIOR &enemy)
{
	int level = player.wf;
	if ( "天然" == player.pTalent->name && 0 > level ) level = 0;
	if ( "天然" == enemy.pTalent->name && 0 < level ) level = 0;
	int WF = 0;
	if ( level > 0 ) WF = player.pCurPet->WF * (1 + level * 0.5);
	else if ( level < 0 ) WF = player.pCurPet->WF / (1 + level * 0.5 * -1);
	else WF = player.pCurPet->WF;

	if ( "神秘鳞片" == player.pTalent->name && 0 != player.pCurPet->state ) WF *= 1.5;

	return WF;
}

int Battle::CalTG(Battle::WARRIOR &player, Battle::WARRIOR &enemy)
{
	int level = player.tg;
	if ( "天然" == player.pTalent->name && 0 > level ) level = 0;
	if ( "天然" == enemy.pTalent->name && 0 < level ) level = 0;
	int TG = 0;
	if ( level > 0 ) TG = player.pCurPet->TG * (1 + level * 0.5) * player.tgCorrect;
	else if ( level < 0 ) TG = player.pCurPet->TG / (1 + level * 0.5 * -1) * player.tgCorrect;
	else TG = player.pCurPet->TG * player.tgCorrect;

	if ( "太阳力量" == player.pTalent->name && Race::huo == m_weather ) TG *= 1.5;

	return TG;
}

int Battle::CalTF(Battle::WARRIOR &player, Battle::WARRIOR &enemy)
{
	int level = player.tf;
	if ( "天然" == player.pTalent->name && 0 > level ) level = 0;
	if ( "天然" == enemy.pTalent->name && 0 < level ) level = 0;
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

static bool GeiLi(data::SKILL *pSkill)
{
	return false;
}

static bool SheShen(data::SKILL *pSkill)
{
	return false;
}

int Battle::CalPower(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef)
{
	int power = playerAck.pSkill->power;
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
	float vf = RaceGood(skillRace, playerDef.pBuddy->race1);
	if ( 0.4 > vf )
	{
		if ( Race::pu == skillRace 
			&& ("胆量" == playerAck.pTalent->name||playerAck.smell) ) vf = 1;
		else return 0;
	}
	vf *= RaceGood(skillRace, playerDef.pBuddy->race2);
	if ( 0.4 > vf )
	{
		if ( Race::pu == playerDef.pSkill->race 
			&& ("胆量" == playerAck.pTalent->name||playerAck.smell) ) vf = 1;
		else return 0;
	}
	power *= vf;
	if ( skillRace == playerAck.pBuddy->race1 
		|| skillRace == playerAck.pBuddy->race2 ) power *= 1.5;

	if ( playerAck.race[skillRace] ) power *= 1.5;
	if ( "技师" == playerAck.pTalent->name && 60 >= playerAck.pSkill->power ) power *= 1.5;
	else if ( "分析" == playerAck.pTalent->name && playerAck.beAttacked ) power *= 2;
	else if ( "有色眼镜" == playerAck.pTalent->name && 1.5 > vf ) power *= 2;
	else if ( "给力" == playerAck.pTalent->name && GeiLi(playerAck.pSkill) )
	{
		playerAck.skillPro = false;
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
	else if ( "舍身" == playerAck.pTalent->name && SheShen(playerAck.pSkill) ) power *= 1.2;
	if ( "沙之力" == playerAck.pTalent->name && Race::yan == m_weather
		&& (Race::di == skillRace||Race::yan == skillRace||Race::gang == skillRace) ) 
	{
		power *= 1.5;
	}
	if ( "黑暗气息" == playerAck.pTalent->name && Race::e == skillRace ) power *= 1.5;

	if ( 0 < playerDef.wall[0] && 1 == playerAck.pSkill->type 
		&& "穿透" != playerAck.pTalent->name ) power /= 2;
	if ( 0 < playerDef.wall[1] && 3 == playerAck.pSkill->type 
		&& "穿透" != playerAck.pTalent->name ) power /= 2;
	//////////////////////////////////////////////////////////////////////////
	//防御特性
	if ( "肥脂肪" == playerDef.pTalent->name 
		&& (Race::huo == skillRace || Race::bing == skillRace) ) power /= 2;
	if ( "圣斗士" == playerDef.pTalent->name ) 
	{
		if ( playerDef.lookSkill[playerAck.pSkill->id] ) power *= 0;
		playerDef.lookSkill[playerAck.pSkill->id] = true;
	}

	return power;
}

bool Battle::Attack(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef)
{
	m_pCurRound->log.push_back(playerAck.name + "使用了" + playerAck.pSkill->name);
	playerAck.lockSkill = playerAck.pSkill->id;

	int G = 0, F = 0, Power = 0;
	if ( 1 == playerAck.pSkill->type ) 
	{
		G = CalWG(playerAck, playerDef);
		F = CalWF(playerDef, playerAck);
	}
	else if ( 3 == playerAck.pSkill->type )
	{
		G = CalTG(playerAck, playerDef);
		if ( "精神冲击" == playerAck.pSkill->name ) F = CalWF(playerDef, playerAck);
		else F = CalTF(playerDef, playerAck);
	}
	if ( 2 == playerAck.pSkill->type ) 
	{
		G = 0;
		return HelpSkill(playerAck, playerDef);
	}
	Power = CalPower(playerAck, playerDef);
	if ( 0 == Power ) return false;

	int shanghai = G*Power/F*0.84*playerAck.randSH/255;
	bool ret = Hurt(playerDef, shanghai);

	return ret;
}

bool Battle::HelpSkill(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef)
{
	if ( 0 < playerAck.tiaoDou )
	{
		m_pCurRound->log.push_back("被挑逗不能使用辅助技能，失败了");
		return false;
	}
	if ( "晴天" == playerAck.pSkill->name )//		火	0	2	101	0	5回合晴天
	{
		if ( Race::huo == m_weather )
		{
			m_pCurRound->log.push_back("已经是晴天，失败了");
			return false;
		}
		m_weather = Race::huo;//天气属性对应
		m_weatherCount = 5;//天气剩余回合数 -1永久
		m_pCurRound->log.push_back("太阳出来了");
		return false;
	}
	if ( "求雨" == playerAck.pSkill->name )//		水	0	2	101	0	5回合雨天
	{
		if ( Race::shui == m_weather )
		{
			m_pCurRound->log.push_back("已经在下雨，失败了");
			return false;
		}
		m_weather = Race::shui;//天气属性对应
		m_weatherCount = 5;//天气剩余回合数 -1永久
		m_pCurRound->log.push_back("下雨了");
		return false;
	}
	if ( "冰雹" == playerAck.pSkill->name )//		冰	0	2	101	0	5回合冰雹天气，冰属性以外的巴迪每回合开始受到1/16伤害
	{
		if ( Race::bing == m_weather )
		{
			m_pCurRound->log.push_back("已经在下雪，失败了");
			return false;
		}
		m_weather = Race::bing;//天气属性对应
		m_weatherCount = 5;//天气剩余回合数 -1永久
		m_pCurRound->log.push_back("下雪了");
	}
	if ( "沙暴" == playerAck.pSkill->name )//		岩	0	2	101	0	5回合沙暴天气，地岩钢以外属性巴迪受到1/16伤害
	{
		if ( Race::yan == m_weather )
		{
			m_pCurRound->log.push_back("已经在沙尘暴中，失败了");
			return false;
		}
		m_weather = Race::yan;//天气属性对应
		m_weatherCount = 5;//天气剩余回合数 -1永久
		m_pCurRound->log.push_back("沙尘暴来了");
		return false;
	}
	if ( "光之壁" == playerAck.pSkill->name )//		超	0	2	101	0	5回合特殊伤害减半
	{
		if ( 0 < playerAck.wall[1] )
		{
			m_pCurRound->log.push_back("已经有了光之壁，失败了");
			return false;
		}
		playerAck.wall[1] = 5;
		m_pCurRound->log.push_back("我方受到光之壁保护，5回合内特殊伤害减半");
		return false;
	}
	if ( "反射盾" == playerAck.pSkill->name )//		超	0	2	101	0	5回合物理伤害减半
	{
		if ( 0 < playerAck.wall[0] )
		{
			m_pCurRound->log.push_back("已经有了反射盾，失败了");
			return false;
		}
		playerAck.wall[0] = 5;
		m_pCurRound->log.push_back("我方受到反射盾保护，5回合内物理伤害减半");
		return false;
	}
	if ( "欺骗空间" == playerAck.pSkill->name )//		超	0	2	101	0	5回合速度慢的先行动
	{
		if ( 0 != m_foolSpace )
		{
			m_pCurRound->log.push_back("已经制造欺骗空间，失败了");
			return false;
		}
		m_foolSpace = 5;
		m_pCurRound->log.push_back("场地转换为欺骗空间，5回合内速度低的先行动");
		return false;
	}
	if ( "撒菱" == playerAck.pSkill->name )//		地	0	2	101	0	使上场巴迪受到1/8伤害
	{
		if ( playerDef.nail[Race::di] )
		{
			m_pCurRound->log.push_back("不能再撒钉子了");
			return false;
		}
		playerDef.nail[Race::di] = true;
		m_pCurRound->log.push_back(playerDef.name + "场上撒满了钉子，巴迪上场时将受到最大HP的1/4的伤害");
		return false;
	}

	if ( "黑雾" == playerAck.pSkill->name )//		冰	0	2	101	0	全体能力等级复原
	{
		playerAck.wg = playerDef.wg = 0;//攻强化等级
		playerAck.wf = playerDef.wf = 0;//防强化等级
		playerAck.tg = playerDef.tg = 0;//特攻强化等级
		playerAck.tf = playerDef.tf = 0;//特防强化等级
		playerAck.sd = playerDef.sd = 0;//速度强化等级
		playerAck.ct = playerDef.ct = 0;//暴击强化等级
		m_pCurRound->log.push_back("双方能力变化消除了");
		return false;
	}
	if ( "吃撑了" == playerAck.pSkill->name )//		普	0	2	101	0	极限物攻
	{
		if ( playerAck.pCurPet->curHP < playerAck.pCurPet->HP / 2 )
		{
			m_pCurRound->log.push_back("体力不足，失败了");
			return false;
		}
		m_pCurRound->log.push_back("损失一半的体力，物理攻击提升到极限了");
		playerAck.wg = 6;
		Hurt(playerAck, playerAck.pCurPet->HP / 2);
		return false;
	}
	if ( "剑舞" == playerAck.pSkill->name )//		普	0	2	101	0	大幅强化物攻
	{
		if ( 6 == playerAck.wg )
		{
			m_pCurRound->log.push_back("物理攻击已达到极限，失败了");
			return false;
		}
		playerAck.wg += 2;
		m_pCurRound->log.push_back("物理攻击大幅上升");
		return false;
	}
	if ( "硬梆梆" == playerAck.pSkill->name )//		普 	0	2	100	0	大幅强化物防
	{
		if ( 6 == playerAck.wf )
		{
			m_pCurRound->log.push_back("物理防御已达到极限，失败了");
			return false;
		}
		if ( 6 > playerAck.wf  )
		{
			playerAck.wf += 2;
			m_pCurRound->log.push_back("物理防御大幅上升");
		}
		return false;
	}
	if ( "萤火" == playerAck.pSkill->name )//		虫	0	2	101	0	大幅强化特攻
	{
		if ( 6 == playerAck.tg )
		{
			m_pCurRound->log.push_back("特殊攻击已达到极限，失败了");
			return false;
		}
		if ( 6 > playerAck.tg  )
		{
			playerAck.tg += 2;
			m_pCurRound->log.push_back("特殊攻击大幅上升");
		}
		return false;
	}
	if ( "超级健忘" == playerAck.pSkill->name )//		超	0	2	101	0	大幅强化特防
	{
		if ( 6 == playerAck.tf )
		{
			m_pCurRound->log.push_back("特殊防御已达到极限，失败了");
			return false;
		}
		if ( 6 > playerAck.tf  )
		{
			playerAck.tf += 2;
			m_pCurRound->log.push_back("特殊防御大幅上升");
		}
		return false;
	}
	if ( "高速移动" == playerAck.pSkill->name )//		超	0	2	101	0	大幅强化速度
	{
		if ( 6 == playerAck.sd )
		{
			m_pCurRound->log.push_back("速度已达到极限，失败了");
			return false;
		}
		if ( 6 > playerAck.sd  )
		{
			playerAck.sd += 2;
			m_pCurRound->log.push_back("速度大幅上升");
		}
		return false;
	}
	if ( "巨大化" == playerAck.pSkill->name )//		斗	0	2	101	0	强化物攻，强化物防
	{
		if ( 6 == playerAck.wg && 6 == playerAck.wf )
		{
			m_pCurRound->log.push_back("物理攻击和物理防御已达到极限，失败了");
			return false;
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
		return false;
	}
	if ( "冥想" == playerAck.pSkill->name )//		超	0	2	101	0	强化特攻，强化特防
	{
		if ( 6 == playerAck.tg && 6 == playerAck.tf )
		{
			m_pCurRound->log.push_back("特殊攻击和特殊防御已达到极限，失败了");
			return false;
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
		return false;
	}
	if ( "磨爪子" == playerAck.pSkill->name )//		恶	0	2	101	0	强化物攻，提高暴击率
	{
		if ( 6 == playerAck.wg && 6 == playerAck.ct )
		{
			m_pCurRound->log.push_back("强化已达到极限，失败了");
			return false;
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
		return false;
	}
	if ( "充电" == playerAck.pSkill->name )//		电	0	2	101	0	强化特攻，电系威力*1.5
	{
		if ( 6 == playerAck.tg && playerAck.race[Race::dian] )
		{
			m_pCurRound->log.push_back("强化已达到极限，失败了");
			return false;
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
		return false;
	}
	if ( "龙之舞" == playerAck.pSkill->name )//		龙	0	2	101	0	强化物攻，强化速度
	{
		if ( 6 == playerAck.wg && 6 == playerAck.sd )
		{
			m_pCurRound->log.push_back("物理攻击和速度已达到极限，失败了");
			return false;
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
		return false;
	}
	if ( "蝶之舞" == playerAck.pSkill->name )//		虫	0	2	101	0	强化特攻，强化特防，强化速度
	{
		if ( 6 == playerAck.tg && 6 == playerAck.tf && 6 == playerAck.sd )
		{
			m_pCurRound->log.push_back("特殊攻击、特殊防御、速度已达到极限，失败了");
			return false;
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
		return false;
	}

	if ( "吃奶" == playerAck.pSkill->name //		普	0	2	101	0	回复50%体力
		|| "羽栖" == playerAck.pSkill->name ) //		飞	0	2	101	0	回复50%体力
	{
		playerAck.pCurPet->curHP += playerAck.pCurPet->HP / 2;
		m_pCurRound->log.push_back("回复了体力");
		return false;
	}
	if ( "光合作用" == playerAck.pSkill->name //		草	0	2	101	0	回复50%HP,晴天2倍，其它天气减半
		|| "晨光" == playerAck.pSkill->name //		普	0	2	101	0	回复50%HP,晴天2倍，其它天气减半
		|| "月光" == playerAck.pSkill->name )//		普	0	2	101	0	回复50%HP,晴天2倍，其它天气减半
	{
		if ( 0 == m_weather ) playerAck.pCurPet->curHP += playerAck.pCurPet->HP / 2;
		else if ( Race::huo == m_weather ) playerAck.pCurPet->curHP += playerAck.pCurPet->HP;
		else if ( Race::huo != m_weather ) playerAck.pCurPet->curHP += playerAck.pCurPet->HP / 4;
		m_pCurRound->log.push_back("回复了体力");
		return false;
	}
	if ( "清新" == playerAck.pSkill->name )//		普	0	2	101	0	解除自身异常
	{
		if ( 0 == playerAck.pCurPet->state )
		{
			m_pCurRound->log.push_back("没有异常状态,失败了");
			return false;
		}
		playerAck.pCurPet->state = 0;
		m_pCurRound->log.push_back("恢复了健康");
		return false;
	}
	if ( "治愈铃声" == playerAck.pSkill->name //		普	0	2	101	0	我方全体异常解除
	|| "芳香疗法" == playerAck.pSkill->name )//		草	0	2	101	0	全体异常解除
	{
		int i = 0;
		for ( i = 0; i < playerAck.pets.size(); i++ )
		{
			if ( playerAck.pets[i].curHP <= 0 ) continue;
			playerAck.pets[i].state = 0;
		}
		m_pCurRound->log.push_back("全队巴迪恢复了健康");
		return false;
	}

	if ( "吼叫" == playerAck.pSkill->name //	普	0	2	101	0	最后出手，强制对方退场
		|| "吹飞" == playerAck.pSkill->name )//		飞	0	2	101	0	最后出手，强制对方退场
	{
		if ( 1 == playerDef.pets.size() )
		{
			m_pCurRound->log.push_back("对方没有可替换的巴迪,失败了");
			return false;
		}
		int i = 0;
		for ( i = 0; true; i++ )
		{
			if ( i >= playerDef.pets.size() ) i = 0;
			if ( playerDef.pCurPet == &playerDef.pets[i] ) continue;
			if ( 0 == rand()%2 ) continue;
			ChangePet(playerDef, playerDef.pets[i].id);
			return false;
		}
	}
	if ( "急救液" == playerAck.pSkill->name )//		水	0	2	101	0	5回合，每回合回复1/16HP
	{
		playerAck.recvHP += playerAck.pCurPet->HP / 16;
		m_pCurRound->log.push_back("被营养液包围着，每回合回复体力");
		return false;
	}
	if ( "梦话" == playerAck.pSkill->name )//		普	0	2	101	0	睡着时，随机使用技能
	{
		int id = rand()%4 + 1;
		if ( 1 == id && playerAck.pSkill->id != playerAck.pCurPet->skill1 ) id = playerAck.pCurPet->skill1;
		else if ( 2 == id && playerAck.pSkill->id != playerAck.pCurPet->skill2 ) id = playerAck.pCurPet->skill2;
		else if ( 3 == id && playerAck.pSkill->id != playerAck.pCurPet->skill3 ) id = playerAck.pCurPet->skill3;
		else if ( 4 == id && playerAck.pSkill->id != playerAck.pCurPet->skill4 ) id = playerAck.pCurPet->skill4;
		playerAck.pSkill = Skill(id, m_game->SkillBook());
		if ( NULL == playerAck.pSkill )
		{
			m_pCurRound->log.push_back("普系技能可以击中鬼系");
			return false;
		}
		return Attack(playerAck, playerDef);
	}
	if ( "睡觉" == playerAck.pSkill->name )//		超	0	2	101	0	睡3回合，HP全满，状态解除
	{
		playerAck.pCurPet->curHP = playerAck.pCurPet->HP;
		playerAck.pCurPet->state = Race::pu;
		playerAck.sleep = 3;
		m_pCurRound->log.push_back("睡着了，回复了全盛状态");
		return false;
	}
	if ( "末日歌" == playerAck.pSkill->name )//		普	0	2	101	0	听过的巴迪，3回合内不换下，则濒死
	{
		if ( 0 < playerDef.mieGe && 0 < playerAck.mieGe )
		{
			m_pCurRound->log.push_back("没有效果");
			return false;
		}
		if ( 0 == playerAck.mieGe ) 
		{
			playerAck.mieGe = 3;
			m_pCurRound->log.push_back("只剩下2回合");
		}
		if ( 0 == playerDef.mieGe ) 
		{
			playerDef.mieGe = 3;
			m_pCurRound->log.push_back("只剩下2回合");
		}
		return false;
	}
	if ( "分享痛楚" == playerAck.pSkill->name )//		普	0	2	100	0	双方HP平分
	{
		int n = (playerAck.pCurPet->curHP + playerDef.pCurPet->curHP) / 2;
		if ( n >= playerAck.pCurPet->curHP ) playerAck.pCurPet->curHP = n;
		else Hurt(playerAck, playerAck.pCurPet->curHP - n);
		if ( n >= playerDef.pCurPet->curHP ) playerDef.pCurPet->curHP = n;
		else Hurt(playerDef, playerDef.pCurPet->curHP - n);
		m_pCurRound->log.push_back("与对方平分了体力");
		return false;
	}
	if ( "替身" == playerAck.pSkill->name )//		普	0	2	100	0	损失1/4HP制造替身
	{
		if ( 0 < playerAck.tiShen )
		{
			m_pCurRound->log.push_back("已经有替身，失败了");
			return false;
		}
		if ( playerAck.pCurPet->curHP <= playerAck.pCurPet->HP / 4 )
		{
			m_pCurRound->log.push_back("体力不足，失败了");
			return false;
		}
		m_pCurRound->log.push_back("消耗1/4体力，制造了一个替身");
		playerAck.tiShen = playerAck.pCurPet->HP / 4;
		Hurt(playerAck, playerAck.tiShen);
		return false;
	}

	if ( 0 < playerDef.tiShen )
	{
		m_pCurRound->log.push_back("替身承受了攻击");
		return false;
	}
	if ( "鬼火" == playerAck.pSkill->name )//		火	0	2	95	0	烧伤
	{
		if ( "磷粉" == playerDef.pTalent->name )
		{
			m_pCurRound->log.push_back("对方免疫了攻击");
			return false;
		}
		if ( "引火" == playerDef.pTalent->name )
		{
			playerDef.race[Race::huo] = true;
			m_pCurRound->log.push_back("对方吸收了火焰，火系威力提升");
			return false;
		}
		if ( !playerAck.skillPro )
		{
			m_pCurRound->log.push_back("攻击Miss了");
			return false;
		}
		playerDef.pCurPet->state = Race::huo;
		m_pCurRound->log.push_back("对方被烧伤");
		return false;
	}
	if ( "电磁波" == playerAck.pSkill->name )//		电	0	2	100	0	麻痹
	{
		if ( "磷粉" == playerDef.pTalent->name 
			|| "柔软" == playerDef.pTalent->name
			|| Race::di == playerDef.pBuddy->race1
			|| Race::di == playerDef.pBuddy->race2)
		{
			m_pCurRound->log.push_back("对方免疫了攻击");
			return false;
		}
		if ( "电气引擎" == playerDef.pTalent->name )
		{
			if ( 6 > playerDef.sd ) playerDef.sd++;
			m_pCurRound->log.push_back("对方吸收了能量，速度提升");
			return false;
		}
		playerDef.pCurPet->state = Race::dian;
		m_pCurRound->log.push_back("对方被麻痹");
		return false;
	}
	if ( "麻痹粉" == playerAck.pSkill->name )//		草	0	2	75	0	麻痹
	{
		if ( "磷粉" == playerDef.pTalent->name 
			|| "柔软" == playerDef.pTalent->name )
		{
			m_pCurRound->log.push_back("对方免疫了攻击");
			return false;
		}
		if ( "食草" == playerDef.pTalent->name )
		{
			if ( 6 > playerDef.wg ) playerDef.wg++;
			m_pCurRound->log.push_back("对方吸收了攻击，物攻提升");
			return false;
		}
		if ( !playerAck.skillPro )
		{
			m_pCurRound->log.push_back("攻击Miss了");
			return false;
		}
		playerDef.pCurPet->state = Race::dian;
		m_pCurRound->log.push_back("对方被麻痹");
		return false;
	}
	if ( "剧毒" == playerAck.pSkill->name )//		毒	0	2	85	0	中毒
	{
		if ( "磷粉" == playerDef.pTalent->name 
			|| "免疫" == playerDef.pTalent->name 
			|| Race::gang == playerDef.pBuddy->race1
			|| Race::gang == playerDef.pBuddy->race2 )
		{
			m_pCurRound->log.push_back("对方免疫了攻击");
			return false;
		}
		if ( !playerAck.skillPro )
		{
			m_pCurRound->log.push_back("攻击Miss了");
			return false;
		}
		playerDef.pCurPet->state = Race::du;
		m_pCurRound->log.push_back("对方中毒了");
		return false;
	}
	if ( "催眠术" == playerAck.pSkill->name //		超	0	2	60	0	催眠对方
		||"恶魔吻" == playerAck.pSkill->name //		普	0	2	75	0	催眠对方
		||"黑洞" == playerAck.pSkill->name //		恶	0	2	80	0	催眠对方
		||"睡眠粉" == playerAck.pSkill->name )//		草	0	2	100	0	催眠对方
	{
		if ( "磷粉" == playerDef.pTalent->name 
			|| "失眠" == playerDef.pTalent->name )
		{
			m_pCurRound->log.push_back("对方免疫了攻击");
			return false;
		}
		if ( "睡眠粉" == playerAck.pSkill->name && "食草" == playerDef.pTalent->name )
		{
			if ( 6 > playerDef.wg ) playerDef.wg++;
			m_pCurRound->log.push_back("对方吸收了攻击，物攻提升");
			return false;
		}
		if ( !playerAck.skillPro )
		{
			m_pCurRound->log.push_back("攻击Miss了");
			return false;
		}
		playerDef.pCurPet->state = Race::pu;
		m_pCurRound->log.push_back("对方睡着了");
		return false;
	}

	if ( "吞噬" == playerAck.pSkill->name //	恶	0	2	101	1	禁止换人和逃跑。非战斗使用可以吞掉物品，法宝转换为正能量
		|| "黑眼" == playerAck.pSkill->name )//	黑眼	普	0	2	101	0	防止对方换人和逃跑
	{
		playerDef.changePetAble = false;
		m_pCurRound->log.push_back("对方不能逃跑");
		return false;
	}
	if ( "莽撞" == playerAck.pSkill->name )//		普	0	2	100	0	对方HP减少到与自己相同
	{
		if ( Race::gui == playerDef.pBuddy->race1 )
		{
			if ( "胆量" != playerAck.pTalent->name && !playerAck.smell )
			{
				m_pCurRound->log.push_back("对方免疫了攻击");
				return false;
			}
		}
		if ( playerDef.pCurPet->curHP > playerAck.pCurPet->curHP )
		{
			Hurt(playerDef, playerDef.pCurPet->curHP - playerAck.pCurPet->curHP);
		}
		return false;
	}
	if ( "掉包" == playerAck.pSkill->name )//		恶	0	2	100	0	交换物品
	{
		if ( 0 == playerAck.pCurPet->itemId || 0== playerDef.pCurPet->itemId )
		{
			m_pCurRound->log.push_back("没有物品失败");
			return false;
		}
		int id = playerAck.pCurPet->itemId;
		playerAck.pCurPet->itemId = playerDef.pCurPet->itemId;
		playerDef.pCurPet->itemId = id;
		data::ITEM *pItem = playerAck.pItem;
		playerAck.pItem= playerDef.pItem;
		playerDef.pItem = pItem;
		m_pCurRound->log.push_back(playerAck.name + "得到了" + playerAck.pItem->name);
		m_pCurRound->log.push_back(playerDef.name + "得到了" + playerDef.pItem->name);
		return false;
	}
	if ( "香甜气息" == playerAck.pSkill->name )//		草	0	2	100	1	非战斗使用可引出野生巴迪
	{
		m_pCurRound->log.push_back("没有效果");
		return false;
	}
	if ( "嗅觉" == playerAck.pSkill->name )//		普	0	2	101	0	普和斗属性技能可打中鬼系巴迪
	{
		playerAck.smell = true;
		m_pCurRound->log.push_back("普系技能可以击中鬼系");
		return false;
	}
	if ( "寄生种子" == playerAck.pSkill->name )//		草	0	2	85	0	每回合吸收1/8体力，对方换下解除
	{
		if ( playerDef.seed || Race::cao == playerDef.pBuddy->race1 || Race::cao == playerDef.pBuddy->race2 )
		{
			m_pCurRound->log.push_back("对方免疫了攻击");
			return false;
		}
		playerDef.seed = true;
		m_pCurRound->log.push_back("对方被种下了种子");
		return false;
	}
	if ( "哈欠" == playerAck.pSkill->name )//		普	0	2	100	0	对方下回合被催眠
	{
		if ( 1 == playerDef.haQian || Race::pu == playerDef.pCurPet->state )
		{
			m_pCurRound->log.push_back("没有效果");
			return false;
		}
		playerDef.haQian = 1;
		m_pCurRound->log.push_back("对方昏昏欲睡");
		return false;
	}
	if ( "鼓掌" == playerAck.pSkill->name )//		普	0	2	100	0	3回合不能更换技能
	{
		if ( 0 == playerDef.lockSkill )
		{
			m_pCurRound->log.push_back("没有效果");
			return false;
		}
		playerDef.lockSkillTime = 3;
		m_pCurRound->log.push_back("受到鼓舞，连续3回合不能更换技能");
		return false;
	}
	if ( "寻衅" == playerAck.pSkill->name )//		恶	0	2	100	0	使对方不能连续使用相同技能
	{
		if ( playerDef.xunXing )
		{
			m_pCurRound->log.push_back("没有效果");
			return false;
		}
		playerDef.xunXing = true;
		m_pCurRound->log.push_back("不能连续使用相同技能");
		return false;
	}
	if ( "挑拨" == playerAck.pSkill->name )//		恶	0	2	100	0	使对方3回合不能使用变化技能
	{
		playerDef.tiaoDou = 3;
		m_pCurRound->log.push_back("3回合不能使用辅助技能");
		return false;
	}

	if ( "封印" == playerAck.pSkill->name )//		超	0	2	101	0	对方不能使用我方会的技能
	{
		if ( playerDef.ban )
		{
			m_pCurRound->log.push_back("对方已经被封印,失败了");
			return false;
		}
		playerDef.ban = true;
		m_pCurRound->log.push_back("不能使用我方拥有的技能");
		return false;
	}
	if ( "同归" == playerAck.pSkill->name )//		鬼	0	2	101	0	受到攻击体力归0时，拉对手一起倒下
	{
		if ( playerDef.tongGui )
		{
			m_pCurRound->log.push_back("对方已经被同归,失败了");
			return false;
		}
		playerDef.tongGui = true;
		m_pCurRound->log.push_back("牺牲时拖对手一起");
		return false;
	}
	if ( "奇怪光" == playerAck.pSkill->name )//		鬼	0	2	100	0	混乱
	{
		playerDef.pCurPet->state = Race::gui;
		playerDef.hunLuan = 3;
		m_pCurRound->log.push_back("对方混乱了");
		return false;
	}

	return false;
}
