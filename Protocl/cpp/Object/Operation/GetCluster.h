#pragma once
#include "../../Buffer.h"

namespace msg
{

//ȡ��Ⱥ��Ϣ
class GetCluster : public Buffer
{
public:
	//�������
	Moudle::Moudle	m_moudle;//Ҫȡ�ķ��������
	//��Ӧ����

public:
	GetCluster();
	virtual ~GetCluster();
	bool Build( bool isResult = false );//���챨�ģ�����
	bool Parse();//�������ģ�����

private:
	unsigned char m_buffer[MAX_MSG_SIZE];
};


}
