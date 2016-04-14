#include "BuddyWorld.h"


BuddyWorld::BuddyWorld(void)
{
}


BuddyWorld::~BuddyWorld(void)
{
}

void BuddyWorld::SetGame(Game *game)
{
	m_game = game;
	m_factory.SetGame(m_game);
}


data::PET BuddyWorld::WildBuddy(data::BUDDY *pBuddy)
{
	return m_factory.WildBuddy(pBuddy);
}

data::PET BuddyWorld::BestBuddy(const std::string &name)
{
	return m_factory.BestBuddy(name);
}

data::PET BuddyWorld::Buddy001()
{
	data::PET pet;
	pet = m_factory.BestBuddy("���ܲ�");
	m_factory.SetTalent(pet, "Ҷ����");
	m_factory.AddItem(pet, "����");
	m_factory.SetNature(pet, "SD", "TF");
	m_factory.SetSkill(pet, "��������", "��ͻ", "���", "�����");
	m_factory.SetMuscle(pet, 6, 0, 0, 252, 0, 252);
	pet.id = 1;

	return pet;
}

data::PET BuddyWorld::Buddy002()
{
	data::PET pet;
	pet = m_factory.BestBuddy("������");
	m_factory.SetTalent(pet, "̫������");
	m_factory.AddItem(pet, "����");
	m_factory.SetNature(pet, "SD", "TF");
	m_factory.SetSkill(pet, "����", "����", "�㱼����ȭ", "��������");
	m_factory.SetMuscle(pet, 6, 0, 0, 252, 0, 252);
	pet.id = 1;

	return pet;
}
