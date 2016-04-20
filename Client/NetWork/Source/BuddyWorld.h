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
	//Ò°¹Ö
	data::PET WildBuddy(data::BUDDY *pBuddy);//pet.id<0Ê§°Ü
	//¼«Æ·
	data::PET BestBuddy(const std::string &name);//pet.id<0Ê§°Ü

	data::PET Buddy001();
	data::PET Buddy002();
	data::PET Buddy003();
	data::PET Buddy006();
	data::PET Buddy008();
	data::PET Buddy023();
	data::PET Buddy026();
	data::PET Buddy095();
	data::PET Buddy103();
	data::PET Buddy110();


private:
	Game *m_game;
	PetFactory	m_factory;
};

#endif