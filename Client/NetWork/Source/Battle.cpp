#include "Battle.h"
#include "Game.h"


Battle::Battle()
{
}


Battle::~Battle()
{
}

void Battle::Init(Game *game, int id, std::vector<data::PET*> &me, std::vector<data::PET*> &she)
{
	m_game = game;
	m_id = id;
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
	m_meWG = 0;//��ǿ���ȼ�
	m_meWF = 0;//��ǿ���ȼ�
	m_meTG = 0;//�ع�ǿ���ȼ�
	m_meTF = 0;//�ط�ǿ���ȼ�
	m_meSD = 0;//�ٶ�ǿ���ȼ�
	m_pShePet = NULL;
	m_sheWG = 0;//��ǿ���ȼ�
	m_sheWF = 0;//��ǿ���ȼ�
	m_sheTG = 0;//�ع�ǿ���ȼ�
	m_sheTF = 0;//�ط�ǿ���ȼ�
	m_sheSD = 0;//�ٶ�ǿ���ȼ�
	return;
}

void Battle::PlayerAction(bool me, Battle::Action act, short objectId, bool skillPro, int speed)
{
	ROUND *pCurRound = &m_log[m_curRound];
	if ( me )
	{
		pCurRound->me = act;
		pCurRound->meObjectId = objectId;
		pCurRound->meSkillPro = skillPro;
		pCurRound->meSpeed = speed;
		m_meReady = true;
	}
	else
	{
		pCurRound->she = act;
		pCurRound->sheObjectId = objectId;
		pCurRound->sheSkillPro = skillPro;
		pCurRound->sheSpeed = speed;
		m_sheReady = true;
	}
	if ( !m_meReady || !m_sheReady ) return;
	PlayRound();
}

void Battle::PlayRound()
{
	StepStart();
	StepChange();
	StepAttack();
	StepEnd();
	m_meReady = m_sheReady = false;
}

void Battle::StepStart()
{
	ROUND *pCurRound = &m_log[m_curRound];
}

void Battle::StepChange()
{
	ROUND *pCurRound = &m_log[m_curRound];
	int i = 0;
	bool meChanged = false;
	bool sheChanged = false;
	if ( Battle::change == pCurRound->me || NULL == m_pMePet ) 
	{
		if ( NULL != m_pMePet )
		{
			if ( m_pMePet->id == pCurRound->meObjectId ) return;
			LeaveStageTalent(m_pMePet);
			m_meWG = 0;//��ǿ���ȼ�
			m_meWF = 0;//��ǿ���ȼ�
			m_meTG = 0;//�ع�ǿ���ȼ�
			m_meTF = 0;//�ط�ǿ���ȼ�
			m_meSD = 0;//�ٶ�ǿ���ȼ�
			pCurRound->log.push_back("�ջ�");
			m_pMePet = NULL;
		}
		for ( i = 0; i < m_me.size(); i++ )
		{
			if ( pCurRound->meObjectId == m_me[i].id )
			{
				m_pMePet = &m_me[i];
				pCurRound->log.push_back("�ų�");
				meChanged = true;
				break;
			}
		}
	}
	if ( Battle::change == pCurRound->she || NULL == m_pShePet )
	{
		if ( NULL != m_pShePet )
		{
			if ( m_pShePet->id == pCurRound->sheObjectId ) return;
			LeaveStageTalent(m_pShePet);
			m_sheWG = 0;//��ǿ���ȼ�
			m_sheWF = 0;//��ǿ���ȼ�
			m_sheTG = 0;//�ع�ǿ���ȼ�
			m_sheTF = 0;//�ط�ǿ���ȼ�
			m_sheSD = 0;//�ٶ�ǿ���ȼ�
			pCurRound->log.push_back("�ջ�");
			m_pShePet = NULL;
		}
		for ( i = 0; i < m_she.size(); i++ )
		{
			if ( pCurRound->sheObjectId == m_she[i].id )
			{
				m_pShePet = &m_she[i];
				pCurRound->log.push_back("�ų�");
				sheChanged = true;
				break;
			}
		}
	}
	if ( meChanged ) EntryStageTalent(m_pMePet);
	if ( meChanged ) EntryStageTalent(m_pShePet);
}

void Battle::StepAttack()
{
	ROUND *pCurRound = &m_log[m_curRound];
}

void Battle::StepEnd()
{
	ROUND *pCurRound = &m_log[m_curRound];
}

void Battle::EntryStageTalent(data::PET *pPet)
{
	ROUND *pCurRound = &m_log[m_curRound];
	data::TALENT *pTalent = Talent(pPet->talent, m_game->m_talentBook);
	int i = 0;
	for ( i = 0; i < pTalent->effects.size(); i++ )
	{
		if ( 2 != pTalent->effects[i].step ) continue;
		if ( 254 == pTalent->effects[i].id ) //����
		{
			if ( pPet == m_pMePet ) m_sheWG = -1;
			else m_meWG = -1;
			pCurRound->log.push_back("�������﹥");
		}
		if ( 259 == pTalent->effects[i].id ) //����
		{
			m_weather = 3;
			m_weatherCount = -1;
			pCurRound->log.push_back("������");
		}
		if ( 260 == pTalent->effects[i].id ) //��ѩ
		{
			m_weather = 5;
			m_weatherCount = -1;
			pCurRound->log.push_back("��ѩ��");
		}
		if ( 261 == pTalent->effects[i].id ) //�ɺ�
		{
			m_weather = 2;
			m_weatherCount = -1;
			pCurRound->log.push_back("̫��������");
		}
		if ( 262 == pTalent->effects[i].id ) //��ɳ
		{
			m_weather = 13;
			m_weatherCount = -1;
			pCurRound->log.push_back("ɳ��������");
		}
	}
}

void Battle::LeaveStageTalent(data::PET *pPet)
{
	if ( 0 >= pPet->curHP ) return;

	ROUND *pCurRound = &m_log[m_curRound];
	int i = 0;
	data::TALENT *pTalent = Talent(pPet->talent, m_game->m_talentBook);
	for ( i = 0; i < pTalent->effects.size(); i++ )
	{
		if ( 2 != pTalent->effects[i].step ) continue;
		if ( 294 == pTalent->effects[i].id ) //��Ȼ�ظ�
		{
			pPet->state = 0;
			pCurRound->log.push_back("�ָ�������״̬");
		}
		if ( 351 == pTalent->effects[i].id ) //����
		{
			pPet->curHP += pPet->HP / 3;
			pCurRound->log.push_back("�ָ���1/3��HP");
		}
	}
}
