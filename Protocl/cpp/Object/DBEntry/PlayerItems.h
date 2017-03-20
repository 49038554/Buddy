#ifndef PLAYERITEMS_H
#define PLAYERITEMS_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//�����Ʒ
class PlayerItems : public Buffer
{
public:
	//��Ӧ����
	std::vector<data::PLAYER_ITEM>	m_items;//���200��

public:
	PlayerItems();
	virtual ~PlayerItems();
	bool Build( bool isResult = false );//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //PLAYERITEMS_H
