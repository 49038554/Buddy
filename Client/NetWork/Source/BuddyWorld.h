#ifndef BUDDYWORLD_H
#define BUDDYWORLD_H

#include "PetFactory.h"

class Game;
class BuddyWorld
{
public:
	BuddyWorld(void);
	virtual ~BuddyWorld(void);
	void SetGame(Game *game);
	//Ұ��
	data::PET WildBuddy(data::BUDDY *pBuddy);//pet.id<0ʧ��
	//��Ʒ
	data::PET BestBuddy(const std::string &name);//pet.id<0ʧ��

	data::PET Buddy001();
	data::PET Buddy002();


private:
	Game *m_game;
	PetFactory	m_factory;
};

#endif