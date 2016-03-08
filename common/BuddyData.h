#ifndef BUDDY_DATA_H
#define BUDDY_DATA_H

#include <string>
#include <vector>
#include <map>

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
		std::string		descript;//���60byte
	}ITEM;

	//����
	typedef struct TALENT
	{
		short			id;
		std::string		name;
		std::vector<EFFECT>	effects;//��Ч,���5��
		std::string		descript;//���60byte
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
		std::string			descript;//���60byte
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

	typedef struct PET
	{
		int		id;
		bool	sync;
		short	number;
		char	talent;//����
		char	nature;//�Ը�
		short	skill1;
		short	skill2;
		short	skill3;
		short	skill4;
		short	itemId;
		short	HP;//Ѫ
		short	WG;//��
		short	WF;//��
		short	TG;//�ع�
		short	TF;//�ط�
		short	SD;//�ٶ�
		char	HPHealthy;//Ѫ��������
		char	WGHealthy;//����������
		char	WFHealthy;//����������
		char	TGHealthy;//�ع���������
		char	TFHealthy;//�ط���������
		char	SDHealthy;//�ٶ���������
		unsigned char	HPMuscle;//Ѫ��������
		unsigned char	WGMuscle;//����������
		unsigned char	WFMuscle;//����������
		unsigned char	TGMuscle;//�ع���������
		unsigned char	TFMuscle;//�ط���������
		unsigned char	SDMuscle;//�ٶȺ�������
		std::vector<char>	race;//17�����ԵĶ�ħ��ʵ�Ƿ�Թ�
	}PET;

	typedef struct PLAYER_ITEM
	{
		short			id;
		int				count;
	}PLAYER_ITEM;

}

#endif //BUDDY_DATA_H