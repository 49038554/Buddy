#ifndef PETFACTORY_H
#define PETFACTORY_H

#include "common/BuddyData.h"

class Game;
class PetFactory
{
public:
	PetFactory();
	virtual ~PetFactory();
	void SetGame(Game *game);
	data::PET Pet(const std::string &name); //pet.id<0ʧ��
	void InitSkill(data::PET &pet, data::BUDDY *pBuddy);//ȫ������
	void CalAbility(data::PET &pet, data::BUDDY *pBuddy);
	void FullSkill(data::PET &pet);
	bool AddSkill(data::PET &pet, Race::Race skill);
	bool AddItem(data::PET &pet, const std::string &name);
	bool SetTalent(data::PET &pet, const std::string &name);
	bool SetNature(data::PET &pet, const std::string &add, const std::string &del);
	bool SetHealthy(data::PET &pet, char sd );
	bool SetMuscle(data::PET &pet, short hp, short wg, short wf, short tg, short tf, short sd );
	bool SetSkill( data::PET &pet, const std::string name1, const std::string name2, const std::string name3, const std::string name4 );
	bool AddMuscle(data::PET &pet, short add);

	bool SkillAble(data::PET &pet, data::SKILL *pSkill);
	//Ұ��
	data::PET WildBuddy(data::BUDDY *pBuddy);//pet.id<0ʧ��
	//��Ʒ
	data::PET BestBuddy(const std::string &name);//pet.id<0ʧ��

private:
	Game *m_game;
};

#endif//PETFACTORY_H