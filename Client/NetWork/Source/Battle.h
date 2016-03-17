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
	bool PlayerAction(bool me, Battle::Action act, short objectId, bool skillPro, bool itemPro, int speed, unsigned char randSH);
	void PlayRound();

private:
	typedef struct ROUND
	{
		Action	me;
		int		meObjectId;
		bool	meSkillPro;
		bool	meItemPro;
		int		meSpeed;
		unsigned char	meRandSH;//217~255

		Action	she;
		int		sheObjectId;
		bool	sheSkillPro;
		bool	sheItemPro;
		int		sheSpeed;
		unsigned char	sheRandSH;//217~255

		std::vector<std::string> log;
	}ROUND;

	typedef struct WARRIOR
	{
		unsigned int			playerId;
		std::string				name;
		std::vector<data::PET>	pets;
		bool					isReady;
		data::PET				*pCurPet;
		data::BUDDY *pBuddy;
		float	wgCorrect;//�﹥����
		float	tgCorrect;//�ع�����
		float	sdCorrect;//������
		short	wg;//��ǿ���ȼ�
		short	wf;//��ǿ���ȼ�
		short	tg;//�ع�ǿ���ȼ�
		short	tf;//�ط�ǿ���ȼ�
		short	sd;//�ٶ�ǿ���ȼ�
		bool	race[18];//����ǿ��
		bool	nail[18];//�Ƿ��ж��ӣ�����idΪ�±꣬Ŀǰֻ�е����Զ���
		char	wall[2];//��أ�ʣ��غ��� -1����
		bool	beAttacked;//��������
		std::map<short, bool>	lookSkill;
		Action	act;
		int		objId;
		bool	skillPro;
		bool	itemPro;
		int		sdLevel;
		unsigned char	randSH;//217~255
		data::SKILL *pSkill;
		data::ITEM *pItem;
		data::TALENT *pTalent;
	}WARRIOR;

	void StepStart();
	void StepChange();
	void StepAttack();
	void StepEnd();
	bool ChangePet(Battle::WARRIOR &player);
	void EntryStage(Battle::WARRIOR &player, Battle::WARRIOR &enemy);
	void LeaveStage(Battle::WARRIOR &player);
	bool Hurt(Battle::WARRIOR &player, int HP);
	bool AttackOrder(Battle::WARRIOR &player, Battle::WARRIOR &enemy);
	bool XianShou(Battle::WARRIOR &player);
	int CalSpeed(Battle::WARRIOR &player, Battle::WARRIOR &enemy);
	int CalWG(Battle::WARRIOR &player, Battle::WARRIOR &enemy);
	int CalWF(Battle::WARRIOR &player, Battle::WARRIOR &enemy);
	int CalTG(Battle::WARRIOR &player, Battle::WARRIOR &enemy);
	int CalTF(Battle::WARRIOR &player, Battle::WARRIOR &enemy);
	int CalPower(Battle::WARRIOR &pAck, Battle::WARRIOR &pDef);
	bool Attack(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef);
	bool HelpSkill(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef);

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
	char					m_weather;//�������Զ�Ӧ
	char					m_weatherCount;//����ʣ��غ��� -1����
	char					m_foolSpace;//��ƭ�ռ�ʣ��غ��� -1����
};

#endif //BATTLE_H