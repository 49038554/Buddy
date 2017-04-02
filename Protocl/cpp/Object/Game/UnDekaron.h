#ifndef UNDEKARON_H
#define UNDEKARON_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//��սȡ��
class Undekaron : public Buffer
{
public:
	//�������
	unsigned int			m_playerId;	// ����ս���id
	//��Ӧ����

public:
	Undekaron();
	virtual ~Undekaron();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //UNDEKARON_H
