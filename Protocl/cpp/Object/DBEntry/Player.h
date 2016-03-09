#ifndef PLAYER_H
#define PLAYER_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//�������
class Player : public Buffer
{
public:
	//��Ӧ����
	int		m_coin;

public:
	Player();
	virtual ~Player();
	bool Build();//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //PLAYER_H
