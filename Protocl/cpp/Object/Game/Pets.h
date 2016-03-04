#ifndef PETS_H
#define PETS_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <map>
#include <vector>

namespace data
{
	typedef struct PET
	{
		int		id;
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
}

namespace msg
{

//��������
class Pets : public Buffer
{
public:
	//��Ӧ����
	std::vector<data::PET> m_pets;//���100ֻ
public:
	Pets();
	virtual ~Pets();
	bool Build();//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //PETS_H
