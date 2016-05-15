#ifndef BATTLE_H
#define BATTLE_H

#include "common/BuddyData.h"

class Game;

class Battle
{
public:
	enum Action
	{
		unknow = 0,
		change = 1,
		useItem = 2,
		attack = 3,
	};

public:
	Battle();
	Battle& operator = ( Battle &copy );
	virtual ~Battle();
	bool Init(Game *game, int id, 
		const std::string &playerName, const std::string &enemyName, 
		unsigned int playerId, unsigned int enemyId, 
		std::vector<data::PET> &me, std::vector<data::PET> &she);
	bool IsAI();
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
	typedef struct WARRIOR
	{
		//////////////////////////////////////////////////////////////////////////
		//ֻ��ʼ��
		unsigned int			playerId;//���Id��0npc
		std::string				name;//���/npc�ǳ�
		std::vector<data::PET>	pets;//�����б�
		data::PET				*pCurPet;//��ǰ���ϰ͵�
		bool	lose;//ʧ��
		bool	isChanged;//�л���

		char	doomDesireRound;//����֮Ըʣ��غ�
		char	predictRound;//Ԥ֪δ��ʣ��غ�
		bool	nail[18];//�Ƿ��ж��ӣ�����idΪ�±꣬Ŀǰֻ�е����Զ���
		char	wall[2];//��أ�ʣ��غ��� -1����

		//////////////////////////////////////////////////////////////////////////
		//�غϿ�ʼ��������
		bool	isReady;//������ѡ��
		bool	isActioned;//�Ѿ��ж�
		bool	isEnd;//����ɻغϽ�������������ʱҲҪ����
		bool	attacked;//������
		int		outputHurt;//����˺�

		//////////////////////////////////////////////////////////////////////////
		//�����������
		data::BUDDY *pBuddy;
		data::ITEM *pItem;
		data::TALENT *pTalent;
		data::SKILL *pSkill;
		data::ITEM *pUseItem;
		Action	act;
		int		objId;
		RAND_PARAM rp;

		//////////////////////////////////////////////////////////////////////////
		//�ϳ���������
		float	wgCorrect;//�﹥����
		float	tgCorrect;//�ع�����
		float	sdCorrect;//������
		char	wg;//��ǿ���ȼ�
		char	wf;//��ǿ���ȼ�
		char	tg;//�ع�ǿ���ȼ�
		char	tf;//�ط�ǿ���ȼ�
		char	sd;//�ٶ�ǿ���ȼ�
		char	ct;//����ǿ���ȼ�
		char	banChangeRound;//��ֹ�����غ���,-1����
		short	lockSkill;//��������
		char	lockSkillTime;//�������ܻغ�����,-1����
		bool	guZhang;//���˹���
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
		char	hunLuan;//����ʣ��غ���
		bool	defensed;//���ع���
		bool	rest;//��Ϣ
		char	luanWu;//����ʣ��غ�
		bool	sunReady;//��������׼����
		char	fear;//����
		bool	race[18];//����ǿ��
		std::map<short, bool>	lookSkill;//ʥ��ʿ�����ļ���
		char	duRound;//�ж��غ�

		WARRIOR();
		void NewRound();//�غϿ�ʼǰ��Ҫ���õ�����
		void ChangePet();//�����͵�ʱ��Ҫ���õ�����
	}WARRIOR;
	Battle::WARRIOR* Player(bool me);
	const char* CheckReady(bool me, Battle::Action act, short objectId, Battle::RAND_PARAM &rp);
	bool Ready(bool me, Battle::Action act, short objectId, Battle::RAND_PARAM &rp);
	bool AIReady();
	const char* ChangePet(bool me, short petId);
	bool IsEnd();
	bool Log( std::vector<std::string> &log );
	bool Log( std::vector<std::vector<std::string> > &log );
	bool Save();//����id
	bool Load(Game *game, int bid);

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
		int	showPos;
	}ROUND;


	void WriteAction(mdk::File &logFile, Battle::Action act, int oId, Battle::RAND_PARAM &rp, std::vector<short> &petId);
	int Load(int bid, std::string &playerName, std::string &enemyName,
		unsigned int &playerId, unsigned int &enemyId, 
		std::vector<data::PET> &me, std::vector<data::PET> &she,
		std::vector<Battle::ROUND> &log);
	int ReadAction(mdk::File &logFile, Battle::Action &act, int &oId, Battle::RAND_PARAM &rp, std::vector<short> &petId);
	const char* SetPetInfo(Battle::WARRIOR &player, int petId);
	bool PlayRound();//��ɷ���true,�жϷ���false
	void End();//ս������
	void StepStart();
	bool StepChange();//��ɷ���true,�жϷ���false
	bool StepAttack();//��ɷ���true,�жϷ���false
	bool StepEnd();//��ɷ���true,�жϷ���false
	bool WaitPlayerCMD();//�ȴ����ָ��
	bool ChangePet(Battle::WARRIOR &player, int petId = 0);
	void EntryStage(Battle::WARRIOR &player, Battle::WARRIOR &enemy);
	void LeaveStage(Battle::WARRIOR &player);
	int Hurt(Battle::WARRIOR &player, int HP, bool unFaint = false);//����ʵ���˺�
	bool AttackOrder(Battle::WARRIOR &player, Battle::WARRIOR &enemy);
	bool IsUnwait(Battle::WARRIOR &player);
	bool IsWait(Battle::WARRIOR &player);
	int CalSpeed(Battle::WARRIOR &player, Battle::WARRIOR &enemy);
	bool CalHitRate(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef);
	bool CriticalHit(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef);
	int CalWG(Battle::WARRIOR &player, bool ct, Battle::WARRIOR &enemy);
	int CalWF(Battle::WARRIOR &player, bool ct, Battle::WARRIOR &enemy);
	int CalTG(Battle::WARRIOR &player, bool ct, Battle::WARRIOR &enemy);
	int CalTF(Battle::WARRIOR &player, bool ct, Battle::WARRIOR &enemy);
	int CalPower(Battle::WARRIOR &pAck, bool ct, Battle::WARRIOR &pDef);
	bool PetAction(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef);//true�а͵�ս�ܣ�falseû�а͵�ս��
	bool UseSkill(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef);//true�а͵�ս�ܣ�falseû�а͵�ս��
	bool ImmuneSkill(Battle::WARRIOR &player, Battle::WARRIOR &enemy);//���߼���
	bool HelpSkill(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef);//��������,���Ǹ������ܷ���false
	bool InterfereSkill(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef);//���ż���
	bool ActionAble(Battle::WARRIOR &player);
	bool LaunchState(Battle::WARRIOR &player);//�����ж�״̬����
	bool Medication(Battle::WARRIOR &player);//������ҩ
	void ChangeSkill(Battle::WARRIOR &player);
	bool BanSkill(Battle::WARRIOR &player, Battle::WARRIOR &enemy);
	bool Confusion(Battle::WARRIOR &player);
	bool AttackEffect(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef);//������Ч
	bool AttackCost(Battle::WARRIOR &playerAck, Battle::WARRIOR &playerDef);//��������
	bool ForcedLeave(Battle::WARRIOR &player);
	bool ImmuneState(Battle::WARRIOR &player);//�����쳣״̬
	bool Faint(Battle::WARRIOR &player);
	void PlayerEnd(Battle::WARRIOR &player, Battle::WARRIOR &enemy);

	//AI
	enum TestResult
	{
		kill0 = 0,//�Է��ң�������
		kill30 = 1,//�Է��ң����˵���30%
		kill60 = 2,//�Է��ң����˵���60%
		kill99 = 3,//�Է��ң����˵���100%
		nohurt = 4,//�����ˣ�����ٷֱȸ��ڶԷ�20%
		hurt30 = 5,//���˵���30%������ٷֱȸ��ڶԷ�20%
		hurt60  = 6,//���˵���60%������ٷֱȸ��ڶԷ�20%
		ack = 7,//���˵���30%������
		def = 8,//���˵���30%������
		dying  = 9,//���˵���80%������ٷֱȸ��ڶԷ�20%
		badSkill = 10,//������ڶԷ�,��������ӽ�,�������˴ﵽ80%
		badDef = 11,//������ڶԷ�,��������ӽ�,�������˴ﵽ80%
		dead100 = 12,//ͬ��
		dead70 = 13,//���ˣ��������70%
		dead50 = 14,//���ˣ��������50%
		dead30 = 16,//���ˣ��������30%
		dead = 16,//���ˣ��������30%
		refuse = 17,//�ܾ�
	};
	Battle::TestResult TestSkill( int skillId, RAND_PARAM &rp );
	Battle::TestResult TestChange( int petPos, RAND_PARAM &rp );
	Battle::TestResult ActionLevel( Battle::WARRIOR &player, Battle::WARRIOR &npc );


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