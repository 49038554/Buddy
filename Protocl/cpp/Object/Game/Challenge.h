#ifndef CHALLENGE_H
#define CHALLENGE_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//Ӧս
class Challenge : public Buffer
{
public:
	//�������
	int						m_battleId;	// ս��Id ��Dekaron����
	unsigned int			m_playerId;	// ��ս���id
	bool					m_accepted;	// ������ս
	std::string				m_dNick;	// ��ս���ǳ�
	std::vector<data::PET>	m_dPet;		// ��ս�߳���
	std::string				m_cNick;	// Ӧս���ǳ�
	std::vector<data::PET>	m_cPet;		// Ӧս�߳���
	//��Ӧ����

public:
	Challenge();
	virtual ~Challenge();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //CHALLENGE_H
