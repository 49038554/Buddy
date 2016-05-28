#ifndef ROUNDREADY_H
#define ROUNDREADY_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//�غ�׼��
class RoundReady : public Buffer
{
public:
	//�������
	int					m_battleId;		//ս��Id
	char				m_action;		//����
	int					m_gameObjId;		//����id
	data::RAND_PARAM	m_rp;			//�����

	//��Ӧ����

public:
	RoundReady();
	virtual ~RoundReady();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //ROUNDREADY_H
