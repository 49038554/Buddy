#ifndef BATTLERESULT_H
#define BATTLERESULT_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//����
class BattleResult : public Buffer
{
public:
	//�������
	int					m_winnerId;		//ʤ��id
	int					m_loserId;		//����id

	//��Ӧ����

public:
	BattleResult();
	virtual ~BattleResult();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //BATTLERESULT_H
