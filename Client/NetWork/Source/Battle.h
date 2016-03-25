#ifndef BATTLE_H
#define BATTLE_H

#include "common/BuddyData.h"

class Game;

class Battle
{
public:
	enum Action
	{
		change = 0,
		useItem = 1,
		attack = 2,
	};

public:
	Battle();
	virtual ~Battle();
	bool Init(Game *game, int id, 
		const std::string &playerName, const std::string &enemyName, 
		unsigned int playerId, unsigned int enemyId, 
		std::vector<data::PET*> &me, std::vector<data::PET*> &she);
	typedef struct RAND_PARAM
	{
		char miss;//命中随机数
		char sePro;//技能特效随机数
		char iePro;//物品特效随机数
		char tePro;//特性特效随机数
		char luanWu;//乱舞回合数
		char sleepRound;//睡眠回合随机数
		char frozenRound;//冰冻回合随机数
		char dian;//麻痹随机数
		bool luan;//混乱随机数
		unsigned char	hurt;//伤害随机数217~255
		unsigned char	speed;//速度随机数
	}RAND_PARAM;
	const char* PlayerRand(bool me, Battle::Action act, short objectId, Battle::RAND_PARAM &rp);
	bool PlayerAction(bool me, Battle::Action act, short objectId, Battle::RAND_PARAM &rp);
	bool IsEnd();

private:
	typedef struct ROUND
	{
		Action	me;
		int		meObjectId;
		RAND_PARAM	meRP;

		Action	she;
		int		sheObjectId;
		RAND_PARAM	sheRP;

		std::vector<std::string> log;
	}ROUND;

	typedef struct WARRIOR
	{
		unsigned int			playerId;
		std::string				name;
		std::vector<data::PET>	pets;
		bool					isReady;
		data::PET				*pCurPet;
		float	wgCorrect;//物攻修正
		float	tgCorrect;//特攻修正
		float	sdCorrect;//速修正
		char	wg;//攻强化等级
		char	wf;//防强化等级
		char	tg;//特攻强化等级
		char	tf;//特防强化等级
		char	sd;//速度强化等级
		char	ct;//暴击强化等级
		char	miss;//闪避强化
		char	unMiss;//命中强化
		bool	changePetAble;//不能换人
		short	lockSkill;//锁定技能
		char	lockSkillTime;//锁定技能回合数量,-1永久
		bool	smell;//嗅觉

		short	recvHP;//回复HP
		bool	seed;//种子
		char	haQian;//中了哈欠
		char	mieGe;//中了灭亡歌
		short	tiShen;//替身生命
		bool	xunXing;//中寻衅
		char	tiaoDou;//挑逗回合
		bool	ban;//中封印
		bool	tongGui;//中同归
		char	sleepRound;//催眠剩余回合
		char	frozenRound;//冰封剩余回合
		char	hunLuan;//混乱剩余回合数

		bool	race[18];//属性强化
		bool	nail[18];//是否有钉子，属性id为下标，目前只有地属性钉子
		char	wall[2];//物、特，剩余回合数 -1永久
		std::map<short, bool>	lookSkill;
		bool	isActioned;//已经行动
		bool	lose;//失败
		data::BUDDY *pBuddy;
		data::ITEM *pItem;
		data::TALENT *pTalent;
		data::SKILL *pSkill;

		Action	act;
		int		objId;
		RAND_PARAM rp;
	}WARRIOR;

	bool PlayRound();//完成返回true,中断返回false
	void StepStart();
	bool StepChange();//完成返回true,中断返回false
	bool StepAttack();//完成返回true,中断返回false
	bool StepEnd();//完成返回true,中断返回false
	//交换巴迪的结果
	enum ChangeResult
	{
		unchange = 0,
		finished = 1,
		faint = 2,
	};
	Battle::ChangeResult ChangePet(Battle::WARRIOR &player, int petId = 0);
	void EntryStage(Battle::WARRIOR &player, Battle::WARRIOR &enemy);
	void LeaveStage(Battle::WARRIOR &player);
	bool Hurt(Battle::WARRIOR &player, int HP);//true有巴迪倒下，false没有巴迪倒下
	bool AttackOrder(Battle::WARRIOR &player, Battle::WARRIOR &enemy);
	bool IsUnwait(Battle::WARRIOR &player);
	bool IsWait(Battle::WARRIOR &player);
	int CalSpeed(Battle::WARRIOR &player, Battle::WARRIOR &enemy);
	int CalWG(Battle::WARRIOR &player, Battle::WARRIOR &enemy);
	int CalWF(Battle::WARRIOR &player, Battle::WARRIOR &enemy);
	int CalTG(Battle::WARRIOR &player, Battle::WARRIOR &enemy);
	int CalTF(Battle::WARRIOR &player, Battle::WARRIOR &enemy);
	int CalPower(Battle::WARRIOR &pAck, Battle::WARRIOR &pDef);
	bool PetAction(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef);//true有巴迪战败，false没有巴迪战败
	bool UseSkill(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef);//true有巴迪战败，false没有巴迪战败
	bool UseHelpSkill(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef);//true有巴迪战败，false没有巴迪战败
	bool ActionAble(Battle::WARRIOR &player);
	bool LaunchState(Battle::WARRIOR &player);
	bool Medication(Battle::WARRIOR &player);
	void ChangeSkill(Battle::WARRIOR &player);
	bool BanSkill(Battle::WARRIOR &player, Battle::WARRIOR &enemy);
	bool Confusion(Battle::WARRIOR &player);

private:
	Game *m_game;
	int						m_id;
	std::vector<data::PET>	m_playerInitPets;
	std::vector<data::PET>	m_enemyInitPets;

	std::vector<ROUND>		m_log;
	int						m_curRound;
	ROUND					*m_pCurRound;
	WARRIOR					m_player;
	WARRIOR					m_enemy;
	char					m_weather;//天气属性对应
	char					m_weatherCount;//天气剩余回合数 -1永久
	char					m_foolSpace;//欺骗空间剩余回合数 -1永久
};

#endif //BATTLE_H