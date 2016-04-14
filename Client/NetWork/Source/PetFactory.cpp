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
	pet.id = -1;
	pet.synced = false;
	data::BUDDY *pBuddy = Buddy(name, m_game->BuddyBook());
	if ( NULL == pBuddy ) return pet;

	pet.state = 0;//状态
	pet.number = pBuddy->number;
	pet.nick = pBuddy->name;//昵称
	pet.talent = pBuddy->talent1;//特性
	pet.nature = GetNature("-", "-");//性格
	pet.HPHealthy = rand() % 31;//血先天体质
	pet.WGHealthy = rand() % 31;//攻先天体质
	pet.WFHealthy = rand() % 31;//防先天体质
	pet.TGHealthy = rand() % 31;//特攻先天体质
	pet.TFHealthy = rand() % 31;//特防先天体质
	pet.SDHealthy = rand() % 31;//速度先天体质
	pet.itemId = 0;
	pet.HPMuscle = 0;//血后天修炼
	pet.WGMuscle = 0;//攻后天修炼
	pet.WFMuscle = 0;//防后天修炼
	pet.TGMuscle = 0;//特攻后天修炼
	pet.TFMuscle = 0;//特防后天修炼
	pet.SDMuscle = 0;//速度后天修炼
	InitSkill(pet, pBuddy);
	CalAbility(pet, pBuddy);

	pet.synced = true;
	pet.id = 1;

	return pet;
}

void PetFactory::InitSkill(data::PET &pet, data::BUDDY *pBuddy)
{
	pet.skill1 = pet.skill2 = pet.skill3 = pet.skill4 = 0;
	std::map<unsigned short, bool>::iterator it = pBuddy->skills.begin();
	data::SKILL *pSkill;
	for ( ; it != pBuddy->skills.end(); it++ )
	{
		pSkill = Skill(it->first, m_game->SkillBook());
		if ( !it->second && !SkillAble(pet, pSkill) ) continue;
		if ( 0 >= pSkill->power ) continue;
		if ( 0 == pet.skill1 ) pet.skill1 = pSkill->id;
		else if ( 0 == pet.skill2 ) pet.skill2 = pSkill->id;
		else if ( 0 == pet.skill3 ) pet.skill3 = pSkill->id;
		else if ( 0 == pet.skill4 ) pet.skill4 = pSkill->id;
		else break;
	}

	return;
}

void PetFactory::CalAbility(data::PET &pet, data::BUDDY *pBuddy)
{
	pet.curHP = pet.HP = (pBuddy->hitPoint * 2 + pet.HPMuscle/4 + pet.HPHealthy) + 100 + 10;//血
	pet.WG = ((pBuddy->physicalA * 2 + pet.WGMuscle/4 + pet.WGHealthy) + 5) * GetNatureCal(pet.nature, "WG");//攻
	pet.WF = ((pBuddy->physicalD * 2 + pet.WFMuscle/4 + pet.WFHealthy) + 5) * GetNatureCal(pet.nature, "WF");//防
	pet.TG = ((pBuddy->specialA * 2 + pet.TFMuscle/4 + pet.TGHealthy) + 5) * GetNatureCal(pet.nature, "TG");//特攻
	pet.TF = ((pBuddy->specialD * 2 + pet.TFMuscle/4 + pet.TFHealthy) + 5) * GetNatureCal(pet.nature, "TF");//特防
	pet.SD = ((pBuddy->speed * 2 + pet.SDMuscle/4 + pet.SDHealthy) + 5) * GetNatureCal(pet.nature, "SD");//速度

	return;
}

void PetFactory::FullSkill(data::PET &pet)
{
	pet.race.clear();
	int i = 0;
	for ( i = 1; i < 18; i++ ) pet.race.push_back(i);//17个属性的恶魔果实是否吃过
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

	data::SKILL *pSkill = NULL;
	data::SKILL *pSkill1 = Skill(name1, m_game->SkillBook());
	data::SKILL *pSkill2 = Skill(name2, m_game->SkillBook());
	data::SKILL *pSkill3 = Skill(name3, m_game->SkillBook());
	data::SKILL *pSkill4 = Skill(name4, m_game->SkillBook());
	if ( NULL == pSkill1 || NULL == pSkill2 
		|| NULL == pSkill3 || NULL == pSkill4 ) return false;
	pet.skill1 = pet.skill2 = pet.skill3 = pet.skill4 = 0;

	std::map<unsigned short, bool>::iterator it;
	for ( it = pBuddy->skills.begin(); it != pBuddy->skills.end(); it++ )
	{
		pSkill = Skill(it->first, m_game->SkillBook());
		if ( !it->second && !SkillAble(pet, pSkill) ) continue;
		if ( pSkill1 == pSkill ) pet.skill1 = it->first;
		if ( pSkill2 == pSkill ) pet.skill2 = it->first;
		if ( pSkill3 == pSkill ) pet.skill3 = it->first;
		if ( pSkill4 == pSkill ) pet.skill4 = it->first;
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

data::PET PetFactory::WildBuddy(data::BUDDY *pBuddy)
{
	data::PET pet;
	pet = Pet(pBuddy->name);
	if ( 0 > pet.id ) return pet;

	//随机特性
	int n = rand()%3;
	if ( 0 == n ) pet.talent = pBuddy->talent1;
	else if ( 1 == n ) pet.talent = pBuddy->talent2;
	else pet.talent = pBuddy->talent3;
	if ( 0 >= pet.talent ) pet.talent = pBuddy->talent1;
	//随机性格
	pet.nature = rand()%21;
	//随机个体
	n = rand()%7;
	int i = 0;
	int attr[7];
	for ( i = 0; i < 6; i++ ) attr[i] = rand()%31;
	for ( i = 0; i < n; i++ )
	{
		for ( ; true; )
		{
			int pos = rand()%6;
			if ( 31 == attr[pos] ) continue;
			attr[pos] = 31;
			break;
		}
	}
	if ( !SetHealthy(pet, attr[0], attr[1], attr[2], attr[3], attr[4], attr[5]) ) return pet;

	pet.id = 1;//也怪每次出1只

	return pet;
}

data::PET PetFactory::BestBuddy(const std::string &name)
{
	data::PET pet;
	pet.id = -1;
	data::BUDDY *pBuddy = Buddy(name, m_game->BuddyBook());
	if ( NULL == pBuddy ) return pet;

	pet = Pet(name);
	if ( 0 > pet.id ) return pet;
	if ( !SetHealthy(pet, 31, 31, 31, 31, 31, 31) ) return pet;

	FullSkill(pet);
	InitSkill(pet, pBuddy);
	pet.id = 0;

	return pet;
}

bool PetFactory::SkillAble(data::PET &pet, data::SKILL *pSkill)
{
	if ( NULL == pSkill ) return false;

	int i = 0;
	for ( i = 0; i < pet.race.size(); i++ )
	{
		if ( pSkill->race != pet.race[i] ) continue;
		return true;
	}

	return false;
}
