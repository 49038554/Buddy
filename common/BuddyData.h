#ifndef BUDDY_DATA_H
#define BUDDY_DATA_H

#include <string>
#include <vector>
#include <map>
#include "mdk/File.h"

namespace Race
{
	enum Race
	{
		unknow = 0,
		pu = 1	,//��
		huo = 2	,//��
		shui = 3	,//ˮ
		dian = 4	,//��
		bing = 5	,//��
		gui = 6	,//��
		e = 7	,//��
		chao = 8	,//��
		longx = 9	,//��
		fei = 10	,//��
		chong = 11	,//��
		cao = 12	,//��
		di = 13	,//��
		yan = 14	,//��
		gang = 15	,//��
		dou = 16	,//��
		du = 17	,//��
	};
}

namespace data
{

	//��Ч
	typedef struct EFFECT
	{
		unsigned char id;	//id
		char step;			//����ʱ��
		char probability;	//����
		char agent;			//���ö���
	}EFFECT;

	//��Ʒ
	typedef struct ITEM
	{
		short			id;
		std::string		name;
		int				coin;//��ֵ
		std::vector<EFFECT>	effects;//��Ч,���5��
		std::string		descript;//���128byte
	}ITEM;

	//����
	typedef struct TALENT
	{
		short			id;
		std::string		name;
		std::vector<EFFECT>	effects;//��Ч,���5��
		std::string		descript;//���128byte
	}TALENT;

	//����
	typedef struct SKILL
	{
		short				id;
		std::string			name;
		char				race;//����
		short				power;//����0~300
		char				type;//1����2�仯��3����
		char				hitRate;//������30~101, 101���м�
		std::vector<EFFECT>	effects;//��Ч,���5��
		bool				isMapSkill;//�ǵ�ͼ����
		std::string			descript;//���128byte
	}SKILL;

	//�͵���
	typedef struct BUDDY
	{
		short		number;//ȫ�����
		std::string	name;//����
		std::string	descript;//����,���60byte
		char		race1;//����1
		char		race2;//����2
		char		talent1;//����1
		char		talent2;//����2
		char		talent3;//������
		short		itemId;//ר����Ʒ��0û��ר����Ʒ
		short		hitPoint;//Ѫ
		short		physicalA;//��
		short		physicalD;//��
		short		specialA;//�ع�
		short		specialD;//�ط�
		short		speed;//�ٶ�
		std::map<unsigned short, bool> skills;//���80��
		char		rare;	//������ 1~99
		char		tame;	//ѱ���� 1~99
		std::vector<short>		upBuddys;//�����������10�ֽ���
	}BUDDY;

	//�͵��޷ֲ�
	typedef struct BUDDY_MAP
	{
		int					id;
		std::vector<short>	buddys;//���100
		char				shape;//��״��0Բ�Σ�1����
		double				x;//latitude
		double				y;//longitude
		int					radius;
		double				right;//latitude
		double				bottom;//longitude
		int					city;//����
		bool				spot;//����
	}BUDDY_MAP;

	typedef struct PLAYER
	{
		unsigned int					playerId;
		std::string						nick;
		int								coin;
		char							petCount;
		short							pet[6];
		time_t							lastLuckTime;
		short							luckCoin;
		bool							synced;
	}PLAYER;

	typedef struct PET
	{
		int		id;//����Id
		short	number;//�͵ϱ��
		char	talent;//����
		char	nature;//�Ը�
		short	skill1;//����1
		short	skill2;//����2
		short	skill3;//����3
		short	skill4;//����4
		short	itemId;//��Ʒid
		short	HP;//Ѫ
		short	WG;//��
		short	WF;//��
		short	TG;//�ع�
		short	TF;//�ط�
		short	SD;//�ٶ�
		char	SDHealthy;//�ٶ���������
		unsigned char	HPMuscle;//Ѫ��������
		unsigned char	WGMuscle;//����������
		unsigned char	WFMuscle;//����������
		unsigned char	TGMuscle;//�ع���������
		unsigned char	TFMuscle;//�ط���������
		unsigned char	SDMuscle;//�ٶȺ�������
		std::vector<char>	race;//17�����ԵĶ�ħ��ʵ�Ƿ�Թ�

		bool	synced;//��ͬ��
		std::string	nick;//�ǳ�
		char	state;//״̬
		short	curHP;//��ǰѪ��
		char	sleepRound;//����ʣ��غ�
		char	frozenRound;//����ʣ��غ�
	}PET;

	typedef struct PLAYER_ITEM
	{
		short			id;
		int				count;

		bool			synced;
	}PLAYER_ITEM;

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
		char			fouceChange;//ǿ�ƻ��ϵ�?ֻPet�������1~5
	}RAND_PARAM;
}

data::ITEM* Item(short id, std::vector<data::ITEM> &items);
data::ITEM* Item(const std::string name, std::vector<data::ITEM> &items);
data::TALENT* Talent(short id, std::vector<data::TALENT> &talents);
data::TALENT* Talent(const std::string name, std::vector<data::TALENT> &talents);
data::SKILL* Skill(short id, std::vector<data::SKILL> &skills);
data::SKILL* Skill(const std::string name, std::vector<data::SKILL> &skills);
data::BUDDY* Buddy(short number, std::vector<data::BUDDY> &buddys);
data::BUDDY* Buddy(const std::string name, std::vector<data::BUDDY> &buddys);
data::BUDDY_MAP* BuddyMap(int id, std::vector<data::BUDDY_MAP> &BuddyMaps);

data::PLAYER_ITEM* PlayerItem(short id, std::vector<data::PLAYER_ITEM> &items);
data::PET* Pet(int id, std::vector<data::PET> &pets);

int GetNature(const std::string &add, const std::string &dec);
double GetNatureCal(int nature, const std::string &attr);
bool StrSame(const std::string &str1, const std::string &str2);
char* StateDes( char state );
bool SavePets(mdk::File &db, std::vector<data::PET> &pets, std::vector<data::BUDDY> &buddyBook);
int LoadPets(mdk::File &db, std::vector<data::PET> &pets, std::vector<data::BUDDY> &buddyBook);

#endif //BUDDY_DATA_H