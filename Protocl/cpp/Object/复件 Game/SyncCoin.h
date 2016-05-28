#ifndef SYNCCOIN_H
#define SYNCCOIN_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//ͬ��������
class SyncCoin : public Buffer
{
public:
	//�������
	int			m_count;	//����

	//��Ӧ����

public:
	SyncCoin();
	virtual ~SyncCoin();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //SYNCCOIN_H
