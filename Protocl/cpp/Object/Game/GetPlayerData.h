#ifndef GETPLAYERDATA_H
#define GETPLAYERDATA_H

#include "../../Buffer.h"
#include "../../Protocl.h"
#include <vector>

namespace msg
{

//ȡ�������
class GetPlayerData : public Buffer
{
public:
	//�������
	//��Ӧ����

public:
	GetPlayerData();
	virtual ~GetPlayerData();
	bool Build(bool isResult = false);//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};

}


#endif //GETPLAYERDATA_H
