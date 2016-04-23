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

data::PET BuddyWorld::Buddy003()
{
	data::PET pet;
	pet = m_factory.BestBuddy("���ڹ�");
	m_factory.SetTalent(pet, "��ˮ");
	m_factory.AddItem(pet, "ʣ��");
	m_factory.SetNature(pet, "SD", "TF");
	m_factory.SetSkill(pet, "����", "�峡", "����", "����Һ");
	m_factory.SetMuscle(pet, 6, 0, 0, 252, 0, 252);
	pet.id = 1;

	return pet;
}

data::PET BuddyWorld::Buddy006()
{
	data::PET pet;
	pet = m_factory.BestBuddy("������");
	m_factory.SetTalent(pet, "��Ȼ�ָ�");
	m_factory.AddItem(pet, "ʣ��");
	m_factory.SetNature(pet, "SD", "TF");
	m_factory.SetSkill(pet, "Ԥ֪δ��", "���౬��", "���ѻ�ת", "Ʈ����ѩ���㴵");
	m_factory.SetMuscle(pet, 6, 0, 0, 252, 0, 252);
	pet.id = 1;

	return pet;
}

data::PET BuddyWorld::Buddy008()
{
	data::PET pet;
	pet = m_factory.BestBuddy("Ů��");
	m_factory.SetTalent(pet, "����");
	m_factory.AddItem(pet, "��Ҷ��");
	m_factory.SetNature(pet, "SD", "TF");
	m_factory.SetSkill(pet, "���е�", "����", "����ȭ", "����ȭ");
	m_factory.SetMuscle(pet, 6, 0, 0, 252, 0, 252);
	pet.id = 1;

	return pet;
}

data::PET BuddyWorld::Buddy023()
{
	data::PET pet;
	pet = m_factory.BestBuddy("����Ů");
	m_factory.SetTalent(pet, "������");
	m_factory.AddItem(pet, "����");
	m_factory.SetNature(pet, "SD", "TF");
	m_factory.SetSkill(pet, "Ҷ��ն", "�����и�", "�������", "Ѱ��");
	m_factory.SetMuscle(pet, 6, 0, 0, 252, 0, 252);
	pet.id = 1;

	return pet;
}

data::PET BuddyWorld::Buddy026()
{
	data::PET pet;
	pet = m_factory.BestBuddy("槼�");
	m_factory.SetTalent(pet, "�ɺ�");
	m_factory.AddItem(pet, "�Ŵ�");
	m_factory.SetNature(pet, "SD", "TF");
	m_factory.SetSkill(pet, "ҧ��", "������", "ȼ�մ���", "�����ƽ�");
	m_factory.SetMuscle(pet, 6, 0, 0, 252, 0, 252);
	pet.id = 1;

	return pet;
}

data::PET BuddyWorld::Buddy095()
{
	data::PET pet;
	pet = m_factory.BestBuddy("����Ů");
	m_factory.SetTalent(pet, "ʥ��ʿ");
	m_factory.AddItem(pet, "ר��ͷ��");
	m_factory.SetNature(pet, "SD", "TF");
	m_factory.SetSkill(pet, "�����ֶ�", "���Ԫ���", "������", "��������");
	m_factory.SetMuscle(pet, 6, 0, 0, 252, 0, 252);
	pet.id = 1;

	return pet;
}

data::PET BuddyWorld::Buddy103()
{
	data::PET pet;
	pet = m_factory.BestBuddy("��ɱ��");
	m_factory.SetTalent(pet, "����");
	m_factory.AddItem(pet, "��Ҷ��");
	m_factory.SetNature(pet, "SD", "TF");
	m_factory.SetSkill(pet, "��Ԫ��", "�����", "�㱼����ȭ", "����");
	m_factory.SetMuscle(pet, 6, 0, 0, 252, 0, 252);
	pet.id = 1;

	return pet;
}

data::PET BuddyWorld::Buddy110()
{
	data::PET pet;
	pet = m_factory.BestBuddy("���з");
	m_factory.SetTalent(pet, "����");
	m_factory.AddItem(pet, "�Ŵ�");
	m_factory.SetNature(pet, "SD", "TF");
	m_factory.SetSkill(pet, "����ȭ", "�׹�ȭ", "����ȭ", "�������");
	m_factory.SetMuscle(pet, 6, 0, 0, 252, 0, 252);
	pet.id = 1;

	return pet;
}
