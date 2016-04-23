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
	pet = m_factory.BestBuddy("怪蛙草");
	m_factory.SetTalent(pet, "叶绿素");
	m_factory.AddItem(pet, "王冠");
	m_factory.SetNature(pet, "SD", "TF");
	m_factory.SetSkill(pet, "寄生种子", "毒突", "吼叫", "反射盾");
	m_factory.SetMuscle(pet, 6, 0, 0, 252, 0, 252);
	pet.id = 1;

	return pet;
}

data::PET BuddyWorld::Buddy002()
{
	data::PET pet;
	pet = m_factory.BestBuddy("爆炎龙");
	m_factory.SetTalent(pet, "太阳力量");
	m_factory.AddItem(pet, "王冠");
	m_factory.SetNature(pet, "SD", "TF");
	m_factory.SetSkill(pet, "晴天", "鼓掌", "裸奔气合拳", "阳光烈焰");
	m_factory.SetMuscle(pet, 6, 0, 0, 252, 0, 252);
	pet.id = 1;

	return pet;
}

data::PET BuddyWorld::Buddy003()
{
	data::PET pet;
	pet = m_factory.BestBuddy("巨炮龟");
	m_factory.SetTalent(pet, "蓄水");
	m_factory.AddItem(pet, "剩饭");
	m_factory.SetNature(pet, "SD", "TF");
	m_factory.SetSkill(pet, "保护", "清场", "忍耐", "急救液");
	m_factory.SetMuscle(pet, 6, 0, 0, 252, 0, 252);
	pet.id = 1;

	return pet;
}

data::PET BuddyWorld::Buddy006()
{
	data::PET pet;
	pet = m_factory.BestBuddy("大仙子");
	m_factory.SetTalent(pet, "自然恢复");
	m_factory.AddItem(pet, "剩饭");
	m_factory.SetNature(pet, "SD", "TF");
	m_factory.SetSkill(pet, "预知未来", "淤泥爆弹", "蜻蜓回转", "飘花淡雪浮香吹");
	m_factory.SetMuscle(pet, 6, 0, 0, 252, 0, 252);
	pet.id = 1;

	return pet;
}

data::PET BuddyWorld::Buddy008()
{
	data::PET pet;
	pet = m_factory.BestBuddy("女皇");
	m_factory.SetTalent(pet, "霸气");
	m_factory.AddItem(pet, "四叶草");
	m_factory.SetNature(pet, "SD", "TF");
	m_factory.SetSkill(pet, "逆刃刀", "暗算", "七伤拳", "气合拳");
	m_factory.SetMuscle(pet, 6, 0, 0, 252, 0, 252);
	pet.id = 1;

	return pet;
}

data::PET BuddyWorld::Buddy023()
{
	data::PET pet;
	pet = m_factory.BestBuddy("炽天女");
	m_factory.SetTalent(pet, "正义心");
	m_factory.AddItem(pet, "王冠");
	m_factory.SetNature(pet, "SD", "TF");
	m_factory.SetSkill(pet, "叶刃斩", "空气切割", "喷射火焰", "寻衅");
	m_factory.SetMuscle(pet, 6, 0, 0, 252, 0, 252);
	pet.id = 1;

	return pet;
}

data::PET BuddyWorld::Buddy026()
{
	data::PET pet;
	pet = m_factory.BestBuddy("妲己");
	m_factory.SetTalent(pet, "干旱");
	m_factory.AddItem(pet, "放大镜");
	m_factory.SetNature(pet, "SD", "TF");
	m_factory.SetSkill(pet, "咬碎", "精神冲击", "燃烧殆尽", "炽热推进");
	m_factory.SetMuscle(pet, 6, 0, 0, 252, 0, 252);
	pet.id = 1;

	return pet;
}

data::PET BuddyWorld::Buddy095()
{
	data::PET pet;
	pet = m_factory.BestBuddy("修罗女");
	m_factory.SetTalent(pet, "圣斗士");
	m_factory.AddItem(pet, "专爱头巾");
	m_factory.SetNature(pet, "SD", "TF");
	m_factory.SetSkill(pet, "最终手段", "异次元冲锋", "亿万冲击", "阳光烈焰");
	m_factory.SetMuscle(pet, 6, 0, 0, 252, 0, 252);
	pet.id = 1;

	return pet;
}

data::PET BuddyWorld::Buddy103()
{
	data::PET pet;
	pet = m_factory.BestBuddy("暗杀兔");
	m_factory.SetTalent(pet, "火珠");
	m_factory.AddItem(pet, "四叶草");
	m_factory.SetNature(pet, "SD", "TF");
	m_factory.SetSkill(pet, "空元气", "进身击", "裸奔气合拳", "替身");
	m_factory.SetMuscle(pet, 6, 0, 0, 252, 0, 252);
	pet.id = 1;

	return pet;
}

data::PET BuddyWorld::Buddy110()
{
	data::PET pet;
	pet = m_factory.BestBuddy("钢螃蟹");
	m_factory.SetTalent(pet, "净体");
	m_factory.AddItem(pet, "放大镜");
	m_factory.SetNature(pet, "SD", "TF");
	m_factory.SetSkill(pet, "彗星拳", "雷光拳", "急冻拳", "精神干扰");
	m_factory.SetMuscle(pet, 6, 0, 0, 252, 0, 252);
	pet.id = 1;

	return pet;
}
