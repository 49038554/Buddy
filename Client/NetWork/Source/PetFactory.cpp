#include "PetFactory.h"
#include "Game.h"


PetFactory::PetFactory()
{
}


PetFactory::~PetFactory()
{
}

void PetFactory::SetGame(Game *game)
{
	m_game = game;
}

data::PET PetFactory::Pet(const std::string &name)
{
	data::PET pet;
	pet.id = 0;
	pet.synced = false;
	data::BUDDY *pBuddy = Buddy(name, m_game->BuddyBook());
	if ( NULL == pBuddy ) return pet;

	pet.state = 0;//״̬
	pet.number = pBuddy->number;
	pet.nick = pBuddy->name;//�ǳ�
	pet.talent = pBuddy->talent1;//����
	pet.nature = GetNature("-", "-");//�Ը�
	pet.HPHealthy = rand() % 31;//Ѫ��������
	pet.WGHealthy = rand() % 31;//����������
	pet.WFHealthy = rand() % 31;//����������
	pet.TGHealthy = rand() % 31;//�ع���������
	pet.TFHealthy = rand() % 31;//�ط���������
	pet.SDHealthy = rand() % 31;//�ٶ���������
	pet.itemId = 0;
	pet.HPMuscle = 0;//Ѫ��������
	pet.WGMuscle = 0;//����������
	pet.WFMuscle = 0;//����������
	pet.TGMuscle = 0;//�ع���������
	pet.TFMuscle = 0;//�ط���������
	pet.SDMuscle = 0;//�ٶȺ�������
	pet.skill1 = pet.skill2 = pet.skill3 = pet.skill4 = 0;
	InitSkill(pet, pBuddy);
	CalAbility(pet, pBuddy);

	pet.synced = true;

	return pet;
}

void PetFactory::InitSkill(data::PET &pet, data::BUDDY *pBuddy)
{
	std::map<unsigned short, bool>::iterator it;
	for ( it = pBuddy->skills.begin(); it != pBuddy->skills.end(); it++ )
	{
		if ( !it->second ) continue;
		if ( 0 == pet.skill1 ) 
		{
			pet.skill1 = it->first;
			continue;
		}
		else if ( 0 == pet.skill2 ) 
		{
			pet.skill2 = it->first;
			continue;
		}
		else if ( 0 == pet.skill3 ) 
		{
			pet.skill3 = it->first;
			continue;
		}
		else if ( 0 == pet.skill4 ) 
		{
			pet.skill4 = it->first;
			continue;
		}
		else return;
	}

	return;
}

void PetFactory::CalAbility(data::PET &pet, data::BUDDY *pBuddy)
{
	pet.curHP = pet.HP = (pBuddy->hitPoint * 2 + pet.HPMuscle/4 + pet.HPHealthy) + 100 + 10;//Ѫ
	pet.WG = ((pBuddy->physicalA * 2 + pet.WGMuscle/4 + pet.WGHealthy) + 5) * GetNatureCal(pet.nature, "WG");//��
	pet.WF = ((pBuddy->physicalD * 2 + pet.WFMuscle/4 + pet.WFHealthy) + 5) * GetNatureCal(pet.nature, "WF");//��
	pet.TG = ((pBuddy->specialA * 2 + pet.TFMuscle/4 + pet.TGHealthy) + 5) * GetNatureCal(pet.nature, "TG");//�ع�
	pet.TF = ((pBuddy->specialD * 2 + pet.TFMuscle/4 + pet.TFHealthy) + 5) * GetNatureCal(pet.nature, "TF");//�ط�
	pet.SD = ((pBuddy->speed * 2 + pet.SDMuscle/4 + pet.SDHealthy) + 5) * GetNatureCal(pet.nature, "SD");//�ٶ�

	return;
}

void PetFactory::FullSkill(data::PET &pet)
{
	pet.race.clear();
	int i = 0;
	for ( i = 1; i < 18; i++ ) pet.race.push_back(i);//17�����ԵĶ�ħ��ʵ�Ƿ�Թ�
}

bool PetFactory::AddSkill(data::PET &pet, Race::Race skill)
{
	int i = 0;
	for ( i = 0; i < pet.race.size(); i++ ) 
	{
		if ( skill == pet.race[i] ) return false;
	}
	pet.race.push_back(skill);

	return true;
}

bool PetFactory::AddItem(data::PET &pet, const std::string &name)
{
	data::ITEM *pItem = Item(name, m_game->ItemBook());
	if ( NULL == pItem ) 
	{
		pet.itemId = 0;
		return false;
	}

	pet.itemId = pItem->id;
	return 0;
}

bool PetFactory::SetTalent(data::PET &pet, const std::string &name)
{
	data::BUDDY *pBuddy = Buddy(pet.number, m_game->BuddyBook());
	data::TALENT *pTalent = Talent(name, m_game->TalentBook());
	if ( NULL == pTalent || NULL == pBuddy ) return false; 
	if (pBuddy->talent1 != pTalent->id 
		&& pBuddy->talent2 != pTalent->id 
		&& pBuddy->talent3 != pTalent->id ) return false;

	pet.talent = pTalent->id;

	return true;
}

bool PetFactory::SetNature(data::PET &pet, const std::string &add, const std::string &del)
{
	data::BUDDY *pBuddy = Buddy(pet.number, m_game->BuddyBook());
	if ( NULL == pBuddy ) return false;

	pet.nature = GetNature(add, del);
	CalAbility(pet, pBuddy);

	return true;
}

bool PetFactory::SetHealthy(data::PET &pet, short hp, short wg, short wf, short tg, short tf, short sd )
{
	if ( 31 < hp || 31 < wg || 31 < wf
		|| 31 < tg || 31 < tf || 31 < sd
		|| 0 > hp || 0 > wg || 0 > wf
		|| 0 > tg || 0 > tf || 0 > sd
		) return false;

	data::BUDDY *pBuddy = Buddy(pet.number, m_game->BuddyBook());
	if ( NULL == pBuddy ) return false;

	pet.HPHealthy = hp;
	pet.WGHealthy = wg;
	pet.WFHealthy = wf;
	pet.TGHealthy = tg;
	pet.TFHealthy = tf;
	pet.SDHealthy = sd;

	CalAbility(pet, pBuddy);

	return true;
}

bool PetFactory::SetMuscle(data::PET &pet, short hp, short wg, short wf, short tg, short tf, short sd )
{
	if ( 510 < hp + wg + wf + tg + tf + sd 
		|| 252 < hp || 252 < wg || 252 < wf
		|| 252 < tg || 252 < tf || 252 < sd
		|| 0 > hp || 0 > wg || 0 > wf
		|| 0 > tg || 0 > tf || 0 > sd
		) return false;

	data::BUDDY *pBuddy = Buddy(pet.number, m_game->BuddyBook());
	if ( NULL == pBuddy ) return false;

	pet.HPMuscle = hp;
	pet.WGMuscle = wg;
	pet.WFMuscle = wf;
	pet.TGMuscle = tg;
	pet.TFMuscle = tf;
	pet.SDMuscle = sd;

	CalAbility(pet, pBuddy);

	return true;
}

bool PetFactory::SetSkill( data::PET &pet, const std::string name1, const std::string name2, const std::string name3, const std::string name4 )
{
	if ( name1 == name2 
		|| name1 == name3 
		|| name1 == name4 
		|| name2 == name3 
		|| name2 == name4 
		|| name3 == name4 
		) return false;

	data::BUDDY *pBuddy = Buddy(pet.number, m_game->BuddyBook());
	if ( NULL == pBuddy ) return false;

	data::SKILL *pSkill1 = Skill(name1, m_game->SkillBook());
	data::SKILL *pSkill2 = Skill(name1, m_game->SkillBook());
	data::SKILL *pSkill3 = Skill(name1, m_game->SkillBook());
	data::SKILL *pSkill4 = Skill(name1, m_game->SkillBook());
	if ( NULL == pSkill1 || NULL == pSkill2 
		|| NULL == pSkill3 || NULL == pSkill4 ) return false;
	pet.skill1 = pet.skill2 = pet.skill3 = pet.skill4 = 0;

	std::map<unsigned short, bool>::iterator it;
	for ( it = pBuddy->skills.begin(); it != pBuddy->skills.end(); it++ )
	{
		if ( !it->second ) continue;
		if ( pSkill1->id == it->first ) pet.skill1 = it->first;
		if ( pSkill2->id == it->first ) pet.skill2 = it->first;
		if ( pSkill3->id == it->first ) pet.skill3 = it->first;
		if ( pSkill4->id == it->first ) pet.skill4 = it->first;
	}

	return true;
}

bool PetFactory::AddMuscle(data::PET &pet, short add)
{
	data::BUDDY *pBuddy = Buddy(pet.number, m_game->BuddyBook());
	if ( NULL == pBuddy ) return false;

	if ( 252 > pet.HPMuscle && 0 < add ) 
	{
		if ( 252 > pet.HPMuscle + add ) pet.HPMuscle += add; 
		else 
		{
			add -= 252 - pet.HPMuscle;
			pet.HPMuscle = 252;
		}
	}
	if ( 252 > pet.WGMuscle && 0 < add ) 
	{
		if ( 252 > pet.WGMuscle + add ) pet.WGMuscle += add; 
		else 
		{
			add -= 252 - pet.WGMuscle;
			pet.WGMuscle = 252;
		}
	}
	if ( 252 > pet.WFMuscle && 0 < add ) 
	{
		if ( 252 > pet.WFMuscle + add ) pet.WFMuscle += add; 
		else 
		{
			add -= 252 - pet.WFMuscle;
			pet.WFMuscle = 252;
		}
	}
	if ( 252 > pet.TGMuscle && 0 < add ) 
	{
		if ( 252 > pet.TGMuscle + add ) pet.TGMuscle += add; 
		else 
		{
			add -= 252 - pet.TGMuscle;
			pet.TGMuscle = 252;
		}
	}
	if ( 252 > pet.TFMuscle && 0 < add ) 
	{
		if ( 252 > pet.TFMuscle + add ) pet.TFMuscle += add; 
		else 
		{
			add -= 252 - pet.TFMuscle;
			pet.TFMuscle = 252;
		}
	}
	if ( 252 > pet.SDMuscle && 0 < add ) 
	{
		if ( 252 > pet.SDMuscle + add ) pet.SDMuscle += add; 
		else 
		{
			add -= 252 - pet.SDMuscle;
			pet.SDMuscle = 252;
		}
	}

	CalAbility(pet, pBuddy);

	return true;
}