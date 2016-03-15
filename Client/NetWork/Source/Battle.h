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
	char					m_weather;//�������Զ�Ӧ
	char					m_weatherCount;//����ʣ��غ��� -1����
	data::PET				*m_pMePet;
	short	m_meWG;//��ǿ���ȼ�
	short	m_meWF;//��ǿ���ȼ�
	short	m_meTG;//�ع�ǿ���ȼ�
	short	m_meTF;//�ط�ǿ���ȼ�
	short	m_meSD;//�ٶ�ǿ���ȼ�
	data::PET				*m_pShePet;
	short	m_sheWG;//��ǿ���ȼ�
	short	m_sheWF;//��ǿ���ȼ�
	short	m_sheTG;//�ع�ǿ���ȼ�
	short	m_sheTF;//�ط�ǿ���ȼ�
	short	m_sheSD;//�ٶ�ǿ���ȼ�
};

#endif //BATTLE_H