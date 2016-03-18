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
		int i;
	}RAND_PARAM;
	bool PlayerRand(bool me, RAND_PARAM &rp, Battle::Action act, short objectId);
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
		float	wgCorrect;//�﹥����
		float	tgCorrect;//�ع�����
		float	sdCorrect;//������
		char	wg;//��ǿ���ȼ�
		char	wf;//��ǿ���ȼ�
		char	tg;//�ع�ǿ���ȼ�
		char	tf;//�ط�ǿ���ȼ�
		char	sd;//�ٶ�ǿ���ȼ�
		char	ct;//����ǿ���ȼ�
		char	miss;//����ǿ��
		char	unMiss;//����ǿ��
		bool	changePetAble;//���ܻ���
		short	lockSkill;//��������
		char	lockSkillTime;//�������ܻغ�����
		bool	smell;//���

		short	recvHP;//�ظ�HP
		char	sleep;//ʣ��غ�
		bool	seed;//����
		char	haQian;//���˹�Ƿ
		char	mieGe;//����������
		short	tiShen;//��������
		bool	xunXing;//��Ѱ��
		char	tiaoDou;//�����غ�
		bool	ban;//�з�ӡ
		bool	tongGui;//��ͬ��
		char	hunLuan;//����ʣ��غ���

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
		data::BUDDY *pBuddy;
		data::ITEM *pItem;
		data::TALENT *pTalent;
		data::SKILL *pSkill;
	}WARRIOR;

	void StepStart();
	void StepChange();
	void StepAttack();
	void StepEnd();
	bool ChangePet(Battle::WARRIOR &player, int petId = 0);
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