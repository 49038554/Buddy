#ifndef CREATEPLAYER_H
#define CREATEPLAYER_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//����
class CreatePlayer : public Buffer
{
public:
	//�������

	//��Ӧ����

public:
	CreatePlayer();
	virtual ~CreatePlayer();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //CREATEPLAYER_H
