#ifndef BUDDYS_H
#define BUDDYS_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

/*
����б�
���100��
*/
class Buddys : public Buffer
{
public:
	enum ListType
	{
		buddyList = 0,//����б�
	};
	typedef struct BUDDY
	{
		unsigned int	id;
		std::string		face;
		std::string		nickName;
	}BUDDY;
	//�������
	ListType			m_type;		//�б�����
	unsigned int		m_userId;	//�û�Id
	std::vector<BUDDY>	m_buddys;	//�б�

	//��Ӧ����

public:
	Buddys();
	virtual ~Buddys();
	bool Build();//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}

#endif //BUDDYS_H