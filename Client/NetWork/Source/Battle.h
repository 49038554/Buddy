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
	void Init(Game *game, int id, std::vector<data::PET*> &me, std::vector<data::PET*> &she);
	void PlayerAction(bool me, Battle::Action act, short objectId, bool skillPro, int speed);
	void PlayRound();

private:
	typedef struct ROUND
	{
		int		mePet;
		Action	me;
		short	meObjectId;
		bool	meSkillPro;
		int		meSpeed;

		int		shePet;
		Action	she;
		short	sheObjectId;
		bool	sheSkillPro;
		int		sheSpeed;

		std::vector<std::string> log;
	}ROUND;

	void StepStart();
	void StepChange();
	void StepAttack();
	void StepEnd();
	void EntryStageTalent(data::PET *pPet);
	void LeaveStageTalent(data::PET *pPet);

private:
	Game *m_game;

	int						m_id;
	std::vector<data::PET>	m_meInit;
	std::vector<data::PET>	m_sheInit;
	std::vector<data::PET>	m_me;
	std::vector<data::PET>	m_she;
	std::vector<ROUND>		m_log;

	int						m_curRound;
	bool					m_meReady;
	bool					m_sheReady;
	char					m_weather;//天气属性对应
	char					m_weatherCount;//天气剩余回合数 -1永久
	data::PET				*m_pMePet;
	short	m_meWG;//攻强化等级
	short	m_meWF;//防强化等级
	short	m_meTG;//特攻强化等级
	short	m_meTF;//特防强化等级
	short	m_meSD;//速度强化等级
	data::PET				*m_pShePet;
	short	m_sheWG;//攻强化等级
	short	m_sheWF;//防强化等级
	short	m_sheTG;//特攻强化等级
	short	m_sheTF;//特防强化等级
	short	m_sheSD;//速度强化等级
};

#endif //BATTLE_H