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
	short	m_meSDCorrect;//������
	short	m_meWG;//��ǿ���ȼ�
	short	m_meWF;//��ǿ���ȼ�
	short	m_meTG;//�ع�ǿ���ȼ�
	short	m_meTF;//�ط�ǿ���ȼ�
	short	m_meSD;//�ٶ�ǿ���ȼ�
	bool	m_meRace[18];//����ǿ��
	data::PET				*m_pShePet;
	short	m_sheSDCorrect;//������
	short	m_sheWG;//��ǿ���ȼ�
	short	m_sheWF;//��ǿ���ȼ�
	short	m_sheTG;//�ع�ǿ���ȼ�
	short	m_sheTF;//�ط�ǿ���ȼ�
	short	m_sheSD;//�ٶ�ǿ���ȼ�
	bool	m_sheRace[18];//����ǿ��

	char					m_weather;//�������Զ�Ӧ
	char					m_weatherCount;//����ʣ��غ��� -1����
	bool					m_meNail[18];//�Ƿ��ж��ӣ�����idΪ�±꣬Ŀǰֻ�е����Զ���
	bool					m_sheNail[18];//�Ƿ��ж��ӣ�����idΪ�±꣬Ŀǰֻ�е����Զ���
	char					m_meWall[2];//���
	char					m_sheWall[2];//���
};

#endif //BATTLE_H