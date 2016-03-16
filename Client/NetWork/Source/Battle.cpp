#include "Battle.h"
#include "Game.h"


Battle::Battle()
{
	m_shePlayerId = 0;
}


Battle::~Battle()
{
}

void Battle::Init(Game *game, int id, unsigned int mePlayerId, unsigned int shePlayerId, 
	std::vector<data::PET*> &me, std::vector<data::PET*> &she)
{
	m_game = game;
	m_id = id;
	m_mePlayerId = mePlayerId;
	m_shePlayerId = shePlayerId;
	int i = 0;
	for ( i = 0; i < me.size(); i++ )
	{
		m_meInit.push_back(*me[i]);
		m_me.push_back(*me[i]);
	}
	for ( i = 0; i < she.size(); i++ )
	{
		m_sheInit.push_back(*she[i]);
		m_she.push_back(*she[i]);
	}

	Battle::ROUND round;
	round.mePet = m_me[0].id;
	round.shePet = m_she[0].id;

	m_log.push_back(round);
	m_curRound = 0;
	m_meReady = m_sheReady = false;
	m_weather = 0;
	m_pMePet = NULL;
	m_meSDCorrect = 1;
	m_meWG = 0;
	m_meWF = 0;
	m_meTG = 0;
	m_meTF = 0;
	m_meSD = 0;
	m_pShePet = NULL;
	m_sheSDCorrect = 1;
	m_sheWG = 0;
	m_sheWF = 0;
	m_sheTG = 0;
	m_sheTF = 0;
	m_sheSD = 0;
	m_weatherCount = 0;
	for ( i = 0; i < 18; i++ ) m_meNail[i] = m_meRace[i] = m_sheNail[i] = m_sheRace[i] = false;
	
	m_meWall[0] = 0;
	m_meWall[1] = 0;
	m_sheWall[0] = 0;
	m_sheWall[1] = 0;
	

	return;
}

void Battle::PlayerAction(bool me, Battle::Action act, short objectId, bool skillPro, bool itemPro, int speed)
{
	if ( me )
	{
		m_pCurRound->me = act;
		m_pCurRound->meObjectId = objectId;
		m_pCurRound->meSkillPro = skillPro;
		m_pCurRound->meItemPro = itemPro;
		m_pCurRound->meSpeed = speed;
		m_meReady = true;
	}
	else
	{
		m_pCurRound->she = act;
		m_pCurRound->sheObjectId = objectId;
		m_pCurRound->sheSkillPro = skillPro;
		m_pCurRound->sheItemPro = itemPro;
		m_pCurRound->sheSpeed = speed;
		m_sheReady = true;
	}
	if ( !m_meReady || !m_sheReady ) return;
	PlayRound();
}

void Battle::PlayRound()
{
	m_pCurRound = &m_log[m_curRound];
	StepStart();
	StepChange();
	StepAttack();
	StepEnd();
	m_meReady = m_sheReady = false;
}

void Battle::StepStart()
{
}

void Battle::StepChange()
{
	int i = 0;
	bool meChanged = false;
	bool sheChanged = false;
	if ( Battle::change == m_pCurRound->me || NULL == m_pMePet ) 
	{
		if ( NULL != m_pMePet )
		{
			if ( m_pMePet->id == m_pCurRound->meObjectId ) return;
			LeaveStage(m_pMePet);
			m_pCurRound->log.push_back("收回");
			m_pMePet = NULL;
		}
		if ( Battle::change == m_pCurRound->me ) m_pCurRound->mePet = m_pCurRound->meObjectId;
		for ( i = 0; i < m_me.size(); i++ )
		{
			if ( m_pCurRound->mePet == m_me[i].id )
			{
				m_pMePet = &m_me[i];
				m_pCurRound->log.push_back("放出");
				meChanged = true;
				break;
			}
		}
		m_meSDCorrect = 1;
		m_meWG = 0;
		m_meWF = 0;
		m_meTG = 0;
		m_meTF = 0;
		m_meSD = 0;
		for ( i = 0; i < 18; i++ ) m_meRace[i] = false;
	}
	if ( Battle::change == m_pCurRound->she || NULL == m_pShePet )
	{
		if ( NULL != m_pShePet )
		{
			if ( m_pShePet->id == m_pCurRound->sheObjectId ) return;
			LeaveStage(m_pShePet);
			m_pCurRound->log.push_back("收回");
			m_pShePet = NULL;
		}
		if ( Battle::change == m_pCurRound->she ) m_pCurRound->shePet = m_pCurRound->meObjectId;
		for ( i = 0; i < m_she.size(); i++ )
		{
			if ( m_pCurRound->shePet == m_she[i].id )
			{
				m_pShePet = &m_she[i];
				m_pCurRound->log.push_back("放出");
				sheChanged = true;
				break;
			}
		}
		m_sheSDCorrect = 1;
		m_sheWG = 0;
		m_sheWF = 0;
		m_sheTG = 0;
		m_sheTF = 0;
		m_sheSD = 0;
		for ( i = 0; i < 18; i++ ) m_sheRace[i] = false;
	}
	if ( meChanged ) EntryStage(m_pMePet);
	if ( meChanged ) EntryStage(m_pShePet);
}

int Battle::CalSpeed(data::PET *pPet)
{
	int speed = 0;

	if ( pPet == m_pMePet )
	{
		if ( m_meSD > 0 ) speed = pPet->SD * m_meSDCorrect * (1 + m_meSD * 0.5);
		else if ( m_meSD < 0 ) speed = pPet->SD * m_meSDCorrect * (1 + m_meSD * 0.5 * -1);
		else speed = pPet->SD * m_meSDCorrect;
	}
	else 
	{
		if ( m_meSD > 0 ) speed = pPet->SD * m_sheSDCorrect * (1 + m_sheSD * 0.5);
		else if ( m_meSD < 0 ) speed = pPet->SD * m_sheSDCorrect * (1 + m_sheSD * 0.5 * -1);
		else speed = pPet->SD * m_sheSDCorrect;
	}

	data::TALENT *pTalent = Talent(pPet->talent, m_game->m_talentBook);
	if ( "轻快" == pTalent->name && 3 == m_weather ) speed *= 2;
	else if ( "叶绿素" == pTalent->name && 2 == m_weather ) speed *= 2; 
	else if ( "挖沙" == pTalent->name && 14 == m_weather ) speed *= 2;
	else if ( "滑雪" == pTalent->name && 5 == m_weather ) speed *= 2;
	else if ( "超级炮台" == pTalent->name ) speed *= 1.2;
	else if ( "飞毛腿" == pTalent->name && 0 != pPet->state) speed *= 1.5;

	if ( 4 == pPet->state && "飞毛腿" != pTalent->name ) speed /= 4;

	return speed;
}

int Battle::CalWG(data::PET *pPet)
{
	int WG = 0;
	if ( pPet == m_pMePet )
	{
		if ( m_meWG > 0 ) WG = pPet->WG * (1 + m_meWG * 0.5);
		else if ( m_meWG < 0 ) WG = pPet->WG / (1 + m_meWG * 0.5 * -1);
		else WG = pPet->WG;
	}
	else
	{
		if ( m_sheWG > 0 ) WG = pPet->WG * (1 + m_sheWG * 0.5);
		else if ( m_sheWG < 0 ) WG = pPet->WG / (1 + m_sheWG * 0.5 * -1);
		else WG = pPet->WG;
	}

	data::TALENT *pTalent = Talent(pPet->talent, m_game->m_talentBook);
	if ( "暴力" == pTalent->name ) WG *= 2;
	else if ( "超级炮台" == pTalent->name ) WG *= 1.2;
	else if ( "根性" == pTalent->name && 0 != pPet->state ) WG *= 1.5;
	if ( 2 == pPet->state && "根性" != pTalent->name ) WG /= 2;

	return WG;
}

int Battle::CalWF(data::PET *pPet)
{
	int WF = 0;
	if ( pPet == m_pMePet )
	{
		if ( m_meWF > 0 ) WF = pPet->WF * (1 + m_meWF * 0.5);
		else if ( m_meWF < 0 ) WF = pPet->WF / (1 + m_meWF * 0.5 * -1);
		else WF = pPet->WF;
	}
	else
	{
		if ( m_sheWF > 0 ) WF = pPet->WF * (1 + m_sheWF * 0.5);
		else if ( m_sheWF < 0 ) WF = pPet->WF / (1 + m_sheWF * 0.5 * -1);
		else WF = pPet->WF;
	}

	data::TALENT *pTalent = Talent(pPet->talent, m_game->m_talentBook);
	if ( "神秘鳞片" == pTalent->name && 0 != pPet->state ) WF *= 1.5;

	return WF;
}

int Battle::CalTG(data::PET *pPet)
{
	int TG = 0;
	if ( pPet == m_pMePet )
	{
		if ( m_meTG > 0 ) TG = pPet->TG * (1 + m_meTG * 0.5);
		else if ( m_meTG < 0 ) TG = pPet->TG / (1 + m_meTG * 0.5 * -1);
		else TG = pPet->TG;
	}
	else
	{
		if ( m_sheTG > 0 ) TG = pPet->TG * (1 + m_sheTG * 0.5);
		else if ( m_sheTG < 0 ) TG = pPet->TG / (1 + m_sheTG * 0.5 * -1);
		else TG = pPet->TG;
	}

	data::TALENT *pTalent = Talent(pPet->talent, m_game->m_talentBook);
	if ( "太阳力量" == pTalent->name && 2 == m_weather ) TG *= 1.5;

	return TG;
}

int Battle::CalTF(data::PET *pPet)
{
	int TF = 0;
	if ( pPet == m_pMePet )
	{
		if ( m_meTF > 0 ) TF = pPet->TF * (1 + m_meTF * 0.5);
		else if ( m_meTF < 0 ) TF = pPet->TF / (1 + m_meTF * 0.5 * -1);
		else TF = pPet->TF;
	}
	else
	{
		if ( m_sheTF > 0 ) TF = pPet->TF * (1 + m_sheTF * 0.5);
		else if ( m_sheTF < 0 ) TF = pPet->TF / (1 + m_sheTF * 0.5 * -1);
		else TF = pPet->TF;
	}
	return TF;
}

float RaceGood( char raceAck, char raceDef )
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
}

int Battle::CalPower(data::SKILL *pSkill, data::PET *pAckPet, data::PET *pDefPet)
{
	data::BUDDY *pAckBuddy = Buddy(pAckPet->number, m_game->BuddyBook());
	if ( NULL == pAckBuddy ) return 0;
	data::BUDDY *pDefBuddy = Buddy(pDefPet->number, m_game->BuddyBook());
	if ( NULL == pDefBuddy ) return 0;
	data::TALENT *pAckTalent = Talent(pAckPet->talent, m_game->m_talentBook);
	if ( NULL == pAckTalent ) return 0;
	data::TALENT *pDefTalent = Talent(pDefPet->talent, m_game->m_talentBook);
	if ( NULL == pDefTalent ) return 0;

	int power = pSkill->power;
	float vf = RaceGood(pSkill->race, pDefBuddy->race1);
	if ( 0 == vf )
	{
		if ( Race::pu == pSkill->race && "胆量" == pAckTalent->name ) vf = 1;
		else return 0;
	}
	power *= vf;
	vf = RaceGood(pSkill->race, pDefBuddy->race2);
	if ( 0 == vf )
	{
		if ( Race::pu == pSkill->race && "胆量" == pAckTalent->name ) vf = 1;
		else return 0;
	}
	power *= vf;
	if ( pSkill->race == pAckBuddy->race1 || pSkill->race == pAckBuddy->race2 ) power *= 1.5;

	if ( m_meRace[pSkill->race] ) power *= 1.5;
	if ( "技师" == pAckTalent->name && 60 >= pSkill->power ) power *= 1.5;
	if ( "有色眼镜" == pAckTalent->name && 2 == m_weather ) power *= 2;
	if ( "给力" == pAckTalent->name && 2 == m_weather ) power *= 1.5;
	if ( "分析" == pAckTalent->name && 2 == m_weather ) power *= 1.5;
	if ( "天空皮肤" == pAckTalent->name && 2 == m_weather ) power *= 1.5;
	if ( "冰冻皮肤" == pAckTalent->name && 2 == m_weather ) power *= 1.5;
	if ( "茂盛" == pAckTalent->name && 2 == m_weather ) power *= 1.5;
	if ( "猛火" == pAckTalent->name && 2 == m_weather ) power *= 1.5;
	if ( "激流" == pAckTalent->name && 2 == m_weather ) power *= 1.5;
	if ( "虫族报警" == pAckTalent->name && 2 == m_weather ) power *= 1.5;
	if ( "舍身" == pAckTalent->name && 2 == m_weather ) power *= 1.5;
	if ( "穿透" == pAckTalent->name && 2 == m_weather ) power *= 1.5;
	if ( "沙之力" == pAckTalent->name && 2 == m_weather ) power *= 1.5;
	if ( "黑暗气息" == pAckTalent->name && 2 == m_weather ) power *= 1.5;
		
		
		
		
		
		
	return 0;
}

bool Battle::XianShou( short itemId, bool itemPro, Battle::Action act, short skill )
{
	if ( Battle::attack != act ) return false;
	data::SKILL *pSkill = Skill(skill, m_game->SkillBook());
	if ( NULL != pSkill ) 
	{
		if ( "冰刀" == pSkill->name ) return true;
		if ( "飞叶快刀" == pSkill->name ) return true;
		if ( "闪电突袭" == pSkill->name ) return true;
		if ( "音速拳" == pSkill->name ) return true;
		if ( "暗算" == pSkill->name ) return true;
		if ( "子弹拳" == pSkill->name ) return true;
		if ( "神速" == pSkill->name ) return true;
	}
	data::ITEM *pItem = Item(itemId, m_game->ItemBook());
	if ( NULL == pItem ) return false;
	if ( "先制爪" != pItem->name ) return false;

	return itemPro;
}

void Battle::StepAttack()
{
	data::SKILL *pMeSkill = NULL;
	data::SKILL *pSheSkill = NULL;
	if ( Battle::attack == m_pCurRound->me )
	{
		pMeSkill = Skill(m_pCurRound->meObjectId, m_game->SkillBook());
		if ( "忍耐" == pMeSkill->name ) 
		{
			if ( m_pCurRound->meSkillPro ) m_pCurRound->log.push_back("忍耐");
			else m_pCurRound->log.push_back("忍耐,但是失败了");
		}
		if ( "保护" == pMeSkill->name ) 
		{
			if ( m_pCurRound->meSkillPro ) m_pCurRound->log.push_back("保护自己");
			else m_pCurRound->log.push_back("保护自己,但是失败了");
		}
	}
	if ( Battle::attack == m_pCurRound->she )
	{
		pSheSkill = Skill(m_pCurRound->sheObjectId, m_game->SkillBook());
		if ( "忍耐" == pSheSkill->name ) 
		{
			if ( m_pCurRound->sheSkillPro ) m_pCurRound->log.push_back("忍耐");
			else m_pCurRound->log.push_back("忍耐,但是失败了");
		}
		if ( "保护" == pSheSkill->name ) 
		{
			if ( m_pCurRound->sheSkillPro ) m_pCurRound->log.push_back("保护自己");
			else m_pCurRound->log.push_back("保护自己,但是失败了");
		}
	}
	bool meXianShou = XianShou(m_pMePet->itemId, m_pCurRound->meItemPro, m_pCurRound->me, m_pCurRound->meObjectId);
	bool sheXianShou = XianShou(m_pShePet->itemId, m_pCurRound->sheItemPro, m_pCurRound->she, m_pCurRound->sheObjectId);
	int meSpeed = CalSpeed(m_pMePet);
	int sheSpeed = CalSpeed(m_pShePet);
	bool meAction = false;
	if ( meXianShou == sheXianShou )
	{
		if (meSpeed == sheSpeed)
		{
			if (m_pCurRound->meSpeed == m_pCurRound->sheSpeed)
			{
				if ( m_mePlayerId > m_shePlayerId ) meAction = true;//me 
			}
			else if ( m_pCurRound->meSpeed > m_pCurRound->sheSpeed ) meAction = true;//me 
		}
		else if ( meSpeed > sheSpeed ) meAction = true;//me 
	}
	else if ( meXianShou ) meAction = true;//me 

	if ( meAction && Battle::attack == m_pCurRound->me )
	{
		int G = 0, F = 0, Power = 0;
		pMeSkill = Skill(m_pCurRound->meObjectId, m_game->SkillBook());
		if ( 1 == pMeSkill->type ) 
		{
			G = CalWG(m_pMePet);
			F = CalWF(m_pShePet);
		}
		else if ( 3 == pMeSkill->type )
		{
			G = CalTG(m_pMePet);
			if ( "精神冲击" == pMeSkill->name ) F = CalWF(m_pShePet);
			else F = CalTF(m_pShePet);
		}
		if ( 2 == pMeSkill->type ) G = 0;
		Power = CalPower(pMeSkill, m_pMePet, m_pShePet);
	}

}

void Battle::StepEnd()
{
	if ( 0 < m_weatherCount )
	{
		m_weatherCount--;
		if ( 0 == m_weatherCount )
		{
			if ( 3 == m_weather ) m_pCurRound->log.push_back("雨停了");
			else if ( 5 == m_weather ) m_pCurRound->log.push_back("雪停了");
			else if ( 2 == m_weather ) m_pCurRound->log.push_back("太阳弱了");
			else if ( 14 == m_weather ) m_pCurRound->log.push_back("沙尘暴停了");
			m_weather = 0;
		}
	}
	if ( 0 < m_meWall[0] )
	{
		m_meWall[0]--;
		if ( 0 == m_meWall[0] ) m_pCurRound->log.push_back("我方反射盾消失了");
	}
	if ( 0 < m_meWall[1] )
	{
		m_meWall[1]--;
		if ( 0 == m_meWall[1] ) m_pCurRound->log.push_back("我方反射盾消失了");
	}
	if ( 0 < m_sheWall[0] )
	{
		m_sheWall[0]--;
		if ( 0 == m_sheWall[0] ) m_pCurRound->log.push_back("对方反射盾消失了");
	}
	if ( 0 < m_sheWall[1] )
	{
		m_sheWall[1]--;
		if ( 0 == m_sheWall[1] ) m_pCurRound->log.push_back("对方反射盾消失了");
	}
}

void Battle::EntryStage(data::PET *pPet)
{
	if ( (pPet == m_pMePet && m_meNail[13]) || (pPet == m_pShePet && m_sheNail[13]))
	{
		Hurt(pPet, pPet->HP / 16);
	}
	data::TALENT *pTalent = Talent(pPet->talent, m_game->m_talentBook);
	if ( "威吓" == pTalent->name ) 
	{
		if ( pPet == m_pMePet ) m_sheWG = -1;
		else m_meWG = -1;
		m_pCurRound->log.push_back("降低了物攻");
	}
	if ( "暴雨" == pTalent->name ) 
	{
		m_weather = 3;
		m_weatherCount = -1;
		m_pCurRound->log.push_back("下雨了");
	}
	if ( "暴雪" == pTalent->name ) 
	{
		m_weather = 5;
		m_weatherCount = -1;
		m_pCurRound->log.push_back("下雪了");
	}
	if ( "干旱" == pTalent->name ) 
	{
		m_weather = 2;
		m_weatherCount = -1;
		m_pCurRound->log.push_back("太阳出来了");
	}
	if ( "扬沙" == pTalent->name ) 
	{
		m_weather = 14;
		m_weatherCount = -1;
		m_pCurRound->log.push_back("沙尘暴来了");
	}
	if ( "花之礼" == pTalent->name ) 
	{
		if ( 2 == m_weather )
		{
			if ( pPet == m_pMePet ) 
			{
				m_meTG += 1;
				m_meTF += 1;
			}
			else 
			{
				m_meTG += 1;
				m_meTF += 1;
			}
		}
	}
}

void Battle::LeaveStage(data::PET *pPet)
{
	if ( 0 >= pPet->curHP ) return;

	data::TALENT *pTalent = Talent(pPet->talent, m_game->m_talentBook);
	if ( "自然回复" == pTalent->name ) //
	{
		pPet->state = 0;
		m_pCurRound->log.push_back("恢复了正常状态");
	}
	if ( "再生" == pTalent->name ) //
	{
		pPet->curHP += pPet->HP / 3;
		m_pCurRound->log.push_back("恢复了1/3的HP");
	}
}

bool Battle::Hurt(data::PET *pPet, int HP)
{
	pPet->curHP -= HP;
	if ( pPet->curHP <= 0 )
	{
		pPet->curHP = 0;
		m_pCurRound->log.push_back("倒下了");
		return true;
	}
	if ( pPet->curHP > pPet->HP/3 ) return false;
	if ( 5 == pPet->itemId ) 
	{
		m_pCurRound->log.push_back("吃了兴奋剂-攻");
	}
	if ( 6 == pPet->itemId ) 
	{
		m_pCurRound->log.push_back("吃了兴奋剂-特攻");
	}
	if ( 7 == pPet->itemId )
	{
		if ( pPet == m_pMePet ) m_meSDCorrect = 1.5;
		else m_sheSDCorrect = 1.5;
		m_pCurRound->log.push_back("吃了兴奋剂-速度");
	}

	return false;
}
