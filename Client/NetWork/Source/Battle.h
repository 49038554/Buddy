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
		std::vector<data::PET> &me, std::vector<data::PET> &she);
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
	const char* CheckReady(bool me, Battle::Action act, short objectId, Battle::RAND_PARAM &rp);
	bool Ready(bool me, Battle::Action act, short objectId, Battle::RAND_PARAM &rp);
	const char* ChangePet(bool me, short petId);
	bool IsEnd();

private:
	typedef struct ROUND
	{
		Action	me;
		int		meObjectId;
		RAND_PARAM	meRP;
		std::vector<short> mePetId;

		Action	she;
		int		sheObjectId;
		RAND_PARAM	sheRP;
		std::vector<short> shePetId;

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
		bool	defensed;//防守过了
		bool	rest;//休息
		char	luanWu;//乱舞剩余回合
		bool	sunReady;//阳光烈焰准备中
		char	doomDesireRound;//破灭之愿剩余回合
		char	predictRound;//预知未来剩余回合
		int		outputHurt;//输出伤害
		bool	race[18];//属性强化
		bool	nail[18];//是否有钉子，属性id为下标，目前只有地属性钉子
		char	wall[2];//物、特，剩余回合数 -1永久
		std::map<short, bool>	lookSkill;

		char	fear;//害怕
		bool	isChanged;//有换人
		bool	isActioned;//已经行动
		bool	isEnd;//回合结束
		bool	lose;//失败
		data::BUDDY *pBuddy;
		data::ITEM *pItem;
		data::TALENT *pTalent;
		data::SKILL *pSkill;
		data::ITEM *pUseItem;


		Action	act;
		int		objId;
		RAND_PARAM rp;
	}WARRIOR;

	const char* SetPetInfo(Battle::WARRIOR &player, int petId);
	bool PlayRound();//完成返回true,中断返回false
	void StepStart();
	bool StepChange();//完成返回true,中断返回false
	bool StepAttack();//完成返回true,中断返回false
	bool StepEnd();//完成返回true,中断返回false
	bool WaitPlayerCMD();//等待玩家指令
	bool ChangePet(Battle::WARRIOR &player, int petId = 0);
	void EntryStage(Battle::WARRIOR &player, Battle::WARRIOR &enemy);
	void LeaveStage(Battle::WARRIOR &player);
	bool Hurt(Battle::WARRIOR &player, int HP, bool unFaint = false);//true有巴迪倒下，false没有巴迪倒下
	bool AttackOrder(Battle::WARRIOR &player, Battle::WARRIOR &enemy);
	bool IsUnwait(Battle::WARRIOR &player);
	bool IsWait(Battle::WARRIOR &player);
	int CalSpeed(Battle::WARRIOR &player, Battle::WARRIOR &enemy);
	bool CalHitRate(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef);
	bool CriticalHit(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef);
	int CalWG(Battle::WARRIOR &player, Battle::WARRIOR &enemy);
	int CalWF(Battle::WARRIOR &player, bool ct, Battle::WARRIOR &enemy);
	int CalTG(Battle::WARRIOR &player, Battle::WARRIOR &enemy);
	int CalTF(Battle::WARRIOR &player, bool ct, Battle::WARRIOR &enemy);
	int CalPower(Battle::WARRIOR &pAck, bool ct, Battle::WARRIOR &pDef);
	bool PetAction(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef);//true有巴迪战败，false没有巴迪战败
	bool UseSkill(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef);//true有巴迪战败，false没有巴迪战败
	bool HelpSkill(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef);//辅助技能,不是辅助技能返回false
	bool InterfereSkill(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef);//干扰技能
	bool ActionAble(Battle::WARRIOR &player);
	bool LaunchState(Battle::WARRIOR &player);
	bool Medication(Battle::WARRIOR &player);
	void ChangeSkill(Battle::WARRIOR &player);
	bool BanSkill(Battle::WARRIOR &player, Battle::WARRIOR &enemy);
	bool Confusion(Battle::WARRIOR &player);
	bool AttackEffect(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef);//攻击特效
	bool AttackCost(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef);//攻击代价
	bool ForcedLeave(Battle::WARRIOR &player);
	bool ImmuneState(Battle::WARRIOR &player);//免疫异常状态
	bool Faint(Battle::WARRIOR &player);
	void PlayerEnd(Battle::WARRIOR &player, Battle::WARRIOR &enemy);

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
// 	摘星术

};

#endif //BATTLE_H