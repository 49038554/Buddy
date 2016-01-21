#ifndef BUDDYBOOK_H
#define BUDDYBOOK_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>
#include <string>
#include <map>

namespace data
{
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
		std::map<unsigned short, bool> skills;//���30��
		char		rare;	//������ 1~99
		char		tame;	//ѱ���� 1~99
		short		upMumber;//�������ţ�0�޽���̬
	}BUDDY;
}

namespace msg
{

//�͵�ͼ��
class BuddyBook : public Buffer
{
public:
	//��Ӧ����
	unsigned short					m_buddyVersion;
	std::vector<data::BUDDY>		m_buddys;//���20��

public:
	BuddyBook();
	virtual ~BuddyBook();
	bool Build();//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}


#endif //BUDDYBOOK_H
