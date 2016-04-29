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
	playerId = 0;//���Id��0npc
	pCurPet = NULL;//��ǰ���ϰ͵�
	lose = false;//δʧ��
	isChanged = false;//δ����
	pBuddy = NULL;
	pItem = NULL;
	pTalent = NULL;
	pSkill = NULL;
	pUseItem = NULL;

	outputHurt = 0;//����˺�
	doomDesireRound = -1;//����֮Ըʣ��غ�
	predictRound = -1;//Ԥ֪δ��ʣ��غ�
	attacked = false;//������
	int i = 0;
	for ( i = 0; i < 18; i++ ) nail[i] = false;//û�ж��ӣ�����idΪ�±꣬Ŀǰֻ�е����Զ���
	wall[0] = 0;
	wall[1] = 0;
}

void Battle::WARRIOR::NewRound()
{
	isReady = false;//δѡ�����
	isActioned = false;//δ�ж�
	isEnd = false;//δ��ɻغϽ�������
	attacked = false;//������
	if ( 0 != banChangeRound && 0 != lockSkillTime ) isReady = true;
}

void Battle::WARRIOR::ChangePet()
{
	isEnd = false;//δ��ɻغϽ�������
	wgCorrect = 1;//�﹥����
	tgCorrect = 1;//�ع�����
	sdCorrect = 1;//������
	wg = 0;//��ǿ���ȼ�
	wf = 0;//��ǿ���ȼ�
	tg = 0;//�ع�ǿ���ȼ�
	tf = 0;//�ط�ǿ���ȼ�
	sd = 0;//�ٶ�ǿ���ȼ�
	ct = 0;//����ǿ���ȼ�
	banChangeRound = 0;//��ֹ�����غ���,-1����
	lockSkill = 0;//��������
	lockSkillTime = ("ר��ͷ��" == pItem->name?-1:0);//�������ܻغ�����,-1����
	guZhang = false;//���˹���
	smell = false;//���
	recvHP = 0;//�ظ�HP
	seed = false;//����
	haQian = -1;//���˹�Ƿ
	mieGe = 0;//����������
	tiShen = 0;//��������
	xunXing = false;//��Ѱ��
	tiaoDou = 0;//�����غ�
	ban = false;//�з�ӡ
	tongGui = false;//��ͬ��
	hunLuan = -1;//����ʣ��غ���
	defensed = false;//���ع���
	rest = false;//��Ϣ
	luanWu = 0;//����ʣ��غ�
	sunReady = false;//��������׼����
	fear = false;//����

	int i = 0;
	for ( i = 0; i < 18; i++ ) race[i] = false;//����ǿ��
	lookSkill.clear();//ʥ��ʿ�����ļ���
	duRound = 0;//�ж��غ�
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

	Battle::ROUND round;
	m_log.push_back(round);
	m_curRound++;
	m_pCurRound = &m_log[m_curRound];
	m_pCurRound->showPos = 0;
	m_player.isChanged = ChangePet(m_player, m_player.pets[0].id);
	m_enemy.isChanged = ChangePet(m_enemy, m_enemy.pets[0].id);
	m_player.pCurPet->sleepRound = 0;//����ʣ��غ�
	m_enemy.pCurPet->frozenRound = 0;//����ʣ��غ�
	StepChange();
	m_player.NewRound();
	m_enemy.NewRound();

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
			if ( NULL == player.pBuddy ) return (reason = "�͵ϲ�����").c_str();
			player.pTalent = Talent(player.pets[i].talent, m_game->TalentBook());
			if ( NULL == player.pTalent ) return (reason = "���Բ�����").c_str();
			if ( 0 >= player.pets[i].curHP ) return (reason = "�͵���ʧȥս������").c_str();
			player.pItem = Item(player.pets[i].itemId, m_game->ItemBook());
			if ( NULL == player.pItem ) player.pItem = m_game->NullItem();
			return NULL;
		}
	}

	return (reason = "�����ڵĳ���").c_str();
}

const char* Battle::CheckReady(bool me, Battle::Action act, short objectId, Battle::RAND_PARAM &rp)
{
	static std::string reason;
	if ( IsEnd() ) return (reason = "ս������").c_str();

	Battle::WARRIOR &player = me?m_player:m_enemy;
	Battle::WARRIOR &enemy = me?m_enemy:m_player;
	if ( player.isReady ) return NULL;

	if ( Battle::attack == act ) 
	{
		if ( objectId != player.pCurPet->skill1 && objectId != player.pCurPet->skill2
			&& objectId != player.pCurPet->skill3 && objectId != player.pCurPet->skill4 ) return (reason = "����˼���").c_str();
		if ( 0 != player.lockSkill 
			&& 0 != player.lockSkillTime 
			&& objectId != player.lockSkill ) 
		{
			if (player.xunXing) 
			{
				player.pSkill = m_game->BornSkill();
				return NULL;
			}
			return (reason = "���ܸ�������").c_str();
		}
		if ( player.xunXing && objectId == player.lockSkill ) return (reason = "��Ѱ�ƣ���������ʹ����ͬ����").c_str();
		player.pSkill = Skill(objectId, m_game->SkillBook());
		if ( NULL == player.pSkill ) return (reason = "���ܲ�����").c_str();
		if ( 0 < player.tiaoDou && 2 == player.pSkill->type ) return (reason = "��������ֻ��ʹ�ù�������").c_str();
		if ( "����" == player.pSkill->name || "����" == player.pSkill->name )
		{
			if ( player.defensed ) player.defensed = rand()%2;
			else player.defensed = true;
		}
		else player.defensed = false;
	}
	if ( Battle::useItem == act ) 
	{
		player.pUseItem = Item(objectId, m_game->ItemBook());
		if ( NULL == player.pUseItem ) return (reason = "��Ʒ������").c_str();
		player.pSkill = NULL;
	}
	if ( Battle::change == act ) 
	{
		if ( 0 != player.banChangeRound ) return (reason = "���ܸ����͵�").c_str();
		if ( NULL != player.pCurPet && player.pCurPet->id == objectId )
		{
			return (reason = "�͵��ѳ���").c_str();
		}
		const char *ret = SetPetInfo(player, objectId);
		if ( NULL != ret ) return ret;
	}

	rp.miss = rand()%100 + 1;//���������
	rp.sePro = rand()%100 + 1;//������Ч�����
	rp.iePro = rand()%100 + 1;//��Ʒ��Ч�����
	rp.tePro = rand()%100 + 1;//������Ч�����
	rp.luanWu = rand()%2 + 2;//����غ���
	rp.sleepRound = rand()%7;//˯�������
	rp.frozenRound = rand()%7;//���������
	rp.dian = rand()%100 + 1;//��������
	rp.luan = rand()%2;//���������
	rp.hurt = rand()%(255 - 217 + 1) + 217;//�˺������217~255
	rp.speed = rand()%100;//�ٶ������

	return NULL;
}

bool Battle::Ready(bool me, Battle::Action act, short objectId, Battle::RAND_PARAM &rp)
{
	if ( IsEnd() ) return false;

	Battle::WARRIOR &player = me?m_player:m_enemy;
	Battle::WARRIOR &enemy = me?m_enemy:m_player;
	if ( player.isReady ) return false;

	if ( Battle::attack == act ) 
	{
		if ( 0 != player.lockSkill && 0 != player.lockSkillTime ) 
		{
			if (player.xunXing)
			{
				player.pSkill = m_game->BornSkill();
			}
		}
		else player.pSkill = Skill(objectId, m_game->SkillBook());
		if ( NULL == player.pSkill ) return false;
	}
	if ( Battle::useItem == act )
	{
		player.defensed = false;
		player.rest = false;
		player.pSkill = NULL;
		player.pUseItem = Item(objectId, m_game->ItemBook());
		if ( NULL == player.pUseItem ) return false;
	}
	if ( Battle::change == act ) 
	{
		if ( NULL != player.pCurPet && player.pCurPet->id == objectId )
		{
			return false;
		}
		if ( NULL != SetPetInfo(player, objectId) ) return false;
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
	if ( !m_player.isReady || !m_enemy.isReady ) return false;

	StepStart();
	if ( Battle::change == m_player.act )
	{
		m_player.isChanged = ChangePet(m_player, m_player.objId);
	}
	if ( Battle::change == m_enemy.act )
	{
		m_enemy.isChanged = ChangePet(m_enemy, m_enemy.objId);
	}
	PlayRound();
	End();
	return true;
}

const char* Battle::ChangePet(bool me, short petId)
{
	static std::string reason;
	Battle::WARRIOR &player = me?m_player:m_enemy;
	Battle::WARRIOR &enemy = me?m_enemy:m_player;
	if ( NULL != player.pCurPet && player.pCurPet->id == petId )
	{
		return (reason = "�͵��ѳ���").c_str();
	}
	if ( me ) m_pCurRound->mePetId.push_back(petId);
	else m_pCurRound->shePetId.push_back(petId);

	const char *ret = SetPetInfo(player, petId);
	if ( NULL != ret ) return ret;
	player.isChanged = ChangePet(player, petId);
	PlayRound();
	End();

	return NULL;
}

bool Battle::PlayRound()
{
	if ( WaitPlayerCMD() || m_player.lose || m_enemy.lose ) return false;

	m_pCurRound->log.push_back("\n");
	StepChange();
	if ( !StepAttack() ) return false;
	if ( !StepEnd() ) return false;
	m_player.NewRound();
	m_enemy.NewRound();

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
		sprintf( result, "ƽ��" );
	}
	else if ( m_player.lose ) 
	{
		sprintf( result, "%d:%d %sʤ", 
			playerCount, enemyCount, m_enemy.name.c_str() );
	}
	else 
	{
		sprintf( result, "%d:%d %sʤ", 
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
	for ( ; m_pCurRound->showPos < m_pCurRound->log.size(); m_pCurRound->showPos++ )
	{
		log.push_back(m_pCurRound->log[m_pCurRound->showPos]);
	}
	if ( 0 >= log.size() ) return false;

	return true;
}

void Battle::StepStart()
{
	Battle::ROUND round;
	m_log.push_back(round);
	m_curRound++;
	m_pCurRound = &m_log[m_curRound];

	char log[256];
	sprintf( log, "�غ�%d��ʼ", m_curRound );
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
		&& ( "����ȭ" == m_player.pSkill->name || "�㱼����ȭ" == m_player.pSkill->name ) )
	{
		if ( "�㱼����ȭ" == m_player.pSkill->name ) 
		{
			m_player.tiShen = 0;
			m_pCurRound->log.push_back( m_player.pCurPet->nick + "����������" );
		}
		m_pCurRound->log.push_back( m_player.pCurPet->nick + "��ʼ����" );
	}
	if ( Battle::attack == m_enemy.act && ActionAble(m_enemy)
		&& ( "����ȭ" == m_enemy.pSkill->name || "�㱼����ȭ" == m_enemy.pSkill->name ) )
	{
		if ( "�㱼����ȭ" == m_enemy.pSkill->name ) 
		{
			m_enemy.tiShen = 0;
			m_pCurRound->log.push_back( m_enemy.pCurPet->nick + "����������" );
		}
		m_pCurRound->log.push_back( m_enemy.pCurPet->nick + "��ʼ����" );
	}

	if ( AttackOrder(m_player, m_enemy) )
	{
		if ( Battle::attack == m_player.act && !m_player.isActioned )
		{
			PetAction(m_player, m_enemy);
			if ( WaitPlayerCMD() || m_player.lose || m_enemy.lose ) return false;
		}
		if ( Battle::attack == m_enemy.act && !m_enemy.isActioned )
		{
			PetAction(m_enemy, m_player);
			if ( WaitPlayerCMD() || m_player.lose || m_enemy.lose ) return false;
		}
	}
	else 
	{
		if ( Battle::attack == m_enemy.act && !m_enemy.isActioned )
		{
			PetAction(m_enemy, m_player);
			if ( WaitPlayerCMD() || m_player.lose || m_enemy.lose ) return false;
		}
		if ( Battle::attack == m_player.act && !m_player.isActioned )
		{
			PetAction(m_player, m_enemy);
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
	if ( 0 >= m_player.pCurPet->curHP ) return false; 

	if ( 0 < m_weatherCount )
	{
		m_weatherCount--;
		if ( 0 == m_weatherCount )
		{
			if ( Race::shui == m_weather ) m_pCurRound->log.push_back("��ͣ��");
			else if ( Race::bing == m_weather ) m_pCurRound->log.push_back("ѩͣ��");
			else if ( Race::huo == m_weather ) m_pCurRound->log.push_back("̫������");
			else if ( Race::yan == m_weather ) m_pCurRound->log.push_back("ɳ����ͣ��");
			m_weather = 0;
		}
	}

	m_pCurRound->log.push_back("\n");
	char log[256];
	sprintf( log, "�غ�%d����", m_curRound );
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

	if ( "����" == player.pTalent->name ) 
	{
		if ( "����" == enemy.pTalent->name )
		{
			m_pCurRound->log.push_back( enemy.pCurPet->nick + "�ľ����ֹ�������½�");
		}
		else
		{
			if ( -6 == enemy.wg ) 
			{
				m_pCurRound->log.push_back(enemy.pCurPet->nick + "���﹥�Ѿ���������");
				return;
			}
			enemy.wg--;
			m_pCurRound->log.push_back(player.pCurPet->nick + "�����Ž�����" + enemy.pCurPet->nick + "���﹥");
		}
	}
	else if ( "����" == player.pTalent->name ) 
	{
		m_weather = Race::shui;
		m_weatherCount = -1;
		m_pCurRound->log.push_back("������");
	}
	else if ( "��ѩ" == player.pTalent->name ) 
	{
		m_weather = Race::bing;
		m_weatherCount = -1;
		m_pCurRound->log.push_back("��ѩ��");
	}
	else if ( "�ɺ�" == player.pTalent->name ) 
	{
		m_weather = Race::huo;
		m_weatherCount = -1;
		m_pCurRound->log.push_back("̫��������");
	}
	else if ( "��ɳ" == player.pTalent->name ) 
	{
		m_weather = Race::yan;
		m_weatherCount = -1;
		m_pCurRound->log.push_back("ɳ��������");
	}
	else if ( "��֮��" == player.pTalent->name && Race::huo == m_weather) 
	{
		player.tg += 1;
		player.tf += 1;
	}
	else if ( "ҧ��" == player.pTalent->name && "����" != enemy.pTalent->name ) 
	{
		m_pCurRound->log.push_back(player.pCurPet->nick + "ҧס��" 
			+ enemy.pCurPet->nick + "�������ܣ��ٶȼ����½��������������⹥����������" );
		enemy.banChangeRound = -1;
		enemy.wgCorrect *= 2;
		enemy.tgCorrect *= 2;
		enemy.sdCorrect /= 2;
	}
}

void Battle::LeaveStage(Battle::WARRIOR &player)
{
	m_pCurRound->log.push_back(player.name + "�ջ�" + player.pCurPet->nick);
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
	if ( "��Ȼ�ָ�" == pTalent->name ) //
	{
		player.pCurPet->state = 0;
		m_pCurRound->log.push_back("�ָ�������״̬");
	}
	if ( "����" == pTalent->name ) //
	{
		player.pCurPet->curHP += player.pCurPet->HP / 3;
		m_pCurRound->log.push_back("�ָ���1/3��HP");
	}
	player.pCurPet = NULL;
}

int Battle::Hurt(Battle::WARRIOR &player, int HP, bool unFaint)
{
	HP = HP <= player.pCurPet->curHP?HP:player.pCurPet->curHP;
	char strHurt[256];
	sprintf( strHurt, "%s�ܵ�%d%%(%d��)�˺�", player.pCurPet->nick.c_str(), 
		HP*100/player.pCurPet->HP, HP );
	m_pCurRound->log.push_back(strHurt);
	player.pCurPet->curHP -= HP;

	if ( unFaint && player.pCurPet->curHP <= 0 )
	{
		player.pCurPet->curHP = 1;
		m_pCurRound->log.push_back(player.pCurPet->nick + "����ס���˺�");
	}
	if ( Faint(player) ) return HP;

	if ( player.pCurPet->curHP > player.pCurPet->HP/3 ) return HP;
	if ( 5 == player.pCurPet->itemId ) 
	{
		player.wgCorrect *= 1.5;
		player.pCurPet->itemId = 0;
		m_pCurRound->log.push_back(player.pCurPet->nick + "�����˷ܼ�-��");
	}
	if ( 6 == player.pCurPet->itemId ) 
	{
		player.tgCorrect = 1.5;
		player.pCurPet->itemId = 0;
		m_pCurRound->log.push_back(player.pCurPet->nick + "�����˷ܼ�-�ع�");
	}
	if ( 7 == player.pCurPet->itemId )
	{
		player.sdCorrect = 1.5;
		player.pCurPet->itemId = 0;
		m_pCurRound->log.push_back(player.pCurPet->nick + "�����˷ܼ�-�ٶ�");
	}

	return HP;
}

bool Battle::ChangePet(Battle::WARRIOR &player, int petId)
{
	player.isActioned = true;//�Ѿ��ж�

	int i = 0;
	if ( NULL != player.pCurPet && player.pCurPet->id != petId )
	{
		LeaveStage(player);
	}
	if ( NULL != player.pCurPet ) 
	{
		m_pCurRound->log.push_back("�ų��ѳ����İ͵� ");
		return false;
	}

	for ( i = 0; i < player.pets.size(); i++ )
	{
		if ( petId == player.pets[i].id )
		{
			player.pCurPet = &player.pets[i];
			m_pCurRound->log.push_back(player.name + "�ų�" + player.pBuddy->name);
			break;
		}
	}
	if ( NULL == player.pCurPet ) 
	{
		m_pCurRound->log.push_back("ָ�������ڵİ͵ϳ���");
		return false;
	}
	player.ChangePet();

	if ( player.nail[Race::di] )
	{
		if ( Race::fei != player.pBuddy->race1 
			&& Race::fei != player.pBuddy->race2 
			&& "ħ������" != player.pTalent->name 
			&& "����" != player.pTalent->name ) 
		{
			Hurt( player, player.pCurPet->HP / 4 );
			if ( 0 >= player.pCurPet->curHP ) return false;//�����͹���
		}
	}
	if ( "����" == player.pItem->name )
	{
		player.pCurPet->state = Race::du;
		player.duRound = 0;
		m_pCurRound->log.push_back( m_player.pCurPet->nick + "Я�������ж���" );
	}
	else if ( "����" == player.pItem->name )
	{
		player.pCurPet->state = Race::huo;
		m_pCurRound->log.push_back( m_player.pCurPet->nick + "Я������������" );
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
		else if ( meSpeed > sheSpeed ) return true;//me 
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
		else if ( meSpeed > sheSpeed ) return true;//me 
	}
	else if ( meXianShou ) return true;//me 

	return false;
}

bool Battle::IsUnwait(Battle::WARRIOR &player)
{
	if ( Battle::attack != player.act ) return false;

	if ( "����" == player.pSkill->name ) return true;
	if ( "��Ҷ�쵶" == player.pSkill->name ) return true;
	if ( "����ͻϮ" == player.pSkill->name ) return true;
	if ( "����ȭ" == player.pSkill->name ) return true;
	if ( "����" == player.pSkill->name ) return true;
	if ( "�ӵ�ȭ" == player.pSkill->name ) return true;
	if ( "����" == player.pSkill->name ) return true;
	if ( "����צ" == player.pItem->name && player.rp.iePro < 21 ) return true;
	if ( "����֮��" == player.pTalent->name && Race::fei == player.pSkill->race ) return true;

	return false;
}

bool Battle::IsWait(Battle::WARRIOR &player)
{
	if ( Battle::attack != player.act ) return false;
	if ( "Ʈ����ѩ���㴵" == player.pSkill->name ) return true;
	if ( "����" == player.pSkill->name ) return true;
	if ( "���" == player.pSkill->name ) return true;
	if ( "����ȭ" == player.pSkill->name ) return true;
	if ( "�㱼����ȭ" == player.pSkill->name ) return true;

	return false;
}

int Battle::CalSpeed(Battle::WARRIOR &player, Battle::WARRIOR &enemy)
{
	int level = player.sd;
	if ( "��Ȼ" == player.pTalent->name && 0 > level ) level = 0;
	if ( "��Ȼ" == enemy.pTalent->name && 0 < level ) level = 0;
	int speed = 0;
	if ( level > 0 ) speed = player.pCurPet->SD * (1 + level * 0.5) * player.sdCorrect;
	else if ( level < 0 ) speed = player.pCurPet->SD * (1 + level * 0.5 * -1) * player.sdCorrect;
	else speed = player.pCurPet->SD * player.sdCorrect;

	if ( "���" == player.pTalent->name && Race::shui == m_weather ) speed *= 2;
	else if ( "Ҷ����" == player.pTalent->name && Race::huo == m_weather ) speed *= 2; 
	else if ( "��ɳ" == player.pTalent->name && Race::yan == m_weather ) speed *= 2;
	else if ( "��ѩ" == player.pTalent->name && Race::bing == m_weather ) speed *= 2;
	else if ( "������̨" == player.pTalent->name ) speed *= 1.2;
	else if ( "��ë��" == player.pTalent->name && 0 != player.pCurPet->state) speed *= 1.5;

	if ( 4 == player.pCurPet->state && "��ë��" != player.pTalent->name ) speed /= 4;

	return speed;
}

bool Battle::CalHitRate(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef)
{
	if ( 100 < playerAck.pSkill->hitRate
		|| "����Ŀ��" == playerAck.pTalent->name
		|| "�Ŵ�" == playerAck.pItem->name ) return true;

	float hr = playerAck.pSkill->hitRate;
	if ( "ѹ��" == playerDef.pTalent->name ) hr *= 0.85;
	if ( "ɳ��" == playerDef.pTalent->name && Race::yan == m_weather ) hr *= 0.7;
	if ( "ѩ��" == playerDef.pTalent->name && Race::bing == m_weather ) hr *= 0.7;
	if ( "���" == playerDef.pItem->name ) hr *= 0.8;
	if ( "����" == playerAck.pTalent->name ) hr *= 1.3;
	if ( playerAck.rp.miss > hr) return false;

	return true;
}

bool Battle::CriticalHit(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef)
{
	if ( "����" == playerDef.pTalent->name ) return false;

	int i = 0;
	float probability = 0;
	for ( i = 0; i < playerAck.pSkill->effects.size(); i++ )
	{
		if ( 88 != playerAck.pSkill->effects[i].id ) continue;//CT
		probability = playerAck.pSkill->effects[i].probability;
		break;
	}
	if ( "��Ҷ��" == playerAck.pItem->name ) probability += 15;
	if ( "ǿ��" == playerAck.pTalent->name ) probability += 15;
	probability += playerAck.ct * 10;
	if ( playerAck.rp.sePro < probability ) return true;

	return false;
}

int Battle::CalWG(Battle::WARRIOR &player, bool ct, Battle::WARRIOR &enemy)
{
	int level = player.wg;
	if ( "��Ȼ" == player.pTalent->name && 0 > level ) level = 0;
	if ( "��Ȼ" == enemy.pTalent->name && 0 < level ) level = 0;
	int WG = 0;
	if ( level > 0 ) WG = player.pCurPet->WG * (1 + level * 0.5) * player.wgCorrect;
	else if ( level < 0 && !ct ) WG = player.pCurPet->WG / (1 + level * 0.5 * -1) * player.wgCorrect;
	else WG = player.pCurPet->WG * player.wgCorrect;
	if ( "ר��ͷ��" == player.pItem->name ) WG *= 1.5;

	if ( "����" == player.pTalent->name ) WG *= 2;
	else if ( "������̨" == player.pTalent->name ) WG *= 1.2;
	else if ( "����" == player.pTalent->name && 0 != player.pCurPet->state ) WG *= 1.5;
	if ( 2 == player.pCurPet->state && "����" != player.pTalent->name ) WG /= 2;

	return WG;
}

int Battle::CalWF(Battle::WARRIOR &player, bool ct, Battle::WARRIOR &enemy)
{
	int level = player.wf;
	if ( "��Ȼ" == player.pTalent->name && 0 > level ) level = 0;
	if ( "��Ȼ" == enemy.pTalent->name && 0 < level ) level = 0;
	if ( level > 0 && ct ) level = 0;
	int WF = 0;
	if ( level > 0 ) WF = player.pCurPet->WF * (1 + level * 0.5);
	else if ( level < 0 ) WF = player.pCurPet->WF / (1 + level * 0.5 * -1);
	else WF = player.pCurPet->WF;

	if ( "������Ƭ" == player.pTalent->name && 0 != player.pCurPet->state ) WF *= 1.5;

	return WF;
}

int Battle::CalTG(Battle::WARRIOR &player, bool ct, Battle::WARRIOR &enemy)
{
	int level = player.tg;
	if ( "��Ȼ" == player.pTalent->name && 0 > level ) level = 0;
	if ( "��Ȼ" == enemy.pTalent->name && 0 < level ) level = 0;
	int TG = 0;
	if ( level > 0 ) TG = player.pCurPet->TG * (1 + level * 0.5) * player.tgCorrect;
	else if ( level < 0 && !ct ) TG = player.pCurPet->TG / (1 + level * 0.5 * -1) * player.tgCorrect;
	else TG = player.pCurPet->TG * player.tgCorrect;

	if ( "̫������" == player.pTalent->name && Race::huo == m_weather ) TG *= 1.5;

	return TG;
}

int Battle::CalTF(Battle::WARRIOR &player, bool ct, Battle::WARRIOR &enemy)
{
	int level = player.tf;
	if ( "��Ȼ" == player.pTalent->name && 0 > level ) level = 0;
	if ( "��Ȼ" == enemy.pTalent->name && 0 < level ) level = 0;
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
	if ( "����" == pSkill->name 
		|| "ȼ�մ���" == pSkill->name 
		|| "�����ƽ�" == pSkill->name 
		|| "���ֱ�" == pSkill->name 
		|| "����ȭ" == pSkill->name 
		|| "ѩ��" == pSkill->name 
		|| "����ѩ" == pSkill->name 
		|| "��������" == pSkill->name 
		|| "����ȭ" == pSkill->name 
		|| "��ˮ" == pSkill->name 
		|| "Ծ����" == pSkill->name 
		|| "����" == pSkill->name 
		|| "����" == pSkill->name 
		|| "ʮ�����" == pSkill->name 
		|| "�׹�ȭ" == pSkill->name 
		|| "�����и�" == pSkill->name 
		|| "������ը" == pSkill->name 
		|| "Ҷ��ն" == pSkill->name 
		|| "��Դ��" == pSkill->name 
		|| "Ʈ����ѩ���㴵" == pSkill->name 
		|| "��צ" == pSkill->name 
		|| "Ӱ����" == pSkill->name 
		|| "�Ե�" == pSkill->name 
		|| "�����" == pSkill->name 
		|| "ҧ��" == pSkill->name 
		|| "�����и�" == pSkill->name 
		|| "����ѹ��" == pSkill->name 
		|| "�������" == pSkill->name 
		|| "������" == pSkill->name 
		|| "����ȭ" == pSkill->name 
		|| "��դ��ũ" == pSkill->name 
		|| "ʮ�ּ�" == pSkill->name 
		|| "����" == pSkill->name 
		|| "�ұ�" == pSkill->name 
		|| "ԭʼ����" == pSkill->name 
		|| "����" == pSkill->name 
		|| "���֮��" == pSkill->name 
		|| "��ͻ" == pSkill->name 
		|| "���౬��" == pSkill->name ) return true;

	return false;
}

static bool BiteSkill(data::SKILL *pSkill)
{
	if ( "ľ�" == pSkill->name
		|| "����ͻϮ" == pSkill->name
		|| "����" == pSkill->name
		|| "�����ƽ�" == pSkill->name
		|| "����ײ" == pSkill->name
		) return true;

	return false;
}

int Battle::CalPower(Battle::WARRIOR &playerAck, bool ct, Battle::WARRIOR &playerDef)
{
	int power = playerAck.pSkill->power;
	if ( "����ȭ" == playerAck.pSkill->name 
		|| "��������" == playerAck.pSkill->name
		|| "�ռ����" == playerAck.pSkill->name
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
	if ( "���Ƥ��" == playerAck.pTalent->name && Race::pu == skillRace ) 
	{
		skillRace = Race::fei;
		power *= 1.3;
	}
	else if ( "����Ƥ��" == playerAck.pTalent->name && Race::pu == skillRace )
	{
		skillRace = Race::bing;
		power *= 1.3;
	}
	float vf = RaceGood(skillRace, playerDef.pBuddy->race1);
	if ( 0.4 > vf )
	{
		if ( Race::pu == skillRace 
			&& ("����" == playerAck.pTalent->name||playerAck.smell) ) vf = 1;
		else return 0;
	}
	vf *= RaceGood(skillRace, playerDef.pBuddy->race2);
	if ( 0.4 > vf )
	{
		if ( Race::pu == skillRace 
			&& ("����" == playerAck.pTalent->name||playerAck.smell) ) vf = 1;
		else return 0;
	}
	power *= vf;
	if ( skillRace == playerAck.pBuddy->race1 
		|| skillRace == playerAck.pBuddy->race2 ) power *= 1.5;

	if ( playerAck.race[skillRace] ) power *= 1.5;
	if ( "��ʦ" == playerAck.pTalent->name && 60 >= playerAck.pSkill->power ) power *= 1.5;
	else if ( "����" == playerAck.pTalent->name && playerDef.isActioned ) power *= 2;
	else if ( "��ɫ�۾�" == playerAck.pTalent->name && 1.5 > vf ) power *= 2;
	else if ( "����" == playerAck.pTalent->name && MagicSkill(playerAck.pSkill) )
	{
		playerAck.rp.sePro = 101;
		power *= 1.3;
	}
	else if ( "ïʢ" == playerAck.pTalent->name 
		&& Race::cao == skillRace
		&& playerAck.pCurPet->curHP < playerAck.pCurPet->HP / 3 ) power *= 1.5;
	else if ( "�ͻ�" == playerAck.pTalent->name 
		&& Race::huo == skillRace
		&& playerAck.pCurPet->curHP < playerAck.pCurPet->HP / 3 ) power *= 1.5;
	else if ( "����" == playerAck.pTalent->name 
		&& Race::shui == skillRace
		&& playerAck.pCurPet->curHP < playerAck.pCurPet->HP / 3 ) power *= 1.5;
	else if ( "���屨��" == playerAck.pTalent->name 
		&& Race::chong == skillRace
		&& playerAck.pCurPet->curHP < playerAck.pCurPet->HP / 3 ) power *= 1.5;
	else if ( "����" == playerAck.pTalent->name && BiteSkill(playerAck.pSkill) ) power *= 1.2;
	if ( "ɳ֮��" == playerAck.pTalent->name && Race::yan == m_weather
		&& (Race::di == skillRace||Race::yan == skillRace||Race::gang == skillRace) ) 
	{
		power *= 1.5;
	}
	if ( "�ڰ���Ϣ" == playerAck.pTalent->name && Race::e == skillRace ) power *= 1.5;

	if ( 0 < playerDef.wall[0] && 1 == playerAck.pSkill->type 
		&& "��͸" != playerAck.pTalent->name && !ct ) power /= 2;
	if ( 0 < playerDef.wall[1] && 3 == playerAck.pSkill->type 
		&& "��͸" != playerAck.pTalent->name && !ct ) power /= 2;

	//////////////////////////////////////////////////////////////////////////
	//װ��
	if ( "������" == playerAck.pItem->name ) power *= 1.25;

	//////////////////////////////////////////////////////////////////////////
	//����
	if ( "��Ԫ��" == playerAck.pSkill->name && 0 != playerAck.pCurPet->state ) power *= 2;


	//////////////////////////////////////////////////////////////////////////
	//��������
	if ( "��֬��" == playerDef.pTalent->name 
		&& (Race::huo == skillRace || Race::bing == skillRace) ) power /= 2;
	if ( "ʥ��ʿ" == playerDef.pTalent->name ) 
	{
		if ( playerDef.lookSkill[playerAck.pSkill->id] ) 
		{
			power = 0;
			m_pCurRound->log.push_back("��ʥ��ʿ����1�εļ�������Ч��");
		}
		else playerDef.lookSkill[playerAck.pSkill->id] = true;
	}
	if ( "ʳ��" == playerDef.pTalent->name && Race::cao == playerAck.pSkill->race )
	{
		power = 0;
		if ( 6 > playerDef.wg ) 
		{
			playerDef.wg++;
			m_pCurRound->log.push_back("�Է������˹������﹥����");
		}
	}
	if ( "��ˮ" == playerDef.pTalent->name && Race::shui == playerAck.pSkill->race )
	{
		power = 0;
		playerDef.pCurPet->curHP += playerDef.pCurPet->HP / 4;
		m_pCurRound->log.push_back("�Է�����ˮ�˷֣��ָ�1/4HP");
	}
	if ( "����" == playerDef.pTalent->name && Race::huo == playerAck.pSkill->race )
	{
		power = 0;
		playerDef.race[Race::huo] = true;
		m_pCurRound->log.push_back("�Է������˻��棬��ϵ��������");
	}
	if ( "��������" == playerDef.pTalent->name && Race::dian == playerAck.pSkill->race )
	{
		power = 0;
		if ( 6 > playerDef.sd ) 
		{
			playerDef.sd++;
			m_pCurRound->log.push_back("�Է��������������ٶ�����");
		}
	}

	return power;
}

bool Battle::PetAction(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef)
{
	playerAck.isActioned = true;
	if ( playerAck.rest )
	{
		playerAck.rest = false;
		m_pCurRound->log.push_back(playerAck.pCurPet->nick + "��Ҫ��Ϣ,�����ж�");
		return false;
	}
	if ( playerAck.fear )
	{
		m_pCurRound->log.push_back(playerAck.pCurPet->nick + "������,�����ж�");
		if ( "����" == playerAck.pTalent->name && playerAck.sd < 6 )
		{
			playerAck.sd++;
			m_pCurRound->log.push_back(playerAck.pCurPet->nick + "��Ϊ�����������ٶ�");
		}
		return false;
	}
	if ( playerAck.attacked 
		&& ( "����ȭ" == playerAck.pSkill->name 
		|| "�㱼����ȭ" == playerAck.pSkill->name ) )
	{
		m_pCurRound->log.push_back(playerAck.pCurPet->nick + "�������ж�");
		return false;
	}
	if ( LaunchState(playerAck) && !Medication(playerAck) ) return false;
	if ( Confusion(playerAck) ) 
	{
		return UseSkill(playerAck, playerAck);
	}

	ChangeSkill(playerAck);
	if ( BanSkill(playerAck, playerDef) ) 
	{
		m_pCurRound->log.push_back(playerAck.pCurPet->nick + "�ܵ���ӡ������ʹ�õط����յļ���");
		return false;
	}

	return UseSkill(playerAck, playerDef);
}

bool Battle::UseSkill(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef)
{
	m_pCurRound->log.push_back(playerAck.pCurPet->nick + "ʹ����" + playerAck.pSkill->name);
	playerAck.lockSkill = playerAck.pSkill->id;
	if ( "����" == playerAck.pSkill->name || "����" == playerAck.pSkill->name )
	{
		if ( !playerAck.defensed ) m_pCurRound->log.push_back("����ʧ����");
		return true;
	}

	//�ǹ�������
	if ( 2 == playerAck.pSkill->type ) 
	{
		if ( 0 < playerAck.tiaoDou )
		{
			m_pCurRound->log.push_back(playerAck.pCurPet->nick + "����������ʹ�ø�������");
			return false;
		}
		if ( !HelpSkill(playerAck, playerDef) ) 
		{
			if ( Battle::attack == playerDef.act && "����" == playerDef.pSkill->name && playerDef.defensed ) 
			{
				m_pCurRound->log.push_back(playerDef.pCurPet->nick + "����ס���Լ�");
				if ( "��ը" == playerAck.pSkill->name ) Hurt(playerAck, playerAck.pCurPet->curHP);
				return false;
			}
			if ( "ħװ����" == playerDef.pTalent->name ) 
			{
				m_pCurRound->log.push_back(playerDef.pCurPet->nick + "��" 
					+ playerDef.pTalent->name + "�����˼���");
				InterfereSkill(playerAck, playerAck);
				return false;
			}
			return InterfereSkill(playerAck, playerDef);
		}
		return true;
	}

	//��������
	if ( "��������" == playerAck.pSkill->name && Race::huo != m_weather )
	{
		if ( !playerAck.sunReady )
		{
			playerAck.sunReady = true;
			if ( 0 == playerAck.lockSkillTime ) playerAck.lockSkillTime = 2;
			playerAck.banChangeRound = 2;
			m_pCurRound->log.push_back(playerAck.pCurPet->nick + "������̫������");
			return true;
		}
		else playerAck.sunReady = false;
	}
	if ( "����֮Ը" == playerAck.pSkill->name )
	{
		if ( 0 < playerAck.doomDesireRound ) 
		{
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "ʧ����");
			return false;
		}
		if ( -1 == playerAck.doomDesireRound )
		{
			playerAck.doomDesireRound = 3;
			return true;
		}
		playerAck.doomDesireRound = -1;
	}
	if ( "Ԥ֪δ��" == playerAck.pSkill->name )
	{
		if ( 0 < playerAck.predictRound ) 
		{
			m_pCurRound->log.push_back(playerAck.pCurPet->nick + "ʧ����");
			return false;
		}
		if ( -1 == playerAck.predictRound )
		{
			playerAck.predictRound = 3;
			return true;
		}
		playerAck.predictRound = -1;
	}

	if ( Battle::attack == playerDef.act && "����" == playerDef.pSkill->name && playerDef.defensed ) 
	{
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "����ס���Լ�");
		if ( "��ը" == playerAck.pSkill->name ) Hurt(playerAck, playerAck.pCurPet->curHP);
		return false;
	}
	if ( !CalHitRate(playerAck, playerDef) )
	{
		m_pCurRound->log.push_back(playerAck.pCurPet->nick + "�Ĺ���û������" );
		return false;
	}
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
		if ( "������" == playerAck.pSkill->name ) F = CalWF(playerDef, ct, playerAck);
		else F = CalTF(playerDef, ct, playerAck);
	}
	Power = CalPower(playerAck, ct, playerDef);
	if ( 0 == Power ) 
	{
		if ( "��ը" == playerAck.pSkill->name ) Hurt(playerAck, playerAck.pCurPet->curHP);
		return false;
	}
	if ( ct ) 		
	{
		m_pCurRound->log.push_back(playerAck.pCurPet->nick + "�����������������䣡����" );
	}


	playerAck.outputHurt = G*Power/F*0.84*playerAck.rp.hurt/255;
	bool unFaint = false;
	if ( "���е�" == playerAck.pSkill->name ) unFaint = true;
	else if ( Battle::attack == playerDef.act && "����" == playerDef.pSkill->name && playerDef.defensed ) unFaint = true;
	else if ( ("��ʵ" == playerDef.pTalent->name || "��������" == playerAck.pItem->name)
		&& playerDef.pCurPet->curHP == playerDef.pCurPet->HP ) 
	{
		unFaint = true;
		playerDef.pCurPet->itemId = 0;
	}
	playerDef.pCurPet->curHP = playerAck.pCurPet->curHP = 9999;
	if ( 0 == playerDef.tiShen )
	{
		playerAck.outputHurt = Hurt(playerDef, playerAck.outputHurt, unFaint); 
		playerDef.attacked = true;
	}
	else
	{
		playerAck.outputHurt = playerAck.outputHurt <= playerDef.tiShen?playerAck.outputHurt:playerDef.tiShen;
		m_pCurRound->log.push_back( playerDef.pCurPet->nick + "������������˺�" );
	}
	if ( "����֮Ը" != playerAck.pSkill->name
		&& "Ԥ֪δ��" != playerAck.pSkill->name )
	{
		AttackEffect(playerAck, playerDef);
	}
	if ( 0 < playerDef.tiShen )
	{
		playerDef.tiShen -= playerAck.outputHurt;
		if ( 0 > playerDef.tiShen ) playerDef.tiShen = 0;
	}

	return true;
}

bool Battle::InterfereSkill(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef)
{
	if ( !CalHitRate(playerAck, playerDef) )
	{
		m_pCurRound->log.push_back(playerAck.pCurPet->nick + "�Ĺ���û������" );
		return false;
	}

	if ( "����" == playerAck.pSkill->name )//		��	0	2	101	0	ʹ�ϳ��͵��ܵ�1/8�˺�
	{
		if ( playerDef.nail[Race::di] )
		{
			m_pCurRound->log.push_back("��������������");
			return false;
		}
		playerDef.nail[Race::di] = true;
		m_pCurRound->log.push_back(playerDef.name + "���������˶��ӣ��͵��ϳ�ʱ���ܵ����HP��1/4���˺�");
		return true;
	}


	if ( "���" == playerAck.pSkill->name //	��	0	2	101	0	�����֣�ǿ�ƶԷ��˳�
		|| "����" == playerAck.pSkill->name )//		��	0	2	101	0	�����֣�ǿ�ƶԷ��˳�
	{
		ForcedLeave(playerDef);
		EntryStage(playerDef, playerAck);
		return true;
	}

	if ( 0 < playerDef.tiShen )
	{
		m_pCurRound->log.push_back("��������˹���");
		return false;
	}
	if ( "���" == playerAck.pSkill->name )//		��	0	2	95	0	����
	{
		if ( "����" == playerDef.pTalent->name )
		{
			playerDef.race[Race::huo] = true;
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "�����˻��棬��ϵ��������");
			return false;
		}
		if ( ImmuneState(playerDef) )
		{
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "�����˹���");
			return false;
		}
		playerDef.pCurPet->state = Race::huo;
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "������");
		return true;
	}
	if ( "��Ų�" == playerAck.pSkill->name )//		��	0	2	100	0	���
	{
		if ( "��������" == playerDef.pTalent->name )
		{
			if ( 6 > playerDef.sd ) 
			{
				playerDef.sd++;
				m_pCurRound->log.push_back(playerDef.pCurPet->nick + "�������������ٶ�����");
			}
			return false;
		}
		if ( ImmuneState(playerDef)  
			|| "����" == playerDef.pTalent->name
			|| Race::di == playerDef.pBuddy->race1
			|| Race::di == playerDef.pBuddy->race2)
		{
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "�����˹���");
			return false;
		}
		playerDef.pCurPet->state = Race::dian;
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "�����");
		return true;
	}
	if ( "��Է�" == playerAck.pSkill->name )//		��	0	2	75	0	���
	{
		if ( "ʳ��" == playerDef.pTalent->name )
		{
			if ( 6 > playerDef.wg ) 
			{
				playerDef.wg++;
				m_pCurRound->log.push_back(playerDef.pCurPet->nick + "�����˹������﹥����");
			}
			return false;
		}
		if ( ImmuneState(playerDef) || "����" == playerDef.pTalent->name )
		{
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "�����˹���");
			return false;
		}
		playerDef.pCurPet->state = Race::dian;
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "�����");
		return true;
	}
	if ( "�綾" == playerAck.pSkill->name )//		��	0	2	85	0	�ж�
	{
		if ( ImmuneState(playerDef) || "����" == playerDef.pTalent->name 
			|| Race::gang == playerDef.pBuddy->race1
			|| Race::gang == playerDef.pBuddy->race2 )
		{
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "�����˹���");
			return false;
		}
		playerDef.pCurPet->state = Race::du;
		playerDef.duRound = 0;
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "�ж���");
		return true;
	}
	if ( "������" == playerAck.pSkill->name //		��	0	2	60	0	���߶Է�
		||"��ħ��" == playerAck.pSkill->name //		��	0	2	75	0	���߶Է�
		||"�ڶ�" == playerAck.pSkill->name //		��	0	2	80	0	���߶Է�
		||"˯�߷�" == playerAck.pSkill->name )//		��	0	2	100	0	���߶Է�
	{
		if ( "˯�߷�" == playerAck.pSkill->name && "ʳ��" == playerDef.pTalent->name )
		{
			if ( 6 > playerDef.wg ) playerDef.wg++;
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "�����˹������﹥����");
			return false;
		}
		if ( ImmuneState(playerDef) || "ʧ��" == playerDef.pTalent->name )
		{
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "�����˹���");
			return false;
		}
		playerDef.pCurPet->state = Race::pu;
		playerDef.pCurPet->sleepRound = playerDef.rp.sleepRound;
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "˯����");
		return true;
	}

	if ( "����" == playerAck.pSkill->name //	��	0	2	101	1	��ֹ���˺����ܡ���ս��ʹ�ÿ����̵���Ʒ������ת��Ϊ������
		|| "����" == playerAck.pSkill->name )//	����	��	0	2	101	0	��ֹ�Է����˺�����
	{
		if ( "����" == playerDef.pTalent->name ) 
		{
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "�����˹���");
			return false;
		}
		playerDef.banChangeRound = -1;
		playerDef.sdCorrect *= 2;
		if ( "����" == playerAck.pSkill->name ) 
		{
			playerDef.wgCorrect *= 1.5;
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "��������,���ٶȼ�������������������");
		}
		if ( "����" == playerAck.pSkill->name )
		{
			playerDef.tgCorrect *= 1.5;
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "��������,���ٶȼ������������⹥������");
		}
		return true;
	}
	if ( "çײ" == playerAck.pSkill->name )//		��	0	2	100	0	�Է�HP���ٵ����Լ���ͬ
	{
		if ( Race::gui == playerDef.pBuddy->race1 )
		{
			if ( "����" != playerAck.pTalent->name && !playerAck.smell )
			{
				m_pCurRound->log.push_back(playerDef.pCurPet->nick + "�����˹���");
				return false;
			}
		}
		if ( playerDef.pCurPet->curHP > playerAck.pCurPet->curHP )
		{
			playerAck.outputHurt = Hurt(playerDef, playerDef.pCurPet->curHP - playerAck.pCurPet->curHP);
		}
		return true;
	}
	if ( "����" == playerAck.pSkill->name )//		��	0	2	100	0	������Ʒ
	{
		if ( 0 == playerAck.pCurPet->itemId || 0 == playerDef.pCurPet->itemId )
		{
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "û����Ʒʧ��");
			return false;
		}
		int id = playerAck.pCurPet->itemId;
		playerAck.pCurPet->itemId = playerDef.pCurPet->itemId;
		playerDef.pCurPet->itemId = id;
		data::ITEM *pItem = playerAck.pItem;
		playerAck.pItem = playerDef.pItem;
		playerDef.pItem = pItem;
		m_pCurRound->log.push_back(playerAck.pCurPet->nick + "�õ���" + playerAck.pItem->name);
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "�õ���" + playerDef.pItem->name);
		return true;
	}
	if ( "��������" == playerAck.pSkill->name )//		��	0	2	85	0	ÿ�غ�����1/8�������Է����½��
	{
		if ( playerDef.seed || Race::cao == playerDef.pBuddy->race1 || Race::cao == playerDef.pBuddy->race2 )
		{
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "�����˹���");
			return false;
		}
		playerDef.seed = true;
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "������������");
		return true;
	}
	if ( "��Ƿ" == playerAck.pSkill->name )//		��	0	2	100	0	�Է��»غϱ�����
	{
		if ( -1 != playerDef.haQian )
		{
			m_pCurRound->log.push_back("ʧ����");
			return false;
		}
		if ( ImmuneState(playerDef) || "ʧ��" == playerDef.pTalent->name )
		{
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "�����˹���");
			return false;
		}
		playerDef.haQian = 1;
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "�����˯");
		return true;
	}
	if ( "����" == playerAck.pSkill->name )//		��	0	2	100	0	3�غϲ��ܸ�������
	{
		if ( 0 == playerDef.lockSkill || 0 != playerDef.lockSkillTime )
		{
			m_pCurRound->log.push_back("ʧ����");
			return false;
		}
		playerDef.guZhang = true;
		playerDef.lockSkillTime = 3;
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "�ܵ�����������3�غϲ��ܸ�������");
		return true;
	}
	if ( "Ѱ��" == playerAck.pSkill->name )//		��	0	2	100	0	ʹ�Է���������ʹ����ͬ����
	{
		if ( playerDef.xunXing )
		{
			m_pCurRound->log.push_back("û��Ч��");
			return false;
		}
		playerDef.xunXing = true;
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "��������ʹ����ͬ����");
		return true;
	}
	if ( "����" == playerAck.pSkill->name )//		��	0	2	100	0	ʹ�Է�3�غϲ���ʹ�ñ仯����
	{
		playerDef.tiaoDou = 3;
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "3�غϲ���ʹ�ø�������");
		return true;
	}

	if ( "��ӡ" == playerAck.pSkill->name )//		��	0	2	101	0	�Է�����ʹ���ҷ���ļ���
	{
		if ( playerDef.ban )
		{
			m_pCurRound->log.push_back("ʧ����");
			return false;
		}
		playerDef.ban = true;
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "����ʹ���ҷ�ӵ�еļ���");
		return true;
	}
	if ( "ͬ��" == playerAck.pSkill->name )//		��	0	2	101	0	�ܵ�����������0ʱ��������һ����
	{
		if ( playerDef.tongGui )
		{
			m_pCurRound->log.push_back("ʧ����");
			return false;
		}
		playerDef.tongGui = true;
		m_pCurRound->log.push_back(playerAck.pCurPet->nick + "����ʱ��" + playerDef.pCurPet->nick + "һ��");
		return true;
	}
	if ( "��ֹ�" == playerAck.pSkill->name )//		��	0	2	100	0	����
	{
		if ( ImmuneState(playerDef) )
		{
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "�����˹���");
			return false;
		}
		playerDef.pCurPet->state = Race::gui;
		playerDef.hunLuan = 3;
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "������");
		return true;
	}

	return false;
}

bool Battle::HelpSkill(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef)
{
	if ( "����" == playerAck.pSkill->name )//		��	0	2	101	0	5�غ�����
	{
		if ( Race::huo == m_weather )
		{
			m_pCurRound->log.push_back("�Ѿ������죬ʧ����");
			return true;
		}
		m_weather = Race::huo;//�������Զ�Ӧ
		m_weatherCount = 5;//����ʣ��غ��� -1����
		if ( "����" == playerAck.pItem->name ) m_weatherCount = 8;
		m_pCurRound->log.push_back("̫��������");
	}
	else if ( "����" == playerAck.pSkill->name )//		ˮ	0	2	101	0	5�غ�����
	{
		if ( Race::shui == m_weather )
		{
			m_pCurRound->log.push_back("�Ѿ������꣬ʧ����");
			return true;
		}
		m_weather = Race::shui;//�������Զ�Ӧ
		m_weatherCount = 5;//����ʣ��غ��� -1����
		if ( "����" == playerAck.pItem->name ) m_weatherCount = 8;
		m_pCurRound->log.push_back("������");
	}
	else if ( "����" == playerAck.pSkill->name )//		��	0	2	101	0	5�غϱ�������������������İ͵�ÿ�غϿ�ʼ�ܵ�1/16�˺�
	{
		if ( Race::bing == m_weather )
		{
			m_pCurRound->log.push_back("�Ѿ�����ѩ��ʧ����");
			return true;
		}
		m_weather = Race::bing;//�������Զ�Ӧ
		m_weatherCount = 5;//����ʣ��غ��� -1����
		if ( "����" == playerAck.pItem->name ) m_weatherCount = 8;
		m_pCurRound->log.push_back("��ѩ��");
	}
	else if ( "ɳ��" == playerAck.pSkill->name )//		��	0	2	101	0	5�غ�ɳ�����������Ҹ��������԰͵��ܵ�1/16�˺�
	{
		if ( Race::yan == m_weather )
		{
			m_pCurRound->log.push_back("�Ѿ���ɳ�����У�ʧ����");
			return true;
		}
		m_weather = Race::yan;//�������Զ�Ӧ
		m_weatherCount = 5;//����ʣ��غ��� -1����
		if ( "����" == playerAck.pItem->name ) m_weatherCount = 8;

		m_pCurRound->log.push_back("ɳ��������");
	}
	else if ( "��֮��" == playerAck.pSkill->name )//		��	0	2	101	0	5�غ������˺�����
	{
		if ( 0 < playerAck.wall[1] )
		{
			m_pCurRound->log.push_back("�Ѿ����˹�֮�ڣ�ʧ����");
			return true;
		}
		if ( "����" == playerAck.pItem->name ) 
		{
			playerAck.wall[1] = 8;
			m_pCurRound->log.push_back("�ҷ��ܵ���֮�ڱ�����8�غ��������˺�����");
		}
		else
		{
			playerAck.wall[1] = 5;
			m_pCurRound->log.push_back("�ҷ��ܵ���֮�ڱ�����5�غ��������˺�����");
		}
	}
	else if ( "�����" == playerAck.pSkill->name )//		��	0	2	101	0	5�غ������˺�����
	{
		if ( 0 < playerAck.wall[0] )
		{
			m_pCurRound->log.push_back("�Ѿ����˷���ܣ�ʧ����");
			return true;
		}
		if ( "����" == playerAck.pItem->name ) 
		{
			playerAck.wall[0] = 8;
			m_pCurRound->log.push_back("�ҷ��ܵ�����ܱ�����8�غ��������˺�����");
		}
		else
		{
			playerAck.wall[0] = 5;
			m_pCurRound->log.push_back("�ҷ��ܵ�����ܱ�����5�غ��������˺�����");
		}
	}
	else if ( "��ƭ�ռ�" == playerAck.pSkill->name )//		��	0	2	101	0	5�غ��ٶ��������ж�
	{
		if ( 0 != m_foolSpace )
		{
			m_pCurRound->log.push_back("�Ѿ�������ƭ�ռ䣬ʧ����");
			return true;
		}
		if ( "����" == playerAck.pItem->name ) 
		{
			m_foolSpace = 8;
			m_pCurRound->log.push_back("����ת��Ϊ��ƭ�ռ䣬8�غ����ٶȵ͵����ж�");
		}
		else
		{
			m_foolSpace = 5;
			m_pCurRound->log.push_back("����ת��Ϊ��ƭ�ռ䣬5�غ����ٶȵ͵����ж�");
		}
	}
	else if ( "����" == playerAck.pSkill->name )//		��	0	2	101	0	ȫ�������ȼ���ԭ
	{
		playerAck.wg = playerDef.wg = 0;//��ǿ���ȼ�
		playerAck.wf = playerDef.wf = 0;//��ǿ���ȼ�
		playerAck.tg = playerDef.tg = 0;//�ع�ǿ���ȼ�
		playerAck.tf = playerDef.tf = 0;//�ط�ǿ���ȼ�
		playerAck.sd = playerDef.sd = 0;//�ٶ�ǿ���ȼ�
		playerAck.ct = playerDef.ct = 0;//����ǿ���ȼ�
		m_pCurRound->log.push_back("˫�������仯������");
	}
	else if ( "�Գ���" == playerAck.pSkill->name )//		��	0	2	101	0	�����﹥
	{
		if ( playerAck.pCurPet->curHP < playerAck.pCurPet->HP / 2 )
		{
			m_pCurRound->log.push_back("�������㣬ʧ����");
			return true;
		}
		m_pCurRound->log.push_back("��ʧһ���������������������������");
		playerAck.wg = 6;
		Hurt(playerAck, playerAck.pCurPet->HP / 2);
	}
	else if ( "����" == playerAck.pSkill->name )//		��	0	2	101	0	���ǿ���﹥
	{
		if ( 6 == playerAck.wg )
		{
			m_pCurRound->log.push_back("�������Ѵﵽ���ޣ�ʧ����");
			return true;
		}
		playerAck.wg += 2;
		m_pCurRound->log.push_back("�������������");
	}
	else if ( "Ӳ���" == playerAck.pSkill->name )//		�� 	0	2	100	0	���ǿ�����
	{
		if ( 6 == playerAck.wf )
		{
			m_pCurRound->log.push_back("��������Ѵﵽ���ޣ�ʧ����");
			return true;
		}
		if ( 6 > playerAck.wf  )
		{
			playerAck.wf += 2;
			m_pCurRound->log.push_back("��������������");
		}
	}
	else if ( "ө��" == playerAck.pSkill->name )//		��	0	2	101	0	���ǿ���ع�
	{
		if ( 6 == playerAck.tg )
		{
			m_pCurRound->log.push_back("���⹥���Ѵﵽ���ޣ�ʧ����");
			return true;
		}
		if ( 6 > playerAck.tg  )
		{
			playerAck.tg += 2;
			m_pCurRound->log.push_back("���⹥���������");
		}
	}
	else if ( "��������" == playerAck.pSkill->name )//		��	0	2	101	0	���ǿ���ط�
	{
		if ( 6 == playerAck.tf )
		{
			m_pCurRound->log.push_back("��������Ѵﵽ���ޣ�ʧ����");
			return true;
		}
		if ( 6 > playerAck.tf  )
		{
			playerAck.tf += 2;
			m_pCurRound->log.push_back("��������������");
		}
	}
	else if ( "�����ƶ�" == playerAck.pSkill->name )//		��	0	2	101	0	���ǿ���ٶ�
	{
		if ( 6 == playerAck.sd )
		{
			m_pCurRound->log.push_back("�ٶ��Ѵﵽ���ޣ�ʧ����");
			return true;
		}
		if ( 6 > playerAck.sd  )
		{
			playerAck.sd += 2;
			m_pCurRound->log.push_back("�ٶȴ������");
		}
	}
	else if ( "�޴�" == playerAck.pSkill->name )//		��	0	2	101	0	ǿ���﹥��ǿ�����
	{
		if ( 6 == playerAck.wg && 6 == playerAck.wf )
		{
			m_pCurRound->log.push_back("����������������Ѵﵽ���ޣ�ʧ����");
			return true;
		}
		if ( 6 > playerAck.wg  )
		{
			playerAck.wg++;
			m_pCurRound->log.push_back("�����������");
		}
		if ( 6 > playerAck.wf  )
		{
			playerAck.wf++;
			m_pCurRound->log.push_back("������������");
		}
	}
	else if ( "ڤ��" == playerAck.pSkill->name )//		��	0	2	101	0	ǿ���ع���ǿ���ط�
	{
		if ( 6 == playerAck.tg && 6 == playerAck.tf )
		{
			m_pCurRound->log.push_back("���⹥������������Ѵﵽ���ޣ�ʧ����");
			return true;
		}
		if ( 6 > playerAck.tg  )
		{
			playerAck.tg++;
			m_pCurRound->log.push_back("��������⹥��");
		}
		if ( 6 > playerAck.tf  )
		{
			playerAck.tf++;
			m_pCurRound->log.push_back("������������");
		}
	}
	else if ( "ĥצ��" == playerAck.pSkill->name )//		��	0	2	101	0	ǿ���﹥����߱�����
	{
		if ( 6 == playerAck.wg && 6 == playerAck.ct )
		{
			m_pCurRound->log.push_back("ǿ���Ѵﵽ���ޣ�ʧ����");
			return true;
		}
		if ( 6 > playerAck.wg  )
		{
			playerAck.wg++;
			m_pCurRound->log.push_back("�����������");
		}
		if ( 6 > playerAck.ct ) 
		{
			playerAck.ct++;
			m_pCurRound->log.push_back("�������߱�����");
		}
	}
	else if ( "���" == playerAck.pSkill->name )//		��	0	2	101	0	ǿ���ع�����ϵ����*1.5
	{
		if ( 6 == playerAck.tg && playerAck.race[Race::dian] )
		{
			m_pCurRound->log.push_back("ǿ���Ѵﵽ���ޣ�ʧ����");
			return true;
		}
		if ( 6 > playerAck.tg  )
		{
			playerAck.tg++;
			m_pCurRound->log.push_back("��������⹥��");
		}
		if ( !playerAck.race[Race::dian] ) 
		{
			playerAck.race[Race::dian] = true;
			m_pCurRound->log.push_back("ǿ���˵�ϵ����");
		}
	}
	else if ( "��֮��" == playerAck.pSkill->name )//		��	0	2	101	0	ǿ���﹥��ǿ���ٶ�
	{
		if ( 6 == playerAck.wg && 6 == playerAck.sd )
		{
			m_pCurRound->log.push_back("���������ٶ��Ѵﵽ���ޣ�ʧ����");
			return true;
		}
		if ( 6 > playerAck.wg  )
		{
			playerAck.wg++;
			m_pCurRound->log.push_back("�����������");
		}
		if ( 6 > playerAck.sd  )
		{
			playerAck.sd++;
			m_pCurRound->log.push_back("������ٶ�");
		}
	}
	else if ( "��֮��" == playerAck.pSkill->name )//		��	0	2	101	0	ǿ���ع���ǿ���ط���ǿ���ٶ�
	{
		if ( 6 == playerAck.tg && 6 == playerAck.tf && 6 == playerAck.sd )
		{
			m_pCurRound->log.push_back("���⹥��������������ٶ��Ѵﵽ���ޣ�ʧ����");
			return true;
		}
		if ( 6 > playerAck.tg  )
		{
			playerAck.tg++;
			m_pCurRound->log.push_back("��������⹥��");
		}
		if ( 6 > playerAck.tf  )
		{
			playerAck.tf++;
			m_pCurRound->log.push_back("������������");
		}
		if ( 6 > playerAck.sd  )
		{
			playerAck.sd++;
			m_pCurRound->log.push_back("������ٶ�");
		}
	}
	else if ( "����" == playerAck.pSkill->name //		��	0	2	101	0	�ظ�50%����
		|| "����" == playerAck.pSkill->name ) //		��	0	2	101	0	�ظ�50%����
	{
		playerAck.pCurPet->curHP += playerAck.pCurPet->HP / 2;
		m_pCurRound->log.push_back("�ظ�������");
	}
	else if ( "�������" == playerAck.pSkill->name //		��	0	2	101	0	�ظ�50%HP,����2����������������
		|| "����" == playerAck.pSkill->name //		��	0	2	101	0	�ظ�50%HP,����2����������������
		|| "�¹�" == playerAck.pSkill->name )//		��	0	2	101	0	�ظ�50%HP,����2����������������
	{
		if ( 0 == m_weather ) playerAck.pCurPet->curHP += playerAck.pCurPet->HP / 2;
		else if ( Race::huo == m_weather ) playerAck.pCurPet->curHP += playerAck.pCurPet->HP;
		else if ( Race::huo != m_weather ) playerAck.pCurPet->curHP += playerAck.pCurPet->HP / 4;
		m_pCurRound->log.push_back("�ظ�������");
	}
	else if ( "����" == playerAck.pSkill->name )//		��	0	2	101	0	��������쳣
	{
		if ( 0 == playerAck.pCurPet->state )
		{
			m_pCurRound->log.push_back("û���쳣״̬,ʧ����");
			return true;
		}
		playerAck.pCurPet->state = 0;
		m_pCurRound->log.push_back("�ָ��˽���");
	}
	else if ( "��������" == playerAck.pSkill->name //		��	0	2	101	0	�ҷ�ȫ���쳣���
		|| "�����Ʒ�" == playerAck.pSkill->name )//		��	0	2	101	0	ȫ���쳣���
	{
		int i = 0;
		for ( i = 0; i < playerAck.pets.size(); i++ )
		{
			if ( playerAck.pets[i].curHP <= 0 ) continue;
			playerAck.pets[i].state = 0;
		}
		m_pCurRound->log.push_back("ȫ�Ӱ͵ϻָ��˽���");
	}
	else if ( "����Һ" == playerAck.pSkill->name )//		ˮ	0	2	101	0	5�غϣ�ÿ�غϻظ�1/16HP
	{
		playerAck.recvHP = playerAck.pCurPet->HP / 16;
		m_pCurRound->log.push_back("��Ӫ��Һ��Χ�ţ�ÿ�غϻظ�����");
	}
	else if ( "˯��" == playerAck.pSkill->name )//		��	0	2	101	0	˯3�غϣ�HPȫ����״̬���
	{
		playerAck.pCurPet->curHP = playerAck.pCurPet->HP;
		playerAck.pCurPet->state = Race::pu;
		playerAck.pCurPet->sleepRound = 3;
		m_pCurRound->log.push_back("˯���ˣ��ظ���ȫʢ״̬");
	}
	else if ( "����" == playerAck.pSkill->name )//		��	0	2	100	0	��ʧ1/4HP��������
	{
		if ( 0 < playerAck.tiShen )
		{
			m_pCurRound->log.push_back("�Ѿ�������ʧ����");
			return true;
		}
		if ( playerAck.pCurPet->curHP <= playerAck.pCurPet->HP / 4 )
		{
			m_pCurRound->log.push_back("�������㣬ʧ����");
			return true;
		}
		m_pCurRound->log.push_back("����1/4������������һ������");
		playerAck.tiShen = playerAck.pCurPet->HP / 4;
		Hurt(playerAck, playerAck.tiShen);
	}
	else if ( "�λ�" == playerAck.pSkill->name )//		��	0	2	101	0	˯��ʱ�����ʹ�ü���
	{
		if ( Race::pu != playerAck.pCurPet->state )
		{
			m_pCurRound->log.push_back("ʧ����");
			return true;
		}
		int id = rand()%4 + 1;
		if ( 1 == id && playerAck.pSkill->id != playerAck.pCurPet->skill1 ) id = playerAck.pCurPet->skill1;
		else if ( 2 == id && playerAck.pSkill->id != playerAck.pCurPet->skill2 ) id = playerAck.pCurPet->skill2;
		else if ( 3 == id && playerAck.pSkill->id != playerAck.pCurPet->skill3 ) id = playerAck.pCurPet->skill3;
		else if ( 4 == id && playerAck.pSkill->id != playerAck.pCurPet->skill4 ) id = playerAck.pCurPet->skill4;
		playerAck.pSkill = Skill(id, m_game->SkillBook());
		if ( NULL == playerAck.pSkill )
		{
			m_pCurRound->log.push_back("�޼��ܿ���");
			return true;
		}
		short skill = playerAck.lockSkill;
		UseSkill(playerAck, playerDef);
		playerAck.lockSkill = skill;
	}
	else if ( "������Ϣ" == playerAck.pSkill->name )//		��	0	2	100	1	��ս��ʹ�ÿ�����Ұ���͵�
	{
		m_pCurRound->log.push_back("û��Ч��");
	}
	else if ( "���" == playerAck.pSkill->name )//		��	0	2	101	0	�պͶ����Լ��ܿɴ��й�ϵ�͵�
	{
		playerAck.smell = true;
		m_pCurRound->log.push_back("��ϵ���ܿ��Ի��й�ϵ");
	}
	else if ( "ĩ�ո�" == playerAck.pSkill->name )//		��	0	2	101	0	�����İ͵ϣ�3�غ��ڲ����£������
	{
		if ( 0 < playerDef.mieGe && 0 < playerAck.mieGe )
		{
			m_pCurRound->log.push_back("û��Ч��");
			return true;
		}
		if ( 0 == playerAck.mieGe ) playerAck.mieGe = 3;
		if ( 0 == playerDef.mieGe ) playerDef.mieGe = 3;
	}
	else if ( "����ʹ��" == playerAck.pSkill->name )//		��	0	2	100	0	˫��HPƽ��
	{
		m_pCurRound->log.push_back("��Է�ƽ��������");
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
			m_pCurRound->log.push_back( player.pCurPet->nick + "������" );
			return false;
		}
		m_pCurRound->log.push_back( player.pCurPet->nick + "����˯��" );
		return true;
	}
	else if ( Race::bing == player.pCurPet->state ) 
	{
		if ( 0 == player.pCurPet->frozenRound )
		{
			player.pCurPet->state = 0;
			m_pCurRound->log.push_back( player.pCurPet->nick + "����˱���" );
			return false;
		}
		m_pCurRound->log.push_back( player.pCurPet->nick + "��������" );
		return true;
	}
	else if ( Race::dian == player.pCurPet->state ) 
	{
		if ( 25 >= player.rp.dian ) return false;
		m_pCurRound->log.push_back( player.pCurPet->nick + "����ˣ������ж�" );
		return true;
	}

	return false;
}

bool Battle::Medication(Battle::WARRIOR &player)
{
	if ( 0 == player.pCurPet->state || "����ҩ" != player.pItem->name ) return false;

	if ( Race::pu == player.pCurPet->state ) 
	{
		if ( 0 == player.pCurPet->sleepRound ) return false;
		player.pCurPet->state = 0;
		m_pCurRound->log.push_back( m_player.pCurPet->nick + "ʹ��������ҩ�������˯��" );
	}
	else if ( Race::bing == player.pCurPet->state ) 
	{
		if ( 0 == player.pCurPet->frozenRound ) return false;
		player.pCurPet->state = 0;
		m_pCurRound->log.push_back( m_player.pCurPet->nick + "ʹ��������ҩ������˱���" );
	}
	else if ( Race::dian == player.pCurPet->state ) 
	{
		if ( 25 >= player.rp.dian ) return false;
		m_pCurRound->log.push_back( m_player.pCurPet->nick + "ʹ��������ҩ����������" );
	}
	else if ( Race::huo == player.pCurPet->state ) 
	{
		m_pCurRound->log.push_back( m_player.pCurPet->nick + "ʹ��������ҩ�����������" );
	}
	else if ( Race::gui == player.pCurPet->state ) 
	{
		m_pCurRound->log.push_back( m_player.pCurPet->nick + "ʹ��������ҩ������˻���" );
	}
	player.pCurPet->state = 0;

	return false;
}

void Battle::ChangeSkill(Battle::WARRIOR &player)
{
	if ( player.xunXing ) return;
	if ( 0 == player.lockSkillTime ) return;
	data::SKILL *pSkill = Skill(player.lockSkill, m_game->SkillBook());
	if ( NULL == pSkill || pSkill == player.pSkill ) return;
	player.pSkill = pSkill;
	m_pCurRound->log.push_back(player.pCurPet->nick + "�������в��ܸ�������");
}

bool Battle::BanSkill(Battle::WARRIOR &player, Battle::WARRIOR &enemy)
{
	if ( !player.ban ) return false;
	if ( player.pSkill->id != enemy.pCurPet->skill1
		|| player.pSkill->id != enemy.pCurPet->skill2
		|| player.pSkill->id != enemy.pCurPet->skill3
		|| player.pSkill->id != enemy.pCurPet->skill4
		) return false; 

	return true;
}

bool Battle::Confusion(Battle::WARRIOR &player)
{
	if ( Race::gui != player.pCurPet->state ) return false;
	if ( 0 == player.hunLuan )
	{
		m_pCurRound->log.push_back( player.pCurPet->nick + "����˻���" );
		return false;
	}
	m_pCurRound->log.push_back( player.pCurPet->nick + "������" );
	if ( !player.rp.luan ) return false;
	m_pCurRound->log.push_back( player.pCurPet->nick + "�������Լ�" );
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
				m_pCurRound->log.push_back(playerDef.pCurPet->nick + "������");
			}
			else if ( 2 == playerAck.pSkill->effects[i].id 
				&& "����" != playerDef.pTalent->name ) //dian
			{
				if ( playerAck.rp.sePro > playerAck.pSkill->effects[i].probability )  continue;
				playerDef.pCurPet->state = Race::dian;
				m_pCurRound->log.push_back(playerDef.pCurPet->nick + "�����");
			}
			else if ( 3 == playerAck.pSkill->effects[i].id 
				&& "������" != playerDef.pTalent->name ) //frozen
			{
				if ( playerAck.rp.sePro > playerAck.pSkill->effects[i].probability )  continue;
				playerDef.pCurPet->state = Race::bing;
				playerDef.pCurPet->frozenRound = playerDef.rp.frozenRound;
				m_pCurRound->log.push_back(playerDef.pCurPet->nick + "������");
			}
			else if ( 5 == playerAck.pSkill->effects[i].id
				&& "����" != playerDef.pTalent->name ) //du
			{
				duShou = true;
				char addCount = 0;
				if ( "����" == playerAck.pTalent->name ) addCount = 30;
				if ( playerAck.rp.sePro > playerAck.pSkill->effects[i].probability + addCount )  continue;
				playerDef.pCurPet->state = Race::du;
				playerDef.duRound = 0;
				m_pCurRound->log.push_back(playerDef.pCurPet->nick + "�ж���");
			}
		}
		if ( 0 < playerDef.pCurPet->curHP && "�׷�" != playerDef.pTalent->name )
		{
			if ( 42 == playerAck.pSkill->effects[i].id && "����" != playerDef.pTalent->name ) //-WF
			{
				if ( playerDef.wf <= -6
					|| 100 == playerAck.pSkill->effects[i].probability 
					|| playerAck.rp.sePro > playerAck.pSkill->effects[i].probability 
					)  continue;
				playerDef.wf--;
				m_pCurRound->log.push_back(playerDef.pCurPet->nick + "�����½���");
			}
			else if ( 44 == playerAck.pSkill->effects[i].id && "����" != playerDef.pTalent->name ) //-TF
			{
				if ( playerDef.tf <= -6
					|| 100 == playerAck.pSkill->effects[i].probability 
					|| playerAck.rp.sePro > playerAck.pSkill->effects[i].probability 
					)  continue;
				playerDef.tf--;
				m_pCurRound->log.push_back(playerDef.pCurPet->nick + "��������½���");
			}
			else if ( 0 < playerDef.pCurPet->curHP 
				&& 86 == playerAck.pSkill->effects[i].id 
				&& "������" != playerDef.pTalent->name ) //haki
			{
				haiPa = true;
				char addCount = 0;
				if ( "����" == playerAck.pItem->name ) addCount = 15;
				if ( "����" == playerAck.pTalent->name ) addCount += 20;
				if ( playerAck.rp.sePro > playerAck.pSkill->effects[i].probability + addCount )  continue;
				playerDef.fear = true;
			}
		}
		if ( 34 == playerAck.pSkill->effects[i].id ) //+WG
		{
			if ( playerAck.wg >= 6 
				|| playerAck.rp.sePro > playerAck.pSkill->effects[i].probability )  continue;
			playerAck.wg++;
			m_pCurRound->log.push_back(playerAck.pCurPet->nick + "����������");
		}
		if ( 120 == playerAck.pSkill->effects[i].id ) //�峡
		{
			if ( playerAck.nail[Race::di] )
			{
				playerAck.nail[Race::di] = false;
				m_pCurRound->log.push_back(playerAck.name + "���϶��ӱ�����ɾ���");
			}
			if ( playerAck.seed )
			{
				playerAck.seed = false;
				m_pCurRound->log.push_back(playerAck.pCurPet->nick + "����������");
			}
		}
	}

	if ( 0 < playerDef.pCurPet->curHP
		&& ("����" == playerAck.pItem->name || "����" == playerAck.pTalent->name)
		&& "����" != playerAck.pTalent->name
		&& "������" != playerDef.pTalent->name
		&& !haiPa )
	{
		char addCount = 0;
		if ( "����" == playerAck.pItem->name ) addCount = 15;
		if ( "����" == playerAck.pTalent->name ) addCount += 20;
		if ( playerAck.rp.sePro <= addCount )
		{
			playerDef.fear = true;
		}
	}
	if ( 0 < playerDef.pCurPet->curHP
		&& "����" == playerAck.pTalent->name 
		&& !ImmuneState(playerDef)
		&& "����" != playerDef.pTalent->name
		&& 0 == playerDef.pCurPet->state 
		&& !duShou )
	{
		if ( playerAck.rp.tePro <= 30 )  
		{
			playerDef.pCurPet->state = Race::du;
			playerDef.duRound = 0;
			m_pCurRound->log.push_back(playerAck.pCurPet->nick + "ʹ��" 
				+ playerAck.pTalent->name + playerDef.pCurPet->nick + "�ж���");
		}
	}

	if ( "���ѻ�ת" == playerAck.pSkill->name ) 
	{
		LeaveStage(playerAck);
		return true;
	}
	else if ( "Ʈ����ѩ���㴵" == playerAck.pSkill->name 
		|| "�����β" == playerAck.pSkill->name 
		|| "��֮β" == playerAck.pSkill->name ) 
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
		m_pCurRound->log.push_back(player.name + "û�п��滻�İ͵�");
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
			m_pCurRound->log.push_back(player.name + "�����д��ڷǷ��͵�");
			return false;
		}

		player.isChanged = ChangePet(player, player.pets[i].id);
		return true;
	}

	return true;
}

bool Battle::AttackCost(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef)
{
	if ( "��ը" == playerAck.pSkill->name ) 
	{
		Hurt(playerAck, playerAck.pCurPet->curHP);
		return true;
	}
	if ( 0 >= playerDef.pCurPet->curHP && playerAck.tongGui) //ͬ��
	{
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "��" 
			+ playerAck.pCurPet->nick + "һ������");
		playerAck.pCurPet->curHP = 0;
		Faint(playerAck);
		return true;
	}
	//����Ӱ��
	if ( 1 == playerAck.pSkill->type && !ImmuneState(playerAck) )
	{
		if ( "����" == playerDef.pTalent->name //�ܵ�������ʱ��30%��Զ���
			&& "����" != playerAck.pTalent->name
			)
		{
			playerAck.pCurPet->state = Race::dian;
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "��" 
				+ playerDef.pTalent->name + "ʹ" + playerAck.pCurPet->nick + "�����");
		}
		if ( "����" == playerDef.pTalent->name //�ܵ�������ʱ��30%�ж�
			&& "����" != playerAck.pTalent->name
			)
		{
			playerAck.pCurPet->state = Race::du;
			playerDef.duRound = 0;
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "��" 
				+ playerDef.pTalent->name + "ʹ" + playerAck.pCurPet->nick + "�ж���");
		}
		if ( "������" == playerDef.pTalent->name )//	���������ܵ�������ʱ��30%����
		{
			playerAck.pCurPet->state = Race::huo;
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "��" 
				+ playerDef.pTalent->name + "ʹ" + playerAck.pCurPet->nick + "������");
		}
		if ( "��������" == playerDef.pTalent->name //	�ܵ�������ʱ��30%����
			&& "������" != playerAck.pTalent->name)
		{
			playerAck.pCurPet->state = Race::bing;
			m_pCurRound->log.push_back(playerDef.pCurPet->nick + "��" 
				+ playerDef.pTalent->name + "ʹ" + playerAck.pCurPet->nick + "������");
		}
	}
	if ( "������" == playerDef.pTalent->name && Race::e == playerAck.pSkill->race 
		&& 6 > playerDef.wg )//	�������Թ�����ǿ���﹥
	{
		playerDef.wg++;
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "��" 
			+ playerDef.pTalent->name + "����˹���");
	}
	if ( 1 == playerAck.pSkill->type && "��Ƥ" == playerDef.pTalent->name 
		&& "ħ������" != playerAck.pTalent->name
		&& "ʯ��" != playerAck.pTalent->name )//����
	{
		float hurt = playerAck.outputHurt / 3;
		bool unFaint = false;
		if ( "����" == playerAck.pTalent->name ) unFaint = true;
		m_pCurRound->log.push_back(playerAck.pCurPet->nick + "�ܵ�����");
		Hurt( playerAck, hurt, unFaint );
		if ( 0 >= playerAck.pCurPet->curHP ) return true;//����
	}
	if ( ("������" == playerAck.pItem->name || BiteSkill(playerAck.pSkill) )
		&& "ħ������" != playerAck.pTalent->name 
		&& "ʯ��" != playerAck.pTalent->name )//����
	{
		float hurt = 0;
		if ( BiteSkill(playerAck.pSkill) ) hurt = playerAck.outputHurt / 3;
		if ( "������" == playerAck.pItem->name ) hurt += playerAck.pCurPet->HP / 8;
		bool unFaint = false;
		if ( "����" == playerAck.pTalent->name ) unFaint = true;
		m_pCurRound->log.push_back(playerAck.pCurPet->nick + "�ܵ�����");
		Hurt( playerAck, hurt, unFaint );
		if ( 0 >= playerAck.pCurPet->curHP ) return true;//����
	}
	//����
	if ( "�����ֶ�" == playerAck.pSkill->name )//���ܸ������ܣ����ܽ����͵ϣ���������
	{
		playerAck.lockSkillTime = -1;
		playerAck.banChangeRound = -1;
	}
	if ( "��ѹˮ��" == playerAck.pSkill->name
		|| "������" == playerAck.pSkill->name)//�»غϲ����ж�
	{
		playerAck.rest = true;
	}
	if ( 0 == playerAck.luanWu
		&&
		( "������" == playerAck.pSkill->name
		|| "�����β" == playerAck.pSkill->name
		|| "��ӿ" == playerAck.pSkill->name
		|| "����" == playerAck.pSkill->name
		|| "��֮��" == playerAck.pSkill->name
		|| "����" == playerAck.pSkill->name
		|| "����" == playerAck.pSkill->name
		|| "���Ԫ���" == playerAck.pSkill->name
		|| "����ӿ��" == playerAck.pSkill->name
		|| "��֮β" == playerAck.pSkill->name )
		)//���裬����ʹ��2~3�غ�
	{
		playerAck.luanWu = playerAck.rp.luanWu;
		if ( -1 != playerAck.lockSkillTime ) playerAck.lockSkillTime = playerAck.luanWu;
		if ( -1 != playerAck.banChangeRound ) playerAck.banChangeRound = playerAck.luanWu;
	}
	if ( "����" != playerAck.pTalent->name )
	{
		if ( "�����" == playerAck.pSkill->name )//����������������������ط�
		{
			if ( -6 < playerAck.wf )
			{
				playerAck.wf--;
				m_pCurRound->log.push_back(playerAck.pCurPet->nick + "�ķ���������");
			}
			if ( -6 < playerAck.tf )
			{
				playerAck.tf--;
				m_pCurRound->log.push_back(playerAck.pCurPet->nick + "���������������");
			}
		}
		if ( "�����ֶ�" == playerAck.pSkill->name )//��������������
		{
			if ( -6 < playerAck.wf )
			{
				playerAck.wf -=2;
				if ( -6 > playerAck.wf ) playerAck.wf = -6;
				m_pCurRound->log.push_back(playerAck.pCurPet->nick + "�ķ�������½�");
			}
		}
		if ( "ȼ�մ���" == playerAck.pSkill->name 
			|| "Ҷ�籩" == playerAck.pSkill->name 
			|| "������ѹ" == playerAck.pSkill->name 
			|| "����Ⱥ" == playerAck.pSkill->name 
			)//������������ع�
		{
			if ( -6 < playerAck.tg )
			{
				playerAck.tg -=2;
				if ( -6 > playerAck.tg ) playerAck.tg = -6;
				m_pCurRound->log.push_back(playerAck.pCurPet->nick + "�����⹥������½�");
			}
		}			
	}

	return true;
}

bool Battle::ImmuneState(Battle::WARRIOR &player)
{
	if ( 0 < player.tiShen ) return true;
	if ( "�׷�" == player.pTalent->name ) return true;
	if ( ("ʪ������" == player.pTalent->name && Race::shui == m_weather) ) return true;
	if ( ("ҶƬ����" == player.pTalent->name && Race::huo == m_weather) ) return true;
	if ( 0 != player.pCurPet->state ) return true;

	return false;
}

bool Battle::Faint(Battle::WARRIOR &player)
{
	if ( player.pCurPet->curHP > 0 ) return false;

	player.pCurPet->curHP = 0;
	m_pCurRound->log.push_back(player.pCurPet->nick + "������");
	if ( 1 == player.pets.size() ) player.lose = true;
	return true;
}

void Battle::PlayerEnd(Battle::WARRIOR &player, Battle::WARRIOR &enemy)
{
	if ( player.isEnd ) return;
	player.isEnd = true;

	if ( 0 < player.doomDesireRound )//����֮Ըʣ��غ�
	{
		player.doomDesireRound--;
		if ( 0 == player.doomDesireRound ) 
		{
			player.pSkill = Skill("����֮Ը", m_game->SkillBook());
			if ( NULL != player.pSkill )
			{
				UseSkill(player, enemy);
				if (WaitPlayerCMD()) return;
			}
		}
	}
	if ( 0 < player.predictRound )//Ԥ֪δ��ʣ��غ�
	{
		player.predictRound--;
		if ( 0 == player.predictRound ) 
		{
			player.pSkill = Skill("Ԥ֪δ��", m_game->SkillBook());
			if ( NULL != player.pSkill )
			{
				UseSkill(player, enemy);
				if (WaitPlayerCMD()) return;
			}
		}
	}

	if ( player.seed )//����
	{
		m_pCurRound->log.push_back(enemy.pCurPet->nick + "������" 
			+ player.pCurPet->nick + "������");
		int hurt = player.pCurPet->HP / 8;
		hurt = hurt > player.pCurPet->curHP?player.pCurPet->curHP:hurt;
		hurt = Hurt(player, hurt);
		enemy.pCurPet->curHP += hurt;
		if ( 0 >= player.pCurPet->curHP ) return;//����
	}

	if ( 0 < player.mieGe )//����������
	{
		player.mieGe--;
		if ( 2 == player.mieGe ) m_pCurRound->log.push_back(player.pCurPet->nick + "ֻʣ��2�غ�����");
		if ( 1 == player.mieGe ) m_pCurRound->log.push_back(player.pCurPet->nick + "ֻʣ��1�غ�����");
		if ( 0 == player.mieGe )
		{
			player.pCurPet->curHP = 0;
			Faint(player);
			return;
		}
	}

	if ( -1 != player.haQian )//���˹�Ƿ
	{
		player.haQian--;
		if ( -1 == player.haQian )
		{
			player.pCurPet->state = Race::pu;
			player.pCurPet->sleepRound = player.rp.sleepRound;
			m_pCurRound->log.push_back(player.pCurPet->nick + "˯����");
			player.pCurPet->sleepRound--;
		}
	}
	//�������ܻغ�����:0δ������>0����,-1ר��/�����ֶ�
	if ( 0 < player.lockSkillTime )
	{
		player.lockSkillTime--;
		if ( 0 == player.lockSkillTime )
		{
			if ( player.guZhang )
			{
				player.guZhang = false;
				m_pCurRound->log.push_back(player.pCurPet->nick + "�Ĺ��ƽ�����");
			}
		}
	}
	if ( 0 < player.tiaoDou ) //����ʣ��غ�
	{
		player.tiaoDou--;
		if ( 0 == player.tiaoDou )
		{
			m_pCurRound->log.push_back(player.pCurPet->nick + "��������Ӱ����");
		}
	}
	if ( 0 < player.pCurPet->sleepRound ) player.pCurPet->sleepRound--;//����ʣ��غ�
	if ( 0 < player.pCurPet->frozenRound ) player.pCurPet->frozenRound--;//����ʣ��غ�
	if ( 0 < player.hunLuan ) player.hunLuan--;//����ʣ��غ���
	if ( 0 < player.luanWu )//����ʣ��غ�
	{
		player.luanWu--;
		if ( 0 == player.luanWu )
		{
			m_pCurRound->log.push_back(player.pCurPet->nick + "ֹͣ������");
		}
	}

	if ( 0 != player.pCurPet->state && "��Ƥ" == player.pTalent->name && player.rp.tePro <= 30 )
	{
		player.pCurPet->state = 0;
		m_pCurRound->log.push_back(player.pCurPet->nick + "��Ƥ������쳣");
	}
	if ( "ħ������" != player.pTalent->name )
	{
		if ( Race::bing == m_weather 
			&& Race::bing != player.pBuddy->race1
			&& Race::bing != player.pBuddy->race2 ) 
		{
			m_pCurRound->log.push_back(player.pCurPet->nick + "�ܵ������˺�");
			Hurt(player, player.pCurPet->HP / 16);
		}
		else if ( Race::huo == m_weather && "̫������" == player.pTalent->name )
		{
			m_pCurRound->log.push_back(player.pCurPet->nick + "�ܵ�̫����������");
			Hurt(player, player.pCurPet->HP / 8);
		}
		else if ( Race::yan == m_weather
			&& Race::di != player.pBuddy->race1
			&& Race::di != player.pBuddy->race2
			&& Race::yan != player.pBuddy->race1
			&& Race::yan != player.pBuddy->race2
			&& Race::gang != player.pBuddy->race1
			&& Race::gang != player.pBuddy->race2
			&& "��ɳ" != player.pTalent->name ) 
		{
			m_pCurRound->log.push_back(player.pCurPet->nick + "�ܵ�ɳ�����˺�");
			Hurt(player, player.pCurPet->HP / 16);
		}
		if ( 0 >= player.pCurPet->curHP ) return;//����

		if ( Race::huo == player.pCurPet->state )
		{
			m_pCurRound->log.push_back(player.pCurPet->nick + "������");
			Hurt(player, player.pCurPet->HP / 8);
		}
		if ( 0 >= player.pCurPet->curHP ) return;//����

		if ( Race::du == player.pCurPet->state
			&& "����" != player.pTalent->name )
		{
			m_pCurRound->log.push_back(player.pCurPet->nick + "�ж���");
			player.duRound++;
			Hurt(player, player.pCurPet->HP * player.duRound / 8);
		}
		if ( 0 >= player.pCurPet->curHP ) return;//����
	}

	if ( 6 > player.sd )
	{
		if ( "����" == player.pTalent->name )
		{
			player.sd++;
			m_pCurRound->log.push_back(player.pCurPet->nick + "���ٶ�������");
		}
	}
	if ( player.pCurPet->curHP < player.pCurPet->HP )
	{
		if ( 0 < player.recvHP )//����Һ
		{
			player.pCurPet->curHP += player.pCurPet->HP/16;
			m_pCurRound->log.push_back(player.pCurPet->nick + "�����˼���Һ,�ָ�������");
		}
		if ( Race::shui == m_weather && "����" == player.pTalent->name ) 
		{
			player.pCurPet->curHP += player.pCurPet->HP/16;
			m_pCurRound->log.push_back(player.pCurPet->nick + "���˿���ˮ,�ָ�������");
		}
		if ( Race::du == player.pCurPet->state && "����" == player.pTalent->name )
		{
			m_pCurRound->log.push_back(player.pCurPet->nick + "�����˶���,�ָ�������");
			player.pCurPet->curHP += player.pCurPet->HP / 8;
		}
		if ( "ʣ��" == player.pItem->name )
		{
			player.pCurPet->curHP += player.pCurPet->HP/16;
			m_pCurRound->log.push_back(player.pCurPet->nick + "���˿�ʣ��,�ָ�������");
		}
		if ( player.pCurPet->curHP > player.pCurPet->HP )
		{
			player.pCurPet->curHP = player.pCurPet->HP;
		}
	}
	if ( 0 < player.banChangeRound )//��ֹ����
	{
		player.banChangeRound--;
	}

	if ( 0 < player.wall[0] )
	{
		player.wall[0]--;
		if ( 0 == player.wall[0] ) m_pCurRound->log.push_back(player.name + "�ķ������ʧ��");
	}
	if ( 0 < player.wall[1] )
	{
		player.wall[1]--;
		if ( 0 == player.wall[1] ) m_pCurRound->log.push_back(player.name + "�Ĺ�֮ǽ��ʧ��");
	}
	player.tongGui = false;//��ͬ��
	player.fear = false;//����
}

Battle::WARRIOR* Battle::Player(bool me)
{
	if ( me ) return &m_player;
	return &m_enemy;
}
