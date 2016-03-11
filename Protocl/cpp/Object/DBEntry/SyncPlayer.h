#ifndef SYNCPLAYER_H
#define SYNCPLAYER_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//ͬ��������
class SyncPlayer : public Buffer
{
public:
	//�������
	data::PLAYER	m_player;

	//��Ӧ����

public:
	SyncPlayer();
	virtual ~SyncPlayer();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //SYNCCOIN_H
