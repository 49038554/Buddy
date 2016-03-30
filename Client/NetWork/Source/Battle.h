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
		char miss;//���������
		char sePro;//������Ч�����
		char iePro;//��Ʒ��Ч�����
		char tePro;//������Ч�����
		char luanWu;//����غ���
		char sleepRound;//˯�߻غ������
		char frozenRound;//�����غ������
		char dian;//��������
		bool luan;//���������
		unsigned char	hurt;//�˺������217~255
		unsigned char	speed;//�ٶ������
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
		float	wgCorrect;//�﹥����
		float	tgCorrect;//�ع�����
		float	sdCorrect;//������
		char	wg;//��ǿ���ȼ�
		char	wf;//��ǿ���ȼ�
		char	tg;//�ع�ǿ���ȼ�
		char	tf;//�ط�ǿ���ȼ�
		char	sd;//�ٶ�ǿ���ȼ�
		char	ct;//����ǿ���ȼ�
		bool	changePetAble;//���ܻ���
		short	lockSkill;//��������
		char	lockSkillTime;//�������ܻغ�����,-1����
		bool	smell;//���

		short	recvHP;//�ظ�HP
		bool	seed;//����
		char	haQian;//���˹�Ƿ
		char	mieGe;//����������
		short	tiShen;//��������
		bool	xunXing;//��Ѱ��
		char	tiaoDou;//�����غ�
		bool	ban;//�з�ӡ
		bool	tongGui;//��ͬ��
		char	sleepRound;//����ʣ��غ�
		char	frozenRound;//����ʣ��غ�
		char	hunLuan;//����ʣ��غ���
		bool	defensed;//���ع���
		bool	rest;//��Ϣ
		char	luanWu;//����ʣ��غ�
		bool	sunReady;//��������׼����
		char	doomDesireRound;//����֮Ըʣ��غ�
		char	predictRound;//Ԥ֪δ��ʣ��غ�
		int		outputHurt;//����˺�
		bool	race[18];//����ǿ��
		bool	nail[18];//�Ƿ��ж��ӣ�����idΪ�±꣬Ŀǰֻ�е����Զ���
		char	wall[2];//��أ�ʣ��غ��� -1����
		std::map<short, bool>	lookSkill;

		char	fear;//����
		bool	isChanged;//�л���
		bool	isActioned;//�Ѿ��ж�
		bool	isEnd;//�غϽ���
		bool	lose;//ʧ��
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
	bool PlayRound();//��ɷ���true,�жϷ���false
	void StepStart();
	bool StepChange();//��ɷ���true,�жϷ���false
	bool StepAttack();//��ɷ���true,�жϷ���false
	bool StepEnd();//��ɷ���true,�жϷ���false
	bool WaitPlayerCMD();//�ȴ����ָ��
	bool ChangePet(Battle::WARRIOR &player, int petId = 0);
	void EntryStage(Battle::WARRIOR &player, Battle::WARRIOR &enemy);
	void LeaveStage(Battle::WARRIOR &player);
	bool Hurt(Battle::WARRIOR &player, int HP, bool unFaint = false);//true�а͵ϵ��£�falseû�а͵ϵ���
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
	bool PetAction(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef);//true�а͵�ս�ܣ�falseû�а͵�ս��
	bool UseSkill(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef);//true�а͵�ս�ܣ�falseû�а͵�ս��
	bool HelpSkill(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef);//��������,���Ǹ������ܷ���false
	bool InterfereSkill(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef);//���ż���
	bool ActionAble(Battle::WARRIOR &player);
	bool LaunchState(Battle::WARRIOR &player);
	bool Medication(Battle::WARRIOR &player);
	void ChangeSkill(Battle::WARRIOR &player);
	bool BanSkill(Battle::WARRIOR &player, Battle::WARRIOR &enemy);
	bool Confusion(Battle::WARRIOR &player);
	bool AttackEffect(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef);//������Ч
	bool AttackCost(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef);//��������
	bool ForcedLeave(Battle::WARRIOR &player);
	bool ImmuneState(Battle::WARRIOR &player);//�����쳣״̬
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
	char					m_weather;//�������Զ�Ӧ
	char					m_weatherCount;//����ʣ��غ��� -1����
	char					m_foolSpace;//��ƭ�ռ�ʣ��غ��� -1����
// 	ժ����

};

#endif //BATTLE_H