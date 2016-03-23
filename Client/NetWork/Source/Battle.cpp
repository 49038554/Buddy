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
	m_player.recvHP = 0;//�ظ�HP
	m_player.sleep = 0;//����ʣ��غ�
	m_player.frozen = 0;//����ʣ��غ�
	m_player.seed = false;//����
	m_player.haQian = -1;//���˹�Ƿ
	m_player.mieGe = -1;//����������
	m_player.tiShen = 0;//��������
	m_player.xunXing = false;//��Ѱ��
	m_player.tiaoDou = -1;//�����غ�
	m_player.ban = false;//�з�ӡ
	m_player.tongGui = false;//��ͬ��
	m_player.hunLuan = -1;//����ʣ��غ���
	m_player.isActioned = false;//�Ѿ��ж�
	m_player.lose = false;//ʧ��
	m_enemy.pCurPet = NULL;
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
	m_enemy.recvHP = 0;//�ظ�HP
	m_enemy.sleep = 0;//����ʣ��غ�
	m_enemy.frozen = 0;//����ʣ��غ�
	m_enemy.seed = false;//����
	m_enemy.haQian = -1;//���˹�Ƿ
	m_enemy.mieGe = -1;//����������
	m_enemy.tiShen = 0;//��������
	m_enemy.xunXing = false;//��Ѱ��
	m_enemy.tiaoDou = -1;//�����غ�
	m_enemy.ban = false;//�з�ӡ
	m_enemy.tongGui = false;//��ͬ��
	m_enemy.hunLuan = -1;//����ʣ��غ���
	m_enemy.isActioned = false;//�Ѿ��ж�
	m_enemy.lose = false;//ʧ��

	for ( i = 0; i < 18; i++ ) m_player.nail[i] = m_player.race[i] = m_enemy.nail[i] = m_enemy.race[i] = false;
	m_player.wall[0] = 0;
	m_player.wall[1] = 0;
	m_enemy.wall[0] = 0;
	m_enemy.wall[1] = 0;
	m_player.lookSkill.clear();
	m_enemy.lookSkill.clear();
	
	m_weather = 0;
	m_weatherCount = 0;

	return true;
}

const char* Battle::PlayerRand(bool me, Battle::Action act, short objectId, Battle::RAND_PARAM &rp)
{
	static std::string reason;
	Battle::WARRIOR &player = me?m_player:m_enemy;
	Battle::WARRIOR &enemy = me?m_enemy:m_player;
	if ( player.isReady ) return NULL;

	if ( Battle::attack == act ) 
	{
		if ( 0 != player.lockSkill && 0 != player.lockSkillTime 
			&& objectId != player.lockSkill ) return (reason = "���ܸ�������").c_str();
		if ( player.xunXing && objectId == player.lockSkill ) return (reason = "��Ѱ�ƣ���������ʹ����ͬ����").c_str();
		player.pSkill = Skill(objectId, m_game->SkillBook());
		if ( NULL == player.pSkill ) return (reason = "���ܲ�����").c_str();
		if ( 0 != player.tiaoDou && 2 == player.pSkill->type ) return (reason = "��������ֻ��ʹ�ù�������").c_str();
	}
	if ( Battle::useItem == act ) 
	{
		player.pItem = Item(objectId, m_game->ItemBook());
		if ( NULL == player.pItem ) return (reason = "��Ʒ������").c_str();
	}
	if ( Battle::change == act ) 
	{
		if ( !player.changePetAble ) return (reason = "���ܸ����͵�").c_str();
		int i = 0;
		for ( i = 0; i < player.pets.size(); i++ )
		{
			if ( objectId == player.pets[i].id )
			{
				player.pBuddy = Buddy(player.pets[i].number, m_game->BuddyBook());
				if ( NULL == player.pBuddy ) return (reason = "�͵ϲ�����").c_str();
				player.pTalent = Talent(player.pets[i].talent, m_game->TalentBook());
				if ( NULL == player.pTalent ) return (reason = "���Բ�����").c_str();
				if ( 0 >= player.pets[i].curHP ) return (reason = "�͵���ʧȥս������").c_str();
			}
		}
	}

	rp.miss = rand()%100 + 1;//���������
	rp.sePro = rand()%100 + 1;//������Ч�����
	rp.iePro = rand()%100 + 1;//��Ʒ��Ч�����
	rp.tePro = rand()%100 + 1;//������Ч�����
	rp.luanWu = rand()%2 + 2;//����غ���
	rp.sleep = rand()%100 + 1;//˯�������
	rp.ice = rand()%100 + 1;//���������
	rp.luan = rand()%2;//���������
	rp.hurt = rand()%(255 - 217 + 1) + 217;//�˺������217~255
	rp.speed = rand()%100;//�ٶ������

	return NULL;
}

bool Battle::PlayerAction(bool me, Battle::Action act, short objectId, Battle::RAND_PARAM &rp)
{
	Battle::WARRIOR &player = me?m_player:m_enemy;
	Battle::WARRIOR &enemy = me?m_enemy:m_player;
	if ( player.isReady ) return false;

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
	PlayRound();

	return true;
}

bool Battle::PlayRound()
{
	m_pCurRound = &m_log[m_curRound];
	StepStart();
	if ( !StepChange() ) return false;
	if ( !StepAttack() ) return false;
	if ( !StepEnd() ) return false;
	m_player.isReady = m_enemy.isReady = false;

	return true;
}

void Battle::StepStart()
{
	m_pCurRound->log.push_back("�غϿ�ʼ");
	m_player.isActioned = false;
	m_enemy.isActioned = false;
}

bool Battle::StepChange()
{
	Battle::ChangeResult meChanged = ChangePet(m_player);
	if ( Battle::faint == meChanged ) return false; 
	Battle::ChangeResult sheChanged = ChangePet(m_enemy);
	if ( Battle::faint == sheChanged ) return false;

	if ( Battle::finished == meChanged ) EntryStage(m_player, m_enemy);
	if ( Battle::finished == sheChanged ) EntryStage(m_enemy, m_player);

	return true;
}

bool Battle::StepAttack()
{
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
	if ( Battle::attack == m_player.act && ActionAble(m_enemy) 
		&& ( "����ȭ" == m_enemy.pSkill->name || "�㱼����ȭ" == m_enemy.pSkill->name ) )
	{
		if ( "�㱼����ȭ" == m_player.pSkill->name ) 
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
			if ( PetAction(m_player, m_enemy) ) return false;
		}
		if ( Battle::attack == m_player.act && !m_player.isActioned )
		{
			if ( PetAction(m_enemy, m_player) ) return false;
		}
	}
	else 
	{
		if ( Battle::attack == m_player.act && !m_player.isActioned )
		{
			if ( PetAction(m_enemy, m_player) ) return false;
		}
		if ( Battle::attack == m_player.act && !m_player.isActioned )
		{
			if ( PetAction(m_player, m_enemy) ) return false;
		}
	}

	return true;
}

bool Battle::StepEnd()
{
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
	if ( 0 < m_player.wall[0] )
	{
		m_player.wall[0]--;
		if ( 0 == m_player.wall[0] ) m_pCurRound->log.push_back("�ҷ��������ʧ��");
	}
	if ( 0 < m_player.wall[1] )
	{
		m_player.wall[1]--;
		if ( 0 == m_player.wall[1] ) m_pCurRound->log.push_back("�ҷ��������ʧ��");
	}
	if ( 0 < m_enemy.wall[0] )
	{
		m_enemy.wall[0]--;
		if ( 0 == m_enemy.wall[0] ) m_pCurRound->log.push_back("�Է��������ʧ��");
	}
	if ( 0 < m_enemy.wall[1] )
	{
		m_enemy.wall[1]--;
		if ( 0 == m_enemy.wall[1] ) m_pCurRound->log.push_back("�Է��������ʧ��");
	}
	m_pCurRound->log.push_back("�غϽ���");
	return true;
}

void Battle::EntryStage(Battle::WARRIOR &player, Battle::WARRIOR &enemy)
{
	if ( "����" == player.pTalent->name ) 
	{
		enemy.wg = -1;
		m_pCurRound->log.push_back("�������﹥");
	}
	if ( "����" == player.pTalent->name ) 
	{
		m_weather = Race::shui;
		m_weatherCount = -1;
		m_pCurRound->log.push_back("������");
	}
	if ( "��ѩ" == player.pTalent->name ) 
	{
		m_weather = Race::bing;
		m_weatherCount = -1;
		m_pCurRound->log.push_back("��ѩ��");
	}
	if ( "�ɺ�" == player.pTalent->name ) 
	{
		m_weather = Race::huo;
		m_weatherCount = -1;
		m_pCurRound->log.push_back("̫��������");
	}
	if ( "��ɳ" == player.pTalent->name ) 
	{
		m_weather = Race::yan;
		m_weatherCount = -1;
		m_pCurRound->log.push_back("ɳ��������");
	}
	if ( "��֮��" == player.pTalent->name && Race::huo == m_weather) 
	{
		player.tg += 1;
		player.tf += 1;
	}
	player.recvHP = 0;
}

void Battle::LeaveStage(Battle::WARRIOR &player)
{
	m_pCurRound->log.push_back("�ջ�");
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

	if ( "��Ȼ�ظ�" == player.pTalent->name ) //
	{
		player.pCurPet->state = 0;
		m_pCurRound->log.push_back("�ָ�������״̬");
	}
	if ( "����" == player.pTalent->name ) //
	{
		player.pCurPet->curHP += player.pCurPet->HP / 3;
		m_pCurRound->log.push_back("�ָ���1/3��HP");
	}
	player.pCurPet = NULL;
}

bool Battle::Hurt(Battle::WARRIOR &player, int HP)
{
	char strHP[256];
	sprintf( strHP, "%d%%", HP/player.pCurPet->HP );
	m_pCurRound->log.push_back("��" + player.pCurPet->nick + "�����" + strHP + "�˺�");
	player.pCurPet->curHP -= HP;
	if ( player.pCurPet->curHP <= 0 )
	{
		player.pCurPet->curHP = 0;
		m_pCurRound->log.push_back(player.pCurPet->nick + "������");
		if ( 1 == player.pets.size() ) player.lose = true;
		return true;
	}
	if ( player.pCurPet->curHP > player.pCurPet->HP/3 ) return false;
	if ( 5 == player.pCurPet->itemId ) 
	{
		player.wgCorrect = 1.5;
		m_pCurRound->log.push_back(player.pCurPet->nick + "�����˷ܼ�-��");
	}
	if ( 6 == player.pCurPet->itemId ) 
	{
		player.tgCorrect = 1.5;
		m_pCurRound->log.push_back(player.pCurPet->nick + "�����˷ܼ�-�ع�");
	}
	if ( 7 == player.pCurPet->itemId )
	{
		player.sdCorrect = 1.5;
		m_pCurRound->log.push_back(player.pCurPet->nick + "�����˷ܼ�-�ٶ�");
	}

	return false;
}

Battle::ChangeResult Battle::ChangePet(Battle::WARRIOR &player, int petId)
{
	if ( Battle::change != player.act && NULL != m_player.pCurPet && 0 == petId ) return Battle::unchange;
	player.isActioned = true;//�Ѿ��ж�

	int id;
	if ( 0 < petId ) id = petId;
	else if ( Battle::change == player.act ) id = player.objId;
	else id = player.pets[0].id;

	int i = 0;
	if ( NULL != player.pCurPet && player.pCurPet->id != id )
	{
		LeaveStage(player);
	}
	if ( NULL != player.pCurPet ) return Battle::unchange;

	for ( i = 0; i < player.pets.size(); i++ )
	{
		if ( id == player.pets[i].id )
		{
			player.pCurPet = &player.pets[i];
			m_pCurRound->log.push_back("�ų�");
			break;
		}
	}
	if ( NULL == player.pCurPet ) return Battle::unchange;

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
	player.lookSkill.clear();
	player.changePetAble = true;
	player.lockSkill = 0;
	player.lockSkillTime = ("ר��ͷ��" == player.pItem->name?-1:0);
	player.smell = false;
	player.recvHP = ("ʣ��" == player.pItem->name?player.pCurPet->HP/16:0);//�ظ�HP
	player.sleep = 0;//����ʣ��غ�
	player.frozen = 0;//����ʣ��غ�
	player.seed = false;//����
	player.haQian = -1;//���˹�Ƿ
	player.mieGe = -1;//����������
	player.tiShen = 0;//��������
	player.xunXing = false;//��Ѱ��
	player.tiaoDou = -1;//�����غ�
	player.ban = false;//�з�ӡ
	player.tongGui = false;//��ͬ��
	player.hunLuan = -1;//����ʣ��غ���

	for ( i = 0; i < 18; i++ ) player.race[i] = false;	
	if ( player.nail[13] )
	{
		if ( Race::fei != player.pBuddy->race1
			&& Race::fei != player.pBuddy->race2  
			&& "ħ������" != player.pTalent->name 
			&& "����" != player.pTalent->name ) 
		{
			if ( Hurt(player, player.pCurPet->HP / 16) )  return Battle::faint;
		}
	}
	if ( "����" == player.pItem->name )
	{
		player.pCurPet->state = Race::du;
		m_pCurRound->log.push_back( m_player.pCurPet->nick + "Я�������ж���" );
	}
	else if ( "����" == player.pItem->name )
	{
		player.pCurPet->state = Race::huo;
		m_pCurRound->log.push_back( m_player.pCurPet->nick + "Я������������" );
	}

	return Battle::finished;
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
	if ( "Ʈ����ѩ���㴵" == player.pSkill->name ) return true;
	if ( "����" == player.pSkill->name ) return true;
	if ( "���" == player.pSkill->name ) return true;

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

int Battle::CalWG(Battle::WARRIOR &player, Battle::WARRIOR &enemy)
{
	int level = player.wg;
	if ( "��Ȼ" == player.pTalent->name && 0 > level ) level = 0;
	if ( "��Ȼ" == enemy.pTalent->name && 0 < level ) level = 0;
	int WG = 0;
	if ( level > 0 ) WG = player.pCurPet->WG * (1 + level * 0.5) * player.wgCorrect;
	else if ( level < 0 ) WG = player.pCurPet->WG / (1 + level * 0.5 * -1) * player.wgCorrect;
	else WG = player.pCurPet->WG * player.wgCorrect;

	if ( "����" == player.pTalent->name ) WG *= 2;
	else if ( "������̨" == player.pTalent->name ) WG *= 1.2;
	else if ( "����" == player.pTalent->name && 0 != player.pCurPet->state ) WG *= 1.5;
	if ( 2 == player.pCurPet->state && "����" != player.pTalent->name ) WG /= 2;

	return WG;
}

int Battle::CalWF(Battle::WARRIOR &player, Battle::WARRIOR &enemy)
{
	int level = player.wf;
	if ( "��Ȼ" == player.pTalent->name && 0 > level ) level = 0;
	if ( "��Ȼ" == enemy.pTalent->name && 0 < level ) level = 0;
	int WF = 0;
	if ( level > 0 ) WF = player.pCurPet->WF * (1 + level * 0.5);
	else if ( level < 0 ) WF = player.pCurPet->WF / (1 + level * 0.5 * -1);
	else WF = player.pCurPet->WF;

	if ( "������Ƭ" == player.pTalent->name && 0 != player.pCurPet->state ) WF *= 1.5;

	return WF;
}

int Battle::CalTG(Battle::WARRIOR &player, Battle::WARRIOR &enemy)
{
	int level = player.tg;
	if ( "��Ȼ" == player.pTalent->name && 0 > level ) level = 0;
	if ( "��Ȼ" == enemy.pTalent->name && 0 < level ) level = 0;
	int TG = 0;
	if ( level > 0 ) TG = player.pCurPet->TG * (1 + level * 0.5) * player.tgCorrect;
	else if ( level < 0 ) TG = player.pCurPet->TG / (1 + level * 0.5 * -1) * player.tgCorrect;
	else TG = player.pCurPet->TG * player.tgCorrect;

	if ( "̫������" == player.pTalent->name && Race::huo == m_weather ) TG *= 1.5;

	return TG;
}

int Battle::CalTF(Battle::WARRIOR &player, Battle::WARRIOR &enemy)
{
	int level = player.tf;
	if ( "��Ȼ" == player.pTalent->name && 0 > level ) level = 0;
	if ( "��Ȼ" == enemy.pTalent->name && 0 < level ) level = 0;
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

int Battle::CalPower(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef)
{
	int power = playerAck.pSkill->power;
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
		if ( Race::pu == playerDef.pSkill->race 
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
		playerAck.rp.sePro = false;
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
		&& "��͸" != playerAck.pTalent->name ) power /= 2;
	if ( 0 < playerDef.wall[1] && 3 == playerAck.pSkill->type 
		&& "��͸" != playerAck.pTalent->name ) power /= 2;
	//////////////////////////////////////////////////////////////////////////
	//��������
	if ( "��֬��" == playerDef.pTalent->name 
		&& (Race::huo == skillRace || Race::bing == skillRace) ) power /= 2;
	if ( "ʥ��ʿ" == playerDef.pTalent->name ) 
	{
		if ( playerDef.lookSkill[playerAck.pSkill->id] ) 
		{
			power *= 0;
			m_pCurRound->log.push_back("��ʥ��ʿ����1�εļ�������Ч��");
		}
		else playerDef.lookSkill[playerAck.pSkill->id] = true;
	}

	return power;
}

bool Battle::PetAction(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef)
{
	playerAck.isActioned = true;
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

	if ( 2 == playerAck.pSkill->type ) return UseHelpSkill(playerAck, playerDef);

	int G = 0, F = 0, Power = 0;
	if ( 1 == playerAck.pSkill->type ) 
	{
		G = CalWG(playerAck, playerDef);
		F = CalWF(playerDef, playerAck);
	}
	else if ( 3 == playerAck.pSkill->type )
	{
		G = CalTG(playerAck, playerDef);
		if ( "������" == playerAck.pSkill->name ) F = CalWF(playerDef, playerAck);
		else F = CalTF(playerDef, playerAck);
	}
	Power = CalPower(playerAck, playerDef);
	if ( 0 == Power ) return false;

	int shanghai = G*Power/F*0.84*playerAck.rp.hurt/255;
	return Hurt(playerDef, shanghai);
}

bool Battle::UseHelpSkill(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef)
{
	if ( 0 < playerAck.tiaoDou )
	{
		m_pCurRound->log.push_back("����������ʹ�ø������ܣ�ʧ����");
		return false;
	}
	if ( playerAck.rp.miss > playerAck.pSkill->hitRate)
	{
		m_pCurRound->log.push_back("����Miss��");
		return false;
	}
	if ( "����" == playerAck.pSkill->name )//		��	0	2	101	0	5�غ�����
	{
		if ( Race::huo == m_weather )
		{
			m_pCurRound->log.push_back("�Ѿ������죬ʧ����");
			return false;
		}
		m_weather = Race::huo;//�������Զ�Ӧ
		m_weatherCount = 5;//����ʣ��غ��� -1����
		m_pCurRound->log.push_back("̫��������");
		return false;
	}
	if ( "����" == playerAck.pSkill->name )//		ˮ	0	2	101	0	5�غ�����
	{
		if ( Race::shui == m_weather )
		{
			m_pCurRound->log.push_back("�Ѿ������꣬ʧ����");
			return false;
		}
		m_weather = Race::shui;//�������Զ�Ӧ
		m_weatherCount = 5;//����ʣ��غ��� -1����
		m_pCurRound->log.push_back("������");
		return false;
	}
	if ( "����" == playerAck.pSkill->name )//		��	0	2	101	0	5�غϱ�������������������İ͵�ÿ�غϿ�ʼ�ܵ�1/16�˺�
	{
		if ( Race::bing == m_weather )
		{
			m_pCurRound->log.push_back("�Ѿ�����ѩ��ʧ����");
			return false;
		}
		m_weather = Race::bing;//�������Զ�Ӧ
		m_weatherCount = 5;//����ʣ��غ��� -1����
		m_pCurRound->log.push_back("��ѩ��");
	}
	if ( "ɳ��" == playerAck.pSkill->name )//		��	0	2	101	0	5�غ�ɳ�����������Ҹ��������԰͵��ܵ�1/16�˺�
	{
		if ( Race::yan == m_weather )
		{
			m_pCurRound->log.push_back("�Ѿ���ɳ�����У�ʧ����");
			return false;
		}
		m_weather = Race::yan;//�������Զ�Ӧ
		m_weatherCount = 5;//����ʣ��غ��� -1����
		m_pCurRound->log.push_back("ɳ��������");
		return false;
	}
	if ( "��֮��" == playerAck.pSkill->name )//		��	0	2	101	0	5�غ������˺�����
	{
		if ( 0 < playerAck.wall[1] )
		{
			m_pCurRound->log.push_back("�Ѿ����˹�֮�ڣ�ʧ����");
			return false;
		}
		playerAck.wall[1] = 5;
		m_pCurRound->log.push_back("�ҷ��ܵ���֮�ڱ�����5�غ��������˺�����");
		return false;
	}
	if ( "�����" == playerAck.pSkill->name )//		��	0	2	101	0	5�غ������˺�����
	{
		if ( 0 < playerAck.wall[0] )
		{
			m_pCurRound->log.push_back("�Ѿ����˷���ܣ�ʧ����");
			return false;
		}
		playerAck.wall[0] = 5;
		m_pCurRound->log.push_back("�ҷ��ܵ�����ܱ�����5�غ��������˺�����");
		return false;
	}
	if ( "��ƭ�ռ�" == playerAck.pSkill->name )//		��	0	2	101	0	5�غ��ٶ��������ж�
	{
		if ( 0 != m_foolSpace )
		{
			m_pCurRound->log.push_back("�Ѿ�������ƭ�ռ䣬ʧ����");
			return false;
		}
		m_foolSpace = 5;
		m_pCurRound->log.push_back("����ת��Ϊ��ƭ�ռ䣬5�غ����ٶȵ͵����ж�");
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
		return false;
	}

	if ( "����" == playerAck.pSkill->name )//		��	0	2	101	0	ȫ�������ȼ���ԭ
	{
		playerAck.wg = playerDef.wg = 0;//��ǿ���ȼ�
		playerAck.wf = playerDef.wf = 0;//��ǿ���ȼ�
		playerAck.tg = playerDef.tg = 0;//�ع�ǿ���ȼ�
		playerAck.tf = playerDef.tf = 0;//�ط�ǿ���ȼ�
		playerAck.sd = playerDef.sd = 0;//�ٶ�ǿ���ȼ�
		playerAck.ct = playerDef.ct = 0;//����ǿ���ȼ�
		m_pCurRound->log.push_back("˫�������仯������");
		return false;
	}
	if ( "�Գ���" == playerAck.pSkill->name )//		��	0	2	101	0	�����﹥
	{
		if ( playerAck.pCurPet->curHP < playerAck.pCurPet->HP / 2 )
		{
			m_pCurRound->log.push_back("�������㣬ʧ����");
			return false;
		}
		m_pCurRound->log.push_back("��ʧһ���������������������������");
		playerAck.wg = 6;
		Hurt(playerAck, playerAck.pCurPet->HP / 2);
		return false;
	}
	if ( "����" == playerAck.pSkill->name )//		��	0	2	101	0	���ǿ���﹥
	{
		if ( 6 == playerAck.wg )
		{
			m_pCurRound->log.push_back("�������Ѵﵽ���ޣ�ʧ����");
			return false;
		}
		playerAck.wg += 2;
		m_pCurRound->log.push_back("�������������");
		return false;
	}
	if ( "Ӳ���" == playerAck.pSkill->name )//		�� 	0	2	100	0	���ǿ�����
	{
		if ( 6 == playerAck.wf )
		{
			m_pCurRound->log.push_back("��������Ѵﵽ���ޣ�ʧ����");
			return false;
		}
		if ( 6 > playerAck.wf  )
		{
			playerAck.wf += 2;
			m_pCurRound->log.push_back("��������������");
		}
		return false;
	}
	if ( "ө��" == playerAck.pSkill->name )//		��	0	2	101	0	���ǿ���ع�
	{
		if ( 6 == playerAck.tg )
		{
			m_pCurRound->log.push_back("���⹥���Ѵﵽ���ޣ�ʧ����");
			return false;
		}
		if ( 6 > playerAck.tg  )
		{
			playerAck.tg += 2;
			m_pCurRound->log.push_back("���⹥���������");
		}
		return false;
	}
	if ( "��������" == playerAck.pSkill->name )//		��	0	2	101	0	���ǿ���ط�
	{
		if ( 6 == playerAck.tf )
		{
			m_pCurRound->log.push_back("��������Ѵﵽ���ޣ�ʧ����");
			return false;
		}
		if ( 6 > playerAck.tf  )
		{
			playerAck.tf += 2;
			m_pCurRound->log.push_back("��������������");
		}
		return false;
	}
	if ( "�����ƶ�" == playerAck.pSkill->name )//		��	0	2	101	0	���ǿ���ٶ�
	{
		if ( 6 == playerAck.sd )
		{
			m_pCurRound->log.push_back("�ٶ��Ѵﵽ���ޣ�ʧ����");
			return false;
		}
		if ( 6 > playerAck.sd  )
		{
			playerAck.sd += 2;
			m_pCurRound->log.push_back("�ٶȴ������");
		}
		return false;
	}
	if ( "�޴�" == playerAck.pSkill->name )//		��	0	2	101	0	ǿ���﹥��ǿ�����
	{
		if ( 6 == playerAck.wg && 6 == playerAck.wf )
		{
			m_pCurRound->log.push_back("����������������Ѵﵽ���ޣ�ʧ����");
			return false;
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
		return false;
	}
	if ( "ڤ��" == playerAck.pSkill->name )//		��	0	2	101	0	ǿ���ع���ǿ���ط�
	{
		if ( 6 == playerAck.tg && 6 == playerAck.tf )
		{
			m_pCurRound->log.push_back("���⹥������������Ѵﵽ���ޣ�ʧ����");
			return false;
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
		return false;
	}
	if ( "ĥצ��" == playerAck.pSkill->name )//		��	0	2	101	0	ǿ���﹥����߱�����
	{
		if ( 6 == playerAck.wg && 6 == playerAck.ct )
		{
			m_pCurRound->log.push_back("ǿ���Ѵﵽ���ޣ�ʧ����");
			return false;
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
		return false;
	}
	if ( "���" == playerAck.pSkill->name )//		��	0	2	101	0	ǿ���ع�����ϵ����*1.5
	{
		if ( 6 == playerAck.tg && playerAck.race[Race::dian] )
		{
			m_pCurRound->log.push_back("ǿ���Ѵﵽ���ޣ�ʧ����");
			return false;
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
		return false;
	}
	if ( "��֮��" == playerAck.pSkill->name )//		��	0	2	101	0	ǿ���﹥��ǿ���ٶ�
	{
		if ( 6 == playerAck.wg && 6 == playerAck.sd )
		{
			m_pCurRound->log.push_back("���������ٶ��Ѵﵽ���ޣ�ʧ����");
			return false;
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
		return false;
	}
	if ( "��֮��" == playerAck.pSkill->name )//		��	0	2	101	0	ǿ���ع���ǿ���ط���ǿ���ٶ�
	{
		if ( 6 == playerAck.tg && 6 == playerAck.tf && 6 == playerAck.sd )
		{
			m_pCurRound->log.push_back("���⹥��������������ٶ��Ѵﵽ���ޣ�ʧ����");
			return false;
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
		return false;
	}

	if ( "����" == playerAck.pSkill->name //		��	0	2	101	0	�ظ�50%����
		|| "����" == playerAck.pSkill->name ) //		��	0	2	101	0	�ظ�50%����
	{
		playerAck.pCurPet->curHP += playerAck.pCurPet->HP / 2;
		m_pCurRound->log.push_back("�ظ�������");
		return false;
	}
	if ( "�������" == playerAck.pSkill->name //		��	0	2	101	0	�ظ�50%HP,����2����������������
		|| "����" == playerAck.pSkill->name //		��	0	2	101	0	�ظ�50%HP,����2����������������
		|| "�¹�" == playerAck.pSkill->name )//		��	0	2	101	0	�ظ�50%HP,����2����������������
	{
		if ( 0 == m_weather ) playerAck.pCurPet->curHP += playerAck.pCurPet->HP / 2;
		else if ( Race::huo == m_weather ) playerAck.pCurPet->curHP += playerAck.pCurPet->HP;
		else if ( Race::huo != m_weather ) playerAck.pCurPet->curHP += playerAck.pCurPet->HP / 4;
		m_pCurRound->log.push_back("�ظ�������");
		return false;
	}
	if ( "����" == playerAck.pSkill->name )//		��	0	2	101	0	��������쳣
	{
		if ( 0 == playerAck.pCurPet->state )
		{
			m_pCurRound->log.push_back("û���쳣״̬,ʧ����");
			return false;
		}
		playerAck.pCurPet->state = 0;
		m_pCurRound->log.push_back("�ָ��˽���");
		return false;
	}
	if ( "��������" == playerAck.pSkill->name //		��	0	2	101	0	�ҷ�ȫ���쳣���
	|| "�����Ʒ�" == playerAck.pSkill->name )//		��	0	2	101	0	ȫ���쳣���
	{
		int i = 0;
		for ( i = 0; i < playerAck.pets.size(); i++ )
		{
			if ( playerAck.pets[i].curHP <= 0 ) continue;
			playerAck.pets[i].state = 0;
		}
		m_pCurRound->log.push_back("ȫ�Ӱ͵ϻָ��˽���");
		return false;
	}

	if ( "���" == playerAck.pSkill->name //	��	0	2	101	0	�����֣�ǿ�ƶԷ��˳�
		|| "����" == playerAck.pSkill->name )//		��	0	2	101	0	�����֣�ǿ�ƶԷ��˳�
	{
		if ( 1 == playerDef.pets.size() )
		{
			m_pCurRound->log.push_back("�Է�û�п��滻�İ͵�,ʧ����");
			return false;
		}
		int i = 0;
		for ( i = 0; true; i++ )
		{
			if ( i >= playerDef.pets.size() ) i = 0;
			if ( playerDef.pCurPet == &playerDef.pets[i] ) continue;
			if ( 0 == rand()%2 ) continue;
			if ( Battle::faint == ChangePet(playerDef, playerDef.pets[i].id) ) return true;
			return false;
		}
	}
	if ( "����Һ" == playerAck.pSkill->name )//		ˮ	0	2	101	0	5�غϣ�ÿ�غϻظ�1/16HP
	{
		playerAck.recvHP += playerAck.pCurPet->HP / 16;
		m_pCurRound->log.push_back("��Ӫ��Һ��Χ�ţ�ÿ�غϻظ�����");
		return false;
	}
	if ( "�λ�" == playerAck.pSkill->name )//		��	0	2	101	0	˯��ʱ�����ʹ�ü���
	{
		int id = rand()%4 + 1;
		if ( 1 == id && playerAck.pSkill->id != playerAck.pCurPet->skill1 ) id = playerAck.pCurPet->skill1;
		else if ( 2 == id && playerAck.pSkill->id != playerAck.pCurPet->skill2 ) id = playerAck.pCurPet->skill2;
		else if ( 3 == id && playerAck.pSkill->id != playerAck.pCurPet->skill3 ) id = playerAck.pCurPet->skill3;
		else if ( 4 == id && playerAck.pSkill->id != playerAck.pCurPet->skill4 ) id = playerAck.pCurPet->skill4;
		playerAck.pSkill = Skill(id, m_game->SkillBook());
		if ( NULL == playerAck.pSkill )
		{
			m_pCurRound->log.push_back("��ϵ���ܿ��Ի��й�ϵ");
			return false;
		}
		short skill = playerAck.lockSkill;
		bool ret = UseSkill(playerAck, playerDef);
		playerAck.lockSkill = skill;
		return ret;
	}
	if ( "˯��" == playerAck.pSkill->name )//		��	0	2	101	0	˯3�غϣ�HPȫ����״̬���
	{
		playerAck.pCurPet->curHP = playerAck.pCurPet->HP;
		playerAck.pCurPet->state = Race::pu;
		playerAck.sleep = 3;
		m_pCurRound->log.push_back("˯���ˣ��ظ���ȫʢ״̬");
		return false;
	}
	if ( "ĩ�ո�" == playerAck.pSkill->name )//		��	0	2	101	0	�����İ͵ϣ�3�غ��ڲ����£������
	{
		if ( 0 < playerDef.mieGe && 0 < playerAck.mieGe )
		{
			m_pCurRound->log.push_back("û��Ч��");
			return false;
		}
		if ( 0 == playerAck.mieGe ) 
		{
			playerAck.mieGe = 3;
			m_pCurRound->log.push_back("ֻʣ��2�غ�");
		}
		if ( 0 == playerDef.mieGe ) 
		{
			playerDef.mieGe = 3;
			m_pCurRound->log.push_back("ֻʣ��2�غ�");
		}
		return false;
	}
	if ( "����ʹ��" == playerAck.pSkill->name )//		��	0	2	100	0	˫��HPƽ��
	{
		int n = (playerAck.pCurPet->curHP + playerDef.pCurPet->curHP) / 2;
		if ( n >= playerAck.pCurPet->curHP ) playerAck.pCurPet->curHP = n;
		else Hurt(playerAck, playerAck.pCurPet->curHP - n);
		if ( n >= playerDef.pCurPet->curHP ) playerDef.pCurPet->curHP = n;
		else Hurt(playerDef, playerDef.pCurPet->curHP - n);
		m_pCurRound->log.push_back("��Է�ƽ��������");
		return false;
	}
	if ( "����" == playerAck.pSkill->name )//		��	0	2	100	0	��ʧ1/4HP��������
	{
		if ( 0 < playerAck.tiShen )
		{
			m_pCurRound->log.push_back("�Ѿ�������ʧ����");
			return false;
		}
		if ( playerAck.pCurPet->curHP <= playerAck.pCurPet->HP / 4 )
		{
			m_pCurRound->log.push_back("�������㣬ʧ����");
			return false;
		}
		m_pCurRound->log.push_back("����1/4������������һ������");
		playerAck.tiShen = playerAck.pCurPet->HP / 4;
		Hurt(playerAck, playerAck.tiShen);
		return false;
	}

	if ( 0 < playerDef.tiShen )
	{
		m_pCurRound->log.push_back("��������˹���");
		return false;
	}
	if ( "���" == playerAck.pSkill->name )//		��	0	2	95	0	����
	{
		if ( "�׷�" == playerDef.pTalent->name )
		{
			m_pCurRound->log.push_back("�Է������˹���");
			return false;
		}
		if ( "����" == playerDef.pTalent->name )
		{
			playerDef.race[Race::huo] = true;
			m_pCurRound->log.push_back("�Է������˻��棬��ϵ��������");
			return false;
		}
		playerDef.pCurPet->state = Race::huo;
		m_pCurRound->log.push_back("�Է�������");
		return false;
	}
	if ( "��Ų�" == playerAck.pSkill->name )//		��	0	2	100	0	���
	{
		if ( "�׷�" == playerDef.pTalent->name 
			|| "����" == playerDef.pTalent->name
			|| Race::di == playerDef.pBuddy->race1
			|| Race::di == playerDef.pBuddy->race2)
		{
			m_pCurRound->log.push_back("�Է������˹���");
			return false;
		}
		if ( "��������" == playerDef.pTalent->name )
		{
			if ( 6 > playerDef.sd ) playerDef.sd++;
			m_pCurRound->log.push_back("�Է��������������ٶ�����");
			return false;
		}
		playerDef.pCurPet->state = Race::dian;
		m_pCurRound->log.push_back("�Է������");
		return false;
	}
	if ( "��Է�" == playerAck.pSkill->name )//		��	0	2	75	0	���
	{
		if ( "�׷�" == playerDef.pTalent->name 
			|| "����" == playerDef.pTalent->name )
		{
			m_pCurRound->log.push_back("�Է������˹���");
			return false;
		}
		if ( "ʳ��" == playerDef.pTalent->name )
		{
			if ( 6 > playerDef.wg ) playerDef.wg++;
			m_pCurRound->log.push_back("�Է������˹������﹥����");
			return false;
		}
		playerDef.pCurPet->state = Race::dian;
		m_pCurRound->log.push_back("�Է������");
		return false;
	}
	if ( "�綾" == playerAck.pSkill->name )//		��	0	2	85	0	�ж�
	{
		if ( "�׷�" == playerDef.pTalent->name 
			|| "����" == playerDef.pTalent->name 
			|| Race::gang == playerDef.pBuddy->race1
			|| Race::gang == playerDef.pBuddy->race2 )
		{
			m_pCurRound->log.push_back("�Է������˹���");
			return false;
		}
		playerDef.pCurPet->state = Race::du;
		m_pCurRound->log.push_back("�Է��ж���");
		return false;
	}
	if ( "������" == playerAck.pSkill->name //		��	0	2	60	0	���߶Է�
		||"��ħ��" == playerAck.pSkill->name //		��	0	2	75	0	���߶Է�
		||"�ڶ�" == playerAck.pSkill->name //		��	0	2	80	0	���߶Է�
		||"˯�߷�" == playerAck.pSkill->name )//		��	0	2	100	0	���߶Է�
	{
		if ( "�׷�" == playerDef.pTalent->name 
			|| "ʧ��" == playerDef.pTalent->name )
		{
			m_pCurRound->log.push_back("�Է������˹���");
			return false;
		}
		if ( "˯�߷�" == playerAck.pSkill->name && "ʳ��" == playerDef.pTalent->name )
		{
			if ( 6 > playerDef.wg ) playerDef.wg++;
			m_pCurRound->log.push_back("�Է������˹������﹥����");
			return false;
		}
		playerDef.pCurPet->state = Race::pu;
		m_pCurRound->log.push_back("�Է�˯����");
		return false;
	}

	if ( "����" == playerAck.pSkill->name //	��	0	2	101	1	��ֹ���˺����ܡ���ս��ʹ�ÿ����̵���Ʒ������ת��Ϊ������
		|| "����" == playerAck.pSkill->name )//	����	��	0	2	101	0	��ֹ�Է����˺�����
	{
		playerDef.changePetAble = 0;
		m_pCurRound->log.push_back("�Է���������");
		return false;
	}
	if ( "çײ" == playerAck.pSkill->name )//		��	0	2	100	0	�Է�HP���ٵ����Լ���ͬ
	{
		if ( Race::gui == playerDef.pBuddy->race1 )
		{
			if ( "����" != playerAck.pTalent->name && !playerAck.smell )
			{
				m_pCurRound->log.push_back("�Է������˹���");
				return false;
			}
		}
		if ( playerDef.pCurPet->curHP > playerAck.pCurPet->curHP )
		{
			Hurt(playerDef, playerDef.pCurPet->curHP - playerAck.pCurPet->curHP);
		}
		return false;
	}
	if ( "����" == playerAck.pSkill->name )//		��	0	2	100	0	������Ʒ
	{
		if ( 0 == playerAck.pCurPet->itemId || 0== playerDef.pCurPet->itemId )
		{
			m_pCurRound->log.push_back("û����Ʒʧ��");
			return false;
		}
		int id = playerAck.pCurPet->itemId;
		playerAck.pCurPet->itemId = playerDef.pCurPet->itemId;
		playerDef.pCurPet->itemId = id;
		data::ITEM *pItem = playerAck.pItem;
		playerAck.pItem= playerDef.pItem;
		playerDef.pItem = pItem;
		m_pCurRound->log.push_back(playerAck.pCurPet->nick + "�õ���" + playerAck.pItem->name);
		m_pCurRound->log.push_back(playerDef.pCurPet->nick + "�õ���" + playerDef.pItem->name);
		return false;
	}
	if ( "������Ϣ" == playerAck.pSkill->name )//		��	0	2	100	1	��ս��ʹ�ÿ�����Ұ���͵�
	{
		m_pCurRound->log.push_back("û��Ч��");
		return false;
	}
	if ( "���" == playerAck.pSkill->name )//		��	0	2	101	0	�պͶ����Լ��ܿɴ��й�ϵ�͵�
	{
		playerAck.smell = true;
		m_pCurRound->log.push_back("��ϵ���ܿ��Ի��й�ϵ");
		return false;
	}
	if ( "��������" == playerAck.pSkill->name )//		��	0	2	85	0	ÿ�غ�����1/8�������Է����½��
	{
		if ( playerDef.seed || Race::cao == playerDef.pBuddy->race1 || Race::cao == playerDef.pBuddy->race2 )
		{
			m_pCurRound->log.push_back("�Է������˹���");
			return false;
		}
		playerDef.seed = true;
		m_pCurRound->log.push_back("�Է�������������");
		return false;
	}
	if ( "��Ƿ" == playerAck.pSkill->name )//		��	0	2	100	0	�Է��»غϱ�����
	{
		if ( 1 == playerDef.haQian || Race::pu == playerDef.pCurPet->state )
		{
			m_pCurRound->log.push_back("û��Ч��");
			return false;
		}
		playerDef.haQian = 1;
		m_pCurRound->log.push_back("�Է������˯");
		return false;
	}
	if ( "����" == playerAck.pSkill->name )//		��	0	2	100	0	3�غϲ��ܸ�������
	{
		if ( 0 == playerDef.lockSkill )
		{
			m_pCurRound->log.push_back("ʧ����");
			return false;
		}
		playerDef.lockSkillTime = 3;
		m_pCurRound->log.push_back("�ܵ����裬����3�غϲ��ܸ�������");
		return false;
	}
	if ( "Ѱ��" == playerAck.pSkill->name )//		��	0	2	100	0	ʹ�Է���������ʹ����ͬ����
	{
		if ( playerDef.xunXing )
		{
			m_pCurRound->log.push_back("û��Ч��");
			return false;
		}
		playerDef.xunXing = true;
		m_pCurRound->log.push_back("��������ʹ����ͬ����");
		return false;
	}
	if ( "����" == playerAck.pSkill->name )//		��	0	2	100	0	ʹ�Է�3�غϲ���ʹ�ñ仯����
	{
		playerDef.tiaoDou = 3;
		m_pCurRound->log.push_back("3�غϲ���ʹ�ø�������");
		return false;
	}

	if ( "��ӡ" == playerAck.pSkill->name )//		��	0	2	101	0	�Է�����ʹ���ҷ���ļ���
	{
		if ( playerDef.ban )
		{
			m_pCurRound->log.push_back("�Է��Ѿ�����ӡ,ʧ����");
			return false;
		}
		playerDef.ban = true;
		m_pCurRound->log.push_back("����ʹ���ҷ�ӵ�еļ���");
		return false;
	}
	if ( "ͬ��" == playerAck.pSkill->name )//		��	0	2	101	0	�ܵ�����������0ʱ��������һ����
	{
		if ( playerDef.tongGui )
		{
			m_pCurRound->log.push_back("�Է��Ѿ���ͬ��,ʧ����");
			return false;
		}
		playerDef.tongGui = true;
		m_pCurRound->log.push_back("����ʱ�϶���һ��");
		return false;
	}
	if ( "��ֹ�" == playerAck.pSkill->name )//		��	0	2	100	0	����
	{
		playerDef.pCurPet->state = Race::gui;
		playerDef.hunLuan = 3;
		m_pCurRound->log.push_back("�Է�������");
		return false;
	}

	return false;
}

bool Battle::ActionAble(Battle::WARRIOR &player)
{
	if ( player.isActioned ) return false;

	if ( Race::pu == player.pCurPet->state ) 
	{
		if ( 0 == player.sleep || 20 >= player.rp.sleep ) return true;
	}
	else if ( Race::bing == player.pCurPet->state ) 
	{
		if ( 0 == player.frozen || 20 >= player.rp.ice ) return true;
	}
	else if ( Race::dian == player.pCurPet->state ) 
	{
		if ( 25 >= player.rp.dian ) return true;
	}

	return false;
}

bool Battle::LaunchState(Battle::WARRIOR &player)
{
	if ( Race::pu == player.pCurPet->state ) 
	{
		if ( 0 == player.sleep || 20 >= player.rp.sleep ) 
		{
			m_pCurRound->log.push_back( m_player.pCurPet->nick + "������" );
			return false;
		}
		m_pCurRound->log.push_back( m_player.pCurPet->nick + "����˯��" );
	}
	else if ( Race::bing == player.pCurPet->state ) 
	{
		if ( 0 == player.frozen || 20 >= player.rp.ice )
		{
			m_pCurRound->log.push_back( m_player.pCurPet->nick + "����˱���" );
			return false;
		}
		m_pCurRound->log.push_back( m_player.pCurPet->nick + "��������" );
	}
	else if ( Race::dian == player.pCurPet->state ) 
	{
		if ( 25 >= player.rp.dian ) return false;
		m_pCurRound->log.push_back( m_player.pCurPet->nick + "����ˣ������ж�" );
	}

	return true;
}

bool Battle::Medication(Battle::WARRIOR &player)
{
	if ( 0 == player.pCurPet->state || "����ҩ" != player.pItem->name ) return false;

	if ( Race::pu == player.pCurPet->state ) 
	{
		if ( 0 == player.sleep || 20 >= player.rp.sleep ) return false;
		m_pCurRound->log.push_back( m_player.pCurPet->nick + "ʹ��������ҩ�������˯��" );
	}
	else if ( Race::bing == player.pCurPet->state ) 
	{
		if ( 0 == player.frozen || 20 >= player.rp.ice ) return false;
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

bool Battle::IsEnd()
{
	if ( m_player.lose || m_enemy.lose ) return true;

	return false;
}
