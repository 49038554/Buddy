#ifndef DEKARON_H
#define DEKARON_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//��ս
class Dekaron : public Buffer
{
public:
	//�������
	int						m_battleId;	// ս��Id Game������ͳһ����
	unsigned int			m_playerId;	// ����ս���id
	std::string				m_nick;		// ��ս���ǳ�
	std::vector<data::PET>	m_pet;		// ��ս�߳���
	//��Ӧ����

public:
	Dekaron();
	virtual ~Dekaron();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //DEKARON_H
