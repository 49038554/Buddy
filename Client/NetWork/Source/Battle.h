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
	void Init(Game *game, int id, unsigned int mePlayerId, unsigned int shePlayerId, 
		std::vector<data::PET*> &me, std::vector<data::PET*> &she);
	void PlayerAction(bool me, Battle::Action act, short objectId, bool skillPro, bool itemPro, int speed);
	void PlayRound();

private:
	typedef struct ROUND
	{
		int		mePet;
		Action	me;
		short	meObjectId;
		bool	meSkillPro;
		bool	meItemPro;
		int		meSpeed;

		int		shePet;
		Action	she;
		short	sheObjectId;
		bool	sheSkillPro;
		bool	sheItemPro;
		int		sheSpeed;

		std::vector<std::string> log;
	}ROUND;

	void StepStart();
	void StepChange();
	void StepAttack();
	void StepEnd();
	void EntryStage(data::PET *pPet);
	void LeaveStage(data::PET *pPet);
	bool Hurt(data::PET *pPet, int HP);
	bool XianShou( short itemId, bool itemPro, Battle::Action act, short skill );
	int CalSpeed(data::PET *pPet);
	int CalWG(data::PET *pPet);
	int CalWF(data::PET *pPet);
	int CalTG(data::PET *pPet);
	int CalTF(data::PET *pPet);
	int CalPower(data::SKILL *pSkill, data::PET *pAckPet, data::PET *pDefPet);

private:
	Game *m_game;

	int						m_id;
	unsigned int			m_mePlayerId;
	unsigned int			m_shePlayerId;
	std::vector<data::PET>	m_meInit;
	std::vector<data::PET>	m_sheInit;
	std::vector<data::PET>	m_me;
	std::vector<data::PET>	m_she;
	std::vector<ROUND>		m_log;

	int						m_curRound;
	ROUND					*m_pCurRound;
	bool					m_meReady;
	bool					m_sheReady;
	data::PET				*m_pMePet;
	short	m_meSDCorrect;//速修正
	short	m_meWG;//攻强化等级
	short	m_meWF;//防强化等级
	short	m_meTG;//特攻强化等级
	short	m_meTF;//特防强化等级
	short	m_meSD;//速度强化等级
	bool	m_meRace[18];//属性强化
	data::PET				*m_pShePet;
	short	m_sheSDCorrect;//速修正
	short	m_sheWG;//攻强化等级
	short	m_sheWF;//防强化等级
	short	m_sheTG;//特攻强化等级
	short	m_sheTF;//特防强化等级
	short	m_sheSD;//速度强化等级
	bool	m_sheRace[18];//属性强化

	char					m_weather;//天气属性对应
	char					m_weatherCount;//天气剩余回合数 -1永久
	bool					m_meNail[18];//是否有钉子，属性id为下标，目前只有地属性钉子
	bool					m_sheNail[18];//是否有钉子，属性id为下标，目前只有地属性钉子
	char					m_meWall[2];//物、特
	char					m_sheWall[2];//物、特
};

#endif //BATTLE_H